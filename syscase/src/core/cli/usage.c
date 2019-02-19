#include "syscase/cli/usage.h"

#include "syscase/common.h"

void usage(char* program) {
  sc_printf("usage:  %s [-tLO] [-i INPUT]\n", program);
  sc_printf("\t\t-t\ttest mode, do not use hypercalls\n");
  sc_printf("\t\t-i\tparse mode, parse given file (implies test mode)\n");
  sc_printf("\t\t-L\tlinux mode, use 6 arguments\n");
  sc_printf("\t\t-O\tOPTEE mode, use 8 arguments\n");
  sc_printf("\t\t-S\tSMC mode, use 8 arguments\n");
  sc_exit(1);
}
