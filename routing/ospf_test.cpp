#include <iostream>
#include <gtest/gtest.h>
//#include "Router.h"
#include "LSDB.h"
#include "LSA.h"

TEST(InitTest, InitTest)
{
	// Used to check if GTest is configured properly
	EXPECT_EQ(1, 1);
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

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
