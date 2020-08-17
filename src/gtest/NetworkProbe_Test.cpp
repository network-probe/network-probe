#include <stdio.h>
#include <iostream>
#include <gtest/gtest.h>

#include "../util/Utils.h"

using namespace std;

TEST(Utils_Test, printBits_Test)
{
	int test_number = 7;
	EXPECT_STREQ(string("111").c_str(), printBits(3, &test_number).c_str());

	test_number = 27;
	EXPECT_STREQ(string("11011").c_str(), printBits(5, &test_number).c_str());
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
