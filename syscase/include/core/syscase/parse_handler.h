#ifndef SYSCASE_PARSE_HANDLER_H
#define SYSCASE_PARSE_HANDLER_H

#include "syscase/parse_state.h"

typedef int (*parse_handler_t)(struct buffer*,
                               struct parse_state*,
                               sc_u_int64_t*);

parse_handler_t get_parse_handler(unsigned char type);

#endif /*SYSCASE_PARSE_HANDLER_H*/
