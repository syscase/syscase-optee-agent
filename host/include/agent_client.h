#ifndef AGENT_CLIENT_H
#define AGENT_CLIENT_H

#include "agent_ta.h"
#include "client.h"

TEEC_Result invokeCall(TEEC_Context *ctx, TEEC_Session *sess);

#endif /*AGENT_CLIENT_H*/

