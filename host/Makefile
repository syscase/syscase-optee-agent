CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf

LDOBJS += client.o
LDOBJS += ../syscase/vendor/jsmn/jsmn.o
LDOBJS += ../syscase/src/core/afl_call.o
LDOBJS += ../syscase/src/core/test_run.o
LDOBJS += ../syscase/src/core/buffer.o
LDOBJS += ../syscase/src/core/parse_state.o
LDOBJS += ../syscase/src/linux/parse_handler.o
LDOBJS += ../syscase/src/linux_common/common.o
LDOBJS += ../syscase/src/core/test_case.o
LDOBJS += ../syscase/src/core/json.o
LDOBJS += ../syscase/src/core/argument/alloc.o
LDOBJS += ../syscase/src/core/argument/buffer.o
LDOBJS += ../syscase/src/core/argument/number.o
LDOBJS += ../syscase/src/core/utils.o
LDOBJS += ../syscase/src/core/cli/usage.o
LDOBJS += ../syscase/src/core/cli/options.o
LDOBJS += ../syscase/src/core/cli/run.o
LDOBJS += ../smcchar/smc_call.o
LDOBJS += execute_smc_driver.o
LDOBJS += agent_client.o

OBJS = main.o
OBJS += $(LDOBJS)

# -D SYSCASE_DEBUG=1 
CFLAGS += -Wall -D JSMN_PARENT_LINKS=1 -I../syscase/vendor/jsmn -I../syscase/include/core -I../syscase/include/linux -I../smcchar/include -I../ta/include -I$(TEEC_EXPORT)/include -I./include
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
