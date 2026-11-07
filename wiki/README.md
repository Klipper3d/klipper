# AD5M Klipper Wiki

FlashForge AD5M 定制版 Klipper 项目知识库。本 wiki 面向 LLM Agent 与项目开发者，记录硬件架构、定制模块、已知漏洞与修复、构建部署流程。

## 文档索引

| 文档 | 内容 | 状态 |
|---|---|---|
| [01-overview.md](01-overview.md) | 项目概述、硬件架构、软件架构、与 Klipper 上游的差异 | ✅ |
| [02-modules.md](02-modules.md) | 定制模块详解：probe_eddy_ng、sensor_ldc1612、homing_override、virtual_sdcard、clocksync | ✅ |
| [03-build-deploy.md](03-build-deploy.md) | 交叉编译构建流程、固件烧录、Python 包部署 | ✅ |
| [04-macros.md](04-macros.md) | AD5M 配置宏系统：homing_override 模板、PAUSE/RESUME 守卫、PLR、timelapse | ✅ |
| [05-bugs-fixed.md](05-bugs-fixed.md) | 已修复漏洞清单（宏守卫、硬件安全、代码逻辑） | ✅ |
| [06-bugs-pending.md](06-bugs-pending.md) | 已知未修复漏洞清单（probe_eddy_ng / sensor_ldc1612） | ✅ |
| [07-debug.md](07-debug.md) | 调试指南：日志路径、moonraker API、SSH 连接、硬件测试 | ✅ |
| [AGENTS.md](AGENTS.md) | LLM Agent 工作流约定、编码规范、部署流程约束 | ✅ |

## 快速入口

- **第一次接触项目？** 读 [01-overview.md](01-overview.md)
- **要改 Python 模块？** 先读 [AGENTS.md](AGENTS.md) 的编码规范 + [02-modules.md](02-modules.md)
- **发现 bug 想查有没有已知？** 搜 [05-bugs-fixed.md](05-bugs-fixed.md) 和 [06-bugs-pending.md](06-bugs-pending.md)
- **要部署测试？** 看 [03-build-deploy.md](03-build-deploy.md)
- **LLM Agent 必读** → [AGENTS.md](AGENTS.md)

## 设备信息（AD5M）

| 项 | 值 |
|---|---|
| IP | 192.168.0.30 |
| Host/Root 密码 | root / root |
| Host | RV1106 (Linux) |
| MCU | STM32F103xe (eboard) |
| 通信 | ttyS4, 230400 baud |
| 探头 | LDC1612 涡流传感器 (Carbide3D Cartographer) |
| 日志路径 | /root/printer_data/logs/klipper.log |
| Klipper Python | /opt/klipper/ |
| Config | /root/printer_data/config/ |
| MCU 固件 | /opt/klipper/out/klipper.elf |
