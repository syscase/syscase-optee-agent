#define _GNU_SOURCE
#include <linux/arm-smccc.h>
#include <linux/slab.h>
#include <linux/string.h>

#include "syscase/common.h"
#include "syscase/optee/smc/argument/msg_type.h"
#include "syscase/system_call.h"
#include "vendor/string.h"

int syscase_max_args = 7;

void* sc_malloc(sc_size_t size) {
  return kmalloc(size, GFP_KERNEL);
}

void* sc_memset(void* s, int c, sc_size_t n) {
  return memset(s, c, n);
}

void sc_free(void* ptr) {
  kfree(ptr);
}

void* sc_memmem(const void* haystack,
                sc_size_t haystacklen,
                const void* needle,
                sc_size_t needlelen) {
  return memmem(haystack, haystacklen, needle, needlelen);
}

int sc_printf(const char* format, ...) {
  int result;
  va_list arglist;
  va_start(arglist, format);
  result = vprintk(format, arglist);
  va_end(arglist);

  return result;
}

static void sc_smc_call(struct system_call* value, struct arm_smccc_res* res) {
  return arm_smccc_smc(value->no, value->args[0], value->args[1],
                       value->args[2], value->args[3], value->args[4],
                       value->args[5], value->args[6], res);
}

static void sc_smc_split(struct system_call* value) {
  int i;
  sc_u_int64_t addr;
  sc_u_int32_t reg0, reg1;
  // Do not check last argument
  for (i = 0; i < NARGS - 1; i++) {
    if (value->types[i] == ARG_OPTEE_MSG) {
      addr = value->args[i];
      reg0 = addr >> 32;
      reg1 = addr;
      value->args[i] = reg0;
      value->args[i + 1] = reg1;
      sc_printf("Split argument %d: %lx -> (%x, %x)", i, (sc_u_long)addr, reg0,
                reg1);
    }
  }
}

unsigned long sc_syscall(struct system_call* value) {
#if !defined(SYSCASE_DUMMY)
  struct arm_smccc_res res;
  sc_smc_split(value);
  sc_printf("sc_syscall: arm_smccc_smc");
  sc_smc_call(value, &res);

  sc_printf("Secure monitor call result: [%lx, %lx, %lx, %lx]\n", res.a0,
            res.a1, res.a2, res.a3);
  return res.a0;
#else
  return 0;
#endif
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
