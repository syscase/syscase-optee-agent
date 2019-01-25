#ifndef SYSCASE_ARGUMENT_REFERENCE_H
#define SYSCASE_ARGUMENT_REFERENCE_H

#include "syscase/parse_state.h"
#include "syscase/types.h"

#define ARG_REFERENCE 10

int parse_argument_reference(struct buffer* buffer,
                             struct parse_state* state,
                             sc_u_int64_t* value);

#endif /*SYSCASE_ARGUMENT_REFERENCE_H*/
