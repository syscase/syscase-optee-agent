#include "tee_api_types.h"

#include "syscase/common.h"
#include "syscase/optee/argument/time.h"
#include "syscase/parse_state.h"

int parse_argument_optee_time(struct buffer* buffer,
                              struct parse_state* state,
                              sc_u_int64_t* value) {
  TEE_Time* time;

  time = sc_malloc(sizeof(TEE_Time));
  sc_memset(time, 0, sizeof(TEE_Time));

  if (get_u_int32_t(buffer, &time->seconds) == -1)
    return -1;

  if (get_u_int32_t(buffer, &time->millis) == -1)
    return -1;

  *value = (sc_u_int64_t)(sc_u_long)time;

  if (syscase_verbose)
    sc_printf("argument optee time: %ds %dms\n", time->seconds, time->millis);

  return 0;
}
