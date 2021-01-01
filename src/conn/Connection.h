/*
 * Connection.h
 *
 *  Created on: 2015. 4. 22.
 *      Author: sound79
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <iostream>
#include <cstring>

using namespace std;

typedef enum
{
	CONN_TCP,
	CONN_UDP
}CONNECTION_TYPE;

typedef enum
{
	SOCK_REUSE_OPT 		= 1 << 0,
	SOCK_BROADCAST_OPT 	= 1 << 1,
}SOCKET_OPTION_TYPE;

struct Connection
{
	CONNECTION_TYPE mConnType;
	string mAddress;
	int mPort;

	void SetConnType(CONNECTION_TYPE type)
	{
		mConnType = type;
	}

	CONNECTION_TYPE GetConnType() const
	{
		return mConnType;
	}

	void SetAddress(string address)
	{
		mAddress = address;
	}

	string GetAddress() const
	{
		return mAddress;
	}

	void SetPort(int port)
	{
		mPort = port;
	}

	int GetPort() const
	{
		return mPort;
	}
};

struct ConnBuffer
{
//	const static int CONN_BUFFER_SIZE = 1024;
	unsigned char *mBuffer;
	int mLength;
	int mBufferSize;

	ConnBuffer(int buf_size)
	{
		mBufferSize = buf_size;
		mBuffer = new unsigned char[mBufferSize];
		mLength = 0;
	}
	~ConnBuffer()
	{
		delete [] mBuffer;
	}

	unsigned char* GetBuffer() const
	{
		return mBuffer;
	}

	int CopyData(const char *str, int length)
	{
		memcpy(mBuffer, str, length);
		mLength = length;

		return mLength;
	}

	const int GetDataSize() const
	{
		return mLength;
	}

	void SetDataSize(int length)
	{
		mLength = length;
	}

	const int GetMaxBufferSize() const
	{
		return mBufferSize;
	}

	void ClearBuffer()
	{
		memset(mBuffer, 0, mBufferSize);
	}
};

typedef struct Connection TConnection;
typedef struct ConnBuffer TConnBuffer;

#endif /* CONNECTION_H_ */
