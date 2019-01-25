#ifndef SYSCASE_SYSTEM_CALL_H
#define SYSCASE_SYSTEM_CALL_H

#include "syscase/types.h"

#define NARGS 8

struct system_call {
#if !defined(SYSCASE_SMC)
  sc_u_int16_t no;
#else
  sc_u_int64_t no;
#endif
  sc_u_int64_t args[NARGS];
  unsigned char types[NARGS];
};

#endif /*SYSCASE_SYSTEM_CALL_H*/
