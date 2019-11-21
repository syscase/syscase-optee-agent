#include "syscase/cli/run.h"

#include "syscase/afl_call.h"
#include "syscase/cli/globals.h"
#include "syscase/cli/options.h"
#include "syscase/cli/trace_handler.h"
#include "syscase/cli/usage.h"
#include "syscase/common.h"
#include "syscase/types.h"
#include "syscase/json.h"
#include "syscase/ncases.h"
#include "syscase/target.h"
#include "syscase/test_run.h"

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
  struct json_case_t json_cases[NCASES];
  int i, njson_cases;

  if (parse_json_cases(input, input_size, json_cases, &njson_cases) != 0) {
    return;
  }

  for (i = 0; i < njson_cases; i++) {

#ifdef SYSCASE_DEBUG
   sc_printf("Run case with fuzzing mode %d and input size %lu\n", json_cases[i].target, json_cases[i].size);
#endif

    run_case(json_cases[i].data, json_cases[i].size, fuzzing_mode,
             json_cases[i].target);
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
    case MODE_SMC_DRIVER:
      /* Trace OPTEE SMC call via driver */
      sc_printf(
          "Trace OPTEE Secure Monitor Call via driver\n");
      trace_smc_driver_handler(input, input_size, syscase_flags);
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
  int trace;

  process_options(argc, argv, &input, &input_size);

  if (fuzzing_mode == 0) {
    usage(argv[0]);
  }

  trace = syscase_flags & FLAG_TRACE;

  if (trace) {
    sc_fork_guard(guard_handler);
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

