#ifndef SYSCASE_CLI_TRACE_HANDLER_H
#define SYSCASE_CLI_TRACE_HANDLER_H

#include "syscase/types.h"

void trace_optee_handler(char* input, sc_u_long input_size);
void trace_linux_handler(char* input, sc_u_long input_size);
void trace_smc_handler(char* input, sc_u_long input_size, int flags);

#endif /*SYSCASE_CLI_TRACE_HANDLER_H*/
