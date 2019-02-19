#ifndef SYSCASE_COMMON_H
#define SYSCASE_COMMON_H

#include "syscase/system_call.h"
#include "syscase/types.h"

extern int syscase_max_args;

void* sc_malloc(sc_size_t size);
void* sc_memset(void* s, int c, sc_size_t n);
void sc_free(void* ptr);
void* sc_memmem(const void* haystack,
                sc_size_t haystacklen,
                const void* needle,
                sc_size_t needlelen);

unsigned long sc_syscall(struct system_call* value);

int sc_printf(const char* format, ...);
int sc_printf1(const char* format);

void sc_read_file(char* filename, char** input, sc_size_t* input_size);

void sc_fork_guard(void (*handler)(void));
void sc_guard(int pid, void (*handler)(void));

void sc_exit(int status);

#endif /*SYSCASE_COMMON_H*/
