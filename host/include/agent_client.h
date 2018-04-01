#ifndef AGENT_CLIENT_H
#define AGENT_CLIENT_H

#include "agent_ta.h"
#include "client.h"
#include "syscase/test_case.h"

#define MODE_LINUX 1
#define MODE_OPTEE 2
#define MODE_SMC 3

void run_test(TEEC_Context *ctx, TEEC_Session *sess, int argc, char **argv);
TEEC_Result invoke_call(TEEC_Session *sess, char *input, sc_u_long input_size);
void usage(char *program);
void process_options(int argc, char **argv, char **input, sc_u_long *input_size);
void trace_linux_kernel(char *input, sc_u_long input_size);

#endif /*AGENT_CLIENT_H*/

