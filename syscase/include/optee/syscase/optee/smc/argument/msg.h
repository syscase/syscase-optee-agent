#ifndef SYSCASE_OPTEE_SMC_ARGUMENT_MSG_H
#define SYSCASE_OPTEE_SMC_ARGUMENT_MSG_H

#include "syscase/optee/smc/argument/msg_type.h"

#include "syscase/parse_state.h"
#include "syscase/types.h"

struct sc_optee_msg_param_tmem {
  sc_u_int64_t buf_ptr;
  sc_u_int64_t size;
  sc_u_int64_t shm_ref;
};

struct sc_optee_msg_param_rmem {
  sc_u_int64_t offs;
  sc_u_int64_t size;
  sc_u_int64_t shm_ref;
};

struct sc_optee_msg_param_value {
  sc_u_int64_t a;
  sc_u_int64_t b;
  sc_u_int64_t c;
};

struct sc_optee_msg_param {
  sc_u_int64_t attr;
  union {
    struct sc_optee_msg_param_tmem tmem;
    struct sc_optee_msg_param_rmem rmem;
    struct sc_optee_msg_param_value value;
  } u;
};

struct optee_msg_arg {
  sc_u_int32_t cmd;
  sc_u_int32_t func;
  sc_u_int32_t session;
  sc_u_int32_t cancel_id;
  sc_u_int32_t pad;
  sc_u_int32_t ret;
  sc_u_int32_t ret_origin;
  sc_u_int32_t num_params;

  struct sc_optee_msg_param params[NPARAMS];
};

int parse_argument_optee_msg(struct buffer* buffer,
                             struct parse_state* state,
                             sc_u_int64_t* value);

#endif /*SYSCASE_OPTEE_SMC_ARGUMENT_MSG_H*/
