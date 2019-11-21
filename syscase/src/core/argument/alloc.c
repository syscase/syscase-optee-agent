#include "syscase/argument/alloc.h"
#include "syscase/common.h"

#include "syscase/cli/globals.h"
#include "syscase/utils.h"

int parse_argument_alloc(struct json_call_t* json_calls,
                int njson_calls,
                struct buffer* data,
                struct system_call* calls,
                int call_index,
                int arg_index) {
  void* p;
  sc_u_int64_t size;
  struct buffer tmp;

  buffer_from(&tmp, buffer_pos(data) + json_calls[call_index].args[arg_index].offset, json_calls[call_index].args[arg_index].size);
  if (get_u_int64_t(&tmp, &size) != 0) {
    return -1;
  }

#ifdef SYSCASE_DEBUG
  dump_hex((unsigned char*) buffer_pos(data) + json_calls[call_index].args[arg_index].offset, json_calls[call_index].args[arg_index].size);
  sc_printf("Parse alloc size %lu at offset %d, size %d\n", size, json_calls[call_index].args[arg_index].offset, json_calls[call_index].args[arg_index].size);
#endif

  // TODO: Fix memory leak, using fork server for garbage collection
  p = sc_malloc(size);
  if (!p)
    return -1;
  sc_memset(p, 0, size);
  
  calls[call_index].args[arg_index] = (sc_u_int64_t)(sc_u_long)p;
  return 0;
}

