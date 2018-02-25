#include "syscase/parse_state.h"

int push_size(struct parse_state *state, sc_u_int64_t size)
{
  sc_size_t stack_pos;

  if(state->stack_pos >= SIZE_STACK_SIZE)
    return -1;
  stack_pos = state->stack_pos++;
  state->size_stack[stack_pos] = size;
  return 0;
}

int pop_size(struct parse_state *state, sc_u_int64_t *size)
{
  sc_size_t stack_pos;
  if(state->stack_pos == 0)
    return -1;
  stack_pos = --state->stack_pos;
  *size = state->size_stack[stack_pos];
  return 0;
}

