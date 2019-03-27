// G-code parser public interface
//
// The parser incrementally ingests text and outputs parsed G-code statements.
// Input buffers need not be aligned on newlines.
//
// Output is in the form of GCodeNode lists.  Output statements can be
// evaluated using a GCodeInterpreter.  The recipient must free statements via
// gcode_node_delete.
//
// Copyright (C) 2019 Greg Lauckhart <greg@lauckhart.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#ifndef __GCODE_PARSER_H
#define __GCODE_PARSER_H

#include "gcode_ast.h"
#include "gcode_error.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct GCodeParser GCodeParser;

// Instantiate a new parser.
//
// Args:
//     context - an opaque handle passed to all callbacks
//     error - invoked on fatal error.  Current statement is skipped after
//         invocation
//     statement - invoked for each statement produced; must be freed using
//         gcode_node_delete
//
// Returns the new parser or NULL on fatal error.
GCodeParser* gcode_parser_new(
    void* context,
    void (*error)(void* context, const GCodeError* error),
    bool (*statement)(void* context, GCodeStatementNode* statements)
);

// Parse a string.  Lexical state persists between calls so buffer may
// terminate anywhere in a statement.  Error handling occurs via the parser
// error callback.
//
// Args:
//     parser - the parser
//     buffer - pointer to characters to parse
//     length - length of the buffer
void gcode_parser_parse(GCodeParser* parser, const char* buffer,
                        size_t length);

// Inform parser that no more data is incoming.  Will terminate the last
// statement if no newline is present.
//
// Args:
//     parser - the parser
void gcode_parser_finish(GCodeParser* parser);

// Frees parser resources.
//
// Args:
//     parser - the parser
void gcode_parser_delete(GCodeParser* parser);

#endif
