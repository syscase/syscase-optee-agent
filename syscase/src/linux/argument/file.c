#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "syscase/linux/argument/file.h"
#include "syscase/parse_state.h"

#include "syscase/cli/globals.h"

int parse_argument_file(struct buffer* buffer,
                        struct parse_state* state,
                        sc_u_int64_t* value) {
  static int n = 0;
  char filename[128];
  int fd;

  if (state->buffer_pos >= state->nbuffers)
    return -1;
  sc_size_t pos = state->buffer_pos++;
  struct buffer* tmp_buffer = state->buffers + pos;

  snprintf(filename, sizeof filename - 1, "/tmp/file%d", n++);
  fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0777);
  if (fd == -1 ||
      write(fd, buffer_pos(tmp_buffer), buffer_size(tmp_buffer)) == -1 ||
      lseek(fd, 0, SEEK_SET) == -1) {
    perror(filename);
    exit(1);
  }
  // Set file permissions if file priviously existed
  fchmod(fd, 0777);
  *value = fd;
  if (syscase_verbose)
    printf("argFile %llx - %ld bytes from %s\n", (unsigned long long)*value,
           (u_long)buffer_size(tmp_buffer), filename);

  dump_buffer(buffer_pos(tmp_buffer), buffer_size(tmp_buffer));
  return 0;
}
