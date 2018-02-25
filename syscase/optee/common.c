#include <stdlib.h>
#include <string.h>

#include "vendor/string.h"

#include "syscase/system_call.h"
#include "syscase/common.h"

void *sc_malloc(sc_size_t size)
{
  return malloc(size);
}

void *sc_memset(void *s, int c, sc_size_t n) {
  return memset(s, c, n);
}

void *sc_memmem(const void *haystack, sc_size_t haystacklen, const void *needle, sc_size_t needlelen)
{
  return memmem(haystack, haystacklen, needle, needlelen);
}

unsigned long sc_syscall(struct system_call *sc)
{
  uint32_t ret;
  sc_printf("sc_syscall optee\n");
  asm("mov x8, %[value]"
          :
          : [value] "r" (sc->no));
  asm("svc #0"
          : "=r"(ret)
          : "r"(sc->args[0]), "r"(sc->args[1]), "r"(sc->args[2]), "r"(sc->args[3]), "r"(sc->args[4]), "r"(sc->args[5]), "r"(sc->args[6]), "r"(sc->args[7])
          );
  return ret;
}

