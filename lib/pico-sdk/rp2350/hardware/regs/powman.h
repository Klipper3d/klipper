// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : POWMAN
// Version        : 1
// Bus type       : apb
// Description    : Controls vreg, bor, lposc, chip resets & xosc startup,
//                  powman and provides scratch register for general use and for
//                  bootcode use
// =============================================================================
#ifndef _HARDWARE_REGS_POWMAN_H
#define _HARDWARE_REGS_POWMAN_H
// =============================================================================
// Register    : POWMAN_BADPASSWD
// Description : Indicates a bad password has been used
#define POWMAN_BADPASSWD_OFFSET _u(0x00000000)
#define POWMAN_BADPASSWD_BITS   _u(0x00000001)
#define POWMAN_BADPASSWD_RESET  _u(0x00000000)
#define POWMAN_BADPASSWD_MSB    _u(0)
#define POWMAN_BADPASSWD_LSB    _u(0)
#define POWMAN_BADPASSWD_ACCESS "WC"
// =============================================================================
// Register    : POWMAN_VREG_CTRL
// Description : Voltage Regulator Control
#define POWMAN_VREG_CTRL_OFFSET _u(0x00000004)
#define POWMAN_VREG_CTRL_BITS   _u(0x0000b170)
#define POWMAN_VREG_CTRL_RESET  _u(0x00008050)
// -----------------------------------------------------------------------------
// Field       : POWMAN_VREG_CTRL_RST_N
// Description : returns the regulator to its startup settings
//               0 - reset
//               1 - not reset (default)
#define POWMAN_VREG_CTRL_RST_N_RESET  _u(0x1)
#define POWMAN_VREG_CTRL_RST_N_BITS   _u(0x00008000)
#define POWMAN_VREG_CTRL_RST_N_MSB    _u(15)
#define POWMAN_VREG_CTRL_RST_N_LSB    _u(15)
#define POWMAN_VREG_CTRL_RST_N_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_VREG_CTRL_UNLOCK
// Description : unlocks the VREG control interface after power up
//               0 - Locked (default)
//               1 - Unlocked
//               It cannot be relocked when it is unlocked.
#define POWMAN_VREG_CTRL_UNLOCK_RESET  _u(0x0)
#define POWMAN_VREG_CTRL_UNLOCK_BITS   _u(0x00002000)
#define POWMAN_VREG_CTRL_UNLOCK_MSB    _u(13)
#define POWMAN_VREG_CTRL_UNLOCK_LSB    _u(13)
#define POWMAN_VREG_CTRL_UNLOCK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_VREG_CTRL_ISOLATE
// Description : isolates the VREG control interface
//               0 - not isolated (default)
//               1 - isolated
#define POWMAN_VREG_CTRL_ISOLATE_RESET  _u(0x0)
#define POWMAN_VREG_CTRL_ISOLATE_BITS   _u(0x00001000)
#define POWMAN_VREG_CTRL_ISOLATE_MSB    _u(12)
#define POWMAN_VREG_CTRL_ISOLATE_LSB    _u(12)
#define POWMAN_VREG_CTRL_ISOLATE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_VREG_CTRL_DISABLE_VOLTAGE_LIMIT
// Description : 0=not disabled, 1=enabled
#define POWMAN_VREG_CTRL_DISABLE_VOLTAGE_LIMIT_RESET  _u(0x0)
#define POWMAN_VREG_CTRL_DISABLE_VOLTAGE_LIMIT_BITS   _u(0x00000100)
#define POWMAN_VREG_CTRL_DISABLE_VOLTAGE_LIMIT_MSB    _u(8)
#define POWMAN_VREG_CTRL_DISABLE_VOLTAGE_LIMIT_LSB    _u(8)
#define POWMAN_VREG_CTRL_DISABLE_VOLTAGE_LIMIT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_VREG_CTRL_HT_TH
// Description : high temperature protection threshold
//               regulator power transistors are disabled when junction
//               temperature exceeds threshold
//               000 - 100C
//               001 - 105C
//               010 - 110C
//               011 - 115C
//               100 - 120C
//               101 - 125C
//               110 - 135C
//               111 - 150C
#define POWMAN_VREG_CTRL_HT_TH_RESET  _u(0x5)
#define POWMAN_VREG_CTRL_HT_TH_BITS   _u(0x00000070)
#define POWMAN_VREG_CTRL_HT_TH_MSB    _u(6)
#define POWMAN_VREG_CTRL_HT_TH_LSB    _u(4)
#define POWMAN_VREG_CTRL_HT_TH_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_VREG_STS
// Description : Voltage Regulator Status
#define POWMAN_VREG_STS_OFFSET _u(0x00000008)
#define POWMAN_VREG_STS_BITS   _u(0x00000011)
#define POWMAN_VREG_STS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : POWMAN_VREG_STS_VOUT_OK
// Description : output regulation status
//               0=not in regulation, 1=in regulation
#define POWMAN_VREG_STS_VOUT_OK_RESET  _u(0x0)
#define POWMAN_VREG_STS_VOUT_OK_BITS   _u(0x00000010)
#define POWMAN_VREG_STS_VOUT_OK_MSB    _u(4)
#define POWMAN_VREG_STS_VOUT_OK_LSB    _u(4)
#define POWMAN_VREG_STS_VOUT_OK_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_VREG_STS_STARTUP
// Description : startup status
//               0=startup complete, 1=starting up
#define POWMAN_VREG_STS_STARTUP_RESET  _u(0x0)
#define POWMAN_VREG_STS_STARTUP_BITS   _u(0x00000001)
#define POWMAN_VREG_STS_STARTUP_MSB    _u(0)
#define POWMAN_VREG_STS_STARTUP_LSB    _u(0)
#define POWMAN_VREG_STS_STARTUP_ACCESS "RO"
// =============================================================================
// Register    : POWMAN_VREG
// Description : Voltage Regulator Settings
#define POWMAN_VREG_OFFSET _u(0x0000000c)
#define POWMAN_VREG_BITS   _u(0x000081f2)
#define POWMAN_VREG_RESET  _u(0x000000b0)
// -----------------------------------------------------------------------------
// Field       : POWMAN_VREG_UPDATE_IN_PROGRESS
// Description : regulator state is being updated
//               writes to the vreg register will be ignored when this field is
//               set
#define POWMAN_VREG_UPDATE_IN_PROGRESS_RESET  _u(0x0)
#define POWMAN_VREG_UPDATE_IN_PROGRESS_BITS   _u(0x00008000)
#define POWMAN_VREG_UPDATE_IN_PROGRESS_MSB    _u(15)
#define POWMAN_VREG_UPDATE_IN_PROGRESS_LSB    _u(15)
#define POWMAN_VREG_UPDATE_IN_PROGRESS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_VREG_VSEL
// Description : output voltage select
//               the regulator output voltage is limited to 1.3V unless the
//               voltage limit
//               is disabled using the disable_voltage_limit field in the
//               vreg_ctrl register
//               00000 - 0.55V
//               00001 - 0.60V
//               00010 - 0.65V
//               00011 - 0.70V
//               00100 - 0.75V
//               00101 - 0.80V
//               00110 - 0.85V
//               00111 - 0.90V
//               01000 - 0.95V
//               01001 - 1.00V
//               01010 - 1.05V
//               01011 - 1.10V (default)
//               01100 - 1.15V
//               01101 - 1.20V
//               01110 - 1.25V
//               01111 - 1.30V
//               10000 - 1.35V
//               10001 - 1.40V
//               10010 - 1.50V
//               10011 - 1.60V
//               10100 - 1.65V
//               10101 - 1.70V
//               10110 - 1.80V
//               10111 - 1.90V
//               11000 - 2.00V
//               11001 - 2.35V
//               11010 - 2.50V
//               11011 - 2.65V
//               11100 - 2.80V
//               11101 - 3.00V
//               11110 - 3.15V
//               11111 - 3.30V
#define POWMAN_VREG_VSEL_RESET  _u(0x0b)
#define POWMAN_VREG_VSEL_BITS   _u(0x000001f0)
#define POWMAN_VREG_VSEL_MSB    _u(8)
#define POWMAN_VREG_VSEL_LSB    _u(4)
#define POWMAN_VREG_VSEL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_VREG_HIZ
// Description : high impedance mode select
//               0=not in high impedance mode, 1=in high impedance mode
#define POWMAN_VREG_HIZ_RESET  _u(0x0)
#define POWMAN_VREG_HIZ_BITS   _u(0x00000002)
#define POWMAN_VREG_HIZ_MSB    _u(1)
#define POWMAN_VREG_HIZ_LSB    _u(1)
#define POWMAN_VREG_HIZ_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_VREG_LP_ENTRY
// Description : Voltage Regulator Low Power Entry Settings
#define POWMAN_VREG_LP_ENTRY_OFFSET _u(0x00000010)
#define POWMAN_VREG_LP_ENTRY_BITS   _u(0x000001f6)
#define POWMAN_VREG_LP_ENTRY_RESET  _u(0x000000b4)
// -----------------------------------------------------------------------------
// Field       : POWMAN_VREG_LP_ENTRY_VSEL
// Description : output voltage select
//               the regulator output voltage is limited to 1.3V unless the
//               voltage limit
//               is disabled using the disable_voltage_limit field in the
//               vreg_ctrl register
//               00000 - 0.55V
//               00001 - 0.60V
//               00010 - 0.65V
//               00011 - 0.70V
//               00100 - 0.75V
//               00101 - 0.80V
//               00110 - 0.85V
//               00111 - 0.90V
//               01000 - 0.95V
//               01001 - 1.00V
//               01010 - 1.05V
//               01011 - 1.10V (default)
//               01100 - 1.15V
//               01101 - 1.20V
//               01110 - 1.25V
//               01111 - 1.30V
//               10000 - 1.35V
//               10001 - 1.40V
//               10010 - 1.50V
//               10011 - 1.60V
//               10100 - 1.65V
//               10101 - 1.70V
//               10110 - 1.80V
//               10111 - 1.90V
//               11000 - 2.00V
//               11001 - 2.35V
//               11010 - 2.50V
//               11011 - 2.65V
//               11100 - 2.80V
//               11101 - 3.00V
//               11110 - 3.15V
//               11111 - 3.30V
#define POWMAN_VREG_LP_ENTRY_VSEL_RESET  _u(0x0b)
#define POWMAN_VREG_LP_ENTRY_VSEL_BITS   _u(0x000001f0)
#define POWMAN_VREG_LP_ENTRY_VSEL_MSB    _u(8)
#define POWMAN_VREG_LP_ENTRY_VSEL_LSB    _u(4)
#define POWMAN_VREG_LP_ENTRY_VSEL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_VREG_LP_ENTRY_MODE
// Description : selects either normal (switching) mode or low power (linear)
//               mode
//               low power mode can only be selected for output voltages up to
//               1.3V
//               0 = normal mode (switching)
//               1 = low power mode (linear)
#define POWMAN_VREG_LP_ENTRY_MODE_RESET  _u(0x1)
#define POWMAN_VREG_LP_ENTRY_MODE_BITS   _u(0x00000004)
#define POWMAN_VREG_LP_ENTRY_MODE_MSB    _u(2)
#define POWMAN_VREG_LP_ENTRY_MODE_LSB    _u(2)
#define POWMAN_VREG_LP_ENTRY_MODE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_VREG_LP_ENTRY_HIZ
// Description : high impedance mode select
//               0=not in high impedance mode, 1=in high impedance mode
#define POWMAN_VREG_LP_ENTRY_HIZ_RESET  _u(0x0)
#define POWMAN_VREG_LP_ENTRY_HIZ_BITS   _u(0x00000002)
#define POWMAN_VREG_LP_ENTRY_HIZ_MSB    _u(1)
#define POWMAN_VREG_LP_ENTRY_HIZ_LSB    _u(1)
#define POWMAN_VREG_LP_ENTRY_HIZ_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_VREG_LP_EXIT
// Description : Voltage Regulator Low Power Exit Settings
#define POWMAN_VREG_LP_EXIT_OFFSET _u(0x00000014)
#define POWMAN_VREG_LP_EXIT_BITS   _u(0x000001f6)
#define POWMAN_VREG_LP_EXIT_RESET  _u(0x000000b0)
// -----------------------------------------------------------------------------
// Field       : POWMAN_VREG_LP_EXIT_VSEL
// Description : output voltage select
//               the regulator output voltage is limited to 1.3V unless the
//               voltage limit
//               is disabled using the disable_voltage_limit field in the
//               vreg_ctrl register
//               00000 - 0.55V
//               00001 - 0.60V
//               00010 - 0.65V
//               00011 - 0.70V
//               00100 - 0.75V
//               00101 - 0.80V
//               00110 - 0.85V
//               00111 - 0.90V
//               01000 - 0.95V
//               01001 - 1.00V
//               01010 - 1.05V
//               01011 - 1.10V (default)
//               01100 - 1.15V
//               01101 - 1.20V
//               01110 - 1.25V
//               01111 - 1.30V
//               10000 - 1.35V
//               10001 - 1.40V
//               10010 - 1.50V
//               10011 - 1.60V
//               10100 - 1.65V
//               10101 - 1.70V
//               10110 - 1.80V
//               10111 - 1.90V
//               11000 - 2.00V
//               11001 - 2.35V
//               11010 - 2.50V
//               11011 - 2.65V
//               11100 - 2.80V
//               11101 - 3.00V
//               11110 - 3.15V
//               11111 - 3.30V
#define POWMAN_VREG_LP_EXIT_VSEL_RESET  _u(0x0b)
#define POWMAN_VREG_LP_EXIT_VSEL_BITS   _u(0x000001f0)
#define POWMAN_VREG_LP_EXIT_VSEL_MSB    _u(8)
#define POWMAN_VREG_LP_EXIT_VSEL_LSB    _u(4)
#define POWMAN_VREG_LP_EXIT_VSEL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_VREG_LP_EXIT_MODE
// Description : selects either normal (switching) mode or low power (linear)
//               mode
//               low power mode can only be selected for output voltages up to
//               1.3V
//               0 = normal mode (switching)
//               1 = low power mode (linear)
#define POWMAN_VREG_LP_EXIT_MODE_RESET  _u(0x0)
#define POWMAN_VREG_LP_EXIT_MODE_BITS   _u(0x00000004)
#define POWMAN_VREG_LP_EXIT_MODE_MSB    _u(2)
#define POWMAN_VREG_LP_EXIT_MODE_LSB    _u(2)
#define POWMAN_VREG_LP_EXIT_MODE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_VREG_LP_EXIT_HIZ
// Description : high impedance mode select
//               0=not in high impedance mode, 1=in high impedance mode
#define POWMAN_VREG_LP_EXIT_HIZ_RESET  _u(0x0)
#define POWMAN_VREG_LP_EXIT_HIZ_BITS   _u(0x00000002)
#define POWMAN_VREG_LP_EXIT_HIZ_MSB    _u(1)
#define POWMAN_VREG_LP_EXIT_HIZ_LSB    _u(1)
#define POWMAN_VREG_LP_EXIT_HIZ_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_BOD_CTRL
// Description : Brown-out Detection Control
#define POWMAN_BOD_CTRL_OFFSET _u(0x00000018)
#define POWMAN_BOD_CTRL_BITS   _u(0x00001000)
#define POWMAN_BOD_CTRL_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : POWMAN_BOD_CTRL_ISOLATE
// Description : isolates the brown-out detection control interface
//               0 - not isolated (default)
//               1 - isolated
#define POWMAN_BOD_CTRL_ISOLATE_RESET  _u(0x0)
#define POWMAN_BOD_CTRL_ISOLATE_BITS   _u(0x00001000)
#define POWMAN_BOD_CTRL_ISOLATE_MSB    _u(12)
#define POWMAN_BOD_CTRL_ISOLATE_LSB    _u(12)
#define POWMAN_BOD_CTRL_ISOLATE_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_BOD
// Description : Brown-out Detection Settings
#define POWMAN_BOD_OFFSET _u(0x0000001c)
#define POWMAN_BOD_BITS   _u(0x000001f1)
#define POWMAN_BOD_RESET  _u(0x000000b1)
// -----------------------------------------------------------------------------
// Field       : POWMAN_BOD_VSEL
// Description : threshold select
//               00000 - 0.473V
//               00001 - 0.516V
//               00010 - 0.559V
//               00011 - 0.602V
//               00100 - 0.645VS
//               00101 - 0.688V
//               00110 - 0.731V
//               00111 - 0.774V
//               01000 - 0.817V
//               01001 - 0.860V (default)
//               01010 - 0.903V
//               01011 - 0.946V
//               01100 - 0.989V
//               01101 - 1.032V
//               01110 - 1.075V
//               01111 - 1.118V
//               10000 - 1.161
//               10001 - 1.204V
#define POWMAN_BOD_VSEL_RESET  _u(0x0b)
#define POWMAN_BOD_VSEL_BITS   _u(0x000001f0)
#define POWMAN_BOD_VSEL_MSB    _u(8)
#define POWMAN_BOD_VSEL_LSB    _u(4)
#define POWMAN_BOD_VSEL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_BOD_EN
// Description : enable brown-out detection
//               0=not enabled, 1=enabled
#define POWMAN_BOD_EN_RESET  _u(0x1)
#define POWMAN_BOD_EN_BITS   _u(0x00000001)
#define POWMAN_BOD_EN_MSB    _u(0)
#define POWMAN_BOD_EN_LSB    _u(0)
#define POWMAN_BOD_EN_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_BOD_LP_ENTRY
// Description : Brown-out Detection Low Power Entry Settings
#define POWMAN_BOD_LP_ENTRY_OFFSET _u(0x00000020)
#define POWMAN_BOD_LP_ENTRY_BITS   _u(0x000001f1)
#define POWMAN_BOD_LP_ENTRY_RESET  _u(0x000000b0)
// -----------------------------------------------------------------------------
// Field       : POWMAN_BOD_LP_ENTRY_VSEL
// Description : threshold select
//               00000 - 0.473V
//               00001 - 0.516V
//               00010 - 0.559V
//               00011 - 0.602V
//               00100 - 0.645VS
//               00101 - 0.688V
//               00110 - 0.731V
//               00111 - 0.774V
//               01000 - 0.817V
//               01001 - 0.860V (default)
//               01010 - 0.903V
//               01011 - 0.946V
//               01100 - 0.989V
//               01101 - 1.032V
//               01110 - 1.075V
//               01111 - 1.118V
//               10000 - 1.161
//               10001 - 1.204V
#define POWMAN_BOD_LP_ENTRY_VSEL_RESET  _u(0x0b)
#define POWMAN_BOD_LP_ENTRY_VSEL_BITS   _u(0x000001f0)
#define POWMAN_BOD_LP_ENTRY_VSEL_MSB    _u(8)
#define POWMAN_BOD_LP_ENTRY_VSEL_LSB    _u(4)
#define POWMAN_BOD_LP_ENTRY_VSEL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_BOD_LP_ENTRY_EN
// Description : enable brown-out detection
//               0=not enabled, 1=enabled
#define POWMAN_BOD_LP_ENTRY_EN_RESET  _u(0x0)
#define POWMAN_BOD_LP_ENTRY_EN_BITS   _u(0x00000001)
#define POWMAN_BOD_LP_ENTRY_EN_MSB    _u(0)
#define POWMAN_BOD_LP_ENTRY_EN_LSB    _u(0)
#define POWMAN_BOD_LP_ENTRY_EN_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_BOD_LP_EXIT
// Description : Brown-out Detection Low Power Exit Settings
#define POWMAN_BOD_LP_EXIT_OFFSET _u(0x00000024)
#define POWMAN_BOD_LP_EXIT_BITS   _u(0x000001f1)
#define POWMAN_BOD_LP_EXIT_RESET  _u(0x000000b1)
// -----------------------------------------------------------------------------
// Field       : POWMAN_BOD_LP_EXIT_VSEL
// Description : threshold select
//               00000 - 0.473V
//               00001 - 0.516V
//               00010 - 0.559V
//               00011 - 0.602V
//               00100 - 0.645VS
//               00101 - 0.688V
//               00110 - 0.731V
//               00111 - 0.774V
//               01000 - 0.817V
//               01001 - 0.860V (default)
//               01010 - 0.903V
//               01011 - 0.946V
//               01100 - 0.989V
//               01101 - 1.032V
//               01110 - 1.075V
//               01111 - 1.118V
//               10000 - 1.161
//               10001 - 1.204V
#define POWMAN_BOD_LP_EXIT_VSEL_RESET  _u(0x0b)
#define POWMAN_BOD_LP_EXIT_VSEL_BITS   _u(0x000001f0)
#define POWMAN_BOD_LP_EXIT_VSEL_MSB    _u(8)
#define POWMAN_BOD_LP_EXIT_VSEL_LSB    _u(4)
#define POWMAN_BOD_LP_EXIT_VSEL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_BOD_LP_EXIT_EN
// Description : enable brown-out detection
//               0=not enabled, 1=enabled
#define POWMAN_BOD_LP_EXIT_EN_RESET  _u(0x1)
#define POWMAN_BOD_LP_EXIT_EN_BITS   _u(0x00000001)
#define POWMAN_BOD_LP_EXIT_EN_MSB    _u(0)
#define POWMAN_BOD_LP_EXIT_EN_LSB    _u(0)
#define POWMAN_BOD_LP_EXIT_EN_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_LPOSC
// Description : Low power oscillator control register.
#define POWMAN_LPOSC_OFFSET _u(0x00000028)
#define POWMAN_LPOSC_BITS   _u(0x000003f3)
#define POWMAN_LPOSC_RESET  _u(0x00000203)
// -----------------------------------------------------------------------------
// Field       : POWMAN_LPOSC_TRIM
// Description : Frequency trim - the trim step is typically 1% of the reset
//               frequency, but can be up to 3%
#define POWMAN_LPOSC_TRIM_RESET  _u(0x20)
#define POWMAN_LPOSC_TRIM_BITS   _u(0x000003f0)
#define POWMAN_LPOSC_TRIM_MSB    _u(9)
#define POWMAN_LPOSC_TRIM_LSB    _u(4)
#define POWMAN_LPOSC_TRIM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_LPOSC_MODE
// Description : This feature has been removed
#define POWMAN_LPOSC_MODE_RESET  _u(0x3)
#define POWMAN_LPOSC_MODE_BITS   _u(0x00000003)
#define POWMAN_LPOSC_MODE_MSB    _u(1)
#define POWMAN_LPOSC_MODE_LSB    _u(0)
#define POWMAN_LPOSC_MODE_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_CHIP_RESET
// Description : Chip reset control and status
#define POWMAN_CHIP_RESET_OFFSET _u(0x0000002c)
#define POWMAN_CHIP_RESET_BITS   _u(0x1fef0011)
#define POWMAN_CHIP_RESET_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_RSM
// Description : Last reset was a watchdog timeout which was configured to reset
//               the power-on state machine
//               This resets:
//               double_tap flag    no
//               DP                 no
//               RPAP               no
//               rescue_flag        no
//               timer              no
//               powman             no
//               swcore             no
//               psm                yes
//               and does not change the power state
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_RSM_RESET  _u(0x0)
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_RSM_BITS   _u(0x10000000)
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_RSM_MSB    _u(28)
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_RSM_LSB    _u(28)
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_RSM_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_CHIP_RESET_HAD_HZD_SYS_RESET_REQ
// Description : Last reset was a system reset from the hazard debugger
//               This resets:
//               double_tap flag    no
//               DP                 no
//               RPAP               no
//               rescue_flag        no
//               timer              no
//               powman             no
//               swcore             no
//               psm                yes
//               and does not change the power state
#define POWMAN_CHIP_RESET_HAD_HZD_SYS_RESET_REQ_RESET  _u(0x0)
#define POWMAN_CHIP_RESET_HAD_HZD_SYS_RESET_REQ_BITS   _u(0x08000000)
#define POWMAN_CHIP_RESET_HAD_HZD_SYS_RESET_REQ_MSB    _u(27)
#define POWMAN_CHIP_RESET_HAD_HZD_SYS_RESET_REQ_LSB    _u(27)
#define POWMAN_CHIP_RESET_HAD_HZD_SYS_RESET_REQ_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_CHIP_RESET_HAD_GLITCH_DETECT
// Description : Last reset was due to a power supply glitch
//               This resets:
//               double_tap flag    no
//               DP                 no
//               RPAP               no
//               rescue_flag        no
//               timer              no
//               powman             no
//               swcore             no
//               psm                yes
//               and does not change the power state
#define POWMAN_CHIP_RESET_HAD_GLITCH_DETECT_RESET  _u(0x0)
#define POWMAN_CHIP_RESET_HAD_GLITCH_DETECT_BITS   _u(0x04000000)
#define POWMAN_CHIP_RESET_HAD_GLITCH_DETECT_MSB    _u(26)
#define POWMAN_CHIP_RESET_HAD_GLITCH_DETECT_LSB    _u(26)
#define POWMAN_CHIP_RESET_HAD_GLITCH_DETECT_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_CHIP_RESET_HAD_SWCORE_PD
// Description : Last reset was a switched core powerdown
//               This resets:
//               double_tap flag    no
//               DP                 no
//               RPAP               no
//               rescue_flag        no
//               timer              no
//               powman             no
//               swcore             yes
//               psm                yes
//               then starts the power sequencer
#define POWMAN_CHIP_RESET_HAD_SWCORE_PD_RESET  _u(0x0)
#define POWMAN_CHIP_RESET_HAD_SWCORE_PD_BITS   _u(0x02000000)
#define POWMAN_CHIP_RESET_HAD_SWCORE_PD_MSB    _u(25)
#define POWMAN_CHIP_RESET_HAD_SWCORE_PD_LSB    _u(25)
#define POWMAN_CHIP_RESET_HAD_SWCORE_PD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_SWCORE
// Description : Last reset was a watchdog timeout which was configured to reset
//               the switched-core
//               This resets:
//               double_tap flag    no
//               DP                 no
//               RPAP               no
//               rescue_flag        no
//               timer              no
//               powman             no
//               swcore             yes
//               psm                yes
//               then starts the power sequencer
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_SWCORE_RESET  _u(0x0)
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_SWCORE_BITS   _u(0x01000000)
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_SWCORE_MSB    _u(24)
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_SWCORE_LSB    _u(24)
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_SWCORE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_POWMAN
// Description : Last reset was a watchdog timeout which was configured to reset
//               the power manager
//               This resets:
//               double_tap flag    no
//               DP                 no
//               RPAP               no
//               rescue_flag        no
//               timer              yes
//               powman             yes
//               swcore             yes
//               psm                yes
//               then starts the power sequencer
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_POWMAN_RESET  _u(0x0)
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_POWMAN_BITS   _u(0x00800000)
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_POWMAN_MSB    _u(23)
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_POWMAN_LSB    _u(23)
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_POWMAN_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_POWMAN_ASYNC
// Description : Last reset was a watchdog timeout which was configured to reset
//               the power manager asynchronously
//               This resets:
//               double_tap flag    no
//               DP                 no
//               RPAP               no
//               rescue_flag        no
//               timer              yes
//               powman             yes
//               swcore             yes
//               psm                yes
//               then starts the power sequencer
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_POWMAN_ASYNC_RESET  _u(0x0)
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_POWMAN_ASYNC_BITS   _u(0x00400000)
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_POWMAN_ASYNC_MSB    _u(22)
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_POWMAN_ASYNC_LSB    _u(22)
#define POWMAN_CHIP_RESET_HAD_WATCHDOG_RESET_POWMAN_ASYNC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_CHIP_RESET_HAD_RESCUE
// Description : Last reset was a rescue reset from the debugger
//               This resets:
//               double_tap flag    no
//               DP                 no
//               RPAP               no
//               rescue_flag        no, it sets this flag
//               timer              yes
//               powman             yes
//               swcore             yes
//               psm                yes
//               then starts the power sequencer
#define POWMAN_CHIP_RESET_HAD_RESCUE_RESET  _u(0x0)
#define POWMAN_CHIP_RESET_HAD_RESCUE_BITS   _u(0x00200000)
#define POWMAN_CHIP_RESET_HAD_RESCUE_MSB    _u(21)
#define POWMAN_CHIP_RESET_HAD_RESCUE_LSB    _u(21)
#define POWMAN_CHIP_RESET_HAD_RESCUE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_CHIP_RESET_HAD_DP_RESET_REQ
// Description : Last reset was an reset request from the arm debugger
//               This resets:
//               double_tap flag    no
//               DP                 no
//               RPAP               no
//               rescue_flag        yes
//               timer              yes
//               powman             yes
//               swcore             yes
//               psm                yes
//               then starts the power sequencer
#define POWMAN_CHIP_RESET_HAD_DP_RESET_REQ_RESET  _u(0x0)
#define POWMAN_CHIP_RESET_HAD_DP_RESET_REQ_BITS   _u(0x00080000)
#define POWMAN_CHIP_RESET_HAD_DP_RESET_REQ_MSB    _u(19)
#define POWMAN_CHIP_RESET_HAD_DP_RESET_REQ_LSB    _u(19)
#define POWMAN_CHIP_RESET_HAD_DP_RESET_REQ_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_CHIP_RESET_HAD_RUN_LOW
// Description : Last reset was from the RUN pin
//               This resets:
//               double_tap flag    no
//               DP                 yes
//               RPAP               yes
//               rescue_flag        yes
//               timer              yes
//               powman             yes
//               swcore             yes
//               psm                yes
//               then starts the power sequencer
#define POWMAN_CHIP_RESET_HAD_RUN_LOW_RESET  _u(0x0)
#define POWMAN_CHIP_RESET_HAD_RUN_LOW_BITS   _u(0x00040000)
#define POWMAN_CHIP_RESET_HAD_RUN_LOW_MSB    _u(18)
#define POWMAN_CHIP_RESET_HAD_RUN_LOW_LSB    _u(18)
#define POWMAN_CHIP_RESET_HAD_RUN_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_CHIP_RESET_HAD_BOR
// Description : Last reset was from the brown-out detection block
//               This resets:
//               double_tap flag    yes
//               DP                 yes
//               RPAP               yes
//               rescue_flag        yes
//               timer              yes
//               powman             yes
//               swcore             yes
//               psm                yes
//               then starts the power sequencer
#define POWMAN_CHIP_RESET_HAD_BOR_RESET  _u(0x0)
#define POWMAN_CHIP_RESET_HAD_BOR_BITS   _u(0x00020000)
#define POWMAN_CHIP_RESET_HAD_BOR_MSB    _u(17)
#define POWMAN_CHIP_RESET_HAD_BOR_LSB    _u(17)
#define POWMAN_CHIP_RESET_HAD_BOR_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_CHIP_RESET_HAD_POR
// Description : Last reset was from the power-on reset
//               This resets:
//               double_tap flag    yes
//               DP                 yes
//               RPAP               yes
//               rescue_flag        yes
//               timer              yes
//               powman             yes
//               swcore             yes
//               psm                yes
//               then starts the power sequencer
#define POWMAN_CHIP_RESET_HAD_POR_RESET  _u(0x0)
#define POWMAN_CHIP_RESET_HAD_POR_BITS   _u(0x00010000)
#define POWMAN_CHIP_RESET_HAD_POR_MSB    _u(16)
#define POWMAN_CHIP_RESET_HAD_POR_LSB    _u(16)
#define POWMAN_CHIP_RESET_HAD_POR_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_CHIP_RESET_RESCUE_FLAG
// Description : This is set by a rescue reset from the RP-AP.
//               Its purpose is to halt before the bootrom before booting from
//               flash in order to recover from a boot lock-up.
//               The debugger can then attach once the bootrom has been halted
//               and flash some working code that does not lock up.
#define POWMAN_CHIP_RESET_RESCUE_FLAG_RESET  _u(0x0)
#define POWMAN_CHIP_RESET_RESCUE_FLAG_BITS   _u(0x00000010)
#define POWMAN_CHIP_RESET_RESCUE_FLAG_MSB    _u(4)
#define POWMAN_CHIP_RESET_RESCUE_FLAG_LSB    _u(4)
#define POWMAN_CHIP_RESET_RESCUE_FLAG_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : POWMAN_CHIP_RESET_DOUBLE_TAP
// Description : This flag is set by double-tapping RUN. It tells bootcode to go
//               into the bootloader.
#define POWMAN_CHIP_RESET_DOUBLE_TAP_RESET  _u(0x0)
#define POWMAN_CHIP_RESET_DOUBLE_TAP_BITS   _u(0x00000001)
#define POWMAN_CHIP_RESET_DOUBLE_TAP_MSB    _u(0)
#define POWMAN_CHIP_RESET_DOUBLE_TAP_LSB    _u(0)
#define POWMAN_CHIP_RESET_DOUBLE_TAP_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_WDSEL
// Description : Allows a watchdog reset to reset the internal state of powman
//               in addition to the power-on state machine (PSM).
//               Note that powman ignores watchdog resets that do not select at
//               least the CLOCKS stage or earlier stages in the PSM. If using
//               these bits, it's recommended to set PSM_WDSEL to all-ones in
//               addition to the desired bits in this register. Failing to
//               select CLOCKS or earlier will result in the POWMAN_WDSEL
//               register having no effect.
#define POWMAN_WDSEL_OFFSET _u(0x00000030)
#define POWMAN_WDSEL_BITS   _u(0x00001111)
#define POWMAN_WDSEL_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : POWMAN_WDSEL_RESET_RSM
// Description : If set to 1, a watchdog reset will run the full power-on state
//               machine (PSM) sequence
//               From a user perspective it is the same as setting
//               RSM_WDSEL_PROC_COLD
//               From a hardware debug perspective it has the same effect as a
//               reset from a glitch detector
#define POWMAN_WDSEL_RESET_RSM_RESET  _u(0x0)
#define POWMAN_WDSEL_RESET_RSM_BITS   _u(0x00001000)
#define POWMAN_WDSEL_RESET_RSM_MSB    _u(12)
#define POWMAN_WDSEL_RESET_RSM_LSB    _u(12)
#define POWMAN_WDSEL_RESET_RSM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_WDSEL_RESET_SWCORE
// Description : If set to 1, a watchdog reset will reset the switched core
//               power domain and run the full power-on state machine (PSM)
//               sequence
//               From a user perspective it is the same as setting
//               RSM_WDSEL_PROC_COLD
//               From a hardware debug perspective it has the same effect as a
//               power-on reset for the switched core power domain
#define POWMAN_WDSEL_RESET_SWCORE_RESET  _u(0x0)
#define POWMAN_WDSEL_RESET_SWCORE_BITS   _u(0x00000100)
#define POWMAN_WDSEL_RESET_SWCORE_MSB    _u(8)
#define POWMAN_WDSEL_RESET_SWCORE_LSB    _u(8)
#define POWMAN_WDSEL_RESET_SWCORE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_WDSEL_RESET_POWMAN
// Description : If set to 1, a watchdog reset will restore powman defaults,
//               reset the timer, reset the switched core power domain
//               and run the full power-on state machine (PSM) sequence
//               This relies on clk_ref running. Use reset_powman_async if that
//               may not be true
#define POWMAN_WDSEL_RESET_POWMAN_RESET  _u(0x0)
#define POWMAN_WDSEL_RESET_POWMAN_BITS   _u(0x00000010)
#define POWMAN_WDSEL_RESET_POWMAN_MSB    _u(4)
#define POWMAN_WDSEL_RESET_POWMAN_LSB    _u(4)
#define POWMAN_WDSEL_RESET_POWMAN_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_WDSEL_RESET_POWMAN_ASYNC
// Description : If set to 1, a watchdog reset will restore powman defaults,
//               reset the timer,
//               reset the switched core domain and run the full power-on state
//               machine (PSM) sequence
//               This does not rely on clk_ref running
#define POWMAN_WDSEL_RESET_POWMAN_ASYNC_RESET  _u(0x0)
#define POWMAN_WDSEL_RESET_POWMAN_ASYNC_BITS   _u(0x00000001)
#define POWMAN_WDSEL_RESET_POWMAN_ASYNC_MSB    _u(0)
#define POWMAN_WDSEL_RESET_POWMAN_ASYNC_LSB    _u(0)
#define POWMAN_WDSEL_RESET_POWMAN_ASYNC_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_SEQ_CFG
// Description : For configuration of the power sequencer
//               Writes are ignored while POWMAN_STATE_CHANGING=1
#define POWMAN_SEQ_CFG_OFFSET _u(0x00000034)
#define POWMAN_SEQ_CFG_BITS   _u(0x001311f3)
#define POWMAN_SEQ_CFG_RESET  _u(0x001011f0)
// -----------------------------------------------------------------------------
// Field       : POWMAN_SEQ_CFG_USING_FAST_POWCK
// Description : 0 indicates the POWMAN clock is running from the low power
//               oscillator (32kHz)
//               1 indicates the POWMAN clock is running from the reference
//               clock (2-50MHz)
#define POWMAN_SEQ_CFG_USING_FAST_POWCK_RESET  _u(0x1)
#define POWMAN_SEQ_CFG_USING_FAST_POWCK_BITS   _u(0x00100000)
#define POWMAN_SEQ_CFG_USING_FAST_POWCK_MSB    _u(20)
#define POWMAN_SEQ_CFG_USING_FAST_POWCK_LSB    _u(20)
#define POWMAN_SEQ_CFG_USING_FAST_POWCK_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_SEQ_CFG_USING_BOD_LP
// Description : Indicates the brown-out detector (BOD) mode
//               0 = BOD high power mode which is the default
//               1 = BOD low power mode
#define POWMAN_SEQ_CFG_USING_BOD_LP_RESET  _u(0x0)
#define POWMAN_SEQ_CFG_USING_BOD_LP_BITS   _u(0x00020000)
#define POWMAN_SEQ_CFG_USING_BOD_LP_MSB    _u(17)
#define POWMAN_SEQ_CFG_USING_BOD_LP_LSB    _u(17)
#define POWMAN_SEQ_CFG_USING_BOD_LP_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_SEQ_CFG_USING_VREG_LP
// Description : Indicates the voltage regulator (VREG) mode
//               0 = VREG high power mode which is the default
//               1 = VREG low power mode
#define POWMAN_SEQ_CFG_USING_VREG_LP_RESET  _u(0x0)
#define POWMAN_SEQ_CFG_USING_VREG_LP_BITS   _u(0x00010000)
#define POWMAN_SEQ_CFG_USING_VREG_LP_MSB    _u(16)
#define POWMAN_SEQ_CFG_USING_VREG_LP_LSB    _u(16)
#define POWMAN_SEQ_CFG_USING_VREG_LP_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_SEQ_CFG_USE_FAST_POWCK
// Description : selects the reference clock (clk_ref) as the source of the
//               POWMAN clock when switched-core is powered. The POWMAN clock
//               always switches to the slow clock (lposc) when switched-core is
//               powered down because the fast clock stops running.
//               0 always run the POWMAN clock from the slow clock (lposc)
//               1 run the POWMAN clock from the fast clock when available
//               This setting takes effect when a power up sequence is next run
#define POWMAN_SEQ_CFG_USE_FAST_POWCK_RESET  _u(0x1)
#define POWMAN_SEQ_CFG_USE_FAST_POWCK_BITS   _u(0x00001000)
#define POWMAN_SEQ_CFG_USE_FAST_POWCK_MSB    _u(12)
#define POWMAN_SEQ_CFG_USE_FAST_POWCK_LSB    _u(12)
#define POWMAN_SEQ_CFG_USE_FAST_POWCK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_SEQ_CFG_RUN_LPOSC_IN_LP
// Description : Set to 0 to stop the low power osc when the switched-core is
//               powered down, which is unwise if using it to clock the timer
//               This setting takes effect when the swcore is next powered down
#define POWMAN_SEQ_CFG_RUN_LPOSC_IN_LP_RESET  _u(0x1)
#define POWMAN_SEQ_CFG_RUN_LPOSC_IN_LP_BITS   _u(0x00000100)
#define POWMAN_SEQ_CFG_RUN_LPOSC_IN_LP_MSB    _u(8)
#define POWMAN_SEQ_CFG_RUN_LPOSC_IN_LP_LSB    _u(8)
#define POWMAN_SEQ_CFG_RUN_LPOSC_IN_LP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_SEQ_CFG_USE_BOD_HP
// Description : Set to 0 to prevent automatic switching to bod high power mode
//               when switched-core is powered up
//               This setting takes effect when the swcore is next powered up
#define POWMAN_SEQ_CFG_USE_BOD_HP_RESET  _u(0x1)
#define POWMAN_SEQ_CFG_USE_BOD_HP_BITS   _u(0x00000080)
#define POWMAN_SEQ_CFG_USE_BOD_HP_MSB    _u(7)
#define POWMAN_SEQ_CFG_USE_BOD_HP_LSB    _u(7)
#define POWMAN_SEQ_CFG_USE_BOD_HP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_SEQ_CFG_USE_BOD_LP
// Description : Set to 0 to prevent automatic switching to bod low power mode
//               when switched-core is powered down
//               This setting takes effect when the swcore is next powered down
#define POWMAN_SEQ_CFG_USE_BOD_LP_RESET  _u(0x1)
#define POWMAN_SEQ_CFG_USE_BOD_LP_BITS   _u(0x00000040)
#define POWMAN_SEQ_CFG_USE_BOD_LP_MSB    _u(6)
#define POWMAN_SEQ_CFG_USE_BOD_LP_LSB    _u(6)
#define POWMAN_SEQ_CFG_USE_BOD_LP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_SEQ_CFG_USE_VREG_HP
// Description : Set to 0 to prevent automatic switching to vreg high power mode
//               when switched-core is powered up
//               This setting takes effect when the swcore is next powered up
#define POWMAN_SEQ_CFG_USE_VREG_HP_RESET  _u(0x1)
#define POWMAN_SEQ_CFG_USE_VREG_HP_BITS   _u(0x00000020)
#define POWMAN_SEQ_CFG_USE_VREG_HP_MSB    _u(5)
#define POWMAN_SEQ_CFG_USE_VREG_HP_LSB    _u(5)
#define POWMAN_SEQ_CFG_USE_VREG_HP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_SEQ_CFG_USE_VREG_LP
// Description : Set to 0 to prevent automatic switching to vreg low power mode
//               when switched-core is powered down
//               This setting takes effect when the swcore is next powered down
#define POWMAN_SEQ_CFG_USE_VREG_LP_RESET  _u(0x1)
#define POWMAN_SEQ_CFG_USE_VREG_LP_BITS   _u(0x00000010)
#define POWMAN_SEQ_CFG_USE_VREG_LP_MSB    _u(4)
#define POWMAN_SEQ_CFG_USE_VREG_LP_LSB    _u(4)
#define POWMAN_SEQ_CFG_USE_VREG_LP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_SEQ_CFG_HW_PWRUP_SRAM0
// Description : Specifies the power state of SRAM0 when powering up swcore from
//               a low power state (P1.xxx) to a high power state (P0.0xx).
//               0=power-up
//               1=no change
#define POWMAN_SEQ_CFG_HW_PWRUP_SRAM0_RESET  _u(0x0)
#define POWMAN_SEQ_CFG_HW_PWRUP_SRAM0_BITS   _u(0x00000002)
#define POWMAN_SEQ_CFG_HW_PWRUP_SRAM0_MSB    _u(1)
#define POWMAN_SEQ_CFG_HW_PWRUP_SRAM0_LSB    _u(1)
#define POWMAN_SEQ_CFG_HW_PWRUP_SRAM0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_SEQ_CFG_HW_PWRUP_SRAM1
// Description : Specifies the power state of SRAM1 when powering up swcore from
//               a low power state (P1.xxx) to a high power state (P0.0xx).
//               0=power-up
//               1=no change
#define POWMAN_SEQ_CFG_HW_PWRUP_SRAM1_RESET  _u(0x0)
#define POWMAN_SEQ_CFG_HW_PWRUP_SRAM1_BITS   _u(0x00000001)
#define POWMAN_SEQ_CFG_HW_PWRUP_SRAM1_MSB    _u(0)
#define POWMAN_SEQ_CFG_HW_PWRUP_SRAM1_LSB    _u(0)
#define POWMAN_SEQ_CFG_HW_PWRUP_SRAM1_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_STATE
// Description : This register controls the power state of the 4 power domains.
//               The current power state is indicated in POWMAN_STATE_CURRENT
//               which is read-only.
//               To change the state, write to POWMAN_STATE_REQ.
//               The coding of POWMAN_STATE_CURRENT & POWMAN_STATE_REQ
//               corresponds to the power states
//               defined in the datasheet:
//               bit 3 = SWCORE
//               bit 2 = XIP cache
//               bit 1 = SRAM0
//               bit 0 = SRAM1
//               0 = powered up
//               1 = powered down
//               When POWMAN_STATE_REQ is written, the POWMAN_STATE_WAITING flag
//               is set while the Power Manager determines what is required. If
//               an invalid transition is requested the Power Manager will still
//               register the request in POWMAN_STATE_REQ but will also set the
//               POWMAN_BAD_REQ flag. It will then implement the power-up
//               requests and ignore the power down requests. To do nothing
//               would risk entering an unrecoverable lock-up state. Invalid
//               requests are: any combination of power up and power down
//               requests any request that results in swcore boing powered and
//               xip unpowered If the request is to power down the switched-core
//               domain then POWMAN_STATE_WAITING stays active until the
//               processors halt. During this time the POWMAN_STATE_REQ field
//               can be re-written to change or cancel the request. When the
//               power state transition begins the POWMAN_STATE_WAITING_flag is
//               cleared, the POWMAN_STATE_CHANGING flag is set and POWMAN
//               register writes are ignored until the transition completes.
#define POWMAN_STATE_OFFSET _u(0x00000038)
#define POWMAN_STATE_BITS   _u(0x00003fff)
#define POWMAN_STATE_RESET  _u(0x0000000f)
// -----------------------------------------------------------------------------
// Field       : POWMAN_STATE_CHANGING
#define POWMAN_STATE_CHANGING_RESET  _u(0x0)
#define POWMAN_STATE_CHANGING_BITS   _u(0x00002000)
#define POWMAN_STATE_CHANGING_MSB    _u(13)
#define POWMAN_STATE_CHANGING_LSB    _u(13)
#define POWMAN_STATE_CHANGING_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_STATE_WAITING
#define POWMAN_STATE_WAITING_RESET  _u(0x0)
#define POWMAN_STATE_WAITING_BITS   _u(0x00001000)
#define POWMAN_STATE_WAITING_MSB    _u(12)
#define POWMAN_STATE_WAITING_LSB    _u(12)
#define POWMAN_STATE_WAITING_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_STATE_BAD_HW_REQ
// Description : Bad hardware initiated state request. Went back to state 0
//               (i.e. everything powered up)
#define POWMAN_STATE_BAD_HW_REQ_RESET  _u(0x0)
#define POWMAN_STATE_BAD_HW_REQ_BITS   _u(0x00000800)
#define POWMAN_STATE_BAD_HW_REQ_MSB    _u(11)
#define POWMAN_STATE_BAD_HW_REQ_LSB    _u(11)
#define POWMAN_STATE_BAD_HW_REQ_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_STATE_BAD_SW_REQ
// Description : Bad software initiated state request. No action taken.
#define POWMAN_STATE_BAD_SW_REQ_RESET  _u(0x0)
#define POWMAN_STATE_BAD_SW_REQ_BITS   _u(0x00000400)
#define POWMAN_STATE_BAD_SW_REQ_MSB    _u(10)
#define POWMAN_STATE_BAD_SW_REQ_LSB    _u(10)
#define POWMAN_STATE_BAD_SW_REQ_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_STATE_PWRUP_WHILE_WAITING
// Description : Request ignored because of a pending pwrup request. See
//               current_pwrup_req. Note this blocks powering up AND powering
//               down.
#define POWMAN_STATE_PWRUP_WHILE_WAITING_RESET  _u(0x0)
#define POWMAN_STATE_PWRUP_WHILE_WAITING_BITS   _u(0x00000200)
#define POWMAN_STATE_PWRUP_WHILE_WAITING_MSB    _u(9)
#define POWMAN_STATE_PWRUP_WHILE_WAITING_LSB    _u(9)
#define POWMAN_STATE_PWRUP_WHILE_WAITING_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : POWMAN_STATE_REQ_IGNORED
#define POWMAN_STATE_REQ_IGNORED_RESET  _u(0x0)
#define POWMAN_STATE_REQ_IGNORED_BITS   _u(0x00000100)
#define POWMAN_STATE_REQ_IGNORED_MSB    _u(8)
#define POWMAN_STATE_REQ_IGNORED_LSB    _u(8)
#define POWMAN_STATE_REQ_IGNORED_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : POWMAN_STATE_REQ
#define POWMAN_STATE_REQ_RESET  _u(0x0)
#define POWMAN_STATE_REQ_BITS   _u(0x000000f0)
#define POWMAN_STATE_REQ_MSB    _u(7)
#define POWMAN_STATE_REQ_LSB    _u(4)
#define POWMAN_STATE_REQ_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_STATE_CURRENT
#define POWMAN_STATE_CURRENT_RESET  _u(0xf)
#define POWMAN_STATE_CURRENT_BITS   _u(0x0000000f)
#define POWMAN_STATE_CURRENT_MSB    _u(3)
#define POWMAN_STATE_CURRENT_LSB    _u(0)
#define POWMAN_STATE_CURRENT_ACCESS "RO"
// =============================================================================
// Register    : POWMAN_POW_FASTDIV
// Description : None
//               divides the POWMAN clock to provide a tick for the delay module
//               and state machines
//               when clk_pow is running from the slow clock it is not divided
//               when clk_pow is running from the fast clock it is divided by
//               tick_div
#define POWMAN_POW_FASTDIV_OFFSET _u(0x0000003c)
#define POWMAN_POW_FASTDIV_BITS   _u(0x000007ff)
#define POWMAN_POW_FASTDIV_RESET  _u(0x00000040)
#define POWMAN_POW_FASTDIV_MSB    _u(10)
#define POWMAN_POW_FASTDIV_LSB    _u(0)
#define POWMAN_POW_FASTDIV_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_POW_DELAY
// Description : power state machine delays
#define POWMAN_POW_DELAY_OFFSET _u(0x00000040)
#define POWMAN_POW_DELAY_BITS   _u(0x0000ffff)
#define POWMAN_POW_DELAY_RESET  _u(0x00002011)
// -----------------------------------------------------------------------------
// Field       : POWMAN_POW_DELAY_SRAM_STEP
// Description : timing between the sram0 and sram1 power state machine steps
//               measured in units of the powman tick period (>=1us), 0 gives a
//               delay of 1 unit
#define POWMAN_POW_DELAY_SRAM_STEP_RESET  _u(0x20)
#define POWMAN_POW_DELAY_SRAM_STEP_BITS   _u(0x0000ff00)
#define POWMAN_POW_DELAY_SRAM_STEP_MSB    _u(15)
#define POWMAN_POW_DELAY_SRAM_STEP_LSB    _u(8)
#define POWMAN_POW_DELAY_SRAM_STEP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_POW_DELAY_XIP_STEP
// Description : timing between the xip power state machine steps
//               measured in units of the lposc period, 0 gives a delay of 1
//               unit
#define POWMAN_POW_DELAY_XIP_STEP_RESET  _u(0x1)
#define POWMAN_POW_DELAY_XIP_STEP_BITS   _u(0x000000f0)
#define POWMAN_POW_DELAY_XIP_STEP_MSB    _u(7)
#define POWMAN_POW_DELAY_XIP_STEP_LSB    _u(4)
#define POWMAN_POW_DELAY_XIP_STEP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_POW_DELAY_SWCORE_STEP
// Description : timing between the swcore power state machine steps
//               measured in units of the lposc period, 0 gives a delay of 1
//               unit
#define POWMAN_POW_DELAY_SWCORE_STEP_RESET  _u(0x1)
#define POWMAN_POW_DELAY_SWCORE_STEP_BITS   _u(0x0000000f)
#define POWMAN_POW_DELAY_SWCORE_STEP_MSB    _u(3)
#define POWMAN_POW_DELAY_SWCORE_STEP_LSB    _u(0)
#define POWMAN_POW_DELAY_SWCORE_STEP_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_EXT_CTRL0
// Description : Configures a gpio as a power mode aware control output
#define POWMAN_EXT_CTRL0_OFFSET _u(0x00000044)
#define POWMAN_EXT_CTRL0_BITS   _u(0x0000713f)
#define POWMAN_EXT_CTRL0_RESET  _u(0x0000003f)
// -----------------------------------------------------------------------------
// Field       : POWMAN_EXT_CTRL0_LP_EXIT_STATE
// Description : output level when exiting the low power state
#define POWMAN_EXT_CTRL0_LP_EXIT_STATE_RESET  _u(0x0)
#define POWMAN_EXT_CTRL0_LP_EXIT_STATE_BITS   _u(0x00004000)
#define POWMAN_EXT_CTRL0_LP_EXIT_STATE_MSB    _u(14)
#define POWMAN_EXT_CTRL0_LP_EXIT_STATE_LSB    _u(14)
#define POWMAN_EXT_CTRL0_LP_EXIT_STATE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_EXT_CTRL0_LP_ENTRY_STATE
// Description : output level when entering the low power state
#define POWMAN_EXT_CTRL0_LP_ENTRY_STATE_RESET  _u(0x0)
#define POWMAN_EXT_CTRL0_LP_ENTRY_STATE_BITS   _u(0x00002000)
#define POWMAN_EXT_CTRL0_LP_ENTRY_STATE_MSB    _u(13)
#define POWMAN_EXT_CTRL0_LP_ENTRY_STATE_LSB    _u(13)
#define POWMAN_EXT_CTRL0_LP_ENTRY_STATE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_EXT_CTRL0_INIT_STATE
#define POWMAN_EXT_CTRL0_INIT_STATE_RESET  _u(0x0)
#define POWMAN_EXT_CTRL0_INIT_STATE_BITS   _u(0x00001000)
#define POWMAN_EXT_CTRL0_INIT_STATE_MSB    _u(12)
#define POWMAN_EXT_CTRL0_INIT_STATE_LSB    _u(12)
#define POWMAN_EXT_CTRL0_INIT_STATE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_EXT_CTRL0_INIT
#define POWMAN_EXT_CTRL0_INIT_RESET  _u(0x0)
#define POWMAN_EXT_CTRL0_INIT_BITS   _u(0x00000100)
#define POWMAN_EXT_CTRL0_INIT_MSB    _u(8)
#define POWMAN_EXT_CTRL0_INIT_LSB    _u(8)
#define POWMAN_EXT_CTRL0_INIT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_EXT_CTRL0_GPIO_SELECT
// Description : selects from gpio 0->30
//               set to 31 to disable this feature
#define POWMAN_EXT_CTRL0_GPIO_SELECT_RESET  _u(0x3f)
#define POWMAN_EXT_CTRL0_GPIO_SELECT_BITS   _u(0x0000003f)
#define POWMAN_EXT_CTRL0_GPIO_SELECT_MSB    _u(5)
#define POWMAN_EXT_CTRL0_GPIO_SELECT_LSB    _u(0)
#define POWMAN_EXT_CTRL0_GPIO_SELECT_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_EXT_CTRL1
// Description : Configures a gpio as a power mode aware control output
#define POWMAN_EXT_CTRL1_OFFSET _u(0x00000048)
#define POWMAN_EXT_CTRL1_BITS   _u(0x0000713f)
#define POWMAN_EXT_CTRL1_RESET  _u(0x0000003f)
// -----------------------------------------------------------------------------
// Field       : POWMAN_EXT_CTRL1_LP_EXIT_STATE
// Description : output level when exiting the low power state
#define POWMAN_EXT_CTRL1_LP_EXIT_STATE_RESET  _u(0x0)
#define POWMAN_EXT_CTRL1_LP_EXIT_STATE_BITS   _u(0x00004000)
#define POWMAN_EXT_CTRL1_LP_EXIT_STATE_MSB    _u(14)
#define POWMAN_EXT_CTRL1_LP_EXIT_STATE_LSB    _u(14)
#define POWMAN_EXT_CTRL1_LP_EXIT_STATE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_EXT_CTRL1_LP_ENTRY_STATE
// Description : output level when entering the low power state
#define POWMAN_EXT_CTRL1_LP_ENTRY_STATE_RESET  _u(0x0)
#define POWMAN_EXT_CTRL1_LP_ENTRY_STATE_BITS   _u(0x00002000)
#define POWMAN_EXT_CTRL1_LP_ENTRY_STATE_MSB    _u(13)
#define POWMAN_EXT_CTRL1_LP_ENTRY_STATE_LSB    _u(13)
#define POWMAN_EXT_CTRL1_LP_ENTRY_STATE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_EXT_CTRL1_INIT_STATE
#define POWMAN_EXT_CTRL1_INIT_STATE_RESET  _u(0x0)
#define POWMAN_EXT_CTRL1_INIT_STATE_BITS   _u(0x00001000)
#define POWMAN_EXT_CTRL1_INIT_STATE_MSB    _u(12)
#define POWMAN_EXT_CTRL1_INIT_STATE_LSB    _u(12)
#define POWMAN_EXT_CTRL1_INIT_STATE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_EXT_CTRL1_INIT
#define POWMAN_EXT_CTRL1_INIT_RESET  _u(0x0)
#define POWMAN_EXT_CTRL1_INIT_BITS   _u(0x00000100)
#define POWMAN_EXT_CTRL1_INIT_MSB    _u(8)
#define POWMAN_EXT_CTRL1_INIT_LSB    _u(8)
#define POWMAN_EXT_CTRL1_INIT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_EXT_CTRL1_GPIO_SELECT
// Description : selects from gpio 0->30
//               set to 31 to disable this feature
#define POWMAN_EXT_CTRL1_GPIO_SELECT_RESET  _u(0x3f)
#define POWMAN_EXT_CTRL1_GPIO_SELECT_BITS   _u(0x0000003f)
#define POWMAN_EXT_CTRL1_GPIO_SELECT_MSB    _u(5)
#define POWMAN_EXT_CTRL1_GPIO_SELECT_LSB    _u(0)
#define POWMAN_EXT_CTRL1_GPIO_SELECT_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_EXT_TIME_REF
// Description : Select a GPIO to use as a time reference, the source can be
//               used to drive the low power clock at 32kHz, or to provide a 1ms
//               tick to the timer, or provide a 1Hz tick to the timer. The tick
//               selection is controlled by the POWMAN_TIMER register.
#define POWMAN_EXT_TIME_REF_OFFSET _u(0x0000004c)
#define POWMAN_EXT_TIME_REF_BITS   _u(0x00000013)
#define POWMAN_EXT_TIME_REF_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : POWMAN_EXT_TIME_REF_DRIVE_LPCK
// Description : Use the selected GPIO to drive the 32kHz low power clock, in
//               place of LPOSC. This field must only be written when
//               POWMAN_TIMER_RUN=0
#define POWMAN_EXT_TIME_REF_DRIVE_LPCK_RESET  _u(0x0)
#define POWMAN_EXT_TIME_REF_DRIVE_LPCK_BITS   _u(0x00000010)
#define POWMAN_EXT_TIME_REF_DRIVE_LPCK_MSB    _u(4)
#define POWMAN_EXT_TIME_REF_DRIVE_LPCK_LSB    _u(4)
#define POWMAN_EXT_TIME_REF_DRIVE_LPCK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_EXT_TIME_REF_SOURCE_SEL
// Description : 0 ->  gpio12
//               1 ->  gpio20
//               2 ->  gpio14
//               3 ->  gpio22
#define POWMAN_EXT_TIME_REF_SOURCE_SEL_RESET  _u(0x0)
#define POWMAN_EXT_TIME_REF_SOURCE_SEL_BITS   _u(0x00000003)
#define POWMAN_EXT_TIME_REF_SOURCE_SEL_MSB    _u(1)
#define POWMAN_EXT_TIME_REF_SOURCE_SEL_LSB    _u(0)
#define POWMAN_EXT_TIME_REF_SOURCE_SEL_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_LPOSC_FREQ_KHZ_INT
// Description : Informs the AON Timer of the integer component of the clock
//               frequency when running off the LPOSC.
//               Integer component of the LPOSC or GPIO clock source frequency
//               in kHz. Default = 32 This field must only be written when
//               POWMAN_TIMER_RUN=0 or POWMAN_TIMER_USING_XOSC=1
#define POWMAN_LPOSC_FREQ_KHZ_INT_OFFSET _u(0x00000050)
#define POWMAN_LPOSC_FREQ_KHZ_INT_BITS   _u(0x0000003f)
#define POWMAN_LPOSC_FREQ_KHZ_INT_RESET  _u(0x00000020)
#define POWMAN_LPOSC_FREQ_KHZ_INT_MSB    _u(5)
#define POWMAN_LPOSC_FREQ_KHZ_INT_LSB    _u(0)
#define POWMAN_LPOSC_FREQ_KHZ_INT_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_LPOSC_FREQ_KHZ_FRAC
// Description : Informs the AON Timer of the fractional component of the clock
//               frequency when running off the LPOSC.
//               Fractional component of the LPOSC or GPIO clock source
//               frequency in kHz. Default = 0.768 This field must only be
//               written when POWMAN_TIMER_RUN=0 or POWMAN_TIMER_USING_XOSC=1
#define POWMAN_LPOSC_FREQ_KHZ_FRAC_OFFSET _u(0x00000054)
#define POWMAN_LPOSC_FREQ_KHZ_FRAC_BITS   _u(0x0000ffff)
#define POWMAN_LPOSC_FREQ_KHZ_FRAC_RESET  _u(0x0000c49c)
#define POWMAN_LPOSC_FREQ_KHZ_FRAC_MSB    _u(15)
#define POWMAN_LPOSC_FREQ_KHZ_FRAC_LSB    _u(0)
#define POWMAN_LPOSC_FREQ_KHZ_FRAC_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_XOSC_FREQ_KHZ_INT
// Description : Informs the AON Timer of the integer component of the clock
//               frequency when running off the XOSC.
//               Integer component of the XOSC frequency in kHz. Default = 12000
//               Must be >1 This field must only be written when
//               POWMAN_TIMER_RUN=0 or POWMAN_TIMER_USING_XOSC=0
#define POWMAN_XOSC_FREQ_KHZ_INT_OFFSET _u(0x00000058)
#define POWMAN_XOSC_FREQ_KHZ_INT_BITS   _u(0x0000ffff)
#define POWMAN_XOSC_FREQ_KHZ_INT_RESET  _u(0x00002ee0)
#define POWMAN_XOSC_FREQ_KHZ_INT_MSB    _u(15)
#define POWMAN_XOSC_FREQ_KHZ_INT_LSB    _u(0)
#define POWMAN_XOSC_FREQ_KHZ_INT_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_XOSC_FREQ_KHZ_FRAC
// Description : Informs the AON Timer of the fractional component of the clock
//               frequency when running off the XOSC.
//               Fractional component of the XOSC frequency in kHz. This field
//               must only be written when POWMAN_TIMER_RUN=0 or
//               POWMAN_TIMER_USING_XOSC=0
#define POWMAN_XOSC_FREQ_KHZ_FRAC_OFFSET _u(0x0000005c)
#define POWMAN_XOSC_FREQ_KHZ_FRAC_BITS   _u(0x0000ffff)
#define POWMAN_XOSC_FREQ_KHZ_FRAC_RESET  _u(0x00000000)
#define POWMAN_XOSC_FREQ_KHZ_FRAC_MSB    _u(15)
#define POWMAN_XOSC_FREQ_KHZ_FRAC_LSB    _u(0)
#define POWMAN_XOSC_FREQ_KHZ_FRAC_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_SET_TIME_63TO48
// Description : None
//               For setting the time, do not use for reading the time, use
//               POWMAN_READ_TIME_UPPER and POWMAN_READ_TIME_LOWER. This field
//               must only be written when POWMAN_TIMER_RUN=0
#define POWMAN_SET_TIME_63TO48_OFFSET _u(0x00000060)
#define POWMAN_SET_TIME_63TO48_BITS   _u(0x0000ffff)
#define POWMAN_SET_TIME_63TO48_RESET  _u(0x00000000)
#define POWMAN_SET_TIME_63TO48_MSB    _u(15)
#define POWMAN_SET_TIME_63TO48_LSB    _u(0)
#define POWMAN_SET_TIME_63TO48_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_SET_TIME_47TO32
// Description : None
//               For setting the time, do not use for reading the time, use
//               POWMAN_READ_TIME_UPPER and POWMAN_READ_TIME_LOWER. This field
//               must only be written when POWMAN_TIMER_RUN=0
#define POWMAN_SET_TIME_47TO32_OFFSET _u(0x00000064)
#define POWMAN_SET_TIME_47TO32_BITS   _u(0x0000ffff)
#define POWMAN_SET_TIME_47TO32_RESET  _u(0x00000000)
#define POWMAN_SET_TIME_47TO32_MSB    _u(15)
#define POWMAN_SET_TIME_47TO32_LSB    _u(0)
#define POWMAN_SET_TIME_47TO32_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_SET_TIME_31TO16
// Description : None
//               For setting the time, do not use for reading the time, use
//               POWMAN_READ_TIME_UPPER and POWMAN_READ_TIME_LOWER. This field
//               must only be written when POWMAN_TIMER_RUN=0
#define POWMAN_SET_TIME_31TO16_OFFSET _u(0x00000068)
#define POWMAN_SET_TIME_31TO16_BITS   _u(0x0000ffff)
#define POWMAN_SET_TIME_31TO16_RESET  _u(0x00000000)
#define POWMAN_SET_TIME_31TO16_MSB    _u(15)
#define POWMAN_SET_TIME_31TO16_LSB    _u(0)
#define POWMAN_SET_TIME_31TO16_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_SET_TIME_15TO0
// Description : None
//               For setting the time, do not use for reading the time, use
//               POWMAN_READ_TIME_UPPER and POWMAN_READ_TIME_LOWER. This field
//               must only be written when POWMAN_TIMER_RUN=0
#define POWMAN_SET_TIME_15TO0_OFFSET _u(0x0000006c)
#define POWMAN_SET_TIME_15TO0_BITS   _u(0x0000ffff)
#define POWMAN_SET_TIME_15TO0_RESET  _u(0x00000000)
#define POWMAN_SET_TIME_15TO0_MSB    _u(15)
#define POWMAN_SET_TIME_15TO0_LSB    _u(0)
#define POWMAN_SET_TIME_15TO0_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_READ_TIME_UPPER
// Description : None
//               For reading bits 63:32 of the timer. When reading all 64 bits
//               it is possible for the LOWER count to rollover during the read.
//               It is recommended to read UPPER, then LOWER, then re-read UPPER
//               and, if it has changed, re-read LOWER.
#define POWMAN_READ_TIME_UPPER_OFFSET _u(0x00000070)
#define POWMAN_READ_TIME_UPPER_BITS   _u(0xffffffff)
#define POWMAN_READ_TIME_UPPER_RESET  _u(0x00000000)
#define POWMAN_READ_TIME_UPPER_MSB    _u(31)
#define POWMAN_READ_TIME_UPPER_LSB    _u(0)
#define POWMAN_READ_TIME_UPPER_ACCESS "RO"
// =============================================================================
// Register    : POWMAN_READ_TIME_LOWER
// Description : None
//               For reading bits 31:0 of the timer.
#define POWMAN_READ_TIME_LOWER_OFFSET _u(0x00000074)
#define POWMAN_READ_TIME_LOWER_BITS   _u(0xffffffff)
#define POWMAN_READ_TIME_LOWER_RESET  _u(0x00000000)
#define POWMAN_READ_TIME_LOWER_MSB    _u(31)
#define POWMAN_READ_TIME_LOWER_LSB    _u(0)
#define POWMAN_READ_TIME_LOWER_ACCESS "RO"
// =============================================================================
// Register    : POWMAN_ALARM_TIME_63TO48
// Description : None
//               This field must only be written when POWMAN_ALARM_ENAB=0
#define POWMAN_ALARM_TIME_63TO48_OFFSET _u(0x00000078)
#define POWMAN_ALARM_TIME_63TO48_BITS   _u(0x0000ffff)
#define POWMAN_ALARM_TIME_63TO48_RESET  _u(0x00000000)
#define POWMAN_ALARM_TIME_63TO48_MSB    _u(15)
#define POWMAN_ALARM_TIME_63TO48_LSB    _u(0)
#define POWMAN_ALARM_TIME_63TO48_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_ALARM_TIME_47TO32
// Description : None
//               This field must only be written when POWMAN_ALARM_ENAB=0
#define POWMAN_ALARM_TIME_47TO32_OFFSET _u(0x0000007c)
#define POWMAN_ALARM_TIME_47TO32_BITS   _u(0x0000ffff)
#define POWMAN_ALARM_TIME_47TO32_RESET  _u(0x00000000)
#define POWMAN_ALARM_TIME_47TO32_MSB    _u(15)
#define POWMAN_ALARM_TIME_47TO32_LSB    _u(0)
#define POWMAN_ALARM_TIME_47TO32_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_ALARM_TIME_31TO16
// Description : None
//               This field must only be written when POWMAN_ALARM_ENAB=0
#define POWMAN_ALARM_TIME_31TO16_OFFSET _u(0x00000080)
#define POWMAN_ALARM_TIME_31TO16_BITS   _u(0x0000ffff)
#define POWMAN_ALARM_TIME_31TO16_RESET  _u(0x00000000)
#define POWMAN_ALARM_TIME_31TO16_MSB    _u(15)
#define POWMAN_ALARM_TIME_31TO16_LSB    _u(0)
#define POWMAN_ALARM_TIME_31TO16_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_ALARM_TIME_15TO0
// Description : None
//               This field must only be written when POWMAN_ALARM_ENAB=0
#define POWMAN_ALARM_TIME_15TO0_OFFSET _u(0x00000084)
#define POWMAN_ALARM_TIME_15TO0_BITS   _u(0x0000ffff)
#define POWMAN_ALARM_TIME_15TO0_RESET  _u(0x00000000)
#define POWMAN_ALARM_TIME_15TO0_MSB    _u(15)
#define POWMAN_ALARM_TIME_15TO0_LSB    _u(0)
#define POWMAN_ALARM_TIME_15TO0_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_TIMER
#define POWMAN_TIMER_OFFSET _u(0x00000088)
#define POWMAN_TIMER_BITS   _u(0x000f2777)
#define POWMAN_TIMER_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : POWMAN_TIMER_USING_GPIO_1HZ
// Description : Timer is synchronised to a 1hz gpio source
#define POWMAN_TIMER_USING_GPIO_1HZ_RESET  _u(0x0)
#define POWMAN_TIMER_USING_GPIO_1HZ_BITS   _u(0x00080000)
#define POWMAN_TIMER_USING_GPIO_1HZ_MSB    _u(19)
#define POWMAN_TIMER_USING_GPIO_1HZ_LSB    _u(19)
#define POWMAN_TIMER_USING_GPIO_1HZ_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_TIMER_USING_GPIO_1KHZ
// Description : Timer is running from a 1khz gpio source
#define POWMAN_TIMER_USING_GPIO_1KHZ_RESET  _u(0x0)
#define POWMAN_TIMER_USING_GPIO_1KHZ_BITS   _u(0x00040000)
#define POWMAN_TIMER_USING_GPIO_1KHZ_MSB    _u(18)
#define POWMAN_TIMER_USING_GPIO_1KHZ_LSB    _u(18)
#define POWMAN_TIMER_USING_GPIO_1KHZ_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_TIMER_USING_LPOSC
// Description : Timer is running from lposc
#define POWMAN_TIMER_USING_LPOSC_RESET  _u(0x0)
#define POWMAN_TIMER_USING_LPOSC_BITS   _u(0x00020000)
#define POWMAN_TIMER_USING_LPOSC_MSB    _u(17)
#define POWMAN_TIMER_USING_LPOSC_LSB    _u(17)
#define POWMAN_TIMER_USING_LPOSC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_TIMER_USING_XOSC
// Description : Timer is running from xosc
#define POWMAN_TIMER_USING_XOSC_RESET  _u(0x0)
#define POWMAN_TIMER_USING_XOSC_BITS   _u(0x00010000)
#define POWMAN_TIMER_USING_XOSC_MSB    _u(16)
#define POWMAN_TIMER_USING_XOSC_LSB    _u(16)
#define POWMAN_TIMER_USING_XOSC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_TIMER_USE_GPIO_1HZ
// Description : Selects the gpio source as the reference for the sec counter.
//               The msec counter will continue to use the lposc or xosc
//               reference.
#define POWMAN_TIMER_USE_GPIO_1HZ_RESET  _u(0x0)
#define POWMAN_TIMER_USE_GPIO_1HZ_BITS   _u(0x00002000)
#define POWMAN_TIMER_USE_GPIO_1HZ_MSB    _u(13)
#define POWMAN_TIMER_USE_GPIO_1HZ_LSB    _u(13)
#define POWMAN_TIMER_USE_GPIO_1HZ_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_TIMER_USE_GPIO_1KHZ
// Description : switch to gpio as the source of the 1kHz timer tick
#define POWMAN_TIMER_USE_GPIO_1KHZ_RESET  _u(0x0)
#define POWMAN_TIMER_USE_GPIO_1KHZ_BITS   _u(0x00000400)
#define POWMAN_TIMER_USE_GPIO_1KHZ_MSB    _u(10)
#define POWMAN_TIMER_USE_GPIO_1KHZ_LSB    _u(10)
#define POWMAN_TIMER_USE_GPIO_1KHZ_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : POWMAN_TIMER_USE_XOSC
// Description : switch to xosc as the source of the 1kHz timer tick
#define POWMAN_TIMER_USE_XOSC_RESET  _u(0x0)
#define POWMAN_TIMER_USE_XOSC_BITS   _u(0x00000200)
#define POWMAN_TIMER_USE_XOSC_MSB    _u(9)
#define POWMAN_TIMER_USE_XOSC_LSB    _u(9)
#define POWMAN_TIMER_USE_XOSC_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : POWMAN_TIMER_USE_LPOSC
// Description : Switch to lposc as the source of the 1kHz timer tick
#define POWMAN_TIMER_USE_LPOSC_RESET  _u(0x0)
#define POWMAN_TIMER_USE_LPOSC_BITS   _u(0x00000100)
#define POWMAN_TIMER_USE_LPOSC_MSB    _u(8)
#define POWMAN_TIMER_USE_LPOSC_LSB    _u(8)
#define POWMAN_TIMER_USE_LPOSC_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : POWMAN_TIMER_ALARM
// Description : Alarm has fired. Write to 1 to clear the alarm.
#define POWMAN_TIMER_ALARM_RESET  _u(0x0)
#define POWMAN_TIMER_ALARM_BITS   _u(0x00000040)
#define POWMAN_TIMER_ALARM_MSB    _u(6)
#define POWMAN_TIMER_ALARM_LSB    _u(6)
#define POWMAN_TIMER_ALARM_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : POWMAN_TIMER_PWRUP_ON_ALARM
// Description : Alarm wakes the chip from low power mode
#define POWMAN_TIMER_PWRUP_ON_ALARM_RESET  _u(0x0)
#define POWMAN_TIMER_PWRUP_ON_ALARM_BITS   _u(0x00000020)
#define POWMAN_TIMER_PWRUP_ON_ALARM_MSB    _u(5)
#define POWMAN_TIMER_PWRUP_ON_ALARM_LSB    _u(5)
#define POWMAN_TIMER_PWRUP_ON_ALARM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_TIMER_ALARM_ENAB
// Description : Enables the alarm. The alarm must be disabled while writing the
//               alarm time.
#define POWMAN_TIMER_ALARM_ENAB_RESET  _u(0x0)
#define POWMAN_TIMER_ALARM_ENAB_BITS   _u(0x00000010)
#define POWMAN_TIMER_ALARM_ENAB_MSB    _u(4)
#define POWMAN_TIMER_ALARM_ENAB_LSB    _u(4)
#define POWMAN_TIMER_ALARM_ENAB_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_TIMER_CLEAR
// Description : Clears the timer, does not disable the timer and does not
//               affect the alarm. This control can be written at any time.
#define POWMAN_TIMER_CLEAR_RESET  _u(0x0)
#define POWMAN_TIMER_CLEAR_BITS   _u(0x00000004)
#define POWMAN_TIMER_CLEAR_MSB    _u(2)
#define POWMAN_TIMER_CLEAR_LSB    _u(2)
#define POWMAN_TIMER_CLEAR_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : POWMAN_TIMER_RUN
// Description : Timer enable. Setting this bit causes the timer to begin
//               counting up from its current value. Clearing this bit stops the
//               timer from counting.
//
//               Before enabling the timer, set the POWMAN_LPOSC_FREQ* and
//               POWMAN_XOSC_FREQ* registers to configure the count rate, and
//               initialise the current time by writing to SET_TIME_63TO48
//               through SET_TIME_15TO0. You must not write to the SET_TIME_x
//               registers when the timer is running.
//
//               Once configured, start the timer by setting POWMAN_TIMER_RUN=1.
//               This will start the timer running from the LPOSC. When the XOSC
//               is available switch the reference clock to XOSC then select it
//               as the timer clock by setting POWMAN_TIMER_USE_XOSC=1
#define POWMAN_TIMER_RUN_RESET  _u(0x0)
#define POWMAN_TIMER_RUN_BITS   _u(0x00000002)
#define POWMAN_TIMER_RUN_MSB    _u(1)
#define POWMAN_TIMER_RUN_LSB    _u(1)
#define POWMAN_TIMER_RUN_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_TIMER_NONSEC_WRITE
// Description : Control whether Non-secure software can write to the timer
//               registers. All other registers are hardwired to be inaccessible
//               to Non-secure.
#define POWMAN_TIMER_NONSEC_WRITE_RESET  _u(0x0)
#define POWMAN_TIMER_NONSEC_WRITE_BITS   _u(0x00000001)
#define POWMAN_TIMER_NONSEC_WRITE_MSB    _u(0)
#define POWMAN_TIMER_NONSEC_WRITE_LSB    _u(0)
#define POWMAN_TIMER_NONSEC_WRITE_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_PWRUP0
// Description : 4 GPIO powerup events can be configured to wake the chip up
//               from a low power state.
//               The pwrups are level/edge sensitive and can be set to trigger
//               on a high/rising or low/falling event
//               The number of gpios available depends on the package option. An
//               invalid selection will be ignored
//               source = 0 selects gpio0
//               .
//               .
//               source = 47 selects gpio47
//               source = 48 selects qspi_ss
//               source = 49 selects qspi_sd0
//               source = 50 selects qspi_sd1
//               source = 51 selects qspi_sd2
//               source = 52 selects qspi_sd3
//               source = 53 selects qspi_sclk
//               level  = 0 triggers the pwrup when the source is low
//               level  = 1 triggers the pwrup when the source is high
#define POWMAN_PWRUP0_OFFSET _u(0x0000008c)
#define POWMAN_PWRUP0_BITS   _u(0x000007ff)
#define POWMAN_PWRUP0_RESET  _u(0x0000003f)
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP0_RAW_STATUS
// Description : Value of selected gpio pin (only if enable == 1)
#define POWMAN_PWRUP0_RAW_STATUS_RESET  _u(0x0)
#define POWMAN_PWRUP0_RAW_STATUS_BITS   _u(0x00000400)
#define POWMAN_PWRUP0_RAW_STATUS_MSB    _u(10)
#define POWMAN_PWRUP0_RAW_STATUS_LSB    _u(10)
#define POWMAN_PWRUP0_RAW_STATUS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP0_STATUS
// Description : Status of gpio wakeup. Write to 1 to clear a latched edge
//               detect.
#define POWMAN_PWRUP0_STATUS_RESET  _u(0x0)
#define POWMAN_PWRUP0_STATUS_BITS   _u(0x00000200)
#define POWMAN_PWRUP0_STATUS_MSB    _u(9)
#define POWMAN_PWRUP0_STATUS_LSB    _u(9)
#define POWMAN_PWRUP0_STATUS_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP0_MODE
// Description : Edge or level detect. Edge will detect a 0 to 1 transition (or
//               1 to 0 transition). Level will detect a 1 or 0. Both types of
//               event get latched into the current_pwrup_req register.
//               0x0 -> level
//               0x1 -> edge
#define POWMAN_PWRUP0_MODE_RESET  _u(0x0)
#define POWMAN_PWRUP0_MODE_BITS   _u(0x00000100)
#define POWMAN_PWRUP0_MODE_MSB    _u(8)
#define POWMAN_PWRUP0_MODE_LSB    _u(8)
#define POWMAN_PWRUP0_MODE_ACCESS "RW"
#define POWMAN_PWRUP0_MODE_VALUE_LEVEL _u(0x0)
#define POWMAN_PWRUP0_MODE_VALUE_EDGE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP0_DIRECTION
//               0x0 -> low_falling
//               0x1 -> high_rising
#define POWMAN_PWRUP0_DIRECTION_RESET  _u(0x0)
#define POWMAN_PWRUP0_DIRECTION_BITS   _u(0x00000080)
#define POWMAN_PWRUP0_DIRECTION_MSB    _u(7)
#define POWMAN_PWRUP0_DIRECTION_LSB    _u(7)
#define POWMAN_PWRUP0_DIRECTION_ACCESS "RW"
#define POWMAN_PWRUP0_DIRECTION_VALUE_LOW_FALLING _u(0x0)
#define POWMAN_PWRUP0_DIRECTION_VALUE_HIGH_RISING _u(0x1)
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP0_ENABLE
// Description : Set to 1 to enable the wakeup source. Set to 0 to disable the
//               wakeup source and clear a pending wakeup event.
//               If using edge detect a latched edge needs to be cleared by
//               writing 1 to the status register also.
#define POWMAN_PWRUP0_ENABLE_RESET  _u(0x0)
#define POWMAN_PWRUP0_ENABLE_BITS   _u(0x00000040)
#define POWMAN_PWRUP0_ENABLE_MSB    _u(6)
#define POWMAN_PWRUP0_ENABLE_LSB    _u(6)
#define POWMAN_PWRUP0_ENABLE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP0_SOURCE
#define POWMAN_PWRUP0_SOURCE_RESET  _u(0x3f)
#define POWMAN_PWRUP0_SOURCE_BITS   _u(0x0000003f)
#define POWMAN_PWRUP0_SOURCE_MSB    _u(5)
#define POWMAN_PWRUP0_SOURCE_LSB    _u(0)
#define POWMAN_PWRUP0_SOURCE_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_PWRUP1
// Description : 4 GPIO powerup events can be configured to wake the chip up
//               from a low power state.
//               The pwrups are level/edge sensitive and can be set to trigger
//               on a high/rising or low/falling event
//               The number of gpios available depends on the package option. An
//               invalid selection will be ignored
//               source = 0 selects gpio0
//               .
//               .
//               source = 47 selects gpio47
//               source = 48 selects qspi_ss
//               source = 49 selects qspi_sd0
//               source = 50 selects qspi_sd1
//               source = 51 selects qspi_sd2
//               source = 52 selects qspi_sd3
//               source = 53 selects qspi_sclk
//               level  = 0 triggers the pwrup when the source is low
//               level  = 1 triggers the pwrup when the source is high
#define POWMAN_PWRUP1_OFFSET _u(0x00000090)
#define POWMAN_PWRUP1_BITS   _u(0x000007ff)
#define POWMAN_PWRUP1_RESET  _u(0x0000003f)
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP1_RAW_STATUS
// Description : Value of selected gpio pin (only if enable == 1)
#define POWMAN_PWRUP1_RAW_STATUS_RESET  _u(0x0)
#define POWMAN_PWRUP1_RAW_STATUS_BITS   _u(0x00000400)
#define POWMAN_PWRUP1_RAW_STATUS_MSB    _u(10)
#define POWMAN_PWRUP1_RAW_STATUS_LSB    _u(10)
#define POWMAN_PWRUP1_RAW_STATUS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP1_STATUS
// Description : Status of gpio wakeup. Write to 1 to clear a latched edge
//               detect.
#define POWMAN_PWRUP1_STATUS_RESET  _u(0x0)
#define POWMAN_PWRUP1_STATUS_BITS   _u(0x00000200)
#define POWMAN_PWRUP1_STATUS_MSB    _u(9)
#define POWMAN_PWRUP1_STATUS_LSB    _u(9)
#define POWMAN_PWRUP1_STATUS_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP1_MODE
// Description : Edge or level detect. Edge will detect a 0 to 1 transition (or
//               1 to 0 transition). Level will detect a 1 or 0. Both types of
//               event get latched into the current_pwrup_req register.
//               0x0 -> level
//               0x1 -> edge
#define POWMAN_PWRUP1_MODE_RESET  _u(0x0)
#define POWMAN_PWRUP1_MODE_BITS   _u(0x00000100)
#define POWMAN_PWRUP1_MODE_MSB    _u(8)
#define POWMAN_PWRUP1_MODE_LSB    _u(8)
#define POWMAN_PWRUP1_MODE_ACCESS "RW"
#define POWMAN_PWRUP1_MODE_VALUE_LEVEL _u(0x0)
#define POWMAN_PWRUP1_MODE_VALUE_EDGE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP1_DIRECTION
//               0x0 -> low_falling
//               0x1 -> high_rising
#define POWMAN_PWRUP1_DIRECTION_RESET  _u(0x0)
#define POWMAN_PWRUP1_DIRECTION_BITS   _u(0x00000080)
#define POWMAN_PWRUP1_DIRECTION_MSB    _u(7)
#define POWMAN_PWRUP1_DIRECTION_LSB    _u(7)
#define POWMAN_PWRUP1_DIRECTION_ACCESS "RW"
#define POWMAN_PWRUP1_DIRECTION_VALUE_LOW_FALLING _u(0x0)
#define POWMAN_PWRUP1_DIRECTION_VALUE_HIGH_RISING _u(0x1)
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP1_ENABLE
// Description : Set to 1 to enable the wakeup source. Set to 0 to disable the
//               wakeup source and clear a pending wakeup event.
//               If using edge detect a latched edge needs to be cleared by
//               writing 1 to the status register also.
#define POWMAN_PWRUP1_ENABLE_RESET  _u(0x0)
#define POWMAN_PWRUP1_ENABLE_BITS   _u(0x00000040)
#define POWMAN_PWRUP1_ENABLE_MSB    _u(6)
#define POWMAN_PWRUP1_ENABLE_LSB    _u(6)
#define POWMAN_PWRUP1_ENABLE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP1_SOURCE
#define POWMAN_PWRUP1_SOURCE_RESET  _u(0x3f)
#define POWMAN_PWRUP1_SOURCE_BITS   _u(0x0000003f)
#define POWMAN_PWRUP1_SOURCE_MSB    _u(5)
#define POWMAN_PWRUP1_SOURCE_LSB    _u(0)
#define POWMAN_PWRUP1_SOURCE_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_PWRUP2
// Description : 4 GPIO powerup events can be configured to wake the chip up
//               from a low power state.
//               The pwrups are level/edge sensitive and can be set to trigger
//               on a high/rising or low/falling event
//               The number of gpios available depends on the package option. An
//               invalid selection will be ignored
//               source = 0 selects gpio0
//               .
//               .
//               source = 47 selects gpio47
//               source = 48 selects qspi_ss
//               source = 49 selects qspi_sd0
//               source = 50 selects qspi_sd1
//               source = 51 selects qspi_sd2
//               source = 52 selects qspi_sd3
//               source = 53 selects qspi_sclk
//               level  = 0 triggers the pwrup when the source is low
//               level  = 1 triggers the pwrup when the source is high
#define POWMAN_PWRUP2_OFFSET _u(0x00000094)
#define POWMAN_PWRUP2_BITS   _u(0x000007ff)
#define POWMAN_PWRUP2_RESET  _u(0x0000003f)
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP2_RAW_STATUS
// Description : Value of selected gpio pin (only if enable == 1)
#define POWMAN_PWRUP2_RAW_STATUS_RESET  _u(0x0)
#define POWMAN_PWRUP2_RAW_STATUS_BITS   _u(0x00000400)
#define POWMAN_PWRUP2_RAW_STATUS_MSB    _u(10)
#define POWMAN_PWRUP2_RAW_STATUS_LSB    _u(10)
#define POWMAN_PWRUP2_RAW_STATUS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP2_STATUS
// Description : Status of gpio wakeup. Write to 1 to clear a latched edge
//               detect.
#define POWMAN_PWRUP2_STATUS_RESET  _u(0x0)
#define POWMAN_PWRUP2_STATUS_BITS   _u(0x00000200)
#define POWMAN_PWRUP2_STATUS_MSB    _u(9)
#define POWMAN_PWRUP2_STATUS_LSB    _u(9)
#define POWMAN_PWRUP2_STATUS_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP2_MODE
// Description : Edge or level detect. Edge will detect a 0 to 1 transition (or
//               1 to 0 transition). Level will detect a 1 or 0. Both types of
//               event get latched into the current_pwrup_req register.
//               0x0 -> level
//               0x1 -> edge
#define POWMAN_PWRUP2_MODE_RESET  _u(0x0)
#define POWMAN_PWRUP2_MODE_BITS   _u(0x00000100)
#define POWMAN_PWRUP2_MODE_MSB    _u(8)
#define POWMAN_PWRUP2_MODE_LSB    _u(8)
#define POWMAN_PWRUP2_MODE_ACCESS "RW"
#define POWMAN_PWRUP2_MODE_VALUE_LEVEL _u(0x0)
#define POWMAN_PWRUP2_MODE_VALUE_EDGE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP2_DIRECTION
//               0x0 -> low_falling
//               0x1 -> high_rising
#define POWMAN_PWRUP2_DIRECTION_RESET  _u(0x0)
#define POWMAN_PWRUP2_DIRECTION_BITS   _u(0x00000080)
#define POWMAN_PWRUP2_DIRECTION_MSB    _u(7)
#define POWMAN_PWRUP2_DIRECTION_LSB    _u(7)
#define POWMAN_PWRUP2_DIRECTION_ACCESS "RW"
#define POWMAN_PWRUP2_DIRECTION_VALUE_LOW_FALLING _u(0x0)
#define POWMAN_PWRUP2_DIRECTION_VALUE_HIGH_RISING _u(0x1)
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP2_ENABLE
// Description : Set to 1 to enable the wakeup source. Set to 0 to disable the
//               wakeup source and clear a pending wakeup event.
//               If using edge detect a latched edge needs to be cleared by
//               writing 1 to the status register also.
#define POWMAN_PWRUP2_ENABLE_RESET  _u(0x0)
#define POWMAN_PWRUP2_ENABLE_BITS   _u(0x00000040)
#define POWMAN_PWRUP2_ENABLE_MSB    _u(6)
#define POWMAN_PWRUP2_ENABLE_LSB    _u(6)
#define POWMAN_PWRUP2_ENABLE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP2_SOURCE
#define POWMAN_PWRUP2_SOURCE_RESET  _u(0x3f)
#define POWMAN_PWRUP2_SOURCE_BITS   _u(0x0000003f)
#define POWMAN_PWRUP2_SOURCE_MSB    _u(5)
#define POWMAN_PWRUP2_SOURCE_LSB    _u(0)
#define POWMAN_PWRUP2_SOURCE_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_PWRUP3
// Description : 4 GPIO powerup events can be configured to wake the chip up
//               from a low power state.
//               The pwrups are level/edge sensitive and can be set to trigger
//               on a high/rising or low/falling event
//               The number of gpios available depends on the package option. An
//               invalid selection will be ignored
//               source = 0 selects gpio0
//               .
//               .
//               source = 47 selects gpio47
//               source = 48 selects qspi_ss
//               source = 49 selects qspi_sd0
//               source = 50 selects qspi_sd1
//               source = 51 selects qspi_sd2
//               source = 52 selects qspi_sd3
//               source = 53 selects qspi_sclk
//               level  = 0 triggers the pwrup when the source is low
//               level  = 1 triggers the pwrup when the source is high
#define POWMAN_PWRUP3_OFFSET _u(0x00000098)
#define POWMAN_PWRUP3_BITS   _u(0x000007ff)
#define POWMAN_PWRUP3_RESET  _u(0x0000003f)
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP3_RAW_STATUS
// Description : Value of selected gpio pin (only if enable == 1)
#define POWMAN_PWRUP3_RAW_STATUS_RESET  _u(0x0)
#define POWMAN_PWRUP3_RAW_STATUS_BITS   _u(0x00000400)
#define POWMAN_PWRUP3_RAW_STATUS_MSB    _u(10)
#define POWMAN_PWRUP3_RAW_STATUS_LSB    _u(10)
#define POWMAN_PWRUP3_RAW_STATUS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP3_STATUS
// Description : Status of gpio wakeup. Write to 1 to clear a latched edge
//               detect.
#define POWMAN_PWRUP3_STATUS_RESET  _u(0x0)
#define POWMAN_PWRUP3_STATUS_BITS   _u(0x00000200)
#define POWMAN_PWRUP3_STATUS_MSB    _u(9)
#define POWMAN_PWRUP3_STATUS_LSB    _u(9)
#define POWMAN_PWRUP3_STATUS_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP3_MODE
// Description : Edge or level detect. Edge will detect a 0 to 1 transition (or
//               1 to 0 transition). Level will detect a 1 or 0. Both types of
//               event get latched into the current_pwrup_req register.
//               0x0 -> level
//               0x1 -> edge
#define POWMAN_PWRUP3_MODE_RESET  _u(0x0)
#define POWMAN_PWRUP3_MODE_BITS   _u(0x00000100)
#define POWMAN_PWRUP3_MODE_MSB    _u(8)
#define POWMAN_PWRUP3_MODE_LSB    _u(8)
#define POWMAN_PWRUP3_MODE_ACCESS "RW"
#define POWMAN_PWRUP3_MODE_VALUE_LEVEL _u(0x0)
#define POWMAN_PWRUP3_MODE_VALUE_EDGE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP3_DIRECTION
//               0x0 -> low_falling
//               0x1 -> high_rising
#define POWMAN_PWRUP3_DIRECTION_RESET  _u(0x0)
#define POWMAN_PWRUP3_DIRECTION_BITS   _u(0x00000080)
#define POWMAN_PWRUP3_DIRECTION_MSB    _u(7)
#define POWMAN_PWRUP3_DIRECTION_LSB    _u(7)
#define POWMAN_PWRUP3_DIRECTION_ACCESS "RW"
#define POWMAN_PWRUP3_DIRECTION_VALUE_LOW_FALLING _u(0x0)
#define POWMAN_PWRUP3_DIRECTION_VALUE_HIGH_RISING _u(0x1)
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP3_ENABLE
// Description : Set to 1 to enable the wakeup source. Set to 0 to disable the
//               wakeup source and clear a pending wakeup event.
//               If using edge detect a latched edge needs to be cleared by
//               writing 1 to the status register also.
#define POWMAN_PWRUP3_ENABLE_RESET  _u(0x0)
#define POWMAN_PWRUP3_ENABLE_BITS   _u(0x00000040)
#define POWMAN_PWRUP3_ENABLE_MSB    _u(6)
#define POWMAN_PWRUP3_ENABLE_LSB    _u(6)
#define POWMAN_PWRUP3_ENABLE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_PWRUP3_SOURCE
#define POWMAN_PWRUP3_SOURCE_RESET  _u(0x3f)
#define POWMAN_PWRUP3_SOURCE_BITS   _u(0x0000003f)
#define POWMAN_PWRUP3_SOURCE_MSB    _u(5)
#define POWMAN_PWRUP3_SOURCE_LSB    _u(0)
#define POWMAN_PWRUP3_SOURCE_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_CURRENT_PWRUP_REQ
// Description : Indicates current powerup request state
//               pwrup events can be cleared by removing the enable from the
//               pwrup register. The alarm pwrup req can be cleared by clearing
//               timer.alarm_enab
//               0 = chip reset, for the source of the last reset see
//               POWMAN_CHIP_RESET
//               1 = pwrup0
//               2 = pwrup1
//               3 = pwrup2
//               4 = pwrup3
//               5 = coresight_pwrup
//               6 = alarm_pwrup
#define POWMAN_CURRENT_PWRUP_REQ_OFFSET _u(0x0000009c)
#define POWMAN_CURRENT_PWRUP_REQ_BITS   _u(0x0000007f)
#define POWMAN_CURRENT_PWRUP_REQ_RESET  _u(0x00000000)
#define POWMAN_CURRENT_PWRUP_REQ_MSB    _u(6)
#define POWMAN_CURRENT_PWRUP_REQ_LSB    _u(0)
#define POWMAN_CURRENT_PWRUP_REQ_ACCESS "RO"
// =============================================================================
// Register    : POWMAN_LAST_SWCORE_PWRUP
// Description : Indicates which pwrup source triggered the last switched-core
//               power up
//               0 = chip reset, for the source of the last reset see
//               POWMAN_CHIP_RESET
//               1 = pwrup0
//               2 = pwrup1
//               3 = pwrup2
//               4 = pwrup3
//               5 = coresight_pwrup
//               6 = alarm_pwrup
#define POWMAN_LAST_SWCORE_PWRUP_OFFSET _u(0x000000a0)
#define POWMAN_LAST_SWCORE_PWRUP_BITS   _u(0x0000007f)
#define POWMAN_LAST_SWCORE_PWRUP_RESET  _u(0x00000000)
#define POWMAN_LAST_SWCORE_PWRUP_MSB    _u(6)
#define POWMAN_LAST_SWCORE_PWRUP_LSB    _u(0)
#define POWMAN_LAST_SWCORE_PWRUP_ACCESS "RO"
// =============================================================================
// Register    : POWMAN_DBG_PWRCFG
#define POWMAN_DBG_PWRCFG_OFFSET _u(0x000000a4)
#define POWMAN_DBG_PWRCFG_BITS   _u(0x00000001)
#define POWMAN_DBG_PWRCFG_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : POWMAN_DBG_PWRCFG_IGNORE
// Description : Ignore pwrup req from debugger. If pwrup req is asserted then
//               this will prevent power down and set powerdown blocked. Set
//               ignore to stop paying attention to pwrup_req
#define POWMAN_DBG_PWRCFG_IGNORE_RESET  _u(0x0)
#define POWMAN_DBG_PWRCFG_IGNORE_BITS   _u(0x00000001)
#define POWMAN_DBG_PWRCFG_IGNORE_MSB    _u(0)
#define POWMAN_DBG_PWRCFG_IGNORE_LSB    _u(0)
#define POWMAN_DBG_PWRCFG_IGNORE_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_BOOTDIS
// Description : Tell the bootrom to ignore the BOOT0..3 registers following the
//               next RSM reset (e.g. the next core power down/up).
//
//               If an early boot stage has soft-locked some OTP pages in order
//               to protect their contents from later stages, there is a risk
//               that Secure code running at a later stage can unlock the pages
//               by powering the core up and down.
//
//               This register can be used to ensure that the bootloader runs as
//               normal on the next power up, preventing Secure code at a later
//               stage from accessing OTP in its unlocked state.
//
//               Should be used in conjunction with the OTP BOOTDIS register.
#define POWMAN_BOOTDIS_OFFSET _u(0x000000a8)
#define POWMAN_BOOTDIS_BITS   _u(0x00000003)
#define POWMAN_BOOTDIS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : POWMAN_BOOTDIS_NEXT
// Description : This flag always ORs writes into its current contents. It can
//               be set but not cleared by software.
//
//               The BOOTDIS_NEXT bit is OR'd into the BOOTDIS_NOW bit when the
//               core is powered down. Simultaneously, the BOOTDIS_NEXT bit is
//               cleared. Setting this bit means that the BOOT0..3 registers
//               will be ignored following the next reset of the RSM by powman.
//
//               This flag should be set by an early boot stage that has soft-
//               locked OTP pages, to prevent later stages from unlocking it by
//               power cycling.
#define POWMAN_BOOTDIS_NEXT_RESET  _u(0x0)
#define POWMAN_BOOTDIS_NEXT_BITS   _u(0x00000002)
#define POWMAN_BOOTDIS_NEXT_MSB    _u(1)
#define POWMAN_BOOTDIS_NEXT_LSB    _u(1)
#define POWMAN_BOOTDIS_NEXT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_BOOTDIS_NOW
// Description : When powman resets the RSM, the current value of BOOTDIS_NEXT
//               is OR'd into BOOTDIS_NOW, and BOOTDIS_NEXT is cleared.
//
//               The bootrom checks this flag before reading the BOOT0..3
//               registers. If it is set, the bootrom clears it, and ignores the
//               BOOT registers. This prevents Secure software from diverting
//               the boot path before a bootloader has had the chance to soft
//               lock OTP pages containing sensitive data.
#define POWMAN_BOOTDIS_NOW_RESET  _u(0x0)
#define POWMAN_BOOTDIS_NOW_BITS   _u(0x00000001)
#define POWMAN_BOOTDIS_NOW_MSB    _u(0)
#define POWMAN_BOOTDIS_NOW_LSB    _u(0)
#define POWMAN_BOOTDIS_NOW_ACCESS "WC"
// =============================================================================
// Register    : POWMAN_DBGCONFIG
#define POWMAN_DBGCONFIG_OFFSET _u(0x000000ac)
#define POWMAN_DBGCONFIG_BITS   _u(0x0000000f)
#define POWMAN_DBGCONFIG_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : POWMAN_DBGCONFIG_DP_INSTID
// Description : Configure DP instance ID for SWD multidrop selection.
//               Recommend that this is NOT changed until you require debug
//               access in multi-chip environment
#define POWMAN_DBGCONFIG_DP_INSTID_RESET  _u(0x0)
#define POWMAN_DBGCONFIG_DP_INSTID_BITS   _u(0x0000000f)
#define POWMAN_DBGCONFIG_DP_INSTID_MSB    _u(3)
#define POWMAN_DBGCONFIG_DP_INSTID_LSB    _u(0)
#define POWMAN_DBGCONFIG_DP_INSTID_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_SCRATCH0
// Description : Scratch register. Information persists in low power mode
#define POWMAN_SCRATCH0_OFFSET _u(0x000000b0)
#define POWMAN_SCRATCH0_BITS   _u(0xffffffff)
#define POWMAN_SCRATCH0_RESET  _u(0x00000000)
#define POWMAN_SCRATCH0_MSB    _u(31)
#define POWMAN_SCRATCH0_LSB    _u(0)
#define POWMAN_SCRATCH0_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_SCRATCH1
// Description : Scratch register. Information persists in low power mode
#define POWMAN_SCRATCH1_OFFSET _u(0x000000b4)
#define POWMAN_SCRATCH1_BITS   _u(0xffffffff)
#define POWMAN_SCRATCH1_RESET  _u(0x00000000)
#define POWMAN_SCRATCH1_MSB    _u(31)
#define POWMAN_SCRATCH1_LSB    _u(0)
#define POWMAN_SCRATCH1_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_SCRATCH2
// Description : Scratch register. Information persists in low power mode
#define POWMAN_SCRATCH2_OFFSET _u(0x000000b8)
#define POWMAN_SCRATCH2_BITS   _u(0xffffffff)
#define POWMAN_SCRATCH2_RESET  _u(0x00000000)
#define POWMAN_SCRATCH2_MSB    _u(31)
#define POWMAN_SCRATCH2_LSB    _u(0)
#define POWMAN_SCRATCH2_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_SCRATCH3
// Description : Scratch register. Information persists in low power mode
#define POWMAN_SCRATCH3_OFFSET _u(0x000000bc)
#define POWMAN_SCRATCH3_BITS   _u(0xffffffff)
#define POWMAN_SCRATCH3_RESET  _u(0x00000000)
#define POWMAN_SCRATCH3_MSB    _u(31)
#define POWMAN_SCRATCH3_LSB    _u(0)
#define POWMAN_SCRATCH3_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_SCRATCH4
// Description : Scratch register. Information persists in low power mode
#define POWMAN_SCRATCH4_OFFSET _u(0x000000c0)
#define POWMAN_SCRATCH4_BITS   _u(0xffffffff)
#define POWMAN_SCRATCH4_RESET  _u(0x00000000)
#define POWMAN_SCRATCH4_MSB    _u(31)
#define POWMAN_SCRATCH4_LSB    _u(0)
#define POWMAN_SCRATCH4_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_SCRATCH5
// Description : Scratch register. Information persists in low power mode
#define POWMAN_SCRATCH5_OFFSET _u(0x000000c4)
#define POWMAN_SCRATCH5_BITS   _u(0xffffffff)
#define POWMAN_SCRATCH5_RESET  _u(0x00000000)
#define POWMAN_SCRATCH5_MSB    _u(31)
#define POWMAN_SCRATCH5_LSB    _u(0)
#define POWMAN_SCRATCH5_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_SCRATCH6
// Description : Scratch register. Information persists in low power mode
#define POWMAN_SCRATCH6_OFFSET _u(0x000000c8)
#define POWMAN_SCRATCH6_BITS   _u(0xffffffff)
#define POWMAN_SCRATCH6_RESET  _u(0x00000000)
#define POWMAN_SCRATCH6_MSB    _u(31)
#define POWMAN_SCRATCH6_LSB    _u(0)
#define POWMAN_SCRATCH6_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_SCRATCH7
// Description : Scratch register. Information persists in low power mode
#define POWMAN_SCRATCH7_OFFSET _u(0x000000cc)
#define POWMAN_SCRATCH7_BITS   _u(0xffffffff)
#define POWMAN_SCRATCH7_RESET  _u(0x00000000)
#define POWMAN_SCRATCH7_MSB    _u(31)
#define POWMAN_SCRATCH7_LSB    _u(0)
#define POWMAN_SCRATCH7_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_BOOT0
// Description : Scratch register. Information persists in low power mode
#define POWMAN_BOOT0_OFFSET _u(0x000000d0)
#define POWMAN_BOOT0_BITS   _u(0xffffffff)
#define POWMAN_BOOT0_RESET  _u(0x00000000)
#define POWMAN_BOOT0_MSB    _u(31)
#define POWMAN_BOOT0_LSB    _u(0)
#define POWMAN_BOOT0_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_BOOT1
// Description : Scratch register. Information persists in low power mode
#define POWMAN_BOOT1_OFFSET _u(0x000000d4)
#define POWMAN_BOOT1_BITS   _u(0xffffffff)
#define POWMAN_BOOT1_RESET  _u(0x00000000)
#define POWMAN_BOOT1_MSB    _u(31)
#define POWMAN_BOOT1_LSB    _u(0)
#define POWMAN_BOOT1_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_BOOT2
// Description : Scratch register. Information persists in low power mode
#define POWMAN_BOOT2_OFFSET _u(0x000000d8)
#define POWMAN_BOOT2_BITS   _u(0xffffffff)
#define POWMAN_BOOT2_RESET  _u(0x00000000)
#define POWMAN_BOOT2_MSB    _u(31)
#define POWMAN_BOOT2_LSB    _u(0)
#define POWMAN_BOOT2_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_BOOT3
// Description : Scratch register. Information persists in low power mode
#define POWMAN_BOOT3_OFFSET _u(0x000000dc)
#define POWMAN_BOOT3_BITS   _u(0xffffffff)
#define POWMAN_BOOT3_RESET  _u(0x00000000)
#define POWMAN_BOOT3_MSB    _u(31)
#define POWMAN_BOOT3_LSB    _u(0)
#define POWMAN_BOOT3_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_INTR
// Description : Raw Interrupts
#define POWMAN_INTR_OFFSET _u(0x000000e0)
#define POWMAN_INTR_BITS   _u(0x0000000f)
#define POWMAN_INTR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : POWMAN_INTR_PWRUP_WHILE_WAITING
// Description : Source is state.pwrup_while_waiting
#define POWMAN_INTR_PWRUP_WHILE_WAITING_RESET  _u(0x0)
#define POWMAN_INTR_PWRUP_WHILE_WAITING_BITS   _u(0x00000008)
#define POWMAN_INTR_PWRUP_WHILE_WAITING_MSB    _u(3)
#define POWMAN_INTR_PWRUP_WHILE_WAITING_LSB    _u(3)
#define POWMAN_INTR_PWRUP_WHILE_WAITING_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_INTR_STATE_REQ_IGNORED
// Description : Source is state.req_ignored
#define POWMAN_INTR_STATE_REQ_IGNORED_RESET  _u(0x0)
#define POWMAN_INTR_STATE_REQ_IGNORED_BITS   _u(0x00000004)
#define POWMAN_INTR_STATE_REQ_IGNORED_MSB    _u(2)
#define POWMAN_INTR_STATE_REQ_IGNORED_LSB    _u(2)
#define POWMAN_INTR_STATE_REQ_IGNORED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_INTR_TIMER
#define POWMAN_INTR_TIMER_RESET  _u(0x0)
#define POWMAN_INTR_TIMER_BITS   _u(0x00000002)
#define POWMAN_INTR_TIMER_MSB    _u(1)
#define POWMAN_INTR_TIMER_LSB    _u(1)
#define POWMAN_INTR_TIMER_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_INTR_VREG_OUTPUT_LOW
#define POWMAN_INTR_VREG_OUTPUT_LOW_RESET  _u(0x0)
#define POWMAN_INTR_VREG_OUTPUT_LOW_BITS   _u(0x00000001)
#define POWMAN_INTR_VREG_OUTPUT_LOW_MSB    _u(0)
#define POWMAN_INTR_VREG_OUTPUT_LOW_LSB    _u(0)
#define POWMAN_INTR_VREG_OUTPUT_LOW_ACCESS "WC"
// =============================================================================
// Register    : POWMAN_INTE
// Description : Interrupt Enable
#define POWMAN_INTE_OFFSET _u(0x000000e4)
#define POWMAN_INTE_BITS   _u(0x0000000f)
#define POWMAN_INTE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : POWMAN_INTE_PWRUP_WHILE_WAITING
// Description : Source is state.pwrup_while_waiting
#define POWMAN_INTE_PWRUP_WHILE_WAITING_RESET  _u(0x0)
#define POWMAN_INTE_PWRUP_WHILE_WAITING_BITS   _u(0x00000008)
#define POWMAN_INTE_PWRUP_WHILE_WAITING_MSB    _u(3)
#define POWMAN_INTE_PWRUP_WHILE_WAITING_LSB    _u(3)
#define POWMAN_INTE_PWRUP_WHILE_WAITING_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_INTE_STATE_REQ_IGNORED
// Description : Source is state.req_ignored
#define POWMAN_INTE_STATE_REQ_IGNORED_RESET  _u(0x0)
#define POWMAN_INTE_STATE_REQ_IGNORED_BITS   _u(0x00000004)
#define POWMAN_INTE_STATE_REQ_IGNORED_MSB    _u(2)
#define POWMAN_INTE_STATE_REQ_IGNORED_LSB    _u(2)
#define POWMAN_INTE_STATE_REQ_IGNORED_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_INTE_TIMER
#define POWMAN_INTE_TIMER_RESET  _u(0x0)
#define POWMAN_INTE_TIMER_BITS   _u(0x00000002)
#define POWMAN_INTE_TIMER_MSB    _u(1)
#define POWMAN_INTE_TIMER_LSB    _u(1)
#define POWMAN_INTE_TIMER_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_INTE_VREG_OUTPUT_LOW
#define POWMAN_INTE_VREG_OUTPUT_LOW_RESET  _u(0x0)
#define POWMAN_INTE_VREG_OUTPUT_LOW_BITS   _u(0x00000001)
#define POWMAN_INTE_VREG_OUTPUT_LOW_MSB    _u(0)
#define POWMAN_INTE_VREG_OUTPUT_LOW_LSB    _u(0)
#define POWMAN_INTE_VREG_OUTPUT_LOW_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_INTF
// Description : Interrupt Force
#define POWMAN_INTF_OFFSET _u(0x000000e8)
#define POWMAN_INTF_BITS   _u(0x0000000f)
#define POWMAN_INTF_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : POWMAN_INTF_PWRUP_WHILE_WAITING
// Description : Source is state.pwrup_while_waiting
#define POWMAN_INTF_PWRUP_WHILE_WAITING_RESET  _u(0x0)
#define POWMAN_INTF_PWRUP_WHILE_WAITING_BITS   _u(0x00000008)
#define POWMAN_INTF_PWRUP_WHILE_WAITING_MSB    _u(3)
#define POWMAN_INTF_PWRUP_WHILE_WAITING_LSB    _u(3)
#define POWMAN_INTF_PWRUP_WHILE_WAITING_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_INTF_STATE_REQ_IGNORED
// Description : Source is state.req_ignored
#define POWMAN_INTF_STATE_REQ_IGNORED_RESET  _u(0x0)
#define POWMAN_INTF_STATE_REQ_IGNORED_BITS   _u(0x00000004)
#define POWMAN_INTF_STATE_REQ_IGNORED_MSB    _u(2)
#define POWMAN_INTF_STATE_REQ_IGNORED_LSB    _u(2)
#define POWMAN_INTF_STATE_REQ_IGNORED_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_INTF_TIMER
#define POWMAN_INTF_TIMER_RESET  _u(0x0)
#define POWMAN_INTF_TIMER_BITS   _u(0x00000002)
#define POWMAN_INTF_TIMER_MSB    _u(1)
#define POWMAN_INTF_TIMER_LSB    _u(1)
#define POWMAN_INTF_TIMER_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : POWMAN_INTF_VREG_OUTPUT_LOW
#define POWMAN_INTF_VREG_OUTPUT_LOW_RESET  _u(0x0)
#define POWMAN_INTF_VREG_OUTPUT_LOW_BITS   _u(0x00000001)
#define POWMAN_INTF_VREG_OUTPUT_LOW_MSB    _u(0)
#define POWMAN_INTF_VREG_OUTPUT_LOW_LSB    _u(0)
#define POWMAN_INTF_VREG_OUTPUT_LOW_ACCESS "RW"
// =============================================================================
// Register    : POWMAN_INTS
// Description : Interrupt status after masking & forcing
#define POWMAN_INTS_OFFSET _u(0x000000ec)
#define POWMAN_INTS_BITS   _u(0x0000000f)
#define POWMAN_INTS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : POWMAN_INTS_PWRUP_WHILE_WAITING
// Description : Source is state.pwrup_while_waiting
#define POWMAN_INTS_PWRUP_WHILE_WAITING_RESET  _u(0x0)
#define POWMAN_INTS_PWRUP_WHILE_WAITING_BITS   _u(0x00000008)
#define POWMAN_INTS_PWRUP_WHILE_WAITING_MSB    _u(3)
#define POWMAN_INTS_PWRUP_WHILE_WAITING_LSB    _u(3)
#define POWMAN_INTS_PWRUP_WHILE_WAITING_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_INTS_STATE_REQ_IGNORED
// Description : Source is state.req_ignored
#define POWMAN_INTS_STATE_REQ_IGNORED_RESET  _u(0x0)
#define POWMAN_INTS_STATE_REQ_IGNORED_BITS   _u(0x00000004)
#define POWMAN_INTS_STATE_REQ_IGNORED_MSB    _u(2)
#define POWMAN_INTS_STATE_REQ_IGNORED_LSB    _u(2)
#define POWMAN_INTS_STATE_REQ_IGNORED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_INTS_TIMER
#define POWMAN_INTS_TIMER_RESET  _u(0x0)
#define POWMAN_INTS_TIMER_BITS   _u(0x00000002)
#define POWMAN_INTS_TIMER_MSB    _u(1)
#define POWMAN_INTS_TIMER_LSB    _u(1)
#define POWMAN_INTS_TIMER_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : POWMAN_INTS_VREG_OUTPUT_LOW
#define POWMAN_INTS_VREG_OUTPUT_LOW_RESET  _u(0x0)
#define POWMAN_INTS_VREG_OUTPUT_LOW_BITS   _u(0x00000001)
#define POWMAN_INTS_VREG_OUTPUT_LOW_MSB    _u(0)
#define POWMAN_INTS_VREG_OUTPUT_LOW_LSB    _u(0)
#define POWMAN_INTS_VREG_OUTPUT_LOW_ACCESS "RO"
// =============================================================================
#endif // _HARDWARE_REGS_POWMAN_H

