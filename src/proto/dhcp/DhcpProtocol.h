/*
 * NTPProtocol.h
 *
 *  Created on: 2015. 5. 16.
 *      Author: sound79
 */

#ifndef DHCP_PROTOCOL_H_
#define DHCP_PROTOCOL_H_

#include "../AbstractProtocol.h"

/*
 * 		DHCP Protocol
 *
 * 		(1) Discovery
 * 		 - The DHCP client broadcasts a DHCPDISCOVER message on the network subnet using the destination address 255.255.255.255 (limited broadcast)
 * 		 	or the specific subnet broadcast address (directed broadcast).
 *
 * 		(2) Offer
 *		 - When a DHCP server receives a DHCPDISCOVER message from a client, which is an IP address lease request,
 *		 	the DHCP server reserves an IP address for the client and makes a lease offer by sending a DHCPOFFER message to the client.
 *
 * 		(3) Request
 * 		 - In response to the DHCP offer, the client replies with a DHCPREQUEST message, broadcast to the server, requesting the offered address.
 * 		 	A client can receive DHCP offers from multiple servers, but it will accept only one DHCP offer.
 *
 * 		(4) Acknowledge
 * 		 - When the DHCP server receives the DHCPREQUEST message from the client, the configuration process enters its final phase.
 * 		 	The acknowledgement phase involves sending a DHCPACK packet to the client.
 */


/*
	0                   1                   2                   3
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|    op (1)   |  htype (1)  |  hlen (1)   |  hops (1)   |
	+---------------+---------------+---------------+---------------+
	|                        xid (4)                        |
	+-------------------------------+-------------------------------+
	|         secs (2)          |         flags (2)         |
	+-------------------------------+-------------------------------+
	|                      ciaddr  (4)                      |
	+---------------------------------------------------------------+
	|                      yiaddr  (4)                      |
	+---------------------------------------------------------------+
	|                      siaddr  (4)                      |
	+---------------------------------------------------------------+
	|                      giaddr  (4)                      |
	+---------------------------------------------------------------+
	|                                                               |
	|                      chaddr  (16)                     |
	|                                                               |
	|                                                               |
	+---------------------------------------------------------------+
	|                                                               |
	|                      sname   (64)                     |
	+---------------------------------------------------------------+
	|                                                               |
	|                      file    (128)                    |
	+---------------------------------------------------------------+
	|                                                               |
	|                      options (variable)               |
	+---------------------------------------------------------------+
                  Figure:  Format of a DHCP message
 */


/*
	   FIELD      OCTETS       DESCRIPTION
   -----      ------       -----------

   op            1  Message op code / message type.
                    1 = BOOTREQUEST, 2 = BOOTREPLY
   htype         1  Hardware address type, see ARP section in "Assigned
                    Numbers" RFC; e.g., '1' = 10mb ethernet.
   hlen          1  Hardware address length (e.g.  '6' for 10mb
                    ethernet).
   hops          1  Client sets to zero, optionally used by relay agents
                    when booting via a relay agent.
   xid           4  Transaction ID, a random number chosen by the
                    client, used by the client and server to associate
                    messages and responses between a client and a
                    server.
   secs          2  Filled in by client, seconds elapsed since client
                    began address acquisition or renewal process.
   flags         2  Flags (see figure 2).
   ciaddr        4  Client IP address; only filled in if client is in
                    BOUND, RENEW or REBINDING state and can respond
                    to ARP requests.
   yiaddr        4  'your' (client) IP address.
   siaddr        4  IP address of next server to use in bootstrap;
                    returned in DHCPOFFER, DHCPACK by server.
   giaddr        4  Relay agent IP address, used in booting via a
                    relay agent.
   chaddr       16  Client hardware address.
   sname        64  Optional server host name, null terminated string.
   file        128  Boot file name, null terminated string; "generic"
                    name or null in DHCPDISCOVER, fully qualified
                    directory-path name in DHCPOFFER.
   options     var  Optional parameters field.  See the options
                    documents for a list of defined options.
*/


/*
	Documented in RFC 2132

	The following tables list the available DHCP options, as listed in RFC 2132[13] and IANA registry. <= written by wikipedia (https://en.wikipedia.org/wiki/Dynamic_Host_Configuration_Protocol)
*/

typedef enum DHCP_OPTIONS_T
{
	DHCP_OPTION_PAD = 0,		//	Pad[13]:Section 3.1	0 octets	Can be used to pad other options so that they are aligned to the word boundary; is not followed by length byte
	DHCP_OPTION_SUBNETMASK = 1,	//	Subnet mask[13]:Section 3.3	4 octets	Must be sent before the router option (option 3) if both are included
	DHCP_OPTION_TIMEOFFSET = 2,	//	Time offset[13]:Section 3.4	4 octets	
	DHCP_OPTION_ROUTER = 3,	//	Router	Multiples of 4 octets	Available routers, should be listed in order of preference
	DHCP_OPTION__TIMESERVER = 4,	//	Time server	Multiples of 4 octets	Available time servers to synchronise with, should be listed in order of preference
	DHCP_OPTION__NAMESERVER = 5, //	Name server	Multiples of 4 octets	Available IEN 116 name servers, should be listed in order of preference
	DHCP_OPTION_DOMAINSERVER = 6, //	Domain name server	Multiples of 4 octets	Available DNS servers, should be listed in order of preference
	DHCP_OPTION_LOGSERVER = 7, //	Log server	Multiples of 4 octets	Available log servers, should be listed in order of preference.
	DHCP_OPTION_COKKIESERVER = 8, //	Cookie server	Multiples of 4 octets	Cookie in this case means "fortune cookie" or "quote of the day", a pithy or humorous anecdote often sent as part of a logon process on large computers; it has nothing to do with cookies sent by websites.
	DHCP_OPTION_LPRSERVER = 9, //	LPR Server	Multiples of 4 octets	
	DHCP_OPTION_IMPRESSSERVER = 10, //	Impress server	Multiples of 4 octets	
	DHCP_OPTION_RESOURCE_LOCATIONSERVER = 11, //	Resource location server	Multiples of 4 octets	
	DHCP_OPTION_HOSTNAME = 12, //	Host name	Minimum of 1 octet	
	DHCP_OPTION_BOOT_FILESIZE = 13, //	Boot file size	2 octets	Length of the boot image in 4KiB blocks
	DHCP_OPTION_MERIT_DUMPFILE = 14, //	Merit dump file	Minimum of 1 octet	Path where crash dumps should be stored
	DHCP_OPTION_DOMAIN_NAME = 15, //	Domain name	Minimum of 1 octet	
	DHCP_OPTION_SWAP_SERVER = 16, //	Swap server	4 octets	
	DHCP_OPTION_ROOTPATH = 17, //	Root path	Minimum of 1 octet	
	DHCP_OPTION_EXTENSIONPATH = 18, //	Extensions path	Minimum of 1 octet	


	DHCP_OPTION_IPFORWARD = 19, //	IP forwarding enable/disable	1 octet	
	DHCP_OPTION_NONLOCAL_SOURCEROUTING = 20, //	Non-local source routing enable/disable	1 octet	
	DHCP_OPTION_POLICY_FILTER = 21, //	Policy filter	Multiples of 8 octets	
	DHCP_OPTION_DATAGRAM_RESSEMBLY_SIZE = 22, //	Maximum datagram reassembly size	2 octets	
	DHCP_OPTION_DEFAULT_IPTIME_TOLIVE = 23, //	Default IP time-to-live	1 octet	
	DHCP_OPTION_MTU_AGING_TIMEOUT = 24, //	Path MTU aging timeout	4 octets	
	DHCP_OPTION_MTU_PLATEAU_TABLE = 25, //	Path MTU plateau table	Multiples of 2 octets	


	DHCP_OPTION_INTERFACE_MTU = 26, //	Interface MTU	2 octets		
	DHCP_OPTION_ALL_SUBNET = 27, //	All subnets are local	1 octet	
	DHCP_OPTION_BROADCAST = 28, //	Broadcast address	4 octets	
	DHCP_OPTION_PERFORM_MASKDISCOVERY = 29, //	Perform mask discovery	1 octet	
	DHCP_OPTION_MASK_SUPPLIER = 30, //	Mask supplier	1 octet	
	DHCP_OPTION_PERFORM_ROUTERDISCOVERY = 31, //	Perform router discovery	1 octet	
	DHCP_OPTION_ROUTER_SOLICTATION_ADDR = 32, //	Router solicitation address	4 octets	
	DHCP_OPTION_STATIC_ROUTE = 33, //	Static route	Multiples of 8 octets	A list of destination/router pairs


	DHCP_OPTION_TRAILER_ENCAPSULATION = 34, //	Trailer encapsulation option	1 octet	
	DHCP_OPTION_ARP_CACHETIMEOUT = 35, //	ARP cache timeout	4 octets	
	DHCP_OPTION_ETHERNET_ENCAPSULATION = 36, //	Ethernet encapsulation	1 octet	


	DHCP_OPTION_TCP_DEFAULT_TTL = 37, //	TCP default TTL	1 octet	
	DHCP_OPTION_TCP_LEEPALIVE_INTERVAL = 38, //	TCP keepalive interval	4 octets	
	DHCP_OPTION_TCP_LEEPALIVE_GARBAGE = 39, //	TCP keepalive garbage	1 octet


	DHCP_OPTION_REQUEST_IPADDRESS = 50, //	Requested IP address	4 octets	
	DHCP_OPTION_IPADDRESS_LEASETIME = 51, //	IP address lease time	4 octets	
	DHCP_OPTION_OVERLOAD = 52, //	Option overload	1 octet	
	DHCP_OPTION_MESSAGE_TYPE = 53, //	DHCP message type	1 octet	
	DHCP_OPTION_SERVER_IDENTIFIER = 54, //	Server identifier	4 octets	
	DHCP_OPTION_PARAM_REQUEST_LIST = 55, //	Parameter request list	Minimum of 1 octet	
	DHCP_OPTION_MESSAGE_MIN = 56, //	Message	Minimum of 1 octet	
	DHCP_OPTION_MAX_MESSAGE_SIZE = 57, //	Maximum DHCP message size	2 octets	
	DHCP_OPTION_RENEWAL_TIME = 58, //	Renewal (T1) time value	4 octets	
	DHCP_OPTION_REBINDING_TIME = 59, //	Rebinding (T2) time value	4 octets	
	DHCP_OPTION_VENDOR_CLASS_IDENTIFIER = 60, //	Vendor class identifier	Minimum of 1 octet	
	DHCP_OPTION_CLIENT_IDENTIFIER = 61, //	Client-identifier	Minimum of 2 octets	
	DHCP_OPTION_TFTP_SERVERNAME = 66, //	TFTP server name	Minimum of 1 octet	
	DHCP_OPTION_BOOTFILE_NAME = 67, //	Bootfile name	Minimum of 1 octet			

	DHCP_OPTION_END = 255, //	End	0 octets	Used to mark the end of the vendor option field
}DHCP_OPTIONS;

typedef struct _DHCP_PROTOCOL_CONFIG
{
	u8 op;
	u8 htype;
	u8 hlen;
	u8 hops;
	u32 xid;
	u16 secs;
	u16 flags;
	u32 ciaddr;
	u32 yiaddr;
	u32 siaddr;
	u32 giaddr;
	u32 chaddr[4];
	u8 bootp_options[192];
	u32 magic_cookie;
}__attribute__((packed)) DHCP_PROTOCOL_CONFIG;

typedef struct _DHCP_PROTOCOL_FORMAT
{
	DHCP_PROTOCOL_CONFIG config;
	u8 opt[512];
}__attribute__((packed)) DHCP_PROTOCOL_FORMAT;


class DhcpProtocol : public AbstractProtocol
{
public:
	DhcpProtocol();
	virtual ~DhcpProtocol();

private:
	DHCP_PROTOCOL_FORMAT mDhcpProto;

public:
	int MakeCommand(PROTOCOL_CMD_TYPE cmd_type, unsigned char *cmd_buffer, ProtocolOption *option);
	int ParseData(unsigned char *recv_buffer, int recv_length);
	string GetPrettyString(void *data);

private:
	string getOPCode(unsigned char op);
	string getHwAddressType(unsigned char htype);
	string getDHCPMessageType(unsigned char type);
	string getOption(unsigned char *optr, int &offset);
};



#endif /* NTPPROTOCOL_H_ */
