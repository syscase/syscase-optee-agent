#ifndef TA_AGENT_H
#define TA_AGENT_H

#include "syscase/system_call.h"

/* This UUID is generated with uuidgen
   the ITU-T UUID generator at http://www.itu.int/ITU-T/asn1/uuid.html */
#define TA_AGENT_UUID { 0x31930cad, 0x51bb, 0x4a66, \
		{ 0xb7, 0xc0, 0x4e, 0x93, 0x25, 0x00, 0x93, 0xb6} }

/* The Trusted Application Function ID(s) implemented in this TA */
#define TA_AGENT_CMD_CALL	0

#endif /*TA_AGENT_H*/
