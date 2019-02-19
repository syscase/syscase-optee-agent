#include <stdlib.h>
#include <string.h>

#include "vendor/string.h"

#include "syscase/common.h"
#include "syscase/system_call.h"

int syscase_max_args = 8;

void* sc_malloc(sc_size_t size) {
  return malloc(size);
}

void* sc_memset(void* s, int c, sc_size_t n) {
  return memset(s, c, n);
}

void sc_free(void* ptr) {
  free(ptr);
}

void* sc_memmem(const void* haystack,
                sc_size_t haystacklen,
                const void* needle,
                sc_size_t needlelen) {
  return memmem(haystack, haystacklen, needle, needlelen);
}

unsigned long sc_syscall(struct system_call* sc) {
  uint32_t ret;
  sc_printf("sc_syscall optee\n");
  if (sc->no == 0) {
    sc_printf("ignore system call: return\n");
    return 0;
  }
  if (sc->no == 2) {
    sc_printf("ignore system call: panic\n");
    return 0;
  }
  if (sc->no == 12) {
    sc_printf("ignore system call: wait\n");
    return 0;
  }
  asm volatile(
      "str %[a7], [sp, #-16]!\n\t"
      "str %[a6], [sp, #-16]!\n\t"
      "str %[a5], [sp, #-16]!\n\t"
      "str %[a4], [sp, #-16]!\n\t"
      "str %[a3], [sp, #-16]!\n\t"
      "str %[a2], [sp, #-16]!\n\t"
      "str %[a1], [sp, #-16]!\n\t"
      "str %[a0], [sp, #-16]!\n\t"
      "str %[no], [sp, #-16]!\n\t"
      "ldr x8, [sp], #16\n\t"
      "ldr x0, [sp], #16\n\t"
      "ldr x1, [sp], #16\n\t"
      "ldr x2, [sp], #16\n\t"
      "ldr x3, [sp], #16\n\t"
      "ldr x4, [sp], #16\n\t"
      "ldr x5, [sp], #16\n\t"
      "ldr x6, [sp], #16\n\t"
      "ldr x7, [sp], #16\n\t"
      "svc #0"
      : "=r"(ret)
      : [no] "r"(sc->no), [a0] "r"(sc->args[0]), [a1] "r"(sc->args[1]),
        [a2] "r"(sc->args[2]), [a3] "r"(sc->args[3]), [a4] "r"(sc->args[4]),
        [a5] "r"(sc->args[5]), [a6] "r"(sc->args[6]), [a7] "r"(sc->args[7]));
  return ret;
}

void sc_read_file(char* filename, char** input, sc_size_t* input_size) {
  // Not supported
}

void sc_fork_guard(void (*handler)(void)) {
  // Not supported
}

void sc_guard(int pid, void (*handler)(void)) {
  // Not supported
}

void sc_exit(int status) {
  // Not supported
}
