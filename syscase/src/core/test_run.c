#include "syscase/test_run.h"
#include "syscase/afl_call.h"
#include "syscase/common.h"
#include "syscase/test_case.h"
#include "syscase/utils.h"
#include "syscase/json.h"
#include "syscase/ncases.h"

void get_test_case(char** input, sc_u_long* input_size, int trace) {
  start_forkserver(0, trace);
  if (trace) {
    *input = get_work(input_size);
  }

#ifdef SYSCASE_DEBUG
  sc_printf("got work: %lu - %.*s\n", *input_size, (int)*input_size, *input);
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
  struct json_call_t json_calls[NCALLS];
  test_case_t calls[NCALLS];
  int njson_calls;

  struct buffer data[2];
  int parse_result;
  sc_u_long result;
  int trace;
  int smc_driver;

  trace = flags & FLAG_TRACE;
  smc_driver = flags & FLAG_SMC_DRIVER;

  if (!(flags & FLAG_COMBINED)) {
    get_test_case(&input, &input_size, trace);
  } else {

#ifdef SYSCASE_DEBUG
    sc_printf("got case: %lu - %.*x\n", input_size, (int)input_size,
              (unsigned char*)input);
    dump_hex((unsigned char*)input, input_size);
#endif

  }

#ifdef SYSCASE_DEBUG
  dump_hex((unsigned char*)input, input_size);
#endif

  /* Trace parsing */
  start_work(start_parse, end_parse, trace);

#ifdef SYSCASE_DEBUG
  sc_printf("Parse JSON\n");
#endif

  if (parse_json_calls(input, input_size, json_calls, &njson_calls, data) != 0) {
    return -1;
  }

#ifdef SYSCASE_DEBUG
  sc_printf("Parse input\n");
#endif

  parse_result =
      parse_calls(json_calls, njson_calls, &data[1], calls);

  if (parse_result !=0) {
    sc_printf("Parsing failed with result %d\n", parse_result);
    return -1;
  }

#ifdef SYSCASE_DEBUG
  sc_printf("read %ld bytes, parse result %d number of calls %d\n", input_size,
            parse_result, (int)njson_calls);
  sc_printf("Dump %d calls\n", njson_calls);
  dump_test_case(calls, njson_calls);

  sc_printf("Trace System Call\n");
#endif

  start_work(start, end, trace);

  if(!smc_driver) {
    result = execute_test_case(calls, njson_calls);
  } else {
    result = execute_smc_driver(calls, njson_calls, json_calls, data);
  }
  sc_printf("system call result: %ld\n", result);

  if (!(flags & FLAG_COMBINED)) {
    done_work(0, trace);
  }

  return result;
}
