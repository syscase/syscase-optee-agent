#ifndef SYSCASE_ARGUMENT_VECTOR_32_H
#define SYSCASE_ARGUMENT_VECTOR_32_H

#include "syscase/parse_state.h"
#include "syscase/types.h"

#define ARG_VECTOR_32 11

int parse_argument_vector_32(struct buffer* buffer,
                             struct parse_state* state,
                             sc_u_int64_t* value);

#endif /*SYSCASE_ARGUMENT_VECTOR_32_H*/
