// G-code error public interface
//
// Copyright (C) 2019 Greg Lauckhart <greg@lauckhart.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#ifndef __GCODE_ERROR_H
#define __GCODE_ERROR_H

#include <stdint.h>

typedef struct GCodeError GCodeError;
typedef uint32_t gcode_line_t;

// Structure for tracking source locations
typedef struct GCodeLocation {
    uint32_t first_line;
    uint32_t first_column;
    uint32_t last_line;
    uint32_t last_column;
} GCodeLocation;

// Allocate a new error.  Errors can be reused.  They are owned by the caller
// and only valid for a single error callback.
//
// Returns the error or NULL on OOM.
GCodeError* gcode_error_new(
    void* context,
    void (*error)(void* context, const GCodeError* error));

// Triggers a new error using printf() formatting.
//
// Args:
//     error - the error object
//     format - the printf format
//     ... - printf inputs
//
// Always succeeds but error may be "Out of memory" if memory is exhausted.
void gcode_error_emit(GCodeError* error, const char* format, ...);

// Forward another error to this error's handler.
//
// Args:
//     error - the error object
//     other - the error to forward
void gcode_error_forward(GCodeError* error, const GCodeError* other);

// Sets the location for the error.
//
// Args:
//     error - the error object
//     location - the location information
void gcode_error_set_location(GCodeError* error, const GCodeLocation* line);

// Retrieve the error text.
//
// Args:
//     error - the error object
//
// Returns the error text.
const char* gcode_error_get(const GCodeError* error);

// Free error resources.
//
// Args:
//      error - the error object
void gcode_error_delete(GCodeError* error);

#define EMIT_ERROR(source, args...) \
    gcode_error_emit(source->error, args)

#endif
