#ifndef SYSCASE_ARGUMENT_ALLOC_H
#define SYSCASE_ARGUMENT_ALLOC_H

#include "syscase/types.h"
#include "syscase/json.h"

#define ARG_TYPE_ID_ALLOC 0x4
#define ARG_TYPE_ALLOC "alloc"

int parse_argument_alloc(struct json_call_t* json_calls,
                int njson_calls,
                struct buffer* data,
                struct system_call* calls,
                int call_index,
                int arg_index);

#endif /*SYSCASE_ARGUMENT_ALLOC_H*/
