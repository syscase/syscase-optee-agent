#ifndef SYSCASE_OPTEE_ARGUMENT_UTEE_ATTRIBUTE_H
#define SYSCASE_OPTEE_ARGUMENT_UTEE_ATTRIBUTE_H

#define ARG_OPTEE_UTEE_ATTRIBUTE 67

int parse_argument_optee_utee_attribute(struct buffer *buffer, struct parse_state *state, sc_u_int64_t *value);

#endif /*SYSCASE_OPTEE_ARGUMENT_UTEE_ATTRIBUTE_H*/

