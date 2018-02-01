#include <err.h>
#include <stdio.h>
#include <string.h>

#include <agent_client.h>

/*
 * Invoke call function
 */
TEEC_Result invokeCall(TEEC_Context *ctx, TEEC_Session *sess)
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

