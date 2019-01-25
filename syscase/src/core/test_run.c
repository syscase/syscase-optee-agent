#include "syscase/test_run.h"
#include "syscase/afl_call.h"
#include "syscase/common.h"
#include "syscase/test_case.h"
#include "syscase/utils.h"

void get_test_case(char** input, sc_u_long* input_size, int trace) {
  start_forkserver(0, trace);
  if (trace) {
    *input = get_work(input_size);
  }
  sc_printf("got work: %lu - %.*s\n", *input_size, (int)*input_size, *input);
#ifdef SYSCASE_DEBUG
  dump_hex((unsigned char*)*input, *input_size);
#endif
}

/*
 * Run test case
 */
sc_u_long trace_test_case(char* input,
                          sc_u_long input_size,
                          sc_u_int64_t start_parse,
                          sc_u_int64_t end_parse,
                          sc_u_int64_t start,
                          sc_u_int64_t end,
                          int flags) {
  struct buffer buffer;
  int parse_result, ncalls;
  test_case_t test_case[3];
  sc_u_long result;
  int trace;

  trace = flags & FLAG_TRACE;

  if (!(flags & FLAG_COMBINED)) {
    get_test_case(&input, &input_size, trace);
  } else {
    sc_printf("got case: %lu - %.*x\n", input_size, (int)input_size,
              (unsigned char*)input);
    dump_hex((unsigned char*)input, input_size);
  }

#ifdef SYSCASE_DEBUG
  dump_hex((unsigned char*)input, input_size);
#endif

  /* Trace parsing */
  start_work(start_parse, end_parse, trace);

  sc_printf("Create buffer\n");
  dump_hex((unsigned char*)input, input_size);
  buffer_from(&buffer, input, input_size);
  sc_printf("Parse input\n");
  parse_result =
      parse_test_case(&buffer, 3, syscase_max_args, test_case, &ncalls);

  if (parse_result == -1) {
    return -1;
  }

  sc_printf("read %ld bytes, parse result %d number of calls %d\n", input_size,
            parse_result, (int)ncalls);
  dump_test_case(test_case, ncalls, syscase_max_args);

  sc_printf("Trace System Call\n");
  start_work(start, end, trace);

  result = execute_test_case(test_case, ncalls);
  sc_printf("system call result: %ld\n", result);

  if (!(flags & FLAG_COMBINED)) {
    done_work(0, trace);
  }

  return result;
}
