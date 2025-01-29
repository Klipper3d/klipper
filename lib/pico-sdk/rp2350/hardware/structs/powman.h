// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_POWMAN_H
#define _HARDWARE_STRUCTS_POWMAN_H

/**
 * \file rp2350/powman.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/powman.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_powman
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/powman.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(POWMAN_BADPASSWD_OFFSET) // POWMAN_BADPASSWD
    // Indicates a bad password has been used
    // 0x00000001 [0]     BADPASSWD    (0) 
    io_rw_32 badpasswd;
 
    _REG_(POWMAN_VREG_CTRL_OFFSET) // POWMAN_VREG_CTRL
    // Voltage Regulator Control
    // 0x00008000 [15]    RST_N        (1) returns the regulator to its startup settings +
    // 0x00002000 [13]    UNLOCK       (0) unlocks the VREG control interface after power up +
    // 0x00001000 [12]    ISOLATE      (0) isolates the VREG control interface +
    // 0x00000100 [8]     DISABLE_VOLTAGE_LIMIT (0) 0=not disabled, 1=enabled
    // 0x00000070 [6:4]   HT_TH        (0x5) high temperature protection threshold +
    io_rw_32 vreg_ctrl;
 
    _REG_(POWMAN_VREG_STS_OFFSET) // POWMAN_VREG_STS
    // Voltage Regulator Status
    // 0x00000010 [4]     VOUT_OK      (0) output regulation status +
    // 0x00000001 [0]     STARTUP      (0) startup status +
    io_ro_32 vreg_sts;
 
    _REG_(POWMAN_VREG_OFFSET) // POWMAN_VREG
    // Voltage Regulator Settings
    // 0x00008000 [15]    UPDATE_IN_PROGRESS (0) regulator state is being updated +
    // 0x000001f0 [8:4]   VSEL         (0x0b) output voltage select +
    // 0x00000002 [1]     HIZ          (0) high impedance mode select +
    io_rw_32 vreg;
 
    _REG_(POWMAN_VREG_LP_ENTRY_OFFSET) // POWMAN_VREG_LP_ENTRY
    // Voltage Regulator Low Power Entry Settings
    // 0x000001f0 [8:4]   VSEL         (0x0b) output voltage select +
    // 0x00000004 [2]     MODE         (1) selects either normal (switching) mode or low power...
    // 0x00000002 [1]     HIZ          (0) high impedance mode select +
    io_rw_32 vreg_lp_entry;
 
    _REG_(POWMAN_VREG_LP_EXIT_OFFSET) // POWMAN_VREG_LP_EXIT
    // Voltage Regulator Low Power Exit Settings
    // 0x000001f0 [8:4]   VSEL         (0x0b) output voltage select +
    // 0x00000004 [2]     MODE         (0) selects either normal (switching) mode or low power...
    // 0x00000002 [1]     HIZ          (0) high impedance mode select +
    io_rw_32 vreg_lp_exit;
 
    _REG_(POWMAN_BOD_CTRL_OFFSET) // POWMAN_BOD_CTRL
    // Brown-out Detection Control
    // 0x00001000 [12]    ISOLATE      (0) isolates the brown-out detection control interface +
    io_rw_32 bod_ctrl;
 
    _REG_(POWMAN_BOD_OFFSET) // POWMAN_BOD
    // Brown-out Detection Settings
    // 0x000001f0 [8:4]   VSEL         (0x0b) threshold select +
    // 0x00000001 [0]     EN           (1) enable brown-out detection +
    io_rw_32 bod;
 
    _REG_(POWMAN_BOD_LP_ENTRY_OFFSET) // POWMAN_BOD_LP_ENTRY
    // Brown-out Detection Low Power Entry Settings
    // 0x000001f0 [8:4]   VSEL         (0x0b) threshold select +
    // 0x00000001 [0]     EN           (0) enable brown-out detection +
    io_rw_32 bod_lp_entry;
 
    _REG_(POWMAN_BOD_LP_EXIT_OFFSET) // POWMAN_BOD_LP_EXIT
    // Brown-out Detection Low Power Exit Settings
    // 0x000001f0 [8:4]   VSEL         (0x0b) threshold select +
    // 0x00000001 [0]     EN           (1) enable brown-out detection +
    io_rw_32 bod_lp_exit;
 
    _REG_(POWMAN_LPOSC_OFFSET) // POWMAN_LPOSC
    // Low power oscillator control register
    // 0x000003f0 [9:4]   TRIM         (0x20) Frequency trim - the trim step is typically 1% of the...
    // 0x00000003 [1:0]   MODE         (0x3) This feature has been removed
    io_rw_32 lposc;
 
    _REG_(POWMAN_CHIP_RESET_OFFSET) // POWMAN_CHIP_RESET
    // Chip reset control and status
    // 0x10000000 [28]    HAD_WATCHDOG_RESET_RSM (0) Last reset was a watchdog timeout which was configured...
    // 0x08000000 [27]    HAD_HZD_SYS_RESET_REQ (0) Last reset was a system reset from the hazard debugger +
    // 0x04000000 [26]    HAD_GLITCH_DETECT (0) Last reset was due to a power supply glitch +
    // 0x02000000 [25]    HAD_SWCORE_PD (0) Last reset was a switched core powerdown +
    // 0x01000000 [24]    HAD_WATCHDOG_RESET_SWCORE (0) Last reset was a watchdog timeout which was configured...
    // 0x00800000 [23]    HAD_WATCHDOG_RESET_POWMAN (0) Last reset was a watchdog timeout which was configured...
    // 0x00400000 [22]    HAD_WATCHDOG_RESET_POWMAN_ASYNC (0) Last reset was a watchdog timeout which was configured...
    // 0x00200000 [21]    HAD_RESCUE   (0) Last reset was a rescue reset from the debugger +
    // 0x00080000 [19]    HAD_DP_RESET_REQ (0) Last reset was an reset request from the arm debugger +
    // 0x00040000 [18]    HAD_RUN_LOW  (0) Last reset was from the RUN pin +
    // 0x00020000 [17]    HAD_BOR      (0) Last reset was from the brown-out detection block +
    // 0x00010000 [16]    HAD_POR      (0) Last reset was from the power-on reset +
    // 0x00000010 [4]     RESCUE_FLAG  (0) This is set by a rescue reset from the RP-AP
    // 0x00000001 [0]     DOUBLE_TAP   (0) This flag is set by double-tapping RUN
    io_rw_32 chip_reset;
 
    _REG_(POWMAN_WDSEL_OFFSET) // POWMAN_WDSEL
    // Allows a watchdog reset to reset the internal state of powman in addition to the power-on state...
    // 0x00001000 [12]    RESET_RSM    (0) If set to 1, a watchdog reset will run the full power-on...
    // 0x00000100 [8]     RESET_SWCORE (0) If set to 1, a watchdog reset will reset the switched...
    // 0x00000010 [4]     RESET_POWMAN (0) If set to 1, a watchdog reset will restore powman...
    // 0x00000001 [0]     RESET_POWMAN_ASYNC (0) If set to 1, a watchdog reset will restore powman...
    io_rw_32 wdsel;
 
    _REG_(POWMAN_SEQ_CFG_OFFSET) // POWMAN_SEQ_CFG
    // For configuration of the power sequencer +
    // 0x00100000 [20]    USING_FAST_POWCK (1) 0 indicates the POWMAN clock is running from the low...
    // 0x00020000 [17]    USING_BOD_LP (0) Indicates the brown-out detector (BOD) mode +
    // 0x00010000 [16]    USING_VREG_LP (0) Indicates the voltage regulator (VREG) mode +
    // 0x00001000 [12]    USE_FAST_POWCK (1) selects the reference clock (clk_ref) as the source of...
    // 0x00000100 [8]     RUN_LPOSC_IN_LP (1) Set to 0 to stop the low power osc when the...
    // 0x00000080 [7]     USE_BOD_HP   (1) Set to 0 to prevent automatic switching to bod high...
    // 0x00000040 [6]     USE_BOD_LP   (1) Set to 0 to prevent automatic switching to bod low power...
    // 0x00000020 [5]     USE_VREG_HP  (1) Set to 0 to prevent automatic switching to vreg high...
    // 0x00000010 [4]     USE_VREG_LP  (1) Set to 0 to prevent automatic switching to vreg low...
    // 0x00000002 [1]     HW_PWRUP_SRAM0 (0) Specifies the power state of SRAM0 when powering up...
    // 0x00000001 [0]     HW_PWRUP_SRAM1 (0) Specifies the power state of SRAM1 when powering up...
    io_rw_32 seq_cfg;
 
    _REG_(POWMAN_STATE_OFFSET) // POWMAN_STATE
    // This register controls the power state of the 4 power domains
    // 0x00002000 [13]    CHANGING     (0) 
    // 0x00001000 [12]    WAITING      (0) 
    // 0x00000800 [11]    BAD_HW_REQ   (0) Bad hardware initiated state request
    // 0x00000400 [10]    BAD_SW_REQ   (0) Bad software initiated state request
    // 0x00000200 [9]     PWRUP_WHILE_WAITING (0) Request ignored because of a pending pwrup request
    // 0x00000100 [8]     REQ_IGNORED  (0) 
    // 0x000000f0 [7:4]   REQ          (0x0) 
    // 0x0000000f [3:0]   CURRENT      (0xf) 
    io_rw_32 state;
 
    _REG_(POWMAN_POW_FASTDIV_OFFSET) // POWMAN_POW_FASTDIV
    // 0x000007ff [10:0]  POW_FASTDIV  (0x040) divides the POWMAN clock to provide a tick for the delay...
    io_rw_32 pow_fastdiv;
 
    _REG_(POWMAN_POW_DELAY_OFFSET) // POWMAN_POW_DELAY
    // power state machine delays
    // 0x0000ff00 [15:8]  SRAM_STEP    (0x20) timing between the sram0 and sram1 power state machine steps +
    // 0x000000f0 [7:4]   XIP_STEP     (0x1) timing between the xip power state machine steps +
    // 0x0000000f [3:0]   SWCORE_STEP  (0x1) timing between the swcore power state machine steps +
    io_rw_32 pow_delay;
 
    // (Description copied from array index 0 register POWMAN_EXT_CTRL0 applies similarly to other array indexes)
    _REG_(POWMAN_EXT_CTRL0_OFFSET) // POWMAN_EXT_CTRL0
    // Configures a gpio as a power mode aware control output
    // 0x00004000 [14]    LP_EXIT_STATE (0) output level when exiting the low power state
    // 0x00002000 [13]    LP_ENTRY_STATE (0) output level when entering the low power state
    // 0x00001000 [12]    INIT_STATE   (0) 
    // 0x00000100 [8]     INIT         (0) 
    // 0x0000003f [5:0]   GPIO_SELECT  (0x3f) selects from gpio 0->30 +
    io_rw_32 ext_ctrl[2];
 
    _REG_(POWMAN_EXT_TIME_REF_OFFSET) // POWMAN_EXT_TIME_REF
    // Select a GPIO to use as a time reference, the source can be used to drive the low power clock at...
    // 0x00000010 [4]     DRIVE_LPCK   (0) Use the selected GPIO to drive the 32kHz low power...
    // 0x00000003 [1:0]   SOURCE_SEL   (0x0) 0 ->  gpio12 +
    io_rw_32 ext_time_ref;
 
    _REG_(POWMAN_LPOSC_FREQ_KHZ_INT_OFFSET) // POWMAN_LPOSC_FREQ_KHZ_INT
    // Informs the AON Timer of the integer component of the clock frequency when running off the LPOSC
    // 0x0000003f [5:0]   LPOSC_FREQ_KHZ_INT (0x20) Integer component of the LPOSC or GPIO clock source...
    io_rw_32 lposc_freq_khz_int;
 
    _REG_(POWMAN_LPOSC_FREQ_KHZ_FRAC_OFFSET) // POWMAN_LPOSC_FREQ_KHZ_FRAC
    // Informs the AON Timer of the fractional component of the clock frequency when running off the LPOSC
    // 0x0000ffff [15:0]  LPOSC_FREQ_KHZ_FRAC (0xc49c) Fractional component of the LPOSC or GPIO clock source...
    io_rw_32 lposc_freq_khz_frac;
 
    _REG_(POWMAN_XOSC_FREQ_KHZ_INT_OFFSET) // POWMAN_XOSC_FREQ_KHZ_INT
    // Informs the AON Timer of the integer component of the clock frequency when running off the XOSC
    // 0x0000ffff [15:0]  XOSC_FREQ_KHZ_INT (0x2ee0) Integer component of the XOSC frequency in kHz
    io_rw_32 xosc_freq_khz_int;
 
    _REG_(POWMAN_XOSC_FREQ_KHZ_FRAC_OFFSET) // POWMAN_XOSC_FREQ_KHZ_FRAC
    // Informs the AON Timer of the fractional component of the clock frequency when running off the XOSC
    // 0x0000ffff [15:0]  XOSC_FREQ_KHZ_FRAC (0x0000) Fractional component of the XOSC frequency in kHz
    io_rw_32 xosc_freq_khz_frac;
 
    _REG_(POWMAN_SET_TIME_63TO48_OFFSET) // POWMAN_SET_TIME_63TO48
    // 0x0000ffff [15:0]  SET_TIME_63TO48 (0x0000) For setting the time, do not use for reading the time,...
    io_rw_32 set_time_63to48;
 
    _REG_(POWMAN_SET_TIME_47TO32_OFFSET) // POWMAN_SET_TIME_47TO32
    // 0x0000ffff [15:0]  SET_TIME_47TO32 (0x0000) For setting the time, do not use for reading the time,...
    io_rw_32 set_time_47to32;
 
    _REG_(POWMAN_SET_TIME_31TO16_OFFSET) // POWMAN_SET_TIME_31TO16
    // 0x0000ffff [15:0]  SET_TIME_31TO16 (0x0000) For setting the time, do not use for reading the time,...
    io_rw_32 set_time_31to16;
 
    _REG_(POWMAN_SET_TIME_15TO0_OFFSET) // POWMAN_SET_TIME_15TO0
    // 0x0000ffff [15:0]  SET_TIME_15TO0 (0x0000) For setting the time, do not use for reading the time,...
    io_rw_32 set_time_15to0;
 
    _REG_(POWMAN_READ_TIME_UPPER_OFFSET) // POWMAN_READ_TIME_UPPER
    // 0xffffffff [31:0]  READ_TIME_UPPER (0x00000000) For reading bits 63:32 of the timer
    io_ro_32 read_time_upper;
 
    _REG_(POWMAN_READ_TIME_LOWER_OFFSET) // POWMAN_READ_TIME_LOWER
    // 0xffffffff [31:0]  READ_TIME_LOWER (0x00000000) For reading bits 31:0 of the timer
    io_ro_32 read_time_lower;
 
    _REG_(POWMAN_ALARM_TIME_63TO48_OFFSET) // POWMAN_ALARM_TIME_63TO48
    // 0x0000ffff [15:0]  ALARM_TIME_63TO48 (0x0000) This field must only be written when POWMAN_ALARM_ENAB=0
    io_rw_32 alarm_time_63to48;
 
    _REG_(POWMAN_ALARM_TIME_47TO32_OFFSET) // POWMAN_ALARM_TIME_47TO32
    // 0x0000ffff [15:0]  ALARM_TIME_47TO32 (0x0000) This field must only be written when POWMAN_ALARM_ENAB=0
    io_rw_32 alarm_time_47to32;
 
    _REG_(POWMAN_ALARM_TIME_31TO16_OFFSET) // POWMAN_ALARM_TIME_31TO16
    // 0x0000ffff [15:0]  ALARM_TIME_31TO16 (0x0000) This field must only be written when POWMAN_ALARM_ENAB=0
    io_rw_32 alarm_time_31to16;
 
    _REG_(POWMAN_ALARM_TIME_15TO0_OFFSET) // POWMAN_ALARM_TIME_15TO0
    // 0x0000ffff [15:0]  ALARM_TIME_15TO0 (0x0000) This field must only be written when POWMAN_ALARM_ENAB=0
    io_rw_32 alarm_time_15to0;
 
    _REG_(POWMAN_TIMER_OFFSET) // POWMAN_TIMER
    // 0x00080000 [19]    USING_GPIO_1HZ (0) Timer is synchronised to a 1hz gpio source
    // 0x00040000 [18]    USING_GPIO_1KHZ (0) Timer is running from a 1khz gpio source
    // 0x00020000 [17]    USING_LPOSC  (0) Timer is running from lposc
    // 0x00010000 [16]    USING_XOSC   (0) Timer is running from xosc
    // 0x00002000 [13]    USE_GPIO_1HZ (0) Selects the gpio source as the reference for the sec counter
    // 0x00000400 [10]    USE_GPIO_1KHZ (0) switch to gpio as the source of the 1kHz timer tick
    // 0x00000200 [9]     USE_XOSC     (0) switch to xosc as the source of the 1kHz timer tick
    // 0x00000100 [8]     USE_LPOSC    (0) Switch to lposc as the source of the 1kHz timer tick
    // 0x00000040 [6]     ALARM        (0) Alarm has fired
    // 0x00000020 [5]     PWRUP_ON_ALARM (0) Alarm wakes the chip from low power mode
    // 0x00000010 [4]     ALARM_ENAB   (0) Enables the alarm
    // 0x00000004 [2]     CLEAR        (0) Clears the timer, does not disable the timer and does...
    // 0x00000002 [1]     RUN          (0) Timer enable
    // 0x00000001 [0]     NONSEC_WRITE (0) Control whether Non-secure software can write to the...
    io_rw_32 timer;
 
    // (Description copied from array index 0 register POWMAN_PWRUP0 applies similarly to other array indexes)
    _REG_(POWMAN_PWRUP0_OFFSET) // POWMAN_PWRUP0
    // 4 GPIO powerup events can be configured to wake the chip up from a low power state
    // 0x00000400 [10]    RAW_STATUS   (0) Value of selected gpio pin (only if enable == 1)
    // 0x00000200 [9]     STATUS       (0) Status of gpio wakeup
    // 0x00000100 [8]     MODE         (0) Edge or level detect
    // 0x00000080 [7]     DIRECTION    (0) 
    // 0x00000040 [6]     ENABLE       (0) Set to 1 to enable the wakeup source
    // 0x0000003f [5:0]   SOURCE       (0x3f) 
    io_rw_32 pwrup[4];
 
    _REG_(POWMAN_CURRENT_PWRUP_REQ_OFFSET) // POWMAN_CURRENT_PWRUP_REQ
    // Indicates current powerup request state +
    // 0x0000007f [6:0]   CURRENT_PWRUP_REQ (0x00) 
    io_ro_32 current_pwrup_req;
 
    _REG_(POWMAN_LAST_SWCORE_PWRUP_OFFSET) // POWMAN_LAST_SWCORE_PWRUP
    // Indicates which pwrup source triggered the last switched-core power up +
    // 0x0000007f [6:0]   LAST_SWCORE_PWRUP (0x00) 
    io_ro_32 last_swcore_pwrup;
 
    _REG_(POWMAN_DBG_PWRCFG_OFFSET) // POWMAN_DBG_PWRCFG
    // 0x00000001 [0]     IGNORE       (0) Ignore pwrup req from debugger
    io_rw_32 dbg_pwrcfg;
 
    _REG_(POWMAN_BOOTDIS_OFFSET) // POWMAN_BOOTDIS
    // Tell the bootrom to ignore the BOOT0
    // 0x00000002 [1]     NEXT         (0) This flag always ORs writes into its current contents
    // 0x00000001 [0]     NOW          (0) When powman resets the RSM, the current value of...
    io_rw_32 bootdis;
 
    _REG_(POWMAN_DBGCONFIG_OFFSET) // POWMAN_DBGCONFIG
    // 0x0000000f [3:0]   DP_INSTID    (0x0) Configure DP instance ID for SWD multidrop selection
    io_rw_32 dbgconfig;
 
    // (Description copied from array index 0 register POWMAN_SCRATCH0 applies similarly to other array indexes)
    _REG_(POWMAN_SCRATCH0_OFFSET) // POWMAN_SCRATCH0
    // Scratch register
    // 0xffffffff [31:0]  SCRATCH0     (0x00000000) 
    io_rw_32 scratch[8];
 
    // (Description copied from array index 0 register POWMAN_BOOT0 applies similarly to other array indexes)
    _REG_(POWMAN_BOOT0_OFFSET) // POWMAN_BOOT0
    // Scratch register
    // 0xffffffff [31:0]  BOOT0        (0x00000000) 
    io_rw_32 boot[4];
 
    _REG_(POWMAN_INTR_OFFSET) // POWMAN_INTR
    // Raw Interrupts
    // 0x00000008 [3]     PWRUP_WHILE_WAITING (0) Source is state
    // 0x00000004 [2]     STATE_REQ_IGNORED (0) Source is state
    // 0x00000002 [1]     TIMER        (0) 
    // 0x00000001 [0]     VREG_OUTPUT_LOW (0) 
    io_rw_32 intr;
 
    _REG_(POWMAN_INTE_OFFSET) // POWMAN_INTE
    // Interrupt Enable
    // 0x00000008 [3]     PWRUP_WHILE_WAITING (0) Source is state
    // 0x00000004 [2]     STATE_REQ_IGNORED (0) Source is state
    // 0x00000002 [1]     TIMER        (0) 
    // 0x00000001 [0]     VREG_OUTPUT_LOW (0) 
    io_rw_32 inte;
 
    _REG_(POWMAN_INTF_OFFSET) // POWMAN_INTF
    // Interrupt Force
    // 0x00000008 [3]     PWRUP_WHILE_WAITING (0) Source is state
    // 0x00000004 [2]     STATE_REQ_IGNORED (0) Source is state
    // 0x00000002 [1]     TIMER        (0) 
    // 0x00000001 [0]     VREG_OUTPUT_LOW (0) 
    io_rw_32 intf;
 
    _REG_(POWMAN_INTS_OFFSET) // POWMAN_INTS
    // Interrupt status after masking & forcing
    // 0x00000008 [3]     PWRUP_WHILE_WAITING (0) Source is state
    // 0x00000004 [2]     STATE_REQ_IGNORED (0) Source is state
    // 0x00000002 [1]     TIMER        (0) 
    // 0x00000001 [0]     VREG_OUTPUT_LOW (0) 
    io_ro_32 ints;
} powman_hw_t;

#define powman_hw ((powman_hw_t *)POWMAN_BASE)
static_assert(sizeof (powman_hw_t) == 0x00f0, "");

#endif // _HARDWARE_STRUCTS_POWMAN_H

