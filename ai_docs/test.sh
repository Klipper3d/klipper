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

echo "Running solver comparison..."
python3 run_tests.py
