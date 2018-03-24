/*
 * AFL hypercalls
 */

#include <stdlib.h>

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include "syscase/afl_call.h"
#include "syscase/common.h"

int afl_test_mode = 0;
int started = 0;

#define SZ 4096
static sc_u_long bufsz;
static char *buf;
static sc_u_int64_t *arr;

static void
afl_init(void)
{
    static int afl_init = 0;
    char *pg;

    if(afl_init)
        return;

    pg = mmap(NULL, SZ, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_LOCKED, -1, 0);
    if(pg == (void*)-1) {
        perror("mmap");
        exit(1);
    }
    sc_memset(pg, 0, SZ); // touch all the bits!

    arr = (sc_u_int64_t *)pg;
    buf = pg + 2 * sizeof arr[0];
    bufsz = SZ - 2 * sizeof arr[0];

    afl_init = 1;
}

static inline sc_u_long
afl_call(sc_u_long a0, sc_u_long a1, sc_u_long a2)
{
#if !defined(SYSCASE_DUMMY)
  sc_u_long ret;
  asm("svc 0xfa32"
          : "=r"(ret)
          : "r"(a0), "r"(a1), "r"(a2)
          );
  return ret;
#else
  return 0;
#endif
}

int
start_forkserver(int ticks)
{
  sc_u_long result;
  afl_init();
  if(afl_test_mode || started)
    return 0;

  result = afl_call(1, ticks, 0);
  started = 1;
  return result;
}

char *
get_work(sc_u_long *sizep)
{
  afl_init();
  if(afl_test_mode)
    *sizep = read(0, buf, bufsz);
  else
    *sizep = afl_call(2, (sc_u_long)buf, bufsz);
  return buf;
}

/* buf should point to sc_u_int64_t[2] */
int
start_work(sc_u_int64_t start, sc_u_int64_t end)
{
  afl_init();
  if(afl_test_mode)
    return 0;
  arr[0] = start;
  arr[1] = end;
  return afl_call(3, (sc_u_long)arr, 0);
}

int
done_work(int val)
{
    afl_init();
    if(afl_test_mode)
        return 0;
    return afl_call(4, (sc_u_long)val, 0);
}

