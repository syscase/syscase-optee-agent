#include "syscase/parse_handler.h"

#include "syscase/argument/alloc.h"
#include "syscase/argument/buffer.h"
#include "syscase/argument/length.h"
#include "syscase/argument/number.h"
#include "syscase/argument/reference.h"
#include "syscase/argument/vector_32.h"
#include "syscase/argument/vector_64.h"

#include "syscase/optee/argument/time.h"
#include "syscase/optee/argument/utee_attribute.h"
#include "syscase/optee/argument/utee_params.h"
#include "syscase/optee/argument/uuid.h"

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
    case ARG_VECTOR_64:
      return parse_argument_vector_64;
    case ARG_REFERENCE:
      return parse_argument_reference;
    case ARG_VECTOR_32:
      return parse_argument_vector_32;
    case ARG_OPTEE_TIME:
      return parse_argument_optee_time;
    case ARG_OPTEE_UTEE_PARAMS:
      return parse_argument_optee_utee_params;
    case ARG_OPTEE_UUID:
      return parse_argument_optee_uuid;
    case ARG_OPTEE_UTEE_ATTRIBUTE:
      return parse_argument_optee_utee_attribute;
    default:
      return (parse_handler_t)-1;
  }
}
