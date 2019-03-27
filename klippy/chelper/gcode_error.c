// G-code error implementation
//
// Copyright (C) 2019 Greg Lauckhart <greg@lauckhart.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "gcode_error.h"

#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

struct GCodeError {
    const char* text;
    char* buffer;
    size_t limit;
    GCodeLocation location;
    void* context;
    void (*emit)(void* context, const GCodeError* error);
};

GCodeError* gcode_error_new(void* context,
                            void (*emit)(void* context,
                                         const GCodeError* error))
{
    GCodeError* error = malloc(sizeof(GCodeError));
    if (!error)
        return NULL;
    error->text = error->buffer = NULL;
    error->limit = 0;
    error->location.first_line = 0;
    error->context = context;
    error->emit = emit;
    return error;
}

// Exand error buffer to contain the designated size
static bool error_alloc(GCodeError* error, size_t length) {
    if (error->limit < length) {
        if (!error->limit)
            error->limit = 256;
        while (error->limit < length)
            error->limit *= 2;
        error->buffer = realloc(error->buffer, error->limit);
        if (!error->buffer) {
            error->text = "Out of memory (gcode_error_emit)";
            return false;
        }
    }
    error->text = error->buffer;
    return true;
}

// vsnprintf with error handling and argp management
static int error_printf(GCodeError* error, const char* format, va_list argp) {
    int rv = vsnprintf(error->buffer, error->limit, format, argp);
    va_end(argp);
    if (rv < 0)
        error->text = "Internal: Failed to format error";
    return rv;
}

void gcode_error_emit(GCodeError* error, const char* format, ...) {
    if (!error_alloc(error, 256))
        return;

    va_list argp;
    va_start(argp, format);
    int rv = error_printf(error, format, argp);

    if (rv > error->limit) {
        if (!error_alloc(error, rv))
            return;

        va_start(argp, format);
        rv = error_printf(error, format, argp);
    }

    if (rv > 0 && error->location.first_line) {
        if (!error_alloc(error, rv + 40))
            return;
        snprintf(error->buffer + rv, 40, " (line %u:%u)",
            error->location.first_line, error->location.first_column);
    }

    error->emit(error->context, error);
}

void gcode_error_forward(GCodeError* error, const GCodeError* other) {
    error->emit(error->context, other);
}

const char* gcode_error_get(const GCodeError* error) {
    return error->text;
}

void gcode_error_delete(GCodeError* error) {
    if (!error)
        return;
    free(error->buffer);
    free(error);
}

void gcode_error_set_location(GCodeError* error,
                              const GCodeLocation* location)
{
    if (location)
        error->location = *location;
}
