#include "syscase/utils.h"
#include "syscase/common.h"

void dump_hex(unsigned char* s, sc_size_t size) {
  for (sc_size_t i = 0; i < size; i++) {
    sc_printf(" %02x", s[i]);
  }
  sc_printf("\n");
}
