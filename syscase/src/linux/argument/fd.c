#include <stdio.h>

#include "syscase/linux/argument/fd.h"
#include "syscase/parse_state.h"

int parse_argument_fd(struct buffer* buffer,
                      struct parse_state* state,
                      sc_u_int64_t* value) {
  int fd;
  unsigned short type;

  if (get_u_int16_t(buffer, &type) == -1)
    return -1;
  fd = get_fd(type);
  if (fd == -1)
    return -1;
  *value = fd;
  if (syscase_verbose)
    printf("argument file descriptor %llx - type %d\n",
           (unsigned long long)*value, type);
  return 0;
}
