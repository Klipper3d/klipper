#!/usr/bin/env python3
# Check that declared cmd_XXX_params schemas stay in sync with the
# gcmd.get()/get_int()/get_float()/get_boolean() calls in the matching
# cmd_XXX handler.
#
# This is a proof-of-concept static check for an opt-in convention: a
# class may declare "cmd_FOO_params = {...}" next to "def cmd_FOO(self,
# gcmd)" to describe FOO's parameters (see klippy/gcode.py's
# register_command()/register_mux_command() "params=" argument, and
# klippy/extras/heaters.py for example declarations). Modules that don't
# use the convention are skipped entirely - this only checks commands
# that have opted in.
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import ast, os, sys

GCMD_GETTERS = ('get', 'get_int', 'get_float', 'get_boolean')

def literal_first_arg(call):
    if not call.args:
        return None
    arg = call.args[0]
    if isinstance(arg, ast.Constant) and isinstance(arg.value, str):
        return arg.value
    return None

def gcmd_param_name(func_node):
    # By convention handlers are "def cmd_XXX(self, gcmd)" - find the
    # actual parameter name so calls on unrelated objects that merely
    # happen to also have a get()/get_int()/etc method (eg, a plain
    # dict from get_start_args()) aren't mistaken for gcmd calls.
    args = func_node.args.args
    if len(args) < 2:
        return None
    return args[1].arg

def find_used_params(func_node):
    used = set()
    dynamic = False
    gcmd_name = gcmd_param_name(func_node)
    if gcmd_name is None:
        return used, True
    for node in ast.walk(func_node):
        if not (isinstance(node, ast.Call)
                and isinstance(node.func, ast.Attribute)
                and node.func.attr in GCMD_GETTERS
                and isinstance(node.func.value, ast.Name)
                and node.func.value.id == gcmd_name):
            continue
        name = literal_first_arg(node)
        if name is None:
            # Non-literal first argument (eg, a loop over axes) - the
            # schema can't be verified for this call, so don't report
            # false positives for it.
            dynamic = True
            continue
        used.add(name)
    return used, dynamic

def check_class(filename, class_node):
    errors = []
    param_specs = {}
    cmd_methods = {}
    for node in class_node.body:
        if (isinstance(node, ast.Assign) and len(node.targets) == 1
                and isinstance(node.targets[0], ast.Name)
                and node.targets[0].id.startswith('cmd_')
                and node.targets[0].id.endswith('_params')):
            cmd_name = node.targets[0].id[len('cmd_'):-len('_params')]
            try:
                param_specs[cmd_name] = ast.literal_eval(node.value)
            except ValueError:
                errors.append("%s:%d: cmd_%s_params is not a literal dict"
                              % (filename, node.lineno, cmd_name))
        elif (isinstance(node, ast.FunctionDef)
              and node.name.startswith('cmd_')):
            cmd_methods[node.name[len('cmd_'):]] = node
    for cmd_name, params in param_specs.items():
        func_node = cmd_methods.get(cmd_name)
        if func_node is None:
            errors.append(
                "%s: cmd_%s_params declared but no cmd_%s handler found"
                % (filename, cmd_name, cmd_name))
            continue
        declared = set(params)
        used, dynamic = find_used_params(func_node)
        undeclared = used - declared
        if undeclared:
            errors.append(
                "%s:%d: cmd_%s reads undeclared param(s): %s"
                % (filename, func_node.lineno, cmd_name,
                   ", ".join(sorted(undeclared))))
        unused = declared - used
        if unused and not dynamic:
            # Not an error - params consumed elsewhere (eg, the mux key
            # of a register_mux_command() is read in gcode.py's
            # _cmd_mux(), not in the handler body) are expected here.
            sys.stderr.write(
                "%s:%d: note: cmd_%s declares param(s) not read in its"
                " own body (may be a mux key, or read elsewhere): %s\n"
                % (filename, func_node.lineno, cmd_name,
                   ", ".join(sorted(unused))))
    return errors

def check_file(filename):
    with open(filename, 'r', encoding='utf-8') as f:
        source = f.read()
    tree = ast.parse(source, filename=filename)
    errors = []
    for node in ast.walk(tree):
        if isinstance(node, ast.ClassDef):
            errors.extend(check_class(filename, node))
    return errors

def main():
    files = sys.argv[1:]
    if not files:
        sys.stderr.write("Usage: %s <file.py> [file.py ...]\n" % (sys.argv[0],))
        sys.exit(-1)
    all_errors = []
    for filename in files:
        if not filename.endswith('.py'):
            continue
        all_errors.extend(check_file(filename))
    if all_errors:
        sys.stderr.write("\nERROR: cmd_XXX_params check failed:\n")
        for err in all_errors:
            sys.stderr.write("%s\n" % (err,))
        sys.exit(-1)

if __name__ == '__main__':
    main()
