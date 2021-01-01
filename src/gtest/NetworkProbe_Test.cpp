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

TEST(Utils_Test, getMacAddress_Test)
{
	//should be changed
	unsigned char mac_test[6] = {0, };
	getMacAddress("wlp2s0", mac_test);
	char mac_str[32] = {0, };
	snprintf(mac_str, sizeof(mac_str), "%02x:%02x:%02x:%02x:%02x:%02x", mac_test[0], mac_test[1], mac_test[2], mac_test[3], mac_test[4], mac_test[5]);

	EXPECT_STREQ(mac_str, getMacAddress("wlp2s0").c_str());
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
