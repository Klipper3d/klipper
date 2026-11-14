# Local development and testing

**English** | [Português brasileiro](pt-BR/Development.md)

Tests do not connect to a printer: heaters, motion and BMCU are simulated.
Parts of the suite run Klipper's real dispatcher, macros, G-code state and
pause/resume. Cutter tests cover all four channels, retraction with a stopped
feeder, buffer, sensor, restart, failures by stage, parking and positioning
with coordinate offsets and print overrides.

## Windows with WSL

Use Ubuntu 22.04 in WSL to reproduce CI. Linux utilities use `pwd`, permissions
and symlinks; do not mock these facilities just to obtain a green result in
Windows Python.

From the clone root inside Ubuntu, prepare the environment once:

```sh
sudo apt-get update &&
sudo apt-get install -y python3-venv python3-dev libffi-dev build-essential &&
python3 -m venv "$HOME/.venvs/ender3-tests" &&
"$HOME/.venvs/ender3-tests/bin/pip" install -r scripts/klippy-requirements.txt
```

Run both suites, still from the clone root:

```sh
"$HOME/.venvs/ender3-tests/bin/python" -B -m unittest discover -s test/bmcu_integracao -p 'test_*.py' &&
"$HOME/.venvs/ender3-tests/bin/python" -B -m unittest discover -s test/e3v3se_display -p 'test_*.py' &&
git diff --check
```

An existing environment may have a different virtualenv path; substitute the
corresponding prefix. Do not install or restart printer services to run local
tests.

The **Build test** workflow also runs Klipper's whitespace checker.
`git diff --check` is not a substitute: it does not enforce the 80-character
Python/C limit. In a clean Linux-filesystem checkout with LF line endings,
a space-free path and no private files, also run:

```sh
bash scripts/check_whitespace.sh
```

Do not publish or remove private files to obtain this checkout; use a separate
public copy. The checker walks directories directly, including ignored files
present in those directories.

## Builds and limitations

The [native Mainsail card guide](Ender3_V3_SE_Mainsail.md#rebuild-and-verify)
covers the pinned web build and local-only browser simulator. Check the real
BMCU 1.1.2 source contract separately; this starts/stops only a synthetic sleeping
process and never opens a serial device:

```sh
git clone --depth 1 --branch v1.1.2 \
  https://github.com/jarczakpawel/BMCU-Klipper.git /path/to/bmcu-upstream
python3 scripts/ender3-v3-se/check_bmcu_contracts.py --source /path/to/bmcu-upstream
```

Firmware compilation also needs `gcc-arm-none-eabi` and
`binutils-arm-none-eabi`. Follow the [build guide](Ender3_V3_SE.md).
The Makefile requires a space-free path; if the Windows clone is inside a
directory with spaces, use a public working copy in the Linux filesystem.
Do not copy private files into public test or build artifacts.

CI discovers `test_*.py` in both suites automatically. Tests, builds and coverage
do not prove physical cutting, gear capture or flow through the nozzle.
Hardware acceptance still requires a supervised individual change before
repeated multicolor tests.
