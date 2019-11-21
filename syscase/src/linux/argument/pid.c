#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "syscase/linux/argument/pid.h"
#include "syscase/parse_state.h"

#include "syscase/cli/globals.h"

int fork_test_child(sc_u_int64_t* result_pid) {
  pid_t pid;
  int i;

  fflush(stdout);
  pid = fork();
  switch (pid) {
    case -1:
      return -1;
    case 0:
      break;
    default:
      *result_pid = pid;
      return 0;
  }

  // child
  for (i = 0; i < 3; i++)
    sleep(1);
  exit(0);
}

int parse_argument_pid(struct buffer* buffer,
                       struct parse_state* state,
                       sc_u_int64_t* value) {
  unsigned char type;

  if (get_u_int8_t(buffer, &type) == -1)
    return -1;

  switch (type) {
    case 0:  // my pid
      *value = getpid();
      break;
    case 1:  // parent pid
      *value = getppid();
      break;
    case 2:  // child pid
      if (fork_test_child(value) == -1)
        return -1;
      break;
    default:
      return -1;
  }

  if (syscase_verbose)
    printf("argument pid %llx - %d\n", (unsigned long long)*value, type);
  return 0;
}
