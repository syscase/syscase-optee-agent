#ifndef SYSCASE_TEST_CASE_H
#define SYSCASE_TEST_CASE_H

#include "syscase/parse_state.h"
#include "syscase/json.h"

#define BUFFER_DELIMITER "\xa5\xc9"
#define CALL_DELIMITER "\xb7\xe3"
#define CASE_DELIMITER "\xe3\xb7"

#define BINARY_DELIMITER "\xb7\xe3"

typedef struct system_call test_case_t;

int parse_argument(struct json_call_t* json_calls,
                   int njson_calls,
                   struct buffer* data,
                   struct system_call* calls,
                   int call_index,
                   int arg_index);

int parse_calls(struct json_call_t* json_calls,
                int njson_calls,
                struct buffer* data,
                struct system_call* calls);

int parse_test_case(struct buffer* buffer,
                    int max_calls,
                    int max_args,
                    test_case_t* test_case,
                    int* ncalls);

int split_binary(struct buffer* buffer,
                 struct buffer* result_buffer,
                 int* ncases);

void dump_call(struct system_call* value);
void dump_test_case(test_case_t* value, int n);
sc_u_long execute_test_case(test_case_t* value, int n);
sc_u_long execute_smc_driver(test_case_t* value,
                             int njson_calls,
                             struct json_call_t* json_calls,
                             struct buffer* data);

#endif /*SYSCASE_TEST_CASE_H*/
