# 07. 调试指南

## 设备连接

```bash
# SSH
ssh root@192.168.0.30
# 密码: root

# Moonraker HTTP API
curl http://192.168.0.30:7125/printer/objects/query?toolhead | python3 -m json.tool

# Web 界面
http://192.168.0.30:7125/
```

## 日志路径

| 日志 | 路径 | 轮转 |
|---|---|---|
| Klippy | `/root/printer_data/logs/klipper.log` | **无轮转**（重启覆盖） |
| Moonraker | `/root/printer_data/logs/moonraker.log` | 无轮转 |
| Device cam | `/root/printer_data/logs/device-cam.log` | - |
| SmartScreen | `/root/printer_data/logs/smartscreen.log` | - |
| MQTT client | `/root/printer_data/logs/mqtt-client.log` | - |

**问题**：设备无 logrotate 配置，重启会覆盖旧日志。故障回溯只能看设备运行期间的日志。

### 快速 grep

```bash
# 查 import/语法错误
ssh root@192.168.0.30 \
  "grep -iE 'error|traceback|import|exception|failed|shutdown' /root/printer_data/logs/klipper.log | tail -50"

# 查归位相关
ssh root@192.168.0.30 \
  "grep -iE 'homing|home|endstop|probe' /root/printer_data/logs/klipper.log | tail -20"

# 查 idle_timeout / motor_off
ssh root@192.168.0.30 \
  "grep -iE 'idle|motor_off|M84' /root/printer_data/logs/klipper.log | tail -20"

# 查 eddy probe
ssh root@192.168.0.30 \
  "grep -iE 'eddy|ldc1612|sensor' /root/printer_data/logs/klipper.log | tail -30"
```

## Klippy 进程管理

```bash
# 进程状态
ssh root@192.168.0.30 "pgrep -af klippy.py"

# 重启
ssh root@192.168.0.30 "kill \$(pgrep -f klippy.py)"
# 守护进程会自动拉起，3-5 秒后恢复

# 确认启动完成
ssh root@192.168.0.30 "tail -5 /root/printer_data/logs/klipper.log"
# 看最后几行是否有 "Stats ..."（正常运行时每秒一条）
```

## Moonraker API

```bash
# 查看当前配置（configfile dump）
curl -s "http://192.168.0.30:7125/printer/configfile.config" \
  | python3 -c "import sys,json; d=json.load(sys.stdin); print(d['result']['config'][:5000])"

# 执行 G-code
curl -X POST "http://192.168.0.30:7125/printer/gcode/script" \
  -d "script=G28 Z"

# 查询 toolhead 位置
curl -s "http://192.168.0.30:7125/printer/objects/query?toolhead" \
  | python3 -m json.tool

# 查询归位状态
curl -s "http://192.168.0.30:7125/printer/objects/query?toolhead" \
  | python3 -c "import sys,json; d=json.load(sys.stdin); print('homed_axes:', d['result']['status']['toolhead']['homed_axes'])"

# FIRMWARE_RESTART
curl -X POST "http://192.168.0.30:7125/printer/gcode/script" \
  -d "script=FIRMWARE_RESTART"
```

### Moonraker API 坑

| 问题 | 解决 |
|---|---|
| FIRMWARE_RESTART 用 JSON body 被拒 | 改用 form 编码 `-d "script=FIRMWARE_RESTART"` |
| `/server/files/upload` 的 path 参数按目录处理 | 用与目标同名的本地文件上传 |
| `/printer/gcode/script` 不解析 JSON body | 用 form 编码 |
| 返回 500 streaming-form-data | 设备端 moonraker 的 streaming-form-data 包 Python 模块与 .so 版本不匹配，需确保 buildroot 里版本一致 |

## 硬件测试命令

### 归位测试（⚠️ 空闲时做）

```bash
# 完整归位
curl -X POST "http://192.168.0.30:7125/printer/gcode/script" -d "script=G28"

# 仅 Z 归位（修复后应该只动 Z）
curl -X POST "http://192.168.0.30:7125/printer/gcode/script" -d "script=G28 Z"

# 仅 X 归位
curl -X POST "http://192.168.0.30:7125/printer/gcode/script" -d "script=G28 X"

# 验证位置
curl -s "http://192.168.0.30:7125/printer/objects/query?toolhead" \
  | python3 -c "import sys,json; d=json.load(sys.stdin); p=d['result']['status']['toolhead']['position']; print(f'X={p[0]:.2f} Y={p[1]:.2f} Z={p[2]:.2f}')"
```

### 探头测试

```bash
# 单次探测
curl -X POST "http://192.168.0.30:7125/printer/gcode/script" -d "script=PROBE"

# 探头精度测试
curl -X POST "http://192.168.0.30:7125/printer/gcode/script" -d "script=PROBE_ACCURACY"

# Eddy probe 状态
curl -X POST "http://192.168.0.30:7125/printer/gcode/script" -d "script=PROBE_STATUS"
```

### SD 卡测试

```bash
# 列出 SD 文件
curl -s "http://192.168.0.30:7125/printer/gcode/script" -d "script=M20"

# 选文件
curl -X POST "http://192.168.0.30:7125/printer/gcode/script" -d "script=M23 test.gcode"

# 开始打印（⚠️ 确保空闲）
curl -X POST "http://192.168.0.30:7125/printer/gcode/script" -d "script=M24"
```

### PLR 模拟测试（⚠️ 不要在打印中途做）

1. 启动一个短的测试打印
2. 打印中途**直接断电**（不要用 M84，PLR 需要硬件断点）
3. 重新上电
4. Klippy 启动后应该自动触发 PLR 恢复流程

### 强制移动测试（⚠️ 确保空闲 + 归位已建立）

```bash
# 强制移动到安全位置
curl -X POST "http://192.168.0.30:7125/printer/gcode/script" -d "script=G1 X105 Y105 F1800"
```

### SET_KINEMATIC_POSITION 测试

```bash
# 清除归位（模拟 motor_off 后）
curl -X POST "http://192.168.0.30:7125/printer/gcode/script" -d "script=M114"  # 看位置

# 软件恢复归位（零物理移动）
curl -X POST "http://192.168.0.30:7125/printer/gcode/script" -d "script=SET_KINEMATIC_POSITION"

# 再次确认
curl -s "http://192.168.0.30:7125/printer/objects/query?toolhead" \
  | python3 -c "import sys,json; d=json.load(sys.stdin); print('homed_axes:', d['result']['status']['toolhead']['homed_axes'])"
```

## 常见故障排查

| 现象 | 可能原因 | 排查 |
|---|---|---|
| "Must home axis first" | motor_off 清归位 | `SET_KINEMATIC_POSITION` 恢复 |
| "Unknown homing error: 2147483648" | sensor_ldc1612 rest_ticks==0 | 重启 Klippy；需 C2 修复 |
| "Internal error in stepcompress" | chelper C 扩展 bug | 看 motion_queuing 日志 |
| TRSYNC_TIMEOUT 100ms | MCU 通信超时 | 串口波特率 / 检查 aio_executor 是否生效 |
| MCU 频繁重连 | USB/UART 问题 | 查 serialqueue 日志 |
| 探头读数不准 | freq_to_height 边界 bug | 见 probe_eddy_ng C1-C3 |
| SD 打印卡死 | virtual_sdcard aio_executor | 确认已恢复 aio_executor 集成 |
