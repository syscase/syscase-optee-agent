#ifndef SYSCASE_CLI_OPTIONS_H
#define SYSCASE_CLI_OPTIONS_H

#include "syscase/types.h"

void process_options(int argc,
                     char** argv,
                     char** input,
                     sc_u_long* input_size);

#endif /*SYSCASE_CLI_OPTIONS_H*/
