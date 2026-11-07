# 03. 构建与部署

## 构建脚本

| 脚本 | 目标 | 交叉工具链 |
|---|---|---|
| `build_eboard.sh` | STM32F103 MCU 固件 | `arm-none-eabi-gcc` |
| `build_linux_rv1106.sh` | RV1106 Host 固件 | `arm-rockchip830-linux-uclibc-gcc` |
| `build_linux_t113.sh` | T113 Host 固件 | `arm-linux-gnueabihf-gcc` |
| `Makefile-rv1106` | RV1106 专用 Makefile | 同上 |

## 构建流程

### Step 1. MCU 固件（eboard）

```bash
cd /home/song/samba/work/project/klipper/klipper3d

# 复制 MCU config
cp mcu_configs/eboard_config .config

# （可选）menuconfig 调整
make menuconfig

# 构建
./build_eboard.sh

# 产物：
# mcu_configs/eboard.hex
# mcu_configs/eboard.bin
```

### Step 2. Host 固件（RV1106）

```bash
# 复制 host config
cp mcu_configs/linux_config .config

# 构建
./build_linux_rv1106.sh

# 产物：
# out/klipper.elf
# out/klipper.bin
```

### 环境依赖

交叉工具链路径（`build_eboard.sh` 里硬编码）：
```bash
# 需要提前在 PATH 中
arm-none-eabi-gcc          # MCU 固件
arm-rockchip830-linux-uclibc-gcc  # RV1106 host
```

## 部署到设备

设备：192.168.0.30 / root / root

### Python 模块更新

```bash
# 1. 上传 .py 文件
scp klippy/extras/homing_override.py root@192.168.0.30:/opt/klipper/klippy/extras/
scp klippy/clocksync.py root@192.168.0.30:/opt/klipper/klippy/
scp klippy/extras/virtual_sdcard.py root@192.168.0.30:/opt/klipper/klippy/extras/

# 2. 清除 Python 字节码缓存（关键！否则加载旧 .pyc）
ssh root@192.168.0.30 \
  "find /opt/klipper -name '__pycache__' -type d -exec rm -rf {} + 2>/dev/null; \
   find /opt/klipper -name '*.pyc' -delete 2>/dev/null"

# 3. 重启 Klippy
ssh root@192.168.0.30 \
  "kill \$(pgrep -f klippy.py); sleep 3; pgrep -af klippy.py"
```

### MCU 固件烧录

```bash
# 烧录 .bin/.hex 到 STM32F103
# 方式一：stm32flash
stm32flash -w mcu_configs/eboard.bin -v /dev/ttyUSB0

# 方式二：通过 RV1106 host 侧工具
ssh root@192.168.0.30
# 然后烧录到 eboard
```

### Klippy 重启方式

| 方式 | 命令 | 说明 |
|---|---|---|
| 进程 kill + 守护进程拉起 | `kill $(pgrep -f klippy.py)` | 最常用，systemd/supervisor 会重启 |
| moonraker FIRMWARE_RESTART | `curl -X POST -d "script=FIRMWARE_RESTART" http://localhost:7125/printer/gcode/script` | G-code 层重启，更优雅 |
| 整机断电 | 直接断电重启 | 最彻底 |

### 验证修改生效

```bash
# 1. 比对 md5
LOCAL_MD5=$(md5sum klippy/extras/homing_override.py | cut -d' ' -f1)
REMOTE_MD5=$(ssh root@192.168.0.30 "md5sum /opt/klipper/klippy/extras/homing_override.py | cut -d' ' -f1")
echo "local:  $LOCAL_MD5"
echo "remote: $REMOTE_MD5"

# 2. 检查日志无 import error
ssh root@192.168.0.30 \
  "tail -100 /root/printer_data/logs/klipper.log | grep -iE 'error|traceback|import|exception' || echo OK"

# 3. G28 测试（部分归位是否只动单轴）
# 在 moonraker 界面跑 G28 X → 预期只动 X 轴
```

### 配置文件部署

配置文件路径：`/root/printer_data/config/`

| 文件 | 说明 | 来源 |
|---|---|---|
| `printer.cfg` | 主配置 | 设备端独立（含 SAVE_CONFIG 自动块） |
| `macros.cfg` | 功能宏 | SDK overlay + 设备独立改动 |
| `client.cfg` | client 宏 | SDK overlay |
| `printer.base.cfg` | 硬件基础配置（corexy, pin） | SDK overlay |
| `timelapse.cfg` | Timelapse 配置 | SDK overlay |
| `power_loss_recovery.cfg` | PLR 配置 | SDK overlay |

**重要**：SAVE_CONFIG 自动生成的校准数据块（bed_mesh/mesh/probe）是**每台设备独立**的，不同步到源码。

---

## SDK overlay 路径（AD5M 批量构建）

```
RV1106 SDK overlay 配置源：
../../../rv1106/rk-sdk/RV1106_IPC_SDK/project/cfg/BoardConfig_IPC/overlay/overlay-gt-buildroot-app/root/printer_data/config/
```

设备端宏修改需要先同步到这个 overlay 目录，再做固件批量构建。

| 文件 | 修改历史 |
|---|---|
| `printer.cfg` | 2026-10-19: +[force_move] enable_force_move: True |
| `macros.cfg` | 2026-10-19: +variable_idle_timeout / END_PRINT / _COMMON_END_PRINT 守卫 |
| `client.cfg` | 2026-10-19: RESUME SET_KINEMATIC_POSITION 守卫 |

---

## 常见坑

| 问题 | 原因 | 解决 |
|---|---|---|
| `__pycache__` 加载旧字节码 | Python 优先加载 .pyc 而非 .py | 部署后必须删 `__pycache__` 和 `.pyc` |
| FIRMWARE_RESTART JSON body 被拒 | 旧版 moonraker 不解析 JSON | 改用 form 编码 `-d "script=..."` |
| moonraker upload path 按目录处理 | 老版本行为 | 用与目标同名的本地文件 |
| MCU 固件改了 Python 没改 | C 层和 Python 层是分开部署的 | 两边都要确认 |
| 归位丢失后整机 G28 | 打印中途 `G28 Z` 会触发完整 override → 撞模型 | 用无参 `SET_KINEMATIC_POSITION` 软件恢复 |
