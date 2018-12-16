CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf

LDOBJS += client.o
LDOBJS += utils.o
LDOBJS += ../syscase/vendor/jsmn/jsmn.o
LDOBJS += ../syscase/afl_call.o
LDOBJS += ../syscase/test_run.o
LDOBJS += ../syscase/buffer.o
LDOBJS += ../syscase/parse_state.o
LDOBJS += ../syscase/linux/parse_handler.o
LDOBJS += ../syscase/linux/common.o
LDOBJS += ../syscase/test_case.o
LDOBJS += ../syscase/argument/number.o
LDOBJS += ../syscase/argument/alloc.o
LDOBJS += ../syscase/argument/buffer.o
LDOBJS += ../syscase/argument/length.o
LDOBJS += ../syscase/argument/vector_32.o
LDOBJS += ../syscase/argument/vector_64.o
LDOBJS += ../syscase/argument/reference.o
LDOBJS += ../syscase/linux/argument/file.o
LDOBJS += ../syscase/linux/argument/file_descriptor.o
LDOBJS += ../syscase/linux/argument/fd.o
LDOBJS += ../syscase/linux/argument/filename.o
LDOBJS += ../syscase/linux/argument/pid.o
LDOBJS += ../syscase/utils.o
LDOBJS += ../smcchar/smc_call.o
LDOBJS += guard.o
LDOBJS += agent_client.o

OBJS = main.o
OBJS += $(LDOBJS)

CFLAGS += -Wall -I../syscase/vendor/jsmn -I../syscase/include -I../smcchar/include -I../ta/include -I$(TEEC_EXPORT)/include -I./include
#Add/link other required libraries here
LDADD += $(LDOBJS) -lteec -L$(TEEC_EXPORT)/lib

BINARY = optee_example_agent

.PHONY: all
all: $(BINARY)

$(BINARY): $(OBJS)
	$(CC) -o $@ $< $(LDADD)

.PHONY: clean
clean:
	rm -f $(OBJS) $(BINARY)
