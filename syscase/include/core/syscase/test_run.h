#ifndef SYSCASE_TEST_RUN_H
#define SYSCASE_TEST_RUN_H

#include "syscase/types.h"

#define FLAG_TRACE 0x1
#define FLAG_COMBINED 0x2

void get_test_case(char **input, sc_u_long *input_size, int trace);
sc_u_long trace_test_case(char *input, sc_u_long input_size, sc_u_int64_t start_parse, sc_u_int64_t end_parse, sc_u_int64_t start, sc_u_int64_t end, int flags);

#endif /*SYSCASE_TEST_RUN_H*/

