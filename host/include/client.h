#ifndef CLIENT_H
#define CLIENT_H

#include <tee_client_api.h>

TEEC_Result initializeContext(TEEC_Context*);
TEEC_Result openSession(TEEC_Context*, TEEC_Session*, TEEC_UUID*);
void closeSession(TEEC_Session*);
void finalizeContext(TEEC_Context*);
void printf_parallel(char* text, int n, int i);
void parallel(TEEC_UUID *uuid, int n, TEEC_Result (*callback)(TEEC_Context *ctx, TEEC_Session *sess));
void run(TEEC_UUID *uuid, TEEC_Result (*callback)(TEEC_Context *ctx, TEEC_Session *sess));

#endif /*CLIENT_H*/
