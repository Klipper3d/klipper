/*
 * Copyright © 2017-2020 The Crust Firmware Authors.
 * SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0-only
 */

#include <macros.S>
#include <asm/spr.h>

func start
	l.mfspr	r2, r0, SPR_SYS_PPC_ADDR
	l.sfltui r2, 0x4000		# Size of exception vector area
	l.bf	1f
	l.srli	r2, r2, 8		# Vector address → exception number
	l.addi	r2, r0, 0		# Set to zero if not an exception
1:	l.addi	r3, r0, 0		# Invalidate instruction cache
	l.addi	r4, r0, 4096		# Cache lines (256) * block size (16)
2:	l.mtspr	r0, r3, SPR_ICACHE_ICBIR_ADDR
	l.sfltu	r3, r4
	l.bf	2b
	l.addi	r3, r3, 16		# Cache block size
	l.psync				# Flush CPU pipeline
	l.mfspr	r3, r0, SPR_SYS_SR_ADDR	# Enable instruction cache
	l.ori	r3, r3, SPR_SYS_SR_ICE_MASK
	l.mtspr	r0, r3, SPR_SYS_SR_ADDR
	l.nop				# One cache block of nops
	l.nop
	l.nop
	l.nop
	l.movhi	r3, hi(__bss_start)	# Clear .bss
	l.ori	r3, r3, lo(__bss_start)
	l.movhi	r4, hi(__bss_end)
	l.ori	r4, r4, lo(__bss_end)
3:	l.sw	0(r3), r0
	l.sfltu	r3, r4
	l.bf	3b
	l.addi	r3, r3, 4
	l.movhi	r1, hi(__stack_end)
	l.ori	r1, r1, lo(__stack_end)	# Initialize stack pointer
	l.j	main			# Jump to C entry point
	l.or	r3, r2, r2
endfunc start
