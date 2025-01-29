// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : BUSCTRL
// Version        : 1
// Bus type       : apb
// Description    : Register block for busfabric control signals and performance
//                  counters
// =============================================================================
#ifndef _HARDWARE_REGS_BUSCTRL_H
#define _HARDWARE_REGS_BUSCTRL_H
// =============================================================================
// Register    : BUSCTRL_BUS_PRIORITY
// Description : Set the priority of each master for bus arbitration.
#define BUSCTRL_BUS_PRIORITY_OFFSET _u(0x00000000)
#define BUSCTRL_BUS_PRIORITY_BITS   _u(0x00001111)
#define BUSCTRL_BUS_PRIORITY_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : BUSCTRL_BUS_PRIORITY_DMA_W
// Description : 0 - low priority, 1 - high priority
#define BUSCTRL_BUS_PRIORITY_DMA_W_RESET  _u(0x0)
#define BUSCTRL_BUS_PRIORITY_DMA_W_BITS   _u(0x00001000)
#define BUSCTRL_BUS_PRIORITY_DMA_W_MSB    _u(12)
#define BUSCTRL_BUS_PRIORITY_DMA_W_LSB    _u(12)
#define BUSCTRL_BUS_PRIORITY_DMA_W_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : BUSCTRL_BUS_PRIORITY_DMA_R
// Description : 0 - low priority, 1 - high priority
#define BUSCTRL_BUS_PRIORITY_DMA_R_RESET  _u(0x0)
#define BUSCTRL_BUS_PRIORITY_DMA_R_BITS   _u(0x00000100)
#define BUSCTRL_BUS_PRIORITY_DMA_R_MSB    _u(8)
#define BUSCTRL_BUS_PRIORITY_DMA_R_LSB    _u(8)
#define BUSCTRL_BUS_PRIORITY_DMA_R_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : BUSCTRL_BUS_PRIORITY_PROC1
// Description : 0 - low priority, 1 - high priority
#define BUSCTRL_BUS_PRIORITY_PROC1_RESET  _u(0x0)
#define BUSCTRL_BUS_PRIORITY_PROC1_BITS   _u(0x00000010)
#define BUSCTRL_BUS_PRIORITY_PROC1_MSB    _u(4)
#define BUSCTRL_BUS_PRIORITY_PROC1_LSB    _u(4)
#define BUSCTRL_BUS_PRIORITY_PROC1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : BUSCTRL_BUS_PRIORITY_PROC0
// Description : 0 - low priority, 1 - high priority
#define BUSCTRL_BUS_PRIORITY_PROC0_RESET  _u(0x0)
#define BUSCTRL_BUS_PRIORITY_PROC0_BITS   _u(0x00000001)
#define BUSCTRL_BUS_PRIORITY_PROC0_MSB    _u(0)
#define BUSCTRL_BUS_PRIORITY_PROC0_LSB    _u(0)
#define BUSCTRL_BUS_PRIORITY_PROC0_ACCESS "RW"
// =============================================================================
// Register    : BUSCTRL_BUS_PRIORITY_ACK
// Description : Bus priority acknowledge
//               Goes to 1 once all arbiters have registered the new global
//               priority levels.
//               Arbiters update their local priority when servicing a new
//               nonsequential access.
//               In normal circumstances this will happen almost immediately.
#define BUSCTRL_BUS_PRIORITY_ACK_OFFSET _u(0x00000004)
#define BUSCTRL_BUS_PRIORITY_ACK_BITS   _u(0x00000001)
#define BUSCTRL_BUS_PRIORITY_ACK_RESET  _u(0x00000000)
#define BUSCTRL_BUS_PRIORITY_ACK_MSB    _u(0)
#define BUSCTRL_BUS_PRIORITY_ACK_LSB    _u(0)
#define BUSCTRL_BUS_PRIORITY_ACK_ACCESS "RO"
// =============================================================================
// Register    : BUSCTRL_PERFCTR_EN
// Description : Enable the performance counters. If 0, the performance counters
//               do not increment. This can be used to precisely start/stop
//               event sampling around the profiled section of code.
//
//               The performance counters are initially disabled, to save
//               energy.
#define BUSCTRL_PERFCTR_EN_OFFSET _u(0x00000008)
#define BUSCTRL_PERFCTR_EN_BITS   _u(0x00000001)
#define BUSCTRL_PERFCTR_EN_RESET  _u(0x00000000)
#define BUSCTRL_PERFCTR_EN_MSB    _u(0)
#define BUSCTRL_PERFCTR_EN_LSB    _u(0)
#define BUSCTRL_PERFCTR_EN_ACCESS "RW"
// =============================================================================
// Register    : BUSCTRL_PERFCTR0
// Description : Bus fabric performance counter 0
//               Busfabric saturating performance counter 0
//               Count some event signal from the busfabric arbiters, if
//               PERFCTR_EN is set.
//               Write any value to clear. Select an event to count using
//               PERFSEL0
#define BUSCTRL_PERFCTR0_OFFSET _u(0x0000000c)
#define BUSCTRL_PERFCTR0_BITS   _u(0x00ffffff)
#define BUSCTRL_PERFCTR0_RESET  _u(0x00000000)
#define BUSCTRL_PERFCTR0_MSB    _u(23)
#define BUSCTRL_PERFCTR0_LSB    _u(0)
#define BUSCTRL_PERFCTR0_ACCESS "WC"
// =============================================================================
// Register    : BUSCTRL_PERFSEL0
// Description : Bus fabric performance event select for PERFCTR0
//               Select an event for PERFCTR0. For each downstream port of the
//               main crossbar, four events are available: ACCESS, an access
//               took place; ACCESS_CONTESTED, an access took place that
//               previously stalled due to contention from other masters;
//               STALL_DOWNSTREAM, count cycles where any master stalled due to
//               a stall on the downstream bus; STALL_UPSTREAM, count cycles
//               where any master stalled for any reason, including contention
//               from other masters.
//               0x00 -> siob_proc1_stall_upstream
//               0x01 -> siob_proc1_stall_downstream
//               0x02 -> siob_proc1_access_contested
//               0x03 -> siob_proc1_access
//               0x04 -> siob_proc0_stall_upstream
//               0x05 -> siob_proc0_stall_downstream
//               0x06 -> siob_proc0_access_contested
//               0x07 -> siob_proc0_access
//               0x08 -> apb_stall_upstream
//               0x09 -> apb_stall_downstream
//               0x0a -> apb_access_contested
//               0x0b -> apb_access
//               0x0c -> fastperi_stall_upstream
//               0x0d -> fastperi_stall_downstream
//               0x0e -> fastperi_access_contested
//               0x0f -> fastperi_access
//               0x10 -> sram9_stall_upstream
//               0x11 -> sram9_stall_downstream
//               0x12 -> sram9_access_contested
//               0x13 -> sram9_access
//               0x14 -> sram8_stall_upstream
//               0x15 -> sram8_stall_downstream
//               0x16 -> sram8_access_contested
//               0x17 -> sram8_access
//               0x18 -> sram7_stall_upstream
//               0x19 -> sram7_stall_downstream
//               0x1a -> sram7_access_contested
//               0x1b -> sram7_access
//               0x1c -> sram6_stall_upstream
//               0x1d -> sram6_stall_downstream
//               0x1e -> sram6_access_contested
//               0x1f -> sram6_access
//               0x20 -> sram5_stall_upstream
//               0x21 -> sram5_stall_downstream
//               0x22 -> sram5_access_contested
//               0x23 -> sram5_access
//               0x24 -> sram4_stall_upstream
//               0x25 -> sram4_stall_downstream
//               0x26 -> sram4_access_contested
//               0x27 -> sram4_access
//               0x28 -> sram3_stall_upstream
//               0x29 -> sram3_stall_downstream
//               0x2a -> sram3_access_contested
//               0x2b -> sram3_access
//               0x2c -> sram2_stall_upstream
//               0x2d -> sram2_stall_downstream
//               0x2e -> sram2_access_contested
//               0x2f -> sram2_access
//               0x30 -> sram1_stall_upstream
//               0x31 -> sram1_stall_downstream
//               0x32 -> sram1_access_contested
//               0x33 -> sram1_access
//               0x34 -> sram0_stall_upstream
//               0x35 -> sram0_stall_downstream
//               0x36 -> sram0_access_contested
//               0x37 -> sram0_access
//               0x38 -> xip_main1_stall_upstream
//               0x39 -> xip_main1_stall_downstream
//               0x3a -> xip_main1_access_contested
//               0x3b -> xip_main1_access
//               0x3c -> xip_main0_stall_upstream
//               0x3d -> xip_main0_stall_downstream
//               0x3e -> xip_main0_access_contested
//               0x3f -> xip_main0_access
//               0x40 -> rom_stall_upstream
//               0x41 -> rom_stall_downstream
//               0x42 -> rom_access_contested
//               0x43 -> rom_access
#define BUSCTRL_PERFSEL0_OFFSET _u(0x00000010)
#define BUSCTRL_PERFSEL0_BITS   _u(0x0000007f)
#define BUSCTRL_PERFSEL0_RESET  _u(0x0000001f)
#define BUSCTRL_PERFSEL0_MSB    _u(6)
#define BUSCTRL_PERFSEL0_LSB    _u(0)
#define BUSCTRL_PERFSEL0_ACCESS "RW"
#define BUSCTRL_PERFSEL0_VALUE_SIOB_PROC1_STALL_UPSTREAM _u(0x00)
#define BUSCTRL_PERFSEL0_VALUE_SIOB_PROC1_STALL_DOWNSTREAM _u(0x01)
#define BUSCTRL_PERFSEL0_VALUE_SIOB_PROC1_ACCESS_CONTESTED _u(0x02)
#define BUSCTRL_PERFSEL0_VALUE_SIOB_PROC1_ACCESS _u(0x03)
#define BUSCTRL_PERFSEL0_VALUE_SIOB_PROC0_STALL_UPSTREAM _u(0x04)
#define BUSCTRL_PERFSEL0_VALUE_SIOB_PROC0_STALL_DOWNSTREAM _u(0x05)
#define BUSCTRL_PERFSEL0_VALUE_SIOB_PROC0_ACCESS_CONTESTED _u(0x06)
#define BUSCTRL_PERFSEL0_VALUE_SIOB_PROC0_ACCESS _u(0x07)
#define BUSCTRL_PERFSEL0_VALUE_APB_STALL_UPSTREAM _u(0x08)
#define BUSCTRL_PERFSEL0_VALUE_APB_STALL_DOWNSTREAM _u(0x09)
#define BUSCTRL_PERFSEL0_VALUE_APB_ACCESS_CONTESTED _u(0x0a)
#define BUSCTRL_PERFSEL0_VALUE_APB_ACCESS _u(0x0b)
#define BUSCTRL_PERFSEL0_VALUE_FASTPERI_STALL_UPSTREAM _u(0x0c)
#define BUSCTRL_PERFSEL0_VALUE_FASTPERI_STALL_DOWNSTREAM _u(0x0d)
#define BUSCTRL_PERFSEL0_VALUE_FASTPERI_ACCESS_CONTESTED _u(0x0e)
#define BUSCTRL_PERFSEL0_VALUE_FASTPERI_ACCESS _u(0x0f)
#define BUSCTRL_PERFSEL0_VALUE_SRAM9_STALL_UPSTREAM _u(0x10)
#define BUSCTRL_PERFSEL0_VALUE_SRAM9_STALL_DOWNSTREAM _u(0x11)
#define BUSCTRL_PERFSEL0_VALUE_SRAM9_ACCESS_CONTESTED _u(0x12)
#define BUSCTRL_PERFSEL0_VALUE_SRAM9_ACCESS _u(0x13)
#define BUSCTRL_PERFSEL0_VALUE_SRAM8_STALL_UPSTREAM _u(0x14)
#define BUSCTRL_PERFSEL0_VALUE_SRAM8_STALL_DOWNSTREAM _u(0x15)
#define BUSCTRL_PERFSEL0_VALUE_SRAM8_ACCESS_CONTESTED _u(0x16)
#define BUSCTRL_PERFSEL0_VALUE_SRAM8_ACCESS _u(0x17)
#define BUSCTRL_PERFSEL0_VALUE_SRAM7_STALL_UPSTREAM _u(0x18)
#define BUSCTRL_PERFSEL0_VALUE_SRAM7_STALL_DOWNSTREAM _u(0x19)
#define BUSCTRL_PERFSEL0_VALUE_SRAM7_ACCESS_CONTESTED _u(0x1a)
#define BUSCTRL_PERFSEL0_VALUE_SRAM7_ACCESS _u(0x1b)
#define BUSCTRL_PERFSEL0_VALUE_SRAM6_STALL_UPSTREAM _u(0x1c)
#define BUSCTRL_PERFSEL0_VALUE_SRAM6_STALL_DOWNSTREAM _u(0x1d)
#define BUSCTRL_PERFSEL0_VALUE_SRAM6_ACCESS_CONTESTED _u(0x1e)
#define BUSCTRL_PERFSEL0_VALUE_SRAM6_ACCESS _u(0x1f)
#define BUSCTRL_PERFSEL0_VALUE_SRAM5_STALL_UPSTREAM _u(0x20)
#define BUSCTRL_PERFSEL0_VALUE_SRAM5_STALL_DOWNSTREAM _u(0x21)
#define BUSCTRL_PERFSEL0_VALUE_SRAM5_ACCESS_CONTESTED _u(0x22)
#define BUSCTRL_PERFSEL0_VALUE_SRAM5_ACCESS _u(0x23)
#define BUSCTRL_PERFSEL0_VALUE_SRAM4_STALL_UPSTREAM _u(0x24)
#define BUSCTRL_PERFSEL0_VALUE_SRAM4_STALL_DOWNSTREAM _u(0x25)
#define BUSCTRL_PERFSEL0_VALUE_SRAM4_ACCESS_CONTESTED _u(0x26)
#define BUSCTRL_PERFSEL0_VALUE_SRAM4_ACCESS _u(0x27)
#define BUSCTRL_PERFSEL0_VALUE_SRAM3_STALL_UPSTREAM _u(0x28)
#define BUSCTRL_PERFSEL0_VALUE_SRAM3_STALL_DOWNSTREAM _u(0x29)
#define BUSCTRL_PERFSEL0_VALUE_SRAM3_ACCESS_CONTESTED _u(0x2a)
#define BUSCTRL_PERFSEL0_VALUE_SRAM3_ACCESS _u(0x2b)
#define BUSCTRL_PERFSEL0_VALUE_SRAM2_STALL_UPSTREAM _u(0x2c)
#define BUSCTRL_PERFSEL0_VALUE_SRAM2_STALL_DOWNSTREAM _u(0x2d)
#define BUSCTRL_PERFSEL0_VALUE_SRAM2_ACCESS_CONTESTED _u(0x2e)
#define BUSCTRL_PERFSEL0_VALUE_SRAM2_ACCESS _u(0x2f)
#define BUSCTRL_PERFSEL0_VALUE_SRAM1_STALL_UPSTREAM _u(0x30)
#define BUSCTRL_PERFSEL0_VALUE_SRAM1_STALL_DOWNSTREAM _u(0x31)
#define BUSCTRL_PERFSEL0_VALUE_SRAM1_ACCESS_CONTESTED _u(0x32)
#define BUSCTRL_PERFSEL0_VALUE_SRAM1_ACCESS _u(0x33)
#define BUSCTRL_PERFSEL0_VALUE_SRAM0_STALL_UPSTREAM _u(0x34)
#define BUSCTRL_PERFSEL0_VALUE_SRAM0_STALL_DOWNSTREAM _u(0x35)
#define BUSCTRL_PERFSEL0_VALUE_SRAM0_ACCESS_CONTESTED _u(0x36)
#define BUSCTRL_PERFSEL0_VALUE_SRAM0_ACCESS _u(0x37)
#define BUSCTRL_PERFSEL0_VALUE_XIP_MAIN1_STALL_UPSTREAM _u(0x38)
#define BUSCTRL_PERFSEL0_VALUE_XIP_MAIN1_STALL_DOWNSTREAM _u(0x39)
#define BUSCTRL_PERFSEL0_VALUE_XIP_MAIN1_ACCESS_CONTESTED _u(0x3a)
#define BUSCTRL_PERFSEL0_VALUE_XIP_MAIN1_ACCESS _u(0x3b)
#define BUSCTRL_PERFSEL0_VALUE_XIP_MAIN0_STALL_UPSTREAM _u(0x3c)
#define BUSCTRL_PERFSEL0_VALUE_XIP_MAIN0_STALL_DOWNSTREAM _u(0x3d)
#define BUSCTRL_PERFSEL0_VALUE_XIP_MAIN0_ACCESS_CONTESTED _u(0x3e)
#define BUSCTRL_PERFSEL0_VALUE_XIP_MAIN0_ACCESS _u(0x3f)
#define BUSCTRL_PERFSEL0_VALUE_ROM_STALL_UPSTREAM _u(0x40)
#define BUSCTRL_PERFSEL0_VALUE_ROM_STALL_DOWNSTREAM _u(0x41)
#define BUSCTRL_PERFSEL0_VALUE_ROM_ACCESS_CONTESTED _u(0x42)
#define BUSCTRL_PERFSEL0_VALUE_ROM_ACCESS _u(0x43)
// =============================================================================
// Register    : BUSCTRL_PERFCTR1
// Description : Bus fabric performance counter 1
//               Busfabric saturating performance counter 1
//               Count some event signal from the busfabric arbiters, if
//               PERFCTR_EN is set.
//               Write any value to clear. Select an event to count using
//               PERFSEL1
#define BUSCTRL_PERFCTR1_OFFSET _u(0x00000014)
#define BUSCTRL_PERFCTR1_BITS   _u(0x00ffffff)
#define BUSCTRL_PERFCTR1_RESET  _u(0x00000000)
#define BUSCTRL_PERFCTR1_MSB    _u(23)
#define BUSCTRL_PERFCTR1_LSB    _u(0)
#define BUSCTRL_PERFCTR1_ACCESS "WC"
// =============================================================================
// Register    : BUSCTRL_PERFSEL1
// Description : Bus fabric performance event select for PERFCTR1
//               Select an event for PERFCTR1. For each downstream port of the
//               main crossbar, four events are available: ACCESS, an access
//               took place; ACCESS_CONTESTED, an access took place that
//               previously stalled due to contention from other masters;
//               STALL_DOWNSTREAM, count cycles where any master stalled due to
//               a stall on the downstream bus; STALL_UPSTREAM, count cycles
//               where any master stalled for any reason, including contention
//               from other masters.
//               0x00 -> siob_proc1_stall_upstream
//               0x01 -> siob_proc1_stall_downstream
//               0x02 -> siob_proc1_access_contested
//               0x03 -> siob_proc1_access
//               0x04 -> siob_proc0_stall_upstream
//               0x05 -> siob_proc0_stall_downstream
//               0x06 -> siob_proc0_access_contested
//               0x07 -> siob_proc0_access
//               0x08 -> apb_stall_upstream
//               0x09 -> apb_stall_downstream
//               0x0a -> apb_access_contested
//               0x0b -> apb_access
//               0x0c -> fastperi_stall_upstream
//               0x0d -> fastperi_stall_downstream
//               0x0e -> fastperi_access_contested
//               0x0f -> fastperi_access
//               0x10 -> sram9_stall_upstream
//               0x11 -> sram9_stall_downstream
//               0x12 -> sram9_access_contested
//               0x13 -> sram9_access
//               0x14 -> sram8_stall_upstream
//               0x15 -> sram8_stall_downstream
//               0x16 -> sram8_access_contested
//               0x17 -> sram8_access
//               0x18 -> sram7_stall_upstream
//               0x19 -> sram7_stall_downstream
//               0x1a -> sram7_access_contested
//               0x1b -> sram7_access
//               0x1c -> sram6_stall_upstream
//               0x1d -> sram6_stall_downstream
//               0x1e -> sram6_access_contested
//               0x1f -> sram6_access
//               0x20 -> sram5_stall_upstream
//               0x21 -> sram5_stall_downstream
//               0x22 -> sram5_access_contested
//               0x23 -> sram5_access
//               0x24 -> sram4_stall_upstream
//               0x25 -> sram4_stall_downstream
//               0x26 -> sram4_access_contested
//               0x27 -> sram4_access
//               0x28 -> sram3_stall_upstream
//               0x29 -> sram3_stall_downstream
//               0x2a -> sram3_access_contested
//               0x2b -> sram3_access
//               0x2c -> sram2_stall_upstream
//               0x2d -> sram2_stall_downstream
//               0x2e -> sram2_access_contested
//               0x2f -> sram2_access
//               0x30 -> sram1_stall_upstream
//               0x31 -> sram1_stall_downstream
//               0x32 -> sram1_access_contested
//               0x33 -> sram1_access
//               0x34 -> sram0_stall_upstream
//               0x35 -> sram0_stall_downstream
//               0x36 -> sram0_access_contested
//               0x37 -> sram0_access
//               0x38 -> xip_main1_stall_upstream
//               0x39 -> xip_main1_stall_downstream
//               0x3a -> xip_main1_access_contested
//               0x3b -> xip_main1_access
//               0x3c -> xip_main0_stall_upstream
//               0x3d -> xip_main0_stall_downstream
//               0x3e -> xip_main0_access_contested
//               0x3f -> xip_main0_access
//               0x40 -> rom_stall_upstream
//               0x41 -> rom_stall_downstream
//               0x42 -> rom_access_contested
//               0x43 -> rom_access
#define BUSCTRL_PERFSEL1_OFFSET _u(0x00000018)
#define BUSCTRL_PERFSEL1_BITS   _u(0x0000007f)
#define BUSCTRL_PERFSEL1_RESET  _u(0x0000001f)
#define BUSCTRL_PERFSEL1_MSB    _u(6)
#define BUSCTRL_PERFSEL1_LSB    _u(0)
#define BUSCTRL_PERFSEL1_ACCESS "RW"
#define BUSCTRL_PERFSEL1_VALUE_SIOB_PROC1_STALL_UPSTREAM _u(0x00)
#define BUSCTRL_PERFSEL1_VALUE_SIOB_PROC1_STALL_DOWNSTREAM _u(0x01)
#define BUSCTRL_PERFSEL1_VALUE_SIOB_PROC1_ACCESS_CONTESTED _u(0x02)
#define BUSCTRL_PERFSEL1_VALUE_SIOB_PROC1_ACCESS _u(0x03)
#define BUSCTRL_PERFSEL1_VALUE_SIOB_PROC0_STALL_UPSTREAM _u(0x04)
#define BUSCTRL_PERFSEL1_VALUE_SIOB_PROC0_STALL_DOWNSTREAM _u(0x05)
#define BUSCTRL_PERFSEL1_VALUE_SIOB_PROC0_ACCESS_CONTESTED _u(0x06)
#define BUSCTRL_PERFSEL1_VALUE_SIOB_PROC0_ACCESS _u(0x07)
#define BUSCTRL_PERFSEL1_VALUE_APB_STALL_UPSTREAM _u(0x08)
#define BUSCTRL_PERFSEL1_VALUE_APB_STALL_DOWNSTREAM _u(0x09)
#define BUSCTRL_PERFSEL1_VALUE_APB_ACCESS_CONTESTED _u(0x0a)
#define BUSCTRL_PERFSEL1_VALUE_APB_ACCESS _u(0x0b)
#define BUSCTRL_PERFSEL1_VALUE_FASTPERI_STALL_UPSTREAM _u(0x0c)
#define BUSCTRL_PERFSEL1_VALUE_FASTPERI_STALL_DOWNSTREAM _u(0x0d)
#define BUSCTRL_PERFSEL1_VALUE_FASTPERI_ACCESS_CONTESTED _u(0x0e)
#define BUSCTRL_PERFSEL1_VALUE_FASTPERI_ACCESS _u(0x0f)
#define BUSCTRL_PERFSEL1_VALUE_SRAM9_STALL_UPSTREAM _u(0x10)
#define BUSCTRL_PERFSEL1_VALUE_SRAM9_STALL_DOWNSTREAM _u(0x11)
#define BUSCTRL_PERFSEL1_VALUE_SRAM9_ACCESS_CONTESTED _u(0x12)
#define BUSCTRL_PERFSEL1_VALUE_SRAM9_ACCESS _u(0x13)
#define BUSCTRL_PERFSEL1_VALUE_SRAM8_STALL_UPSTREAM _u(0x14)
#define BUSCTRL_PERFSEL1_VALUE_SRAM8_STALL_DOWNSTREAM _u(0x15)
#define BUSCTRL_PERFSEL1_VALUE_SRAM8_ACCESS_CONTESTED _u(0x16)
#define BUSCTRL_PERFSEL1_VALUE_SRAM8_ACCESS _u(0x17)
#define BUSCTRL_PERFSEL1_VALUE_SRAM7_STALL_UPSTREAM _u(0x18)
#define BUSCTRL_PERFSEL1_VALUE_SRAM7_STALL_DOWNSTREAM _u(0x19)
#define BUSCTRL_PERFSEL1_VALUE_SRAM7_ACCESS_CONTESTED _u(0x1a)
#define BUSCTRL_PERFSEL1_VALUE_SRAM7_ACCESS _u(0x1b)
#define BUSCTRL_PERFSEL1_VALUE_SRAM6_STALL_UPSTREAM _u(0x1c)
#define BUSCTRL_PERFSEL1_VALUE_SRAM6_STALL_DOWNSTREAM _u(0x1d)
#define BUSCTRL_PERFSEL1_VALUE_SRAM6_ACCESS_CONTESTED _u(0x1e)
#define BUSCTRL_PERFSEL1_VALUE_SRAM6_ACCESS _u(0x1f)
#define BUSCTRL_PERFSEL1_VALUE_SRAM5_STALL_UPSTREAM _u(0x20)
#define BUSCTRL_PERFSEL1_VALUE_SRAM5_STALL_DOWNSTREAM _u(0x21)
#define BUSCTRL_PERFSEL1_VALUE_SRAM5_ACCESS_CONTESTED _u(0x22)
#define BUSCTRL_PERFSEL1_VALUE_SRAM5_ACCESS _u(0x23)
#define BUSCTRL_PERFSEL1_VALUE_SRAM4_STALL_UPSTREAM _u(0x24)
#define BUSCTRL_PERFSEL1_VALUE_SRAM4_STALL_DOWNSTREAM _u(0x25)
#define BUSCTRL_PERFSEL1_VALUE_SRAM4_ACCESS_CONTESTED _u(0x26)
#define BUSCTRL_PERFSEL1_VALUE_SRAM4_ACCESS _u(0x27)
#define BUSCTRL_PERFSEL1_VALUE_SRAM3_STALL_UPSTREAM _u(0x28)
#define BUSCTRL_PERFSEL1_VALUE_SRAM3_STALL_DOWNSTREAM _u(0x29)
#define BUSCTRL_PERFSEL1_VALUE_SRAM3_ACCESS_CONTESTED _u(0x2a)
#define BUSCTRL_PERFSEL1_VALUE_SRAM3_ACCESS _u(0x2b)
#define BUSCTRL_PERFSEL1_VALUE_SRAM2_STALL_UPSTREAM _u(0x2c)
#define BUSCTRL_PERFSEL1_VALUE_SRAM2_STALL_DOWNSTREAM _u(0x2d)
#define BUSCTRL_PERFSEL1_VALUE_SRAM2_ACCESS_CONTESTED _u(0x2e)
#define BUSCTRL_PERFSEL1_VALUE_SRAM2_ACCESS _u(0x2f)
#define BUSCTRL_PERFSEL1_VALUE_SRAM1_STALL_UPSTREAM _u(0x30)
#define BUSCTRL_PERFSEL1_VALUE_SRAM1_STALL_DOWNSTREAM _u(0x31)
#define BUSCTRL_PERFSEL1_VALUE_SRAM1_ACCESS_CONTESTED _u(0x32)
#define BUSCTRL_PERFSEL1_VALUE_SRAM1_ACCESS _u(0x33)
#define BUSCTRL_PERFSEL1_VALUE_SRAM0_STALL_UPSTREAM _u(0x34)
#define BUSCTRL_PERFSEL1_VALUE_SRAM0_STALL_DOWNSTREAM _u(0x35)
#define BUSCTRL_PERFSEL1_VALUE_SRAM0_ACCESS_CONTESTED _u(0x36)
#define BUSCTRL_PERFSEL1_VALUE_SRAM0_ACCESS _u(0x37)
#define BUSCTRL_PERFSEL1_VALUE_XIP_MAIN1_STALL_UPSTREAM _u(0x38)
#define BUSCTRL_PERFSEL1_VALUE_XIP_MAIN1_STALL_DOWNSTREAM _u(0x39)
#define BUSCTRL_PERFSEL1_VALUE_XIP_MAIN1_ACCESS_CONTESTED _u(0x3a)
#define BUSCTRL_PERFSEL1_VALUE_XIP_MAIN1_ACCESS _u(0x3b)
#define BUSCTRL_PERFSEL1_VALUE_XIP_MAIN0_STALL_UPSTREAM _u(0x3c)
#define BUSCTRL_PERFSEL1_VALUE_XIP_MAIN0_STALL_DOWNSTREAM _u(0x3d)
#define BUSCTRL_PERFSEL1_VALUE_XIP_MAIN0_ACCESS_CONTESTED _u(0x3e)
#define BUSCTRL_PERFSEL1_VALUE_XIP_MAIN0_ACCESS _u(0x3f)
#define BUSCTRL_PERFSEL1_VALUE_ROM_STALL_UPSTREAM _u(0x40)
#define BUSCTRL_PERFSEL1_VALUE_ROM_STALL_DOWNSTREAM _u(0x41)
#define BUSCTRL_PERFSEL1_VALUE_ROM_ACCESS_CONTESTED _u(0x42)
#define BUSCTRL_PERFSEL1_VALUE_ROM_ACCESS _u(0x43)
// =============================================================================
// Register    : BUSCTRL_PERFCTR2
// Description : Bus fabric performance counter 2
//               Busfabric saturating performance counter 2
//               Count some event signal from the busfabric arbiters, if
//               PERFCTR_EN is set.
//               Write any value to clear. Select an event to count using
//               PERFSEL2
#define BUSCTRL_PERFCTR2_OFFSET _u(0x0000001c)
#define BUSCTRL_PERFCTR2_BITS   _u(0x00ffffff)
#define BUSCTRL_PERFCTR2_RESET  _u(0x00000000)
#define BUSCTRL_PERFCTR2_MSB    _u(23)
#define BUSCTRL_PERFCTR2_LSB    _u(0)
#define BUSCTRL_PERFCTR2_ACCESS "WC"
// =============================================================================
// Register    : BUSCTRL_PERFSEL2
// Description : Bus fabric performance event select for PERFCTR2
//               Select an event for PERFCTR2. For each downstream port of the
//               main crossbar, four events are available: ACCESS, an access
//               took place; ACCESS_CONTESTED, an access took place that
//               previously stalled due to contention from other masters;
//               STALL_DOWNSTREAM, count cycles where any master stalled due to
//               a stall on the downstream bus; STALL_UPSTREAM, count cycles
//               where any master stalled for any reason, including contention
//               from other masters.
//               0x00 -> siob_proc1_stall_upstream
//               0x01 -> siob_proc1_stall_downstream
//               0x02 -> siob_proc1_access_contested
//               0x03 -> siob_proc1_access
//               0x04 -> siob_proc0_stall_upstream
//               0x05 -> siob_proc0_stall_downstream
//               0x06 -> siob_proc0_access_contested
//               0x07 -> siob_proc0_access
//               0x08 -> apb_stall_upstream
//               0x09 -> apb_stall_downstream
//               0x0a -> apb_access_contested
//               0x0b -> apb_access
//               0x0c -> fastperi_stall_upstream
//               0x0d -> fastperi_stall_downstream
//               0x0e -> fastperi_access_contested
//               0x0f -> fastperi_access
//               0x10 -> sram9_stall_upstream
//               0x11 -> sram9_stall_downstream
//               0x12 -> sram9_access_contested
//               0x13 -> sram9_access
//               0x14 -> sram8_stall_upstream
//               0x15 -> sram8_stall_downstream
//               0x16 -> sram8_access_contested
//               0x17 -> sram8_access
//               0x18 -> sram7_stall_upstream
//               0x19 -> sram7_stall_downstream
//               0x1a -> sram7_access_contested
//               0x1b -> sram7_access
//               0x1c -> sram6_stall_upstream
//               0x1d -> sram6_stall_downstream
//               0x1e -> sram6_access_contested
//               0x1f -> sram6_access
//               0x20 -> sram5_stall_upstream
//               0x21 -> sram5_stall_downstream
//               0x22 -> sram5_access_contested
//               0x23 -> sram5_access
//               0x24 -> sram4_stall_upstream
//               0x25 -> sram4_stall_downstream
//               0x26 -> sram4_access_contested
//               0x27 -> sram4_access
//               0x28 -> sram3_stall_upstream
//               0x29 -> sram3_stall_downstream
//               0x2a -> sram3_access_contested
//               0x2b -> sram3_access
//               0x2c -> sram2_stall_upstream
//               0x2d -> sram2_stall_downstream
//               0x2e -> sram2_access_contested
//               0x2f -> sram2_access
//               0x30 -> sram1_stall_upstream
//               0x31 -> sram1_stall_downstream
//               0x32 -> sram1_access_contested
//               0x33 -> sram1_access
//               0x34 -> sram0_stall_upstream
//               0x35 -> sram0_stall_downstream
//               0x36 -> sram0_access_contested
//               0x37 -> sram0_access
//               0x38 -> xip_main1_stall_upstream
//               0x39 -> xip_main1_stall_downstream
//               0x3a -> xip_main1_access_contested
//               0x3b -> xip_main1_access
//               0x3c -> xip_main0_stall_upstream
//               0x3d -> xip_main0_stall_downstream
//               0x3e -> xip_main0_access_contested
//               0x3f -> xip_main0_access
//               0x40 -> rom_stall_upstream
//               0x41 -> rom_stall_downstream
//               0x42 -> rom_access_contested
//               0x43 -> rom_access
#define BUSCTRL_PERFSEL2_OFFSET _u(0x00000020)
#define BUSCTRL_PERFSEL2_BITS   _u(0x0000007f)
#define BUSCTRL_PERFSEL2_RESET  _u(0x0000001f)
#define BUSCTRL_PERFSEL2_MSB    _u(6)
#define BUSCTRL_PERFSEL2_LSB    _u(0)
#define BUSCTRL_PERFSEL2_ACCESS "RW"
#define BUSCTRL_PERFSEL2_VALUE_SIOB_PROC1_STALL_UPSTREAM _u(0x00)
#define BUSCTRL_PERFSEL2_VALUE_SIOB_PROC1_STALL_DOWNSTREAM _u(0x01)
#define BUSCTRL_PERFSEL2_VALUE_SIOB_PROC1_ACCESS_CONTESTED _u(0x02)
#define BUSCTRL_PERFSEL2_VALUE_SIOB_PROC1_ACCESS _u(0x03)
#define BUSCTRL_PERFSEL2_VALUE_SIOB_PROC0_STALL_UPSTREAM _u(0x04)
#define BUSCTRL_PERFSEL2_VALUE_SIOB_PROC0_STALL_DOWNSTREAM _u(0x05)
#define BUSCTRL_PERFSEL2_VALUE_SIOB_PROC0_ACCESS_CONTESTED _u(0x06)
#define BUSCTRL_PERFSEL2_VALUE_SIOB_PROC0_ACCESS _u(0x07)
#define BUSCTRL_PERFSEL2_VALUE_APB_STALL_UPSTREAM _u(0x08)
#define BUSCTRL_PERFSEL2_VALUE_APB_STALL_DOWNSTREAM _u(0x09)
#define BUSCTRL_PERFSEL2_VALUE_APB_ACCESS_CONTESTED _u(0x0a)
#define BUSCTRL_PERFSEL2_VALUE_APB_ACCESS _u(0x0b)
#define BUSCTRL_PERFSEL2_VALUE_FASTPERI_STALL_UPSTREAM _u(0x0c)
#define BUSCTRL_PERFSEL2_VALUE_FASTPERI_STALL_DOWNSTREAM _u(0x0d)
#define BUSCTRL_PERFSEL2_VALUE_FASTPERI_ACCESS_CONTESTED _u(0x0e)
#define BUSCTRL_PERFSEL2_VALUE_FASTPERI_ACCESS _u(0x0f)
#define BUSCTRL_PERFSEL2_VALUE_SRAM9_STALL_UPSTREAM _u(0x10)
#define BUSCTRL_PERFSEL2_VALUE_SRAM9_STALL_DOWNSTREAM _u(0x11)
#define BUSCTRL_PERFSEL2_VALUE_SRAM9_ACCESS_CONTESTED _u(0x12)
#define BUSCTRL_PERFSEL2_VALUE_SRAM9_ACCESS _u(0x13)
#define BUSCTRL_PERFSEL2_VALUE_SRAM8_STALL_UPSTREAM _u(0x14)
#define BUSCTRL_PERFSEL2_VALUE_SRAM8_STALL_DOWNSTREAM _u(0x15)
#define BUSCTRL_PERFSEL2_VALUE_SRAM8_ACCESS_CONTESTED _u(0x16)
#define BUSCTRL_PERFSEL2_VALUE_SRAM8_ACCESS _u(0x17)
#define BUSCTRL_PERFSEL2_VALUE_SRAM7_STALL_UPSTREAM _u(0x18)
#define BUSCTRL_PERFSEL2_VALUE_SRAM7_STALL_DOWNSTREAM _u(0x19)
#define BUSCTRL_PERFSEL2_VALUE_SRAM7_ACCESS_CONTESTED _u(0x1a)
#define BUSCTRL_PERFSEL2_VALUE_SRAM7_ACCESS _u(0x1b)
#define BUSCTRL_PERFSEL2_VALUE_SRAM6_STALL_UPSTREAM _u(0x1c)
#define BUSCTRL_PERFSEL2_VALUE_SRAM6_STALL_DOWNSTREAM _u(0x1d)
#define BUSCTRL_PERFSEL2_VALUE_SRAM6_ACCESS_CONTESTED _u(0x1e)
#define BUSCTRL_PERFSEL2_VALUE_SRAM6_ACCESS _u(0x1f)
#define BUSCTRL_PERFSEL2_VALUE_SRAM5_STALL_UPSTREAM _u(0x20)
#define BUSCTRL_PERFSEL2_VALUE_SRAM5_STALL_DOWNSTREAM _u(0x21)
#define BUSCTRL_PERFSEL2_VALUE_SRAM5_ACCESS_CONTESTED _u(0x22)
#define BUSCTRL_PERFSEL2_VALUE_SRAM5_ACCESS _u(0x23)
#define BUSCTRL_PERFSEL2_VALUE_SRAM4_STALL_UPSTREAM _u(0x24)
#define BUSCTRL_PERFSEL2_VALUE_SRAM4_STALL_DOWNSTREAM _u(0x25)
#define BUSCTRL_PERFSEL2_VALUE_SRAM4_ACCESS_CONTESTED _u(0x26)
#define BUSCTRL_PERFSEL2_VALUE_SRAM4_ACCESS _u(0x27)
#define BUSCTRL_PERFSEL2_VALUE_SRAM3_STALL_UPSTREAM _u(0x28)
#define BUSCTRL_PERFSEL2_VALUE_SRAM3_STALL_DOWNSTREAM _u(0x29)
#define BUSCTRL_PERFSEL2_VALUE_SRAM3_ACCESS_CONTESTED _u(0x2a)
#define BUSCTRL_PERFSEL2_VALUE_SRAM3_ACCESS _u(0x2b)
#define BUSCTRL_PERFSEL2_VALUE_SRAM2_STALL_UPSTREAM _u(0x2c)
#define BUSCTRL_PERFSEL2_VALUE_SRAM2_STALL_DOWNSTREAM _u(0x2d)
#define BUSCTRL_PERFSEL2_VALUE_SRAM2_ACCESS_CONTESTED _u(0x2e)
#define BUSCTRL_PERFSEL2_VALUE_SRAM2_ACCESS _u(0x2f)
#define BUSCTRL_PERFSEL2_VALUE_SRAM1_STALL_UPSTREAM _u(0x30)
#define BUSCTRL_PERFSEL2_VALUE_SRAM1_STALL_DOWNSTREAM _u(0x31)
#define BUSCTRL_PERFSEL2_VALUE_SRAM1_ACCESS_CONTESTED _u(0x32)
#define BUSCTRL_PERFSEL2_VALUE_SRAM1_ACCESS _u(0x33)
#define BUSCTRL_PERFSEL2_VALUE_SRAM0_STALL_UPSTREAM _u(0x34)
#define BUSCTRL_PERFSEL2_VALUE_SRAM0_STALL_DOWNSTREAM _u(0x35)
#define BUSCTRL_PERFSEL2_VALUE_SRAM0_ACCESS_CONTESTED _u(0x36)
#define BUSCTRL_PERFSEL2_VALUE_SRAM0_ACCESS _u(0x37)
#define BUSCTRL_PERFSEL2_VALUE_XIP_MAIN1_STALL_UPSTREAM _u(0x38)
#define BUSCTRL_PERFSEL2_VALUE_XIP_MAIN1_STALL_DOWNSTREAM _u(0x39)
#define BUSCTRL_PERFSEL2_VALUE_XIP_MAIN1_ACCESS_CONTESTED _u(0x3a)
#define BUSCTRL_PERFSEL2_VALUE_XIP_MAIN1_ACCESS _u(0x3b)
#define BUSCTRL_PERFSEL2_VALUE_XIP_MAIN0_STALL_UPSTREAM _u(0x3c)
#define BUSCTRL_PERFSEL2_VALUE_XIP_MAIN0_STALL_DOWNSTREAM _u(0x3d)
#define BUSCTRL_PERFSEL2_VALUE_XIP_MAIN0_ACCESS_CONTESTED _u(0x3e)
#define BUSCTRL_PERFSEL2_VALUE_XIP_MAIN0_ACCESS _u(0x3f)
#define BUSCTRL_PERFSEL2_VALUE_ROM_STALL_UPSTREAM _u(0x40)
#define BUSCTRL_PERFSEL2_VALUE_ROM_STALL_DOWNSTREAM _u(0x41)
#define BUSCTRL_PERFSEL2_VALUE_ROM_ACCESS_CONTESTED _u(0x42)
#define BUSCTRL_PERFSEL2_VALUE_ROM_ACCESS _u(0x43)
// =============================================================================
// Register    : BUSCTRL_PERFCTR3
// Description : Bus fabric performance counter 3
//               Busfabric saturating performance counter 3
//               Count some event signal from the busfabric arbiters, if
//               PERFCTR_EN is set.
//               Write any value to clear. Select an event to count using
//               PERFSEL3
#define BUSCTRL_PERFCTR3_OFFSET _u(0x00000024)
#define BUSCTRL_PERFCTR3_BITS   _u(0x00ffffff)
#define BUSCTRL_PERFCTR3_RESET  _u(0x00000000)
#define BUSCTRL_PERFCTR3_MSB    _u(23)
#define BUSCTRL_PERFCTR3_LSB    _u(0)
#define BUSCTRL_PERFCTR3_ACCESS "WC"
// =============================================================================
// Register    : BUSCTRL_PERFSEL3
// Description : Bus fabric performance event select for PERFCTR3
//               Select an event for PERFCTR3. For each downstream port of the
//               main crossbar, four events are available: ACCESS, an access
//               took place; ACCESS_CONTESTED, an access took place that
//               previously stalled due to contention from other masters;
//               STALL_DOWNSTREAM, count cycles where any master stalled due to
//               a stall on the downstream bus; STALL_UPSTREAM, count cycles
//               where any master stalled for any reason, including contention
//               from other masters.
//               0x00 -> siob_proc1_stall_upstream
//               0x01 -> siob_proc1_stall_downstream
//               0x02 -> siob_proc1_access_contested
//               0x03 -> siob_proc1_access
//               0x04 -> siob_proc0_stall_upstream
//               0x05 -> siob_proc0_stall_downstream
//               0x06 -> siob_proc0_access_contested
//               0x07 -> siob_proc0_access
//               0x08 -> apb_stall_upstream
//               0x09 -> apb_stall_downstream
//               0x0a -> apb_access_contested
//               0x0b -> apb_access
//               0x0c -> fastperi_stall_upstream
//               0x0d -> fastperi_stall_downstream
//               0x0e -> fastperi_access_contested
//               0x0f -> fastperi_access
//               0x10 -> sram9_stall_upstream
//               0x11 -> sram9_stall_downstream
//               0x12 -> sram9_access_contested
//               0x13 -> sram9_access
//               0x14 -> sram8_stall_upstream
//               0x15 -> sram8_stall_downstream
//               0x16 -> sram8_access_contested
//               0x17 -> sram8_access
//               0x18 -> sram7_stall_upstream
//               0x19 -> sram7_stall_downstream
//               0x1a -> sram7_access_contested
//               0x1b -> sram7_access
//               0x1c -> sram6_stall_upstream
//               0x1d -> sram6_stall_downstream
//               0x1e -> sram6_access_contested
//               0x1f -> sram6_access
//               0x20 -> sram5_stall_upstream
//               0x21 -> sram5_stall_downstream
//               0x22 -> sram5_access_contested
//               0x23 -> sram5_access
//               0x24 -> sram4_stall_upstream
//               0x25 -> sram4_stall_downstream
//               0x26 -> sram4_access_contested
//               0x27 -> sram4_access
//               0x28 -> sram3_stall_upstream
//               0x29 -> sram3_stall_downstream
//               0x2a -> sram3_access_contested
//               0x2b -> sram3_access
//               0x2c -> sram2_stall_upstream
//               0x2d -> sram2_stall_downstream
//               0x2e -> sram2_access_contested
//               0x2f -> sram2_access
//               0x30 -> sram1_stall_upstream
//               0x31 -> sram1_stall_downstream
//               0x32 -> sram1_access_contested
//               0x33 -> sram1_access
//               0x34 -> sram0_stall_upstream
//               0x35 -> sram0_stall_downstream
//               0x36 -> sram0_access_contested
//               0x37 -> sram0_access
//               0x38 -> xip_main1_stall_upstream
//               0x39 -> xip_main1_stall_downstream
//               0x3a -> xip_main1_access_contested
//               0x3b -> xip_main1_access
//               0x3c -> xip_main0_stall_upstream
//               0x3d -> xip_main0_stall_downstream
//               0x3e -> xip_main0_access_contested
//               0x3f -> xip_main0_access
//               0x40 -> rom_stall_upstream
//               0x41 -> rom_stall_downstream
//               0x42 -> rom_access_contested
//               0x43 -> rom_access
#define BUSCTRL_PERFSEL3_OFFSET _u(0x00000028)
#define BUSCTRL_PERFSEL3_BITS   _u(0x0000007f)
#define BUSCTRL_PERFSEL3_RESET  _u(0x0000001f)
#define BUSCTRL_PERFSEL3_MSB    _u(6)
#define BUSCTRL_PERFSEL3_LSB    _u(0)
#define BUSCTRL_PERFSEL3_ACCESS "RW"
#define BUSCTRL_PERFSEL3_VALUE_SIOB_PROC1_STALL_UPSTREAM _u(0x00)
#define BUSCTRL_PERFSEL3_VALUE_SIOB_PROC1_STALL_DOWNSTREAM _u(0x01)
#define BUSCTRL_PERFSEL3_VALUE_SIOB_PROC1_ACCESS_CONTESTED _u(0x02)
#define BUSCTRL_PERFSEL3_VALUE_SIOB_PROC1_ACCESS _u(0x03)
#define BUSCTRL_PERFSEL3_VALUE_SIOB_PROC0_STALL_UPSTREAM _u(0x04)
#define BUSCTRL_PERFSEL3_VALUE_SIOB_PROC0_STALL_DOWNSTREAM _u(0x05)
#define BUSCTRL_PERFSEL3_VALUE_SIOB_PROC0_ACCESS_CONTESTED _u(0x06)
#define BUSCTRL_PERFSEL3_VALUE_SIOB_PROC0_ACCESS _u(0x07)
#define BUSCTRL_PERFSEL3_VALUE_APB_STALL_UPSTREAM _u(0x08)
#define BUSCTRL_PERFSEL3_VALUE_APB_STALL_DOWNSTREAM _u(0x09)
#define BUSCTRL_PERFSEL3_VALUE_APB_ACCESS_CONTESTED _u(0x0a)
#define BUSCTRL_PERFSEL3_VALUE_APB_ACCESS _u(0x0b)
#define BUSCTRL_PERFSEL3_VALUE_FASTPERI_STALL_UPSTREAM _u(0x0c)
#define BUSCTRL_PERFSEL3_VALUE_FASTPERI_STALL_DOWNSTREAM _u(0x0d)
#define BUSCTRL_PERFSEL3_VALUE_FASTPERI_ACCESS_CONTESTED _u(0x0e)
#define BUSCTRL_PERFSEL3_VALUE_FASTPERI_ACCESS _u(0x0f)
#define BUSCTRL_PERFSEL3_VALUE_SRAM9_STALL_UPSTREAM _u(0x10)
#define BUSCTRL_PERFSEL3_VALUE_SRAM9_STALL_DOWNSTREAM _u(0x11)
#define BUSCTRL_PERFSEL3_VALUE_SRAM9_ACCESS_CONTESTED _u(0x12)
#define BUSCTRL_PERFSEL3_VALUE_SRAM9_ACCESS _u(0x13)
#define BUSCTRL_PERFSEL3_VALUE_SRAM8_STALL_UPSTREAM _u(0x14)
#define BUSCTRL_PERFSEL3_VALUE_SRAM8_STALL_DOWNSTREAM _u(0x15)
#define BUSCTRL_PERFSEL3_VALUE_SRAM8_ACCESS_CONTESTED _u(0x16)
#define BUSCTRL_PERFSEL3_VALUE_SRAM8_ACCESS _u(0x17)
#define BUSCTRL_PERFSEL3_VALUE_SRAM7_STALL_UPSTREAM _u(0x18)
#define BUSCTRL_PERFSEL3_VALUE_SRAM7_STALL_DOWNSTREAM _u(0x19)
#define BUSCTRL_PERFSEL3_VALUE_SRAM7_ACCESS_CONTESTED _u(0x1a)
#define BUSCTRL_PERFSEL3_VALUE_SRAM7_ACCESS _u(0x1b)
#define BUSCTRL_PERFSEL3_VALUE_SRAM6_STALL_UPSTREAM _u(0x1c)
#define BUSCTRL_PERFSEL3_VALUE_SRAM6_STALL_DOWNSTREAM _u(0x1d)
#define BUSCTRL_PERFSEL3_VALUE_SRAM6_ACCESS_CONTESTED _u(0x1e)
#define BUSCTRL_PERFSEL3_VALUE_SRAM6_ACCESS _u(0x1f)
#define BUSCTRL_PERFSEL3_VALUE_SRAM5_STALL_UPSTREAM _u(0x20)
#define BUSCTRL_PERFSEL3_VALUE_SRAM5_STALL_DOWNSTREAM _u(0x21)
#define BUSCTRL_PERFSEL3_VALUE_SRAM5_ACCESS_CONTESTED _u(0x22)
#define BUSCTRL_PERFSEL3_VALUE_SRAM5_ACCESS _u(0x23)
#define BUSCTRL_PERFSEL3_VALUE_SRAM4_STALL_UPSTREAM _u(0x24)
#define BUSCTRL_PERFSEL3_VALUE_SRAM4_STALL_DOWNSTREAM _u(0x25)
#define BUSCTRL_PERFSEL3_VALUE_SRAM4_ACCESS_CONTESTED _u(0x26)
#define BUSCTRL_PERFSEL3_VALUE_SRAM4_ACCESS _u(0x27)
#define BUSCTRL_PERFSEL3_VALUE_SRAM3_STALL_UPSTREAM _u(0x28)
#define BUSCTRL_PERFSEL3_VALUE_SRAM3_STALL_DOWNSTREAM _u(0x29)
#define BUSCTRL_PERFSEL3_VALUE_SRAM3_ACCESS_CONTESTED _u(0x2a)
#define BUSCTRL_PERFSEL3_VALUE_SRAM3_ACCESS _u(0x2b)
#define BUSCTRL_PERFSEL3_VALUE_SRAM2_STALL_UPSTREAM _u(0x2c)
#define BUSCTRL_PERFSEL3_VALUE_SRAM2_STALL_DOWNSTREAM _u(0x2d)
#define BUSCTRL_PERFSEL3_VALUE_SRAM2_ACCESS_CONTESTED _u(0x2e)
#define BUSCTRL_PERFSEL3_VALUE_SRAM2_ACCESS _u(0x2f)
#define BUSCTRL_PERFSEL3_VALUE_SRAM1_STALL_UPSTREAM _u(0x30)
#define BUSCTRL_PERFSEL3_VALUE_SRAM1_STALL_DOWNSTREAM _u(0x31)
#define BUSCTRL_PERFSEL3_VALUE_SRAM1_ACCESS_CONTESTED _u(0x32)
#define BUSCTRL_PERFSEL3_VALUE_SRAM1_ACCESS _u(0x33)
#define BUSCTRL_PERFSEL3_VALUE_SRAM0_STALL_UPSTREAM _u(0x34)
#define BUSCTRL_PERFSEL3_VALUE_SRAM0_STALL_DOWNSTREAM _u(0x35)
#define BUSCTRL_PERFSEL3_VALUE_SRAM0_ACCESS_CONTESTED _u(0x36)
#define BUSCTRL_PERFSEL3_VALUE_SRAM0_ACCESS _u(0x37)
#define BUSCTRL_PERFSEL3_VALUE_XIP_MAIN1_STALL_UPSTREAM _u(0x38)
#define BUSCTRL_PERFSEL3_VALUE_XIP_MAIN1_STALL_DOWNSTREAM _u(0x39)
#define BUSCTRL_PERFSEL3_VALUE_XIP_MAIN1_ACCESS_CONTESTED _u(0x3a)
#define BUSCTRL_PERFSEL3_VALUE_XIP_MAIN1_ACCESS _u(0x3b)
#define BUSCTRL_PERFSEL3_VALUE_XIP_MAIN0_STALL_UPSTREAM _u(0x3c)
#define BUSCTRL_PERFSEL3_VALUE_XIP_MAIN0_STALL_DOWNSTREAM _u(0x3d)
#define BUSCTRL_PERFSEL3_VALUE_XIP_MAIN0_ACCESS_CONTESTED _u(0x3e)
#define BUSCTRL_PERFSEL3_VALUE_XIP_MAIN0_ACCESS _u(0x3f)
#define BUSCTRL_PERFSEL3_VALUE_ROM_STALL_UPSTREAM _u(0x40)
#define BUSCTRL_PERFSEL3_VALUE_ROM_STALL_DOWNSTREAM _u(0x41)
#define BUSCTRL_PERFSEL3_VALUE_ROM_ACCESS_CONTESTED _u(0x42)
#define BUSCTRL_PERFSEL3_VALUE_ROM_ACCESS _u(0x43)
// =============================================================================
#endif // _HARDWARE_REGS_BUSCTRL_H

