#include <err.h>
#include <stdio.h>
#include <string.h>

#include "agent_client.h"

#include "afl_call.h"

TEEC_Result invokeCall(TEEC_Session *sess, char* input, u_long input_size)
{
  uint32_t err_origin;
  TEEC_Operation op;

  // Clear the TEEC_Operation struct
  memset(&op, 0, sizeof(op));

  // Prepare arguments
  op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_NONE,
                    TEEC_NONE, TEEC_NONE);
  op.params[0].value.a = 43;

  // Invoke TA_AGENT_CMD_CALL
  printf("Invoking call with argument: %d\n", op.params[0].value.a);
  TEEC_Result res = TEEC_InvokeCommand(sess, TA_AGENT_CMD_CALL, &op,
                      &err_origin);
  if (res != TEEC_SUCCESS)
    errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
      res, err_origin);
  printf("Receive TA response: %d\n", op.params[0].value.a);

  return res;
}

/*
 * Run test case
 */
void runTest(TEEC_Context *ctx, TEEC_Session *sess)
{
  u_long input_size;
  char *input;
  input = getWork(&input_size);
  printf("got work: %lu - %.*s\n", input_size, (int) input_size, input);

  /* Trace agent */
  //extern void _start(), __libc_start_main();
  //startWork((u_long)_start, (u_long)__libc_start_main)

  /* Trace Linux Kernel */
  //startWork(0xffff000000000000L, 0xffffffffffffffffL);

  /* Trace OPTEE Core */
  startWork(0xe100000, 0xe143fff);

  invokeCall(sess, input, input_size);

  doneWork(0);
}

