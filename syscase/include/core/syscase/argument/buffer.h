#ifndef SYSCASE_ARGUMENT_BUFFER_H
#define SYSCASE_ARGUMENT_BUFFER_H

#include "syscase/types.h"
#include "syscase/json.h"

#define ARG_TYPE_ID_BUFFER 0x2
#define ARG_TYPE_BUFFER "buffer"

int parse_argument_buffer(struct json_call_t* json_calls,
                int njson_calls,
                struct buffer* data,
                struct system_call* calls,
                int call_index,
                int arg_index);

#endif /*SYSCASE_ARGUMENT_BUFFER_H*/
