#ifndef SYSCASE_COMMON_H
#define SYSCASE_COMMON_H

#include "syscase/types.h"
#include "syscase/system_call.h"

extern int syscase_max_args;

void *sc_malloc(sc_size_t size);
void *sc_memset(void *s, int c, sc_size_t n);
void sc_free(void *ptr);
void *sc_memmem(const void *haystack, sc_size_t haystacklen, const void *needle, sc_size_t needlelen);

unsigned long sc_syscall(struct system_call *value);

int sc_printf(const char *format, ...);
int sc_printf1(const char *format);

#endif /*SYSCASE_COMMON_H*/

