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


//void test_PosixSocketLib();
void ntp_test();

int main(void)
{
	cout << "Start >>>" << endl;

	ntp_test();

	return EXIT_SUCCESS;
}

void ntp_test()
{
	ConnectionAdapter *connAdapter = new ConnectionAdapter();
	TConnection connection;

	connection.SetAddress(string("216.239.35.0"));
	connection.SetPort(123);
	connection.SetConnType(CONN_UDP);
	if(connAdapter->TryConnect(connection) < 0)
	{
		cerr << "Connect Socket Error" << endl;
		return;
	}

	NTPProtocol *ntp = new NTPProtocol();
	unsigned char *ntp_buffer = new unsigned char [sizeof(NTP_PROTOCOL_FORMAT)];
	ntp->MakeCommand(NTP_1_XXX, ntp_buffer);

	TConnBuffer buffer(sizeof(NTP_PROTOCOL_FORMAT));
	buffer.CopyData(reinterpret_cast<char *>(ntp_buffer), sizeof(NTP_PROTOCOL_FORMAT));

	if(connAdapter->TrySend(buffer) == buffer.GetDataSize())
	{
		cout << "Request: " << buffer.GetBuffer() << endl;
	}

	buffer.ClearBuffer();
	int recvLen = connAdapter->TryReceive(buffer);
	if(recvLen < 0)
	{
		cerr << "Receive Error" << endl;
	}
	cout << "Reply: " << buffer.GetBuffer() << endl;
	ntp->ParseData(buffer.GetBuffer());

	while(1)
	{
		sleep(1);
	}

	if(connAdapter->TryDisconnect() < 0)
	{
		cerr << "Disconnect Socket Error" << endl;
	}

	delete [] ntp_buffer;
	delete ntp;
	delete connAdapter;
}
