#ifndef AGENT_CLIENT_H
#define AGENT_CLIENT_H

#include "agent_ta.h"
#include "client.h"
#include "syscase/test_case.h"

void runTest(TEEC_Context *ctx, TEEC_Session *sess, int argc, char **argv);
TEEC_Result invokeCall(TEEC_Session *sess, test_case_t* test_case, sc_u_long test_case_size, int ncalls);
void usage(char *program);
void process_options(int argc, char **argv, char **input, sc_u_long *input_size);
void trace_linux_kernel(test_case_t *test_case, int ncalls);

#endif /*AGENT_CLIENT_H*/

