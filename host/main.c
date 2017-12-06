#include <agent_client.h>

int main(int argc, char *argv[])
{
  TEEC_Context ctx, ctx2;
  TEEC_Session sess, sess2;
  TEEC_UUID uuid = TA_AGENT_UUID;

  initializeContext(&ctx);
  initializeContext(&ctx2);
  openSession(&ctx, &sess, &uuid);
  openSession(&ctx2, &sess2, &uuid);
  invokeCall(&sess);
  invokeCall(&sess2);
  closeSession(&sess);
  closeSession(&sess2);
  finalizeContext(&ctx);
  finalizeContext(&ctx2);

  return 0;
}
