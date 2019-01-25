#ifndef SYSCASE_TEST_CASE_H
#define SYSCASE_TEST_CASE_H

#include "syscase/parse_state.h"

#define BUFFER_DELIMITER "\xa5\xc9"
#define CALL_DELIMITER "\xb7\xe3"
#define CASE_DELIMITER "\xe3\xb7"

typedef struct system_call test_case_t;

int parse_argument(struct buffer* buffer,
                   struct parse_state* state,
                   int index,
                   sc_u_int64_t* value);
int parse_calls(struct system_call* calls,
                int max_args,
                int ncalls,
                struct buffer* buffer,
                struct system_call* value);
int parse_test_case(struct buffer* buffer,
                    int max_calls,
                    int max_args,
                    test_case_t* test_case,
                    int* ncalls);
int split_test_cases(struct buffer* buffer,
                     int max_cases,
                     struct buffer* cases,
                     int* ncases);
void dump_call(struct system_call* value, int max_args);
void dump_test_case(test_case_t* value, int n, int max_args);
sc_u_long execute_test_case(test_case_t* value, int n);

#endif /*SYSCASE_TEST_CASE_H*/
