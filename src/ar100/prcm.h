#ifndef _PRCM_H
#define _PRCM_H

#define R_PRCM_BASE               0x01F01400
#define CPUS_CLK_REG              R_PRCM_BASE + 0x0000 // CPUS Clock Register
#define APB0_CFG_REG              R_PRCM_BASE + 0x000C // APB0 Cfg Register
#define APB0_CLK_GATING_REG       R_PRCM_BASE + 0x0028 // APB0 Clock Gating Reg
#define PLL_CTRL_REG1             R_PRCM_BASE + 0x0044 // PLL Control Reg 1
#define R_CIR_RX_CLK_REG          R_PRCM_BASE + 0x0054 // R_CIR_RX Clock Reg
#define APB0_SOFT_RST_REG         R_PRCM_BASE + 0x00B0 // APB0 SW Reset Reg
#define C0CPUX_PWROFF_GATING_REG  R_PRCM_BASE + 0x0100
#define C1CPUX_PWROFF_GATING_REG  R_PRCM_BASE + 0x0104
#define VDD_SYS_PWROFF_GATING_REG R_PRCM_BASE + 0x0110
#define GPU_PWROFF_GATING_REG     R_PRCM_BASE + 0x0118
#define VDD_SYS_PWROFF_RST_REG    R_PRCM_BASE + 0x0120
#define C0_CPU0_PWR_SWITCH_CTRL   R_PRCM_BASE + 0x0140
#define C0_CPU1_PWR_SWITCH_CTRL   R_PRCM_BASE + 0x0144
#define C0_CPU2_PWR_SWITCH_CTRL   R_PRCM_BASE + 0x0148
#define C0_CPU3_PWR_SWITCH_CTRL   R_PRCM_BASE + 0x014C
#define C1_CPU0_PWR_SWITCH_CTRL   R_PRCM_BASE + 0x0150
#define C1_CPU1_PWR_SWITCH_CTRL   R_PRCM_BASE + 0x0154
#define C1_CPU2_PWR_SWITCH_CTRL   R_PRCM_BASE + 0x0158
#define C1_CPU3_PWR_SWITCH_CTRL   R_PRCM_BASE + 0x015C
#define RPIO_HOLD_CTRL_REG        R_PRCM_BASE + 0x01F0
#define OSC24M_CTRL_REG           R_PRCM_BASE + 0x01F4

typedef enum {
    INTERNAL,
    OSC24M,
    PLL_PERIPH,
    IOSC
} CPUs_freq;

void r_prcm_uart_enable(void);
void r_prcm_set_cpus_clk_rate(CPUs_freq freq);
#endif
