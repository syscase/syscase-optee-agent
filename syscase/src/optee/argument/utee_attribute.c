#include "utee_types.h"

#include "syscase/parse_state.h"
#include "syscase/common.h"
#include "syscase/optee/argument/utee_attribute.h"

int parse_argument_optee_utee_attribute(struct buffer *buffer, struct parse_state *state, sc_u_int64_t *value)
{
  struct utee_attribute *attr;

  attr = sc_malloc(sizeof(struct utee_attribute));
  sc_memset(attr, 0, sizeof(struct utee_attribute));

  if(get_u_int64_t(buffer, &attr->a) == -1)
    return -1;
  
  if(get_u_int64_t(buffer, &attr->b) == -1)
    return -1;

  if(get_u_int32_t(buffer, &attr->attribute_id) == -1)
    return -1;

  *value = (sc_u_int64_t)(sc_u_long) attr;

  if(syscase_verbose)
    sc_printf("argument optee utee attribute: (%ld, %ld, %d)\n", attr->a, attr->b, attr->attribute_id);

  return 0;
}
