// Simplified interface for C <-> Python, implementation
//
// Manages the parser and interpreter.  Queues statements and statement-related
// errors in a ring buffer.  Ring entries execute when Python logic invokes
// gcode_bridge_exec_next.
//
// Managing the queue in C allows us avoid expensive Python logic during timing
// sensitive loops in gcode.py.
//
// Copyright (C) 2019 Greg Lauckhart <greg@lauckhart.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "gcode_bridge.h"
#include "gcode_parser.h"
#include "gcode_interpreter.h"

// A single queue entry
typedef struct RingEntry {
    bool is_error;
    bool need_free;
    union {
        GCodeStatementNode* statement;
        char* error;
    };
} RingEntry;

// Encapsulates a single parsing context
struct GCodeQueue {
    // Public (used by CFFI)
    size_t size;

    // Private
    GCodeExecutor* executor;
    GCodeParser* parser;

    RingEntry* ring;
    size_t ring_pos;
    size_t ring_size;
    RingEntry* last_response;
};

// Encapsulates global interpreter context
struct GCodeExecutor {
    // Private
    void* context;
    GCodeInterpreter* interp;
};

static void ring_entry_free(RingEntry* entry) {
    if (entry->need_free) {
        if (entry->is_error)
            free(entry->error);
        else
            gcode_node_delete((GCodeNode*)entry->statement);
    }
}

static void ring_add(GCodeQueue* queue, RingEntry entry) {
    // Grow ring if full
    if (queue->size == queue->ring_size) {
        size_t ring_size = 2 * queue->ring_size;
        RingEntry* new_ring = realloc(queue->ring,
                                      2 * ring_size * sizeof(RingEntry));
        if (!new_ring) {
            gcode_python_fatal(queue->executor->context,
                               "Out of memory (ring_add)");
            ring_entry_free(&entry);
            return;
        }

        // Must move entries before current position into new slots
        for (size_t i = 0; i < queue->ring_pos; i++)
            new_ring[queue->ring_size + i] = new_ring[i];

        queue->ring = new_ring;
        queue->ring_size = ring_size;
    }
    size_t slot = (queue->ring_pos + queue->size) % queue->ring_size;
    queue->ring[slot] = entry;
    queue->size++;
}

static inline void free_last_response(GCodeQueue* queue) {
    if (queue->last_response) {
        ring_entry_free(queue->last_response);
        queue->last_response = NULL;
    }
}

void gcode_queue_delete(GCodeQueue* queue) {
    free_last_response(queue);
    if (queue->ring)
        for (size_t i = queue->ring_pos; i < queue->ring_pos + queue->size; i++)
            ring_entry_free(&queue->ring[i % queue->ring_size]);
    free(queue->ring);
    free(queue);
}

static void parse_error(void* context, const GCodeError* error) {
    GCodeQueue* queue = context;
    RingEntry e = { true, true, .error = strdup(gcode_error_get(error)) };
    if (!e.error) {
        e.need_free = false;
        e.error = "Out of memory (parse_error)";
    }
    ring_add(queue, e);
}

static bool parse_statement(void* context, GCodeStatementNode* statement) {
    GCodeQueue* queue = context;
    RingEntry e = { false, true, .statement = statement };
    ring_add(queue, e);
    if (!strcmp(statement->command, "M112"))
        gcode_python_m112(queue->executor->context);
    return true;
}

GCodeQueue* gcode_queue_new(GCodeExecutor* executor) {
    GCodeQueue* queue = malloc(sizeof(GCodeQueue));
    if (!queue)
        return NULL;

    queue->executor = executor;
    queue->parser = gcode_parser_new(queue, parse_error, parse_statement);

    queue->ring_size = 32;
    queue->ring = malloc(queue->ring_size * sizeof(RingEntry));

    if (!queue->parser || !queue->ring) {
        gcode_queue_delete(queue);
        return NULL;
    }

    queue->executor = executor;
    queue->size = 0;
    queue->ring_pos = 0;
    queue->ring = calloc(queue->ring_size, sizeof(RingEntry));
    queue->parser = gcode_parser_new(queue, parse_error, parse_statement);
    queue->last_response = NULL;

    return queue;
}

size_t gcode_queue_parse(GCodeQueue* queue, const char* buffer, size_t length) {
    free_last_response(queue);
    gcode_parser_parse(queue->parser, buffer, length);
    return queue->size;
}

size_t gcode_queue_parse_finish(GCodeQueue* queue) {
    free_last_response(queue);
    gcode_parser_finish(queue->parser);
    return queue->size;
}

size_t gcode_queue_exec_next(GCodeQueue* queue, GCodePyResult* result) {
    free_last_response(queue);
    if (!queue->size) {
        result->type = GCODE_PY_EMPTY;
        return 0;
    }

    size_t slot = queue->ring_pos % queue->ring_size;
    RingEntry* entry = &queue->ring[slot];

    if (entry->is_error) {
        result->type = GCODE_PY_ERROR;
        result->error = entry->error;
    } else {
        GCodeResult* gcode_result = gcode_interp_exec(queue->executor->interp,
                                                      entry->statement);
        switch (gcode_result->type) {
            case GCODE_RESULT_UNKNOWN:
                result->type = GCODE_PY_ERROR;
                result->error = "Internal: No result from gcode_interp_exec";
                break;

            case GCODE_RESULT_ERROR:
                result->type = GCODE_PY_ERROR;
                result->error = gcode_error_get(gcode_result->error);
                break;

            case GCODE_RESULT_COMMAND:
                result->type = GCODE_PY_COMMAND;
                result->command = gcode_result->command.name;
                result->parameters = gcode_result->command.parameters;
                result->count = gcode_result->command.count;
                break;
        }
    }

    queue->last_response = &queue->ring[slot];
    queue->size--;
    if (++queue->ring_pos == queue->ring_size)
        queue->ring_pos = 0;

    return queue->size;
}

static bool interp_lookup(void* context, const GCodeVal* key,
                          dict_handle_t parent, GCodeVal* result)
{
    GCodeExecutor* executor = context;
    const char* key_str = gcode_str_cast(executor->interp, key);
    if (key_str)
        gcode_python_lookup(executor->context, parent, key_str, result);
    return true;
}

static const char* interp_serialize(void* context, dict_handle_t dict) {
    GCodeExecutor* executor = context;
    return gcode_python_serialize(executor->context, dict);
}

GCodeExecutor* gcode_executor_new(void* context) {
    GCodeExecutor* executor = malloc(sizeof(GCodeExecutor));
    if (!executor)
        return NULL;
    executor->context = context;
    executor->interp = gcode_interp_new(executor, interp_lookup,
                                        interp_serialize);
    if (!executor->interp)
        gcode_executor_delete(executor);
    return executor;
}

void gcode_executor_delete(GCodeExecutor* executor) {
    gcode_interp_delete(executor->interp);
    free(executor);
}

const char* gcode_executor_str(GCodeExecutor* executor, const char* text) {
    size_t length = strlen(text);
    char* new_str = gcode_interp_str_alloc(executor->interp, length);
    strncpy(new_str, text, length);
    return new_str;
}
