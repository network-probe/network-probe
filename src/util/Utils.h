#include <iostream>
#include <fstream>
#include <regex>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <string.h>

using namespace std;

string printBits(int const size, void const * const ptr);

string getMacAddress(string eth_name);
int getMacAddress(string eth_name, unsigned char mac[6]);
string getDottedAddress(unsigned int ip);
string getDottedAddress(unsigned char *ip);
vector<string> getInterfaceNames();

int charBufToInt(unsigned char *buf);