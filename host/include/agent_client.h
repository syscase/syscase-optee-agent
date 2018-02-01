#ifndef AGENT_CLIENT_H
#define AGENT_CLIENT_H

#include <sys/types.h>
#include "agent_ta.h"
#include "client.h"

void runTest(TEEC_Context *ctx, TEEC_Session *sess);
TEEC_Result invokeCall(TEEC_Session *sess, char* input, u_long size);

#endif /*AGENT_CLIENT_H*/

