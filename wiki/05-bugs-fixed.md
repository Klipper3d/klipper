# 05. 已修复漏洞清单

## 2026-10-19 修复批次

### 🔴 Critical

#### #1 homing_override 部分 G28 触发完整归位（撞模型）

| 项 | 值 |
|---|---|
| 文件 | `klippy/extras/homing_override.py` |
| Commit | `378e62ec` |
| 问题 | `G28 X` / `G28 Z0` / `G28 X Y` 都触发完整 XYZ override 模板 → 打印中途撞已打印模型 |
| 修复 | 只有无参数 G28 触发 override，有参数 G28 直接调 `prev_G28` |
| 验证 | `G28 X` 后 `printer.toolhead.position` 仅 X 变，Y/Z 不动；`G28` 完整触发 override 模板 |

#### #2 PLR 断电恢复 SET_KINEMATIC_POSITION Z=0 空中打印

| 项 | 值 |
|---|---|
| 文件 | `power_loss_recovery.cfg`（设备端） |
| Commit | 设备端手动修改 |
| 问题 | 归位后 Z≈210（override `G1 Z210`），PLR 宏硬设 Z=0 → 软件基准与物理位置脱节 210mm → 恢复移动在空中打印 |
| 修复 | 删掉 PLR 里的 `SET_KINEMATIC_POSITION Z=0`，归位已建立正确基准 |
| 影响 | **数据恢复关键路径**，不修等于白断电 |

#### #3 virtual_sdcard 硬编码 220°C + wait=False 冷打印

| 项 | 值 |
|---|---|
| 文件 | `klippy/extras/virtual_sdcard.py` |
| Commit | 未 commit（本地 HEAD） |
| 引入 | AD5M commit `17753205` "add eddy-coil" |
| 问题 | `work_handler` 开头调 `_check_and_set_extruder_temperature` → 硬编码 220°C + `wait=False` → 热端还没到温就挤出；乱码注释 `220??C` |
| 修复 | 删掉整个 `_check_and_set_extruder_temperature` 函数 + 调用点 + 乱码注释 |
| 影响 | 切片文件没 M109/M190 等待温度时冷料堵嘴 |

### 🟡 Medium

#### #4 motor_off 清归位 → RESUME/CANCEL 宏链死

| 项 | 值 |
|---|---|
| 文件 | `client.cfg`（设备端 config） |
| 问题 | PAUSE 搁置 ≥10min → idle_timeout 触发 M84 → 清归位 → RESUME 宏内首次移动报 "Must home axis first" → 宏链中止 → CANCEL_PRINT_BASE 永远执行不到 → 任务卡 paused |
| 修复 | 三层防御：① PAUSE 宏设 `variable_idle_timeout: 86400` 防 motor_off；② RESUME 宏开头 `if 'x' not in homed_axes: SET_KINEMATIC_POSITION` 软件恢复；③ END_PRINT/_COMMON_END_PRINT 移动前加 homed/can_extrude 守卫（跳过而非报错） |
| 前置条件 | `[force_move] enable_force_move: True`（已加 printer.cfg） |

#### #5 virtual_sdcard 回滚上游 aio_executor

| 项 | 值 |
|---|---|
| 文件 | `klippy/extras/virtual_sdcard.py` |
| 引入 | AD5M commit `17753205` |
| 问题 | 删掉了 Timofey Titovets 的 threaded io 集成（commit `cb020566`）→ SD 卡 IO 阻塞 reactor 主循环 → 大文件可能触发 TRSYNC_TIMEOUT |
| 修复 | 恢复 `aio_executor.allocate_executor("virtual_sdcard")` + `executor.submit(io.open, ...)` + `WrapperAIO` 包装 |
| 验证 | 本地 ext4 分区 read(8192) 微秒级，实际影响低，但恢复是正确方向 |

#### #6 timelapse 渲染硬调 PAUSE_BASE/RESUME_BASE

| 项 | 值 |
|---|---|
| 文件 | `timelapse.cfg`（设备端 config） |
| 问题 | `TIMELAPSE_TAKE_FRAME` 和 `TIMELAPSE_RENDER` 用 `rename_existing` 取 PAUSE_BASE/RESUME_BASE，绕过 client.cfg 里的守卫宏 |
| 修复 | 改成直接调 PAUSE/RESUME 宏 |

#### #7 _SET_TIMELAPSE_SETUP 潜伏回归点

| 项 | 值 |
|---|---|
| 文件 | `timelapse.cfg` |
| 问题 | 这个宏会把 `variable_macro` 覆盖成 `PAUSE_BASE/RESUME_BASE`——所有守卫全部失效 |
| 修复 | 覆盖值改成 PAUSE/RESUME 而非 rename_existing |

### 🟢 Low

#### #8 clocksync time_variance 除零

| 项 | 值 |
|---|---|
| 文件 | `klippy/clocksync.py` L114 |
| Commit | 未 commit（本地 HEAD） |
| 问题 | `new_freq = clock_covariance / time_variance`，极罕见 RTC 零抖动时除零 |
| 修复 | 加 `if time_variance > 1e-30: new_freq = ... else: new_freq = mcu_freq` |
| 实际概率 | 极低（connect() 先跑 8 次 _handle_clock，time_variance 必非零） |

#### #9 M300 被 macros-marlin.cfg 静默

| 项 | 值 |
|---|---|
| 文件 | `macros-marlin.cfg` |
| 问题 | M300 定义成 no-op（设备无扬声器硬件），取消打印警告音不响 |
| 状态 | 功能正常（设备确实没有扬声器），只是没声音反馈 |

#### #10 SET_SKEW/RESET_SKEW 无提示

| 项 | 值 |
|---|---|
| 文件 | `macros-marlin.cfg` |
| 问题 | 空实现无 RESPOND 提示，用户看不出没生效 |
| 修复 | 加 RESPOND MSG='skew_correction not configured, ignoring' |

---

## 修复优先级回顾

| 优先级 | 问题 | 状态 | 理由 |
|---|---|---|---|
| **P0** | #3 virtual_sdcard 硬编码温度 | ✅ 已修 | 冷料堵嘴直接影响打印质量 |
| **P0** | #1 homing_override 撞模型 | ✅ 已修 | 物理安全 |
| **P0** | #4 RESUME/CANCEL 宏链死 | ✅ 已修 | 任务卡 paused |
| **P1** | #2 PLR Z=0 空中打印 | ✅ 已修 | 断电恢复关键路径 |
| **P1** | #5 aio_executor 恢复 | ✅ 已修 | 预防大文件 TRSYNC |
| **P1** | #6 timelapse 绕守卫 | ✅ 已修 | 守卫一致性 |
| **P2** | #7 _SET_TIMELAPSE_SETUP | ✅ 已修 | 潜伏回归 |
| **P2** | #8 clocksync 除零 | ✅ 已修 | 保险 |
| P3 | #9/#10 Low 项 | ⏸️ 可选 | 体验改善 |
