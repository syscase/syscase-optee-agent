#ifndef AGENT_CLIENT_H
#define AGENT_CLIENT_H

#include "agent_ta.h"
#include "client.h"
#include "syscase/test_run.h"

#define MODE_LINUX 0x1
#define MODE_OPTEE 0x2
#define MODE_SMC 0x4

#define NCASES 10

void run_test(TEEC_Context *ctx, TEEC_Session *sess, int argc, char **argv);
TEEC_Result invoke_call(TEEC_Session *sess, char *input, sc_u_long input_size);
void usage(char *program);
void process_options(int argc, char **argv, char **input, sc_u_long *input_size);
int is_combined(int mode);
void run_combined(TEEC_Context *ctx, TEEC_Session *sess, char *input, sc_u_long input_size, int fuzzing_mode);
void run_case(TEEC_Context *ctx, TEEC_Session *sess, char *input, sc_u_long input_size, int fuzzing_mode, int mode);
void trace_linux_kernel(char *input, sc_u_long input_size);

#endif /*AGENT_CLIENT_H*/

