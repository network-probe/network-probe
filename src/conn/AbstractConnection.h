/*
 * AbstractConnection.h
 *
 *  Created on: 2015. 4. 22.
 *      Author: sound79
 */

#ifndef ABSTRACTCONNECTION_H_
#define ABSTRACTCONNECTION_H_

#include "Connection.h"
#include "IConnection.h"

class AbstractConnection
{
public:
	IConnection *mCallbackInstance;

public:
	AbstractConnection() : mCallbackInstance(NULL) {}
	AbstractConnection(IConnection* connIntp) : mCallbackInstance(connIntp) {}
	virtual ~AbstractConnection() {}
	virtual int Connect(TConnection &conn) = 0;
	virtual int Disconnect() = 0;
	virtual int Send(TConnBuffer &buffer) = 0;
	virtual int Receive(TConnBuffer &buffer) = 0;

	virtual int OnReceive() = 0;
};

#endif /* ABSTRACTCONNECTION_H_ */
