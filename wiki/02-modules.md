# 02. 定制模块详解

## probe_eddy_ng.py — 涡流探头 Python 主入口

**文件**：`klippy/extras/probe_eddy_ng.py`（3497 行）  
**Config section**：`[probe_eddy_ng]` 或 `[probe_eddy_ng <name>]`  
**Entry point**：`load_config()` 返回 `ProbeEddy` 实例（L431 `__init__`）

### 核心类结构

```
ProbeEddyParams          L142   配置参数解析 + 校验
ProbeEddyProbeResult     L378   单次探测结果封装（valid/value/stddev）
ProbeEddy                L431   主类，gcode 命令注册 + 高层操作
 ├─ TapResult            L1753  tap 微调结果
 ├─ TapConfig            L1763  tap 参数
 └─ ProbeEddySampler     L2749  底层采样器（greenlet）
ProbeEddyScanningProbe   L2364  bed_mesh/QGL 扫描实现
ProbeEddyEndstopWrapper  L2497  作为 Z endstop 用的 wrapper
ProbeEddyFrequencyMap    L3010  频率 ↔ 高度多项式映射
BedMeshScanHelper        L3347  bed_mesh 点遍历辅助
```

### 配置参数（常见）

```ini
[probe_eddy_ng]
sensor_type: ldc1612_ng     # LDC1612 型号
home_trigger_height: 2.0    # Z 归位触发高度（mm）
tap_mode: true              # 启用 tap 微调
tap_start_z: 3.0            # tap 起始高度
tap_target_z: 0.1           # tap 目标高度
reg_drive_current: 1750     # 调节模式驱动电流 (mA)
tap_drive_current: 1250     # tap 模式驱动电流 (mA)
```

### G-code 命令

| 命令 | 位置 | 说明 |
|---|---|---|
| `PROBE` | L1028 | 单次探测 |
| `PROBE_STATIC` | L1046 | 静态探测（固定位置） |
| `PROBE_ACCURACY` | L864 | 精度测试（默认 Z=5.0 开始） |
| `AUTO_CALIBRATE` | L1324 | 自动频率校准 |
| `CALIBRATE` | L1243 | 手动校准 |
| `SET_TAP_OFFSET` | L967 | 设 tap offset |
| `CLEAR_CALIBRATION` | L943 | 清除校准 |

### 已知设计问题（见 06-bugs-pending.md）

- L1833 `if "a" or "b" in s` 永远 True（运算符优先级）
- L3320 单值 vs 批量边界条件不一致
- L3166 low/high 样本区间重叠 0.5mm

---

## sensor_ldc1612_ng.c — MCU 侧 LDC1612 驱动

**文件**：`src/sensor_ldc1612_ng.c`（1001 行）+ `.h`  
**MCU 命令**：`config_ldc1612_ng`（Python 侧 `ldc1612_ng.py` 注册）

### 硬件支持的产品型号

```c
PRODUCT_BTT_EDDY_CURRENT    // BTT Eddy Current
PRODUCT_CARTOGRAPHER        // Carbide3D Cartographer (AD5M 用)
PRODUCT_EUMAKER_REVO        // Eumaker Revo
PRODUCT_BTT_EDDY_V2         // BTT v2
```

### 关键数据结构

| 结构 | 说明 |
|---|---|
| `struct ldc1612_ng_s` | 主结构：i2c master、product、sample config、drive current |
| `struct ldc1612_ng_homing_s` | homing 专用：trigger time、home mode、reason |

### 错误码

```c
#define REASON_BASE     0x80000000  // 偏移基准
#define REASON_ERROR_SENSOR    (REASON_BASE + 0)  // 传感器错误
#define REASON_TOO_LOW         (REASON_BASE + 1)  // 触发高度过低
#define REASON_TOO_EARLY       (REASON_BASE + 2)  // 过早触发
```

### I2C 错误容忍（0da5a425 commit）

```c
#define LDC_I2C_MAX_CONSECUTIVE_ERRS 16   // off-by-one，实际容忍 17 次
```

I2C 单次 NACK 不会立即 shutdown，累积到阈值才炸。

### 已知设计问题（见 06-bugs-pending.md）

- L430 LSB 读取失败时 `lastval` 保持过期值
- L544 `rest_ticks==0` 用 `other_reason_base` 导致 Unknown homing error
- L292 `i2c_err_count > 16` 应为 `>= 16`

---

## homing_override.py — 部分归位触发修复

**文件**：`klippy/extras/homing_override.py`（61 行）  
**Config section**：`[homing_override] axes: XYZ`

### 原版行为（有 bug）

```python
for axis in self.axes:           # axes='XYZ'
    if gcmd.get(axis, None) is not None:  # G28 X → X is not None
        override = True         # 触发完整 XYZ override 模板 ❌
```

任何带 XYZ 参数的 G28（`G28 X`、`G28 Z0`、`G28 X Y`）都触发完整 XYZ 归位。

### 修复后行为（378e62ec）

```python
no_axis = True
for axis in 'XYZ':
    if gcmd.get(axis, None) is not None:
        no_axis = False
        break

if not no_axis:
    self.prev_G28(gcmd)         # 直接调真实 G28，跳过 override ✅
```

只有**无参数 G28**（完整归位）触发 override 模板。`G28 X` 只归位 X。

### 设备 override 模板（AD5M）

```gcode
[homing_override]
axes: XYZ
set_position_z: 0
gcode:
    M204 S300           ; 降加速度
    G28 Z0              ; 递归 → prev_G28（因为 in_script=True）
    G28 X0              ; 递归 → prev_G28
    G28 Y0              ; 递归 → prev_G28
    G1 X105 Y105 Z210 F1800   ; 移动到安全位置
```

递归保护：模板内的 `G28 X0/Y0/Z0` 因为 `in_script=True`，走真实 `prev_G28`。

---

## virtual_sdcard.py — SD 卡打印

**文件**：`klippy/extras/virtual_sdcard.py`  
**Config section**：`[virtual_sdcard] path: /root/gcodes`

### AD5M 改过的点

| 版本 | 改动 | 状态 |
|---|---|---|
| 17753205 | 加 `_check_and_set_extruder_temperature`（硬编码 220°C + wait=False） | ❌ 已删 |
| 17753205 | 删上游 aio_executor 集成 | ❌ 已恢复 |
| 当前 HEAD | 恢复 aio_executor + 硬编码函数 | ✅ |

### 当前文件打开方式

```python
# 从上游 Timofey Titovets 的 threaded io 集成
aio = self.printer.load_object(config, 'aio_executor')
self.executor = aio.allocate_executor("virtual_sdcard")

# _load_file 里
f = self.executor.submit(io.open, fname, 'rb', buffering=0)
f = self.executor.wrap_obj(f)
f = io.BufferedReader(f)
f = io.TextIOWrapper(f, newline='')
```

`aio_executor.py` 把阻塞 io 放到独立线程，避免 SD 卡读取阻塞 Klippy reactor 主循环导致 TRSYNC 超时。

---

## clocksync.py — MCU ↔ Host 时钟同步

**文件**：`klippy/clocksync.py`（217 行）

### 原理

```
_host monotonic time (sent_time)
MCU clock count (clock)
→ 线性回归: clock = freq * sent_time + offset
→ 周期性 (0.9839s) get_clock query 更新 freq
```

### AD5M 改过的点

| 版本 | 改动 |
|---|---|
| 17753205 | 回滚 Kevin O'Connor 上游重构（把 `_handle_clock` 拆成子函数，AD5M 合并回去） |
| HEAD | L114 加 `time_variance > 1e-30` guard（除零保护） |

### 除零 guard

```python
if self.time_variance > 1e-30:
    new_freq = self.clock_covariance / self.time_variance
else:
    new_freq = self.mcu_freq   # fallback 到原始频率
```

实际触发概率极低：`connect()` 里先跑 8 次 `_handle_clock`，`time_variance` 必非零。加 guard 只是保险。
