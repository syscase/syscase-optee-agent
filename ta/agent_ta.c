#define STR_TRACE_USER_TA "AGENT"

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <stdint.h>
#include "syscase/ta_system_call.h"

#include "agent_ta.h"

int global;

static inline uint32_t
optee_system_call(struct ta_system_call *sc)
{
  uint32_t ret;
  asm("mov x8, %[value]"
          :
          : [value] "r" (sc->no));
  asm("svc #0"
          : "=r"(ret)
          : "r"(sc->args[0]), "r"(sc->args[1]), "r"(sc->args[2]), "r"(sc->args[3]), "r"(sc->args[4]), "r"(sc->args[5]), "r"(sc->args[6]), "r"(sc->args[7])
          );
  return ret;
}

static void dump_call(struct ta_system_call *value)
{
  printf("syscall %d(", value->no);
  for(int i = 0; i < NARGS; i++) {
    printf("%lx", (unsigned long) value->args[i]);
    if(i == NARGS - 1){
      printf(")\n");
      return;
    }

    printf(", ");
  }
}

static uint32_t execute_test_case(ta_test_case_t *value, int n)
{
  uint32_t result;
  int i;

  result = 0;
  for(i = 0; i < n; i++) {
    dump_call(value + i);
    result = optee_system_call(value + i);
  }
  return result;
}


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
  ta_test_case_t* test_case;
  uint64_t test_case_size;
  int ncalls;
  uint32_t result;

	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						   TEE_PARAM_TYPE_MEMREF_INPUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("TA AGENT: Receive test case");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

  test_case = params[1].memref.buffer;
  test_case_size = params[1].memref.size;
  ncalls = params[0].value.a;
	IMSG("TA AGENT: With test case size: %lu", test_case_size);
	IMSG("TA AGENT: With ncalls: %u", ncalls);
	IMSG("TA AGENT: Execute test case:");
  result = execute_test_case(test_case, ncalls);
	params[0].value.a = result;
	IMSG("TA AGENT response: %u", result);
	IMSG("global: %p", (void*)&global);

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
