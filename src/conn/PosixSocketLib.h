/*
 * PosixSocketLib.h
 *
 *  Created on: 2015. 4. 20.
 *      Author: sound79
 */

#ifndef POSIXSOCKETLIB_H_
#define POSIXSOCKETLIB_H_

#include <map>
#include "AbstractConnection.h"

#include <pthread.h>
/*
 * PosixSocketLib.cpp
 *
 *  Created on: 2015. 4. 18.
 *      Author: sound79
 */

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class PosixSocketLib : public AbstractConnection
{
private:
	int mSocketFD;
	int mAddressType;		//IPv4(0), IPv6(1)
	int mProtocolType;	//TCP(0), UDP(1)
	string mAddress;
	int mPort;

public:
	PosixSocketLib() : mSocketFD(-1), mAddressType(0), mProtocolType(0), mAddress(), mPort(-1)
	{

	}
	PosixSocketLib(IConnection* connIntp) : AbstractConnection(connIntp),
			mSocketFD(-1), mAddressType(0), mProtocolType(0)
	{

	}
	virtual ~PosixSocketLib()
	{

	}

private:
	int create_socket();
	int close_socket();

	int get_socket();
	int set_socket();
	int connect_socket(const char *address, int port, int proto_type);
	int disconnect_socket();
	int send_socket(const char *msg, int msgLen);
	int receive_socket(char *msgBuf, int msgBufLen);

public:
	int Connect(TConnection &conn);
	int Disconnect();
	int Send(TConnBuffer &buffer);
	int Receive(TConnBuffer &buffer);

	int OnReceive();
	void OnDisconnected();
};

class SocketMultiplexing
{
private:
	SocketMultiplexing();
	SocketMultiplexing(const SocketMultiplexing& other);
	~SocketMultiplexing();

private:
	static SocketMultiplexing* mpInstance;
	map<int, PosixSocketLib&> mPosixSocketList;
	pthread_t mThreadID;
	bool mNeedToUpdate;

public:
	static SocketMultiplexing* GetInstance();

	int AddEvent(int sock, PosixSocketLib& instance);
	int DelEvent(int sock);
	bool CheckEvent() { return mNeedToUpdate; }
	void UpdateEvent(bool update) { mNeedToUpdate = update; }
	int GetPollFromEvent(void **pollStr);
	void OnEvent(void *pollStr, int max_count);

	static void *PollThread(void *arg);
	void StartThread();
};

#endif /* POSIXSOCKETLIB_H_ */
