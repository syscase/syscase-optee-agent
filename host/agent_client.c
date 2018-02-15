#include <err.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "agent_client.h"

#include "syscase/utils.h"
#include "afl_call.h"
#include "guard.h"

int syscase_verbose = 1;

int linux_mode = 0;
int optee_mode = 1;

// TODO: Set to 8, after OPTEE test case definition
int syscase_max_args = 6;

TEEC_Result invokeCall(TEEC_Session *sess, char* input, u_long input_size)
{
  uint32_t err_origin;
  TEEC_Operation op;

  // Clear the TEEC_Operation struct
  memset(&op, 0, sizeof(op));

  // Prepare arguments
  op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_MEMREF_TEMP_INPUT,
                    TEEC_NONE, TEEC_NONE);
  op.params[0].value.a = 43;

	op.params[1].tmpref.buffer = input;
	op.params[1].tmpref.size = input_size;

  // Invoke TA_AGENT_CMD_CALL
  printf("Invoking call with argument: %d\n", op.params[0].value.a);
  TEEC_Result res = TEEC_InvokeCommand(sess, TA_AGENT_CMD_CALL, &op,
                      &err_origin);
  if (res != TEEC_SUCCESS)
    errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
      res, err_origin);
  printf("Receive TA response: %d\n", op.params[0].value.a);

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

void process_options(int argc, char **argv, char **input, u_long *input_size)
{
  int opt;
  while((opt = getopt (argc, argv, "i:tLO")) != -1) {
    switch(opt) {
      case 'i':
        read_file(optarg, input, input_size);
      case 't':
        aflTestMode = 1;
        break;
      case 'O':
        syscase_max_args = 8;
        optee_mode = 1;
        linux_mode= 0;
        break;
      case 'L':
        syscase_max_args = 6;
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
  doneWork(0);
}

/*
 * Run test case
 */
void runTest(TEEC_Context *ctx, TEEC_Session *sess, int argc, char **argv)
{
  u_long input_size;
  char *input;
  struct buffer buffer;
  int parse_result, ncalls;
  test_case_t test_case[3];
  input_size = 0;

  process_options(argc, argv, &input, &input_size);

  fork_guard(guard_handler);
  startForkserver(0);
  if(!input_size)
    input = getWork(&input_size);
  printf("got work: %lu - %.*s\n", input_size, (int) input_size, input);
  dump_hex((unsigned char*) input, input_size);

  /* Trace agent */
  extern void _start(), __libc_start_main();
  startWork((u_long)_start, (u_long)__libc_start_main);

  buffer_from(&buffer, input, input_size);
  parse_result = parse_test_case(&buffer, 3, syscase_max_args, test_case, &ncalls);

  printf("read %ld bytes, parse result %d number of calls %d\n", input_size, parse_result, (int)ncalls);
  // if(parse_result == 0)
  dump_test_case(test_case, ncalls, syscase_max_args);

  if(linux_mode == 1) {
    trace_linux_kernel(test_case, ncalls);
  }
  else {
    /* Trace OPTEE Core */
    printf("Trace OPTEE System Call\n");
    startWork(0xe100000, 0xe143fff);
    invokeCall(sess, input, input_size);
  }

  doneWork(0);
}

/*
 * Trace Linux kernel
 */
void trace_linux_kernel(test_case_t *test_case, int ncalls)
{
  long linux_result;
  printf("Trace Linux System Call\n");
  startWork(0xffff000000000000L, 0xffffffffffffffffL);
  linux_result = execute_linux_test_case(test_case, ncalls);
  printf("system call result: %ld\n", linux_result);
}
