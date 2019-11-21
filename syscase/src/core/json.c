#include "syscase/json.h"

#include "syscase/types.h"
#include "syscase/buffer.h"
#include "syscase/common.h"
#include "syscase/ncases.h"
#include "syscase/target.h"
#include "syscase/target.h"
#include "syscase/test_case.h"
#include "syscase/parse_handler.h"
#include "syscase/utils.h"

#include "jsmn.h"

int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  return sc_strncmp(json + tok->start, s, tok->end - tok->start);
}

int target_mode_for(char* s, sc_u_long size) {

#ifdef SYSCASE_DEBUG
  sc_printf("%d, %d\n", size, sizeof(TARGET_OPTEE));
#endif

  if (sizeof(TARGET_LINUX) >= size && sc_strncmp(s, TARGET_LINUX, size) == 0) {
    return MODE_LINUX;
  }
  if (sizeof(TARGET_OPTEE) >= size && sc_strncmp(s, TARGET_OPTEE, size) == 0) {
    return MODE_OPTEE;
  }
  if (sizeof(TARGET_SMC) >= size && sc_strncmp(s, TARGET_SMC, size) == 0) {
    return MODE_SMC;
  }
  if (sizeof(TARGET_SMC_DRIVER) >= size && sc_strncmp(s, TARGET_SMC_DRIVER, size) == 0) {
    return MODE_SMC_DRIVER;
  }
  return -1;
}

int parse_json_cases(char* input, sc_u_long input_size, struct json_case_t* json_cases, int* njson_cases) {
  struct buffer buffer;
  struct buffer data[2];
  int parse_result, json_result, nsegments, i, last_root_index;
  sc_u_long case_data_size;
  char* case_data;

  jsmn_parser parser;
  jsmntok_t tokens[NTOKENS];
  jsmn_init(&parser);

#ifdef SYSCASE_DEBUG
  sc_printf("Create combined input buffer\n");
#endif

  buffer_from(&buffer, input, input_size);

#ifdef SYSCASE_DEBUG
  sc_printf("Parse combined input\n");
#endif

  parse_result = split_binary(&buffer, data, &nsegments);

  if (parse_result == -1){
    sc_printf("Parsing failed\n");
    return -1;
  }

  if (nsegments != 2) {
    sc_printf("Input should have two segments, got %d: JSON_HEADER | BINARY_DELIMITER  | BINARY\n", nsegments);
    return -2;
  }

#ifdef SYSCASE_DEBUG
  sc_printf("read %ld bytes, parse result %d number of segments %d\n", input_size,
            parse_result, (int)nsegments);

  sc_printf("buffer: ");
  dump_hex(buffer_pos(&data[0]), buffer_size(&data[0]));
  sc_printf("meta: %.*s\n", buffer_size(&data[0]), buffer_pos(&data[0]));
#endif

  json_result = jsmn_parse(&parser, (const char *) buffer_pos(&data[0]), buffer_size(&data[0]), tokens, sizeof(tokens)/sizeof(tokens[0]));

  if (json_result < 0) {
    sc_printf("Failed to parse JSON: %d\n", json_result); 
    return -3;
  }

	if (json_result < 1 || tokens[0].type != JSMN_ARRAY) {
		sc_printf("JSON Array expected\n");
		return -4;
	}

#ifdef SYSCASE_DEBUG
  sc_printf("Number of JSON tokens: %d\n", json_result);
#endif

  last_root_index = -1;
  *njson_cases = 0;
  for (i = 1; i < json_result; i++) {
    // Each element with root parent is a test case
    if (tokens[i].parent == 0) {
      last_root_index = i;
      if((*njson_cases) >= NCASES) {
        sc_printf("Can not store more than %d cases. Increase NCASES.\n", NCASES);
        return -6;
      }

#ifdef SYSCASE_DEBUG
      sc_printf("Case data size: %d\n", tokens[i].end-tokens[i].start);
      dump_hex(buffer_pos(&data[1]), buffer_size(&data[1]));
#endif

      case_data_size = tokens[i].end-tokens[i].start + sizeof(BINARY_DELIMITER) - 1 + buffer_size(&data[1]);
      case_data = sc_malloc(case_data_size);
      sc_memset(case_data, 0, case_data_size);
      sc_memcpy(case_data, buffer_pos(&data[0]) + tokens[i].start, tokens[i].end-tokens[i].start);
      sc_memcpy(case_data + tokens[i].end-tokens[i].start, BINARY_DELIMITER, sizeof(BINARY_DELIMITER) - 1);
      sc_memcpy(case_data + tokens[i].end-tokens[i].start + sizeof(BINARY_DELIMITER) - 1, buffer_pos(&data[1]), buffer_size(&data[1]));
      json_cases[*njson_cases].data = case_data;
      json_cases[*njson_cases].size = case_data_size;

#ifdef SYSCASE_DEBUG
      sc_printf("Key: %.*s -- %d -- %d\n", tokens[i].end-tokens[i].start, buffer_pos(&data[0]) + tokens[i].start, tokens[i].size, tokens[i].parent);
#endif

    }
    if (last_root_index != -1 && tokens[i].parent == last_root_index) {
      if (jsoneq((const char *) buffer_pos(&data[0]), &tokens[i], "target") == 0) {
        json_cases[*njson_cases].target = target_mode_for((char *) buffer_pos(&data[0]) + tokens[i+1].start, tokens[i+1].end-tokens[i+1].start);
        if (json_cases[*njson_cases].target == -1) {
            sc_printf("Unknown target\n");
          return -5;
        }

#ifdef SYSCASE_DEBUG
        sc_printf("Target: %.*s\n", tokens[i+1].end-tokens[i+1].start, buffer_pos(&data[0]) + tokens[i+1].start);
#endif

        (*njson_cases)++;
      }
    }
  }
  return 0;
}

int parse_json_calls(char* input, sc_u_long input_size, struct json_call_t* json_calls, int* njson_calls, struct buffer* data) {
  struct buffer buffer;
  int parse_result, json_result, nsegments, i, last_call_index, last_arg_index, call_parent_index, call_index;

  jsmn_parser parser;
  jsmntok_t tokens[NTOKENS];
  jsmn_init(&parser);

#ifdef SYSCASE_DEBUG
  sc_printf("Create combined input buffer\n");
#endif

  buffer_from(&buffer, input, input_size);

#ifdef SYSCASE_DEBUG
  sc_printf("Parse combined input\n");
#endif

  parse_result = split_binary(&buffer, data, &nsegments);

  if (parse_result == -1){
    sc_printf("Parsing failed\n");
    return -1;
  }

  if (nsegments != 2) {
    sc_printf("Input should have two segments, got %d: JSON_HEADER | BINARY_DELIMITER  | BINARY\n", nsegments);
    return -2;
  }

#ifdef SYSCASE_DEBUG
  sc_printf("read %ld bytes, parse result %d number of segments %d\n", input_size,
            parse_result, (int)nsegments);
  sc_printf("buffer: ");
  dump_hex(buffer_pos(&data[0]), buffer_size(&data[0]));
  sc_printf("meta: %.*s\n", buffer_size(&data[0]), buffer_pos(&data[0]));
#endif

  json_result = jsmn_parse(&parser, (const char *) buffer_pos(&data[0]), buffer_size(&data[0]), tokens, sizeof(tokens)/sizeof(tokens[0]));

  if (json_result < 0) {
    sc_printf("Failed to parse JSON: %d\n", json_result); 
    return -3;
  }

  // Extract lines above to new function
	if (json_result < 1 || tokens[0].type != JSMN_OBJECT) {
		sc_printf("JSON Object expected\n");
		return -4;
	}

#ifdef SYSCASE_DEBUG
  sc_printf("Number of JSON tokens: %d\n", json_result);
#endif

  last_call_index = -1;
  last_arg_index = -1;
  call_parent_index = -1;
  call_index = -1;
  *njson_calls = 0;
  for (i = 1; i < json_result; i++) {
    //sc_printf("%d (%d)[%d]: %.*s\n", i, tokens[i].parent, tokens[i].size, tokens[i].end-tokens[i].start, buffer_pos(&data[0]) + tokens[i].start);
    if(call_parent_index != -1) {
      if (tokens[i].parent == call_parent_index) {
        last_call_index = i;

#ifdef SYSCASE_DEBUG
        sc_printf("Set last call index to: %d \n", last_call_index);
#endif

        if(call_index >= NCALLS) {
          sc_printf("Can not store more than %d calls. Increase NCALLS.\n", NCALLS);
          return -6;
        }

        call_index++;
        (*njson_calls)++;
        json_calls[call_index].size = 0;

#ifdef SYSCASE_DEBUG
        sc_printf("Key: %.*s -- %d -- %d\n", tokens[i].end-tokens[i].start, buffer_pos(&data[0]) + tokens[i].start, tokens[i].size, tokens[i].parent);
#endif

      }
    }
    else if (jsoneq((const char *) buffer_pos(&data[0]), &tokens[i], "calls") == 0) {
      call_parent_index = i + 1;

#ifdef SYSCASE_DEBUG
      sc_printf("Set call parent index to: %d\n", call_parent_index);
#endif

    }
    if (last_call_index != -1 && tokens[i].parent == last_call_index) {
      last_arg_index = i;

#ifdef SYSCASE_DEBUG
      sc_printf("Set last arg index to: %d\n", last_arg_index);
#endif

      if(json_calls[call_index].size >= NARGS) {
        sc_printf("Can not store more than %d args. Increase NARGS.\n", NARGS);
        return -7;
      }
      json_calls[call_index].size++;
    }
    if (last_arg_index != -1 && tokens[i].parent == last_arg_index) {
      if (jsoneq((const char *) buffer_pos(&data[0]), &tokens[i], "type") == 0) {
        json_calls[call_index].args[json_calls[call_index].size - 1].type = parse_type_for((char *) buffer_pos(&data[0]) + tokens[i+1].start, tokens[i+1].end-tokens[i+1].start);
        if (json_calls[call_index].args[json_calls[call_index].size - 1].type == -1) {
            sc_printf("Unknown type %.*s\n", tokens[i+1].end-tokens[i + 1].start, buffer_pos(&data[0]) + tokens[i+1].start);
          return -5;
        }

#ifdef SYSCASE_DEBUG
        sc_printf("Parsing argument %d\n", json_calls[call_index].size - 1);
        sc_printf("Type: %.*s\n", tokens[i+1].end-tokens[i+1].start, buffer_pos(&data[0]) + tokens[i+1].start);
        sc_printf("Type ID: %d\n", json_calls[call_index].args[json_calls[call_index].size - 1].type);
#endif

      } else if (jsoneq((const char *) buffer_pos(&data[0]), &tokens[i], "size") == 0) {
        json_calls[call_index].args[json_calls[call_index].size - 1].size = argument_long_for((char *) buffer_pos(&data[0]) + tokens[i+1].start, tokens[i+1].end-tokens[i+1].start);

#ifdef SYSCASE_DEBUG
        sc_printf("Size: %.*s\n", tokens[i+1].end-tokens[i+1].start, buffer_pos(&data[0]) + tokens[i+1].start);
#endif

      } else if (jsoneq((const char *) buffer_pos(&data[0]), &tokens[i], "offset") == 0) {
        json_calls[call_index].args[json_calls[call_index].size - 1].offset = argument_long_for((char *) buffer_pos(&data[0]) + tokens[i+1].start, tokens[i+1].end-tokens[i+1].start);

#ifdef SYSCASE_DEBUG
        sc_printf("Offset: %.*s\n", tokens[i+1].end-tokens[i+1].start, buffer_pos(&data[0]) + tokens[i+1].start);
#endif

      }
    }
  }

  return 0;
}

sc_u_long argument_long_for(char* s, sc_u_long size) {
  char* text;
  sc_u_long result;

  // Create clean null terminated string for strtoul
  text = sc_malloc(size + 1);
  sc_memset(text, 0, size + 1);
  sc_memcpy(text, s, size);
  result = sc_strtoul(text, NULL, 10);

#ifdef SYSCASE_DEBUG
  sc_printf("Parse long: %ld\n", result);
#endif

  sc_free(text);
  return result;
}
