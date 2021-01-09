/*
 * AbstractProtocol.h
 *
 *  Created on: 2015. 5. 16.
 *      Author: sound79
 */

#ifndef ABSTRACTPROTOCOL_H_
#define ABSTRACTPROTOCOL_H_

#include <iostream>
#include <sstream>
#include <arpa/inet.h>
#include "../util/Utils.h"
#include "../util/Debug.h"

using namespace std;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef enum
{
	NTP_1_XXX = 0,
	DHCP_DISCOVERY = 10,
	DHCP_OFFER,
	DHCP_REQUEST,
	DHCP_ACKNOWLEDGE,
	MAX_PROTOCOL_CMD_COUNT
} PROTOCOL_CMD_TYPE;

class AbstractProtocol
{
public:
	AbstractProtocol() {}
	virtual ~AbstractProtocol() {}

public:
	virtual int MakeCommand(PROTOCOL_CMD_TYPE cmd_type, unsigned char *cmd_buffer) = 0;
	virtual int ParseData(unsigned char *recv_buffer) = 0;
	virtual string GetPrettyString(void *data) = 0;
};


#endif /* ABSTRACTPROTOCOL_H_ */
