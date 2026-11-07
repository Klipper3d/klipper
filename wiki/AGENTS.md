# AGENTS.md — LLM Agent 工作流约定

本文件面向 LLM Agent（Trae / Copilot / Cursor 等），定义 AD5M Klipper 项目的工作流约束、编码规范和部署安全红线。

---

## 一、项目定位

FlashForge AD5M 3D 打印机的 Klipper 固件 fork。MCU 侧 STM32F103（eboard），Host 侧 RV1106（Linux），探头 LDC1612 涡流传感器。**本地目录就是源头**（commit 不主动推送远端，用户要求时才 push）。

## 二、前置检查（编码前必须做）

### 2.1 确认改动目标

```bash
# 本地改了哪些文件
git diff --name-only

# 哪些 commit 是 AD5M 自己的（非上游合并）
git log --author="song" --oneline
```

### 2.2 确认设备连通性

```bash
# 先确保能连上设备再改涉及硬件安全的代码
ssh root@192.168.0.30 "pgrep -af klippy.py"
```

### 2.3 确认无残留调试代码

```bash
grep -rn "traceback.print_stack\|logging.info.*debug\|__HO_STATUS__\|TODO.*debug\|FIXME.*test" \
  klippy/extras/ klippy/ | grep -v -i upstream | head -20
```

## 三、编码规范（Python / Klipper 定制）

### 3.1 Klipper 定制 Python 模块结构

```python
class SomeFeature:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.reactor = self.printer.get_reactor()
        # ... register commands
    # cmd_* 方法作为 G-code handler
    def cmd_SOME_COMMAND(self, gcmd):
        ...

def load_config(config):
    return SomeFeature(config)
```

### 3.2 硬件安全红线（不可违反）

| 红线 | 原因 |
|---|---|
| 打印中途绝对不可整机 G28 | homing_override 模板最后 G1 Z210 会撞已打印模型 |
| 归位丢失时用 SET_KINEMATIC_POSITION 软件恢复 | G28 会撞模型；SET_KINEMATIC_POSITION 零物理移动 |
| 移动指令前检查 homed_axes | `{% if 'x' in printer.toolhead.homed_axes %}` |
| 挤出前检查 can_extrude | `{% if printer.extruder.can_extrude %}` |
| 不硬编码温度/速度/位置 | 所有参数从 config 读 |

### 3.3 异常处理模式

```python
# 好：guard 跳过而非 raise（保证宏链能继续）
{% if 'x' in printer.toolhead.homed_axes %}
    G1 X105 Y105 F1800
{% endif %}

# 好：先恢复再执行
{% if 'x' not in printer.toolhead.homed_axes %}
    SET_KINEMATIC_POSITION
{% endif %}
G1 X105 Y105 F1800

# 不好：直接 raise（Klipper 宏遇错即中止整条链）
raise command_error("Must home axis first")
```

### 3.4 运算符优先级陷阱（Python）

```python
# 永远 True！！！不要写
if "a" or "b" in s:

# 正确写法
if any(kw in s for kw in ["a", "b"]):
```

### 3.5 除法保护

```python
# 总是检查除零风险
if self.time_variance > 1e-30:
    new_freq = self.covariance / self.time_variance
else:
    new_freq = self.default_freq
```

### 3.6 日志规范

```python
import logging
logging.info("msg with value %.2f", value)   # 不要用 f-string 拼日志
logging.debug("...")    # 调试消息
# 不要在 production 代码里用 traceback.print_stack()（阻塞 reactor）
```

## 四、部署流程（改 Python 模块后）

```bash
# 1. 先本地 diff 确认改动范围
git diff --stat

# 2. 备份设备原文件
ssh root@192.168.0.30 \
  "cp /opt/klipper/klippy/extras/some_mod.py /opt/klipper/klippy/extras/some_mod.py.bak"

# 3. SCP 上传
scp klippy/extras/some_mod.py root@192.168.0.30:/opt/klipper/klippy/extras/

# 4. 清除 __pycache__（关键！）
ssh root@192.168.0.30 \
  "find /opt/klipper -name '__pycache__' -type d -exec rm -rf {} + 2>/dev/null; \
   find /opt/klipper -name '*.pyc' -delete 2>/dev/null"

# 5. 重启 Klippy
ssh root@192.168.0.30 \
  "kill \$(pgrep -f klippy.py)"

# 6. 验证
#    a) md5 一致
LOCAL=$(md5sum klippy/extras/some_mod.py | cut -d' ' -f1)
REMOTE=$(ssh root@192.168.0.30 "md5sum /opt/klipper/klippy/extras/some_mod.py | cut -d' ' -f1")
test "$LOCAL" = "$REMOTE" && echo OK || echo FAIL

#    b) 无 import/error
ssh root@192.168.0.30 \
  "tail -50 /root/printer_data/logs/klipper.log | grep -iE 'error|traceback|import|exception' || echo OK"

#    c) Stats 正常（每秒一条 "Stats ..."）
ssh root@192.168.0.30 "tail -3 /root/printer_data/logs/klipper.log"
```

### 部署安全红线

| 红线 | 原因 |
|---|---|
| **部署前必须空闲** | 打印中途部署新模块会导致打印失败 |
| **部署后清 __pycache__** | Python 优先加载 .pyc，不改会加载旧字节码 |
| **不要用 FIRMWARE_RESTART JSON body** | 旧版 moonraker 不认 JSON，用 form 编码 |
| **不要整机断电测试 MCU 固件** | 除非确认当前状态安全 |

## 五、commit 规范

### 5.1 何时 commit

- **不主动 commit**。改完告诉用户就行，用户要 push 时再 commit。
- 涉及 `homing_override`、归位逻辑、宏守卫的改动 → 必须 commit（这是 AD5M fork 独有的）
- 纯测试/临时改动 → 不要 commit

### 5.2 commit message 格式

```
type(scope): description

Body: 为什么改、改了什么、有什么影响

AD5M-specific: 是/否
Tested-on-device: 是/否 (IP: 192.168.0.30)
```

### 5.3 SDK overlay 同步

设备端宏配置改动需同步到 SDK overlay：
```
../../../rv1106/rk-sdk/RV1106_IPC_SDK/project/cfg/BoardConfig_IPC/overlay/overlay-gt-buildroot-app/root/printer_data/config/
```
**但 SAVE_CONFIG 自动块不同步**（bed_mesh/mesh/probe 校准数据是设备独立的）。

## 六、调试工作流

### 发现 bug 时的步骤

1. **收集证据链**（不要凭假设改代码）
   - 设备日志：`tail -100 /root/printer_data/logs/klipper.log | grep -iE 'error|traceback'`
   - config dump：`curl -s http://192.168.0.30:7125/printer/configfile.config`
   - git blame：谁写的、什么时候、为了什么

2. **最小改动修复**（不要大段替换）

3. **修了之后必须验证**
   - md5 确认文件一致
   - 无 import error
   - Stats 正常

### 常见坑速查

| 现象 | 先查什么 |
|---|---|
| 改了 .py 不生效 | __pycache__ 清了吗？设备上文件 md5 和本地一致吗？ |
| "Must home axis first" | idle_timeout 是不是清了归位？variable_idle_timeout 设了 86400 吗？ |
| "Unknown homing error: 2147483648" | sensor_ldc1612 rest_ticks==0，需 C2 修复 |
| SD 打印卡住 | virtual_sdcard 的 aio_executor 恢复了吗？ |
| "a" or "b" in s 永远 True | 用 `any(kw in s for kw in [...])` |

## 七、参考文档

- [01-overview.md](01-overview.md) — 项目架构
- [02-modules.md](02-modules.md) — 定制模块详解
- [05-bugs-fixed.md](05-bugs-fixed.md) — 已修复漏洞
- [06-bugs-pending.md](06-bugs-pending.md) — 已知未修复漏洞
- [07-debug.md](07-debug.md) — 调试命令
- [project_memory.md](file:///home/song/.trae-cn/memory/projects/-home-song-samba-work-project-klipper-klipper3d--p2-aca29769d6dfe6f7655b/project_memory.md) — Trae memory 经验教训
