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
#include "util/Debug.h"
#include "util/Config.h"

using namespace std;


//void test_PosixSocketLib();
void ntp_test(ProtocolOption *opt);
void dhcp_test(ProtocolOption *opt);

int main(int argc, char *argv[])
{
	Logger::SetLogging(Logger::NP_LOG_LEVEL_TRACE);
	// NP_LOGGER(Logger::NP_LOG_LEVEL_TRACE, "%s\n", "trace");
	// NP_LOGGER(Logger::NP_LOG_LEVEL_INFO, "%s\n", "info");
	// NP_LOGGER(Logger::NP_LOG_LEVEL_DEBUG, "%s\n", "debug");
	// NP_LOGGER(Logger::NP_LOG_LEVEL_WARN, "%s\n", "warn");
	// NP_LOGGER(Logger::NP_LOG_LEVEL_ERROR, "%s\n", "error");
	NP_LOGGER(Logger::NP_LOG_LEVEL_INFO, "Network Probe Start >>>\n");

	OptionArgs::GetInstance()->ParseOptions(argc, argv);
	vector<ProtocolOption *> parsed_protocols = OptionArgs::GetInstance()->GetOptions();
	for(vector<ProtocolOption *>::iterator iter = parsed_protocols.begin(); iter != parsed_protocols.end(); ++iter)
	{
		if((*iter)->GetProtocol() == NP_PROTO_NTP)
		{
			ntp_test(*iter);
		}
		else if((*iter)->GetProtocol() == NP_PROTO_DHCP)
		{
			dhcp_test(*iter);
		}
	}

	while(1)
	{
		sleep(1);
	}

	return EXIT_SUCCESS;
}

void ntp_test(ProtocolOption *opt)
{
	ConnectionAdapter *connAdapter = new ConnectionAdapter();
	TConnection connection;
#if 0
	connection.SetAddress(string("216.239.35.0"));
	connection.SetPort(123);
#else
	if(!opt->GetAddress().empty())
	{
		connection.SetAddress(opt->GetAddress());
	}
	connection.SetPort(opt->GetPort());
#endif
	connection.SetConnType(CONN_UDP);
	if(connAdapter->TryConnect(connection) < 0)
	{
		NP_LOGGER(Logger::NP_LOG_LEVEL_WARN, "Connect Error, Address=[%s], Port=[%d]\n", connection.GetAddress().c_str(), connection.GetPort());
		return;
	}

	NTPProtocol *ntp = new NTPProtocol();
	unsigned char *ntp_buffer = new unsigned char [sizeof(NTP_PROTOCOL_FORMAT)];
	ntp->MakeCommand(NTP_1_XXX, ntp_buffer, opt);

	TConnBuffer buffer(sizeof(NTP_PROTOCOL_FORMAT));
	buffer.CopyData(reinterpret_cast<char *>(ntp_buffer), sizeof(NTP_PROTOCOL_FORMAT));

	if(connAdapter->TrySend(buffer) != buffer.GetDataSize())
	{
		NP_LOGGER(Logger::NP_LOG_LEVEL_WARN, "Send Error, Address=[%s], Port=[%d], Desc=[%s]\n", connection.GetAddress().c_str(), connection.GetPort(), strerror(errno));
	}

	buffer.ClearBuffer();
	int recvLen = connAdapter->TryReceive(buffer);
	if(recvLen < 0)
	{
		NP_LOGGER(Logger::NP_LOG_LEVEL_WARN, "Receive Error, Address=[%s], Port=[%d], Desc=[%s]\n", connection.GetAddress().c_str(), connection.GetPort(), strerror(errno));
	}
	ntp->ParseData(buffer.GetBuffer(), buffer.GetDataSize());

	sleep(3);

	if(connAdapter->TryDisconnect() < 0)
	{
		NP_LOGGER(Logger::NP_LOG_LEVEL_WARN, "Disconnect Error, Address=[%s], Port=[%d], Desc=[%s]\n", connection.GetAddress().c_str(), connection.GetPort(), strerror(errno));
	}

	delete [] ntp_buffer;
	delete ntp;
	delete connAdapter;
}

void dhcp_test(ProtocolOption *opt)
{
	DhcpProtocol *dhcp = new DhcpProtocol();

	ConnectionAdapter *r_connAdapter = new ConnectionAdapter(dhcp);
	{
		TConnection r_connection;
#if 0		
		r_connection.SetPort(68);
#else
		r_connection.SetPort(opt->GetPort());
#endif		
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

		unsigned char *dhcp_buffer = new unsigned char [sizeof(DHCP_PROTOCOL_FORMAT)];
		int length = dhcp->MakeCommand(DHCP_DISCOVERY, dhcp_buffer, opt);
		TConnBuffer buffer(length);
		buffer.CopyData(reinterpret_cast<char *>(dhcp_buffer), length);		
		s_connAdapter->TrySend(buffer);

		sleep(3);

		length = dhcp->MakeCommand(DHCP_REQUEST, dhcp_buffer, opt);
		buffer.ClearBuffer();
		buffer.CopyData(reinterpret_cast<char *>(dhcp_buffer), length);		
		s_connAdapter->TrySend(buffer);

		delete [] dhcp_buffer;
	}

	sleep(3);

	delete r_connAdapter;
	delete s_connAdapter;
	delete dhcp;
}
