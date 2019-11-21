#ifndef SYSCASE_OPTEE_ARGUMENT_UUID_H
#define SYSCASE_OPTEE_ARGUMENT_UUID_H

#include "syscase/parse_state.h"
#include "syscase/types.h"

#define ARG_OPTEE_UUID 66

int parse_argument_optee_uuid(struct buffer* buffer,
                              struct parse_state* state,
                              sc_u_int64_t* value);

#endif /*SYSCASE_OPTEE_ARGUMENT_UUID_H*/
