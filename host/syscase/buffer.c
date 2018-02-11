#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "syscase/buffer.h"
#include "syscase/utils.h"

void buffer_from(struct buffer *buffer, void *input, size_t size)
{
  buffer->pos = (u_int8_t *) input;
  buffer->end = buffer->pos + size;
}

unsigned char *buffer_pos(struct buffer *buffer)
{
  return buffer->pos;
}

size_t buffer_size(struct buffer *buffer)
{
  return buffer->end - buffer->pos;
}

int has_content(struct buffer *buffer)
{
  if(buffer->pos != buffer->end)
    return -1;
  return 0;
}

int get_u_int8_t(struct buffer *buffer, u_int8_t *value)
{
  if(buffer->pos >= buffer->end)
    return -1;
  *value = *buffer->pos;
  buffer->pos++;
  return 0;
}

int get_u_int16_t(struct buffer *buffer, u_int16_t *value)
{
  u_int8_t h, l;
  if(get_u_int8_t(buffer, &h) == -1 || get_u_int8_t(buffer, &l) == -1)
    return -1;
  *value = (h << 8) | l;
  return 0;
}

int get_u_int32_t(struct buffer *buffer, u_int32_t *value)
{
  u_int16_t h, l;
  if(get_u_int16_t(buffer, &h) == -1 || get_u_int16_t(buffer, &l) == -1)
    return -1;
  *value = (h << 16) | l;
  return 0;
}

int get_u_int64_t(struct buffer *buffer, u_int64_t *value)
{
  u_int32_t h, l;
  if(get_u_int32_t(buffer, &h) == -1 || get_u_int32_t(buffer, &l) == -1)
    return -1;
  *value = ((u_int64_t) h << 32) | l;
  return 0;
}

int split_buffer(struct buffer *buffer, char *delimiter, int delimiter_size,
                 size_t max, struct buffer *values, size_t *nvalues)
{
  unsigned char *pos;
  size_t i;

  for(i = 0; i < max && buffer->pos != buffer->end; i++) {
    pos = memmem(buffer->pos, buffer->end - buffer->pos, delimiter, delimiter_size);
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

void dump_buffer(unsigned char *buffer, size_t size)
{
  if(syscase_verbose > 1) {
    printf("buffer: ");
    dump_hex(buffer, size);
  }
}

