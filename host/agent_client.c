#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "agent_client.h"

#include "syscase/afl_call.h"
#include "syscase/test_run.h"
#include "syscase/test_case.h"
#include "syscase/buffer.h"
#include "syscase/utils.h"
#include "syscase/smcchar/smc_call.h"
#include "utils.h"
#include "guard.h"

int syscase_verbose = 1;

int fuzzing_mode = 0;
int syscase_flags = FLAG_TRACE;

TEEC_Result invoke_call(TEEC_Session *sess, char *input, sc_u_long input_size)
{
  uint32_t err_origin;
  TEEC_Operation op;

  // Clear the TEEC_Operation struct
  memset(&op, 0, sizeof(op));

  // Prepare arguments
  op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_MEMREF_TEMP_INPUT,
                    TEEC_NONE, TEEC_NONE);

  op.params[0].value.a = syscase_flags;
  if(!(syscase_flags & FLAG_TRACE) || syscase_flags & FLAG_COMBINED) {
	  op.params[1].tmpref.buffer = input;
	  op.params[1].tmpref.size = input_size;
  }

  // Invoke TA_AGENT_CMD_CALL
  TEEC_Result res = TEEC_InvokeCommand(sess, TA_AGENT_CMD_CALL, &op,
                      &err_origin);
  if (res != TEEC_SUCCESS)
    errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
      res, err_origin);
  printf("Receive TA system call result: %d\n", op.params[0].value.a);

  return res;
}

void usage(char *program)
{
  printf("usage:  %s [-tLO] [-i INPUT]\n", program);
  printf("\t\t-t\ttest mode, do not use hypercalls\n");
  printf("\t\t-i\tparse mode, parse given file (implies test mode)\n");
  printf("\t\t-L\tlinux mode, use 6 arguments\n");
  printf("\t\t-O\tOPTEE mode, use 8 arguments\n");
  printf("\t\t-S\tSMC mode, use 8 arguments\n");
  exit(1);
}

void process_options(int argc, char **argv, char **input, sc_u_long *input_size)
{
  int opt;
  while((opt = getopt (argc, argv, "i:tLOS")) != -1) {
    switch(opt) {
      case 'i':
        read_file(optarg, input, input_size);
      case 't':
        syscase_flags &= ~FLAG_TRACE;
        break;
      case 'O':
        fuzzing_mode |= MODE_OPTEE;
        break;
      case 'L':
        fuzzing_mode |= MODE_LINUX;
        break;
      case 'S':
        fuzzing_mode |= MODE_SMC;
        break;
      case '?':
      default:
        usage(argv[0]);
        break;
    }
  }
}

static void guard_handler(void)
{
  printf("guard: done work!\n");
  done_work(0, syscase_flags);
}

static int is_power_of_two(int x)
{
  return x > 0 && (x & (x-1)) == 0;
}

int is_combined(int mode)
{
  return !is_power_of_two(mode);
}

void run_combined(TEEC_Context *ctx, TEEC_Session *sess, char *input, sc_u_long input_size, int fuzzing_mode) {
  struct buffer buffer;
  struct buffer cases[NCASES];
  int parse_result, ncases, i;
  unsigned char mode;

  printf("Create combined input buffer\n");
  buffer_from(&buffer, input, input_size);
  printf("Parse combined input\n");
  parse_result = split_test_cases(&buffer, NCASES, cases, &ncases);

  if(parse_result == -1) {
    return;
  }

  printf("read %ld bytes, parse result %d number of cases %d\n", input_size, parse_result, (int)ncases);

  for(i = 0; i < ncases; i++) {
    if(get_u_int8_t(&cases[i], &mode) == -1) {
      printf("Can not parse fuzzing mode!\n");
      return;
    }
    printf("Run case with fuzzing mode %d and input size %lu\n", (int) mode, buffer_size(&cases[i]));
    run_case(ctx, sess, (char *) buffer_pos(&cases[i]), buffer_size(&cases[i]), fuzzing_mode,(int) mode);
  }
}

void run_case(TEEC_Context *ctx, TEEC_Session *sess, char *input, sc_u_long input_size, int fuzzing_mode, int mode)
{
  switch(mode & fuzzing_mode) {
    case MODE_LINUX:
      trace_linux_kernel(input, input_size);
      break;
    case MODE_OPTEE:
      /* Trace OPTEE Core */
      printf("Trace OPTEE System Call: Forward input to TA\n");
      invoke_call(sess, input, input_size);
      break;
    case MODE_SMC:
      /* Trace OPTEE SMC call */
      printf("Trace OPTEE Secure Monitor Call: Forward input to SMC Kernel Module\n");
      smc_call(input, input_size, syscase_flags);
      break;
    default:
      printf("Unknown fuzzing mode %d\n", mode);
  }
}

/*
 * Run test case
 */
void run_test(TEEC_Context *ctx, TEEC_Session *sess, int argc, char **argv)
{
  sc_u_long input_size;
  char *input;

  process_options(argc, argv, &input, &input_size);

  fork_guard(guard_handler);

  if(fuzzing_mode == 0) {
    usage(argv[0]);
  }

  if(is_combined(fuzzing_mode)) {
    syscase_flags |= FLAG_COMBINED;
    // Get test case in combined test case format
    // start_forkserver and get_work is executed by host applicaton
    get_test_case(&input, &input_size, syscase_flags & FLAG_TRACE);
    // Execute combined test case format by responsible agent
    run_combined(ctx, sess, input, input_size, fuzzing_mode);
    done_work(0, syscase_flags & FLAG_TRACE);
    return;
  }

  // Execute single test case format
  // start_forkserver and get_work is executed by responsible agent
  run_case(ctx, sess, input, input_size, fuzzing_mode, fuzzing_mode);
}

/*
 * Trace Linux kernel
 */
void trace_linux_kernel(char *input, sc_u_long input_size)
{
  /* Trace agent */
  extern void _start(), __libc_start_main();

  trace_test_case(
      input,
      input_size,
      (sc_u_long)_start,
      (sc_u_long)__libc_start_main,
      0xffff000000000000L,
      0xffffffffffffffffL,
      syscase_flags
  );
}
