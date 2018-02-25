#include "syscase/test_case.h"
#include "syscase/parse_handler.h"
#include "syscase/common.h"

int parse_argument(struct buffer *buffer, struct parse_state *state, int index, sc_u_int64_t *value)
{
  unsigned char type;
  parse_handler_t handler;


  if(get_u_int8_t(buffer, &type) == -1 || (handler = get_parse_handler(type)) == (parse_handler_t) -1)
    return -1;

  if(index >= 0)
    state->calls->types[index] = type;

  return handler(buffer, state, value);
}

int parse_calls(struct system_call *calls, int max_args, int ncalls, struct buffer *buffer, struct system_call *value)
{
  struct parse_state state;

  if(split_buffer(buffer, BUFFER_DELIMITER, sizeof BUFFER_DELIMITER - 1, NBUFFERS, state.buffers, &state.nbuffers) == -1
      || state.nbuffers < 1)
    return -1;

  buffer = &state.buffers[0];
  state.buffer_pos = 1;
  state.stack_pos = 0;
  state.calls = calls;
  state.ncalls = ncalls;

  if(get_u_int16_t(buffer, &value->no) == -1)
    return -1;

  if(syscase_verbose)
    sc_printf("call %d\n", value->no);

  for(int i = 0; i < max_args; i++) {
    if(syscase_verbose)
      sc_printf("arg %d: ", i);
    if(parse_argument(buffer, &state, i, &value->args[i]) == -1)
      return -1;
  }

  return 0;
}

int parse_test_case(struct buffer *buffer, int max_calls, int max_args, test_case_t *test_case, int *ncalls)
{
  struct buffer buffers[10];
  sc_size_t nbuffers;

  if(max_calls > 10)
    max_calls = 10;

  if(max_args > NARGS)
    max_args = NARGS;

  if(split_buffer(buffer, CALL_DELIMITER, sizeof CALL_DELIMITER - 1, max_calls, buffers, &nbuffers) == -1)
    return -1;

  for(sc_size_t i = 0; i < nbuffers; i++) {
    if(parse_calls(test_case, max_args, i, buffers + i, test_case + i) == -1)
      return -1;
  }

  *ncalls = nbuffers;
  
  return 0;
}

void dump_call(struct system_call *value, int max_args)
{
  sc_printf("syscall %d(", value->no);
  for(int i = 0; i < max_args; i++) {
    sc_printf("%lx", (sc_u_long) value->args[i]);
    if(i == max_args - 1){
      sc_printf(")\n");
      return;
    }

    sc_printf(", ");
  }
}

void dump_test_case(test_case_t *value, int n, int max_args)
{
  int i;

  if(max_args > NARGS)
    max_args = NARGS;

  for(i = 0; i < n; i++)
      dump_call(value + i, max_args);
}

unsigned long execute_test_case(test_case_t *value, int n)
{
  unsigned long result;
  int i;

  result = 0;
  for(i = 0; i < n; i++)
    result = sc_syscall(value + i);
  return result;
}
