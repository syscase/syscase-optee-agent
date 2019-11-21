#ifndef SYSCASE_ARGUMENT_NUMBER_H
#define SYSCASE_ARGUMENT_NUMBER_H

#include "syscase/types.h"
#include "syscase/json.h"

#define ARG_TYPE_ID_NUMBER 0x1
#define ARG_TYPE_NUMBER "number"

int parse_argument_number(struct json_call_t* json_calls,
                int njson_calls,
                struct buffer* data,
                struct system_call* calls,
                int call_index,
                int arg_index);

#endif /*SYSCASE_ARGUMENT_NUMBER_H*/
