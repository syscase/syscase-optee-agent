#include "syscase/cli/run.h"

#include "syscase/afl_call.h"
#include "syscase/buffer.h"
#include "syscase/cli/globals.h"
#include "syscase/cli/mode.h"
#include "syscase/cli/ncases.h"
#include "syscase/cli/options.h"
#include "syscase/cli/trace_handler.h"
#include "syscase/cli/usage.h"
#include "syscase/common.h"
#include "syscase/test_case.h"
#include "syscase/test_run.h"
#include "syscase/types.h"

static int is_power_of_two(int x) {
  return x > 0 && (x & (x - 1)) == 0;
}

static void guard_handler(void) {
  sc_printf("guard: done work!\n");
  done_work(0, syscase_flags);
}

int is_combined(int mode) {
  return !is_power_of_two(mode);
}

void run_combined(char* input, sc_u_long input_size, int fuzzing_mode) {
  struct buffer buffer;
  struct buffer cases[NCASES];
  int parse_result, ncases, i;
  unsigned char mode;

  sc_printf("Create combined input buffer\n");
  buffer_from(&buffer, input, input_size);
  sc_printf("Parse combined input\n");
  parse_result = split_test_cases(&buffer, NCASES, cases, &ncases);

  if (parse_result == -1) {
    return;
  }

  sc_printf("read %ld bytes, parse result %d number of cases %d\n", input_size,
            parse_result, (int)ncases);

  for (i = 0; i < ncases; i++) {
    if (get_u_int8_t(&cases[i], &mode) == -1) {
      sc_printf("Can not parse fuzzing mode!\n");
      return;
    }
    sc_printf("Run case with fuzzing mode %d and input size %lu\n", (int)mode,
              buffer_size(&cases[i]));
    run_case((char*)buffer_pos(&cases[i]), buffer_size(&cases[i]), fuzzing_mode,
             (int)mode);
  }
}

void run_case(char* input, sc_u_long input_size, int fuzzing_mode, int mode) {
  switch (mode & fuzzing_mode) {
    case MODE_LINUX:
      trace_linux_handler(input, input_size);
      break;
    case MODE_OPTEE:
      /* Trace OPTEE Core */
      sc_printf("Trace OPTEE System Call: Forward input to TA\n");
      trace_optee_handler(input, input_size);
      break;
    case MODE_SMC:
      /* Trace OPTEE SMC call */
      sc_printf(
          "Trace OPTEE Secure Monitor Call: Forward input to SMC Kernel "
          "Module\n");
      trace_smc_handler(input, input_size, syscase_flags);
      break;
    default:
      sc_printf("Unknown fuzzing mode %d\n", mode);
  }
}

/*
 * Invoke test case
 */
void invoke_test(int argc, char** argv) {
  sc_u_long input_size;
  char* input;

  process_options(argc, argv, &input, &input_size);

  sc_fork_guard(guard_handler);

  if (fuzzing_mode == 0) {
    usage(argv[0]);
  }

  if (is_combined(fuzzing_mode)) {
    syscase_flags |= FLAG_COMBINED;
    // Get test case in combined test case format
    // start_forkserver and get_work is executed by host applicaton
    get_test_case(&input, &input_size, syscase_flags & FLAG_TRACE);
    // Execute combined test case format by responsible agent
    run_combined(input, input_size, fuzzing_mode);
    done_work(0, syscase_flags & FLAG_TRACE);
    return;
  }

  // Execute single test case format
  // start_forkserver and get_work is executed by responsible agent
  run_case(input, input_size, fuzzing_mode, fuzzing_mode);
}
