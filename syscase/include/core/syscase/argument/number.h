#ifndef SYSCASE_ARGUMENT_NUMBER_H
#define SYSCASE_ARGUMENT_NUMBER_H

#include "syscase/parse_state.h"
#include "syscase/types.h"

#define ARG_NUMBER 0

int parse_argument_number(struct buffer* buffer,
                          struct parse_state* state,
                          sc_u_int64_t* value);

#endif /*SYSCASE_ARGUMENT_NUMBER_H*/
