#ifndef AGENT_CLIENT_H
#define AGENT_CLIENT_H

#include "agent_ta.h"
#include "client.h"

void run_test(TEEC_Context *ctx, TEEC_Session *sess, int argc, char **argv);
TEEC_Result invoke_call(TEEC_Session *sess, char *input, sc_u_long input_size);

#endif /*AGENT_CLIENT_H*/

