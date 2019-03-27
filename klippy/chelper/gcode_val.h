// G-code interpreter value type
//
// These definitions support the simple value type used by the G-Code
// interpreter.
//
// Copyright (C) 2019 Greg Lauckhart <greg@lauckhart.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#ifndef __GCODE_VAL_H
#define __GCODE_VAL_H

#include <stdbool.h>
#include <stddef.h>

typedef void* dict_handle_t;

// Type system for G-Code values produced by the interpreter
typedef enum gcode_val_type_t {
    GCODE_VAL_UNKNOWN,
    GCODE_VAL_STR,
    GCODE_VAL_BOOL,
    GCODE_VAL_INT,
    GCODE_VAL_FLOAT,
    GCODE_VAL_DICT
} gcode_val_type_t;

// Each value produced by the interpreter is encoded using this class.
typedef struct GCodeVal {
    gcode_val_type_t type;

    union {
        // An opaque handle used for environmental lookup
        dict_handle_t dict_val;

        // Integers
        int64_t int_val;

        // Floating point numbers
        double float_val;

        // Strings.  Non-static strings should be allocated using one of the
        // interpreter functions that return a char*.  These are allocated in
        // an internal buffer that lives for the lifetime of statement
        // execution.
        const char* str_val;

        // Booleans
        bool bool_val;
    };
} GCodeVal;

#endif
