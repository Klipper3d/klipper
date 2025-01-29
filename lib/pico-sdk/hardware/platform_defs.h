/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_PLATFORM_DEFS_H
#define _HARDWARE_PLATFORM_DEFS_H

#define NUM_CORES 2u

#define NUM_DMA_CHANNELS 12u

#define NUM_GENERIC_TIMERS 1u
#define NUM_ALARMS 4u

#define NUM_IRQS 32u

#define NUM_SPIN_LOCKS 32u

#define XOSC_HZ 12000000u

#define NUM_SPIN_LOCKS 32u

#define NUM_BANK0_GPIOS 30

#ifndef _u
#define _u(x) x ## u
#endif

#endif
