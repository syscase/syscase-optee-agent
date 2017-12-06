#ifndef CLIENT_H
#define CLIENT_H

#include <tee_client_api.h>

TEEC_Result initializeContext(TEEC_Context*);
TEEC_Result openSession(TEEC_Context*, TEEC_Session*, TEEC_UUID*);
void closeSession(TEEC_Session*);
void finalizeContext(TEEC_Context*);

#endif /*CLIENT_H*/
