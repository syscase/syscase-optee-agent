#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "agent_client.h"

#include "syscase/afl_call.h"
#include "syscase/test_run.h"
#include "syscase/utils.h"
#include "utils.h"
#include "guard.h"

int syscase_verbose = 1;

int linux_mode = 0;
int optee_mode = 1;
int trace = 1;

TEEC_Result invoke_call(TEEC_Session *sess, char *input, sc_u_long input_size)
{
  uint32_t err_origin;
  TEEC_Operation op;

  // Clear the TEEC_Operation struct
  memset(&op, 0, sizeof(op));

  // Prepare arguments
  op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_MEMREF_TEMP_INPUT,
                    TEEC_NONE, TEEC_NONE);

  op.params[0].value.a = trace;
  if(!trace) {
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
  exit(1);
}

void process_options(int argc, char **argv, char **input, sc_u_long *input_size)
{
  int opt;
  while((opt = getopt (argc, argv, "i:tLO")) != -1) {
    switch(opt) {
      case 'i':
        read_file(optarg, input, input_size);
      case 't':
        trace = 0;
        break;
      case 'O':
        optee_mode = 1;
        linux_mode= 0;
        break;
      case 'L':
        optee_mode = 0;
        linux_mode= 1;
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
  done_work(0, trace);
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

  if(linux_mode == 1) {
    trace_linux_kernel(input, input_size);
  }
  else {
    /* Trace OPTEE Core */
    printf("Trace OPTEE System Call: Forward input to TA\n");
    invoke_call(sess, input, input_size);
  }
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
      trace
  );
}
