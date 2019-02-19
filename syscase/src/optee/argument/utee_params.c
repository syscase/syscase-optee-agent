#include "tee_api_defines.h"
#include "utee_types.h"

#include "syscase/common.h"
#include "syscase/optee/argument/utee_params.h"
#include "syscase/parse_state.h"

#include "syscase/cli/globals.h"

int parse_argument_optee_utee_params(struct buffer* buffer,
                                     struct parse_state* state,
                                     sc_u_int64_t* value) {
  struct utee_params* params;
  int i;

  params = sc_malloc(sizeof(struct utee_params));
  sc_memset(params, 0, sizeof(struct utee_params));

  if (get_u_int64_t(buffer, &params->types) == -1)
    return -1;

  for (i = 0; i < TEE_NUM_PARAMS * 2; i++) {
    if (get_u_int64_t(buffer, &params->vals[i]) == -1)
      return -1;
  }

  *value = (sc_u_int64_t)(sc_u_long)params;

  if (syscase_verbose)
    sc_printf("argument optee utee_params type: %lx\n", params->types);

  return 0;
}
