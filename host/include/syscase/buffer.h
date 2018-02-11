#ifndef SYSCASE_BUFFER_H
#define SYSCASE_BUFFER_H

#include <stdlib.h>

extern int syscase_verbose;

struct buffer {
    unsigned char *pos;
    unsigned char *end;
};

void buffer_from(struct buffer *buffer, void *input, size_t size);
unsigned char *buffer_pos(struct buffer *buffer);
size_t buffer_size(struct buffer *buffer);
int has_content(struct buffer *buffer);
int get_u_int8_t(struct buffer *buffer, u_int8_t *value);
int get_u_int16_t(struct buffer *buffer, u_int16_t *value);
int get_u_int32_t(struct buffer *buffer, u_int32_t *value);
int get_u_int64_t(struct buffer *buffer, u_int64_t *value);
int split_buffer(struct buffer *buffer, char *delimiter, int delimiter_size,
                 size_t max, struct buffer *value, size_t *nvalue);
void dump_buffer(unsigned char *buffer, size_t size);

#endif /*SYSCASE_BUFFER_H*/

