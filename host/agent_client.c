#include <err.h>
#include <stdio.h>
#include <string.h>

#include "agent_client.h"

#include "syscase/cli/globals.h"
#include "syscase/cli/run.h"
#include "syscase/cli/trace_handler.h"
#include "syscase/types.h"
#include "syscase/test_run.h"
#include "syscase/smcchar/smc_call.h"

int syscase_verbose = 1;

int fuzzing_mode = 0;
int syscase_flags = FLAG_TRACE;

TEEC_Session *current_session = NULL;

TEEC_Result invoke_call(TEEC_Session *sess, char *input, sc_u_long input_size)
{
  uint32_t err_origin;
  TEEC_Operation op;

  // Clear the TEEC_Operation struct
  memset(&op, 0, sizeof(op));

  // Prepare arguments
  op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_MEMREF_TEMP_INPUT,
                    TEEC_NONE, TEEC_NONE);

  op.params[0].value.a = syscase_flags;
  if(!(syscase_flags & FLAG_TRACE) || syscase_flags & FLAG_COMBINED) {
	  op.params[1].tmpref.buffer = input;
	  op.params[1].tmpref.size = input_size;
  }

  // Invoke TA_AGENT_CMD_CALL
  TEEC_Result res = TEEC_InvokeCommand(sess, TA_AGENT_CMD_CALL, &op,
                      &err_origin);
  if (res != TEEC_SUCCESS)
    errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
      res, err_origin);
  printf("Receive TA system call result: %d\n", op.params[0].value.a);

  return res;
}

/*
 * Run test case
 */
void run_test(TEEC_Context *ctx, TEEC_Session *sess, int argc, char **argv)
{
  current_session = sess;
  invoke_test(argc, argv);
  current_session = NULL;
}

void trace_optee_handler(char* input, sc_u_long input_size) {
  invoke_call(current_session, input, input_size);
}

void trace_smc_handler(char* input, sc_u_long input_size, int flags) {
  smc_call(input, input_size, syscase_flags);
}

void trace_linux_handler(char *input, sc_u_long input_size)
{
  /* Trace agent */
  extern void _start(), __libc_start_main();

  trace_test_case(
      input,
      input_size,
      (sc_u_long)_start,
      (sc_u_long)__libc_start_main,
      0xffff000000000000L,
      0xffffffffffffffffL,
      syscase_flags
  );
}
