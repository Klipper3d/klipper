// G-code interpreter implementation
//
// Copyright (C) 2019 Greg Lauckhart <greg@lauckhart.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "gcode_interpreter.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>

struct GCodeInterpreter {
    void* context;

    const char** field_buf;
    size_t field_count;
    size_t field_limit;

    char* str_buf;
    size_t str_length;
    size_t str_limit;

    GCodeError* error;
    GCodeResult result;

    bool (*lookup)(void*,  const GCodeVal*, dict_handle_t, GCodeVal*);
    const char* (*serialize)(void*, dict_handle_t);
    bool (*exec)(void*, const char*, const char**, size_t);
};

static void interp_error(void* context, const GCodeError* error) {
    GCodeInterpreter* interp = context;
    interp->result.type = GCODE_RESULT_ERROR;
    interp->result.error = error;
}

GCodeInterpreter* gcode_interp_new(
    void* context,
    bool (*lookup)(void*, const GCodeVal*, dict_handle_t, GCodeVal*),
    const char* (*serialize)(void*, dict_handle_t))
{
    GCodeInterpreter* interp = malloc(sizeof(GCodeInterpreter));
    if (!interp)
        return NULL;

    interp->error = gcode_error_new(interp, interp_error);
    if (!interp->error) {
        free(interp);
        return NULL;
    }

    interp->context = context;
    interp->lookup = lookup;
    interp->serialize = serialize;

    interp->field_buf = NULL;
    interp->field_count = 0;
    interp->field_limit = 0;

    interp->str_buf = NULL;
    interp->str_length = 0;
    interp->str_limit = 0;

    return interp;
}

static bool str_expand(GCodeInterpreter* interp, size_t size) {
    if (interp->str_limit - interp->str_length < size) {
        if (!interp->str_limit)
            interp->str_limit = 512;
        while (interp->str_limit < size)
            interp->str_limit *= 2;
        interp->str_buf = realloc(interp->str_buf, interp->str_limit);
        if (!interp->str_buf) {
            EMIT_ERROR(interp, "Out of memory (str_expand)");
            return false;
        }
    }
    return true;
}

char* gcode_interp_str_alloc(GCodeInterpreter* interp, size_t size) {
    if (!str_expand(interp, size + 1))
        return NULL;
    char* result = interp->str_buf + interp->str_length;
    result[size] = '\0';
    interp->str_length += size + 1;
    return result;
}

const char* gcode_interp_printf(GCodeInterpreter* interp, const char* format,
                                ...)
{
    if (!interp->str_buf && !str_expand(interp, 512))
        return NULL;

    va_list argp;
    va_start(argp, format);
    size_t available = interp->str_limit - interp->str_length;
    int length = vsnprintf(interp->str_buf + interp->str_length,
                           interp->str_limit - interp->str_length,
                           format,
                           argp);
    if (length < 0) {
        va_end(argp);
        EMIT_ERROR(interp, "Internal: GCodeInterpreter printf failure");
        return NULL;
    }

    if (length > available) {
        if (!str_expand(interp, length + 1)) {
            va_end(argp);
            return false;
        }
        length = vsnprintf(interp->str_buf + interp->str_length,
                           length,
                           format,
                           argp);
        if (length < 0) {
            va_end(argp);
            EMIT_ERROR(interp, "Internal: GCodeInterpreter printf failure");
            return NULL;
        }
    }

    va_end(argp);
    return gcode_interp_str_alloc(interp, length);
}

static void reset(GCodeInterpreter* interp) {
    interp->field_count = 0;
    interp->str_length = 0;
}

inline const char* gcode_str_cast(GCodeInterpreter* interp,
                                  const GCodeVal* val)
{
    if (!val)
        return "";

    switch (val->type) {
    case GCODE_VAL_STR:
        return val->str_val;

    case GCODE_VAL_BOOL:
        return val->bool_val ? "true" : "false";

    case GCODE_VAL_INT:
        return gcode_interp_printf(interp, "%" PRIi64, val->int_val);

    case GCODE_VAL_FLOAT:
        return gcode_interp_printf(interp, "%g", val->float_val);

    case GCODE_VAL_DICT:
        if (interp->serialize)
            return interp->serialize(interp->context, val->dict_val);
        else
            return "<obj>";

    default:
        return "";
    }
}

inline int64_t gcode_int_cast(const GCodeVal* val) {
    if (!val)
        return 0;

    switch (val->type) {
    case GCODE_VAL_STR:
        return strtoll(val->str_val, NULL, 0);

    case GCODE_VAL_BOOL:
        return val->bool_val;

    case GCODE_VAL_INT:
        return val->int_val;

    case GCODE_VAL_FLOAT:
        return val->float_val;

    default:
        return 0;
    }
}

inline bool gcode_bool_cast(const GCodeVal* val) {
    if (!val)
        return false;

    switch (val->type) {
    case GCODE_VAL_STR: {
        const char* s = val->str_val;
        if (s[0] == '\0')
            return false;
        if ((s[0] == 'f' || s[0] == 'F')
            && (s[1] == 'a' || s[1] == 'A')
            && (s[2] == 'l' || s[2] == 'L')
            && (s[3] == 's' || s[3] == 'S')
            && (s[4] == 'e' || s[4] == 'E')
        )
            return false;
        for (; *s; s++)
            if ((*s != ' ')
                && (*s != '\t')
                && (*s != '\v')
                && (*s != '\r')
                && (*s != '\n')
                && (*s != '0')
            )
                return true;
        return false;
    }

    case GCODE_VAL_BOOL:
        return val->bool_val;

    case GCODE_VAL_INT:
        return val->int_val;

    case GCODE_VAL_FLOAT:
        return val->float_val;

    default:
        return false;
    }
}

inline double gcode_float_cast(const GCodeVal* val) {
    if (!val)
        return 0;

    switch (val->type) {
    case GCODE_VAL_STR:
        return atof(val->str_val);

    case GCODE_VAL_BOOL:
        return val->bool_val;

    case GCODE_VAL_INT:
        return val->int_val;

    case GCODE_VAL_FLOAT:
        return val->float_val;

    default:
        return true;
    }
}

static bool eval(GCodeInterpreter* interp, const GCodeNode* input,
                 GCodeVal* output);

static inline int compare_floats(const GCodeVal* n1, const GCodeVal* n2) {
    double f1 = gcode_float_cast(n1);
    double f2 = gcode_float_cast(n2);
    if (f1 < f2)
        return -1;
    if (f1 > f2)
        return 1;
    if (f1 == f2)
        return 0;

    // NaN
    return 2;
}

static bool compare(GCodeInterpreter* interp, GCodeVal* a, GCodeVal* b,
                    int8_t* result)
{
    if (!a || !b)
        EMIT_ERROR(interp, "Internal: NULL comparison");

    if (a->type == GCODE_VAL_DICT || b->type == GCODE_VAL_DICT) {
        *result = a->type == b->type && a->dict_val == b->dict_val ? 0 : 2;
        return true;
    }

    switch (a->type) {
    case GCODE_VAL_STR: {
        const char* bstr = gcode_str_cast(interp, b);
        if (!bstr)
            return false;
        *result = strcmp(a->str_val, bstr);
        break;
    }

    case GCODE_VAL_BOOL: {
        bool val = gcode_bool_cast(b);
        *result = a->bool_val < val ? -1 : a->bool_val > val ? 1 : 0;
        break;
    }

    case GCODE_VAL_INT:
        switch (b->type) {
            case GCODE_VAL_STR:
            case GCODE_VAL_FLOAT:
                *result = compare_floats(a, b);
                break;

            default: {
                int bi = gcode_int_cast(b);
                *result = a->int_val < bi ? -1 : a->int_val > bi ? 1 : 0;
                break;
            }
        }
        break;

    case GCODE_VAL_FLOAT:
        *result = compare_floats(a, b);
        break;

    default:
        EMIT_ERROR(interp, "Internal: Comparison of unknown value type %d",
                   a->type);
        return false;
    }
    return true;
}

#define EVALN(node, val) \
    if (!eval(interp, node, val)) \
        return false;

#define EVALX(var) \
    GCodeVal var; \
    EVALN(child, &var); \
    child = gcode_next(child);

#define EVAL1 EVALX(a);
#define EVAL2 EVALX(a); EVALX(b);
#define EVAL3 EVALX(a); EVALX(b); EVALX(c);
#define COMPARE \
    int8_t cval; \
    if (!compare(interp, &a, &b, &cval)) \
        return false;

#define SET_BOOL(test) { \
    output->type = GCODE_VAL_BOOL; \
    output->bool_val = test; \
}

#define BINARY_NUM_OP(int_case, float_case) \
    EVAL2 \
    if (force_to_num2(&a, &b) == GCODE_VAL_INT) { \
        output->type = GCODE_VAL_INT; \
        output->int_val = int_case; \
        break; \
    } else { \
        output->type = GCODE_VAL_FLOAT; \
        output->float_val = float_case; \
        break; \
    }

static inline bool lookup(GCodeInterpreter* interp,
                          const GCodeVal* parent,
                          const GCodeVal* key,
                          GCodeVal* result)
{
    result->type = GCODE_VAL_UNKNOWN;
    if (!parent || parent->type == GCODE_VAL_DICT)
        return interp->lookup(interp->context, key,
                              parent ? parent->dict_val : NULL, result);
    return true;
}

static inline gcode_val_type_t force_to_num(GCodeVal* val) {
    switch (val->type) {
    case GCODE_VAL_INT:
    case GCODE_VAL_FLOAT:
        break;

    case GCODE_VAL_STR:
        val->float_val = gcode_float_cast(val);
        val->type = GCODE_VAL_FLOAT;
        break;

    default:
        val->int_val = gcode_int_cast(val);
        val->type = GCODE_VAL_INT;
        break;
    }
    return val->type;
}

static inline gcode_val_type_t force_to_num2(GCodeVal* a, GCodeVal* b) {
    force_to_num(a);
    if (force_to_num(b) == GCODE_VAL_INT) {
        if (a->type == GCODE_VAL_FLOAT) {
            b->type = GCODE_VAL_FLOAT;
            b->float_val = b->int_val;
            return GCODE_VAL_FLOAT;
        }
        return GCODE_VAL_INT;
    }
    if (a->type == GCODE_VAL_INT) {
        a->type = GCODE_VAL_FLOAT;
        a->float_val = a->int_val;
    }
    return GCODE_VAL_FLOAT;
}

static inline bool eval_operator(GCodeInterpreter* interp,
                                 const GCodeOperatorNode* input,
                                 GCodeVal* output)
{
    const GCodeNode* child = input->children;
    switch (input->operator) {
    case GCODE_AND: {
        EVAL2;
        SET_BOOL(gcode_bool_cast(&a) && gcode_bool_cast(&b));
        break;
    }

    case GCODE_OR: {
        EVAL2;
        SET_BOOL(gcode_bool_cast(&a) || gcode_bool_cast(&b));
        break;
    }

    case GCODE_EQUALS: {
        EVAL2;
        COMPARE;
        SET_BOOL(cval == 0);
        break;
    }

    case GCODE_CONCAT: {
        EVAL2;
        const char* s1 = gcode_str_cast(interp, &a);
        if (!s1)
            return false;
        const char* s2 = gcode_str_cast(interp, &b);
        if (!s2)
            return false;
        int l1 = strlen(s1);
        int l2 = strlen(s2);
        output->type = GCODE_VAL_STR;
        char* s = gcode_interp_str_alloc(interp, l1 + l2);
        if (!s)
            return false;
        memcpy(s, s1, l1);
        memcpy(s + l1, s2, l2);
        s[l1 + l2] = '\0';
        output->str_val = s;
        break;
    }

    case GCODE_ADD: {
        BINARY_NUM_OP(
            a.int_val + b.int_val,
            a.float_val + b.float_val
        );
        break;
    }

    case GCODE_SUBTRACT: {
        BINARY_NUM_OP(
            a.int_val - b.int_val,
            a.float_val - b.float_val
        );
        break;
    }

    case GCODE_MODULUS: {
        BINARY_NUM_OP(
            b.int_val == 0 ? NAN : a.int_val % b.int_val,
            b.float_val == 0 ? NAN : fmod(a.float_val, b.float_val)
        );
        break;
    }

    case GCODE_POWER: {
        BINARY_NUM_OP(
            pow(a.int_val, b.int_val) + .5, // Add .5 for correctness
            pow(a.float_val, b.float_val)
        );
        break;
    }

    case GCODE_MULTIPLY: {
        BINARY_NUM_OP(
            a.int_val * b.int_val,
            a.float_val * b.float_val
        );
        break;
    }

    case GCODE_DIVIDE: {
        EVAL2
        if (force_to_num2(&a, &b) == GCODE_VAL_INT) {
            if (b.int_val == 0) {
                output->type = GCODE_VAL_FLOAT;
                output->float_val = NAN;
            } else {
                output->type = GCODE_VAL_INT;
                output->int_val = a.int_val / b.int_val;
            }
        } else {
            output->type = GCODE_VAL_FLOAT;
            if (b.float_val == 0)
                output->float_val = NAN;
            else
                output->float_val = a.float_val / b.float_val;
            break;
        }
        break;
    }

    case GCODE_LT: {
        EVAL2;
        COMPARE;
        SET_BOOL(cval == -1);
        break;
    }

    case GCODE_GT: {
        EVAL2;
        COMPARE;
        SET_BOOL(cval == 1);
        break;
    }

    case GCODE_LTE: {
        EVAL2;
        COMPARE;
        SET_BOOL(cval == -1 || cval == 0);
        break;
    }

    case GCODE_GTE: {
        EVAL2;
        COMPARE;
        SET_BOOL(cval == 1 || cval == 0);
        break;
    }

    case GCODE_NOT: {
        EVAL1;
        SET_BOOL(!gcode_bool_cast(&a));
        break;
    }

    case GCODE_NEGATE: {
        EVALN(child, output);
        if (force_to_num(output) == GCODE_VAL_INT)
            output->int_val = -output->int_val;
        else
                output->float_val = -output->float_val;
        break;
    }

    case GCODE_IFELSE: {
        const GCodeNode* test = gcode_next(child);
        GCodeVal comp;
        EVALN(test, &comp);
        if (gcode_bool_cast(&comp)) {
            EVALN(child, output);
        } else {
            EVALN(gcode_next(test), output);
        }
        break;
    }

    case GCODE_LOOKUP: {
        EVAL2;
        if (!lookup(interp, &a, &b, output))
            return false;
        if (output->type == GCODE_VAL_UNKNOWN) {
            const char* key = gcode_str_cast(interp, &b);
            if (!key)
                return false;
            EMIT_ERROR(interp, "Undefined property '%s'", key);
            return false;
        }
        break;
    }

    case GCODE_STR_CAST: {
        EVAL1;
        output->type = GCODE_VAL_STR;
        output->str_val = gcode_str_cast(interp, &a);
        break;
    }

    case GCODE_INT_CAST: {
        EVAL1;
        output->type = GCODE_VAL_INT;
        output->int_val = gcode_int_cast(&a);
        break;
    }

    case GCODE_BOOL_CAST: {
        EVAL1;
        output->type = GCODE_VAL_BOOL;
        output->bool_val = gcode_bool_cast(&a);
        break;
    }

    case GCODE_FLOAT_CAST: {
        EVAL1;
        output->type = GCODE_VAL_FLOAT;
        output->float_val = gcode_float_cast(&a);
        break;
    }

    default:
        EMIT_ERROR(interp, "Internal: Unknown operator type %d",
                   input->operator);
        return false;
    }
    return true;
}

static bool eval(GCodeInterpreter* interp, const GCodeNode* input,
                 GCodeVal* output)
{
    if (!input) {
        EMIT_ERROR(interp, "Internal: Eval of NULL expression node");
        return false;
    }
    switch (input->type) {
    case GCODE_STATEMENT:
        EMIT_ERROR(interp, "Internal: Unexpected statement in expression");
        return false;

    case GCODE_PARAMETER: {
        const GCodeVal key = {
            GCODE_VAL_STR,
            .str_val = ((const GCodeParameterNode*)input)->name
        };
        if (!lookup(interp, NULL, &key, output))
            return false;
        if (output->type == GCODE_VAL_UNKNOWN) {
            EMIT_ERROR(interp, "Undefined parameter '%s'", key.str_val);
            return false;
        }
        break;
    }

    case GCODE_STR:
        output->type = GCODE_VAL_STR;
        output->str_val = ((GCodeStrNode*)input)->value;
        break;

    case GCODE_BOOL:
        output->type = GCODE_VAL_BOOL;
        output->bool_val = ((GCodeBoolNode*)input)->value;
        break;

    case GCODE_INT:
        output->type = GCODE_VAL_INT;
        output->int_val = ((GCodeIntNode*)input)->value;
        break;

    case GCODE_FLOAT:
        output->type = GCODE_VAL_FLOAT;
        output->float_val = ((GCodeFloatNode*)input)->value;
        break;

    case GCODE_OPERATOR:
        return eval_operator(interp, (GCodeOperatorNode*)input, output);

    case GCODE_FUNCTION:
        // No functions currently
        EMIT_ERROR(interp, "No such function '%s'",
                   ((GCodeFunctionNode*)input)->name);
        return false;

    default:
        EMIT_ERROR(interp, "Internal: Unknown node type %d", input->type);
        return false;
    }
    return true;
}

static inline bool buffer_field(GCodeInterpreter* interp, const char* text) {
    if (interp->field_count == interp->field_limit) {
        size_t new_limit =
            interp->field_limit ? interp->field_limit * 2 : 16;
        interp->field_buf = realloc(interp->field_buf,
                                    new_limit * sizeof(char*));
        if (!interp->field_buf) {
            interp->field_count = interp->field_limit = 0;
            EMIT_ERROR(interp, "Out of memory (buffer_field)");
            return false;
        }
        interp->field_limit = new_limit;
    }

    interp->field_buf[interp->field_count++] = text;
    return true;
}

GCodeResult* gcode_interp_exec(GCodeInterpreter* interp,
                               const GCodeStatementNode* statement)
{
    reset(interp);

    interp->result.type = GCODE_RESULT_UNKNOWN;

    GCodeVal result_val;
    for (GCodeNode* n = statement->args; n; n = n->next) {
        if (!eval(interp, n, &result_val))
            return &interp->result;
        const char* str = gcode_str_cast(interp, &result_val);
        if (!str)
            return &interp->result;
        if (!buffer_field(interp, str))
            return &interp->result;
    }

    interp->result.type = GCODE_RESULT_COMMAND;
    interp->result.command.name = statement->command;
    interp->result.command.parameters = interp->field_buf;
    interp->result.command.count = interp->field_count;

    return &interp->result;
}

void gcode_interp_delete(GCodeInterpreter* interp) {
    if (!interp)
        return;
    gcode_error_delete(interp->error);
    free(interp->field_buf);
    free(interp->str_buf);
    free(interp);
}
