#!/bin/bash

# Code generator for Klipper g-code parser
#
# This script generates the lexer symbol table using a combination of bash, sed
# and gperf.  Input files are gcode_parser.y and gcode_parser.generated.c.
#
# Copyright (C) 2019 Greg Lauckhart <greg@lauckhart.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

set -e

if [ ! -x "$(which sed)" ]; then
    echo "Error: Install sed to generate lexer symbol table" 1>&2
    exit 2
fi

if [ ! -x "$(which sed)" ]; then
    echo "Error: Install gperf to generate lexer symbol table" 1>&2
    exit 2
fi

TAB=$'\t'

mkdir -p out

# Read token strings from bison source file
LINES=$(
    sed -n '/^%token <keyword>/{s/%token <keyword> *//; p}' gcode_parser.y |
        grep '"'
)
declare -A STRS_BY_NAME
while read -r IDENT STR; do
    STRS_BY_NAME[$IDENT]=$STR
done <<< "$LINES"

# Extract bison's symbol IDs; these are the canonical IDs the lexer must share
LINES=$(
    sed -n '/enum yytokentype/{:x;n;/{/bx;/}/b;s/ *TOK_//g;s/=/ /;s/,//;p;bx}' \
    ../gcode_parser.generated.c)
declare -A IDS_BY_NAME
while read -r IDENT ID; do
    IDS_BY_NAME[$IDENT]=$ID
done <<< "$LINES"

# Generate input for gperf which actually generates the symbol table
mkdir -p out
{
    cat <<END
%{
#include <stdlib.h>
#include <string.h>
%}
%define lookup-function-name gcode_keyword_lookup
%struct-type
struct GCodeKeywordDetail { const char* name; int id; };
%%
END
    for KEY in "${!STRS_BY_NAME[@]}"; do
        echo "${STRS_BY_NAME[$KEY]}$TAB${IDS_BY_NAME[$KEY]}"
    done
} > out/gcode_parser.keywords.gperf

gperf -l -e"$TAB" out/gcode_parser.keywords.gperf \
    | grep -v "#line" \
    > ../gcode_keywords.generated.c
