#ifndef __COMMAND_H
#define __COMMAND_H

#include <stdarg.h> // va_list
#include <stddef.h> // size_t
#include <stdint.h> // uint8_t
#include "compiler.h" // __section

// Declare a function to run when the specified command is received
#define DECL_COMMAND(FUNC, MSG)                 \
    _DECL_COMMAND(FUNC, 0, MSG)
#define DECL_COMMAND_FLAGS(FUNC, FLAGS, MSG)    \
    _DECL_COMMAND(FUNC, FLAGS, MSG)

// Flags for command handler declarations.
#define HF_IN_SHUTDOWN   0x01   // Handler can run even when in emergency stop

// Declare a constant exported to the host
#define DECL_CONSTANT(NAME, VALUE)              \
    _DECL_CONSTANT(NAME, VALUE)

// Send an output message (and declare a static message type for it)
#define output(FMT, args...)                    \
    _sendf(_DECL_OUTPUT(FMT) , ##args )

// Declare a message type and transmit it.
#define sendf(FMT, args...)                     \
    _sendf(_DECL_PARSER(FMT) , ##args)

// Shut down the machine (also declares a static string to transmit)
#define shutdown(msg)                           \
    sched_shutdown(_DECL_STATIC_STR(msg))
#define try_shutdown(msg)                       \
    sched_try_shutdown(_DECL_STATIC_STR(msg))

// command.c
void _sendf(uint8_t parserid, ...);

// out/compile_time_request.c (auto generated file)
struct command_encoder {
    uint8_t msg_id, max_size, num_params;
    const uint8_t *param_types;
};
struct command_parser {
    uint8_t msg_id, num_args, flags, num_params;
    const uint8_t *param_types;
    void (*func)(uint32_t *args);
};
enum {
    PT_uint32, PT_int32, PT_uint16, PT_int16, PT_byte,
    PT_string, PT_progmem_buffer, PT_buffer,
};
extern const struct command_encoder command_encoders[];
extern const struct command_parser * const command_index[];
extern const uint8_t command_index_size;
extern const uint8_t command_identify_data[];
extern const uint32_t command_identify_size;

// Compiler glue for DECL_COMMAND macros above.
#define _DECL_COMMAND(FUNC, FLAGS, MSG)                                 \
    char __PASTE(_DECLS_ ## FUNC ## _, __LINE__) []                     \
        __visible __section(".compile_time_request")                    \
        = "_DECL_COMMAND " __stringify(FUNC) " " __stringify(FLAGS) " " MSG; \
    void __visible FUNC(uint32_t*)

#define _DECL_CONSTANT(NAME, VALUE)             \
    char __PASTE(_DECLC_ ## NAME ## _, __LINE__) []                     \
        __visible __section(".compile_time_request")                    \
        = "_DECL_CONSTANT " __stringify(NAME) " " __stringify(VALUE)

// Create a compile time request and return a unique (incrementing id)
// for that request.
#define _DECL_REQUEST_ID(REQUEST, ID_SECTION) ({                \
    static char __PASTE(_DECLS_, __LINE__)[]                    \
        __section(".compile_time_request") = REQUEST;           \
    asm volatile("" : : "m"(__PASTE(_DECLS_, __LINE__)));       \
    static char __PASTE(_DECLI_, __LINE__)                      \
        __section(".compile_time_request." ID_SECTION);         \
    (size_t)&__PASTE(_DECLI_, __LINE__); })

#define _DECL_PARSER(FMT)                               \
    _DECL_REQUEST_ID("_DECL_PARSER " FMT, "parsers")

#define _DECL_OUTPUT(FMT)                               \
    _DECL_REQUEST_ID("_DECL_OUTPUT " FMT, "parsers")

#define _DECL_STATIC_STR(FMT)                                   \
    _DECL_REQUEST_ID("_DECL_STATIC_STR " FMT, "static_strings")

#endif // command.h
