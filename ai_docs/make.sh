#!/usr/bin/env bash
set -euo pipefail

target="${1:-motorstepstocartesiantest_quadratic.cpp}"
g++ -g -fconcepts -I../../../hangprinter-flex-compensation -I. ../../../hangprinter-flex-compensation/flex.cpp ../../../hangprinter-flex-compensation/flex_qp.cpp "$target" && ./a.out
