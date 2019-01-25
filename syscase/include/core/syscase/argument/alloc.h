#ifndef SYSCASE_ARGUMENT_ALLOC_H
#define SYSCASE_ARGUMENT_ALLOC_H

#include "syscase/parse_state.h"
#include "syscase/types.h"

#define ARG_ALLOC 1

int parse_argument_alloc(struct buffer* buffer,
                         struct parse_state* state,
                         sc_u_int64_t* value);

#endif /*SYSCASE_ARGUMENT_ALLOC_H*/
