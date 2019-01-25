#ifndef SYSCASE_ARGUMENT_BUFFER_H
#define SYSCASE_ARGUMENT_BUFFER_H

#include "syscase/parse_state.h"
#include "syscase/types.h"

#define ARG_BUFFER 2

int parse_argument_buffer(struct buffer* buffer,
                          struct parse_state* state,
                          sc_u_int64_t* value);

#endif /*SYSCASE_ARGUMENT_BUFFER_H*/
