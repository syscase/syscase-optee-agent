#ifndef SYSCASE_SYSTEM_CALL_H
#define SYSCASE_SYSTEM_CALL_H

#define NARGS 8

struct system_call {
    u_int16_t no;
    u_int64_t args[NARGS];
};

#endif /*SYSCASE_SYSTEM_CALL_H*/

