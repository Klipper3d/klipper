# STM32F103 无 BOOT0 串口升级方案（Katapult bootloader 路径）

> 目标：F103 跑 Klipper 时，发一个串口魔术串 + 纯软件复位，跳进 Katapult bootloader，用 `flashtool.py` 升级，全程**无需控制 BOOT0 pin**。
>
> Katapult 已按 F103 编好（`bootloader/` → `../bootloader-katapult`），**2672 字节 / 只开串口 / 无 USB·CAN·按键·双击复位**。

---

## 1. 硬事实（Katapult .config + autoconf.h 实测）

Katapult 构建（`bootloader/.config`）：

| 配置 | 值 | 说明 |
|---|---|---|
| `CONFIG_MACH_STM32F103` | 1 | F103 目标芯片 |
| `CONFIG_MACH_STM32F103x6` | 1 | 低RAM变体（对bootloader握手地址无影响） |
| `CONFIG_MCU` | `"stm32f103xe"` | high-density |
| `CONFIG_FLASH_BOOT_ADDRESS` | `0x8000000` | Katapult Flash 基址 |
| `CONFIG_LAUNCH_APP_ADDRESS` | `0x8001000` | **应用从 4KiB 偏移起** |
| `CONFIG_FLASH_APPLICATION_ADDRESS` | `0x8000000` | Katapult 自己构建时 FLASH_APPLICATION_ADDRESS=FLASH_BOOT_ADDRESS（它是 bootloader，不关心这个） |
| `CONFIG_STM32_APP_START_1000` | y | Katapult 编进应用链接脚本时用的偏移选项 |
| `CONFIG_STM32_SERIAL_USART1` | y | USART1 PA10(RX)/PA9(TX) |
| `CONFIG_SERIAL_BAUD` | 250000 | Katapult 运行期波特率 |
| `CONFIG_SERIAL` | y | 开串口传输 |
| `CONFIG_USBSERIAL` / `CONFIG_CANSERIAL` | 关 | USB、CAN 全关 |
| `CONFIG_ENABLE_DOUBLE_RESET` / `CONFIG_ENABLE_BUTTON` | 关 | 双击复位、按键全关 |
| `CONFIG_OPTIMIZE_FLAG` | `"-Os"` | 尺寸优化 |
| `CONFIG_STACK_SIZE` | 512 | bootloader 栈大小 |
| `katapult.bin` | **2672 字节** | 实测文件大小 |

### 最小偏移 4 KiB 的原因

Katapult 二进制 2.6 KiB > 2 KiB，`STM32_APP_START_1000`（4 KiB）是 F103 上的最小可行偏移选项——2 KiB 偏移装不下。

### Klipper 侧 Kconfig 对应的选项

Klipper 的 `src/stm32/Kconfig` 里，当选 `STM32_FLASH_START_1000`（4 KiB）时，`CONFIG_FLASH_APPLICATION_ADDRESS` 自动派生为 `0x8001000`，**不等于** `CONFIG_FLASH_BOOT_ADDRESS (0x8000000)` —— 这是 `canboot_reset()` 不早退的前提条件。

---

## 2. 现状诊断

### 2.1 Klipper 当前 `.config`：`FLASH_START_0000` 导致空转

```
CONFIG_FLASH_BOOT_ADDRESS       = 0x8000000
CONFIG_FLASH_APPLICATION_ADDRESS = 0x8000000   ← 两个地址相同
CONFIG_STM32_FLASH_START_0000 = 1
```

`src/generic/armcm_reset.c` 的 `canboot_reset()` 第一行：

```c
static void canboot_reset(uint64_t req_signature) {
    if (CONFIG_FLASH_APPLICATION_ADDRESS == CONFIG_FLASH_BOOT_ADDRESS)
        return;                              // ← 直接退，签名校验都不跑
    ...
```

### 2.2 stm32f1.c 其余三个分支也全 false

```c
// src/stm32/stm32f1.c:254-264
void bootloader_request(void) {
    try_request_canboot();                                    // 因地址相同早退
    if (CONFIG_STM32_FLASH_START_800)                         // false
        usb_hid_bootloader();
    else if (CONFIG_STM32_FLASH_START_2000)                   // false
        usb_stm32duino_bootloader();
    // ← 无任何 fallback
    // N32G45x 分支要求 CONFIG_MACH_N32G455 && CONFIG_STM32_FLASH_START_10000，此处无
}
```

### 2.3 小结：两件事缺一不可

| # | 问题 | 怎么解决 |
|---|---|---|
| A | `.config` 没留 bootloader 空间 → `canboot_reset()` 早退 | Kconfig 选 `STM32_FLASH_START_1000`（4 KiB），让 `FLASH_APPLICATION_ADDRESS = 0x8001000` |
| B | Flash 0x08000000 没烧 Katapult → 签名校验失败 | 首次烧 `bootloader/out/katapult.bin` 到 `0x8000000` |

---

## 3. Katapult 内部握手（无 BOOT0 原理）

Klipper 侧请求和 Katapult 侧启动，靠 **RAM 里 Katapult 初始 SP 位置（`bl_vectors[0]`）的 64 位 bootup_code** 交换意图，全程不碰 BOOT0 pin。

```
Klipper 运行期 bootloader_request()
  └─ try_request_canboot() → canboot_reset(CANBOOT_REQUEST)
       ├─ FLASH_APPLICATION_ADDRESS != FLASH_BOOT_ADDRESS ✓
       ├─ CANBOOT_SIGNATURE (0x21746f6f426e6143) 存在于 Katapult 向量表附近 ✓
       ├─ irq_disable()
       ├─ 写 CANBOOT_REQUEST (0x5984E3FA6CA1589B) 到 bl_vectors[0]（Katapult 初始 SP）
       └─ NVIC_SystemReset()

── F103 复位 ──

Katapult ResetHandler（早于 Katapult 自己的 .data/.bss 拷贝）
  ├─ bootup_code = get_bootup_code()
  ├─ bootup_code != REQUEST_START_APP → 不跳应用
  │   → reset_handler_stage_two → armcm_main()
  │     → serial_init() USART1 PA10/PA9 250000
  │     → sched_main() 等待 flashtool.py
  ├─ 升级完成 → COMPLETE → application_jump()
  │     写 REQUEST_START_APP + NVIC_SystemReset()
  ├─ Katapult 再次复位 → bootup_code == REQUEST_START_APP
  │     boot_start_application() → bx 跳 0x8001000（Klipper 新固件）
```

---

## 4. 改动清单

### 改动 A：Klipper 侧重配

```
make menuconfig
  Processor model: STM32F103
  Bootloader offset: 4 KiB bootloader (STM32_FLASH_START_1000)
  Communication interface: Serial (on USART1 PA10/PA9)
  Serial baud rate: <Klipper 之前用的值，比如 230400>
  ↓
CONFIG_FLASH_APPLICATION_ADDRESS = 0x8001000    ← 自动生效
```

重新编译后 `canboot_reset()` 地址比较不再早退，签名校验和复位恢复执行。

### 改动 B：首刷引导（一次性）

改动 B 把 Katapult 烧到 0x8000000 **会覆盖旧 Klipper 固件（0-offset 构建的前 2.6KB，向量表被毁，旧固件报废——属预期）**。此时新 Klipper（4KiB 偏移）还没烧进去，Katapult 会因应用校验失败每次复位都停在 bootloader——这正是我们要的：首刷时 Katapult 自动跑起来，不用再发魔术串。

正确的首刷序列：

```
步骤 1：BOOT0 置高 + 复位 → 进 F103 ROM System Bootloader
步骤 2：烧 Katapult 到 0x8000000
步骤 3：释放 BOOT0 + 复位 → Katapult 自动起来（等在 USART1 上，250000）
步骤 4：直接 flashtool.py 烧新 Klipper 到 0x8001000（不用魔术串，Katapult 已在等）
步骤 5：COMPLETE → Katapult 复位 → 跳 Klipper 0x8001000 → 新固件启动
```

**烧 Katapult**（BOOT0 + stm32flash）：
```shell
stm32flash -w bootloader/out/katapult.bin -v -g 0 /dev/ttyUSB0
# -g 0：烧完跳到 0x08000000（Katapult 位置）
# stm32flash 默认 8E1（偶校验），F103 ROM bootloader 要求，需 tty 支持 parenb
```

**烧新 Klipper**（Katapult 已起来在等）：
```shell
python3 klipper/lib/katapult/flashtool.py \
    -d /dev/ttyUSB0 -b 250000 -f klipper/out/klipper.bin
```

### 改动 C：后续升级（魔术串路径）

首刷完成后，Klipper 在 0x8001000 运行，Katapult 在 0x8000000 待命，以后升级不用 BOOT0：

```shell
# 1. 停 klippy（否则串口被占用）
systemctl stop klipper

# 2. 发魔术串（Klipper 运行期波特率）
stty -F /dev/ttyS4 230400
printf '~ \x1c Request Serial Bootloader!! ~' > /dev/ttyS4

# 3. Katapult 起来后 flashtool.py 下新固件（Katapult 波特率 250000）
stty -F /dev/ttyS4 250000
python3 klipper/lib/katapult/flashtool.py \
    -d /dev/ttyS4 -b 250000 -f klipper/out/klipper.bin

# 4. 重启 klippy
systemctl start klipper
```

---

## 5. 完整触发流程

```
宿主 (230400)                              STM32F103 (Klipper 0x8001000)
────────────                              ─────────────────────────────
systemctl stop klipper
stty /dev/ttyS4 230400
printf '~ \x1c Request Serial Bootloader!! ~' >> /dev/ttyS4
                                          serial_irq.c 识别 32B 魔术串
                                            → bootloader_request()
                                              canboot_reset()
                                                地址不同 ✓
                                                CANBOOT_SIGNATURE 匹配 ✓
                                                写 CANBOOT_REQUEST 到 bl_vectors[0]
                                                NVIC_SystemReset()
── F103 复位 ──
                                          Katapult ResetHandler
                                            bootup_code != REQUEST_START_APP
                                            armcm_main()
                                              serial_init() USART1 PA10/PA9 250000
                                              sched_main()

宿主 (切 250000)
stty /dev/ttyS4 250000
flashtool.py -d /dev/ttyS4 -b 250000 -f klipper.bin
                                          写 0x8001000 + SHA-1 验证
                                          COMPLETE
                                            写 REQUEST_START_APP + NVIC_SystemReset()
── F103 复位 ──
                                          Katapult ResetHandler
                                            bootup_code == REQUEST_START_APP
                                            boot_start_application() → bx 跳 0x8001000
── Klipper 新固件启动 ──
```

**波特率切换**：Klipper 运行期 230400（发魔术串）→ Katapult 运行期 250000（flashtool.py）。两个独立固件各自配置，不冲突。

---

## 6. 风险与回退

| 风险 | 概率 | 处理 |
|---|---|---|
| `canboot_reset()` 签名校验失败 | 低（Katapult ResetHandler 前硬编码了签名） | 确认 0x8000000 处真的是 katapult.bin |
| Katapult 250000 在串口上不稳 | 低（需实测） | 改 Katapult CONFIG_SERIAL_BAUD 重编，flashtool.py -b 与之一致 |
| F103 ROM bootloader + stm32flash 需偶校验，RV1106 ttyS4 若不支持 | 低（需实测） | `stty parenb -parodd cs8 -cstopb 115200` 验证；不行换 USB 转串口 |
| 魔术串进来了但 F103 没复位 | 低 | 查 serial_irq.c 日志；BOOT0 + ROM bootloader 排除硬件问题 |

**降级通道**：F103 ROM System Bootloader（ST 芯片原生、永远可用，入口 `0x1FFFF000`）通过 BOOT0 置高 + reset 进入，用 stm32flash 重烧 katapult.bin。如果 Katapult 彻底坏了（比如 Flash 前 4KB 被覆盖），这条通道能救回来。

---

## 7. 精简能力总结

| 维度 | 现状 | 继续精简的可能 |
|---|---|---|
| Flash 占用 | **2672 字节**（.bin） | 关 LED → -几十字节；`-Oz` → -几十~上百字节 |
| Flash 偏移 | **4 KiB** | 2 KiB 偏移装不下 Katapult（2.6 KiB > 2 KiB），4 KiB 是最小 |
| 栈 | 512 字节（STACK_SIZE） | 缩 STACK_SIZE 风险太高 |
| 功能开关 | USB / CAN / 按键 / 双击复位 全关 | 只剩 LED 开着（几字节） |

2.6 KiB / 4 KiB 偏移是 F103 上 Katapult + 串口这条路径的实战合理平衡。

---

## 附录：Katapult 帧与命令速查（flashtool.py 内部用）

```
+--------+--------+-----------+----------------+----------+----------+
| HEADER |  CMD   | WORD_CNT  |    PAYLOAD     |   CRC16  | TRAILER  |
| 2 字节 | 1 字节 | 1 字节    | 0..N 字节      | 2 字节   | 2 字节   |
+--------+--------+-----------+----------------+----------+----------+
 0x01 0x88                                   little-endian  0x99 0x03
```

| 命令 | 值 | 方向 |
|---|---|---|
| CONNECT | 0x11 | 主机→bootloader，返回协议版本 + LAUNCH_APP_ADDRESS + block_size(64) |
| SEND_BLOCK | 0x12 | 主机→bootloader，`<I` 地址前缀 + 块数据 |
| SEND_EOF | 0x13 | 主机→bootloader，返回已写页数 |
| REQUEST_BLOCK | 0x14 | 主机→bootloader，回读 SHA-1 验证 |
| COMPLETE | 0x15 | 主机→bootloader，写 REQUEST_START_APP + 复位 |
| GET_CANBUS_ID | 0x16 | 主机→bootloader，取回 CANBUS UUID |
