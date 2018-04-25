#include "syscase/test_run.h"
#include "syscase/common.h"
#include "syscase/test_case.h"
#include "syscase/afl_call.h"
#include "syscase/utils.h"

/*
 * Run test case
 */
sc_u_long trace_test_case(char *input, sc_u_long input_size, sc_u_int64_t start_parse, sc_u_int64_t end_parse, sc_u_int64_t start, sc_u_int64_t end, int flags)
{
  struct buffer buffer;
  int parse_result, ncalls;
  test_case_t test_case[3];
  sc_u_long result;
  int trace;

  trace = flags & FLAG_TRACE;

  start_forkserver(0, trace);
  if(trace) {
    input = get_work(&input_size);
  }
  sc_printf("got work: %lu - %.*s\n", input_size, (int) input_size, input);
#ifdef SYSCASE_DEBUG
  dump_hex((unsigned char*) input, input_size);
#endif

  /* Trace parsing */
  start_work(start_parse, end_parse, trace);

  sc_printf("Create buffer\n");
  buffer_from(&buffer, input, input_size);
  sc_printf("Parse input\n");
  parse_result = parse_test_case(&buffer, 3, syscase_max_args, test_case, &ncalls);

  sc_printf("read %ld bytes, parse result %d number of calls %d\n", input_size, parse_result, (int)ncalls);
  dump_test_case(test_case, ncalls, syscase_max_args);

  sc_printf("Trace System Call\n");
  start_work(start, end, trace);

  result = execute_test_case(test_case, ncalls);
  sc_printf("system call result: %ld\n", result);
  done_work(0, trace);

  return result;
}
