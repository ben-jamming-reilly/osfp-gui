#include <iostream>
#include <gtest/gtest.h>

TEST(InitTest, InitTest)
{
	// Used to check if GTest is configured properly
	EXPECT_EQ(1, 1);
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
