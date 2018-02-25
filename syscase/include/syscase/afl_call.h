#ifndef SYSCASE_AFL_CALL_H
#define SYSCASE_AFL_CALL_H

#include "syscase/types.h"

extern int afl_test_mode;
int start_forkserver(int ticks);
char *get_work(sc_u_long *sizep);
int start_work(sc_u_int64_t start, sc_u_int64_t end);
int done_work(int val);

#endif /*SYSCASE_AFL_CALL_H*/

