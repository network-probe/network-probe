//============================================================================
// Name        : NetworkProbe.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "conn/PosixSocketLib.h"
#include "conn/ConnectionAdapter.h"
#include "proto/NTPProtocol.h"

using namespace std;


void test_PosixSocketLib();
void ntp_test(TConnBuffer &buffer);

int main(void)
{
	cout << "Start >>>" << endl;

	test_PosixSocketLib();

	return EXIT_SUCCESS;
}

void test_PosixSocketLib()
{
	ConnectionAdapter *connAdapter = new ConnectionAdapter();
	TConnection testConn;
	testConn.SetAddress(string("216.239.35.0"));
	testConn.SetPort(123);
	testConn.SetConnType(CONN_UDP);
	if(connAdapter->TryConnect(testConn) < 0)
	{
		cerr << "Connect Socket Error" << endl;
		return;
	}

	TConnBuffer testBuffer;
//	char testData[1024] = {0,};
//	strncpy(testData, "Hello\n", strlen("Hello\n"));
//	testBuffer.CopyData(testData, strlen(testData));
	ntp_test(testBuffer);
	sleep(1);
	if(connAdapter->TrySend(testBuffer) == testBuffer.GetDataSize())
	{
		cout << "Request: " << testBuffer.GetBuffer() << endl;
	}

	testBuffer.ClearBuffer();

//	int recvLen = connAdapter->TryReceive(testBuffer);
//	if(recvLen < 0)
//	{
//		cerr << "Receive Error" << endl;
//	}
//	cout << "Reply: " << testBuffer.GetBuffer() << endl;

	while(1)
	{
		sleep(1);
	}

	if(connAdapter->TryDisconnect() < 0)
	{
		cerr << "Disconnect Socket Error" << endl;
	}

	/*
	PosixSocketLib *testSocket = new PosixSocketLib();
	if(testSocket->ConnectSocket((char *)("127.0.0.1"), 8001) < 0)
	{
		cerr << "Connect Socket Error" << endl;
	}

	string msg("Hello\n");
	int sentLen = testSocket->SendSocket(msg.c_str(), msg.length());
	if(sentLen == static_cast<int>(msg.length()))
	{
		cout << "Request: " << msg << endl;
	}

	char recvBuf[1024] = {0,};
	int recvLen = testSocket->ReceiveSocket(recvBuf, 1024);
	recvBuf[recvLen] = '\0';
	cout << "Reply: " << recvBuf << endl;

	if(testSocket->DisconnectSocket() < 0)
	{
		cerr << "Disconnect Socket Error" << endl;
	}
	*/
}

void ntp_test(TConnBuffer &buffer)
{
	NTPProtocol *ntp = new NTPProtocol();
	unsigned char *ntp_buffer = new unsigned char [sizeof(NTP_PROTOCOL_FORMAT)];
	ntp->MakeCommand(NTP_1_XXX, ntp_buffer);
	buffer.CopyData(reinterpret_cast<char *>(ntp_buffer), sizeof(NTP_PROTOCOL_FORMAT));
	delete [] ntp_buffer;
}
