#include <linux/slab.h>
#include <linux/string.h>
#include <linux/mm.h>

#include "syscase/parse_state.h"
#include "syscase/common.h"
#include "syscase/optee/smc/argument/msg.h"

static int shm_store(struct optee_msg_arg *msg, sc_u_int64_t *paddr)
{
  size_t msg_size;
  unsigned int order;
  struct page *page;
  sc_u_int64_t kaddr;

  msg_size = sizeof(struct optee_msg_arg);
  order = get_order(msg_size);

  page = alloc_pages(GFP_KERNEL | __GFP_ZERO, order);
  if(!page)
    return -ENOMEM;

  // optee_msg_arg addr
  kaddr = (sc_u_int64_t) page_address(page);
  memcpy((void *) kaddr, msg, msg_size);
	*paddr = page_to_phys(page);
	// size = PAGE_SIZE << order;

  return 0;
}

int parse_argument_optee_msg(struct buffer *buffer, struct parse_state *state, sc_u_int64_t *value)
{
  struct optee_msg_arg msg;
  sc_u_int64_t paddr = 0;

  int i;

  if(get_u_int32_t(buffer, &msg.cmd) == -1)
    return -1;

  if(get_u_int32_t(buffer, &msg.func) == -1)
    return -1;

  if(get_u_int32_t(buffer, &msg.session) == -1)
    return -1;

  if(get_u_int32_t(buffer, &msg.cancel_id) == -1)
    return -1;

  if(get_u_int32_t(buffer, &msg.pad) == -1)
    return -1;

  if(get_u_int32_t(buffer, &msg.ret) == -1)
    return -1;

  if(get_u_int32_t(buffer, &msg.ret_origin) == -1)
    return -1;

  if(get_u_int32_t(buffer, &msg.num_params) == -1)
    return -1;

  if(msg.num_params > NPARAMS) {
    msg.num_params = NPARAMS;
  }

  for(i = 0; i < msg.num_params; i++) {
    if(get_u_int64_t(buffer, &msg.params[i].attr) == -1)
      return -1;

    if(get_u_int64_t(buffer, &msg.params[i].u.tmem.buf_ptr) == -1)
      return -1;
    if(get_u_int64_t(buffer, &msg.params[i].u.tmem.size) == -1)
      return -1;
    if(get_u_int64_t(buffer, &msg.params[i].u.tmem.shm_ref) == -1)
      return -1;

    if(get_u_int64_t(buffer, &msg.params[i].u.rmem.offs) == -1)
      return -1;
    if(get_u_int64_t(buffer, &msg.params[i].u.rmem.size) == -1)
      return -1;
    if(get_u_int64_t(buffer, &msg.params[i].u.rmem.shm_ref) == -1)
      return -1;

    if(get_u_int64_t(buffer, &msg.params[i].u.value.a) == -1)
      return -1;
    if(get_u_int64_t(buffer, &msg.params[i].u.value.b) == -1)
      return -1;
    if(get_u_int64_t(buffer, &msg.params[i].u.value.c) == -1)
      return -1;
  }

  shm_store(&msg, &paddr);

  *value = paddr;

  if(syscase_verbose)
    sc_printf("argument optee msg %lx - cmd %x\n", paddr, msg.cmd);
  return 0;
}

