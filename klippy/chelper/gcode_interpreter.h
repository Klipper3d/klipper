// G-code interpreter public interface
//
// The interpreter ingests parsed statements and generates rows of raw gcode
// (pure text with all interpreted constructs removed).  Callers can register
// functions to perform (possibly recursive) environmental lookup.
//
// Copyright (C) 2019 Greg Lauckhart <greg@lauckhart.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#ifndef __GCODE_INTERPRETER_H
#define __GCODE_INTERPRETER_H

#include "gcode_ast.h"
#include "gcode_error.h"
#include "gcode_val.h"

typedef struct GCodeInterpreter GCodeInterpreter;

// Execution output types
typedef enum exec_result_type_t {
    GCODE_RESULT_UNKNOWN,
    GCODE_RESULT_ERROR,
    GCODE_RESULT_COMMAND
} exec_result_type_t;

// Output for RESULT_COMMAND
typedef struct GCodeCommand {
    const char* name;
    const char** parameters;
    size_t count;
} GCodeCommand;

// Execution output
typedef struct GCodeResult {
    exec_result_type_t type;
    union {
        const GCodeError* error;
        GCodeCommand command;
    };
} GCodeResult;

// Instantiate a new interpreter.  All callbacks should return false on error.
//
// Args:
//     context - opaque handled used in all callbacks
//     lookup - dictionary lookup callback, handles foo.bar and foo["bar"].
//         If the child is not found, the callback should leave the result type
//         as GCODE_VAL_UNKNOWN and return true.  Strings should live the
//         lifetime of the current statement; the char* functions below return
//         strings that meet this criteria
//     serialize - callback for serializing dicts
//
// Returns the new interpreter or NULL on OOM.
GCodeInterpreter* gcode_interp_new(
    void* context,
    bool (*lookup)(void* context, const GCodeVal* key, dict_handle_t parent,
                   GCodeVal* result),
    const char* (*serialize)(void* context, dict_handle_t dict)
);

// Allocate space on the interpreter string buffer.  No memory management is
// necessary for these strings but they only persist for the execution of a
// single statement.  Adds an extra '\0' byte to terminate the string.
//
// Args:
//     interp - the interpreter
//     size - # of bytes allocated (excluding '\0' terminator)
//
// Returns a new string or NULL on fatal error.
char* gcode_interp_str_alloc(GCodeInterpreter* interp, size_t size);

// Allocate a new string on the intepreter string buffer and fill using printf.
//
// Args:
//     interp - the interpreter
//     format - print format
//     ... printf inputs
//
// Returns the new string or NULL on fatal error.
const char* gcode_interp_printf(GCodeInterpreter* interp, const char* format,
                                ...);

// Convert a GCodeVal to text.
//
// Args:
//     interp - the interpreter
//     val - the value to serialize
//
// Returns the stringified value or NULL on fatal error.
const char* gcode_str_cast(GCodeInterpreter* interp, const GCodeVal* val);

// Convert a GCodeVal to int64_t.
//
// Args:
//     interp - the interpreter
//     val - the value to convert
//
// Returns the integer value.  Cannot fail.
int64_t gcode_int_cast(const GCodeVal* val);

// Convert a GCodeVal to bool.
//
// Args:
//     interp - the interpreter
//     val - the value to convert
//
// Returns the bool value.  Cannot fail.
bool gcode_bool_cast(const GCodeVal* val);

// Convert a GCodeVal to double.
//
// Args:
//     interp - the interpreter
//     val - the value to convert
//
// Returns the double value.  Cannot fail.
double gcode_float_cast(const GCodeVal* val);

// Pass statements to the G-Code interpreter for execution.  Error handling
// occurs entirely via the interpreter error callback.
//
// Args:
//     interp - the interpreter
//     statement - one or more statements chained via ->next
//
// Returns result of execution, valid until next call.
GCodeResult* gcode_interp_exec(GCodeInterpreter* interp,
                               const GCodeStatementNode* statement);

// Release all resources associated with an interpreter.
//
// Args:
//     interp - the interpreter
//
void gcode_interp_delete(GCodeInterpreter* interp);

#endif
