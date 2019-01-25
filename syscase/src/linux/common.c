#define _GNU_SOURCE
#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "syscase/system_call.h"
#include "syscase/common.h"

int syscase_max_args = 6;

void *sc_malloc(sc_size_t size)
{
  return malloc(size);
}

void *sc_memset(void *s, int c, sc_size_t n) {
  return memset(s, c, n);
}

void sc_free(void *ptr) {
  free(ptr);
}

void *sc_memmem(const void *haystack, sc_size_t haystacklen, const void *needle, sc_size_t needlelen)
{
  return memmem(haystack, haystacklen, needle, needlelen);
}

int sc_printf(const char *format, ...)
{
  int result;
  va_list arglist;
  va_start(arglist, format);
  result = vprintf(format, arglist);
  va_end(arglist);

  return result;
}

unsigned long sc_syscall(struct system_call *value)
{
  return syscall(
      value->no,
      value->args[0],
      value->args[1],
      value->args[2],
      value->args[3],
      value->args[4],
      value->args[5]
  );
}

