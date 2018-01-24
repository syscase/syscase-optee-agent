#include <stdlib.h>

struct slice {
    unsigned char *cur;
    unsigned char *end;
};

extern int aflTestMode;
int startForkserver(int ticks);
char *getWork(u_long *sizep);
int startWork(u_int64_t start, u_int64_t end);
int doneWork(int val);
