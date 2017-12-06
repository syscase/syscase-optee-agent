#include <stdio.h>
#include <string.h>

#include <agent_client.h>

int main(int argc, char *argv[])
{
  TEEC_Context ctx, ctx2;
  TEEC_Session sess, sess2;
  TEEC_UUID uuid = TA_AGENT_UUID;

  printf("Open Context 1\n");
  initializeContext(&ctx);
  printf("Open Context 2\n");
  initializeContext(&ctx2);
  printf("Open Session 1\n");
  openSession(&ctx, &sess, &uuid);
  printf("Open Session 2\n");
  openSession(&ctx2, &sess2, &uuid);
  printf("Invoke Call 1\n");
  invokeCall(&sess);
  printf("Invoke Call 2\n");
  invokeCall(&sess2);
  printf("Close Session 1\n");
  closeSession(&sess);
  printf("Close Session 2\n");
  closeSession(&sess2);
  printf("Finalize Context 1\n");
  finalizeContext(&ctx);
  printf("Finalize Context 2\n");
  finalizeContext(&ctx2);

  return 0;
}
