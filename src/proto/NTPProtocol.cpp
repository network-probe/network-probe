/*
 * NTPProtocol.cpp
 *
 *  Created on: 2015. 5. 16.
 *      Author: sound79
 */


#include "NTPProtocol.h"

NTPProtocol::NTPProtocol()
{

}

NTPProtocol::~NTPProtocol()
{

}


int NTPProtocol::MakeCommand(PROTOCOL_CMD_TYPE cmd_type, unsigned char *cmd_buffer)
{
	int ret = 0;

	NTP_PROTOCOL_FORMAT* ntpCmd = reinterpret_cast<NTP_PROTOCOL_FORMAT*>(cmd_buffer);
	ntpCmd->leap = NTP_LEAP_UNKNOWN_UNSYNC;
	ntpCmd->version = 4;
	ntpCmd->mode = NTP_MODE_CLIENT;
	ntpCmd->stratum = 0; //unspecified or invalid
	ntpCmd->poll = 3;
	ntpCmd->rho = 0xFA;

	ntpCmd->delta_r = 1;
	ntpCmd->epsilon_r = 1;
	ntpCmd->refid = 0;

	ntpCmd->reftime = 0;
	ntpCmd->T1 = 0;
	ntpCmd->T2 = 0;

	return ret;
}

int NTPProtocol::ParseData()
{
	int ret = 0;

	return ret;
}

