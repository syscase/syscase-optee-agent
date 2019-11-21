#ifndef SYSCASE_PARSE_HANDLER_H
#define SYSCASE_PARSE_HANDLER_H

#include "syscase/json.h"

typedef int (*parse_handler_t)(struct json_call_t* json_calls,
                int njson_calls,
                struct buffer* data,
                struct system_call* calls,
                int call_index,
                int arg_index);

parse_handler_t get_parse_handler(int type);
int parse_type_for(char* s, sc_u_long size);

#endif /*SYSCASE_PARSE_HANDLER_H*/
