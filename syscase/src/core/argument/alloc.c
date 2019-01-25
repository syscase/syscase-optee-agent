#include "syscase/argument/alloc.h"
#include "syscase/common.h"
#include "syscase/parse_state.h"

int parse_argument_alloc(struct buffer* buffer,
                         struct parse_state* state,
                         sc_u_int64_t* value) {
  void* p;
  sc_u_int32_t size;

  if (get_u_int32_t(buffer, &size) == -1)
    return -1;
  // TODO: Fix memory leak, using fork server for garbage collection
  p = sc_malloc(size);
  if (!p || push_size(state, size) == -1)
    return -1;
  sc_memset(p, 0, size);
  *value = (sc_u_int64_t)(sc_u_long)p;
  if (syscase_verbose)
    sc_printf("argument alloc %llx - allocated %x bytes\n",
              (unsigned long long)*value, size);
  return 0;
}
