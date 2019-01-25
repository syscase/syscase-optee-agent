#ifndef SYSCASE_BUFFER_H
#define SYSCASE_BUFFER_H

#include "syscase/types.h"

extern int syscase_verbose;

struct buffer {
  unsigned char* pos;
  unsigned char* end;
};

void buffer_from(struct buffer* buffer, void* input, sc_size_t size);
unsigned char* buffer_pos(struct buffer* buffer);
sc_size_t buffer_size(struct buffer* buffer);
int has_content(struct buffer* buffer);
int get_u_int8_t(struct buffer* buffer, sc_u_int8_t* value);
int get_u_int16_t(struct buffer* buffer, sc_u_int16_t* value);
int get_u_int32_t(struct buffer* buffer, sc_u_int32_t* value);
int get_u_int64_t(struct buffer* buffer, sc_u_int64_t* value);
int split_buffer(struct buffer* buffer,
                 const char* delimiter,
                 int delimiter_size,
                 sc_size_t max,
                 struct buffer* value,
                 sc_size_t* nvalue);
void dump_buffer(unsigned char* buffer, sc_size_t size);

#endif /*SYSCASE_BUFFER_H*/
