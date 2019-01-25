#ifndef SYSCASE_AFL_CALL_H
#define SYSCASE_AFL_CALL_H

#include "syscase/types.h"

int start_forkserver(int ticks, int trace);
char* get_work(sc_u_long* sizep);
int start_work(sc_u_int64_t start, sc_u_int64_t end, int trace);
int done_work(int value, int trace);

#endif /*SYSCASE_AFL_CALL_H*/
