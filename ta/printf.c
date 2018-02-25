#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <printk.h>

#include "syscase/common.h"

int sc_printf(const char *format, ...)
{
  char *to_format;
  va_list arglist;
  size_t size;
  int result;

  // Limit printf to fixed size
  // TODO: Calculate real size of arglist
  size = 4096;

  to_format = malloc(size);
  memset(to_format, 0, size);
  
  va_start(arglist, format);
  vsnprintf(to_format, size, format, arglist);
  va_end(arglist);
  result = printf("%s", to_format);
  
  free(to_format);
  
  return result;
}

