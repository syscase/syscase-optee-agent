#include "syscase/parse_state.h"
#include "syscase/common.h"
#include "syscase/argument/length.h"

int parse_argument_length(struct buffer *buffer, struct parse_state *state, sc_u_int64_t *value)
{
  if(pop_size(state, value) == -1)
    return -1;
  if(syscase_verbose)
    sc_printf("argument legnth %llx (buffer size %lu)\n", (unsigned long long) *value, buffer_size(buffer));
  return 0;
}

