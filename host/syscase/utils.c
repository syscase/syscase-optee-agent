#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "syscase/utils.h"

void read_file(char *filename, char **input, size_t *input_size)
{
  struct stat info;
  if(stat(filename, &info) != 0) {
    printf("Can not read file (stat): %s\n", filename);
    exit(1);
  }
  *input_size = info.st_size;
  
  *input = malloc(*input_size);
  if(*input == NULL) {
    printf("Can not read file (malloc): %s\n", filename);
    exit(1);
  }

  FILE *fp = fopen(filename, "rb");
  if(fp == NULL) {
    printf("Can not read file (fopen): %s\n", filename);
  }

  size_t blocks_read = fread(*input, info.st_size, 1, fp);
  if (blocks_read != 1) {
    printf("Can not read file (fread): %s\n", filename);
    exit(1);
  }

  fclose(fp);
}

void dump_hex(unsigned char *s, size_t size)
{
 for (size_t i = 0; i < size; i ++) {
      printf(" %02x", s[i]);
  }
  putchar('\n');
}

