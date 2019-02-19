#include "syscase/cli/options.h"

#include "syscase/cli/globals.h"
#include "syscase/cli/mode.h"
#include "syscase/cli/usage.h"
#include "syscase/common.h"
#include "syscase/test_run.h"
#include "syscase/types.h"

#include <unistd.h>

void process_options(int argc,
                     char** argv,
                     char** input,
                     sc_u_long* input_size) {
  int opt;
  while ((opt = getopt(argc, argv, "i:tLOS")) != -1) {
    switch (opt) {
      case 'i':
        sc_read_file(optarg, input, input_size);
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
