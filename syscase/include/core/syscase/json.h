#ifndef SYSCASE_JSON_H
#define SYSCASE_JSON_H

#include "syscase/types.h"
#include "syscase/system_call.h"
#include "syscase/buffer.h"
#include "jsmn.h"

#define NTOKENS 4096

struct json_case_t {
  int target;
  sc_u_long size;
  char* data;
};

struct json_arg_t {
  int type;
  sc_u_long size;
  sc_u_long offset;
};

struct json_call_t {
  int size;
  struct json_arg_t args[NARGS];
};

int jsoneq(const char *json, jsmntok_t *tok, const char *s);
int target_mode_for(char* s, sc_u_long size);
int parse_json_cases(char* input, sc_u_long input_size, struct json_case_t* json_cases, int* njson_cases);
int parse_json_calls(char* input, sc_u_long input_size, struct json_call_t* json_calls, int* njson_calls, struct buffer* data);
int argument_type_for(char* s, sc_u_long size);
sc_u_long argument_long_for(char* s, sc_u_long size);

#endif /*SYSCASE_JSON_H*/
