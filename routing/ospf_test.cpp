#include <iostream>
#include <vector>
#include <string>
#include <gtest/gtest.h>
#include "Router.h"
#include "LSDB.h"
#include "LSA.h"
#include "native-ospf/router_json_parser.h"
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

TEST(RapidJsonTest, MakingNestedArrays)
{
	// forwarding table for node 0 in the following network topology:
	//			[1]
	//		 5 /   \ 1
	//		[0]-----[2]
	//			 11
	std::vector< std::vector<int> > forwardingTable(3);
	forwardingTable.at(0) = {0, 0, 0};
	forwardingTable.at(1) = {1, 0, 5};
	forwardingTable.at(2) = {2, 1, 6};

	std::string composed_json = composeForwardingTable(forwardingTable);
	std::string correct_json = "{\"forwardingTable\":[[0,0,0],[1,0,5],[2,1,6]]}";
	
	ASSERT_EQ(composed_json, correct_json);
}

TEST(RapidJsonTest, MakingNestedArraysWithStrings)
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

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
