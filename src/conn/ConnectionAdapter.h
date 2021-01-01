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
#include "../proto/AbstractProtocol.h"
#include "PosixSocketLib.h"

class ConnectionAdapter : public IConnection
{
private:
	PosixSocketLib *mConnector;
	AbstractProtocol *mProto;

public:
	ConnectionAdapter()
	{
		mConnector = new PosixSocketLib(this);
		mProto = NULL;
	}
	ConnectionAdapter(AbstractProtocol *protocol)
	{
		mConnector = new PosixSocketLib(this);
		mProto = protocol;
	}
	virtual ~ConnectionAdapter()
	{
		delete mConnector;
	}

private:
	virtual int ICreate(TConnection &conn)
	{
		return mConnector->Create(conn);
	}

	virtual int IConnect(TConnection &conn)
	{
		return mConnector->Connect(conn);
	}

	virtual int IDisconnect()
	{
		return mConnector->Disconnect();
	}

	virtual int IBind(TConnection &conn)
	{
		return mConnector->Bind(conn.GetPort());
	}

	virtual int ISend(TConnBuffer &connBuffer)
	{
		return mConnector->Send(connBuffer);
	}

	virtual int IReceive(TConnBuffer &connBuffer)
	{
		return mConnector->Receive(connBuffer);
	}

	virtual int ISetEvent()
	{
		return mConnector->SetEvent();
	}

	virtual int ISetSocketOption(unsigned int type, int value)
	{
		return mConnector->SetSocketOption(type, value);
	}

public:
	int OnSend();
	int OnReceive(unsigned char *buffer, int length);

public:
	int TryCreate(Connection &conn);
	int TryConnect(Connection &conn);
	int TryDisconnect();
	int TryBind(Connection &conn);
	int TrySend(TConnBuffer &buffer);
	int TryReceive(TConnBuffer &buffer);
	int TrySetEvent();

	int TrySetSocketOption(unsigned int type, int value);
};

#endif /* CONNECTIONADAPTER_H_ */
