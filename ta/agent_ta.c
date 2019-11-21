#define STR_TRACE_USER_TA "AGENT"

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <stdint.h>
#include "syscase/test_run.h"
#include "syscase/cli/trace_handler.h"
#include "syscase/test_case.h"

#include "agent_ta.h"

int syscase_verbose = 1;

/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */
TEE_Result TA_CreateEntryPoint(void)
{
	DMSG("TA AGENT: Created");

	return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void TA_DestroyEntryPoint(void)
{
	DMSG("TA AGENT: Destroyed");
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA. In this function you will normally do the global initialization for the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param __maybe_unused params[4],
		void __maybe_unused **sess_ctx)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("TA AGENT: Session opened");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Unused parameters */
	(void)&params;
	(void)&sess_ctx;

	/*
	 * The DMSG() macro is non-standard, TEE Internal API doesn't
	 * specify any means to logging from a TA.
	 */
	IMSG("TA AGENT: Hello!\n");

	/* If return value != TEE_SUCCESS the session will not be created. */
	return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx)
{
	(void)&sess_ctx; /* Unused parameter */
	DMSG("TA AGENT: Session closed!");
}

static TEE_Result call(uint32_t param_types,
	TEE_Param params[4])
{
  char* input;
  sc_u_long input_size;
  int flags;
  sc_u_long result;

	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						   TEE_PARAM_TYPE_MEMREF_INPUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("TA AGENT: Receive test case");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

  flags = params[0].value.a;
  if(!(flags & FLAG_TRACE) || flags & FLAG_COMBINED) {
    input = params[1].memref.buffer;
    input_size = params[1].memref.size;
	  IMSG("TA AGENT: With input size: %lu", input_size);
  } else {
    input = NULL;
    input_size = 0;
  }

	IMSG("TA AGENT: With flags: %x", flags);
	IMSG("TA AGENT: Execute test case:");

  result = trace_test_case(
      input,
      input_size,
      // TA
      0x40000000,
      0x3fe00000,
      // OPTEE core
      0xe100000,
      0xe143fff,
      flags
  );

	params[0].value.a = result;
	IMSG("TA AGENT response: %lu", result);

	return TEE_SUCCESS;
}

/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the paramters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(void __maybe_unused *sess_ctx,
			uint32_t cmd_id,
			uint32_t param_types, TEE_Param params[4])
{
	(void)&sess_ctx; /* Unused parameter */

	switch (cmd_id) {
	case TA_AGENT_CMD_CALL:
		return call(param_types, params);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}

sc_u_long execute_smc_driver(test_case_t* value,
                             int njson_calls,
                             struct json_call_t* json_calls,
                             struct buffer* data) {
 // Not supported
 return -1;
}

