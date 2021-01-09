/*
 * ConnectionAdapter.cpp
 *
 *  Created on: 2015. 4. 18.
 *      Author: sound79
 */

#include <stdio.h>

#include "../util/Debug.h"
#include "ConnectionAdapter.h"


//ConnectionAdapter::ConnectionAdapter()
//{
//	mConnector = new PosixSocketLib();
//}
//
//ConnectionAdapter::~ConnectionAdapter()
//{ 

int ConnectionAdapter::OnSend()
{
	return 0;
}

int ConnectionAdapter::OnReceive(unsigned char* buffer, int length)
{
	if(mProto)
	{
		mProto->ParseData(buffer);
	}

	return 0;
}

int ConnectionAdapter::TryCreate(Connection &conn)
{
	return ICreate(conn);
}

int ConnectionAdapter::TryConnect(Connection &conn)
{
	return IConnect(conn);
}

int ConnectionAdapter::TryDisconnect()
{
	return IDisconnect();
}

int ConnectionAdapter::TryBind(Connection &conn)
{
	return IBind(conn);
}

int ConnectionAdapter::TrySend(TConnBuffer &buffer)
{
	return ISend(buffer);
}

int ConnectionAdapter::TryReceive(TConnBuffer &buffer)
{
	return IReceive(buffer);
}

int ConnectionAdapter::TrySetEvent()
{
	return ISetEvent();
}

int ConnectionAdapter::TrySetSocketOption(unsigned int type, int value)
{
	return ISetSocketOption(type, value);
}
