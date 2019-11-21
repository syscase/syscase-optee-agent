#ifndef SYSCASE_LINUX_ARGUMENT_FILENAME_H
#define SYSCASE_LINUX_ARGUMENT_FILENAME_H

#include "syscase/parse_state.h"
#include "syscase/types.h"

#define ARG_FILENAME 8

int parse_argument_filename(struct buffer* buffer,
                            struct parse_state* state,
                            sc_u_int64_t* value);

#endif /*SYSCASE_LINUX_ARGUMENT_FILENAME_H*/
