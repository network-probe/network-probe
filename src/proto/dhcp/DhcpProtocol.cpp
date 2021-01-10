/*
 * NTPProtocol.cpp
 *
 *  Created on: 2015. 5. 16.
 *      Author: sound79
 */


#include "DhcpProtocol.h"
#include <time.h>
#include <algorithm>

DhcpProtocol::DhcpProtocol()
{

}

DhcpProtocol::~DhcpProtocol()
{

}


int DhcpProtocol::MakeCommand(PROTOCOL_CMD_TYPE cmd_type, unsigned char *cmd_buffer, ProtocolOption *option)
{
	int opt_length = 0;

	DHCP_PROTOCOL_FORMAT *format = reinterpret_cast<DHCP_PROTOCOL_FORMAT *>(cmd_buffer);

	if(cmd_type == DHCP_DISCOVERY || cmd_type == DHCP_REQUEST)
	{
		format->config.op = 0x01;
		format->config.htype = 0x01;
		format->config.hlen = 0x06;
		format->config.hops = 0x00;

		format->config.xid = 0x3903F326;
		format->config.secs = 0x0000;
		format->config.flags = 0x0000;

		unsigned char mac[6] = {0, };
		vector<string> if_names = getInterfaceNames();
		if(!option->GetEthAddress().empty())
		{
			vector<string>::iterator iter = find(if_names.begin(), if_names.end(), option->GetEthAddress());
			if(iter->empty())
			{
				//This is mac address, so check and set to buffer
				int last = -1;
    			int rc = sscanf(option->GetEthAddress().c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx%n",
                    &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5], &last);

				if(rc != 6 || (int)option->GetEthAddress().size() != last)
				{
					NP_LOGGER(Logger::NP_LOG_LEVEL_DEBUG, "invalid mac address format = %s\n", option->GetEthAddress().c_str());
				}	
			}
			else
			{
				getMacAddress(option->GetEthAddress(), mac);
			}
		}
		else
		{
			//Use First Interface
			if(!if_names.empty())
			{
				getMacAddress(if_names.at(0), mac);
			}
		}

		if(cmd_type == DHCP_DISCOVERY)
		{
			format->config.ciaddr = 0x00000000;
			format->config.yiaddr = 0x00000000;
			format->config.siaddr = 0x00000000;
			format->config.giaddr = 0x00000000;
		}
		else if(cmd_type == DHCP_REQUEST)
		{
			format->config.ciaddr = mDhcpProto.config.yiaddr;
			format->config.yiaddr = mDhcpProto.config.siaddr;
			format->config.siaddr = 0x00000000;
			format->config.giaddr = 0x00000000;
		}


		format->config.chaddr[0] = (unsigned int)mac[3] << 24 | (unsigned int)mac[2] << 16 | (unsigned int)mac[1] << 8 | (unsigned int)mac[0];
		format->config.chaddr[1] = (unsigned int)mac[5] << 8 | (unsigned int)mac[4];
		format->config.chaddr[2] = 0x00000000 ;
		format->config.chaddr[3] = 0x00000000 ;

		format->config.magic_cookie = 0x63538263;

		//options
		//-->
		//DHCP Message Type
		format->opt[opt_length++] = 0x35;
		format->opt[opt_length++] = 0x01;
		if(cmd_type == DHCP_DISCOVERY)
		{
			format->opt[opt_length++] = 0x01;	//Discovery
		}
		else if(cmd_type == DHCP_REQUEST)
		{
			format->opt[opt_length++] = 0x03;	//Request
		}

		if(cmd_type == DHCP_DISCOVERY)
		{		
			//Request IP Address
			format->opt[opt_length++] = 0x32;
			format->opt[opt_length++] = 0x04;
			opt_length += 4;

			//Options: Parameter Request List
			format->opt[opt_length++] = 0x37;
			int length_index = opt_length;
			{
				opt_length++;
				format->opt[opt_length++] = DHCP_OPTION_SUBNETMASK;
				format->opt[opt_length++] = DHCP_OPTION_ROUTER;
				format->opt[opt_length++] = DHCP_OPTION_DOMAINSERVER;
				format->opt[opt_length++] = DHCP_OPTION_DOMAIN_NAME;
				format->opt[opt_length++] = DHCP_OPTION_BROADCAST;
			}
			format->opt[length_index] = opt_length - length_index - 1;
		}
		else if(cmd_type == DHCP_REQUEST)
		{
			;
		}

		format->opt[opt_length++] = 0xff;
		format->opt[opt_length++] = 0x01;
		format->opt[opt_length++] = 0x00;	
	}
	// else if(cmd_type == DHCP_OFFER)
	// {
	// 	//server side
	// }
	// else if(cmd_type == DHCP_REQUEST)
	// {

	// }
	// else if(cmd_type == DHCP_ACKNOWLEDGE)
	// {
	// 	//server side
	// }
	else
	{

	}

	return (sizeof(DHCP_PROTOCOL_CONFIG)+opt_length);
}

int DhcpProtocol::ParseData(unsigned char *recv_buffer, int recv_length)
{
	int ret = 0;
	if(recv_length < (int)sizeof(DHCP_PROTOCOL_FORMAT))
	{
		memcpy(&mDhcpProto, recv_buffer, recv_length);
	}
	else
	{
		memcpy(&mDhcpProto, recv_buffer, sizeof(DHCP_PROTOCOL_FORMAT));
	}
	cout << GetPrettyString(recv_buffer);
	return ret;
}

string DhcpProtocol::GetPrettyString(void *data)
{
	stringstream ss;

	DHCP_PROTOCOL_FORMAT *dhcp_data = reinterpret_cast<DHCP_PROTOCOL_FORMAT *>(data);

	ss << "" << endl;
	ss << "Dynamic Host Configuration Protocol (" << getDHCPMessageType(dhcp_data->opt[0]) << ")" << endl;
	ss << "\t Message type: " << getOPCode(dhcp_data->config.op) <<  " (" << static_cast<int>(dhcp_data->config.op) << ")" << endl;
	ss << "\t Hardware type: " << getHwAddressType(dhcp_data->config.htype) << " (" << static_cast<int>(dhcp_data->config.htype) << ")" << endl;
	ss << "\t Hardware address length: " << static_cast<int>(dhcp_data->config.hlen) << endl;
	ss << "\t Hops: " << static_cast<int>(dhcp_data->config.hops) << endl;
	ss << "\t Transaction ID: 0x" << hex << htonl(dhcp_data->config.xid) << endl;
	ss << "\t Seconds elapsed: " << static_cast<int>(dhcp_data->config.secs) << endl;

	ss << "\t Bootp flags: 0x" << hex << htons(dhcp_data->config.flags) << " (Unicast)" << endl;
	ss << "\t\t Broadcast flags: (Unicast)" << endl;
	ss << "\t\t Reserved flags: (0x0000)" << endl;

	ss << "\t Client IP address: " << getDottedAddress(dhcp_data->config.ciaddr) << endl;
	ss << "\t Your (client) IP address: " << getDottedAddress(dhcp_data->config.yiaddr) << endl;
	ss << "\t Next server IP address: " << getDottedAddress(dhcp_data->config.siaddr) << endl;
	ss << "\t Relay agent IP address: " << getDottedAddress(dhcp_data->config.giaddr) << endl;
	ss << "\t Client MAC address: (" << hex << (dhcp_data->config.chaddr[0] & 0xFF) << ":" << (dhcp_data->config.chaddr[0] >> 8 & 0xFF) << ":" << (dhcp_data->config.chaddr[0] >> 16 & 0xFF) << ":" << (dhcp_data->config.chaddr[0] >> 24 & 0xFF) 
												<< ":" << (dhcp_data->config.chaddr[1] & 0xFF) << ":" << (dhcp_data->config.chaddr[1] >> 8 & 0xFF) << ")" << endl;

	ss << "..." << endl;

	int index = 0;
	int offset = 0;
	
	while(dhcp_data->opt[index] != 0xFF)
	{
		ss << "\t Option: " << getOption(&dhcp_data->opt[index], offset) << endl; index += offset;
	}
	ss << "\t Option: (255) End" << endl;

	return ss.str();
}

string DhcpProtocol::getOPCode(unsigned char op)
{
	string message;
	if(op == 1)
	{
		message = "BOOTREQUEST";
	}
	else if(op == 2)
	{
		message = "BOOTREPLY";
	}
	else
	{
		message = "UNKNOWN";
	}
	
	return message;
}

string DhcpProtocol::getHwAddressType(unsigned char htype)
{
	//Ref ARP.
	string hardwareType;
	if(htype == 1)
	{
		hardwareType = "Ethernet";
	}
	else if(htype == 6)
	{
		hardwareType = "IEEE 802";
	}
		else
	{
		hardwareType = "UNKNOWN";
	}

	return hardwareType;
}

string DhcpProtocol::getDHCPMessageType(unsigned char type)
{
	string messageType;
	if(type == 0x01)
	{
		messageType = "Discover";
	}
	else if(type == 0x02)
	{
		messageType = "Offer";
	}
	else if(type == 0x03)
	{
		messageType = "Request";
	}
	else if(type == 0x04)
	{
		messageType = "Ack";
	}
	else if(type == 0x05)
	{
		messageType = "Nak";
	}
	else if(type == 0x06)
	{
		messageType = "Decline";
	}
	else if(type == 0x07)
	{
		messageType = "Release";
	}
	else if(type == 0x08)
	{
		messageType = "Inform";
	}

	return messageType;
}

string DhcpProtocol::getOption(unsigned char *optr, int &offset)
{
	stringstream ss;

	unsigned char *ptr = optr;
	int type = static_cast<int>(*ptr++);
	int legnth = static_cast<int>(*ptr++);

	switch(type)
	{
		case DHCP_OPTION_SUBNETMASK:
		{
			ss << "(" << type << ")" << " Subnet Mask (" << getDottedAddress(ptr) << ")";
		}
		break;

		case DHCP_OPTION_ROUTER:
		{
			ss << "(" << type << ")" << " Router (" << getDottedAddress(ptr) << ")";
		}
		break;

		case DHCP_OPTION_DOMAINSERVER:
		{
			ss << "(" << type << ")" << " Domain Name Server (" << getDottedAddress(ptr) << ")";
		}
		break;

		case DHCP_OPTION_IPADDRESS_LEASETIME:
		{
			ss << "(" << type << ")" << " IP Address Lease Time (" << charBufToInt(ptr) << ")";
		}
		break;

		case DHCP_OPTION_MESSAGE_TYPE:
		{
			ss << "(" << type << ")" << " DHCP Message Type (" << getDHCPMessageType(*ptr) << ")";	
		}
		break;

		case DHCP_OPTION_SERVER_IDENTIFIER:
		{
			ss << "(" << type << ")" << " DHCP Server Identifier (" << getDottedAddress(ptr) << ")";
		}
		break;

		default:
		{
			ss << "(" << type << ")" << " Unknown";
		}
		break;
	}

	offset = (ptr - optr) + legnth;

	return ss.str();
}