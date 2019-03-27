// G-code abstract syntax tree implementation
//
// Copyright (C) 2019 Greg Lauckhart <greg@lauckhart.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "gcode_ast.h"

GCodeNode* gcode_parameter_new(const char* name) {
    if (!name)
        return NULL;
    size_t l = strlen(name);
    GCodeParameterNode* n = malloc(sizeof(GCodeParameterNode) + l + 1);
    if (!n)
        return NULL;
    n->type = GCODE_PARAMETER;
    n->next = NULL;
    n->name = (char*)(n + 1);
    strncpy((char*)(n + 1), name, l + 1);
    return (GCodeNode*)n;
}

GCodeNode* gcode_bool_new(bool value) {
    GCodeBoolNode* n = malloc(sizeof(GCodeBoolNode));
    if (!n)
        return NULL;
    n->type = GCODE_BOOL;
    n->next = NULL;
    n->value = value;
    return (GCodeNode*)n;
}

GCodeNode* gcode_int_new(int64_t value) {
    GCodeIntNode* n = malloc(sizeof(GCodeIntNode));
    if (!n)
        return NULL;
    n->type = GCODE_INT;
    n->next = NULL;
    n->value = value;
    return (GCodeNode*)n;
}

GCodeNode* gcode_float_new(double value) {
    GCodeFloatNode* n = malloc(sizeof(GCodeFloatNode));
    if (!n)
        return NULL;
    n->type = GCODE_FLOAT;
    n->next = NULL;
    n->value = value;
    return (GCodeNode*)n;
}

GCodeNode* gcode_operator_new(gcode_operator_type_t type,
                              GCodeNode* children)
{
    GCodeOperatorNode* n = malloc(sizeof(GCodeOperatorNode));
    if (!n)
        return NULL;
    n->type = GCODE_OPERATOR;
    n->next = NULL;
    n->operator = type;
    n->children = children;
    return (GCodeNode*)n;
}

GCodeNode* gcode_function_new(const char* name, GCodeNode* children) {
    if (!name)
        return NULL;
    size_t l = strlen(name);
    GCodeFunctionNode* n = malloc(sizeof(GCodeFunctionNode) + l + 1);
    if (!n)
        return NULL;
    n->type = GCODE_FUNCTION;
    n->next = NULL;
    n->name = (char*)(n + 1);
    strncpy((char*)(n + 1), name, l + 1);
    n->children = children;
    return (GCodeNode*)n;
}

GCodeNode* gcode_add_next(GCodeNode* sibling, GCodeNode* child) {
    if (!sibling)
        return child;
    while (sibling->next)
        sibling = sibling->next;
    sibling->next = child;
    return sibling;
}

GCodeNode* gcode_add_child(GCodeNode* parent, GCodeNode* child) {
    if (!parent || !child || !gcode_is_parent_node(parent)) {
        gcode_node_delete(child);
        return parent;
    }
    GCodeParentNode* p = (GCodeParentNode*)parent;
    if (!p->children) {
        p->children = child;
        return parent;
    }
    GCodeNode* n = p->children;
    while (n->next)
        n = n->next;
    n->next = child;
    return parent;
}

void gcode_node_delete(GCodeNode* node) {
    if (!node)
        return;
    if (gcode_is_parent_node(node))
        gcode_node_delete(((GCodeOperatorNode*)node)->children);
    gcode_node_delete(node->next);
    free(node);
}
