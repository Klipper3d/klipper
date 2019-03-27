#!/bin/bash

# Run G-Code tests
#
# We test G-Code using gcode-cli to interpret test.input.gcode and comparing
# the result to test.output.gcode.
#
# Copyright (C) 2019 Greg Lauckhart <greg@lauckhart.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

if [ ! -e out/gcode-cli ]; then
    echo "Must build gcode-cli before testing"
fi

out/gcode-cli test.input.gcode > out/test.result.gcode
RESULT=$?
if [ $RESULT -ne 0 ]; then
    echo "FAIL: gcode-cli returned error"
    exit $RESULT
fi

cmp --silent test.output.gcode out/test.result.gcode
RESULT=$?
if [ $RESULT -ne 0 ]; then
    diff -u --color=auto test.output.gcode out/test.result.gcode
    echo "FAIL: Output deviates from expectation"
    rm out/test.result.gcode
    exit $RESULT
fi

exit 0
