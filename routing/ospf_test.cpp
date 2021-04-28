#include <iostream>
#include <vector>
#include <string>
#include <gtest/gtest.h>
#include "Router.h"
#include "LSDB.h"
#include "LSA.h"
#include "native_ospf/router_json_parser.h"
#include "ospf.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

TEST(InitTest, InitTest)
{
	// Used to check if GTest is configured properly
	EXPECT_EQ(1, 1);
}

TEST(RapidJsonTest, DetectingParsingErrors)
{
	rapidjson::Document document;
	
	const char good_json[] = "{\"array\":[-1,0,1.5]}";
	ASSERT_FALSE(document.Parse(good_json).HasParseError());

	const char bad_json[] = "{\"array\":[-1,0,1.5,]}";
	ASSERT_TRUE(document.Parse(bad_json).HasParseError());
}

TEST(RapidJsonTest, ParsingArrays)
{
	const char json[] = "{\"array\":[-1,0,1.5]}";

	rapidjson::Document document;

	// ParseInsitu expects dynamically allocated char*
	char buffer[sizeof(json)];
    memcpy(buffer, json, sizeof(json));

	// parse json
	document.ParseInsitu(buffer);

    ASSERT_FALSE(document.HasParseError());
	ASSERT_TRUE(document.IsObject());
	ASSERT_TRUE(document.HasMember("array"));
	
	const rapidjson::Value& array = document["array"];
	ASSERT_TRUE(array.IsArray());
}

TEST(RapidJsonTest, ParsingStrings)
{
	const char json[] = "{\"key0\":\"value0\",\"key1\":\"value1\"}";

	// ParseInsitu expects dyanmically allocated char*
	char buffer[sizeof(json)];
	memcpy(buffer, json, sizeof(json));

	// parse json
	rapidjson::Document document;
	document.ParseInsitu(buffer);

	ASSERT_FALSE(document.HasParseError());
	ASSERT_TRUE(document.IsObject());
	ASSERT_TRUE(document.HasMember("key0"));
	ASSERT_STREQ(document["key0"].GetString(), "value0");
	ASSERT_TRUE(document.HasMember("key1"));
	ASSERT_STREQ(document["key1"].GetString(), "value1");
}

TEST(RapidJsonTest, ParsingNetworkTopology)
{
	std::vector< std::vector<int> > network_topology;
	std::string json = "{\"networkTopology\":[[0,1,5],[1,0,5],[0,2,11],[2,0,11],[1,2,1],[2,1,1]]}";

	network_topology = parseNetworkTopology(json);

	ASSERT_EQ(network_topology.at(0).at(0), 0);
	ASSERT_EQ(network_topology.at(0).at(1), 1);
	ASSERT_EQ(network_topology.at(0).at(2), 5);
	ASSERT_EQ(network_topology.at(1).at(0), 1);
	ASSERT_EQ(network_topology.at(1).at(1), 0);
	ASSERT_EQ(network_topology.at(1).at(2), 5);
	ASSERT_EQ(network_topology.at(2).at(0), 0);
	ASSERT_EQ(network_topology.at(2).at(1), 2);
	ASSERT_EQ(network_topology.at(2).at(2), 11);
	ASSERT_EQ(network_topology.at(3).at(0), 2);
	ASSERT_EQ(network_topology.at(3).at(1), 0);
	ASSERT_EQ(network_topology.at(3).at(2), 11);
	ASSERT_EQ(network_topology.at(4).at(0), 1);
	ASSERT_EQ(network_topology.at(4).at(1), 2);
	ASSERT_EQ(network_topology.at(4).at(2), 1);
	ASSERT_EQ(network_topology.at(5).at(0), 2);
	ASSERT_EQ(network_topology.at(5).at(1), 1);
	ASSERT_EQ(network_topology.at(5).at(2), 1);
}

TEST(RapidJsonTest, ParsingRouterIDs)
{
	std::vector< std::vector<int> > network_topology;
	std::vector<int> routerIDs;
	std::string json = "{\"networkTopology\":[[0,1,5],[1,0,5],[0,2,11],[2,0,11],[1,2,1],[2,1,1]]}";

	network_topology = parseNetworkTopology(json);
	routerIDs = parseRouterIDs(network_topology);

	ASSERT_EQ(routerIDs.size(), 3);
	ASSERT_EQ(std::count(routerIDs.begin(), routerIDs.end(), 0), 1);
	ASSERT_EQ(std::count(routerIDs.begin(), routerIDs.end(), 1), 1);
	ASSERT_EQ(std::count(routerIDs.begin(), routerIDs.end(), 2), 1);
}

TEST(RapidJsonTest, ComposingForwardingTable)
{
	// forwarding table for nodes 0,1,2 in the following network topology:
	//			[1]
	//		 5 /   \ 1
	//		[0]-----[2]
	//			 11
	ForwardingTable firstForwardingTable = {
		{0, 0, 0},
		{1, 0, 5},
		{2, 1, 6}
	};

	ForwardingTable secondForwardingTable = {
		{0, 0, 5},
		{1, 1, 0},
		{2, 2, 1}
	};

	ForwardingTable thirdForwardingTable = {
		{0, 1, 6},
		{1, 1, 1},
		{2, 2, 0}
	};

	std::vector<ForwardingTable> forwardingTables = {
		firstForwardingTable,
		secondForwardingTable,
		thirdForwardingTable
	};

	std::vector<int> routerIDs = {0, 1, 2};
	std::string composed_json = composeForwardingTable(forwardingTables, routerIDs);
	std::string correct_json = "{\"forwardingTable\":{\"0\":[[0,0,0],[1,0,5],[2,1,6]],\"1\":[[0,0,5],[1,1,0],[2,2,1]],\"2\":[[0,1,6],[1,1,1],[2,2,0]]}}";
	
	ASSERT_EQ(composed_json, correct_json);
}

TEST(RapidJsonTest, ComposeLowestCostPathsTable)
{
	// lowest cost table for the following network topology:
	//			[1]
	//		 5 /   \ 1
	//		[0]-----[2]
	//			 11
	std::vector< std::vector<std::string> > leastCostPathsTable(3);
	leastCostPathsTable.at(0) = {"0,0", "0,1", "0,1,2"};
	leastCostPathsTable.at(1) = {"1,0", "1,1", "1,2"};
	leastCostPathsTable.at(2) = {"2,1,0", "2,1", "2,2"};

	std::string composed_json = composeLeastCostPathsTable(leastCostPathsTable);
	std::string correct_json = "{\"lowestCostPaths\":{\"0\":{\"0\":\"0,0\",\"1\":\"0,1\",\"2\":\"0,1,2\"},\"1\":{\"0\":\"1,0\",\"1\":\"1,1\",\"2\":\"1,2\"},\"2\":{\"0\":\"2,1,0\",\"1\":\"2,1\",\"2\":\"2,2\"}}}";
	
	ASSERT_EQ(composed_json, correct_json);
}

TEST(LSDBTest, AddRouterLSATest)
{
	// define some nodes to make debugging easier to read
	enum node
	{
		u = 0,
		v = 1,
		w = 2,
		x = 3
	};

	RouterLSA lsa = { Link(u, v), INIT_SEQ_NUM, 1};
	LSDB lsdb;
	ASSERT_EQ(lsdb.size(), 0);
	
	// duplicate LSAs won't be added to database
	lsdb.add_router_lsa(lsa);
	ASSERT_EQ(lsdb.size(), 1);
	lsdb.add_router_lsa(lsa);
	ASSERT_EQ(lsdb.size(), 1);

	// the same sequence number can be used for different links
	lsa = { Link(v, w), INIT_SEQ_NUM, 1};
	lsdb.add_router_lsa(lsa);
	ASSERT_EQ(lsdb.size(), 2);

	// LSAs with higher sequence number are prioritized
	lsa = { Link(v, w), INIT_SEQ_NUM+1, 1};
	lsdb.add_router_lsa(lsa);
	ASSERT_EQ(lsdb.size(), 2);
}

TEST(LSDBTest, RouterLSAExists)
{
	// define some nodes to make debugging easier to read
	enum node
	{
		u = 0,
		v = 1,
		w = 2,
		x = 3
	};

	RouterLSA lsa = { Link(u, v), INIT_SEQ_NUM, 1};
	LSDB lsdb;
	
	// false is always returned when database empty
	ASSERT_FALSE(lsdb.router_lsa_exists(lsa));

	lsdb.add_router_lsa(lsa);

	// true is returned when LSA is in database
	ASSERT_TRUE(lsdb.router_lsa_exists(lsa));

	lsa = { Link(u, v), INIT_SEQ_NUM+1, 1 };

	// false is returned when LSAs differ in seq #
	ASSERT_FALSE(lsdb.router_lsa_exists(lsa));
}

TEST(LSDBTest, AdvertiseDatabase)
{
	// define some nodes to make debugging easier to read
	enum node
	{
		u = 0,
		v = 1,
		w = 2,
		x = 3
	};

	RouterLSA lsa1 = { Link(u, v), INIT_SEQ_NUM, 1};
	RouterLSA lsa2 = { Link(v, w), INIT_SEQ_NUM, 4};
	RouterLSA lsa3 = { Link(w, u), INIT_SEQ_NUM, 2};
	LSDB lsdb;

	std::vector<RouterLSA> retVal;

	// empty database returns empty vector
	retVal = lsdb.advertise_database();
	ASSERT_EQ(retVal.size(), 0);

	lsdb.add_router_lsa(lsa1);

	// database with one element returns size 1
	retVal = lsdb.advertise_database();
	ASSERT_EQ(retVal.size(), 1);

	lsdb.add_router_lsa(lsa2);

	// database with two elements returns size 2, in order
	retVal = lsdb.advertise_database();
	ASSERT_EQ(retVal.size(), 2);
	ASSERT_LT(retVal.at(0).LINK, retVal.at(1).LINK);

	lsdb.add_router_lsa(lsa3);

	// database with three elements returns size 3, in order
	retVal = lsdb.advertise_database();
	ASSERT_EQ(retVal.size(), 3);
	ASSERT_LT(retVal.at(0).LINK, retVal.at(1).LINK);
	ASSERT_LT(retVal.at(1).LINK, retVal.at(2).LINK);
}

TEST(LSDBTest, ClearDatabase)
{
	// define some nodes to make debugging easier to read
	enum node
	{
		u = 0,
		v = 1,
		w = 2,
		x = 3
	};

	RouterLSA lsa1 = { Link(u, v), INIT_SEQ_NUM, 1};
	RouterLSA lsa2 = { Link(v, w), INIT_SEQ_NUM, 4};
	RouterLSA lsa3 = { Link(w, u), INIT_SEQ_NUM, 2};
	LSDB lsdb;

	// clearing an empty database does nothing
	ASSERT_EQ(lsdb.size(), 0);
	lsdb.clear();
	ASSERT_EQ(lsdb.size(), 0);

	// clearing a non-empty database makes it empty again
	lsdb.add_router_lsa(lsa1);
	lsdb.add_router_lsa(lsa2);
	lsdb.add_router_lsa(lsa3);
	ASSERT_EQ(lsdb.size(), 3);
	lsdb.clear();
	ASSERT_EQ(lsdb.size(), 0);
}

TEST(LSDBTest, UpdateDatabase)
{
	// define some nodes to make debugging easier to read
	enum node
	{
		u = 0,
		v = 1,
		w = 2,
		x = 3
	};

	RouterLSA lsa1 = { Link(u, v), INIT_SEQ_NUM+1, 1};
	RouterLSA lsa2 = { Link(v, w), INIT_SEQ_NUM+1, 2};
	RouterLSA lsa3 = { Link(w, u), INIT_SEQ_NUM+1, 3};
	LSDB advertising_lsdb;

	// construct advertising router LSDB
	advertising_lsdb.add_router_lsa(lsa1);
	advertising_lsdb.add_router_lsa(lsa2);
	advertising_lsdb.add_router_lsa(lsa3);

	// empty database updates with all received links
	LSDB receiving_lsdb;

	std::vector<RouterLSA> lsa_list = advertising_lsdb.advertise_database();
	receiving_lsdb.update_database(lsa_list);
	lsa_list = receiving_lsdb.advertise_database();

	ASSERT_EQ(lsa_list.at(0).LINK_COST, 1);
	ASSERT_EQ(lsa_list.at(1).LINK_COST, 2);
	ASSERT_EQ(lsa_list.at(2).LINK_COST, 3);

	advertising_lsdb.clear();
	lsa_list.clear();

	// database won't overwrite newer LSAs updates with older LSAs

	lsa1 = { Link(u, v), INIT_SEQ_NUM, 1};
	lsa2 = { Link(v, w), INIT_SEQ_NUM, 2};
	lsa3 = { Link(w, u), INIT_SEQ_NUM, 3};

	advertising_lsdb.add_router_lsa(lsa1);
	advertising_lsdb.add_router_lsa(lsa2);	
	advertising_lsdb.add_router_lsa(lsa3);

	lsa_list = advertising_lsdb.advertise_database();
	receiving_lsdb.update_database(lsa_list);
	lsa_list = receiving_lsdb.advertise_database();

	ASSERT_EQ(lsa_list.at(0).SEQ_NUM, INIT_SEQ_NUM+1);
	ASSERT_EQ(lsa_list.at(1).SEQ_NUM, INIT_SEQ_NUM+1);
	ASSERT_EQ(lsa_list.at(2).SEQ_NUM, INIT_SEQ_NUM+1);
}

TEST(RouterTest, RouterID)
{
	// initialize Router
	Router router(99);
	ASSERT_EQ(router.getID(), 99);
}

TEST(LSDBTest, get_all_destinations) {
	enum node
	{
		u = 0,
		v = 1,
		w = 2,
		x = 3
	};

	std::vector<int> destinations;
	std::vector<int> answers;
	LSDB lsdb;

	// Grabs the correct 2 routers
	RouterLSA lsa = { Link(u, v), INIT_SEQ_NUM, 1};
	lsdb.add_router_lsa(lsa);
	destinations = lsdb.get_all_destinations();
	answers = {1, 0};
	ASSERT_EQ(destinations, answers);

	// Grabs correct number with routers in multiple links
	lsa = { Link(u, w), INIT_SEQ_NUM, 1};
	lsdb.add_router_lsa(lsa);
	destinations = lsdb.get_all_destinations();
	answers = {1, 0, 2};
	ASSERT_EQ(destinations, answers);

}

TEST(LSDBTest, find_connections_with) {
		enum node
	{
		u = 0,
		v = 1,
		w = 2,
		x = 3
	};

	std::vector<std::pair<int, int>> destinations;
	std::vector<std::pair<int, int>> answers;
	LSDB lsdb;

	// Grabs the 1 link
	RouterLSA lsa = { Link(u, v), INIT_SEQ_NUM, 1};
	lsdb.add_router_lsa(lsa);
	destinations = lsdb.find_connections_with(u);
	std::pair<int, int> pair = std::make_pair(v, 1);
	answers.push_back(pair);
	ASSERT_EQ(destinations, answers);

	// Grabs 2 links
	lsa = { Link(u, x), INIT_SEQ_NUM, 2};
	lsdb.add_router_lsa(lsa);
	destinations = lsdb.find_connections_with(u);
	pair = std::make_pair(x, 2);
	answers.push_back(pair);
	ASSERT_EQ(destinations, answers);

	// Doesn't grab the unrelated link
	lsa = { Link(w, x), INIT_SEQ_NUM, 2};
	lsdb.add_router_lsa(lsa);
	destinations = lsdb.find_connections_with(u);
	ASSERT_EQ(destinations, answers);

}

TEST(RouterTests, Dijkstras) {
	enum node
	{
		u = 0,
		v = 1,
		w = 2,
		x = 3
	};

	LSDB lsdb;
	Router router(u);
	std::vector<std::tuple<int, int, unsigned int>> answers;
	std::tuple<int, int, unsigned int> package;

	// Test adjacent
	RouterLSA lsa = { Link(u, v), INIT_SEQ_NUM, 1};
	lsdb.add_router_lsa(lsa);
	router->set_networkLSD(lsdb);
	router->calculate_dijkstras();
	package = std::make_tuple(v, u, 1);
    answers.push_back(package);
	ASSERT_EQ(router->get_least_cost_dest(), answers);

	// Test 2 adjacent
	RouterLSA lsa = { Link(u, w), INIT_SEQ_NUM, 3};
	lsdb.add_router_lsa(lsa);
	router->set_networkLSD(lsdb);
	router->calculate_dijkstras();
	package = std::make_tuple(w, u, 3);
    answers.push_back(package);
	ASSERT_EQ(router->get_least_cost_dest(), answers);

	// Test 1 non-adjacent
	RouterLSA lsa = { Link(x, w), INIT_SEQ_NUM, 1};
	lsdb.add_router_lsa(lsa);
	router->set_networkLSD(lsdb);
	router->calculate_dijkstras();
	package = std::make_tuple(x, w, 4);
    answers.push_back(package);
	ASSERT_EQ(router->get_least_cost_dest(), answers);

	// Test 2 non-adjacent
	RouterLSA lsa = { Link(x, v), INIT_SEQ_NUM, 7};
	lsdb.add_router_lsa(lsa);
	router->set_networkLSD(lsdb);
	router->calculate_dijkstras();
	ASSERT_EQ(router->get_least_cost_dest(), answers);
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
