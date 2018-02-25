#include "syscase/buffer.h"
#include "syscase/utils.h"
#include "syscase/common.h"

void buffer_from(struct buffer *buffer, void *input, sc_size_t size)
{
  buffer->pos = (sc_u_int8_t *) input;
  buffer->end = buffer->pos + size;
}

unsigned char *buffer_pos(struct buffer *buffer)
{
  return buffer->pos;
}

sc_size_t buffer_size(struct buffer *buffer)
{
  return buffer->end - buffer->pos;
}

int has_content(struct buffer *buffer)
{
  if(buffer->pos != buffer->end)
    return -1;
  return 0;
}

int get_u_int8_t(struct buffer *buffer, sc_u_int8_t *value)
{
  if(buffer->pos >= buffer->end)
    return -1;
  *value = *buffer->pos;
  buffer->pos++;
  return 0;
}

int get_u_int16_t(struct buffer *buffer, sc_u_int16_t *value)
{
  sc_u_int8_t h, l;
  if(get_u_int8_t(buffer, &h) == -1 || get_u_int8_t(buffer, &l) == -1)
    return -1;
  *value = (h << 8) | l;
  return 0;
}

int get_u_int32_t(struct buffer *buffer, sc_u_int32_t *value)
{
  sc_u_int16_t h, l;
  if(get_u_int16_t(buffer, &h) == -1 || get_u_int16_t(buffer, &l) == -1)
    return -1;
  *value = (h << 16) | l;
  return 0;
}

int get_u_int64_t(struct buffer *buffer, sc_u_int64_t *value)
{
  sc_u_int32_t h, l;
  if(get_u_int32_t(buffer, &h) == -1 || get_u_int32_t(buffer, &l) == -1)
    return -1;
  *value = ((sc_u_int64_t) h << 32) | l;
  return 0;
}

int split_buffer(struct buffer *buffer, const char *delimiter, int delimiter_size,
                 sc_size_t max, struct buffer *values, sc_size_t *nvalues)
{
  unsigned char *pos;
  sc_size_t i;

  for(i = 0; i < max && buffer->pos != buffer->end; i++) {
    pos = sc_memmem(buffer->pos, buffer->end - buffer->pos, delimiter, delimiter_size);
    values[i].pos = buffer->pos;
    if(pos) {
      buffer->pos = pos + delimiter_size;
    } else {
      buffer->pos = buffer->end;
      pos = buffer->end;
    }
    values[i].end = pos;
  }

  if(buffer->pos != buffer->end)
    return -1;
  *nvalues = i;
  return 0;
}

void dump_buffer(unsigned char *buffer, sc_size_t size)
{
  if(syscase_verbose > 1) {
    sc_printf("buffer: ");
    dump_hex(buffer, size);
  }
}

