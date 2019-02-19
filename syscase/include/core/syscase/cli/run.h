#ifndef SYSCASE_CLI_RUN_H
#define SYSCASE_CLI_RUN_H

#include "syscase/types.h"

void invoke_test(int argc, char** argv);
int is_combined(int mode);
void run_combined(char* input, sc_u_long input_size, int fuzzing_mode);
void run_case(char* input, sc_u_long input_size, int fuzzing_mode, int mode);

#endif /*SYSCASE_CLI_RUN_H*/
