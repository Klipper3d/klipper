# 06. 已知未修复漏洞清单

**这些漏洞已记录，暂不修。** 详见 `project_memory.md` 的 "Pending Code Bugs" 章节。

---

## probe_eddy_ng.py — Python 层

### C1. do_one_tap 异常处理永远 True（运算符优先级）

| 项 | 值 |
|---|---|
| 位置 | `klippy/extras/probe_eddy_ng.py` L1833 |
| 代码 | `if "Sensor error" or "Probe completed movement" or "Probe triggered prior" in str(err):` |
| 等价于 | `if ("Sensor error") or ("Probe completed movement") or ("Probe triggered prior" in str(err)):` |
| 问题 | 前两个非空字符串永远 True → if 永远 True → 所有 tap 错误都走 return TapResult(error=err) 而非 raise |
| 后果 | 任何 Klippy 内部错误（包括不该忽略的）都被降级成 tap 失败，上层可能误判为"可以重试" |
| 修复 | `if any(kw in str(err) for kw in ["Sensor error", "Probe completed movement", "Probe triggered prior"]):` |

### C2. freq_to_height 单值 vs 批量边界条件不一致

| 项 | 值 |
|---|---|
| 位置 | 单值版 L3320 vs 批量版 L3330 |
| 单值版 | `if self._ftoh_high is not None and invfreq < self._ftoh.domain[0]:` |
| 批量版 | `if invfreqs > self._ftoh.domain[1]:` |
| 问题 | 单值版检查下界（`invfreq < domain[0]`），批量版检查上界（`invfreq > domain[1]`） |
| 根因 | `_ftoh` 多项式的 x 域是 `domain = [min, max]`（近床区，小 invfreq 值）。远床区 invfreq > domain[1] 应使用 high fn。单值版逻辑反了。 |
| 触发 | 远床区单值查询（`cmd_PROBE_STATIC` / `height_to_freq` / `freq_to_height`）全部错误地用 low fn |
| 修复 | L3320 改成 `if self._ftoh_high is not None and invfreq > self._ftoh.domain[1]:` |

### C3. low/high 样本区间重叠 0.5mm

| 项 | 值 |
|---|---|
| 位置 | L3165-3166 |
| 代码 | `low_samples = heights <= 5.0` + `high_samples = heights >= 4.5` |
| 问题 | 重叠区 4.5~5.0mm 的样本同时进入 low 和 high 多项式拟合 |
| 后果 | 边界附近探头读数可能不连续 |
| 修复 | `high_samples = heights > 5.0`（无重叠） |

### M2. non-rapid scan start_time off-by-one

| 项 | 值 |
|---|---|
| 位置 | L2427-2428 |
| 代码 | `th.dwell(sample_time_delay)` → `start_time = th.get_last_move_time()` |
| 问题 | `get_last_move_time()` 返回 dwell **前**的 move 结束时间，注释说应该是 dwell **后**的稳态时间 |
| 后果 | `find_height_at_time` 可能拿到移动中的非稳态读数 |

### M3. Sampler.ffinish 竞态时 raise

| 项 | 值 |
|---|---|
| 位置 | L2827 |
| 问题 | `if self.eddy._sampler is not self: raise command_error(...)` |
| 触发 | `_handle_command_error` 和 `_handle_homing_move_end` 都可能独立调 `sampler.finish()`。竞态时 `eddy._sampler` 可能已被替换。 |
| 后果 | raise 后 homing 状态残留（`_homing_in_progress = True` 没清） |

### M3b. traceback.print_stack 阻塞 reactor

| 项 | 值 |
|---|---|
| 位置 | L2892 |
| 问题 | `if max_wait_time > 30.0: traceback.print_stack()` 在 reactor 线程打印整个调用栈，可能阻塞数毫秒 |
| 后果 | 某些情况下触发 MCU TRSYNC_TIMEOUT（100ms） |

---

## sensor_ldc1612_ng.c — MCU 固件层

### C1. LSB 读取失败时返回过期值

| 项 | 值 |
|---|---|
| 位置 | `src/sensor_ldc1612_ng.c` L430-438 |
| 代码 | 两次 `read_reg` 读 MSB/LSB，只有两次都成功才更新 `lastval` 和 `i2c_err_count=0` |
| 问题 | LSB 失败时 `lastval` 保持旧值，`i2c_err_count` 也没清 → Python 侧看到旧数据以为传感器没变化 + 后续 15 次失败后直接炸 |
| 修复 | 两次 `read_reg` 独立处理错误，LSB 失败时也调 `note_i2c_error(ld, ret)` |

### C2. rest_ticks==0 Unknown homing error

| 项 | 值 |
|---|---|
| 位置 | L541-546 |
| 代码 | `if (ld->rest_ticks == 0) notify_trigger(ld, 0, other_reason_base)` |
| 问题 | 传感器未启动（rest_ticks==0）时用 `REASON_BASE` 作为 reason → Python 侧映射不到明确错误码 → 显示 "Unknown homing error: 2147483648" |
| 修复 | 改成 `other_reason_base + REASON_ERROR_SENSOR` → Python 侧显示 "Sensor error" |

### M1. i2c_err_count off-by-one

| 项 | 值 |
|---|---|
| 位置 | L292 |
| 代码 | `if (++ld->i2c_err_count > LDC_I2C_MAX_CONSECUTIVE_ERRS)` |
| 问题 | `>` 而非 `>=`，实际容忍 17 次连续错误才 shutdown（注释写 16） |
| 影响 | 轻微冗余，多容忍一次 I2C 失败 → 多跑一次完整 Z 归位 |

### M2. pull_probed_results 空 _notes 无保护

| 项 | 值 |
|---|---|
| 位置 | probe_eddy_ng.py L2438 |
| 代码 | `self._sampler.wait_for_sample_at_time(self._notes[-1][0] + self._sample_time)` |
| 问题 | bed_mesh/QGL 所有 probe 点跳过（或用户中途中断）→ `_notes = []` → `[-1]` IndexError |
| 后果 | 宏链中断 |
| 修复 | 函数开头加 `if not self._notes: return []` |

---

## AD5M 宏配置（非代码）

### START_PRINT SKEW_PROFILE 静默失败

| 项 | 值 |
|---|---|
| 文件 | `macros.cfg` L48 |
| 问题 | `SKEW_PROFILE LOAD=skew_profile`，但设备没有 `[skew_correction]` section → Klipper 不报错也不生效 |
| 修复 | 加 guard：`{% if 'skew_correction' in printer.configfile.settings %} SKEW_PROFILE LOAD=skew_profile {% endif %}` 或直接删 |

### START_SPEED_TEST 默认 EXTRUDER_TEMP=0

| 项 | 值 |
|---|---|
| 文件 | `macros.cfg` L725 |
| 问题 | 默认 EXTRUDER_TEMP=0 → 跳过加热 → CLEAN_NOZZLE 把热端冷却到 0°C → 后续挤出无 can_extrude 守卫 |
| 修复 | 默认值改 200 或加 can_extrude 检查 |

### PROBE_ACCURACY 默认 Z=5.0 可能超校准范围

| 项 | 值 |
|---|---|
| 文件 | probe_eddy_ng.py L871 |
| 问题 | `PROBE_ACCURACY Z=5.0`，如果用户校准范围最大只有 3mm，会返回 extrapolated 值 |
| 修复 | 默认值从配置读取或改为 tap_start_z |

---

## 修复优先级

| 优先级 | 问题 | 理由 |
|---|---|---|
| **P0** | probe_eddy_ng C1 | 运算符优先级经典 bug，所有 tap 错误被吞 |
| **P0** | probe_eddy_ng C2 | 单值 vs 批量边界反 |
| **P1** | sensor_ldc1612 C1 | LSB 失败返回旧值 + 17 次错误后炸固件 |
| **P1** | sensor_ldc1612 C2 | Unknown homing error 调试困难 |
| **P1** | probe_eddy_ng C3 | 边界样本重叠 |
| **P2** | sensor_ldc1612 M2 | IndexError 宏链死 |
| **P2** | probe_eddy_ng M2/M3/M3b | 精度/竞态/reactor 阻塞 |
| **P3** | sensor_ldc1612 M1 | off-by-one |
| **P3** | 宏配置项 | 体验改善 |
