## Synthetic Dataset Usage

Paths:
- `datasets/<scenario>_<geometry>.jsonl`

Each `.jsonl` file has one JSON object per line:
- `dataset`: scenario name (`clean_baseline`, `larger_baseline`, `near_singularities`, `at_singularities`, `outside_singularities`, `systematic_bias`)
- `geometry`: `HP5`, `Slideprinter`, `Spidercam`, or `CubeCorners`
- `anchor_set`: 0-based set index (only `larger_baseline` has three sets; others use set 0)
- `anchors`: list of anchor `[x, y, z]` in mm; length depends on geometry (HP5=5, Slideprinter=3, Spidercam=4, CubeCorners=8)
- `real_xyz`: list of mover poses (mm) to feed forward transforms
- `config`: overrides for generation; only `systematic_bias` sets `{"min_force": 50.0}`; others are `{}`. See `generate_synthetic_data.py` for available keys (spool radii, buildup factor, gear teeth, mechanical_advantage, lines_per_spool, spring_k_per_unit_length, mover_weight, min/max force, guy_wire_lengths, use_flex, ignore_gravity, ignore_pretension, lambda_reg, tol, max_iters_target, g).
- `config_used`: fully resolved values passed to `pos_to_motor_pos_samples` (arrays are lists). Includes `spool_buildup_factor`, `spool_r_in_origin`, `spool_gear_teeth`, `motor_gear_teeth`, `spool_to_motor_gearing_factor`, `mechanical_advantage`, `lines_per_spool`, `min_force`, `max_force`, `spring_k_per_unit_length`, `mover_weight`, `use_flex`, `ignore_gravity`, `ignore_pretension`, `lambda_reg`, `tol`, `max_iters_target`, `g`, and optional `guy_wire_lengths`.
- `motor_samples`: generated motor positions in degrees (360° per rotation), matching `real_xyz` order; five columns for HP5, etc.

Scenarios mapped to the suggested comparison suite:
- Clean baseline → suite #1 (no noise, flex enabled); HP5 positions mirror `simulation_data_for_hangprinter_forward_transform.py`; Slideprinter is Z=0 projection of those points.
- Larger baseline → suite #2 (3 anchor sets per geometry, 10 random poses each; ≥1000×1000 mm reach).
- Near/At/Outside singularities → suite #6 boundary stress: HP5/Slideprinter points projected radially to the reachable boundary (−0.1 mm inside, exactly on, +0.1 mm outside). Spidercam/CubeCorners use rectangle footprints similarly.
- Systematic bias → suite #4 pretension variant (min_force=50).
Noise/quantization/bias for other suites (#3, #5, #8, #9, #10) should be injected in the C++ tests themselves.

Practical hints for the C++ forward-transform tests:
- Inputs are degrees, not steps; convert if your harness assumes steps (gear ratio 255/20, mechAdv HP5 default [2,2,2,2,4]). For non-HP5 geometries, check the anchor count and use the corresponding motor column count.
- Each line is standalone JSON; your loader can stream line by line. The JSON is compact (no extra spacing), and keys stay in the order written by the generator (`dataset`, `geometry`, `anchor_set`, `anchors`, `real_xyz`, `config`, `motor_samples`).
- To add noise or biases, perturb `motor_samples` or derived lengths in your test harness (e.g., Gaussian σ=1–5 mm on lengths, per-line constant offset for bias, 2–5% mislength for infeasible mixes, quantize to encoder resolution).
- Flex is enabled in these datasets (generation default `use_flex=true`); if you need non-flex variants, re-run generation with `config.use_flex=false`.
- Anchor set 0 is the canonical geometry per type; sets 1 and 2 (only in `larger_baseline`) are larger/smaller variants to test generality.

Files on disk are compact JSONL (one object per line, no extra whitespace); keep that form for downstream parsers.***
