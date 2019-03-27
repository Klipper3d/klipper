// Simplified interface for C <-> Python, definitions
//
// Copyright (C) 2019 Greg Lauckhart <greg@lauckhart.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#ifndef __GCODE_BRIDGE
#define __GCODE_BRIDGE

#include <stddef.h>
#include <stdbool.h>

typedef struct GCodeQueue GCodeQueue;
typedef struct GCodeExecutor GCodeExecutor;

typedef enum {
    GCODE_PY_EMPTY,
    GCODE_PY_ERROR,
    GCODE_PY_COMMAND
} gcode_py_result_type_t;

typedef struct GCodePyResult {
    gcode_py_result_type_t type;
    const char* error;
    const char* command;
    const char** parameters;
    size_t count;
} GCodePyResult;

typedef struct GCodeVal GCodeVal;

// Callbacks (implemented in Python)
void gcode_python_fatal(void* queue, const char* error);
void gcode_python_m112(void* queue);
void gcode_python_lookup(void* executor, void* dict, const char* key,
                         GCodeVal* result);
char* gcode_python_serialize(void* executor, void* dict);

GCodeQueue* gcode_queue_new(GCodeExecutor* executor);
size_t gcode_queue_parse(GCodeQueue* queue, const char* buf, size_t length);
size_t gcode_queue_parse_finish(GCodeQueue* queue);
size_t gcode_queue_exec_next(GCodeQueue* executor, GCodePyResult* result);
void gcode_queue_delete(GCodeQueue* executor);

GCodeExecutor* gcode_executor_new(void* context);
void gcode_executor_delete(GCodeExecutor* executor);
const char* gcode_executor_str(GCodeExecutor* executor, const char* text);

#endif
