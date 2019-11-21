#include "syscase/argument/number.h"
#include "syscase/common.h"

#include "syscase/cli/globals.h"
#include "syscase/utils.h"

int parse_argument_number(struct json_call_t* json_calls,
                int njson_calls,
                struct buffer* data,
                struct system_call* calls,
                int call_index,
                int arg_index) {
  struct buffer tmp;
  buffer_from(&tmp, buffer_pos(data) + json_calls[call_index].args[arg_index].offset, json_calls[call_index].args[arg_index].size);
  if (get_u_int64_t(&tmp, &calls[call_index].args[arg_index]) != 0) {
    return -1;
  }

#ifdef SYSCASE_DEBUG
  dump_hex((unsigned char*) buffer_pos(data) + json_calls[call_index].args[arg_index].offset, json_calls[call_index].args[arg_index].size);
  sc_printf("Parse number %lu at offset %d, size %d\n", calls[call_index].args[arg_index], json_calls[call_index].args[arg_index].offset, json_calls[call_index].args[arg_index].size);
#endif

  return 0;
}
