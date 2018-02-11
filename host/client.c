#include <err.h>
#include <stdio.h>
#include <string.h>

#include "client.h"

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

/*
 * Print message with given text. Appends index of run i, if n > 1.
 */
void printf_parallel(char* text, int n, int i)
{
  if(n==1)
    printf("%s\n", text);
  else
    printf("%s %d/%d\n", text, i+1, n);
}

/*
 * Runs n parallel context's and session's.
 */
void parallel(TEEC_UUID *uuid, int n, void (*callback)(TEEC_Context *ctx, TEEC_Session *sess, int argc, char **argv), int argc, char **argv)
{
  TEEC_Context ctx[n];
  TEEC_Session sess[n];

  for(int i = 0; i < n; i++) {
    printf_parallel("Open Context", n, i);
    initializeContext(&ctx[i]);
  }

  for(int i = 0; i < n; i++) {
    printf_parallel("Open Session", n, i);
    openSession(&ctx[i], &sess[i], uuid);
  }

  for(int i = 0; i < n; i++) {
    printf_parallel("Invoke Callback", n, i);
    callback(&ctx[i], &sess[i], argc, argv);
  }

  for(int i = 0; i < n; i++) {
    printf_parallel("Close Session", n, i);
    closeSession(&sess[i]);
  }

  for(int i = 0; i < n; i++) {
    printf_parallel("Finalize Context", n, i);
    finalizeContext(&ctx[i]);
  }
}

/*
 * Run one context and session. Call callback, when session is opened.
 */
void run(TEEC_UUID *uuid, void (*callback)(TEEC_Context *ctx, TEEC_Session *sess, int argc, char **argv), int argc, char **argv)
{
  parallel(uuid, 1, callback, argc, argv);
}

