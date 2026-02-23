#!/usr/bin/env python3
"""
Synthetic dataset test harness for the Hangprinter forward transforms.
Reads the JSONL datasets, feeds them to the three solver binaries, and prints a brief report.
"""
from __future__ import annotations

import json
import math
import random
import statistics
import subprocess
import sys
from collections import defaultdict
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, Iterable, List, Sequence

ROOT = Path(__file__).parent
DATASETS = ROOT / "datasets"
BUILD = ROOT / "build"
MAX_ANCHORS = 8  # solver supports up to CubeCorners
MM_TO_DEG = 360.0 / (2.0 * math.pi * 75.0)  # approx conversion for 75 mm spool
GEOMETRY_ORDER = ["HP5", "Slideprinter", "Spidercam", "CubeCorners"]
# Treat solutions with larger error as failures in summary stats.
MAX_SUCCESS_ERR_MM = 10.0


@dataclass
class Sample:
    anchors: List[List[float]]
    motor_deg: List[float]
    gt: List[float]
    geometry: str
    dataset: str
    anchor_set: int
    config: Dict[str, Any] = field(default_factory=dict)
    unsupported: bool = False


@dataclass
class SolverResult:
    ok: bool
    pos: List[float]
    iterations: int
    cost: float
    runtime_ms: float
    unsupported: bool = False


@dataclass
class Stats:
    total: int
    supported: int
    success_rate: float
    mae: float
    med_err: float
    std_err: float
    mean_cost: float
    mean_iters: float
    mean_ms: float
    skipped: int


@dataclass
class SolverSummary:
    overall: Stats
    by_geo: dict[str, Stats]


SOLVERS = {
    "quadratic": {
        "path": BUILD / "solver_quadratic",
        "supports_no_flex": False,
        "label": "quadratic (reference)",
        "type": "binary",
    },
    "calc_position": {
        "path": BUILD / "solver_calc_position",
        "supports_no_flex": False,
        "label": "calc_position",
        "type": "binary",
    },
}


def load_samples(pattern: str) -> List[Sample]:
    samples: List[Sample] = []
    for path in sorted(DATASETS.glob(pattern)):
        with path.open() as fh:
            for line in fh:
                obj = json.loads(line)
                anchors = obj["anchors"]
                motors = obj["motor_samples"]
                poses = obj["real_xyz"]
                config = obj.get("config_used", {}) or {}
                unsupported = len(anchors) > MAX_ANCHORS
                for pose, motor in zip(poses, motors):
                    samples.append(
                        Sample(
                            anchors=anchors,
                            motor_deg=motor,
                            gt=pose,
                            geometry=obj.get("geometry", path.stem),
                            dataset=obj.get("dataset", path.stem),
                            anchor_set=obj.get("anchor_set", 0),
                            config=config,
                            unsupported=unsupported,
                        )
                    )
    return samples


def add_gaussian_noise(samples: Sequence[Sample], sigma_mm: float, rng: random.Random) -> List[Sample]:
    noisy: List[Sample] = []
    sigma_deg = sigma_mm * MM_TO_DEG
    for s in samples:
        motors = [m + rng.gauss(0.0, sigma_deg) for m in s.motor_deg]
        noisy.append(
            Sample(
                anchors=s.anchors,
                motor_deg=motors,
                gt=s.gt,
                geometry=s.geometry,
                dataset=s.dataset,
                anchor_set=s.anchor_set,
                config=s.config,
                unsupported=s.unsupported,
            )
        )
    return noisy


def add_bias(samples: Sequence[Sample], biases_mm: Sequence[float]) -> List[Sample]:
    biases_deg = [b * MM_TO_DEG for b in biases_mm]
    biased: List[Sample] = []
    for s in samples:
        motors = [m + biases_deg[i % len(biases_deg)] for i, m in enumerate(s.motor_deg)]
        biased.append(
            Sample(
                anchors=s.anchors,
                motor_deg=motors,
                gt=s.gt,
                geometry=s.geometry,
                dataset=s.dataset,
                anchor_set=s.anchor_set,
                config=s.config,
                unsupported=s.unsupported,
            )
        )
    return biased


DEFAULT_CONFIG = {
    "spool_buildup_factor": 0.043003,
    "spool_r_in_origin": [75.0],
    "spool_gear_teeth": 255.0,
    "motor_gear_teeth": 20.0,
    "mechanical_advantage": [2.0, 2.0, 2.0, 2.0, 4.0],
    "lines_per_spool": [1.0],
    "min_force": 3.0,
    "max_force": 120.0,
    "spring_k_per_unit_length": 20000.0,
    "mover_weight": 2.0,
    "lambda_reg": 1e-3,
    "tol": 1e-3,
    "max_iters_target": 100,
    "g": 9.81,
    "guy_wire_lengths": 0.0,
}


def _broadcast_array(value: Any, count: int, default: Any) -> List[float]:
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
    if len(arr) < count:
        arr += [arr[-1]] * (count - len(arr))
    elif len(arr) > count:
        arr = arr[:count]
    return [float(x) for x in arr]


def make_config_header(cfg: Dict[str, Any], num_anchors: int) -> str:
    spool_buildup = float(cfg.get("spool_buildup_factor", DEFAULT_CONFIG["spool_buildup_factor"]))
    spring_k = float(cfg.get("spring_k_per_unit_length", DEFAULT_CONFIG["spring_k_per_unit_length"]))
    mover_weight = float(cfg.get("mover_weight", DEFAULT_CONFIG["mover_weight"]))
    spool_gear = float(cfg.get("spool_gear_teeth", DEFAULT_CONFIG["spool_gear_teeth"]))
    motor_gear = float(cfg.get("motor_gear_teeth", DEFAULT_CONFIG["motor_gear_teeth"]))
    steps_per_rev = 360.0  # motor_samples are in degrees
    use_flex = bool(cfg.get("use_flex", True))
    ignore_gravity = bool(cfg.get("ignore_gravity", False))
    ignore_pretension = bool(cfg.get("ignore_pretension", False))
    lambda_reg = float(cfg.get("lambda_reg", DEFAULT_CONFIG["lambda_reg"]))
    tol = float(cfg.get("tol", DEFAULT_CONFIG["tol"]))
    max_iters_target = int(cfg.get("max_iters_target", DEFAULT_CONFIG["max_iters_target"]))
    g = float(cfg.get("g", DEFAULT_CONFIG["g"]))

    spool_r = _broadcast_array(cfg.get("spool_r_in_origin") or cfg.get("spool_r"), num_anchors, DEFAULT_CONFIG["spool_r_in_origin"])
    mech_adv = _broadcast_array(cfg.get("mechanical_advantage"), num_anchors, DEFAULT_CONFIG["mechanical_advantage"])
    lines = _broadcast_array(cfg.get("lines_per_spool"), num_anchors, DEFAULT_CONFIG["lines_per_spool"])
    min_force = _broadcast_array(cfg.get("min_force"), num_anchors, DEFAULT_CONFIG["min_force"])
    max_force = _broadcast_array(cfg.get("max_force"), num_anchors, DEFAULT_CONFIG["max_force"])
    guy_wires = _broadcast_array(cfg.get("guy_wire_lengths"), num_anchors, DEFAULT_CONFIG["guy_wire_lengths"])

    def fmt(val: float) -> str:
        return f"{val:.9g}"

    parts = [
        "CFG",
        str(num_anchors),
        fmt(spool_buildup),
        fmt(spring_k),
        fmt(mover_weight),
        fmt(spool_gear),
        fmt(motor_gear),
        fmt(steps_per_rev),
        "1" if use_flex else "0",
        "1" if ignore_gravity else "0",
        "1" if ignore_pretension else "0",
        fmt(lambda_reg),
        fmt(tol),
        str(max_iters_target),
        fmt(g),
    ]
    for arr in (spool_r, mech_adv, lines, min_force, max_force, guy_wires):
        parts.extend(fmt(v) for v in arr)
    return " ".join(parts)


def make_input_lines(samples: Sequence[Sample], use_flex: bool) -> tuple[List[str], List[int], int]:
    lines: List[str] = []
    idx_map: List[int] = []
    skipped = 0
    for idx, sample in enumerate(samples):
        if sample.unsupported:
            skipped += 1
            continue
        num = len(sample.anchors)
        motor_str = " ".join(f"{m:.6f}" for m in sample.motor_deg[:num])
        anchor_flat = [coord for anchor in sample.anchors[:num] for coord in anchor]
        anchor_str = " ".join(f"{a:.6f}" for a in anchor_flat)
        lines.append(f"{num} {1 if use_flex else 0} {motor_str} {anchor_str}")
        idx_map.append(idx)
    return lines, idx_map, skipped


def run_solver(samples: Sequence[Sample], solver_key: str, use_flex: bool) -> List[SolverResult]:
    solver = SOLVERS[solver_key]
    exe = solver["path"]
    if not exe.exists():
        raise FileNotFoundError(f"Solver binary missing: {exe}")

    grouped: Dict[str, List[int]] = defaultdict(list)
    headers: Dict[str, str] = {}
    for idx, sample in enumerate(samples):
        header = make_config_header(sample.config, len(sample.anchors))
        grouped[header].append(idx)
        headers[header] = header

    results = [
        SolverResult(ok=False, pos=[0.0, 0.0, 0.0], iterations=0, cost=0.0, runtime_ms=0.0, unsupported=s.unsupported)
        for s in samples
    ]

    for header, indices in grouped.items():
        subset = [samples[i] for i in indices]
        lines, idx_map, skipped = make_input_lines(subset, use_flex)
        if not lines:
            continue
        payload = [header] + lines if header else lines
        proc = subprocess.run(
            [str(exe)],
            input="\n".join(payload) + "\n",
            text=True,
            capture_output=True,
            check=False,
        )
        output_lines = [l for l in proc.stdout.splitlines() if l.strip()]
        if len(output_lines) != len(idx_map):
            raise RuntimeError(f"{solver_key} returned {len(output_lines)} lines for {len(idx_map)} samples (header {header})")

        for raw, subset_idx in zip(output_lines, idx_map):
            parts = raw.strip().split()
            if len(parts) < 7:
                continue
            status, x, y, z, iters, cost, runtime = parts[:7]
            ok = status == "ok"
            sample_idx = indices[subset_idx]
            results[sample_idx] = SolverResult(
                ok=ok,
                pos=[float(x), float(y), float(z)],
                iterations=int(float(iters)),
                cost=float(cost),
                runtime_ms=float(runtime),
                unsupported=False,
            )
    return results


def summarise(samples: Sequence[Sample], results: Sequence[SolverResult]) -> SolverSummary:
    records_by_geo: dict[str, list[tuple[bool, float | None, float, int, float]]] = defaultdict(list)
    skipped_by_geo: dict[str, int] = defaultdict(int)
    skipped_total = 0

    for sample, res in zip(samples, results):
        geo = sample.geometry
        if res.unsupported or sample.unsupported:
            skipped_total += 1
            skipped_by_geo[geo] += 1
            continue

        err = None
        if res.ok:
            dx = res.pos[0] - sample.gt[0]
            dy = res.pos[1] - sample.gt[1]
            dz = res.pos[2] - sample.gt[2]
            err = math.sqrt(dx * dx + dy * dy + dz * dz)

        ok = res.ok
        if res.ok and err is not None and err > MAX_SUCCESS_ERR_MM:
            ok = False
            err = None

        records_by_geo[geo].append((ok, err, res.cost, res.iterations, res.runtime_ms))

    def build_stats(records: list[tuple[bool, float | None, float, int, float]], skipped: int) -> Stats:
        supported = len(records)
        success = sum(1 for r in records if r[0])
        errs = [r[1] for r in records if r[0] and r[1] is not None]
        costs = [r[2] for r in records if r[0]]
        iters = [r[3] for r in records if r[0]]
        runtimes = [r[4] for r in records if r[0]]
        return Stats(
            total=supported + skipped,
            supported=supported,
            success_rate=(success / supported) * 100 if supported else 0.0,
            mae=statistics.mean(errs) if errs else float("nan"),
            med_err=statistics.median(errs) if errs else float("nan"),
            std_err=statistics.pstdev(errs) if errs else float("nan"),
            mean_cost=statistics.mean(costs) if costs else float("nan"),
            mean_iters=statistics.mean(iters) if iters else float("nan"),
            mean_ms=statistics.mean(runtimes) if runtimes else float("nan"),
            skipped=skipped,
        )

    overall_records: list[tuple[bool, float | None, float, int, float]] = []
    for recs in records_by_geo.values():
        overall_records.extend(recs)

    overall = build_stats(overall_records, skipped_total)
    by_geo = {geo: build_stats(recs, skipped_by_geo.get(geo, 0)) for geo, recs in records_by_geo.items()}
    return SolverSummary(overall=overall, by_geo=by_geo)


def summarise_difference(samples: Sequence[Sample], ref: Sequence[SolverResult], alt: Sequence[SolverResult]) -> SolverSummary:
    records_by_geo: dict[str, list[tuple[bool, float | None, float, int, float]]] = defaultdict(list)
    skipped_by_geo: dict[str, int] = defaultdict(int)
    skipped_total = 0

    for sample, r_ref, r_alt in zip(samples, ref, alt):
        geo = sample.geometry
        if r_ref.unsupported or r_alt.unsupported or sample.unsupported:
            skipped_total += 1
            skipped_by_geo[geo] += 1
            continue
        if not (r_ref.ok and r_alt.ok):
            records_by_geo[geo].append((False, None, 0.0, 0, 0.0))
            continue
        dx = r_ref.pos[0] - r_alt.pos[0]
        dy = r_ref.pos[1] - r_alt.pos[1]
        dz = r_ref.pos[2] - r_alt.pos[2]
        err = math.sqrt(dx * dx + dy * dy + dz * dz)
        ok = err <= MAX_SUCCESS_ERR_MM
        cost = abs(r_ref.cost - r_alt.cost)
        iters = abs(r_ref.iterations - r_alt.iterations)
        dt_ms = abs(r_ref.runtime_ms - r_alt.runtime_ms)
        records_by_geo[geo].append((ok, err, cost, iters, dt_ms))

    def build_stats(records: list[tuple[bool, float | None, float, int, float]], skipped: int) -> Stats:
        supported = len(records)
        success = sum(1 for r in records if r[0])
        errs = [r[1] for r in records if r[0] and r[1] is not None]
        costs = [r[2] for r in records if r[0]]
        iters = [r[3] for r in records if r[0]]
        runtimes = [r[4] for r in records if r[0]]
        return Stats(
            total=supported + skipped,
            supported=supported,
            success_rate=(success / supported) * 100 if supported else 0.0,
            mae=statistics.mean(errs) if errs else float("nan"),
            med_err=statistics.median(errs) if errs else float("nan"),
            std_err=statistics.pstdev(errs) if errs else float("nan"),
            mean_cost=statistics.mean(costs) if costs else float("nan"),
            mean_iters=statistics.mean(iters) if iters else float("nan"),
            mean_ms=statistics.mean(runtimes) if runtimes else float("nan"),
            skipped=skipped,
        )

    overall_records: list[tuple[bool, float | None, float, int, float]] = []
    for recs in records_by_geo.values():
        overall_records.extend(recs)
    overall = build_stats(overall_records, skipped_total)
    by_geo = {geo: build_stats(recs, skipped_by_geo.get(geo, 0)) for geo, recs in records_by_geo.items()}
    return SolverSummary(overall=overall, by_geo=by_geo)


def print_summary(title: str, summaries: dict):
    print(f"\n{title}")
    for solver, stats in summaries.items():
        overall = stats.overall
        geo_cols = []
        for geo in GEOMETRY_ORDER:
            if geo in stats.by_geo:
                g = stats.by_geo[geo]
                geo_cols.append(f"{geo} {g.success_rate:.1f}%/{g.mae:.3f}mm/±{g.std_err:.3f}mm")
        geo_suffix = f" | {' | '.join(geo_cols)}" if geo_cols else ""
        print(
            f"- {solver:10s} success {overall.success_rate:.1f}% | mae {overall.mae:.3f} mm | "
            f"median {overall.med_err:.3f} mm | iters {overall.mean_iters:.1f} | "
            f"cost {overall.mean_cost:.2e} | {overall.mean_ms:.3f} ms/solve "
            f"(unsupported {overall.skipped}){geo_suffix}"
        )


def run_suite(name: str, sample_patterns: Iterable[str], use_flex: bool = True, samples: List[Sample] | None = None) -> dict:
    if samples is None:
        combined: List[Sample] = []
        for pat in sample_patterns:
            combined.extend(load_samples(pat))
        samples = combined
    summaries = {}
    results_store: dict[str, List[SolverResult]] = {}
    for key, meta in SOLVERS.items():
        results = run_solver(samples, key, use_flex=use_flex)
        label = meta["label"]
        if not use_flex and meta["supports_no_flex"]:
            label = f"{label} (noflex)"
        summaries[label] = summarise(samples, results)
        results_store[label] = results

    # Pairwise difference between reference and calc_position if available
    ref_label = SOLVERS["quadratic"]["label"]
    alt_label = SOLVERS.get("calc_position", {}).get("label")
    if ref_label in results_store and alt_label in results_store:
        diff_stats = summarise_difference(samples, results_store[ref_label], results_store[alt_label])
        summaries["difference"] = diff_stats
    print_summary(name, summaries)
    return summaries


def main() -> int:
    rng = random.Random(42)
    baseline_patterns = [
        "clean_baseline_*.jsonl",
        "larger_baseline_*.jsonl",
        "near_singularities_*.jsonl",
        "at_singularities_*.jsonl",
        "outside_singularities_*.jsonl",
        "systematic_bias_*.jsonl",
    ]

    for pat in baseline_patterns:
        run_suite(f"Dataset {pat}", [pat])

    noise_levels = [1.0, 3.0, 5.0]
    clean_samples = load_samples("clean_baseline_*.jsonl")
    for sigma in noise_levels:
        noisy = add_gaussian_noise(clean_samples, sigma_mm=sigma, rng=rng)
        run_suite(f"Noise σ={sigma} mm", [], samples=noisy)

    bias_pattern = [5.0, -5.0, 2.5, -2.5, 0.0]
    biased = add_bias(clean_samples, bias_pattern)
    run_suite("Per-line bias (+/-5 mm)", [], samples=biased)

    # Flex vs no-flex (Pott supports the toggle; other solvers keep flex enabled)
    flex_summaries = {}
    for use_flex in (True, False):
        summaries = {}
        for key, meta in SOLVERS.items():
            if not use_flex and not meta["supports_no_flex"]:
                continue
            results = run_solver(clean_samples, key, use_flex=use_flex)
            label = meta["label"]
            if not use_flex and meta["supports_no_flex"]:
                label = f"{label} (noflex)"
            summaries[label] = summarise(clean_samples, results)
        print_summary(f"Flex toggle (clean baseline, use_flex={use_flex})", summaries)
        flex_summaries.update(summaries)

    # Performance microbench: mix of clean + larger
    perf_samples = load_samples("clean_baseline_*.jsonl") + load_samples("larger_baseline_*.jsonl")
    perf_samples = perf_samples[:1000]
    perf_summaries = {}
    for key, meta in SOLVERS.items():
        results = run_solver(perf_samples, key, use_flex=True)
        perf_summaries[meta["label"]] = summarise(perf_samples, results)
    print_summary("Performance microbench (<=1000 samples)", perf_summaries)

    return 0


if __name__ == "__main__":
    sys.exit(main())
