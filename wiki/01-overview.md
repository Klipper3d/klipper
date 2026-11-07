# 01. 项目概述

## 定位

本项目是 FlashForge AD5M 3D 打印机的 Klipper 固件 fork。Klipper 是运行在通用计算机上的 3D 打印控制软件，通过 MCU 固件与硬件交互。AD5M 在 Klipper 上游基础上增加了：

1. **涡流探头支持** — LDC1612 传感器驱动（Python + C 双层），用于 Z 归位和 bed_mesh 扫描
2. **homing_override 部分归位触发修复** — 原版 `G28 X` 会触发完整 XYZ 归位
3. **virtual_sdcard aio_executor 恢复** — 回滚后恢复了上游 threaded io 集成
4. **shaketune 二进制包** — klippain-shaketune-v6.0.0.tar.gz 集成

## 硬件架构

```
┌─────────────────────────────────────────────────────┐
│  RV1106 (Host)                                      │
│  ┌──────────────┐  ttyS4/230400  ┌────────────────┐ │
│  │  Klippy.py   │───────────────▶│  STM32F103xe   │ │
│  │  (Python 3)  │◀───────────────│  (eboard MCU)   │ │
│  └──────────────┘  LDC1612 I2C   └──────┬─────────┘ │
│                                         │           │
│  ┌──────────────┐                      │           │
│  │  Moonraker   │                      ▼           │
│  │  (REST API)  │              ┌──────────────┐    │
│  └──────────────┘              │  LDC1612     │    │
│                                │  涡流探头     │    │
│  日志: /root/printer_data/logs/ │  (Z 归位/扫描) │    │
└─────────────────────────────────────────────────────┘
```

| 组件 | 型号 | 说明 |
|---|---|---|
| Host CPU | Rockchip RV1106 | 运行 Klippy + Moonraker |
| MCU | STM32F103xe | eboard 子板 |
| 通信 | UART ttyS4 @ 230400 | Host ↔ MCU |
| 探头 | LDC1612 (Cartographer) | I2C，涡流检测 |
| 运动学 | CoreXY | AD5M 采用 |
| 构建交叉工具链 | arm-none-eabi-gcc / arm-rockchip830-gcc | 见 [03-build-deploy.md](03-build-deploy.md) |

## 软件架构

```
klipper3d/
├── klippy/                 # Python 层
│   ├── reactor.py          # 事件循环 / 定时器 / GC
│   ├── gcode.py            # G-code 调度
│   ├── clocksync.py        # MCU ↔ Host 时钟同步
│   ├── chelper/            # C 扩展（stepcompress, motion queue）
│   └── extras/             # 功能模块
│       ├── probe_eddy_ng.py      # 涡流探头 Python 主入口（AD5M 定制）
│       ├── ldc1612_ng.py         # LDC1612 I2C 封装（AD5M 定制）
│       ├── homing_override.py    # 部分归位触发修复（AD5M 定制）
│       ├── virtual_sdcard.py     # SD 卡打印（AD5M 修 aio_executor + 删硬编码温度）
│       ├── aio_executor.py       # threaded io executor（Timofey Titovets 上游）
│       └── ...                  # 其他上游模块
├── src/                    # C 层（MCU 固件）
│   ├── sensor_ldc1612_ng.c       # LDC1612 传感器驱动（AD5M 定制）
│   ├── sensor_ldc1612_ng.h
│   └── ...                      # 其他 MCU 固件（Klipper 上游）
├── Makefile-rv1106         # Host 固件构建
├── build_eboard.sh         # MCU 固件构建脚本
└── Makefile                # 通用构建
```

## 与 Klipper 上游的差异

**上游 fork 来源**：`git@github.com:snsicam/klipper3d.git`（AD5M 自有仓库，基于 Klipper 主分支）

**AD5M 自有 commit（4 个）**：

| Commit | 作者 | 内容 |
|---|---|---|
| `378e62ec` | song | homing_override 仅无参数 G28 触发 override |
| `0da5a425` | song | sensor_ldc1612_ng 容忍 I2C 瞬态错误 |
| `d83031df` | song | 添加 klippain-shaketune 二进制包 |
| `17753205` | song | 添加 eddy-coil 支持（最大 commit，夹带改了 virtual_sdcard/clocksync/多个 Python/C 文件） |

**合并进来的上游 commit**（主要来自 Kevin O'Connor、Timofey Titovets）：reactor GC、aio_executor threaded io、clocksync 重构、serialqueue FD 关闭修复等。这些都不是 AD5M 自己写的。

**AD5M 实际改动的 Python 文件（grep blame 定位）**：

| 文件 | AD5M 改了什么 |
|---|---|
| `klippy/extras/homing_override.py` | `cmd_G28` 加 no_axis 判断（378e62ec） |
| `klippy/extras/virtual_sdcard.py` | 17753205 加 `_check_and_set_extruder_temperature`（后来删除）、删 aio_executor（后来恢复）、加 load_file event |
| `klippy/clocksync.py` | 17753205 回滚了上游重构（删 return True）、l114 加除零 guard |

**AD5M 新增文件**：

| 文件 | 类型 | 说明 |
|---|---|---|
| `klippy/extras/probe_eddy_ng.py` | Python | 涡流探头主逻辑（3497 行） |
| `klippy/extras/ldc1612_ng.py` | Python | LDC1612 I2C 封装 |
| `src/sensor_ldc1612_ng.c` | C | MCU 侧 LDC1612 驱动 |
| `src/sensor_ldc1612_ng.h` | C header | 同上 |
| `klippy/extras/probe_eddy_ng - 副本.py` | Python | 备份文件（应删除） |
| `klippain-shaketune-v6.0.0.tar.gz` | Binary | ShakeTune 集成包 |
