#include "syscase/argument/vector_64.h"
#include "syscase/common.h"
#include "syscase/parse_state.h"
#include "syscase/test_case.h"

#include "syscase/cli/globals.h"

int parse_argument_vector_64(struct buffer* buffer,
                             struct parse_state* state,
                             sc_u_int64_t* value) {
  sc_u_int64_t* vector;
  sc_u_int8_t size;

  if (get_u_int8_t(buffer, &size) == -1)
    return -1;
  // TODO: Fix memory leak, using fork server for garbage collection
  vector = sc_malloc(size * sizeof vector[0]);
  if (size && !vector)
    return -1;
  if (syscase_verbose)
    sc_printf("argument vector 64 %llx - size %d\n",
              (unsigned long long)(sc_u_long)vector, size);
  for (int i = 0; i < size; i++) {
    if (syscase_verbose)
      sc_printf("vector %d: ", i);
    if (parse_argument(buffer, state, -1, &vector[i]) == -1)
      return -1;
  }
  if (push_size(state, size) == -1)
    return -1;
  *value = (sc_u_int64_t)(sc_u_long)vector;
  return 0;
}
