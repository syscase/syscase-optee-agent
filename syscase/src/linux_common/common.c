#define _GNU_SOURCE
#include <string.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

#include "syscase/common.h"
#include "syscase/system_call.h"

int syscase_max_args = 6;

void* sc_malloc(sc_size_t size) {
  return malloc(size);
}

void* sc_memset(void* s, int c, sc_size_t n) {
  return memset(s, c, n);
}

void sc_free(void* ptr) {
  free(ptr);
}

void* sc_memmem(const void* haystack,
                sc_size_t haystacklen,
                const void* needle,
                sc_size_t needlelen) {
  return memmem(haystack, haystacklen, needle, needlelen);
}

int sc_printf(const char* format, ...) {
  int result;
  va_list arglist;
  va_start(arglist, format);
  result = vprintf(format, arglist);
  va_end(arglist);

  return result;
}

unsigned long sc_syscall(struct system_call* value) {
  return syscall(value->no, value->args[0], value->args[1], value->args[2],
                 value->args[3], value->args[4], value->args[5]);
}

void sc_read_file(char* filename, char** input, sc_size_t* input_size) {
  struct stat info;
  if (stat(filename, &info) != 0) {
    printf("Can not read file (stat): %s\n", filename);
    exit(1);
  }
  *input_size = info.st_size;

  *input = malloc(*input_size);
  if (*input == NULL) {
    printf("Can not read file (malloc): %s\n", filename);
    exit(1);
  }

  FILE* fp = fopen(filename, "rb");
  if (fp == NULL) {
    printf("Can not read file (fopen): %s\n", filename);
  }

  sc_size_t blocks_read = fread(*input, info.st_size, 1, fp);
  if (blocks_read != 1) {
    printf("Can not read file (fread): %s\n", filename);
    exit(1);
  }

  fclose(fp);
}

void sc_fork_guard(void (*handler)(void)) {
  int pid;

  if ((pid = fork()) == 0)
    return;

  sc_guard(pid, handler);
}

void sc_guard(int pid, void (*handler)(void)) {
  int status;
  waitpid(pid, &status, 0);
  handler();
  exit(0);
}

void sc_exit(int status) {
  exit(status);
}
