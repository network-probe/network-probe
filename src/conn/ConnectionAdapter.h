/*
 * ConnectionAdapter.h
 *
 *  Created on: 2015. 4. 18.
 *      Author: sound79
 */

#ifndef CONNECTIONADAPTER_H_
#define CONNECTIONADAPTER_H_

#include "AbstractConnection.h"
#include "IConnection.h"
#include "PosixSocketLib.h"

class ConnectionAdapter : public IConnection
{
private:
	PosixSocketLib *mConnector;

public:
	ConnectionAdapter()
	{
		mConnector = new PosixSocketLib(this);
	}
	virtual ~ConnectionAdapter()
	{
		delete mConnector;
	}

private:
	virtual int IConnect(TConnection &conn)
	{
		return mConnector->Connect(conn);
	}

	virtual int IDisconnect()
	{
		return mConnector->Disconnect();
	}

	virtual int ISend(TConnBuffer &connBuffer)
	{
		return mConnector->Send(connBuffer);
	}

	virtual int IReceive(TConnBuffer &connBuffer)
	{
		return mConnector->Receive(connBuffer);
	}

public:
	int OnSend();
	int OnReceive(char* buffer, int length);

public:
	int TryConnect(Connection &conn);
	int TryDisconnect();
	int TrySend(TConnBuffer &buffer);
	int TryReceive(TConnBuffer &buffer);
};

#endif /* CONNECTIONADAPTER_H_ */
