// Copyright (C) 2020  Elias Bakken <elias@iagent.no>
// This file may be distributed under the terms of the GNU GPLv3 license.

#ifndef _PRCM_H
#define _PRCM_H

#define R_PRCM_BASE               0x01F01400
#define CPUS_CLK_REG              R_PRCM_BASE + 0x0000 // CPUS Clock Register
#define APB0_CFG_REG              R_PRCM_BASE + 0x000C // APB0 Configuration Register
#define APB0_CLK_GATING_REG       R_PRCM_BASE + 0x0028 // APB0 Clock Gating Register
#define PLL_CTRL_REG1             R_PRCM_BASE + 0x0044 // PLL Control Register 1
#define R_CIR_RX_CLK_REG          R_PRCM_BASE + 0x0054 // R_CIR_RX Clock Register
#define APB0_SOFT_RST_REG         R_PRCM_BASE + 0x00B0 // APB0 Software Reset Register
#define C0CPUX_PWROFF_GATING_REG  R_PRCM_BASE + 0x0100 // Cluster0 CPUX Power Off Gating Register
#define C1CPUX_PWROFF_GATING_REG  R_PRCM_BASE + 0x0104 // Cluster1 CPUX Power Off Gating Register
#define VDD_SYS_PWROFF_GATING_REG R_PRCM_BASE + 0x0110 // VDD_SYS Power Off Gating Register
#define GPU_PWROFF_GATING_REG     R_PRCM_BASE + 0x0118 // GPU Power Off Gating Register
#define VDD_SYS_PWROFF_RST_REG    R_PRCM_BASE + 0x0120 // VDD_SYS Power Domain Reset Register
#define C0_CPU0_PWR_SWITCH_CTRL   R_PRCM_BASE + 0x0140 // C0_CPU0 Power Switch Control Register
#define C0_CPU1_PWR_SWITCH_CTRL   R_PRCM_BASE + 0x0144 // C0_CPU1 Power Switch Control Register
#define C0_CPU2_PWR_SWITCH_CTRL   R_PRCM_BASE + 0x0148 // C0_CPU2 Power Switch Control Register
#define C0_CPU3_PWR_SWITCH_CTRL   R_PRCM_BASE + 0x014C // C0_CPU3 Power Switch Control Register
#define C1_CPU0_PWR_SWITCH_CTRL   R_PRCM_BASE + 0x0150 // C1_CPU0 Power Switch Control Register
#define C1_CPU1_PWR_SWITCH_CTRL   R_PRCM_BASE + 0x0154 // C1_CPU1 Power Switch Control Register
#define C1_CPU2_PWR_SWITCH_CTRL   R_PRCM_BASE + 0x0158 // C1_CPU2 Power Switch Control Register
#define C1_CPU3_PWR_SWITCH_CTRL   R_PRCM_BASE + 0x015C // C1_CPU3 Power Switch Control Register
#define RPIO_HOLD_CTRL_REG        R_PRCM_BASE + 0x01F0 // R_PIO Hold Control Register
#define OSC24M_CTRL_REG           R_PRCM_BASE + 0x01F4 // OSC24M Control Register

typedef enum {
  INTERNAL,
  OSC24M,
  PLL_PERIPH,
  IOSC
} CPUs_freq;

void r_prcm_uart_enable(void);
void r_prcm_set_cpus_clk_rate(CPUs_freq freq);
#endif