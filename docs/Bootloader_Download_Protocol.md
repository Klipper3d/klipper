# Klipper Bootloader 下载协议整理

> 本文整理 Klipper 生态中"把固件映像下载到 bootloader"所涉及的协议。
> 进入 bootloader 的机制见 [`Bootloader_Code_Analysis.md`](Bootloader_Code_Analysis.md)，
> 本文聚焦**进入之后的下载（download/flash）协议**。
>
> 主要代码来源：
> - `scripts/flash_usb.py`（按 MCU 类型选择后端的统一入口）
> - `lib/katapult/flashtool.py`（Katapult/CanBoot 下载协议主机侧，Klipper 派生）
> - `bootloader/`（Katapult bootloader **固件源码**，支持 CAN/USB/**UART**，
>   是 Klipper MCU 源码的裁剪版；`out/katapult.bin` 为编译产物）
> - `docs/Bootloaders.md`（各平台 bootloader 与刷写方法）

---

## 1. 整体架构：下载由 `flash_usb.py` 编排

`flash_usb.py` 本身**不实现任何传输协议**，它只是根据 MCU 类型选择对应的
第三方/专用后端工具并调用。映射关系（`flash_usb.py:345` 的 `MCUTYPES`）：

| MCU 类型 | 后端工具 | 底层协议 |
|---|---|---|
| `sam3` / `sam4` / `same70` / `same5` | `bossac` (`lib/bossac`) | SAM-BA |
| `samd` | `bossac`（带 `--offset`） | SAM-BA |
| `lpc176` | `dfu-util` | USB DFU |
| `stm32f103` / `stm32f4` / `stm32f0x2` 等 | `dfu-util` 或 `hid-flash` | USB DFU / HID |
| `rp2`（rp2040/rp2350） | `rp2040_flash` (`lib/rp2040_flash`) | picoboot |
| 任何检测到 Katapult 的设备 | `lib/katapult/flashtool.py` | Katapult/CanBoot 自定义协议 |

关键判断：当设备复位后 USB 的 `idVendor:idProduct` 等于 `1d50:6177`
（`CANBOOT_ID`，`flash_usb.py:76`）时，统一改走 `flashtool.py`
（`call_flashcan`，`flash_usb.py:90`）。

下载前置：必须先让 MCU 进入 bootloader。常见触发方式：
- USB 1200 波特脉冲 DTR（`enter_bootloader`，`flash_usb.py:13`）；
- 物理串口发送魔术串（见第 2.5 节）；
- CAN admin 消息（见第 2.4 节）。

---

## 2. Katapult / CanBoot 下载协议（重点）

Katapult（原 CanBoot）是 Klipper 派生的 bootloader，支持 **CANBUS 与 USB/串口** 两种
下载通道，使用**自定义二进制帧协议**。完整实现见 `lib/katapult/flashtool.py`。

### 2.1 帧格式（Framing）

每一条命令/应答都是一个字节流帧，由 `_build_command()` 构造
（`flashtool.py:253`）：

```
+--------+--------+-----------+----------------+----------+----------+
| HEADER |  CMD   | WORD_CNT  |    PAYLOAD     |   CRC16  | TRAILER  |
| 2 字节 | 1 字节 | 1 字节    | 0..N 字节      | 2 字节   | 2 字节   |
+--------+--------+-----------+----------------+----------+----------+
 0x01 0x88                                   little-endian  0x99 0x03
```

- **HEADER** = `0x01 0x88`（`CMD_HEADER`，`flashtool.py:56`）。
- **CMD** = 命令/应答类型字节。
- **WORD_CNT** = `len(payload) // 4 & 0xFF`（`flashtool.py:254`）；接收端据此算出
  `recd_len = word_cnt * 4`（`flashtool.py:344`）。
- **PAYLOAD** = 命令参数；整帧长度 = `recd_len + 8`（header2 + cmd1 + cnt1 + payload
  + crc2 + trailer2，`flashtool.py:345`）。
- **CRC16** = CRC-16/CCITT，**little-endian**，覆盖范围 = 从 CMD 字节开始到 PAYLOAD
  结尾（即帧 `[2:]` 部分，`flashtool.py:260`）。校验函数与 Klipper `msgproto.py`
  完全一致（`flashtool.py:42`）。
- **TRAILER** = `0x99 0x03`（`CMD_TRAILER`，`flashtool.py:57`）。

接收端用 `readuntil(CMD_TRAILER)` 帧同步，并丢弃不以 HEADER 开头的字节
（`flashtool.py:338-346`），保证字节流中帧的重新对齐。

### 2.2 命令与应答类型

命令集（`BOOTLOADER_CMDS`，`flashtool.py:58`）：

| 名称 | 值 | 方向/含义 |
|---|---|---|
| `CONNECT` | `0x11` | 主机→bootloader，建立连接并获取设备信息 |
| `SEND_BLOCK` | `0x12` | 主机→bootloader，下发一个数据块 |
| `SEND_EOF` | `0x13` | 主机→bootloader，文件结束，返回已写页数 |
| `REQUEST_BLOCK` | `0x14` | 主机→bootloader，回读一个数据块（验证用） |
| `COMPLETE` | `0x15` | 主机→bootloader，完成并退出 bootloader |
| `GET_CANBUS_ID` | `0x16` | 主机→bootloader，取回 CANBUS UUID |

应答/ACK 字节（`flashtool.py:67`）：

| 名称 | 值 | 含义 |
|---|---|---|
| `ACK_SUCCESS` | `0xa0` | 成功 |
| `NACK` | `0xf1` | 否定应答 |
| `ACK_ERROR` | `0xf2` | 错误 |
| `ACK_BUSY` | `0xf3` | 忙，主机需重试（`flashtool.py:383` 会 `sleep(1.5)` 后重试） |

应答帧里 `[2]` 为 ACK 字节，`[4:8]` 为回显的命令类型（`<I`），其后为命令返回数据。

### 2.3 下载流程（主机侧 `CanFlasher`）

完整流程在 `CanFlasher` 类中（`flashtool.py:211`）：

1. **Prime（可选）**（`prime()`，`flashtool.py:265`）：先发一个**无效命令**
   `0x90`。对带 USB 双缓冲（double buffering）的 STM32，这一步强制设备对首条命令
   立即应答，避免 `CONNECT` 被缓冲吞掉（`flashtool.py:1071` 的
   `_has_double_buffering`）。

2. **CONNECT**（`connect_btl()`，`flashtool.py:273`）：返回 12 字节 `pinfo` +
   MCU 信息字符串。 `pinfo` 解包为 `<4sII`：
   - 前 4 字节 = 协议版本（取前 3 字节，**字节序反转**成 `(major, minor, patch)`，
     `flashtool.py:282`）；
   - 接着 `start_addr`（应用起始地址，`app_start_addr`）；
   - 接着 `block_size`（块大小，**必须为 64/128/256/512 之一**，否则报错，
     `flashtool.py:284`）。
   - 协议 ≥ `1.1.0` 时 MCU 信息以 `\x00` 分隔 MCU 类型与软件版本（`flashtool.py:287`）。
   - 若主机手头有 `klipper.bin` 内嵌的 `klipper.dict`（`_check_binary`，
     `flashtool.py:228`），会校验其 MCU 字段与 bootloader 回报一致，防止刷错芯片。

3. **GET_CANBUS_ID**（`verify_canbus_uuid()`，`flashtool.py:314`）：返回 6 字节
   UUID，与命令行 `-u` 提供的 UUID 比对，不一致则中止。

4. **SEND_FILE**（`send_file()`，`flashtool.py:409`）：核心下载。
   - 以 `block_size` 为单元循环读取固件；末尾不足一个块时以 `0xFF` 补齐
     （`flashtool.py:423`）。
   - 每个块 payload = `<I` 地址前缀 + 块数据（`flashtool.py:426`）。
   - 发 `SEND_BLOCK`，bootloader 回显写入地址；若与发送地址不符则重试最多 3 次
     （`flashtool.py:427`）。
   - 全部发完后发 `SEND_EOF`，bootloader 返回已写 **页数** `page_count`
     （`<I`，`flashtool.py:456`）。

5. **VERIFY_FILE**（`verify_file()`，`flashtool.py:459`）：逐块用 `REQUEST_BLOCK`
   回读，累加 SHA-1；最后与本地固件 SHA-1 比对（`flashtool.py:485`），不一致即报错。

6. **COMPLETE**（`finish()`，`flashtool.py:492`）：发 `COMPLETE` 命令，bootloader
   **退出并跳转到应用程序**。即便前面出错也会尝试发送（除非通信已断）。

> 进度显示：下载/验证按 2% 步进打印 `#`（`flashtool.py:452`）。

### 2.4 CAN 通道封装与 admin 消息

CAN 帧格式 `CAN_FMT = "<IB3x8s"`（`flashtool.py:52`）：`can_id(4B) + len(1B) +
3B 填充 + 8B 数据`。payload 超过 8 字节时由 `send()` 自动**拆成多条 8 字节 CAN 帧**
（`flashtool.py:669`），接收端按 16 字节（标准 socketcan 帧）重组
（`_process_packet`，`flashtool.py:654`）。

CAN 相关 ID 与 admin 命令（`flashtool.py:73-89`）：

| 名称 | 值 | 含义 |
|---|---|---|
| `KLIPPER_ADMIN_ID` / `CANBUS_ID_ADMIN` | `0x3f0` | Klipper/CANBoot 管理消息 ID |
| `CANBUS_ID_ADMIN_RESP` | `0x3f1` | 管理响应 ID |
| `KLIPPER_REBOOT_CMD` | `0x02` | 跳转到 bootloader 的管理命令 |
| `CANBUS_CMD_QUERY_UNASSIGNED` | `0x00` | 查询未分配 UUID 的节点（`-q` 查询） |
| `CANBUS_CMD_SET_NODEID` | `0x11` | 为节点分配临时 node id |
| `CANBUS_CMD_CLEAR_NODE_ID` | `0x12` | 清空所有节点 ID |
| `CANBUS_RESP_NEED_NODEID` | `0x20` | "需要分配 node id" 响应 |

**跳转到 bootloader**（`_jump_to_bootloader`，`flashtool.py:692`）：向
`KLIPPER_ADMIN_ID` 发送 `[REBOOT_CMD, UUID(6字节)]`，运行中的 Klipper 收到后
执行第 2 节 bootloader 请求逻辑并复位。

**节点发现/分配**（`run()`，`flashtool.py:812`）：
- 若 `-q`：发 `QUERY_UNASSIGNED`，收集 2 秒内回 `RESP_NEED_NODEID` 且应用为
  "Katapult" 的 UUID（`_query_uuids`，`flashtool.py:698`）。
- 否则 `_set_node_id()`（`flashtool.py:734`）发 `SET_NODEID` 给目标 UUID，
  bootloader 返回一个临时 node id（基址 `CANBUS_NODEID_OFFSET=128`），
  后续下载均用该 node id 通信。

### 2.5 USB / 串口通道与进入方式

- **USB 进入**：对运行 Klipper 的 USB 设备（VID/PID `1d50:614e`），通过
  1200 波特 + 脉冲 DTR 触发（`_request_usb_bootloader`，`flashtool.py:962`），
  复位后等待设备变为 Katapult（`1d50:6177` 或厂商 "katapult"）再继续。
- **串口进入**：直接向串口发送魔术串
  `SERIAL_BL_REQ = b"~ \x1c Request Serial Bootloader!! ~"`
  （`flashtool.py:90`，`_request_serial_bootloader`，`flashtool.py:1013`），
  这正是运行期 Klipper 固件识别并触发 `bootloader_request()` 的串（`src/generic/serial_irq.c:83`）。
- 进入后 USB/串口通道直接走 2.1 节的字节流帧（无 CAN 拆包），由 `SerialSocket`
  处理（`flashtool.py:861`）。USB 设备的双缓冲机型需先 `prime()`（`flashtool.py:1071`）。

### 2.6 Katapult 串口版 STM32F103 体积分析（以本项目实际构建为例）

> Katapult 本身是一个**独立的 bootloader 工程**（仓库根目录 `bootloader/`，即
> `lib/katapult` 的上游源码），复用 Klipper 的 MCU 硬件抽象层并裁剪到最小，支持
> **CAN / USB / UART(串口)** 三种接口。针对 STM32F103 只开串口通道的构建，二进制
> 可以做到极小——这正是"精简 boot 尺寸"的可行路径。

#### 实测体积
本项目 `bootloader/out/katapult.bin`（及同体积 `canboot.bin`）为 **2672 字节（约 2.6 KiB）**，
对应 `bootloader/out/autoconf.h` 的关键配置：

| 配置 | 值 | 说明 |
|---|---|---|
| `CONFIG_SERIAL` | `1` | 启用 UART 传输 |
| `CONFIG_CANSERIAL` | `0` | 不编 CAN |
| `CONFIG_USBSERIAL` | `0` | 不编 USB |
| `CONFIG_OPTIMIZE_FLAG` | `"-Os"` | 尺寸优化 |
| `CONFIG_ENABLE_DOUBLE_RESET` | `0` | 关双击复位 |
| `CONFIG_ENABLE_BUTTON` | `0` | 关按键进入 |
| `CONFIG_ENABLE_LED` | `1` | LED 仍开（影响极小） |

#### 为什么只有 2.6 KiB（构建裁剪）
体积小的根本原因是"**按接口条件编译 + 裁剪版 HAL + 尺寸优化**"：

- 传输层只链入串口。`bootloader/src/stm32/Makefile:53`：
  ```53:53:bootloader/src/stm32/Makefile
  src-$(CONFIG_SERIAL) += stm32/serial.c generic/serial_irq.c
  ```
  而 USB（`src-$(CONFIG_USBSERIAL)`，`Makefile:56`：`stm32/usbfs.c`、
  `stm32/usbotg.c`、`generic/usb_cdc.c`）与 CAN（`src-$(CONFIG_CANSERIAL)`，
  `Makefile:60`：`canserial.c`、`canbus.c`、`stm32/can.c`、
  `lib/fast-hash/fasthash.c`）在这两个开关为 0 时**完全不进二进制**——整套 USB
  描述符/CDC 与 CAN 控制器/fast-hash UUID 生成逻辑都被排除。
- 核心只编极少量文件（`bootloader/src/Makefile:3` + `src/stm32/Makefile:47`）：
  `sched.c bootentry.c command.c flashcmd.c initial_pins.c` +
  `generic/armcm_canboot.c` + `stm32/{flash,clockline,dfu_reboot,stm32f1}.c` +
  `serial.c serial_irq.c crc16_ccitt.c armcm_irq.c`，约 17 个 `.o`。
- `stm32/stm32f1.c` 里的 `usb_hid_bootloader()` / `usb_stm32duino_bootloader()`
  虽然存在于源码，但因 `CONFIG_STM32_FLASH_START_800/2000` 都为 0，被**死代码消除**，
  不占空间。
- `-Os` 尺寸优化，并关掉双击复位/按键（LED 仍开，影响极小）。

> 协议本身也很轻：`protocol.md` 的 `CONNECT/SEND_BLOCK/EOF/REQUEST_BLOCK/
> COMPLETE/GET_CANBUS_ID` 几个命令由 `command.c`/`flashcmd.c` 处理，仅一份
> `crc16_ccitt.c`，无多余功能。

#### 串口下载涉及的源码文件
- **UART 驱动** `bootloader/src/stm32/serial.c`：寄存器级 USART（RXNE/TXE 中断），
  波特率来自 `CONFIG_SERIAL_BAUD`（默认 250000），引脚可选
  USART1(PA9/PA10)/USART2/USART3 等（`serial.c:15-51`）。
- **帧解析/分发** `bootloader/src/generic/serial_irq.c`：把串口字节流按 2.1 节的
  `HEADER+CMD+LEN+PAYLOAD+CRC+TRAILER` 帧重组，识别命令并回调。
- **写 Flash** `bootloader/src/stm32/flash.c` + `flashcmd.c`：`SEND_BLOCK` 写入，
  `SEND_EOF` 收尾，`REQUEST_BLOCK` 回读校验。
- **留/跳判定** `bootloader/src/bootentry.c:54` `bootentry_check()`：满足任一条就进
  bootloader——① RAM(`_stack_end`) 请求签名 `== REQUEST_CANBOOT`；② 应用区无效
  （首向量非 `0`/非 `0xffffffff`）；③ 按键；否则跳应用
  （`armcm_canboot.c:76` `boot_start_application()`）。
- 上位机：`python3 flashtool.py -d <串口设备> -b <波特率>`（`README.md:121`）。

#### 如何进一步精简 + F103 最小偏移约束
二进制已 2.6K，可继续压的杠杆：
1. **选 Serial 接口**（最大头，砍掉 USB/CAN 整条链路）。
2. **优化等级**：`-Os`（默认）已很好；想更激进可改 `-Oz`（`Kconfig` 的
   `HAVE_OPTIMIZE_OZ`），但需实测功能正常。
3. **关可选功能**：`ENABLE_LED` / `ENABLE_DOUBLE_RESET` / `ENABLE_BUTTON` /
   `INITIAL_PINS` 全部关闭，再省几十~上百字节。
4. **预留 Flash 取最小能装下的偏移**：F103 应用起始偏移在菜单里**最小是 4 KiB**
   （`STM32_APP_START_1000` → `LAUNCH_APP_ADDRESS 0x8001000`，
   `bootloader/src/stm32/Kconfig:506/514`）。因为二进制 2.6K 已超过 2KiB、且 F1
   没有 2KiB 的 app 偏移选项，所以**实际最少要为 bootloader 预留 4 KiB**（bootloader
   只占约 2.6K，余量约 1.4K）。要压到 <4K 占用，需自加 2KiB 的 F1 偏移项且保证
   二进制 <2KiB。

#### 串口进入的注意点
`README.md:88` 明确：**UART 设备无法被 flashtool 自动检测**。进 bootloader 不能靠
"自动发现"，需手动触发之一：空 Flash 自动进入、双击复位、配置按键/GPIO，或
`flashtool.py -r` 先发请求再正式烧写（与 CAN/USB 不同）。

---

## 3. 其它后端下载协议概要

以下协议实现位于第三方工具内（仓库以二进制/子模块形式提供），此处仅说明要点。

### 3.1 USB DFU（`dfu-util`）
- 标准 **USB Device Firmware Upgrade**（USB-IF DFU 类，类似 ST AN3156）。
- 适用：STM32（内置系统 bootloader，ROM DFU）、LPC176x（Smoothieware DFU）。
- 调用示例（`flash_usb.py:144`）：`dfu-util -p <bus> -D klipper.bin`，
  STM32F4 带 `-a 0 -s 0x8004000:leave`（`flash_usb.py:313`）。
- 特点：无需驱动（标准类），但部分板卡进 DFU 时 GPIO 状态不确定（详见
  [`Bootloader_Entry.md`](Bootloader_Entry.md) 的 DFU 警告）。

### 3.2 HID Bootloader（`hid-flash`）
- 自定义 **USB HID 报文**协议（`lib/hidflash`），无驱、免 libusb 驱动。
- 适用：STM32F1（2KiB 偏移，`1209:beba`）、STM32F4（16KiB 偏移）。
- 调用（`flash_usb.py:158`）：`lib/hidflash/hid-flash klipper.bin`。

### 3.3 BOSSA / `bossac`
- Atmel **SAM-BA** 协议（通过 USB-ACM 或 3.3V 串口）。
- 适用：ATSAM3/4/21/51/70 等。
- 调用（`flash_usb.py:113`）：`bossac -U -p <tty> [-e] -w klipper.bin -v [-b -R]`；
  SAMD 系列额外带 `--offset=0x2000/0x4000`。

### 3.4 picoboot（`rp2040_flash`）
- Raspberry Pi 的 **PICOBOOT** 协议（`lib/rp2040_flash`），通过 USB 控制端点
  读写 RP2040 片内 Flash（也可经 `elf2uf2` 生成 UF2 拖拽烧录）。
- 适用：RP2040 / RP2350。
- 调用（`flash_usb.py:183`）：`rp2040_flash klipper.bin [bus addr]`。

---

## 4. 命令/常量速查表

### Katapult 帧
| 字段 | 值/算法 |
|---|---|
| HEADER | `0x01 0x88` |
| TRAILER | `0x99 0x03` |
| CRC | CRC-16/CCITT，little-endian，覆盖 `frame[2:]` |
| 长度 | `recd_len + 8`，`recd_len = WORD_CNT * 4` |

### Katapult 命令
| 命令 | 值 |
|---|---|
| CONNECT | `0x11` |
| SEND_BLOCK | `0x12` |
| SEND_EOF | `0x13` |
| REQUEST_BLOCK | `0x14` |
| COMPLETE | `0x15` |
| GET_CANBUS_ID | `0x16` |

### Katapult 应答
| 应答 | 值 |
|---|---|
| ACK_SUCCESS | `0xa0` |
| NACK | `0xf1` |
| ACK_ERROR | `0xf2` |
| ACK_BUSY | `0xf3` |

### CAN / admin
| 项 | 值 |
|---|---|
| Admin ID | `0x3f0`（resp `0x3f1`） |
| Reboot→bootloader | `0x02` |
| Query unassigned | `0x00` |
| Set node id | `0x11` |
| Clear node id | `0x12` |
| Node id 基址 | `128` |
| Katapult USB ID | `1d50:6177` |
| Klipper USB ID | `1d50:614e` |
| 合法 block_size | 64 / 128 / 256 / 512 |

### 进入 bootloader 触发
| 通道 | 触发 |
|---|---|
| USB | 1200 波特 + 脉冲 DTR |
| 串口 | 发送 `b"~ \x1c Request Serial Bootloader!! ~"` |
| CAN | admin `0x3f0` + `[0x02, UUID(6B)]` |

---

## 5. 小结

- Klipper 自己的固件**不实现下载协议**，它只负责"进入 bootloader"
  （`Bootloader_Code_Analysis.md`）。
- 下载由 `flash_usb.py` 按平台分派到 `dfu-util` / `hid-flash` / `bossac` /
  `rp2040_flash` / `flashtool.py`。
- **Katapult/CanBoot 是唯一由 Klipper 派生、协议自定义的下载通道**（固件源码在
  `bootloader/`，主机侧在 `lib/katapult/flashtool.py`），其帧格式为
  `HEADER + CMD + WORD_CNT + PAYLOAD + CRC16 + TRAILER`，支持 CAN/USB/串口三种
  物理通道，并以 SHA-1 做整包校验、以 `COMPLETE` 命令收尾跳转应用。
- **精简实例**：STM32F103 只开串口通道的 Katapult 构建仅 **2672 字节（≈2.6 KiB）**
  （`bootloader/out/katapult.bin`），靠"按接口条件编译 + `-Os` + 关闭可选功能"实现；
  F103 上为它预留的最小 Flash 为 **4 KiB**（`LAUNCH_APP_ADDRESS 0x8001000`）。
- 其余后端均为业界既有协议（DFU / HID / SAM-BA / PICOBOOT），由专用工具完成。
