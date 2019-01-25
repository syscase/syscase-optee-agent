#ifndef SYSCASE_LINUX_ARGUMENT_FD_H
#define SYSCASE_LINUX_ARGUMENT_FD_H

#define ARG_FD 5

int parse_argument_fd(struct buffer *buffer, struct parse_state *state, sc_u_int64_t *value);
int get_fd(int type);

#endif /*SYSCASE_LINUX_ARGUMENT_FD_H*/

