#include "syscase/argument/buffer.h"
#include "syscase/common.h"
#include "syscase/parse_state.h"

#include "syscase/cli/globals.h"

int parse_argument_buffer(struct buffer* buffer,
                          struct parse_state* state,
                          sc_u_int64_t* value) {
  sc_size_t pos;
  sc_size_t size;
  struct buffer* tmp_buffer;

  if (state->buffer_pos >= state->nbuffers)
    return -1;
  pos = state->buffer_pos++;
  tmp_buffer = state->buffers + pos;
  size = buffer_size(tmp_buffer);
  if (push_size(state, size) == -1)
    return -1;
  *value = (sc_u_int64_t)(sc_u_long)buffer_pos(tmp_buffer);
  if (syscase_verbose)
    sc_printf("argument buffer %llx from %ld bytes (buffer size %lu)\n",
              (unsigned long long)*value, size, buffer_size(buffer));
  dump_buffer(buffer_pos(tmp_buffer), size);
  return 0;
}
