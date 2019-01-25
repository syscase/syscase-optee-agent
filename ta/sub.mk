global-incdirs-y += include
global-incdirs-y += ../host/include
global-incdirs-y += ../syscase/vendor/jsmn
global-incdirs-y += ../syscase/include/core
global-incdirs-y += ../syscase/include/optee
srcs-y += agent_ta.c
#srcs-y += vendor/memchr.c
#srcs-y += vendor/memcmp.c
srcs-y += vendor/memmem.c
srcs-y += printf.c
srcs-y += ../syscase/vendor/jsmn/jsmn.c
srcs-y += ../syscase/src/core/afl_call.c
srcs-y += ../syscase/src/core/test_run.c
srcs-y += ../syscase/src/core/buffer.c
srcs-y += ../syscase/src/core/parse_state.c
srcs-y += ../syscase/src/optee/parse_handler.c
srcs-y += ../syscase/src/optee/common.c
srcs-y += ../syscase/src/core/test_case.c
srcs-y += ../syscase/src/core/argument/number.c
srcs-y += ../syscase/src/core/argument/alloc.c
srcs-y += ../syscase/src/core/argument/buffer.c
srcs-y += ../syscase/src/core/argument/length.c
srcs-y += ../syscase/src/core/argument/vector_32.c
srcs-y += ../syscase/src/core/argument/vector_64.c
srcs-y += ../syscase/src/core/argument/reference.c
srcs-y += ../syscase/src/optee/argument/time.c
srcs-y += ../syscase/src/optee/argument/utee_params.c
srcs-y += ../syscase/src/optee/argument/uuid.c
srcs-y += ../syscase/src/optee/argument/utee_attribute.c
srcs-y += ../syscase/src/core/utils.c
# To remove a certain compiler flag, add a line like this
#cflags-template_ta.c-y += -Wno-strict-prototypes
cflags-printf.c-y += -Wno-error=suggest-attribute=format
cflags-../syscase/src/optee/argument/time.c-y += -Wno-error=unused-parameter
cflags-../syscase/src/optee/argument/utee_params.c-y += -Wno-error=unused-parameter
cflags-../syscase/src/optee/argument/uuid.c-y += -Wno-error=unused-parameter
cflags-../syscase/src/optee/argument/utee_attribute.c-y += -Wno-error=unused-parameter
cflags-../syscase/vendor/jsmn/jsmn.c-y += -Wno-error=switch-default
