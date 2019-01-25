#ifndef SYSCASE_OPTEE_ARGUMENT_UTEE_PARAMS_H
#define SYSCASE_OPTEE_ARGUMENT_UTEE_PARAMS_H

#include "syscase/parse_state.h"
#include "syscase/types.h"

#define ARG_OPTEE_UTEE_PARAMS 65

int parse_argument_optee_utee_params(struct buffer* buffer,
                                     struct parse_state* state,
                                     sc_u_int64_t* value);

#endif /*SYSCASE_OPTEE_ARGUMENT_UTEE_PARAMS_H*/
