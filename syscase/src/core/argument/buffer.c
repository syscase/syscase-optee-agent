#include "syscase/argument/buffer.h"
#include "syscase/common.h"

#include "syscase/cli/globals.h"
#include "syscase/utils.h"

int parse_argument_buffer(struct json_call_t* json_calls,
                int njson_calls,
                struct buffer* data,
                struct system_call* calls,
                int call_index,
                int arg_index) {
  calls[call_index].args[arg_index] = (sc_u_int64_t) (buffer_pos(data) + json_calls[call_index].args[arg_index].offset);

#ifdef SYSCASE_DEBUG
  dump_hex((unsigned char*) buffer_pos(data) + json_calls[call_index].args[arg_index].offset, json_calls[call_index].args[arg_index].size);
#endif

  return 0;
}

