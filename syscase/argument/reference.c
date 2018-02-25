#include "syscase/parse_state.h"
#include "syscase/common.h"
#include "syscase/argument/reference.h"

int parse_argument_reference(struct buffer *buffer, struct parse_state *state, sc_u_int64_t *value)
{
  unsigned char call_index, argument_index;

  if(get_u_int8_t(buffer, &call_index) == -1 || get_u_int8_t(buffer, &argument_index) == -1
      || call_index >= state->ncalls
      || argument_index >= NARGS)
    return -1;

  *value = state->calls[call_index].args[argument_index];

  if(syscase_verbose)
    sc_printf("argument reference %llx - call %d  arg %d\n", (unsigned long long) *value, call_index, argument_index);

  return 0;
}

