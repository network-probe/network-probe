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
#include "proto/ntp/NTPProtocol.h"
#include "proto/dhcp/DhcpProtocol.h"

using namespace std;


//void test_PosixSocketLib();
void ntp_test();
void dhcp_test();

int main(void)
{
	cout << "Start >>>" << endl;

	ntp_test();
	dhcp_test();

	while(1)
	{
		sleep(1);
	}

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

	sleep(3);

	if(connAdapter->TryDisconnect() < 0)
	{
		cerr << "Disconnect Socket Error" << endl;
	}

	delete [] ntp_buffer;
	delete ntp;
	delete connAdapter;
}

void dhcp_test()
{
	DhcpProtocol *dhcp = new DhcpProtocol();

	ConnectionAdapter *r_connAdapter = new ConnectionAdapter(dhcp);
	{
		TConnection r_connection;
		r_connection.SetPort(68);
		r_connection.SetConnType(CONN_UDP);

		r_connAdapter->TryCreate(r_connection);
		r_connAdapter->TrySetSocketOption((SOCK_REUSE_OPT | SOCK_BROADCAST_OPT), 1);

		r_connAdapter->TryBind(r_connection);
		r_connAdapter->TrySetEvent();
	}

	ConnectionAdapter *s_connAdapter = new ConnectionAdapter(dhcp);
	{
		TConnection s_connection;
		s_connection.SetAddress(string("255.255.255.255"));
		s_connection.SetPort(67);
		s_connection.SetConnType(CONN_UDP);

		s_connAdapter->TryCreate(s_connection);
		s_connAdapter->TrySetSocketOption(SOCK_BROADCAST_OPT, 1);
		s_connAdapter->TryConnect(s_connection);

#if 0
		unsigned char *dhcp_buffer = new unsigned char [sizeof(DHCP_PROTOCOL_FORMAT)];
		dhcp->MakeCommand(DHCP_DISCOVERY, dhcp_buffer);

		TConnBuffer buffer(sizeof(DHCP_PROTOCOL_FORMAT));
		buffer.CopyData(reinterpret_cast<char *>(dhcp_buffer), sizeof(DHCP_PROTOCOL_FORMAT));
#else
		unsigned char *dhcp_buffer = new unsigned char [sizeof(DHCP_PROTOCOL_FORMAT2)];
		int length = dhcp->MakeCommand(DHCP_DISCOVERY, dhcp_buffer);
		TConnBuffer buffer(length);
		buffer.CopyData(reinterpret_cast<char *>(dhcp_buffer), length);		
#endif		
		s_connAdapter->TrySend(buffer);
	}

	sleep(3);

	delete r_connAdapter;
	delete s_connAdapter;
	delete dhcp;
}
