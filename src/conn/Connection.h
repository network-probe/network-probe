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
	const static int CONN_BUFFER_SIZE = 1024;
	char *mBuffer;
	int mLength;

	ConnBuffer()
	{
		mBuffer = new char[CONN_BUFFER_SIZE];
		mLength = 0;
	}
	~ConnBuffer()
	{
		delete [] mBuffer;
	}

	char* GetBuffer() const
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
		return CONN_BUFFER_SIZE;
	}

	void ClearBuffer()
	{
		memset(mBuffer, 0, CONN_BUFFER_SIZE);
	}
};

typedef struct Connection TConnection;
typedef struct ConnBuffer TConnBuffer;

#endif /* CONNECTION_H_ */
