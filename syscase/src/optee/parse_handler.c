#include "syscase/parse_handler.h"

#include "syscase/common.h"

#include "syscase/argument/alloc.h"
#include "syscase/argument/buffer.h"
#include "syscase/argument/number.h"
//#include "syscase/optee/argument/time.h"
//#include "syscase/optee/argument/utee_attribute.h"
//#include "syscase/optee/argument/utee_params.h"
//#include "syscase/optee/argument/uuid.h"

parse_handler_t get_parse_handler(int type) {
  switch (type) {
    case ARG_TYPE_ID_NUMBER:
      return parse_argument_number;
    case ARG_TYPE_ID_BUFFER:
      return parse_argument_buffer;
    case ARG_TYPE_ID_ALLOC:
      return parse_argument_alloc;
  //case ARG_OPTEE_TIME:
  //  return parse_argument_optee_time;
  //case ARG_OPTEE_UTEE_PARAMS:
  //  return parse_argument_optee_utee_params;
  //case ARG_OPTEE_UUID:
  //  return parse_argument_optee_uuid;
  //case ARG_OPTEE_UTEE_ATTRIBUTE:
  //  return parse_argument_optee_utee_attribute;
    default:
      return parse_argument_number;
  }

  return (parse_handler_t)-1;
}

int parse_type_for(char* s, sc_u_long size) {
  if (sizeof(ARG_TYPE_NUMBER) >= size && sc_strncmp(s, ARG_TYPE_NUMBER, size) == 0) {
    return ARG_TYPE_ID_NUMBER;
  }
  if (sizeof(ARG_TYPE_BUFFER) >= size && sc_strncmp(s, ARG_TYPE_BUFFER, size) == 0) {
    return ARG_TYPE_ID_BUFFER;
  }
  if (sizeof(ARG_TYPE_ALLOC) >= size && sc_strncmp(s, ARG_TYPE_ALLOC, size) == 0) {
    return ARG_TYPE_ID_ALLOC;
  }
  return -1;
}
