#ifndef SYSCASE_OPTEE_ARGUMENT_TIME_H
#define SYSCASE_OPTEE_ARGUMENT_TIME_H

#define ARG_OPTEE_TIME 64

int parse_argument_optee_time(struct buffer *buffer, struct parse_state *state, sc_u_int64_t *value);

#endif /*SYSCASE_OPTEE_ARGUMENT_TIME_H*/

