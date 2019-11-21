#ifndef SYSCASE_LINUX_ARGUMENT_PID_H
#define SYSCASE_LINUX_ARGUMENT_PID_H

#include "syscase/parse_state.h"
#include "syscase/types.h"

#define ARG_PID 9

int fork_test_child(sc_u_int64_t* result_pid);
int parse_argument_pid(struct buffer* buffer,
                       struct parse_state* state,
                       sc_u_int64_t* value);

#endif /*SYSCASE_LINUX_ARGUMENT_PID_H*/
