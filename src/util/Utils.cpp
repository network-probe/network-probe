#include "Utils.h"
#include "Debug.h"

string printBits(int const size, void const * const ptr)
{
	string bits;
	if(size > 32)
	{
		;
		//todo
	}
	else
	{
		unsigned int a = 0x01;
		unsigned int *b = (unsigned int *)(ptr);

		for(int i=size-1; i >= 0; i--)
		{
			if ((*b) & (a << i))
			{
				bits += "1";
			}
			else
			{
				bits += "0";
			}
		}
	}

	return bits;
}

string getMacAddress(string eth_name)
{
#if 0
	char mac[32] = {0, };
	struct ifreq s;
	int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

	strcpy(s.ifr_name, eth_name.c_str());
	if (0 == ioctl(fd, SIOCGIFHWADDR, &s))
	{
		snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x", (unsigned char)s.ifr_addr.sa_data[0], (unsigned char)s.ifr_addr.sa_data[1], (unsigned char)s.ifr_addr.sa_data[2],
											(unsigned char)s.ifr_addr.sa_data[3], (unsigned char)s.ifr_addr.sa_data[4], (unsigned char)s.ifr_addr.sa_data[5]);
	}

	return string(mac);
#endif

#if 1
	ifstream iface("/sys/class/net/" + eth_name + "/address");
	string mac((istreambuf_iterator<char>(iface)), istreambuf_iterator<char>());
	string str = regex_replace(mac, std::regex("\n"), "");

	return str;
#endif
}

int getMacAddress(string eth_name, unsigned char mac[6])
{
	struct ifreq s;
	int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

	strcpy(s.ifr_name, eth_name.c_str());
	if (0 == ioctl(fd, SIOCGIFHWADDR, &s))
	{
		for(int i=0; i<6; i++)
		{
			mac[i] = (unsigned char)s.ifr_addr.sa_data[i];
//			snprintf(&mac[i], sizeof(char), "%02x", (unsigned char)s.ifr_addr.sa_data[i]);
		}
//		snprintf(mac, sizeof(char) * 6, "%02x:%02x:%02x:%02x:%02x:%02x", (unsigned char)s.ifr_addr.sa_data[0], (unsigned char)s.ifr_addr.sa_data[1], (unsigned char)s.ifr_addr.sa_data[2],
//											(unsigned char)s.ifr_addr.sa_data[3], (unsigned char)s.ifr_addr.sa_data[4], (unsigned char)s.ifr_addr.sa_data[5]);
	}

	return 1;
}

string getDottedAddress(unsigned int ip)
{
    struct in_addr ip_addr;
    ip_addr.s_addr = ip;
	return inet_ntoa(ip_addr);
}

string getDottedAddress(unsigned char *ip)
{
	char format[32] = {0, };
	snprintf(format, sizeof(format), "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

	string address = format;
	return address;
}

vector<string> getInterfaceNames()
{
	vector<string> names;

	struct ifaddrs *ifaddr;

	if (getifaddrs(&ifaddr) == -1) 
	{
		NP_LOGGER(Logger::NP_LOG_LEVEL_DEBUG, "Can't find network interface\n");
	}
	else
	{
		for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
		{
			if (ifa->ifa_addr == NULL)
			{
				continue;
			}
	#if 0
			int family		
			family = ifa->ifa_addr->sa_family;
			printf("%-8s %s (%d)\n", 
			ifa->ifa_name,
			(family == AF_PACKET) ? "AF_PACKET" :
			(family == AF_INET) ? "AF_INET" :
			(family == AF_INET6) ? "AF_INET6" : "???",
			family);
	#endif	
			//except "lo"
			if(string("lo").compare(ifa->ifa_name) != 0)
			{
				names.push_back(ifa->ifa_name);
			}			
		}
	}

	return names;
}

int charBufToInt(unsigned char *buf)
{
#if 1	
	int intergerNum = int((unsigned char)(buf[0]) << 24 |
            (unsigned char)(buf[1]) << 16 |
            (unsigned char)(buf[2]) << 8 |
            (unsigned char)(buf[3]));

	return intergerNum;
#else
	//CheckTodo: Not working
    int intergerNum;
    memcpy( &intergerNum, buf, sizeof( int ) );
    return intergerNum;	
#endif	
}