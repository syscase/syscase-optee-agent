#ifndef SYSCASE_UTILS_H
#define SYSCASE_UTILS_H

#include <sys/types.h>

void read_file(char *filename, char **input, size_t *input_size);
void dump_hex(unsigned char *s, size_t size);

#endif /*SYSCASE_UTILS_H*/

