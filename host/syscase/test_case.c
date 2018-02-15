#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include <sys/stat.h>

#include "syscase/test_case.h"

int push_size(struct parse_state *state, u_int64_t size)
{
  if(state->stack_pos >= SIZE_STACK_SIZE)
    return -1;
  size_t stack_pos = state->stack_pos++;
  state->size_stack[stack_pos] = size;
  return 0;
}

int pop_size(struct parse_state *state, u_int64_t *size)
{
  if(state->stack_pos == 0)
    return -1;
  size_t stack_pos = --state->stack_pos;
  *size = state->size_stack[stack_pos];
  return 0;
}

int parse_argument_number(struct buffer *buffer, struct parse_state *state, u_int64_t *value)
{
  if(get_u_int64_t(buffer, value) == -1)
    return -1;
  if(syscase_verbose)
    printf("argument number %llx\n", (unsigned long long)*value);
  return 0;
}

int parse_argument_alloc(struct buffer *buffer, struct parse_state *state, u_int64_t *value)
{
  void *p;
  u_int32_t size;

  if(get_u_int32_t(buffer, &size) == -1)
    return -1;
  // TODO: Fix memory leak, using fork server for garbage collection
  p = malloc(size);
  if(!p || push_size(state, size) == -1)
      return -1;
  memset(p, 0, size);
  *value = (u_int64_t)(u_long) p;
  if(syscase_verbose)
    printf("argument alloc %llx - allocated %x bytes\n", (unsigned long long)*value, size);
  return 0;
}

int parse_argument_buffer(struct buffer *buffer, struct parse_state *state, u_int64_t *value)
{
  if(state->buffer_pos >= state->nbuffers)
    return -1;
  size_t pos = state->buffer_pos++;
  struct buffer *tmp_buffer = state->buffers + pos;
  size_t size = buffer_size(tmp_buffer);
  if(push_size(state, size) == -1)
    return -1;
  *value = (u_int64_t)(u_long) buffer_pos(tmp_buffer);
  if(syscase_verbose)
    printf("argument buffer %llx from %ld bytes\n", (unsigned long long) *value, size);
  dump_buffer(buffer_pos(tmp_buffer), size);
  return 0;
}

int parse_argument_length(struct buffer *buffer, struct parse_state *state, u_int64_t *value)
{
  if(pop_size(state, value) == -1)
    return -1;
  if(syscase_verbose)
    printf("argument legnth %llx\n", (unsigned long long) *value);
  return 0;
}

int parse_argument_file(struct buffer *buffer, struct parse_state *state, u_int64_t *value)
{
  static int n = 0;
  char filename[128];
  int fd;

  if(state->buffer_pos >= state->nbuffers)
    return -1;
  size_t pos = state->buffer_pos++;
  struct buffer *tmp_buffer = state->buffers + pos;

  snprintf(filename, sizeof filename - 1, "/tmp/file%d", n++);
  fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0777);
  if(fd == -1 || write(fd, buffer_pos(tmp_buffer), buffer_size(tmp_buffer)) == -1
      || lseek(fd, 0, SEEK_SET) == -1) {
    perror(filename);
    exit(1);
  }
  // Set file permissions if file priviously existed
  fchmod(fd, 0777);
  *value = fd;
  if(syscase_verbose)
    printf("argFile %llx - %ld bytes from %s\n", (unsigned long long) *value, (u_long) buffer_size(tmp_buffer), filename);
  
  dump_buffer(buffer_pos(tmp_buffer), buffer_size(tmp_buffer));
  return 0;
}

int parse_argument_fd(struct buffer *buffer, struct parse_state *state, u_int64_t *value)
{
  int fd;
  unsigned short type;

  if(get_u_int16_t(buffer, &type) == -1)
    return -1;
  fd = get_fd(type);
  if(fd == -1)
    return -1;
  *value = fd;
  if(syscase_verbose) printf("argument file descriptor %llx - type %d\n", (unsigned long long) *value, type);
  return 0;
}

int parse_argument_vector_64(struct buffer *buffer, struct parse_state *state, u_int64_t *value)
{
  u_int64_t *vector;
  u_int8_t size;

  if(get_u_int8_t(buffer, &size) == -1)
    return -1;
  // TODO: Fix memory leak, using fork server for garbage collection
  vector = malloc(size * sizeof vector[0]);
  if(size && !vector)
    return -1;
  if(syscase_verbose) printf("argument vector 64 %llx - size %d\n", (unsigned long long)(u_long) vector, size);
  for(int i = 0; i < size; i++) {
    if(syscase_verbose)
      printf("vector %d: ", i);
    if(parse_argument(buffer, state, &vector[i]) == -1)
      return -1;
  }
  if(push_size(state, size) == -1)
      return -1;
  *value = (u_int64_t)(u_long) vector;
  return 0;
}

int parse_argument_filename(struct buffer *buffer, struct parse_state *state, u_int64_t *value)
{
  static int n = 0;
  char filename[128];
  int fd;

  if(state->buffer_pos >= state->nbuffers)
    return -1;
  size_t pos = state->buffer_pos++;
  struct buffer *tmp_buffer = state->buffers + pos;

  snprintf(filename, sizeof filename - 1, "/tmp/file%d", n++);
  fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
  if(fd == -1 || write(fd, buffer_pos(tmp_buffer), buffer_size(tmp_buffer)) == -1
      || close(fd) == -1) {
    perror(filename);
    exit(1);
  }
  // TODO: Fix memory leak, using fork server for garbage collection
  *value = (u_int64_t)(u_long) strdup(filename);
  
  if(syscase_verbose)
    printf("argument filename %llx - %ld bytes from %s\n", (unsigned long long) *value, (u_long) buffer_pos(tmp_buffer), filename);
  dump_buffer(buffer_pos(tmp_buffer), buffer_size(tmp_buffer));
  return 0;
}

int fork_test_child(u_int64_t *result_pid)
{
  pid_t pid;
  int i;

  fflush(stdout);
  pid = fork();
  switch(pid) {
    case -1:
      return -1;
    case 0:
      break;
    default:
      *result_pid = pid;
      return 0;
  }

  // child
  for(i = 0; i < 3; i++)
    sleep(1);
  exit(0);
}

int parse_argument_pid(struct buffer *buffer, struct parse_state *state, u_int64_t *value)
{
  unsigned char type;

  if(get_u_int8_t(buffer, &type) == -1)
    return -1;

  switch(type) {
    case 0: // my pid
      *value = getpid(); 
      break;
    case 1: // parent pid
      *value = getppid(); 
      break;
    case 2: // child pid
      if(fork_test_child(value) == -1)
        return -1;
      break;
    default:
      return -1;
  }

  if(syscase_verbose)
    printf("argument pid %llx - %d\n", (unsigned long long)*value, type);
  return 0;
}

int parse_argument_reference(struct buffer *buffer, struct parse_state *state, u_int64_t *value)
{
  unsigned char call_index, argument_index;

  if(get_u_int8_t(buffer, &call_index) == -1 || get_u_int8_t(buffer, &argument_index) == -1
      || call_index >= state->ncalls
      || argument_index >= 6)
    return -1;

  *value = state->calls[call_index].args[argument_index];

  if(syscase_verbose)
    printf("argument reference %llx - call %d  arg %d\n", (unsigned long long) *value, call_index, argument_index);

  return 0;
}

int parse_argument_vector_32(struct buffer *buffer, struct parse_state *state, u_int64_t *value)
{
  u_int64_t item;
  u_int32_t *vector;
  int i;
  u_int8_t size;

  if(get_u_int8_t(buffer, &size) == -1)
    return -1;
  // TODO: Fix memory leak, using fork server for garbage collection
  vector = malloc(size * sizeof vector[0]);
  if(size && !vector)
    return -1;
  if(syscase_verbose)
    printf("argument_vector 32 %llx - size %d\n", (unsigned long long)(u_long) vector, size);
  for(i = 0; i < size; i++) {
    if(syscase_verbose)
      printf("vector %d: ", i);
    if(parse_argument(buffer, state, &item) == -1)
      return -1;
    vector[i] = item;
  }

  if(push_size(state, size) == -1)
    return -1;
  *value = (u_int64_t)(u_long) vector;
  return 0;
}

parse_handler_t parse_map(unsigned char type)
{
  switch(type) {
    case 0: return parse_argument_number;
    case 1: return parse_argument_alloc;
    case 2: return parse_argument_buffer;
    case 3: return parse_argument_length;
    case 4: return parse_argument_file;
    case 5: return parse_argument_fd;
    case 7: return parse_argument_vector_64;
    case 8: return parse_argument_filename;
    case 9: return parse_argument_pid;
    case 10: return parse_argument_reference;
    case 11: return parse_argument_vector_32;
  }

  return (parse_handler_t) -1;
}

int parse_argument(struct buffer *buffer, struct parse_state *state, u_int64_t *value)
{
  unsigned char type;
  parse_handler_t handler;


  if(get_u_int8_t(buffer, &type) == -1 || (handler = parse_map(type)) == (parse_handler_t) -1)
    return -1;

  return handler(buffer, state, value);
}

int parse_calls(struct system_call *calls, int ncalls, struct buffer *buffer, struct system_call *value)
{
  struct parse_state state;

  if(split_buffer(buffer, BUFFER_DELIMITER, sizeof BUFFER_DELIMITER - 1, NBUFFERS, state.buffers, &state.nbuffers) == -1
      || state.nbuffers < 1)
    return -1;

  buffer = &state.buffers[0];
  state.buffer_pos = 1;
  state.stack_pos = 0;
  state.calls = calls;
  state.ncalls = ncalls;

  if(get_u_int16_t(buffer, &value->no) == -1)
    return -1;

  if(syscase_verbose)
    printf("call %d\n", value->no);

  for(int i = 0; i < 6; i++) {
    if(syscase_verbose)
      printf("arg %d: ", i);
    if(parse_argument(buffer, &state, &value->args[i]) == -1)
      return -1;
  }

  return 0;
}

int parse_test_case(struct buffer *buffer, int max_calls, test_case_t *test_case, int *ncalls)
{
  struct buffer buffers[10];
  size_t nbuffers;

  if(max_calls > 10)
    max_calls = 10;
  if(split_buffer(buffer, CALL_DELIMITER, sizeof CALL_DELIMITER - 1, max_calls, buffers, &nbuffers) == -1)
    return -1;

  for(size_t i = 0; i < nbuffers; i++) {
    if(parse_calls(test_case, i, buffers + i, test_case + i) == -1)
      return -1;
  }

  *ncalls = nbuffers;
  
  return 0;
}

void dump_call(struct system_call *value)
{
  printf(
      "syscall %d (%lx, %lx, %lx, %lx, %lx, %lx)\n",
      value->no,
      (u_long) value->args[0],
      (u_long) value->args[1],
      (u_long) value->args[2],
      (u_long) value->args[3],
      (u_long) value->args[4],
      (u_long) value->args[5]
  );
}

void dump_test_case(test_case_t *value, int n)
{
  int i;

  for(i = 0; i < n; i++)
      dump_call(value + i);
}

unsigned long execute_call(struct system_call *value)
{
  return syscall(
      value->no,
      value->args[0],
      value->args[1],
      value->args[2],
      value->args[3],
      value->args[4],
      value->args[5]
  );
}

unsigned long execute_test_case(test_case_t *value, int n)
{
  unsigned long result;
  int i;

  result = 0;
  for(i = 0; i < n; i++)
    result = execute_call(value + i);
  return result;
}
