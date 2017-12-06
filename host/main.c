#include <agent_client.h>

int main(int argc, char *argv[])
{
  TEEC_Context ctx;
  TEEC_Session sess;
  TEEC_UUID uuid = TA_AGENT_UUID;

  initializeContext(&ctx);
  openSession(&ctx, &sess, &uuid);
  invokeCall(&sess);
  closeSession(&sess);
  finalizeContext(&ctx);

  return 0;
}
