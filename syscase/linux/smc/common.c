#define _GNU_SOURCE
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/arm-smccc.h>

#include "vendor/string.h"
#include "syscase/system_call.h"
#include "syscase/common.h"

int syscase_max_args = 7;

void *sc_malloc(sc_size_t size)
{
  return kmalloc(size, GFP_KERNEL);
}

void *sc_memset(void *s, int c, sc_size_t n) {
  return memset(s, c, n);
}

void sc_free(void *ptr) {
  kfree(ptr);
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
  result = vprintk(format, arglist);
  va_end(arglist);

  return result;
}

unsigned long sc_syscall(struct system_call *value)
{
#if !defined(SYSCASE_DUMMY)
  struct arm_smccc_res res;
  sc_printf("sc_syscall: arm_smccc_smc");
  arm_smccc_smc(
      value->no,
      value->args[0],
      value->args[1],
      value->args[2],
      value->args[3],
      value->args[4],
      value->args[5],
      value->args[6],
      &res
  );

  sc_printf("Secure monitor call result: [%lx, %lx, %lx, %lx]\n", res.a0, res.a1, res.a2, res.a3);
  return res.a0;
#else
  return 0;
#endif
}

