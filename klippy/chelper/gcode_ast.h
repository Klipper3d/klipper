// G-code abstract syntax tree public interface
//
// This AST is the output type for GCodeParser.
//
// Copyright (C) 2019 Greg Lauckhart <greg@lauckhart.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#ifndef __GCODE_AST_H
#define __GCODE_AST_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// The AST forms a tree with nodes that are differentiated using this type.
typedef enum gcode_node_type_t {
    GCODE_UNKNOWN_NODE,
    GCODE_STATEMENT,
    GCODE_PARAMETER,
    GCODE_STR,
    GCODE_BOOL,
    GCODE_INT,
    GCODE_FLOAT,
    GCODE_OPERATOR,
    GCODE_FUNCTION
} gcode_node_type_t;

// Operator nodes are further differentiated by this subtype.
typedef enum gcode_operator_type_t {
    GCODE_UNKNOWN_OPERATOR,
    GCODE_AND,
    GCODE_OR,
    GCODE_EQUALS,
    GCODE_CONCAT,
    GCODE_ADD,
    GCODE_SUBTRACT,
    GCODE_MODULUS,
    GCODE_POWER,
    GCODE_MULTIPLY,
    GCODE_DIVIDE,
    GCODE_LT,
    GCODE_GT,
    GCODE_LTE,
    GCODE_GTE,
    GCODE_NOT,
    GCODE_NEGATE,
    GCODE_IFELSE,
    GCODE_LOOKUP,
    GCODE_STR_CAST,
    GCODE_INT_CAST,
    GCODE_BOOL_CAST,
    GCODE_FLOAT_CAST
} gcode_operator_type_t;

// The base "class" for all nodes.  All nodes share a type differentiation and
// a pointer that allows them to participate in linked lists..
typedef struct GCodeNode GCodeNode;
typedef struct GCodeNode {
    gcode_node_type_t type;
    GCodeNode* next;
} GCodeNode;

// A base class for nodes that have parent nodes.  Adds a pointer to the first
// child.  Subsequent children are available via child->next..
typedef struct GCodeParentNode {
    gcode_node_type_t type;
    GCodeNode* next;
    GCodeNode* children;
} GCodeParentNode;

// A "statement" is a single line of G-Code.  Arguments are a list of key/value
// pairs.
typedef struct GCodeStatementNode GCodeStatementNode;
struct GCodeStatementNode {
    gcode_node_type_t type;
    GCodeStatementNode* next;
    GCodeNode* args;
    const char* command;
};

// A 'parameter" is a variable input value to a G-Code statement.'
typedef struct GCodeParameterNode {
    gcode_node_type_t type;
    GCodeNode* next;
    const char* name;
} GCodeParameterNode;

// A string literal.  The value* is owned by the node.
typedef struct GCodeStrNode {
    gcode_node_type_t type;
    GCodeNode* next;
    const char* value;
} GCodeStrNode;

// Boolean node.
typedef struct GCodeBoolNode {
    gcode_node_type_t type;
    GCodeNode* next;
    bool value;
} GCodeBoolNode;

// Int node, represented by a 64-bit integer.
typedef struct GCodeIntNode {
    gcode_node_type_t type;
    GCodeNode* next;
    int64_t value;
} GCodeIntNode;

// Floating point node, represented by a C double.
typedef struct GCodeFloatNode {
    gcode_node_type_t type;
    GCodeNode* next;
    double value;
} GCodeFloatNode;

// An operator node.  The operand represented by "operator" applies to children
// available in "children".
typedef struct GCodeOperatorNode {
    gcode_node_type_t type;
    GCodeNode* next;
    GCodeNode* children;
    gcode_operator_type_t operator;
} GCodeOperatorNode;

// A function.  Similar to an operator except the function is identified at
// runtime rather than during parsing.
typedef struct GCodeFunctionNode {
    gcode_node_type_t type;
    GCodeNode* next;
    GCodeNode* children;
    const char* name;
} GCodeFunctionNode;

// Count the number of nodes in a node list starting from the supplied node.
static inline size_t gcode_node_length(const GCodeNode* node) {
    size_t l = 0;
    for (; node; node = node->next)
        l++;
    return l;
}

// Create a new statement node.
//
// Args:
//     command - the name of the statement command (e.g. G1)
//     args - optional list of arguments; ownership is assumed by the
//         statement.  Each child represents a single word in traditional
//         G-Code.
//
// Returns the new node or NULL if OOM.  Caller assumes ownership.
static inline GCodeNode* gcode_statement_new(const char* command,
                                             GCodeNode* args)
{
    size_t l = strlen(command);
    GCodeStatementNode* n = malloc(sizeof(GCodeStatementNode) + l + 1);
    if (!n)
        return NULL;
    n->type = GCODE_STATEMENT;
    n->next = NULL;
    n->args = args;
    n->command = (char*)(n + 1);
    strncpy((char*)(n + 1), command, l + 1);
    return (GCodeNode*)n;
}

// Create a new string node.
//
// Args:
//     value - the string value of the node.  Copied during construction.
//
// Returns the new node or NULL if OOM.
static inline GCodeNode* gcode_str_new(const char* value) {
    if (!value)
        return NULL;
    size_t l = strlen(value);
    GCodeStrNode* n = malloc(sizeof(GCodeStrNode) + l + 1);
    if (!n)
        return NULL;
    n->type = GCODE_STR;
    n->next = NULL;
    n->value = (char*)(n + 1);
    strncpy((char*)(n + 1), value, l + 1);
    return (GCodeNode*)n;
}

// Determine whether a node is a parent node type.  If true, the node can be
// cast to GCodeParentNode.
//
// Args:
//     node - the node to test.  Can be NULL.
//
// Returns true iff the node is a parent node.
static inline bool gcode_is_parent_node(const GCodeNode* node) {
    if (!node)
        return false;
    switch (node->type) {
        case GCODE_FUNCTION:
        case GCODE_OPERATOR:
        case GCODE_STATEMENT:
            return true;

        default:
            return false;
    }
}

// Navigate to the next node.
//
// Args:
//     node - the node to test.  Can be NULL.
static inline const GCodeNode* gcode_next(const GCodeNode* node) {
    if (!node)
        return NULL;
    return node->next;
}

// Instantiate a new parameter node.
//
// Args:
//     name - the parameter name.  Copied during construction.
//
// Returns the node or NULL on OOM.
GCodeNode* gcode_parameter_new(const char* name);

//
// Instantiate a new bool node.
//
// Args:
//     value - the bool value for the node.
//
// Returns the node or NULL on OOM.
//
GCodeNode* gcode_bool_new(bool value);

//
// Instantiate a new integer node.
//
// Args:
//     value - the value for the node
//
// Returns the node or NULL on OOM.
GCodeNode* gcode_int_new(int64_t value);

// Instantiate a new float node.
//
// Args:
//     value - the value for the node.  May represent +/-INF or NAN.
//
// Returns the node or NULL on OOM.
GCodeNode* gcode_float_new(double value);

// Instantiate a new operator node.
//
// Args:
//     type - a member of gcode_operator_type_t
//     children - operator arguments.  The new node assumes ownership.
//
// Returns the node or NULL on OOM.
GCodeNode* gcode_operator_new(gcode_operator_type_t type, GCodeNode* children);

// Instantiate a new function node.
//
// Args:
//     name - the name of the function
//     children - operator arguments.  The new node assumes ownership.
//
// Returns the node or NULL on OOM.
GCodeNode* gcode_function_new(const char* name, GCodeNode* children);

// Add a node to a node list.
//
// Args:
//     sibling - the first node in the list
//     next - the node appended to the end of the list.  The list assumes
//         ownership.
//
// Returns the parent node.  Cannot fail.
GCodeNode* gcode_add_next(GCodeNode* sibling, GCodeNode* next);

// Add a child node to a parent node.
//
// Args:
//     parent - the parent node.  If this is not an actual parent, the child is
//         deleted.
//     child - the child to add.  The parent assumes ownership.
//
// Returns the parent node.  Cannot fail.
GCodeNode* gcode_add_child(GCodeNode* parent, GCodeNode* child);

// Free memory associated with a node and all linked nodes.
//
// Args:
//     node - the node to free.  May be NULL.
void gcode_node_delete(GCodeNode* node);

#endif
