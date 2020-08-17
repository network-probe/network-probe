/*
 * IConnection.h
 *
 *  Created on: 2015. 4. 22.
 *      Author: sound79
 */

#ifndef ICONNECTION_H_
#define ICONNECTION_H_

class IConnection
{
public:
	IConnection() {}
	virtual ~IConnection() {}
	virtual int IConnect(TConnection &conn) = 0;
	virtual int IDisconnect() = 0;
	virtual int ISend(TConnBuffer &connBuffer) = 0;
	virtual int IReceive(TConnBuffer &connBuffer) = 0;

	virtual int OnSend() = 0;
	virtual int OnReceive(unsigned char* buffer, int length) = 0;
};


#endif /* ICONNECTION_H_ */
