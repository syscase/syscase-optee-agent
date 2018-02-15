#ifndef SYSCASE_TEST_CASE_H
#define SYSCASE_TEST_CASE_H

#include "syscase/buffer.h"

#define BUFFER_DELIMITER "\xa5\xc9"
#define CALL_DELIMITER "\xb7\xe3"

#define NARGS 8
#define NBUFFERS 7
#define SIZE_STACK_SIZE 256

extern int syscase_verbose;

struct system_call {
    u_int16_t no;
    u_int64_t args[NARGS];
};

struct parse_state {
  struct system_call *calls;
  int ncalls;
  struct buffer buffers[NBUFFERS];
  u_int64_t size_stack[SIZE_STACK_SIZE];
  size_t nbuffers;
  size_t buffer_pos;
  size_t stack_pos;
};

typedef struct system_call test_case_t;
typedef int (*parse_handler_t)(struct buffer *, struct parse_state *, u_int64_t *);

int push_size(struct parse_state *state, u_int64_t size);
int pop_size(struct parse_state *state, u_int64_t *size);
int parse_argument_number(struct buffer *buffer, struct parse_state *state, u_int64_t *value);
int parse_argument_alloc(struct buffer *buffer, struct parse_state *state, u_int64_t *value);
int parse_argument_buffer(struct buffer *buffer, struct parse_state *state, u_int64_t *value);
int parse_argument_length(struct buffer *buffer, struct parse_state *state, u_int64_t *value);
int parse_argument_file(struct buffer *buffer, struct parse_state *state, u_int64_t *value);
int parse_argument_fd(struct buffer *buffer, struct parse_state *state, u_int64_t *value);
int parse_argument_vector_64(struct buffer *buffer, struct parse_state *state, u_int64_t *value);
int parse_argument_filename(struct buffer *buffer, struct parse_state *state, u_int64_t *value);
int fork_test_child(u_int64_t *result_pid);
int parse_argument_pid(struct buffer *buffer, struct parse_state *state, u_int64_t *value);
int parse_argument_reference(struct buffer *buffer, struct parse_state *state, u_int64_t *value);
int parse_argument_vector_32(struct buffer *buffer, struct parse_state *state, u_int64_t *value);

parse_handler_t parse_map(unsigned char type);

int parse_argument(struct buffer *buffer, struct parse_state *state, u_int64_t *value);
int parse_calls(struct system_call *calls, int max_args, int ncalls, struct buffer *buffer, struct system_call *value);
int parse_test_case(struct buffer *buffer, int max_calls, int max_args, test_case_t *test_case, int *ncalls);
void dump_call(struct system_call *value, int max_args);
void dump_test_case(test_case_t *value, int n, int max_args);
unsigned long execute_call(struct system_call *value);
unsigned long execute_test_case(test_case_t *value, int n);

int get_fd(int type);

#endif /*SYSCASE_TEST_CASE_H*/

