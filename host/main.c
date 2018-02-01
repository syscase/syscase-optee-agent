#include <stdio.h>
#include <string.h>

#include "agent_client.h"
#include "afl_call.h"

int main(int argc, char *argv[])
{
  TEEC_UUID uuid = TA_AGENT_UUID;

  startForkserver(0);
  run(&uuid, runTest);

  return 0;
}
