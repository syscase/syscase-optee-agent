#include "syscase/test_case.h"
#include "syscase/common.h"
#include "syscase/parse_handler.h"
#include "syscase/json.h"

#include "syscase/cli/globals.h"
#include "syscase/argument/number.h"

int parse_argument(struct json_call_t* json_calls,
                int njson_calls,
                struct buffer* data,
                struct system_call* calls,
                int call_index,
                int arg_index) {
  int type;
  parse_handler_t handler;

  type = json_calls[call_index].args[arg_index].type;

  if ((handler = get_parse_handler(type)) == (parse_handler_t)-1)
    return -1;

  calls[call_index].types[arg_index] = type;

  return handler(json_calls, njson_calls, data, calls, call_index, arg_index);
}

int parse_calls(struct json_call_t* json_calls,
                int njson_calls,
                struct buffer* data,
                struct system_call* calls) {
  for(int i = 0; i < njson_calls; i++) {
    // Set defaults
    for(int j = 0; j < NARGS; j++) {
      calls[i].args[j] = 0;
      calls[i].types[j] = ARG_TYPE_ID_NUMBER;
    }

    for(int j = 0; j < json_calls[i].size; j++) {

#ifdef SYSCASE_DEBUG
      sc_printf("Parse argument %d\n", j);
#endif
      parse_argument(json_calls, njson_calls, data, calls, i, j);
    }
  }

  return 0;
}

int split_binary(struct buffer* buffer,
                     struct buffer* result_buffer,
                     int* ncases) {
  sc_size_t nbuffers;

#ifdef SYSCASE_DEBUG
  sc_printf("split binary\n");
#endif

  if (split_buffer(buffer, BINARY_DELIMITER, sizeof BINARY_DELIMITER - 1, 2,
                   result_buffer, &nbuffers) == -1)
    return -1;

  *ncases = nbuffers;

  return 0;
}

void dump_call(struct system_call* value) {
  sc_printf("Call (");
  for (int i = 0; i < NARGS; i++) {
    sc_printf("%lx", (sc_u_long)value->args[i]);
    if (i == NARGS - 1) {
      sc_printf(")\n");
      return;
    }

    sc_printf(", ");
  }
}

void dump_test_case(test_case_t* value, int n) {
  for (int i = 0; i < n; i++)
    dump_call(value + i);
}

sc_u_long execute_test_case(test_case_t* value, int n) {
  sc_u_long result;
  int i;

  result = 0;
  for (i = 0; i < n; i++)
    result = sc_syscall(value + i);
  return result;
}
