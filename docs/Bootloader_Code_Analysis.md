# Klipper 固件 Bootloader 机制代码解读与分析

> 本文基于本仓库 `src/` 下的固件源码，分析 Klipper 如何在运行期"进入/请求 bootloader"，
> 以及芯片上电后的启动引导流程。注意：**Klipper 固件本身不是 bootloader**，它内置的是
> "请求进入 bootloader" 的支持逻辑 + 各平台的 ROM/第三方 bootloader 跳转代码。

---

## 1. 总体设计

Klipper 的固件在运行期可以通过多种外部触发方式，主动重启并进入 bootloader。
设计上分三层：

1. **统一触发入口** `bootloader_request()`（声明于 `src/generic/misc.h:21`）——
   由各平台 `main.c` 自己实现，把所有"如何进 bootloader"的细节封装在平台代码里。
2. **跨平台的 CANBoot/Katapult 请求机制**（`src/generic/armcm_reset.c`）——
   先尝试通知已安装的 Katapult 在下次启动保持激活，再复位。
3. **平台专属 ROM/硬件 bootloader 跳转**——
   STM32 DFU（`src/stm32/dfu_reboot.c`）、RP2040 BOOTROM（`src/rp2040/bootrom.c`）、
   ATSAM GPNVM（`src/atsam/main.c`）、LPC176x WDT flag（`src/lpc176x/main.c`）、
   STM32F1 的 HID/STM32duino（`src/stm32/stm32f1.c`）等。

官方文档 [`docs/Bootloader_Entry.md`](Bootloader_Entry.md) 与
[`docs/Bootloaders.md`](Bootloaders.md) 描述了用户侧触发方式（1200 波特 DTR、
串口魔术串、CAN admin 消息），对应底层实现如下。

---

## 2. 触发入口与事件来源

### 2.1 统一接口 `bootloader_request()`

```21:21:src/generic/misc.h
void bootloader_request(void);
```

每个平台在自己 `main.c` 中实现它。例如 LPC176x：

```43:58:src/lpc176x/main.c
// Try to reboot into bootloader
void
bootloader_request(void)
{
    if (!CONFIG_FLASH_APPLICATION_ADDRESS)
        return;
    try_request_canboot();
    // Disable USB and pause for 5ms so host recognizes a disconnect
    irq_disable();
    if (CONFIG_USB)
        usb_disconnect();
    // The "LPC17xx-DFU-Bootloader" will enter the bootloader if the
    // watchdog timeout flag is set.
    LPC_WDT->WDMOD = 0x07;
    NVIC_SystemReset();
}
```

通用套路是：先 `try_request_canboot()`（见第 3 节），再用平台专属方式复位进 bootloader。

### 2.2 触发事件（谁会调用 `bootloader_request()`）

| 触发来源 | 代码位置 | 机制 |
|---|---|---|
| USB CDC 1200 波特 | `src/generic/usb_cdc.c:448` | 设置波特率 1200 且未拉 DTR 时视作 Arduino 式请求 |
| 物理串口魔术串 | `src/generic/serial_irq.c:82` | 收到 32 字节 ` \x1c Request Serial Bootloader!! ~` |
| CANBUS admin 消息 | `src/generic/canserial.c:186` | `CANBUS_CMD_REQUEST_BOOTLOADER` 且 UUID 匹配 |
| `reset` 命令 | `src/generic/armcm_reset.c:61` | 宿主下发 `reset` 指令 |

USB 1200 波特请求（经典 Arduino bootloader 约定）：

```448:456:src/generic/usb_cdc.c
static void
check_reboot(void)
{
    if (!CONFIG_HAVE_BOOTLOADER_REQUEST)
        return;
    if (line_coding.dwDTERate == 1200 && !(line_control_state & 0x01))
        // A baud of 1200 is an Arduino style request to enter the bootloader
        bootloader_request();
}
```

物理串口魔术串（需恰好整块 32 字节，且非合法协议消息）：

```81:84:src/generic/serial_irq.c
    if (ret) {
        if (CONFIG_HAVE_BOOTLOADER_REQUEST && ret < 0 && pop_count == 32
            && !memcmp(receive_buf, " \x1c Request Serial Bootloader!! ~", 32))
            bootloader_request();
```

CANBUS admin 消息（即便 MCU 已 shutdown、已分配 nodeid 也会响应）：

```186:192:src/generic/canserial.c
static void
can_process_request_bootloader(struct canbus_msg *msg)
{
    if (!CONFIG_HAVE_BOOTLOADER_REQUEST || !can_check_uuid(msg))
        return;
    bootloader_request();
}
```

---

## 3. CANBoot / Katapult 请求机制 (`armcm_reset.c`)

Katapult（原 CANBoot）是运行在 `FLASH_BOOT_ADDRESS` 处的第三方 bootloader。
Klipper 不强制依赖它，但若有安装则优先复用它来完成固件刷新。

核心思路：在 Katapult 向量表附近的内存里写入一个 64 位"请求签名"，然后软件复位。
Katapult 下次启动读该签名，决定是直接进应用还是保持 bootloader 激活。

```13:36:src/generic/armcm_reset.c
#define CANBOOT_SIGNATURE 0x21746f6f426e6143
#define CANBOOT_REQUEST   0x5984E3FA6CA1589B
#define CANBOOT_BYPASS    0x7b06ec45a9a8243d

static void
canboot_reset(uint64_t req_signature)
{
    if (CONFIG_FLASH_APPLICATION_ADDRESS == CONFIG_FLASH_BOOT_ADDRESS)
        // No bootloader
        return;
    uint32_t *bl_vectors = (uint32_t *)CONFIG_FLASH_BOOT_ADDRESS;
    uint64_t *boot_sig = (uint64_t *)(bl_vectors[1] - 9);
    uint64_t *req_sig = (uint64_t *)bl_vectors[0];
    if (boot_sig != (void*)ALIGN((size_t)boot_sig, 8)
        || *boot_sig != CANBOOT_SIGNATURE
        || req_sig != (void*)ALIGN((size_t)req_sig, 8))
        return;
    irq_disable();
    *req_sig = req_signature;
#if __CORTEX_M == 7
    SCB_CleanDCache_by_Addr((void*)req_sig, sizeof(*req_sig));
#endif
    NVIC_SystemReset();
}
```

- 当 `FLASH_APPLICATION_ADDRESS == FLASH_BOOT_ADDRESS`（无独立 bootloader）时直接返回。
- 通过校验 `boot_sig`（Katapult 在 `bl_vectors[1]-9` 处写入的 `CANBOOT_SIGNATURE`，
  且地址 8 字节对齐）来确认确实安装了 Katapult，避免误写其他固件内存。
- 写入 `req_sig` 后复位。

两个对外函数：
- `try_request_canboot()` 写 `CANBOOT_REQUEST`（请求 Katapult 保留激活）；
- `command_reset()`（注册为 `reset` 命令，`HF_IN_SHUTDOWN` 标志表示 shutdown 时也可执行）
  写 `CANBOOT_BYPASS`（让 Katapult 下次直接跳过、启动应用），再复位。

```38:42:src/generic/armcm_reset.c
void
try_request_canboot(void)
{
    canboot_reset(CANBOOT_REQUEST);
}
```

```61:75:src/generic/armcm_reset.c
void
command_reset(uint32_t *args)
{
    if (CONFIG_MACH_N32G455 && CONFIG_STM32_FLASH_START_10000)
    {
        // Bootloader bypass for Flashforge 5M(Pro) eboard MCU
        N32G45x_command_reset();
    }
    else
    {
        canboot_reset(CANBOOT_BYPASS);
        NVIC_SystemReset();
    }
}
DECL_COMMAND_FLAGS(command_reset, HF_IN_SHUTDOWN, "reset");
```

注意 `N32G45x_command_reset()`（同文件 44-59 行）是个特例：它把魔数 `0x1234`
写入备份域寄存器 `BKP->DR1`（需先开 `PWR_CR_DBP` 写保护），让该板卡 bootloader
复位后直接启动应用，再 `NVIC_SystemReset()`。

---

## 4. STM32 ROM DFU 跳转 (`dfu_reboot.c`)

很多 STM32 芯片在 ROM 中有 USB DFU bootloader。要进入它，需要复位后"跳转到 ROM 指定地址"。
难点是：软件无法在复位前"跳转"，只能**先写标记→复位→重启时读标记再跳**。

### 4.1 请求：写标记 + 复位

```30:43:src/stm32/dfu_reboot.c
// Flag that bootloader is desired and reboot
void
dfu_reboot(void)
{
    if (!CONFIG_STM32_DFU_ROM_ADDRESS || !CONFIG_HAVE_BOOTLOADER_REQUEST)
        return;
    irq_disable();
    uint64_t *bflag = (void*)USB_BOOT_FLAG_ADDR;
    *bflag = USB_BOOT_FLAG;
#if __CORTEX_M >= 7
    SCB_CleanDCache_by_Addr((void*)bflag, sizeof(*bflag));
#endif
    NVIC_SystemReset();
}
```

- 标记地址选在 RAM 末尾 `CONFIG_RAM_START + CONFIG_RAM_SIZE - 1024`（H7 用 AXI SRAM），
  复位后一般不会被覆盖。
- 标记值是 ASCII `"USB BOOT"`（`0x55534220424f4f54`）。
- Cortex-M7 需手动 Clean D-Cache，否则写可能停留在 cache 没落 RAM。

### 4.2 启动检查：读标记 → 跳 ROM

```45:57:src/stm32/dfu_reboot.c
// Check if rebooting into system DFU Bootloader
void
dfu_reboot_check(void)
{
    if (!CONFIG_STM32_DFU_ROM_ADDRESS || !CONFIG_HAVE_BOOTLOADER_REQUEST)
        return;
    if (*(uint64_t*)USB_BOOT_FLAG_ADDR != USB_BOOT_FLAG)
        return;
    *(uint64_t*)USB_BOOT_FLAG_ADDR = 0;
    uint32_t *sysbase = (uint32_t*)CONFIG_STM32_DFU_ROM_ADDRESS;
    asm volatile("mov sp, %0\n bx %1"
                 : : "r"(sysbase[0]), "r"(sysbase[1]));
}
```

- 读 ROM 向量表首两项：栈顶 `sysbase[0]` 和复位向量 `sysbase[1]`，直接设 SP 并 `bx` 跳转。
- 跳转前清掉标记，避免死循环反复进 DFU。
- `dfu_reboot_check()` 在每个 STM32 平台的 `armcm_main()` 最开头调用（见 `stm32f4.c:239`、
  `stm32f7.c:161`、`stm32h7.c:230`、`stm32g0.c:185`、`stm32l4.c:161`）。

各芯片 ROM 地址由 Kconfig 决定（`src/stm32/Kconfig:292`）：

```292:299:src/stm32/Kconfig
config STM32_DFU_ROM_ADDRESS
    hex
    default 0 if !USB
    default 0x1fffc400 if MACH_STM32F042
    default 0x1fffc800 if MACH_STM32F072
    default 0x1fff0000 if MACH_STM32F4 || MACH_STM32F7 || MACH_STM32G0 || MACH_STM32G4 || MACH_STM32L4
    default 0x1ff09800 if MACH_STM32H7
```

---

## 5. 平台专属实现

### 5.1 RP2040 / RP2350（BOOTROM 调用）

RP2040 没有"跳到 ROM 地址"的通用做法，而是调用芯片内建的 BOOTROM 函数表
（`src/rp2040/bootrom.c`）。所有涉及 flash 的函数都标记为 `_ramfunc`
（见 `internal.h:22` 的 `__section(".ramfunc...")`），因为在 XIP 不可用时才能调用。

```75:79:src/rp2040/bootrom.c
void
bootrom_reboot_usb_bootloader(void)
{
    reset_to_usb_boot(0, 0);
}
```

`reset_to_usb_boot` 通过 ROM 函数表查找 `('U','B')` 项得到函数指针并调用
（`bootrom.c:41-47`），进入 USB BOOTSEL 模式。

RP2350 的 BOOTROM 接口略有差异（`src/rp2040/rp2350_bootrom.c:30`），调用
`ROM_FUNC_REBOOT` 并带 `REBOOT2_FLAG_REBOOT_TYPE_BOOTSEL` 标志。

RP2040 的 `bootloader_request()`：

```49:55:src/rp2040/main.c
void
bootloader_request(void)
{
    watchdog_hw->ctrl = 0;
    try_request_canboot();
    bootrom_reboot_usb_bootloader();
}
```

### 5.2 ATSAM（GPNVM bit 1 → ROM 启动）

```102:117:src/atsam/main.c
void noinline __aligned(16) // align for predictable flash code access
bootloader_request(void)
{
    irq_disable();
    // Request boot from ROM (instead of boot from flash)
    while ((EFC_HW->EEFC_FSR & EEFC_FSR_FRDY) == 0)
        ;
    EFC_HW->EEFC_FCR = (EEFC_FCR_FCMD_CGPB | EEFC_FCR_FARG(1)
                        | EEFC_FCR_FKEY_PASSWD);
    while ((EFC_HW->EEFC_FSR & EEFC_FSR_FRDY) == 0)
        ;
    // Reboot
    RSTC->RSTC_CR = RST_PARAMS;
    for (;;)
        ;
}
```

通过 Flash 控制器把 **General Purpose NVM bit 1 (GPNVM1)** 置位，使芯片复位后从
ROM（而非 Flash）启动，从而进入 SAM-BA bootloader。`noinline __aligned(16)`
保证这段写 flash 的代码执行稳定。

### 5.3 STM32F1 系列（HID / STM32duino / N32G45x）

```253:264:src/stm32/stm32f1.c
// Handle reboot requests
void
bootloader_request(void)
{
    try_request_canboot();
    if (CONFIG_STM32_FLASH_START_800)
        usb_hid_bootloader();
    else if (CONFIG_STM32_FLASH_START_2000)
        usb_stm32duino_bootloader();
    else if (CONFIG_MACH_N32G455 && CONFIG_STM32_FLASH_START_10000)
        N32G45x_bootloader_request();
}
```

- `STM32_FLASH_START_800`（2KiB bootloader）→ 走 `usb_hid_bootloader()`（HID 方式）。
- `STM32_FLASH_START_2000`（8KiB）→ `usb_stm32duino_bootloader()`，写备份寄存器
  `BKP->DR10 = 0x01`（stm32duino 魔数）后复位（`stm32f1.c:240-242`）。
- 国民技术 N32G455（64KiB bootloader）→ `N32G45x_bootloader_request()` 仅需复位，
  因为该板 bootloader 是默认启动行为。

---

## 6. 芯片上电引导流程 (`armcm_boot.c`)

所有 Cortex-M 平台共用 `src/generic/armcm_boot.c` 处理复位与早期启动。

### 6.1 两阶段复位处理

```110:121:src/generic/armcm_boot.c
// Initial code entry point - invoked by the processor after a reset
// Reset interrupts and stack to take control from bootloaders
void __section(".text.armcm_boot.stage_one")
ResetHandler(void)
{
    __disable_irq();

    // Explicitly load the stack pointer, jump to stage two
    asm volatile("mov sp, %0\n bx %1"
                 : : "r"(&_stack_end), "r"(reset_handler_stage_two));
}
DECL_ARMCM_IRQ(ResetHandler, -15);
```

- 阶段一（`stage_one`）关中断、显式加载 SP，跳到阶段二。
- 阶段二（`reset_handler_stage_two`，`armcm_boot.c:49`）做：
  1. 清空 NVIC 全部中断使能与挂起（`ICER`/`ICPR`）；
  2. 复位所有中断优先级（`IP`/`IPR`、`SHP`/`SHPR`）；
  3. 关闭 SysTick、清 PENDSV/PENDST；
  4. 把 `.data` 从 Flash 拷到 RAM、清零 `.bss`（用内联 `boot_memset/boot_memcpy`
     避免早期调用 libc）；
  5. 调用平台 `armcm_main()`。

> 关键意义：**即便固件是从 bootloader 跳过来的，Klipper 也会重新初始化中断/栈/
> 数据段，从而"接管"控制权，使 bootloader 的存在对应用透明。** 这也是
> `dfu_reboot_check()` 能在 `armcm_main()` 开头安全执行的前提。

### 6.2 动态内存区间

```136:148:src/generic/armcm_boot.c
// Return the start of memory available for dynamic allocations
void *
dynmem_start(void)
{
    return &_bss_end;
}

// Return the end of memory available for dynamic allocations
void *
dynmem_end(void)
{
    return &_stack_start;
}
```

动态分配区位于 `.bss` 末尾到栈底之间，由链接脚本符号界定。

---

## 7. 配置项 (`Kconfig`)

| 配置 | 位置 | 含义 |
|---|---|---|
| `HAVE_BOOTLOADER_REQUEST` | `src/Kconfig:337` | 全局开关，决定是否编译各类 bootloader 请求逻辑 |
| `HAVE_CHIPID` / `HAVE_LIMITED_CODE_SIZE` | `src/Kconfig:335-339` | 相关能力开关 |
| `FLASH_BOOT_ADDRESS` | `src/stm32/Kconfig:249` | bootloader 基址（默认 `0x8000000`） |
| `FLASH_APPLICATION_ADDRESS` | `src/stm32/Kconfig:339` | 应用起始地址，由"Bootloader offset" choice 决定 |
| `STM32_DFU_ROM_ADDRESS` | `src/stm32/Kconfig:292` | 各芯片 ROM DFU 入口地址 |
| `STM32_FLASH_START_*` | `src/stm32/Kconfig:306-338` | 选择预留给 bootloader 的 Flash 偏移 |

STM32 平台默认 `select HAVE_BOOTLOADER_REQUEST`（`src/stm32/Kconfig:17`）。
当 `FLASH_APPLICATION_ADDRESS == FLASH_BOOT_ADDRESS` 时表示"无独立 bootloader"，
`canboot_reset()`/`armcm_reset` 相关逻辑会直接跳过。

---

## 8. 数据流与调用关系小结

```
外部触发 (USB 1200波特 / 串口魔术串 / CAN admin / reset命令)
        │
        ▼
bootloader_request()                    ← 平台实现 (main.c)
        │
        ├─ try_request_canboot()         ← armcm_reset.c
        │      └─ 若装了 Katapult：在 bootloader 向量表附近写
        │         CANBOOT_REQUEST / CANBOOT_BYPASS，再 NVIC_SystemReset()
        │
        └─ 平台专属进 bootloader 方式：
               STM32 : dfu_reboot() 写 RAM 标记 → 复位 → 下次启动 dfu_reboot_check() 跳 ROM
               RP2040: bootrom_reboot_usb_bootloader() 调 BOOTROM → BOOTSEL
               ATSAM : 置 GPNVM1 → 复位 → ROM(SAM-BA) 启动
               LPC176x: 置 WDT flag → 复位 → DFU bootloader 接管
               STM32F1: HID / STM32duino(BKP魔数) / N32G45x
```

---

## 9. 分析与注意事项

1. **优先级顺序**：`bootloader_request()` 总是先 `try_request_canboot()`。
   如果板子上装有 Katapult，则优先复用它；只有当 Katapult 不存在
   （签名校验失败/未安装）时，才会走芯片原生 bootloader。这与
   [`docs/Bootloader_Entry.md`](Bootloader_Entry.md) "reboot to Katapult if
   installed, then a hardware specific bootloader" 的描述一致。

2. **复位后状态清理是正确性的关键**：`armcm_boot.c` 的 `reset_handler_stage_two`
   会重新初始化中断、栈、`.data`/`.bss`，确保从任意 bootloader 跳入时都能干净接管。
   若没有这步，bootloader 遗留的中断使能/栈指针会导致未定义行为。

3. **标记持久性风险**：DFU 与 Katapult 都靠"复位存活的 RAM 标记"跨复位传递意图。
   - `USB_BOOT_FLAG` 放在 RAM 末尾 1KB，正常情况下不被覆盖；
   - `CANBOOT_*` 写在 Katapult 向量表附近固定偏移，依赖 Katapult 自身布局。
   一旦标记未被正确清除（如跳转前异常），可能造成反复进 bootloader。

4. **DCache 一致性**：Cortex-M7（H7）写标记后必须 `SCB_CleanDCache_by_Addr`，
   否则写可能留在 cache，复位后 RAM 中仍是旧值，DFU 标记失效。

5. **DFU 安全风险**（官方已提示）：部分板卡进入 STM32 DFU 时 GPIO 状态不确定，
   可能误触发加热器等危险动作（如 Octopus Pro v1）。刷新固件前建议断开加热器。

6. **平台碎片化**：bootloader 机制高度依赖芯片特性（ROM 地址、GPNVM、WDT flag、
   BOOTROM API），因此 `bootloader_request()` 必须在各平台 `main.c` 单独实现，
   而 CANBoot 请求与触发事件解析则尽量下沉到 `generic/` 通用层，做到"请求逻辑
   统一、跳转细节平台化"。

---

## 10. 关键文件索引

| 文件 | 职责 |
|---|---|
| `src/generic/misc.h` | `bootloader_request()` 声明 |
| `src/generic/serial_irq.c` | 物理串口魔术串触发 |
| `src/generic/usb_cdc.c` | USB 1200 波特 DTR 触发 |
| `src/generic/canserial.c` | CANBUS admin 触发 + 解析 |
| `src/generic/armcm_reset.c` | Katapult/CANBoot 请求 + `reset` 命令 |
| `src/generic/armcm_boot.c` | Cortex-M 复位/启动引导、向量表、data/bss |
| `src/stm32/dfu_reboot.c` | STM32 ROM DFU 标记与跳转 |
| `src/stm32/stm32f1.c` | STM32F1 HID/STM32duino/N32G45x 跳转 |
| `src/rp2040/bootrom.c` / `rp2350_bootrom.c` | RP 系列 BOOTROM 调用 |
| `src/atsam/main.c` | ATSAM GPNVM 置位进 SAM-BA |
| `src/lpc176x/main.c` | LPC176x WDT flag 进 DFU |
| `src/stm32/Kconfig` | STM32 bootloader 偏移/ROM 地址配置 |
| `docs/Bootloader_Entry.md` / `docs/Bootloaders.md` | 用户侧触发文档 |
