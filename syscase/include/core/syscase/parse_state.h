#ifndef SYSCASE_PARSE_STATE_H
#define SYSCASE_PARSE_STATE_H

#include "syscase/buffer.h"
#include "syscase/system_call.h"

#define NBUFFERS 7
#define SIZE_STACK_SIZE 256

struct parse_state {
  struct system_call* calls;
  int ncalls;
  struct buffer buffers[NBUFFERS];
  sc_u_int64_t size_stack[SIZE_STACK_SIZE];
  sc_size_t nbuffers;
  sc_size_t buffer_pos;
  sc_size_t stack_pos;
};

int push_size(struct parse_state* state, sc_u_int64_t size);
int pop_size(struct parse_state* state, sc_u_int64_t* size);

#endif /*SYSCASE_PARSE_STATE_H*/
