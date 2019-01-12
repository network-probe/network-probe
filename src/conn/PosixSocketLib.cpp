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
#include <stdlib.h>

#include "PosixSocketLib.h"

#include <poll.h>
#include <stdio.h>

//PosixSocketLib::PosixSocketLib() : mSocketFD(-1), mAddressType(0), mProtocolType(0)
//{
//}
//
//PosixSocketLib::~PosixSocketLib()
//{
//	close_socket();
//}

int PosixSocketLib::create_socket()
{
	if(mSocketFD > 0)
	{
		//Todo: print error message
		close_socket();
	}
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		//error
		mSocketFD = -1;
		return -1;
	}

	mSocketFD = sock;
	return 0;
}

int PosixSocketLib::close_socket()
{
	if(mSocketFD > 0)
	{
		close(mSocketFD);
		mSocketFD = -1;
	}
	return 0;
}

int PosixSocketLib::get_socket()
{
	return mSocketFD;
}

int PosixSocketLib::set_socket()
{
	return 0;
}

int PosixSocketLib::connect_socket(const char *address, int port)
{
	if(address == NULL || port < 0)
	{
		return -1;
	}

	if(create_socket() < 0)
	{
		return -1;
	}

	struct sockaddr_in targetAddr;
	targetAddr.sin_family = AF_INET;
	targetAddr.sin_addr.s_addr = inet_addr(address);
	targetAddr.sin_port = htons(port);

	int targetAddrLen = sizeof(targetAddr);

	if(connect(mSocketFD, (struct sockaddr *)&targetAddr, targetAddrLen) < 0)
	{
		return -1;
	}

	return 0;
}

int PosixSocketLib::disconnect_socket()
{
	int ret = close_socket();
	return ret;
}

int PosixSocketLib::send_socket(const char *msg, int msgLen)
{
	if(mSocketFD <= 0)
		return -1;

	int sentLen = send(mSocketFD, msg, msgLen, 0);
	return sentLen;
}

int PosixSocketLib::receive_socket(char *msgBuf, int msgBufLen)
{
	if(mSocketFD <= 0)
		return -1;

	int recvLen = recv(mSocketFD, msgBuf, msgBufLen, 0);
	return recvLen;
}

int PosixSocketLib::Connect(TConnection &conn)
{
	create_socket();
	int ret = connect_socket(conn.GetAddress().c_str(), conn.GetPort());
	if(ret == 0)
	{
		SocketMultiplexing::GetInstance()->AddEvent(get_socket(), reinterpret_cast<PosixSocketLib&>(*this));
	}

	return ret;
}

int PosixSocketLib::Disconnect()
{
	int ret = disconnect_socket();
	SocketMultiplexing::GetInstance()->DelEvent(get_socket());

	return ret;
}

int PosixSocketLib::Send(TConnBuffer &buffer)
{
	int ret = send_socket(buffer.GetBuffer(), buffer.GetDataSize());

//	if(mCallbackInstance)
//		mCallbackInstance->OnSend();

	return ret;
}

int PosixSocketLib::Receive(TConnBuffer &buffer)
{
	int ret = receive_socket(buffer.GetBuffer(), buffer.GetMaxBufferSize());
	buffer.SetDataSize(ret);
	return ret;
}

int PosixSocketLib::OnReceive()
{
	char testBuffer[1024] = {0,};
	int ret = receive_socket(testBuffer, 1024);
	if(ret > 0)
	{
		fprintf(stderr, "Receive[%d]: %s", ret, testBuffer);
		if(mCallbackInstance)
			mCallbackInstance->OnReceive(testBuffer, ret);
	}
	else if(ret == 0)
	{
		//Disconnected
		OnDisconnected();
	}
	else
	{
		//Error
	}

	return 0;
}

void PosixSocketLib::OnDisconnected()
{
	disconnect_socket();
}



SocketMultiplexing* SocketMultiplexing::mpInstance = NULL;

SocketMultiplexing::SocketMultiplexing()
{
	StartThread();
}

SocketMultiplexing::SocketMultiplexing(const SocketMultiplexing& other)
{
//	StartThread();
}
SocketMultiplexing::~SocketMultiplexing()
{

}

SocketMultiplexing* SocketMultiplexing::GetInstance()
{
	if(mpInstance == NULL)
	{
		mpInstance = new SocketMultiplexing();
	}

	return mpInstance;
}

int SocketMultiplexing::AddEvent(int sock, PosixSocketLib& instance)
{
	//Todo: Need to lock
	map<int, PosixSocketLib&>::iterator iter = mPosixSocketList.find(sock);
	if(iter != mPosixSocketList.end())
	{
		//Todo: Error Routine
		return -1;
	}
	else
	{
		mPosixSocketList.insert(map<int, PosixSocketLib&>::value_type(1, instance));
		UpdateEvent(true);
		return mPosixSocketList.size();
	}
}

int SocketMultiplexing::DelEvent(int sock)
{
	//Todo: Need to lock
	map<int, PosixSocketLib&>::iterator iter = mPosixSocketList.find(sock);
	if(iter == mPosixSocketList.end())
	{
		//Todo: Error Routine
		return -1;
	}
	else
	{
		mPosixSocketList.erase(sock);
		UpdateEvent(true);
		return mPosixSocketList.size();
	}
}

void SocketMultiplexing::OnEvent(void *pollStr, int max_count)
{
	//Todo: Need to lock
	struct pollfd* p = reinterpret_cast<struct pollfd*>(pollStr);
	for(int i=0; i < max_count; i++, p++)
	{
		if(p->revents & POLLIN)
		{
			map<int, PosixSocketLib&>::iterator iter = mPosixSocketList.find(p->fd);
			if(iter != mPosixSocketList.end())
			{
				if(iter->second.OnReceive() <= 0)
				{
					mPosixSocketList.erase(iter->first);
					UpdateEvent(true);
				}
			}
			else
			{
				cerr << "OnEvent Error" << endl;
			}
		}
	}
}

int SocketMultiplexing::GetPollFromEvent(void **pollStr)
{
	struct pollfd* p = reinterpret_cast<struct pollfd*>(*pollStr);
	map<int, PosixSocketLib&>::iterator iter = mPosixSocketList.begin();
	for(; iter != mPosixSocketList.end(); ++iter)
	{
		p->fd = iter->first;
		p->events = POLLIN;
		p->revents = 0;
	}

	return mPosixSocketList.size();
}

void *SocketMultiplexing::PollThread(void *arg)
{
	SocketMultiplexing *obj = reinterpret_cast<SocketMultiplexing *>(arg);

#define MAX_SOCKET_POLL_SIZE	1024
	struct pollfd *pollSet;
	struct pollfd **pollPtr = &pollSet;
	*pollPtr = (struct pollfd *)malloc(sizeof(struct pollfd) * MAX_SOCKET_POLL_SIZE);
	memset(pollSet, 0, MAX_SOCKET_POLL_SIZE);

	int curEventCount = 0;
	int ret = 0;

	while(1)
	{
		if(obj->CheckEvent() == true)
		{
			memset(pollSet, 0, MAX_SOCKET_POLL_SIZE);
			curEventCount = obj->GetPollFromEvent((void **)(pollPtr));
			obj->UpdateEvent(false);
		}

		ret = poll(pollSet, curEventCount+1, 1000);
		if(ret > 0)
		{
			fprintf(stderr, "[Event]: %s %s %d:\n", __FILE__, __FUNCTION__, __LINE__);
			obj->OnEvent(pollSet, curEventCount);
			obj->UpdateEvent(true);
		}
		else if(ret == 0)
		{
			//Todo: Poll Timeout
		}
		else
		{
			//Todo: Poll Error
		}

		cout << "Thread Loop" << endl;
//		sleep(1);
	}

	return NULL;
}

void SocketMultiplexing::StartThread()
{
	int ret = pthread_create(&mThreadID, NULL, &PollThread, this);
	if(ret != 0)
	{
		//Todo

	}
}
