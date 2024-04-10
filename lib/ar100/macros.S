/*
 * Copyright © 2013-2017, ARM Limited and Contributors. All rights reserved.
 * Copyright © 2017-2020 The Crust Firmware Authors.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MACROS_S
#define MACROS_S

	/* This macro marks a global data declaration. */
	.macro data name
	.section .data.\name, "aw", @progbits
	.global \name
	.type \name, %object
	.align 4
\name:
	.endm

	/* This macro marks the beginning of a function. */
	.macro func name
	.section .text.\name, "ax", @progbits
	.global \name
	.type \name, %function
	.func \name
	.cfi_sections .debug_frame
	.cfi_startproc
	.align 4
\name:
	.endm

	/* This macro marks the end of a function. */
	.macro endfunc name
	.cfi_endproc
	.endfunc
	.size \name, . - \name
	.endm

#endif /* MACROS_S */
