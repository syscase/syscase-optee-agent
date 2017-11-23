#include <err.h>
#include <stdio.h>
#include <string.h>

#include <tee_client_api.h>

#include <agent_ta.h>

int main(int argc, char *argv[])
{
	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op;
	TEEC_UUID uuid = TA_AGENT_UUID;
	uint32_t err_origin;

	/*
   * Initialize a context
   */
	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	/*
	 * Open session
	 */
	res = TEEC_OpenSession(&ctx, &sess, &uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
			res, err_origin);

	/*
	 * Invoke call function
	 */

	// Clear the TEEC_Operation struct
	memset(&op, 0, sizeof(op));

	// Prepare arguments
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);
	op.params[0].value.a = 43;

	// Invoke TA_AGENT_CMD_CALL
	printf("Invoking call with argument: %d\n", op.params[0].value.a);
	res = TEEC_InvokeCommand(&sess, TA_AGENT_CMD_CALL, &op,
				 &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);
	printf("Receive TA response: %d\n", op.params[0].value.a);

	/*
	 * Close session
	 */

	TEEC_CloseSession(&sess);

	/*
	 * Destroy context
	 */

	TEEC_FinalizeContext(&ctx);

	return 0;
}
