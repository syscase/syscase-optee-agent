#include "syscase/parse_handler.h"

#include "syscase/argument/alloc.h"
#include "syscase/argument/buffer.h"
#include "syscase/argument/length.h"
#include "syscase/argument/number.h"
#include "syscase/argument/reference.h"
#include "syscase/argument/vector_32.h"
#include "syscase/argument/vector_64.h"
#include "syscase/linux/argument/fd.h"
#include "syscase/linux/argument/file.h"
#include "syscase/linux/argument/filename.h"
#include "syscase/linux/argument/pid.h"

parse_handler_t get_parse_handler(unsigned char type) {
  switch (type) {
    case ARG_NUMBER:
      return parse_argument_number;
    case ARG_ALLOC:
      return parse_argument_alloc;
    case ARG_BUFFER:
      return parse_argument_buffer;
    case ARG_LENGTH:
      return parse_argument_length;
    case ARG_FILE:
      return parse_argument_file;
    case ARG_FD:
      return parse_argument_fd;
    case ARG_VECTOR_64:
      return parse_argument_vector_64;
    case ARG_FILENAME:
      return parse_argument_filename;
    case ARG_PID:
      return parse_argument_pid;
    case ARG_REFERENCE:
      return parse_argument_reference;
    case ARG_VECTOR_32:
      return parse_argument_vector_32;
  }

  return (parse_handler_t)-1;
}
