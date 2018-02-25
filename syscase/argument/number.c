#include "syscase/parse_state.h"
#include "syscase/common.h"
#include "syscase/argument/number.h"

int parse_argument_number(struct buffer *buffer, struct parse_state *state, sc_u_int64_t *value)
{
  if(get_u_int64_t(buffer, value) == -1)
    return -1;
  if(syscase_verbose)
    sc_printf("argument number %llx (ncalls %d)\n", (unsigned long long)*value, state->ncalls);
  return 0;
}

