#include <err.h>
#include <stdio.h>
#include <string.h>

#include "client.h"
#include "syscase/types.h"
#include "syscase/test_run.h"
#include "syscase/test_case.h"

#include <stdlib.h>
#include "syscase/afl_call.h"

static void log_to_afl(TEEC_UUID *uuid, uint32_t cmd_id, TEEC_Operation *op) {
  char *buffer;
  const char header[] = "[{\"target\":\"smc_driver\",\"calls\":[[{\"type\":\"buffer\",\"offset\":0,\"size\":16516}]]}]\xb7\xe3";
  size_t tmpref_buffer_size = 4097;
  size_t header_size =  sizeof(header) - 1;
  size_t uuid_size = sizeof(*uuid);
  size_t cmd_id_size = sizeof(uint32_t);
  size_t op_size = sizeof(*op);
  size_t op_params_count = sizeof(op->params)/sizeof(op->params[0]);
  size_t memref_size = tmpref_buffer_size;
  size_t size = header_size + uuid_size + sizeof(uint32_t) + op_size + op_params_count * memref_size + op_params_count * tmpref_buffer_size;
  
#ifdef SYSCASE_DEBUG
  printf("Allocate buffer\n");
#endif

  buffer = malloc(size);
  memset(buffer, 0, size);

#ifdef SYSCASE_DEBUG
  printf("Copy to buffer\n");
#endif

  memcpy(buffer, &header, header_size);
  if (uuid) {
    memcpy(buffer + header_size, uuid, uuid_size);
  }
  memcpy(buffer + header_size + uuid_size, &cmd_id, cmd_id_size);
  if (op) {
    memcpy(buffer + header_size + uuid_size + cmd_id_size, op, op_size);

#ifdef SYSCASE_DEBUG
    printf("Copy params to buffer\n");
#endif

    for(unsigned int i = 0; i < op_params_count; i++) {
      size_t buffer_size = tmpref_buffer_size;

      if (op->params[i].tmpref.size <= tmpref_buffer_size) {
        buffer_size = op->params[i].tmpref.size;

#ifdef SYSCASE_DEBUG
        printf("Source buffer size: %ld\n", op->params[i].tmpref.size);
        printf("Copy buffer size: %ld\n", buffer_size);
#endif

      } else {
        printf("Can not store buffer: Source buffer size %ld > target buffer size %ld\n", op->params[i].tmpref.size, tmpref_buffer_size);
      }

      size_t current_memref_size = memref_size;

      if (op->params[i].memref.size <=memref_size) {
        current_memref_size = op->params[i].memref.size;

#ifdef SYSCASE_DEBUG
        printf("Source memref size: %ld\n", op->params[i].memref.size);
        printf("Copy memref size: %ld\n", current_memref_size);
#endif

      } else {
        printf("Can not store memref: Source memref size %ld > target memref size %ld\n", op->params[i].memref.size, current_memref_size);
      }
      
      if(op->params[i].memref.parent && op->params[i].memref.size > 0) {
#ifdef SYSCASE_DEBUG
        printf("Copy memref\n");
#endif
        memcpy(buffer + header_size + uuid_size + cmd_id_size + op_size + i * memref_size, op->params[i].memref.parent, current_memref_size);
      }

      if(op->params[i].tmpref.buffer && buffer_size > 0) {
#ifdef SYSCASE_DEBUG
        printf("Copy tmpref\n");
#endif

        memcpy(buffer + header_size + uuid_size + cmd_id_size + op_size + op_params_count * memref_size + i * tmpref_buffer_size, op->params[i].tmpref.buffer, buffer_size);
      }
    }
  }

#ifdef SYSCASE_DEBUG
  printf("Write log file\n");
#endif

  log_file(buffer, size);
  free(buffer);

#ifdef SYSCASE_DEBUG
  printf("Finished writing log file\n");
#endif
}

static sc_u_long execute_smc_driver_call(unsigned char* data, sc_u_long data_size) {
  TEEC_Context ctx;
  TEEC_Session sess;
  uint32_t err_origin;
  TEEC_UUID uuid;
  TEEC_Operation op;
  uint32_t cmd_id;
  size_t uuid_size = sizeof(uuid);
  size_t cmd_id_size = sizeof(uint32_t);
  size_t op_size = sizeof(op);
  size_t op_params_count = sizeof(op.params)/sizeof(op.params[0]);
  // Minimum size for each buffer is 1 byte
  size_t fixed_size = uuid_size + cmd_id_size + op_size;
  size_t min_size = fixed_size + 2 * op_params_count;
  if (data_size < min_size) {
    printf("Can not execute smc driver call with: data size %ld < minimal size %ld\n", data_size, min_size);
    return -1;
  }
  size_t tmpref_buffer_size = (data_size - fixed_size) / (2 * op_params_count);
  size_t memref_size = tmpref_buffer_size;

#ifdef SYSCASE_DEBUG
  printf("data size: %ld\n", data_size);
  printf("fixed size: %ld\n", fixed_size);
  printf("total buffer size: %ld\n", data_size - fixed_size);
  printf("tmpref buffer size: %ld\n", tmpref_buffer_size);
  TEEC_UUID uuid_smc = {
	  0x5f8b97df, 0x2d0d, 0x4ad2,
	  {0x98, 0xd2, 0x74, 0xf4, 0x38, 0x27, 0x98, 0xbb},
  };

  printf("TEEC_UUID size: %ld\n", uuid_size);
  printf("TEEC_Operation size: %ld\n", op_size);
  printf("TEEC_SharedMemory size: %ld\n", memref_size);
#endif

  // Clear the TEEC_Operation struct
  memset(&op, 0, op_size);
  
  memcpy(&uuid, data, uuid_size);
  memcpy(&cmd_id, data + uuid_size, cmd_id_size);
  memcpy(&op, data + uuid_size + cmd_id_size, op_size);

#ifdef SYSCASE_DEBUG
  for (int i = 0; i < uuid_size; ++i) {
    printf("%02X ",((unsigned char*) data)[i]);
  }
  printf("TEEC_UUID: %x, %x, %x, { %x, %x, %x, %x, %x, %x, %x , %x }\n", uuid.timeLow, uuid.timeMid, uuid.timeHiAndVersion, uuid.clockSeqAndNode[0], uuid.clockSeqAndNode[1], uuid.clockSeqAndNode[2], uuid.clockSeqAndNode[3], uuid.clockSeqAndNode[4], uuid.clockSeqAndNode[5], uuid.clockSeqAndNode[6], uuid.clockSeqAndNode[7]);
  for (int i = 0; i < sizeof(uuid); ++i) {
    printf("%02X ",((unsigned char*)&uuid)[i]);
  }
  printf("expected TEEC_UUID: %.*x\n", (int) sizeof(uuid_smc), uuid_smc);
  for (int i = 0; i < sizeof(uuid_smc); ++i) {
    printf("%02X ",((unsigned char*)&uuid_smc)[i]);
  }
  printf("\n");
  printf("expected TEEC_UUID: %x, %x, %x, { %x, %x, %x, %x, %x, %x, %x , %x }\n", uuid_smc.timeLow, uuid_smc.timeMid, uuid_smc.timeHiAndVersion, uuid_smc.clockSeqAndNode[0], uuid_smc.clockSeqAndNode[1], uuid_smc.clockSeqAndNode[2], uuid_smc.clockSeqAndNode[3], uuid_smc.clockSeqAndNode[4], uuid_smc.clockSeqAndNode[5], uuid_smc.clockSeqAndNode[6], uuid_smc.clockSeqAndNode[7]);
#endif

  // BINARY: TEEC_UUID | COMMAND ID | TEEC_OPERATION | TEEC_SHARED_MEMORY_0 | .. | TEEC_SHARED_MEMORY_3 | TMPREF_BUFFER_0 | .. | TEMPREF_BUFFER_3
  // LENGTH: 16        | 4          | 112            | max buffer size      | .. | max buffer size      | max buffer size | .. | max buffer size
  // The TEMPREF buffer size and shared memory size are fixed, but they are defined in
  // `op.params[*].tmpref.size` / `op.params[*].memref.size` and get mutated while fuzzing.
  // Maximum buffer size is defined by first argument size (= data size):
  //   buffer size = (argument size - fixed size) / number of buffers

  for(int i = 0; i < op_params_count; i++) {
    op.params[i].memref.parent = (TEEC_SharedMemory *) data + uuid_size + cmd_id_size + op_size + i * memref_size;
    op.params[i].tmpref.buffer = data + uuid_size + cmd_id_size + op_size + + op_params_count * memref_size + i * tmpref_buffer_size;
  }

#ifdef SYSCASE_DEBUG
  printf("Log to AFL\n");
  log_to_afl(&uuid, cmd_id, &op);
#endif

  initializeContext(&ctx);
  openSession(&ctx, &sess, &uuid);

  TEEC_InvokeCommand(&sess, cmd_id, &op, &err_origin);

  closeSession(&sess);
  finalizeContext(&ctx);
  return 0;
}

sc_u_long execute_smc_driver(test_case_t* value,
                             int njson_calls,
                             struct json_call_t* json_calls,
                             struct buffer* data) {
  for(int i = 0; i < njson_calls; i++) {
    if (json_calls[i].size != 1) {
      printf("SMC driver calls should have only one argument!\n");
      return -1;
    }

#ifdef SYSCASE_DEBUG
    printf("Execute driver call with offset: %lu\n", json_calls[i].args[0].offset);
#endif
    execute_smc_driver_call(buffer_pos(&data[1]) + json_calls[i].args[0].offset, json_calls[i].args[0].size);
  }

  return 0;
}
