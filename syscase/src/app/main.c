#include "syscase/cli/globals.h"

#include "syscase/cli/run.h"
#include "syscase/cli/trace_handler.h"
#include "syscase/common.h"
#include "syscase/test_run.h"
#include "syscase/types.h"
#include "syscase/utils.h"

int syscase_verbose = 1;

int fuzzing_mode = 0;
int syscase_flags = FLAG_TRACE;

int main(int argc, char** argv) {
  invoke_test(argc, argv);
}

void trace_optee_handler(char* input, sc_u_long input_size) {
  sc_printf("got work (OPTEE): %lu - %.*s\n", input_size, (int)input_size,
            input);
  dump_hex((unsigned char*)input, input_size);
}

void trace_linux_handler(char* input, sc_u_long input_size) {
  sc_printf("got work(Linux): %lu - %.*s\n", input_size, (int)input_size,
            input);
  dump_hex((unsigned char*)input, input_size);
}

void trace_smc_handler(char* input, sc_u_long input_size, int flags) {
  sc_printf("got work (SMC): %lu - %.*s\n", input_size, (int)input_size, input);
  dump_hex((unsigned char*)input, input_size);
}
