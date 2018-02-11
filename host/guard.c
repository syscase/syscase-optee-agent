#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "guard.h"

void fork_guard(void (*handler)(void))
{
  int pid;

  if((pid = fork()) == 0)
    return;

  guard(pid, handler);
}

void guard(int pid, void (*handler)(void))
{
  int status;
  waitpid(pid, &status, 0);
  handler();
  exit(0);
}

