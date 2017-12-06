#include <err.h>
#include <stdio.h>
#include <string.h>

#include <client.h>

/*
 * Initialize context
 */
TEEC_Result initializeContext(TEEC_Context *ctx)
{
  TEEC_Result res = TEEC_InitializeContext(NULL, ctx);
  if (res != TEEC_SUCCESS)
    errx(1, "TEEC_InitializeContext failed with code 0x%x", res);
  return res;
}

/*
 * Open session
 */
TEEC_Result openSession(TEEC_Context *ctx, TEEC_Session *sess, TEEC_UUID *uuid)
{
  uint32_t err_origin;
  TEEC_Result res = TEEC_OpenSession(ctx, sess, uuid, TEEC_LOGIN_PUBLIC, NULL,
                      NULL, &err_origin);
  if (res != TEEC_SUCCESS)
    errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x", res,
      err_origin);
  return res;
}

/*
 * Close session
 */
void closeSession(TEEC_Session *sess)
{
  TEEC_CloseSession(sess);
}

/*
 * Finalize context
 */
void finalizeContext(TEEC_Context *ctx)
{
  TEEC_FinalizeContext(ctx);
}

