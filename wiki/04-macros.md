# 04. 宏系统设计

## AD5M 配置文件结构

```
printer.cfg
  ├── include printer.base.cfg      # corexy, pin 定义, 传感器
  ├── include client.cfg             # PAUSE/RESUME/CANCEL_PRINT 重写
  ├── include macros.cfg             # 功能宏（START_PRINT/END_PRINT/PROBE_EDDY...）
  ├── include macros-marlin.cfg      # Marlin 兼容性宏
  ├── include timelapse.cfg          # Timelapse 配置
  ├── include power_loss_recovery.cfg # PLR 断电恢复
  └── [force_move] enable_force_move: True  # 关键！SET_KINEMATIC_POSITION 依赖
```

## 归位守卫（homing_override）

### 模板（设备端）

```gcode
[homing_override]
axes: XYZ
set_position_z: 0
gcode:
    M204 S300                       ; 降加速度
    G28 Z0                          ; 递归 → prev_G28（in_script=True）
    G28 X0                          ; 递归 → prev_G28
    G28 Y0                          ; 递归 → prev_G28
    G1 X105 Y105 Z210 F1800         ; 安全位置
```

### 之前的问题（C2 漏洞）

原版 `homing_override.py` 的逻辑：

```python
for axis in self.axes:    # axes='XYZ'
    if gcmd.get(axis, None) is not None:  # G28 X → True
        override = True    # 触发完整 override ❌
```

任何带轴参数的 G28 都触发完整 XYZ 归位。在打印中途执行 `G28 Z` → 完整 override → 床走到 Z=220 → **撞已打印模型**。

### 修复后（378e62ec）

```python
no_axis = True
for axis in 'XYZ':
    if gcmd.get(axis, None) is not None:
        no_axis = False
        break

if not no_axis:
    self.prev_G28(gcmd)    # 直接调真实 G28
    return
```

| G-code | 修复前 | 修复后 |
|---|---|---|
| `G28` (无参) | 完整 override ✅ | 完整 override ✅ |
| `G28 X` | 完整 override ❌ | 仅归位 X ✅ |
| `G28 Z0` | 完整 override ❌ | 仅归位 Z ✅ |
| `G28 X Y` | 完整 override ❌ | 仅归位 X+Y ✅ |

---

## PAUSE / RESUME / CANCEL_PRINT 守卫

### 根因链

```
暂停搁置 ≥10min
  → idle_timeout (600s) 触发 TURN_OFF_HEATERS + M84
  → M84 → stepper_enable.motor_off() → clear_homing_state("xyz")
  → 归位标志被清，但 toolhead.commanded_pos 保留
  → 点 RESUME / CANCEL_PRINT
  → 宏内第一处含 XYZ 的移动指令报 "Must home axis first"
  → Klipper 宏遇错即中止整条链
  → CANCEL_PRINT_BASE 永远执行不到
  → 任务卡在 paused → UI 报"取消打印失败"
```

### 修复（client.cfg）

```gcode
[gcode_macro PAUSE]
rename_existing: PAUSE_BASE
variable_idle_timeout: 86400   # 24h，防止 idle_timeout 触发 motor_off
gcode:
    SET_VARIABLE PAUSE_QUEUEING PAUSE
    M117 PAUSE
    PAUSE_BASE
    _TOOLHEAD_PARK_PAUSE_CANCEL   # 移动到安全位置

[gcode_macro RESUME]
rename_existing: RESUME_BASE
gcode:
    {% if printer.idle_timeout.state == "Idle" %}
        {% set idle_timeout = 600 %}
    {% else %}
        {% set idle_timeout = 86400 %}
    {% endif %}
    SET_GCODE_VARIABLE MACRO RESUME VARIABLE_IDLE_TIMEOUT {idle_timeout}
    {% if 'x' not in printer.toolhead.homed_axes %}
        SET_KINEMATIC_POSITION   # 软件恢复归位，零物理移动
    {% endif %}
    RESUME_BASE

[gcode_macro CANCEL_PRINT]
rename_existing: CANCEL_PRINT_BASE
gcode:
    _USER_CANCEL_MACRO
    CANCEL_PRINT_BASE
```

### END_PRINT 守卫

```gcode
[gcode_macro END_PRINT]
variable_idle_timeout: 600
gcode:
    {% if 'x' in printer.toolhead.homed_axes %}
        G91 G1 Z+0.5 F1000   # 抬喷头（仅归位已建立时）
        G90
    {% endif %}
    {% if printer.extruder.can_extrude %}
        G1 E-0.8 F3500        # 回抽（仅热端够热时）
    {% endif %}
    ...
    _COMMON_END_PRINT
```

`_COMMON_END_PRINT` 也加了归位守卫——关键是保证 CANCEL_PRINT_BASE 能执行到，否则任务永久卡 paused。

### SET_KINEMATIC_POSITION 语义

- **纯软件操作**：只调 `toolhead.set_position()`，不驱动电机
- 无参时取 `toolhead.get_position()`（即 `commanded_pos`，motor_off 不丢这个）
- 把 xyz 标记为已归位
- 由 `force_move.py` 条件注册，必须启用 `[force_move] enable_force_move: True`

---

## PLR（断电恢复）问题

**之前的设计缺陷（C1 漏洞）**：

```gcode
[power_loss_recovery]
# 恢复流程
_before_resume_gcode:
    G28 X                  # 触发完整 override（C2）
    G28 Y                  # 又一次完整 override
    SET_KINEMATIC_POSITION Z=0   # 软件基准错了！物理 Z=210，软件设 Z=0
```

归位后 Z≈210（override 最后一步 `G1 Z210`），但 PLR 宏硬设 `Z=0`——让软件基准与物理位置脱节 210mm：

```
归位后: 物理 Z=210, 软件 Z=210 ✅
SET_KINEMATIC_POSITION Z=0: 物理 Z=210, 软件 Z=0 ❌
G1 Z=2.5: 软件认为 Z 从 0→2.5, 物理从 210→212.5（空中!）
```

**修复**：删掉 PLR 里的 `SET_KINEMATIC_POSITION Z=0`，归位已建立正确的 Z 基准。

---

## Timelapse 守卫绕开

**之前的问题（C3/M1 漏洞）**：

```jinja
# timelapse.cfg 里
{printer.configfile.settings['gcode_macro pause'].rename_existing}   # 硬调 PAUSE_BASE
{printer.configfile.settings['gcode_macro resume'].rename_existing}  # 硬调 RESUME_BASE
```

渲染期间 idle_timeout 可能触发清归位，RESUME_BASE 会死。

**修复**：改成直接调 PAUSE/RESUME 宏（带守卫版本）。

---

## 宏执行安全模型

Klipper 宏系统的关键特性：**宏遇错即中止整条链**。这意味着：

1. CANCEL_PRINT 宏里，任何一步 raise → CANCEL_PRINT_BASE 永远执行不到
2. PAUSE 期间 motor_off 清归位 → 后续任何含移动指令的宏都死
3. 修复模式：**在最前面做恢复性检查**（归位守卫、SET_KINEMATIC_POSITION），在关键路径前加守卫跳过而非报错

通用修复模式：

```gcode
# 1. 状态检查 + 恢复
{% if 'x' not in printer.toolhead.homed_axes %}
    SET_KINEMATIC_POSITION   # 零物理移动恢复
{% endif %}

# 2. 条件守卫（跳过而非报错）
{% if 'x' in printer.toolhead.homed_axes %}
    G1 X105 Y105 F1800       # 只有归位已建立才执行
{% endif %}
```
