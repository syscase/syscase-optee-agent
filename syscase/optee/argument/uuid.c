#include "tee_api_types.h"

#include "syscase/parse_state.h"
#include "syscase/common.h"
#include "syscase/optee/argument/uuid.h"

int parse_argument_optee_uuid(struct buffer *buffer, struct parse_state *state, sc_u_int64_t *value)
{
  TEE_UUID *uuid;
  int i;

  uuid = sc_malloc(sizeof(TEE_UUID));
  sc_memset(uuid, 0, sizeof(TEE_UUID));

  if(get_u_int32_t(buffer, &uuid->timeLow) == -1)
    return -1;

  if(get_u_int16_t(buffer, &uuid->timeMid) == -1)
    return -1;

  if(get_u_int16_t(buffer, &uuid->timeHiAndVersion) == -1)
    return -1;

  for(i = 0; i < 8; i++) {
    if(get_u_int8_t(buffer, &uuid->clockSeqAndNode[i]) == -1)
      return -1;
  }

  *value = (sc_u_int64_t)(sc_u_long) uuid;

  if(syscase_verbose)
    sc_printf("argument optee uuid: (%d, %d, %d, ...)\n", uuid->timeLow, uuid->timeMid, uuid->timeHiAndVersion);

  return 0;
}
