/*
 * AbstractProtocol.h
 *
 *  Created on: 2015. 5. 16.
 *      Author: sound79
 */

#ifndef ABSTRACTPROTOCOL_H_
#define ABSTRACTPROTOCOL_H_


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

typedef enum
{
	NTP_1_XXX = 0,
	MAX_PROTOCOL_CMD_COUNT
} PROTOCOL_CMD_TYPE;

class AbstractProtocol
{
public:
	AbstractProtocol() {}
	virtual ~AbstractProtocol() {}

public:
	virtual int MakeCommand(PROTOCOL_CMD_TYPE cmd_type, unsigned char *cmd_buffer) = 0;
	virtual int ParseData() = 0;
};


#endif /* ABSTRACTPROTOCOL_H_ */
