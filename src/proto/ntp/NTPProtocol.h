/*
 * NTPProtocol.h
 *
 *  Created on: 2015. 5. 16.
 *      Author: sound79
 */

#ifndef NTPPROTOCOL_H_
#define NTPPROTOCOL_H_

#include "../AbstractProtocol.h"

typedef struct _NTP_PROTOCOL_FORMAT
{
#if 1
	u8 leap : 2;
	u8 version: 3;
	u8 mode: 3;
#else
	u8 li_vn_mode;
#endif
	u8 stratum;
	u8 poll;
	u8 rho;			//precision
	u32 delta_r;		//root delay
	u32 epsilon_r;	//root dispersion
	u32 refid;
#if 0
	u64 reftime;	//reference timestamp
	u64 T1;		//origin timestamp
	u64 T2;		//receive timestamp
	u64 T3;		//transmit timestamp
#else
	u32 reftime_s;
	u32 reftime_f;
	u32 origintime_s;
	u32 origintime_f;
	u32 receivetime_s;
	u32 receivetime_f;
	u32 transmittime_s;
	u32 transmittime_f;
#endif
#if 0	//extension
	u64 T4;
	u64 keyid;
	u64 dgst;
#endif
}NTP_PROTOCOL_FORMAT;


typedef enum
{
	NTP_LEAP_NO_WARNING = 0,
	NTP_LEAP_LMOTD_61,
	NTP_LEAP_LMOTD_59,
	NTP_LEAP_UNKNOWN_UNSYNC
}NTP_LEAP_TYPE;

typedef enum
{
	NTP_MODE_RESERVED = 0,
	NTP_MODE_SYMMETRIC_ACTIVE,
	NTP_MODE_SYMMETRIC_PASSIVE,
	NTP_MODE_CLIENT,
	NTP_MODE_SERVER,
	NTP_MODE_BROADCAST,
	NTP_MODE_NTP_CONTROL_MSG,
	NTP_MODE_RESERVED_PRIVATE_USE
}NTP_LEAP_ASSOC_MODE;

/*
+--------+-----------------------------------------------------+
| Value  | Meaning                                             |
+--------+-----------------------------------------------------+
| 0      | unspecified or invalid                              |
| 1      | primary server (e.g., equipped with a GPS receiver) |
| 2-15   | secondary server (via NTP)                          |
| 16     | unsynchronized                                      |
| 17-255 | reserved                                            |
+--------+-----------------------------------------------------+
*/

/*
+------+----------------------------------------------------------+
| ID   | Clock Source                                             |
+------+----------------------------------------------------------+
| GOES | Geosynchronous Orbit Environment Satellite               |
| GPS  | Global Position System                                   |
| GAL  | Galileo Positioning System                               |
| PPS  | Generic pulse-per-second                                 |
| IRIG | Inter-Range Instrumentation Group                        |
| WWVB | LF Radio WWVB Ft. Collins, CO 60 kHz                     |
| DCF  | LF Radio DCF77 Mainflingen, DE 77.5 kHz                  |
| HBG  | LF Radio HBG Prangins, HB 75 kHz                         |
| MSF  | LF Radio MSF Anthorn, UK 60 kHz                          |
| JJY  | LF Radio JJY Fukushima, JP 40 kHz, Saga, JP 60 kHz       |
| LORC | MF Radio LORAN C station, 100 kHz                        |
| TDF  | MF Radio Allouis, FR 162 kHz                             |
| CHU  | HF Radio CHU Ottawa, Ontario                             |
| WWV  | HF Radio WWV Ft. Collins, CO                             |
| WWVH | HF Radio WWVH Kauai, HI                                  |
| NIST | NIST telephone modem                                     |
| ACTS | NIST telephone modem                                     |
| USNO | USNO telephone modem                                     |
| PTB  | European telephone modem                                 |
+------+----------------------------------------------------------+
*/

#define NTP_TIMESTAMP_DELTA 2208988800ull

class NTPProtocol : public AbstractProtocol
{
public:
	NTPProtocol();
	virtual ~NTPProtocol();

	int MakeCommand(PROTOCOL_CMD_TYPE cmd_type, unsigned char *cmd_buffer);
	int ParseData(unsigned char *recv_buffer);
	string GetPrettyString(void *data);
	string PacketHeaderFormat();
	string PrintPrettyTime(unsigned int time);
};



#endif /* NTPPROTOCOL_H_ */
