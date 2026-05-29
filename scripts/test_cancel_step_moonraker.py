#!/usr/bin/env python3
"""
Stress-test MANUAL_STEPPER cancel timing via Moonraker HTTP API.

For each iteration:
  1) Reset manual stepper logical position to 0
  2) Start a long SYNC=0 move
  3) Wait a short delay
  4) Issue CANCEL_STEP

This is intended to catch intermittent timing races (eg. "Timer too close",
missing trsync responses, or unexpected shutdowns) when repeatedly canceling
active manual_stepper moves.
"""
import argparse
import json
import sys
import time
from urllib import error, parse, request


def post_gcode(base_url, gcode, timeout):
    endpoint = base_url.rstrip("/") + "/printer/gcode/script"
    payload = parse.urlencode({"script": gcode}).encode()
    req = request.Request(endpoint, data=payload, method="POST")
    with request.urlopen(req, timeout=timeout) as resp:
        raw = resp.read().decode("utf-8", errors="replace")
    try:
        return json.loads(raw)
    except json.JSONDecodeError:
        return {"raw": raw}


def run(args):
    failures = 0
    current_enabled = True
    if args.enable_before_move:
        current_enabled = False
    for i in range(1, args.iterations + 1):
        print(f"[{i:04d}] SET_POSITION -> MOVE -> CANCEL", flush=True)
        try:
            if args.enable_before_move and not current_enabled:
                ren = post_gcode(
                    args.base_url,
                    f"MANUAL_STEPPER STEPPER={args.stepper} ENABLE=1",
                    args.timeout,
                )
                if "error" in ren:
                    failures += 1
                    print(f"  FAIL enable: {ren['error']}", flush=True)
                    continue
                current_enabled = True

            r1 = post_gcode(
                args.base_url,
                f"MANUAL_STEPPER STEPPER={args.stepper} SET_POSITION=0",
                args.timeout,
            )
            if "error" in r1:
                failures += 1
                print(f"  FAIL set_position: {r1['error']}", flush=True)
                continue

            r2 = post_gcode(
                args.base_url,
                (
                    f"MANUAL_STEPPER STEPPER={args.stepper} "
                    f"MOVE={args.move} SPEED={args.speed} ACCEL={args.accel} SYNC=0"
                ),
                args.timeout,
            )
            if "error" in r2:
                failures += 1
                print(f"  FAIL move: {r2['error']}", flush=True)
                continue

            time.sleep(args.cancel_delay)

            if args.cancel_script:
                cancel_script = args.cancel_script.format(stepper=args.stepper)
            else:
                cancel_script = f"CANCEL_STEP STEPPER={args.stepper}"
            r3 = post_gcode(args.base_url, cancel_script, args.timeout)
            if "error" in r3:
                failures += 1
                print(f"  FAIL cancel: {r3['error']}", flush=True)
                if args.stop_on_error:
                    return 1
            else:
                print("  OK", flush=True)
                if args.enable_before_move:
                    # Assume cancel may disable the motor.
                    current_enabled = False

            if args.loop_delay > 0.0:
                time.sleep(args.loop_delay)
        except error.HTTPError as e:
            failures += 1
            body = e.read().decode("utf-8", errors="replace")
            print(f"  HTTP {e.code}: {body}", flush=True)
            if args.stop_on_error:
                return 1
        except Exception as e:
            failures += 1
            print(f"  EXCEPTION: {e}", flush=True)
            if args.stop_on_error:
                return 1

    print(
        (
            f"Done. iterations={args.iterations} failures={failures} "
            f"successes={args.iterations - failures}"
        ),
        flush=True,
    )
    return 0 if failures == 0 else 1


def build_argparser():
    p = argparse.ArgumentParser(
        description="Stress-test CANCEL_STEP over Moonraker HTTP"
    )
    p.add_argument(
        "--base-url",
        default="http://127.0.0.1:7125",
        help="Moonraker base URL (default: %(default)s)",
    )
    p.add_argument(
        "--stepper",
        required=True,
        help="manual_stepper config name (eg. back_left_drive)",
    )
    p.add_argument(
        "--iterations",
        type=int,
        default=50,
        help="number of MOVE/CANCEL loops (default: %(default)s)",
    )
    p.add_argument(
        "--move",
        type=float,
        default=200.0,
        help="target MOVE position (default: %(default)s)",
    )
    p.add_argument(
        "--speed",
        type=float,
        default=30.0,
        help="MOVE speed in mm/s (default: %(default)s)",
    )
    p.add_argument(
        "--accel",
        type=float,
        default=200.0,
        help="MOVE acceleration in mm/s^2 (default: %(default)s)",
    )
    p.add_argument(
        "--cancel-delay",
        type=float,
        default=0.1,
        help="seconds to wait after MOVE before CANCEL_STEP (default: %(default)s)",
    )
    p.add_argument(
        "--loop-delay",
        type=float,
        default=0.05,
        help="seconds to wait between iterations (default: %(default)s)",
    )
    p.add_argument(
        "--timeout",
        type=float,
        default=10.0,
        help="HTTP request timeout in seconds (default: %(default)s)",
    )
    p.add_argument(
        "--stop-on-error",
        action="store_true",
        help="exit immediately on first failure",
    )
    p.add_argument(
        "--cancel-script",
        default="",
        help=(
            "override cancel gcode script (use {stepper} placeholder), "
            "default sends CANCEL_STEP STEPPER={stepper}"
        ),
    )
    p.add_argument(
        "--enable-before-move",
        action="store_true",
        help="send MANUAL_STEPPER ... ENABLE=1 before each move loop",
    )
    return p


def main():
    args = build_argparser().parse_args()
    return run(args)


if __name__ == "__main__":
    sys.exit(main())
