#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"
mkdir -p build

compile() {
  local src="$1"
  local out="$2"
  echo "Building $out from $src"
  g++ -O2 -std=c++17 -fconcepts -DRUN_DATASET_CLI -I../../../hangprinter-flex-compensation -I. \
    ../../../hangprinter-flex-compensation/flex.cpp ../../../hangprinter-flex-compensation/flex_qp.cpp "$src" -o "build/$out"
}

compile motorstepstocartesiantest_quadratic.cpp solver_quadratic
echo "Building solver_calc_position from solver_calc_position.c"
gcc -O2 -std=gnu11 -D_GNU_SOURCE -I../klippy/chelper solver_calc_position.c -lm -o build/solver_calc_position

echo "Running solver comparison..."
python3 run_tests.py
