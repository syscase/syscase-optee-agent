#include "syscase/cli/globals.h"

#include "syscase/cli/run.h"
#include "syscase/cli/trace_handler.h"
#include "syscase/common.h"
#include "syscase/test_run.h"
#include "syscase/types.h"
#include "syscase/utils.h"
#include "syscase/json.h"
#include "syscase/test_case.h"

int syscase_verbose = 1;

int fuzzing_mode = 0;
int syscase_flags = FLAG_TRACE;

int main(int argc, char** argv) {
  invoke_test(argc, argv);
}

void trace_optee_handler(char* input, sc_u_long input_size) {
  struct json_arg_t json_args[NARGS];
  int njson_args;

  sc_printf("got work (OPTEE): %lu - %.*s\n", input_size, (int)input_size,
            input);
  dump_hex((unsigned char*)input, input_size);

  trace_test_case(
      input,
      input_size,
      // TA
      0x40000000,
      0x3fe00000,
      // OPTEE core
      0xe100000,
      0xe143fff,
      FLAG_COMBINED
  );
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

void trace_smc_driver_handler(char* input, sc_u_long input_size, int flags) {
  sc_printf("got work (SMC Driver): %lu - %.*s\n", input_size, (int)input_size, input);
  dump_hex((unsigned char*)input, input_size);
}

sc_u_long execute_smc_driver(test_case_t* value,
                             int njson_calls,
                             struct json_call_t* json_calls,
                             struct buffer* data) {
 // Not supported
 return -1;
}

