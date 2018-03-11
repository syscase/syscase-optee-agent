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
  asm volatile(
      "mov x8, %[no]\n\t"
      "mov x0, %[a0]\n\t"
      "mov x1, %[a1]\n\t"
      "mov x2, %[a2]\n\t"
      "mov x3, %[a3]\n\t"
      "mov x4, %[a4]\n\t"
      "mov x5, %[a5]\n\t"
      "mov x6, %[a6]\n\t"
      "mov x7, %[a7]\n\t"
      "svc #0"
      : "=r"(ret)
      : [no] "r" (sc->no),
        [a0] "r" (sc->args[0]),
        [a1] "r" (sc->args[1]),
        [a2] "r" (sc->args[2]),
        [a3] "r" (sc->args[3]),
        [a4] "r" (sc->args[4]),
        [a5] "r" (sc->args[5]),
        [a6] "r" (sc->args[6]),
        [a7] "r" (sc->args[7])
  );
  return ret;
}

