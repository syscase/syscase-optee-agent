#ifndef SYSCASE_TA_SYSTEM_CALL_H
#define SYSCASE_TA_SYSTEM_CALL_H

#include <stdint.h>

#define NARGS 8

typedef struct ta_system_call ta_test_case_t;

struct ta_system_call {
    uint16_t no;
    uint64_t args[NARGS];
};

#endif /*SYSCASE_TA_SYSTEM_CALL_H*/

