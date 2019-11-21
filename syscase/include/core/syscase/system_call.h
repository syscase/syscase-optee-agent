#ifndef SYSCASE_SYSTEM_CALL_H
#define SYSCASE_SYSTEM_CALL_H

#include "syscase/types.h"

#define NARGS 9

struct system_call {
  sc_u_int64_t args[NARGS];
  int types[NARGS];
};

#endif /*SYSCASE_SYSTEM_CALL_H*/
