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
//
//}

int ConnectionAdapter::OnSend()
{
	return 0;
}

int ConnectionAdapter::OnReceive(char* buffer, int length)
{
	np_trace;
	return 0;
}

int ConnectionAdapter::TryConnect(Connection &conn)
{
	return IConnect(conn);
}

int ConnectionAdapter::TryDisconnect()
{
	return IDisconnect();
}

int ConnectionAdapter::TrySend(TConnBuffer &buffer)
{
	return ISend(buffer);
}

int ConnectionAdapter::TryReceive(TConnBuffer &buffer)
{
	return IReceive(buffer);
}
