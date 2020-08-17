/*
 * NTPProtocol.cpp
 *
 *  Created on: 2015. 5. 16.
 *      Author: sound79
 */


#include "NTPProtocol.h"
#include <time.h>

NTPProtocol::NTPProtocol()
{

}

NTPProtocol::~NTPProtocol()
{

}


int NTPProtocol::MakeCommand(PROTOCOL_CMD_TYPE cmd_type, unsigned char *cmd_buffer)
{
	int ret = 0;
#if 1
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

	ntpCmd->reftime_s = ntpCmd->receivetime_f = 0;
	ntpCmd->origintime_s = ntpCmd->origintime_f = 0;
	ntpCmd->receivetime_s = ntpCmd->receivetime_f = 0;
	ntpCmd->transmittime_s = ntpCmd->transmittime_f = 0;
#else
	*( ( char * ) cmd_buffer + 0 ) = 0x1b; // Represents 27 in base 10 or 00011011 in base 2.
#endif
	return ret;
}

int NTPProtocol::ParseData(unsigned char *recv_buffer)
{
	int ret = 0;

	cout << Dump(recv_buffer) << endl;

	NTP_PROTOCOL_FORMAT* ntp_protocol = reinterpret_cast<NTP_PROTOCOL_FORMAT*>(recv_buffer);
	ntp_protocol->transmittime_s = ntohl( ntp_protocol->transmittime_s );
	ntp_protocol->transmittime_f = ntohl( ntp_protocol->transmittime_f );


	time_t txTm = ( time_t ) ( ntp_protocol->transmittime_s - NTP_TIMESTAMP_DELTA );
	cout << "Time: " << ctime( ( const time_t* ) &txTm ) << endl;

	return ret;
}

string NTPProtocol::PacketHeaderFormat()
{
	stringstream ss;

	ss << "0                   1                   2                   3\n";
	ss << "0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1\n";
	ss << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
	ss << "|LI | VN  |Mode |    Stratum     |     Poll      |  Precision   |\n";
	ss << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
	ss << "|                         Root Delay                            |\n";
	ss << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
	ss << "|                         Root Dispersion                       |\n";
	ss << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
	ss << "|                          Reference ID                         |\n";
	ss << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
	ss << "|                                                               |\n";
	ss << "+                     Reference Timestamp (64)                  +\n";
	ss << "|                                                               |\n";
	ss << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
	ss << "|                                                               |\n";
	ss << "+                      Origin Timestamp (64)                    +\n";
	ss << "|                                                               |\n";
	ss << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
	ss << "|                                                               |\n";
	ss << "+                      Receive Timestamp (64)                   +\n";
	ss << "|                                                               |\n";
	ss << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
	ss << "|                                                               |\n";
	ss << "+                      Transmit Timestamp (64)                  +\n";
	ss << "|                                                               |\n";
	ss << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
	ss << "|                                                               |\n";
	ss << ".                                                               .\n";
	ss << ".                    Extension Field 1 (variable)               .\n";
	ss << ".                                                               .\n";
	ss << "|                                                               |\n";
	ss << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
	ss << "|                                                               |\n";
	ss << ".                                                               .\n";
	ss << ".                    Extension Field 2 (variable)               .\n";
	ss << ".                                                               .\n";
	ss << "|                                                               |\n";
	ss << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
	ss << "|                          Key Identifier                       |\n";
	ss << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
	ss << "|                                                               |\n";
	ss << "|                            dgst (128)                         |\n";
	ss << "|                                                               |\n";
	ss << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";

	return ss.str();
}

string NTPProtocol::Dump(unsigned char *buffer)
{
	stringstream ss;
	NTP_PROTOCOL_FORMAT* protocol = reinterpret_cast<NTP_PROTOCOL_FORMAT *>(buffer);
	unsigned char _leap = protocol->leap;
	unsigned char _version = protocol->version;
	unsigned char _mode = protocol->mode;
	ss << "<Flags> " << "leap: " << printBits(2, &_leap) << ", version: " << printBits(3, &_version) << ", mode: " << printBits(3, &_mode) << "\n";
	ss << "Stratum: " << printBits(8, &protocol->stratum) << ", Poll: " << printBits(8, &protocol->poll) << ", Precision: " << printBits(8, &protocol->rho) << "\n";
	ss << "Root Delay: " << printBits(32, &protocol->delta_r) << "\n";
	ss << "Root Dispersion: " << printBits(32, &protocol->epsilon_r) << "\n";
	ss << "Root Reference ID: " << printBits(32, &protocol->refid) << "\n";

	ss << "Reference TimeStamp: " << printBits(32, &protocol->reftime_s) << printBits(32, &protocol->reftime_f) << "\n";
	ss << "Origin TimeStamp: " << printBits(32, &protocol->origintime_s) << printBits(32, &protocol->origintime_f) << "\n";
	ss << "Receive TimeStamp: " << printBits(32, &protocol->receivetime_s) << printBits(32, &protocol->receivetime_f) << "\n";
	ss << "Transmit TimeStamp: " << printBits(32, &protocol->transmittime_s) << printBits(32, &protocol->transmittime_f) << "\n";
//
//	char dump_buffer[1024] = {0, };
//	NTP_PROTOCOL_FORMAT* protocol = reinterpret_cast<NTP_PROTOCOL_FORMAT *>(buffer);
//
//	snprintf(dump_buffer, sizeof(dump_buffer), "Flags: leap: %x, version: %x, mode: %x\n"
//													"Startum: %x, Poll: %x, Precision: %x\n",
//													protocol->leap, protocol->version, protocol->mode,
//													protocol->stratum, protocol->poll, protocol->rho);
//	ss << dump_buffer;
//	ss << "aaaa\n";

//	ss << "leap: " << protocol->leap << " | " << "version: " << protocol->version << " | " << "mode: " << protocol->mode << endl;
//	ss << "stratum: " << protocol->stratum << " | " << "poll: " << protocol->poll << " | " << "rho: " << protocol->rho << endl;
//	ss << "delta_r: " << protocol->delta_r << " | " << "epsilon_r: " << protocol->epsilon_r << " | " << "refid: " << protocol->refid;
//	ss << "reftime_s: " << protocol->reftime_s << " | " << "reftime_f: " << protocol->reftime_f << endl;
//	ss << "origintime_s: " << protocol->origintime_s << " | " << "origintime_f: " << protocol->origintime_f << endl;
//	ss << "receivetime_s: " << protocol->receivetime_s << " | " << "receivetime_f: " << protocol->receivetime_f << endl;
//	ss << "transmittime_s: " << protocol->transmittime_s << " | " << "transmittime_f: " << protocol->transmittime_f << endl;

	return ss.str();
}

