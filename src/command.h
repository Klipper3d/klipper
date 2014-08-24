#ifndef __COMMAND_H
#define __COMMAND_H

#include <stdarg.h> // va_list
#include <stdint.h>
#include "board/pgm.h" // PROGMEM
#include "compiler.h" // __section

// Declare a function to run when the specified command is received
#define DECL_COMMAND(CMD, FUNC) \
    _DECL_COMMAND(CMD, FUNC, 0)
#define DECL_COMMAND_FLAGS(CMD, FUNC, FLAGS) \
    _DECL_COMMAND(CMD, FUNC, FLAGS)

// Flags for command handler declarations.
#define HF_IN_SHUTDOWN   0x01   // Handler can run even when in emergency stop

// Flags that a command handler can return.
#define CR_OK           0x00
#define CR_RETRY        0x01

// Structure for storing and passing of a command request.
#define MAX_COMMAND 80
struct command {
    char cmd[MAX_COMMAND];
    uint8_t len;
    uint8_t flags;
};

// struct command flags
#define CF_REPEAT       0x01
#define CF_COMMENT      0x02
#define CF_RESPONDED    0x04

// command.c
void voutput_P(const char *fmt_P, va_list args);
#define output(fmt, args...) \
    output_P(PSTR(fmt) , ##args )
void output_P(const char *fmt_P, ...)
    __attribute__ ((format (printf, 1, 2)));
#define command_respond(cmd, fmt, args...) \
    command_respond_P(cmd, PSTR(fmt) , ##args)
void command_respond_P(struct command *cmd, const char *fmt_P, ...);
void command_respond_ok(struct command *cmd);
#define command_parse(str, fmt, args...) \
    command_parse_P(str, PSTR(fmt) , ##args)
void command_parse_P(struct command *cmd, const char *fmt_P, ...)
    __attribute__ ((format (scanf, 2, 3)));

// Compiler glue for DECL_COMMAND macros above.
struct command_handler {
    const char *cmd;
    uint8_t flags;
    int8_t (*func)(struct command *);
};
#define _DECL_COMMAND(CMD, FUNC, FLAGS)                                 \
    static const char __PASTE(_DECLS_ ## FUNC ## _, __LINE__) [] PROGMEM = CMD; \
    const struct command_handler __PASTE(_DECL_ ## FUNC ## _, __LINE__) \
        __visible __section(".progmem.data.commandfuncs")               \
        __aligned(__alignof__(struct command_handler)) = {              \
        .cmd = __PASTE(_DECLS_ ## FUNC ## _, __LINE__),                 \
        .flags = (FLAGS), .func = FUNC }

#endif // command.h
