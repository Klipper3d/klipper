#!/usr/bin/env python3
"""
Exercise MANUAL_STEPPER RETARGET= over Moonraker and diagnose results from
klippy.log.

Typical flywheel sequence per iteration:
  1) ENABLE=1, SET_POSITION=0
  2) MOVE=<far> SPEED=<v> ACCEL=0 SYNC=0   (starts async move)
  3) wait --retarget-delay
  4) RETARGET=<pos> SPEED=<v2> ACCEL=0
  5) optional second RETARGET (direction / speed change)

After each iteration the script prints:
  - Moonraker HTTP result (error field if any)
  - New klippy.log lines since iteration start
  - Parsed manual_stepper_retarget / cancel / shutdown lines
  - A short verdict

Examples:
  ./scripts/test_retarget_step_moonraker.py --stepper back_left_drive
  ./scripts/test_retarget_step_moonraker.py --stepper flywheel --log-file \\
      ~/printer_data/logs/klippy.log --iterations 5
  ./scripts/test_retarget_step_moonraker.py --analyze-only \\
      --log-file ~/printer_data/logs/klippy.log --tail 200
"""
from __future__ import annotations

import argparse
import json
import os
import re
import sys
import time
from urllib import error, parse, request

# Lines we care about when diagnosing RETARGET
RE_RETARGET = re.compile(
    r"manual_stepper_retarget |"
    r"RETARGET requires |"
    r"RETARGET failed |"
    r"RETARGET cannot be combined |"
    r"Move out of range|"
    r"manual_stepper_cancel_|"
    r"Timer too close|"
    r"Shutdown|"
    r"Unable to build C code|"
    r"trapq_truncate|"
    r"AttributeError|"
    r"Internal error in stepcompress"
)
RE_RETARGET_OK = re.compile(
    r"manual_stepper_retarget stepper=.* splice_time=.* splice_pos=.*"
)


def resolve_log_path(user_path: str | None) -> str:
    if user_path:
        return os.path.expanduser(user_path)
    candidates = [
        "~/printer_data/logs/klippy.log",
        "/tmp/klippy.log",
    ]
    for c in candidates:
        p = os.path.expanduser(c)
        if os.path.isfile(p):
            return p
    return os.path.expanduser(candidates[0])


def read_log_tail(path: str, max_bytes: int = 2_000_000) -> str:
    try:
        size = os.path.getsize(path)
        with open(path, "rb") as f:
            if size > max_bytes:
                f.seek(size - max_bytes)
            data = f.read()
        return data.decode("utf-8", errors="replace")
    except OSError as e:
        return f"<could not read {path}: {e}>"


def log_byte_offset(path: str) -> int:
    try:
        return os.path.getsize(path)
    except OSError:
        return 0


def read_log_from_offset(path: str, offset: int) -> str:
    try:
        with open(path, "rb") as f:
            f.seek(offset)
            return f.read().decode("utf-8", errors="replace")
    except OSError as e:
        return f"<could not read {path}: {e}>"


def filter_interesting(lines: list[str]) -> list[str]:
    out = []
    for line in lines:
        if RE_RETARGET.search(line):
            out.append(line.rstrip())
    return out


def post_gcode(base_url: str, script: str, timeout: float) -> dict:
    endpoint = base_url.rstrip("/") + "/printer/gcode/script"
    payload = parse.urlencode({"script": script}).encode()
    req = request.Request(endpoint, data=payload, method="POST")
    with request.urlopen(req, timeout=timeout) as resp:
        raw = resp.read().decode("utf-8", errors="replace")
    try:
        return json.loads(raw)
    except json.JSONDecodeError:
        return {"raw": raw}


def check_moonraker_error(resp: dict) -> str | None:
    if not isinstance(resp, dict):
        return str(resp)
    if "error" in resp:
        return str(resp["error"])
    result = resp.get("result")
    if isinstance(result, str) and "error" in result.lower():
        return result
    return None


def verdict_for_chunk(chunk: str, stepper: str) -> str:
    if "RETARGET cannot be combined with" in chunk:
        return (
            "FAIL: RETARGET was sent with ENABLE/MOVE/SET_POSITION in the same "
            "line — use separate gcode lines (see script sequence in --help)"
        )
    if "RETARGET requires active MOVE SYNC=0" in chunk:
        return (
            "FAIL: no active SYNC=0 move (start MOVE ... SYNC=0 first, or move "
            "already finished / active_move_id cleared)"
        )
    if "RETARGET failed to truncate trapq" in chunk:
        return "FAIL: trapq_truncate_after returned -1 (empty trapq?)"
    if "Invalid sequence" in chunk or "Internal error in stepcompress" in chunk:
        return (
            "FAIL: stepcompress could not generate steps after retarget "
            "(host stepcompress not re-anchored — update klippy/stepper.py)"
        )
    if "Timer too close" in chunk or "Shutdown" in chunk:
        return "FAIL: MCU shutdown or timing error after retarget"
    if "AttributeError" in chunk and "trapq_truncate" in chunk:
        return (
            "FAIL: trapq_truncate_after missing from c_helper.so — restart "
            "klippy after rebuilding chelper"
        )
    if RE_RETARGET_OK.search(chunk):
        short = stepper.split()[-1] if " " in stepper else stepper
        if f"stepper=manual_stepper {short}" in chunk or f"stepper={short}" in chunk:
            return "OK: manual_stepper_retarget logged"
        return "OK: manual_stepper_retarget logged (check stepper name in log)"
    if "Unable to build C code" in chunk:
        return "FAIL: chelper build error on klippy start"
    return "UNKNOWN: no manual_stepper_retarget line — see log excerpt below"


def analyze_log_text(text: str, stepper: str) -> None:
    lines = text.splitlines()
    hits = filter_interesting(lines)
    print(f"  interesting log lines: {len(hits)}")
    for line in hits[-40:]:
        print(f"    {line}")
    print(f"  >> {verdict_for_chunk(text, stepper)}")


def run_iteration(args, iteration: int, log_path: str) -> bool:
    stepper = args.stepper
    log_off = log_byte_offset(log_path)
    tag = f"[{iteration:04d}]"
    ok = True

    def run_script(label: str, script: str) -> bool:
        nonlocal ok
        print(f"{tag} {label}", flush=True)
        try:
            resp = post_gcode(args.base_url, script, args.timeout)
        except error.HTTPError as e:
            body = e.read().decode("utf-8", errors="replace")
            print(f"  HTTP {e.code}: {body}", flush=True)
            ok = False
            return False
        except Exception as e:
            print(f"  EXCEPTION: {e}", flush=True)
            ok = False
            return False
        err = check_moonraker_error(resp)
        if err:
            print(f"  Moonraker/Klipper error: {err}", flush=True)
            ok = False
        else:
            print("  gcode script accepted", flush=True)
        return err is None

    if args.enable_before_move:
        if not run_script(
            "ENABLE=1",
            f"MANUAL_STEPPER STEPPER={stepper} ENABLE=1",
        ):
            return False

    if not run_script(
        "SET_POSITION=0",
        f"MANUAL_STEPPER STEPPER={stepper} SET_POSITION=0",
    ):
        return False

    move_gcode = (
        f"MANUAL_STEPPER STEPPER={stepper} "
        f"MOVE={args.move} SPEED={args.speed} ACCEL={args.accel} SYNC=0"
    )
    if not run_script("MOVE SYNC=0", move_gcode):
        return False

    if args.retarget_delay > 0:
        time.sleep(args.retarget_delay)

    retarget_gcode = (
        f"MANUAL_STEPPER STEPPER={stepper} "
        f"RETARGET={args.retarget} SPEED={args.retarget_speed} ACCEL={args.accel}"
    )
    if not run_script("RETARGET", retarget_gcode):
        return False

    if args.second_retarget is not None:
        if args.second_retarget_delay > 0:
            time.sleep(args.second_retarget_delay)
        retarget2 = (
            f"MANUAL_STEPPER STEPPER={stepper} "
            f"RETARGET={args.second_retarget} "
            f"SPEED={args.second_retarget_speed} ACCEL={args.accel}"
        )
        if not run_script("RETARGET #2", retarget2):
            return False

    # Allow klippy to flush log lines
    time.sleep(args.log_settle)
    chunk = read_log_from_offset(log_path, log_off)
    print(f"{tag} klippy.log (+{len(chunk)} bytes since iteration start)")
    analyze_log_text(chunk, stepper)
    return ok and "OK:" in verdict_for_chunk(chunk, stepper)


def run(args) -> int:
    log_path = resolve_log_path(args.log_file)
    print(f"Using klippy.log: {log_path}")
    if not os.path.isfile(log_path):
        print(
            "WARNING: log file not found yet; run a test anyway or pass "
            "--log-file",
            file=sys.stderr,
        )

    if args.analyze_only:
        text = read_log_tail(log_path)
        if args.tail > 0:
            lines = text.splitlines()
            text = "\n".join(lines[-args.tail :])
        print(f"Analyzing last {args.tail or 'all'} lines of {log_path}")
        analyze_log_text(text, args.stepper)
        return 0

    failures = 0
    for i in range(1, args.iterations + 1):
        if not run_iteration(args, i, log_path):
            failures += 1
            if args.stop_on_error:
                return 1
        if args.loop_delay > 0 and i < args.iterations:
            time.sleep(args.loop_delay)

    print(
        f"Done. iterations={args.iterations} failures={failures} "
        f"successes={args.iterations - failures}",
        flush=True,
    )
    print()
    print("Manual log checks:")
    print(f"  grep manual_stepper_retarget {log_path}")
    print(f"  grep -E 'RETARGET|Timer too close|Shutdown' {log_path} | tail -30")
    return 0 if failures == 0 else 1


def build_argparser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(
        description="Test MANUAL_STEPPER RETARGET= via Moonraker + klippy.log"
    )
    p.add_argument(
        "--base-url",
        default="http://127.0.0.1:7125",
        help="Moonraker base URL (default: %(default)s)",
    )
    p.add_argument(
        "--stepper",
        required=True,
        help="manual_stepper config name (e.g. back_left_drive)",
    )
    p.add_argument(
        "--log-file",
        default=None,
        help="klippy.log path (default: ~/printer_data/logs/klippy.log or /tmp)",
    )
    p.add_argument("--iterations", type=int, default=3)
    p.add_argument("--move", type=float, default=99999.0,
                   help="initial MOVE target (default: %(default)s)")
    p.add_argument("--speed", type=float, default=30.0,
                   help="initial MOVE speed (default: %(default)s)")
    p.add_argument("--retarget", type=float, default=99999.0,
                   help="first RETARGET position (default: %(default)s)")
    p.add_argument("--retarget-speed", type=float, default=45.0,
                   help="first RETARGET speed (default: %(default)s)")
    p.add_argument(
        "--second-retarget",
        type=float,
        default=None,
        help="optional second RETARGET position (e.g. -99999 for reverse)",
    )
    p.add_argument(
        "--second-retarget-speed",
        type=float,
        default=45.0,
        help="second RETARGET speed (default: %(default)s)",
    )
    p.add_argument("--accel", type=float, default=0.0)
    p.add_argument(
        "--retarget-delay",
        type=float,
        default=0.15,
        help="seconds after MOVE before RETARGET (default: %(default)s)",
    )
    p.add_argument(
        "--second-retarget-delay",
        type=float,
        default=0.15,
        help="seconds between RETARGET commands (default: %(default)s)",
    )
    p.add_argument(
        "--log-settle",
        type=float,
        default=0.25,
        help="seconds to wait before reading new log lines (default: %(default)s)",
    )
    p.add_argument("--loop-delay", type=float, default=0.5)
    p.add_argument("--timeout", type=float, default=15.0)
    p.add_argument("--stop-on-error", action="store_true")
    p.add_argument(
        "--enable-before-move",
        action="store_true",
        help="send ENABLE=1 before each iteration",
    )
    p.add_argument(
        "--analyze-only",
        action="store_true",
        help="do not send gcode; only parse existing klippy.log",
    )
    p.add_argument(
        "--tail",
        type=int,
        default=150,
        help="with --analyze-only, number of log lines to scan (default: %(default)s)",
    )
    return p


def main() -> int:
    return run(build_argparser().parse_args())


if __name__ == "__main__":
    sys.exit(main())
