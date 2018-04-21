global-incdirs-y += include
global-incdirs-y += ../host/include
global-incdirs-y += ../syscase/include
srcs-y += agent_ta.c
#srcs-y += vendor/memchr.c
#srcs-y += vendor/memcmp.c
srcs-y += vendor/memmem.c
srcs-y += printf.c
srcs-y += ../syscase/afl_call.c
srcs-y += ../syscase/test_run.c
srcs-y += ../syscase/buffer.c
srcs-y += ../syscase/parse_state.c
srcs-y += ../syscase/optee/parse_handler.c
srcs-y += ../syscase/optee/common.c
srcs-y += ../syscase/test_case.c
srcs-y += ../syscase/argument/number.c
srcs-y += ../syscase/argument/alloc.c
srcs-y += ../syscase/argument/buffer.c
srcs-y += ../syscase/argument/length.c
srcs-y += ../syscase/argument/vector_32.c
srcs-y += ../syscase/argument/vector_64.c
srcs-y += ../syscase/argument/reference.c
srcs-y += ../syscase/optee/argument/time.c
srcs-y += ../syscase/utils.c
# To remove a certain compiler flag, add a line like this
#cflags-template_ta.c-y += -Wno-strict-prototypes
cflags-printf.c-y += -Wno-error=suggest-attribute=format
cflags-../syscase/optee/argument/time.c-y += -Wno-error=unused-parameter
