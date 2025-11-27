#!/usr/bin/env python3
"""
Dataset-driven harness that feeds line lengths into Klipper's winch forward
transform (WinchKinematics.calc_position) and compares against the reference
quadratic solver output.

It reuses the datasets and solver runner from ai_docs/run_tests.py. For each
sample, it:
 1) Runs solver_quadratic to get a baseline pose from motor degrees.
  2) Builds a WinchFlexHelper with the sample's config and anchors.
  3) Computes cable lengths as (distance - flex) at the baseline pose.
  4) Calls the Klipper forward transform with those lengths.
 5) Reports Cartesian error statistics against both the quadratic solver and
    the dataset ground truth.
"""
from __future__ import annotations

import math
from dataclasses import dataclass
from typing import Dict, Iterable, List, Sequence, Tuple

import sys
from pathlib import Path
import importlib.util

ROOT = Path(__file__).resolve().parents[1]
KLIPPY_DIR = ROOT / "klippy"
AI_DOCS_DIR = ROOT / "ai_docs"
for p in (KLIPPY_DIR, AI_DOCS_DIR):
    if str(p) not in sys.path:
        sys.path.insert(0, str(p))

# Load winch.py directly to avoid importing klippy.py (which has heavier deps)
WINCH_PATH = KLIPPY_DIR / "kinematics" / "winch.py"
spec = importlib.util.spec_from_file_location("winch_module", WINCH_PATH)
winch_module = importlib.util.module_from_spec(spec)
assert spec and spec.loader
sys.modules[spec.name] = winch_module
spec.loader.exec_module(winch_module)

WinchFlexHelper = winch_module.WinchFlexHelper  # type: ignore
WinchKinematics = winch_module.WinchKinematics  # type: ignore
import mathutil  # resolved from KLIPPY_DIR on sys.path

RUN_TESTS_PATH = AI_DOCS_DIR / "run_tests.py"
rt_spec = importlib.util.spec_from_file_location("run_tests_module", RUN_TESTS_PATH)
rt_module = importlib.util.module_from_spec(rt_spec)
assert rt_spec and rt_spec.loader
sys.modules[rt_spec.name] = rt_module
rt_spec.loader.exec_module(rt_module)

GEOMETRY_ORDER = rt_module.GEOMETRY_ORDER
MAX_SUCCESS_ERR_MM = rt_module.MAX_SUCCESS_ERR_MM
Sample = rt_module.Sample
SolverResult = rt_module.SolverResult
Stats = rt_module.Stats
load_samples = rt_module.load_samples
run_solver = rt_module.run_solver
summarise = rt_module.summarise


# Minimal config shim for WinchFlexHelper
class FlexConfigShim:
    def __init__(self, cfg: Dict[str, object], num_anchors: int):
        self.cfg = cfg
        self.num = num_anchors

    def _broadcast(self, value, default):
        if value is None:
            base = default
        else:
            base = value
        if isinstance(base, (list, tuple)):
            arr = list(base)
        else:
            arr = [base]
        if not arr:
            arr = [default] if not isinstance(default, (list, tuple)) else list(default)
        if len(arr) < self.num:
            arr += [arr[-1]] * (self.num - len(arr))
        elif len(arr) > self.num:
            arr = arr[: self.num]
        return arr

    def getfloat(self, name, default=0.0, minval=None, maxval=None):
        return float(self.cfg.get(name, default))

    def getfloatlist(self, name, default, count=None):
        vals = self._broadcast(self.cfg.get(name), default)
        if count is not None and len(vals) < count:
            vals += [vals[-1]] * (count - len(vals))
        return [float(v) for v in vals]

    def getchoice(self, name, choices, default=None):
        val = self.cfg.get(name, default)
        return val if val in choices else default

    def getboolean(self, name, default=False):
        return bool(self.cfg.get(name, default))

    # Unused in this shim but kept for compatibility with other config readers
    def get_printer(self):
        return None


class DummyStepper:
    def __init__(self, name: str):
        self._name = name

    def get_name(self):
        return self._name


def build_flex_config(sample_cfg: Dict[str, object]) -> Dict[str, object]:
    # Map dataset config keys to Klipper's names
    cfg = {}
    cfg["winch_mover_weight"] = sample_cfg.get("mover_weight", 0.0)
    cfg["winch_spring_constant"] = sample_cfg.get("spring_k_per_unit_length", 0.0)
    cfg["winch_min_force"] = sample_cfg.get("min_force", 0.0)
    cfg["winch_max_force"] = sample_cfg.get("max_force", 120.0)
    cfg["winch_guy_wire_lengths"] = sample_cfg.get("guy_wire_lengths", 0.0)
    cfg["flex_compensation_algorithm"] = "qp"
    cfg["ignore_gravity"] = sample_cfg.get("ignore_gravity", False)
    cfg["ignore_pretension"] = sample_cfg.get("ignore_pretension", False)
    return cfg


def compute_lengths(helper: WinchFlexHelper, pos: Sequence[float]) -> List[float]:
    distances, flex = helper.calc_arrays(pos)
    return [d - f for d, f in zip(distances, flex)]


def make_forward_transform(anchors: List[List[float]], helper: WinchFlexHelper):
    kin = WinchKinematics.__new__(WinchKinematics)
    kin.anchors = anchors
    kin.flex_helper = helper
    kin.steppers = [DummyStepper(f"s{i}") for i in range(len(anchors))]
    kin._halley_eta = 1e-3
    kin._halley_tol = 1e-4
    kin._halley_max_iters = 30
    kin._halley_hybrid_iters = 3
    kin._flex_outer_iters = 2
    kin._last_forward = None
    return kin


def klipper_forward_errors(
    samples: Sequence[Sample],
    baseline: Sequence[SolverResult],
    use_flex: bool,
    use_ground_truth: bool,
) -> List[Tuple[float | None, bool]]:
    errors: List[Tuple[float | None, bool]] = []
    for sample, ref in zip(samples, baseline):
        if sample.unsupported or ref.unsupported or not ref.ok:
            errors.append((None, False))
            continue
        cfg_map = build_flex_config(sample.config)
        cfg = FlexConfigShim(cfg_map, len(sample.anchors))
        helper = WinchFlexHelper(sample.anchors, cfg)
        helper.set_active(use_flex and sample.config.get("use_flex", True))
        target_pos = sample.gt if use_ground_truth else ref.pos
        # Always derive line lengths from the solver output (motor -> pose),
        # but optionally evaluate the error against the dataset's ground truth.
        lengths = compute_lengths(helper, ref.pos)
        kin = make_forward_transform(sample.anchors, helper)
        stepper_positions = {s.get_name(): l for s, l in zip(kin.steppers, lengths)}
        try:
            pos = kin.calc_position(stepper_positions)
        except Exception:
            errors.append((None, False))
            continue
        dx = pos[0] - target_pos[0]
        dy = pos[1] - target_pos[1]
        dz = pos[2] - target_pos[2]
        err = math.sqrt(dx * dx + dy * dy + dz * dz)
        ok = err <= MAX_SUCCESS_ERR_MM
        errors.append((err, ok))
    return errors


def summarise_klipper(samples: Sequence[Sample], errors: Sequence[Tuple[float | None, bool]]):
    records_by_geo: Dict[str, List[Tuple[bool, float | None]]] = {}
    skipped_by_geo: Dict[str, int] = {}
    for sample, (err, ok) in zip(samples, errors):
        geo = sample.geometry
        if geo not in records_by_geo:
            records_by_geo[geo] = []
            skipped_by_geo[geo] = 0
        if err is None:
            skipped_by_geo[geo] += 1
            continue
        records_by_geo[geo].append((ok, err))

    def stats_for(records: List[Tuple[bool, float | None]], skipped: int) -> Stats:
        supported = len(records)
        success = sum(1 for r in records if r[0])
        errs = [r[1] for r in records if r[0] and r[1] is not None]
        mae = sum(errs) / len(errs) if errs else float("nan")
        med = sorted(errs)[len(errs) // 2] if errs else float("nan")
        std = math.sqrt(sum((e - mae) ** 2 for e in errs) / len(errs)) if errs else float("nan")
        return Stats(
            total=supported + skipped,
            supported=supported,
            success_rate=(success / supported) * 100 if supported else 0.0,
            mae=mae,
            med_err=med,
            std_err=std,
            mean_cost=float("nan"),
            mean_iters=float("nan"),
            mean_ms=float("nan"),
            skipped=skipped,
        )

    overall_records: List[Tuple[bool, float | None]] = []
    for recs in records_by_geo.values():
        overall_records.extend(recs)

    overall_stats = stats_for(overall_records, sum(skipped_by_geo.values()))
    per_geo = {geo: stats_for(recs, skipped_by_geo[geo]) for geo, recs in records_by_geo.items()}
    return overall_stats, per_geo


def print_klipper_summary(title: str, overall: Stats, per_geo: Dict[str, Stats]):
    geo_cols = []
    for geo in GEOMETRY_ORDER:
        if geo in per_geo:
            g = per_geo[geo]
            geo_cols.append(f"{geo} {g.success_rate:.1f}%/{g.mae:.3f}mm/±{g.std_err:.3f}mm")
    geo_suffix = f" | {' | '.join(geo_cols)}" if geo_cols else ""
    print(
        f"{title}\n"
        f"- klipper    success {overall.success_rate:.1f}% | mae {overall.mae:.3f} mm | "
        f"median {overall.med_err:.3f} mm | (unsupported {overall.skipped}){geo_suffix}"
    )


def run_suite(name: str, pattern: str):
    samples = load_samples(pattern)
    baseline = run_solver(samples, "quadratic", use_flex=True)
    errors_vs_quad = klipper_forward_errors(samples, baseline, use_flex=True, use_ground_truth=False)
    overall_q, per_geo_q = summarise_klipper(samples, errors_vs_quad)
    print_klipper_summary(f"{name} (vs quadratic)", overall_q, per_geo_q)

    errors_vs_gt = klipper_forward_errors(samples, baseline, use_flex=True, use_ground_truth=True)
    overall_gt, per_geo_gt = summarise_klipper(samples, errors_vs_gt)
    print_klipper_summary(f"{name} (vs ground truth)", overall_gt, per_geo_gt)


def main():
    patterns = [
        "clean_baseline_*.jsonl",
        "larger_baseline_*.jsonl",
        "near_singularities_*.jsonl",
        "at_singularities_*.jsonl",
        "outside_singularities_*.jsonl",
        "systematic_bias_*.jsonl",
    ]
    for pat in patterns:
        run_suite(f"Dataset {pat}", pat)


if __name__ == "__main__":
    main()
