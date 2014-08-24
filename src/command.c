// Code for default command console.
//
// Copyright (C) 2014  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU LGPLv3 license.

#include <ctype.h> // isspace
#include <stdarg.h> // va_start
#include <stdio.h> // vsnprintf
#include <stdlib.h> // strtod
#include <string.h> // strcasecmp
#include "board/pgm.h" // READP
#include "board/serial.h" // serial_push
#include "command.h" // output_P
#include "sched.h" // DECL_TASK


/****************************************************************
 * Output generation
 ****************************************************************/

void
voutput_P(const char *fmt_P, va_list args)
{
    // XXX - implement local vprintf?
    char output[80];
    vsnprintf_P(output, sizeof(output), fmt_P, args);
    output[sizeof(output)-1] = '\0';

    char *p = output;
    while (*p) {
        serial_push(*p++);
    }
}

// Write a line to the board serial port.  Uses printf syntax.
void
output_P(const char *fmt_P, ...)
{
    va_list args;
    va_start(args, fmt_P);
    voutput_P(fmt_P, args);
    va_end(args);
}

// Return a final response to the console.
void
command_respond_P(struct command *cmd, const char *fmt_P, ...)
{
    if (cmd->flags & CF_RESPONDED)
        return;
    cmd->flags |= CF_RESPONDED;
    va_list args;
    va_start(args, fmt_P);
    voutput_P(fmt_P, args);
    va_end(args);
}

// Return the standard "ok" response to the console.
void
command_respond_ok(struct command *cmd)
{
    command_respond(cmd, "ok\n");
}


/****************************************************************
 * Command argument parsing
 ****************************************************************/

static void
arg_parse_P(const char *str, const char *fmt_P, va_list args)
{
    char *s = (char*)fmt_P;
    for (;; s++) {
        char c = READP(*s);
        if (!c)
            break;
        // Skip white space
        if (!isalpha(c))
            continue;
        // Read gcode to scan for.
        uint8_t gcode = c;
        char *n = s+1;
        c = READP(*n);
        // Read % char.
        if (c != '%')
            continue;
        // Read modifier flags.
        uint8_t width = 0;
        n++;
        c = READP(*n);
        if (c == 'l') {
            width = 4;
            n++;
            c = READP(*n);
        }
        if (c == 'h') {
            width = 2;
            n++;
            c = READP(*n);
        }
        if (c == 'h') {
            width = 1;
            n++;
            c = READP(*n);
        }
        // Read parse action.
        uint8_t action = c;
        if (action != 'd' && action != 'f')
            continue;
        void *arg = va_arg(args, void*);
        // Find argument in input.
        char *p = strchr(str, gcode);
        if (!p)
            continue;
        p++;
        // Parse input.
        if (action == 'd') {
            switch (width) {
            case 0:
                *((int*)arg) = atoi(p);
                break;
            case 1:
                *((char*)arg) = atoi(p);
                break;
            case 2:
                *((short*)arg) = atoi(p);
                break;
            case 4:
                *((long*)arg) = atoi(p);
                break;
            }
        } else {
            *((float*)arg) = strtod(p, NULL);
        }
    }
}

// Parse the arguments of a command.  Uses sscanf like syntax.
void
command_parse_P(struct command *cmd, const char *fmt_P, ...)
{
    va_list args;
    va_start(args, fmt_P);
    arg_parse_P(cmd->cmd, fmt_P, args);
    va_end(args);
}


/****************************************************************
 * Command routing
 ****************************************************************/

// Default command handler when no appropriate command found.
static int8_t
command_default(struct command *cmd)
{
    command_respond(cmd, "Unknown command: %s\n", cmd);
    return 0;
}

// Default command handler when machine is in shutdown state.
static int8_t
command_in_shutdown(struct command *cmd)
{
    command_respond(cmd, "Machine is shutdown, can not process command\n");
    return 0;
}

// Find command handler (that was defined with DECL_COMMAND)
static void *
lookup_handler(struct command *cmd)
{
    // Extract command
    char *p = cmd->cmd;
    while (*p && isspace(*p))
        p++;
    char *c = p;
    while (*p && !isspace(*p))
        p++;
    *p = '\0';
    // Find associated handler.
    struct command_handler *hdl;
    foreachdecl(hdl, commandfuncs) {
        const char *hcmd = READP(hdl->cmd);
        if (strcasecmp_P(c, hcmd) == 0) {
            // Found a match.
            *p = ' ';
            if (sched_is_shutdown() && !(READP(hdl->flags) & HF_IN_SHUTDOWN))
                return command_in_shutdown;
            return READP(hdl->func);
        }
    }
    *p = ' ';
    return command_default;
}

// Process a complete command.
int8_t
command_process(struct command *cmd)
{
    // Find and run handler.
    int8_t (*func)(struct command *) = lookup_handler(cmd);
    int8_t ret = func(cmd);
    if (ret == CR_RETRY) {
        // Try running this command again.
        cmd->flags |= CF_REPEAT;
        return ret;
    }
    if (ret)
        command_respond(cmd, "Error processing command.");
    else
        command_respond_ok(cmd);
    cmd->len = cmd->flags = 0;
    return ret;
}

// Check if a command is ready to run, and run it if so.
int8_t
command_check_run(struct command *cmd)
{
    if (!cmd->len || cmd->cmd[cmd->len-1] != '\0')
        // Not ready to run.
        return 0;
    // Process command.
    return command_process(cmd);
}

// Add a new character to a command buffer being built.
void
command_push_char(struct command *cmd, char c)
{
    if (isspace(c)) {
        if (!cmd->len)
            // Skip leading white space.
            return;
        if (c == '\n') {
            // Command complete.
            cmd->cmd[cmd->len++] = '\0';
            return;
        }
        if (isspace(cmd->cmd[cmd->len-1]))
            // Ignore repetitive white space.
            return;
    }
    if (c == ';' || cmd->flags & CF_COMMENT) {
        // Ignore comments.
        cmd->flags |= CF_COMMENT;
        return;
    }
    if (cmd->len + 2 >= sizeof(cmd->cmd)) {
        shutdown("Command overflow.");
        return;
    }
    cmd->cmd[cmd->len++] = c;
}


/****************************************************************
 * Console reading task
 ****************************************************************/

static struct command console_cmd;

// Background task that reads commands from the board serial port
static void
console_task(void)
{
    for (;;) {
        uint8_t ret = command_check_run(&console_cmd);
        if (ret)
            break;
        if (!serial_canpop())
            break;
        command_push_char(&console_cmd, serial_pop());
    }
}
DECL_TASK(console_task);
