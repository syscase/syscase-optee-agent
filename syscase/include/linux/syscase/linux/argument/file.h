#ifndef SYSCASE_LINUX_ARGUMENT_FILE_H
#define SYSCASE_LINUX_ARGUMENT_FILE_H

#include "syscase/parse_state.h"
#include "syscase/types.h"

#define ARG_FILE 4

int parse_argument_file(struct buffer* buffer,
                        struct parse_state* state,
                        sc_u_int64_t* value);

#endif /*SYSCASE_LINUX_ARGUMENT_FILE_H*/
