// Power, Reset and Clock Management for ar100
//
// Copyright (C) 2020-2021  Elias Bakken <elias@iagent.no>
//
// This file may be distributed under the terms of the GNU GPLv3 license.


#include "prcm.h"
#include "util.h"
#define CPUS_POST_DIV 1

void
r_prcm_set_cpus_clk_rate(CPUs_freq freq){
    write_reg(CPUS_CLK_REG, freq<<16 | CPUS_POST_DIV<<8);
}

void
r_prcm_uart_enable(void){
    clear_bit(APB0_CLK_GATING_REG, 4);
    set_bit(APB0_SOFT_RST_REG, 4);
    set_bit(APB0_CLK_GATING_REG, 4);
}
