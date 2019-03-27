// G-code lexer public interface
//
// This is an incremental single-pass lexer that performs minimal heap
// allocation.  It is used internally by gcode_parser.
//
// Copyright (C) 2019 Greg Lauckhart <greg@lauckhart.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#ifndef __GCODE_LEXER_H
#define __GCODE_LEXER_H

#include "gcode_error.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct GCodeLexer GCodeLexer;
typedef int16_t gcode_keyword_t;

// Instantiate a new lexer.  All callbacks should return false on error.  This
// puts the lexer into error parsing state, where all tokens are ignored until
// end-of-statement.
//
// Args:
//     context - an opaque handle passed to all callbacks
//     location - optional positional tracking
//     error - error emitter.  After invocation, lexing skips to the following
//         line
//     keyword - callback for langauage keywords
//     identifier - callback for non-keyword tokens
//     str_literal - callback for string values
//     int_literal - callback for integer values
//     float_literal - callback for float values
//     bridge - callback for bridge (field concatenation such as x{1})
//     end_statement - callback for end-of-statement (newline)
//
// Returns the new lexer or NULL on fatal error.
GCodeLexer* gcode_lexer_new(
    void* context,
    struct GCodeLocation* location,
    void (*error)(void* context, const GCodeError* error),
    bool (*keyword)(void* context, gcode_keyword_t id),
    bool (*identifier)(void* context, const char* name),
    bool (*str_literal)(void* context, const char* value),
    bool (*int_literal)(void* context, int64_t value),
    bool (*float_literal)(void* context, double value),
    bool (*bridge)(void* context),
    bool (*end_statement)(void* context)
);

// Tokenize a string.  Lexical state persists between calls so buffer may
// terminate anywhere in a statement.  Error handling occurs via the lexer
// error callback.
//
// Args:
//     lexer - the lexer
//     buffer - pointer to characters to scan
//     length - length of the buffer
void gcode_lexer_scan(GCodeLexer* lexer, const char* buffer, size_t length);

// Reset lexical state.  After the call the lexer may be reused.
//
// Args:
//     lexer - the lexer
void gcode_lexer_reset(GCodeLexer* lexer);

// Terminate lexing and trigger remaining callbacks.
//
// Args:
//     lexer - the lexer
void gcode_lexer_finish(GCodeLexer* lexer);

// Release lexer resources
//
// Args:
//     lexer - the lexer
void gcode_lexer_delete(GCodeLexer* lexer);

#endif
