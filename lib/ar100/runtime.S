/*
 * Copyright © 2017-2020 The Crust Firmware Authors.
 * SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0-only
 */

#include <macros.S>

func __divsi3
	l.sflts	r3, r0
	l.sw	-8(r1), r18
	l.bnf	1f
	l.ori	r18, r0, 0
	l.sub	r3, r0, r3		# Negate x if it is negative
	l.addi	r18, r18, 1		# Increment the flag if x is negative
1:	l.sflts	r4, r0
	l.bnf	2f
	l.sw	-4(r1), r9
	l.sub	r4, r0, r4		# Negate y if it is negative
	l.addi	r18, r18, -1		# Decrement the flag if y is negative
2:	l.jal	__udivsi3
	l.addi	r1, r1, -8
	l.sfne	r18, r0
	l.bnf	3f
	l.addi	r1, r1, 8
	l.sub	r11, r0, r11		# Negate q if the flag is nonzero
3:	l.lwz	r9, -4(r1)
	l.jr	r9
	l.lwz	r18, -8(r1)
endfunc __divsi3

/*
 * Of the three ORBIS32 32-bit multiplication instructions (l.mul, l.muli, and
 * l.mulu), only l.mul works. By passing "-msoft-mul" to the compiler, and
 * delegating to this function, we can force all multiplication to use l.mul.
 */
func __mulsi3
	l.jr	r9
	l.mul	r11, r3, r4
endfunc __mulsi3

/*
 * Derived from the "best method for counting bits in a 32-bit integer" at
 * https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel.
 *
 * Signed multiplication is used because l.mulu is broken in hardware. This is
 * safe because the previous bit masking ensures neither operand is negative.
 */
func __popcountsi2
	l.movhi	r5, 0x5555		# Statement 1:
	l.ori	r5, r5, 0x5555		#	r5 = 0x55555555
	l.srli	r4, r3, 1		#	r4 = v >> 1
	l.and	r4, r4, r5		#	r4 = (v >> 1) & 0x55555555
	l.sub	r3, r3, r4		#	v = v - ((v >> 1) & 0x55555555)
	l.movhi	r5, 0x3333		# Statement 2:
	l.ori	r5, r5, 0x3333		#	r5 = 0x33333333
	l.srli	r4, r3, 2		#	r4 = v >> 2
	l.and	r4, r4, r5		#	r4 = (v >> 2) & 0x33333333
	l.and	r3, r3, r5		#	v = v & 0x33333333
	l.add	r3, r3, r4		#	v += ((v >> 2) & 0x33333333)
	l.movhi	r5, 0x0f0f		# Statement 3:
	l.ori	r5, r5, 0x0f0f		#	r5 = 0x0f0f0f0f
	l.srli	r4, r3, 4		#	r4 = v >> 4
	l.add	r4, r3, r4		#	r4 = v + (v >> 4)
	l.and	r4, r4, r5		#	r4 = v + (v >> 4) & 0x0f0f0f0f
	l.movhi	r5, 0x0101
	l.ori	r5, r5, 0x0101		#	r5 = 0x01010101
	l.mul	r11, r4, r5		#	c = r4 * 0x01010101
	l.jr	r9
	l.srli	r11, r11, 24		#	return c >> 24
endfunc __popcountsi2

/*
 * Optimized implementation of the "shift divisor method" algorithm from
 * T. Rodeheffer. Software Integer Division. Microsoft Research, 2008.
 *
 * In addition to returning the quotient in r11, this function also returns
 * the remainder in r12. __umodsi3 simply copies the remainder into r11.
 */
func __udivsi3				# u32 __udivsi3(u32 x, u32 y) {
	l.sfeqi	r4, 1			#	if (y == 1)
	l.bf	5f			#		goto identity;
	l.ori	r12, r3, 0		#	u32 r = x;
	l.ori	r5, r4, 0		#	u32 y0 = y;
	l.addi	r11, r0, 0		#	u32 q = 0;
	l.sfltu	r3, r4			#	if (x >= y) {
	l.bf	2f
	l.sub	r3, r3, r4		#		x = x−y;
1:	l.sfltu	r3, r4			#		while (x >= y) {
	l.bf	2f
	l.sub	r3, r3, r4		#			x = x−y;
	l.add	r4, r4, r4		#			y *= 2;
	l.j	1b			#		}
2:	l.sfltu	r12, r4			#	} for (;;) {
	l.bf	3f			#		if (r >= y) {
	l.sfeq	r4, r5			#		[if (y == y0)]
	l.sub	r12, r12, r4		#			r = r−y;
	l.addi	r11, r11, 1		#			q = q + 1;
3:	l.bf	4f			#		} if (y == y0) break;
	l.srli	r4, r4, 1		#		y >>= 1;
	l.j	2b			#	}
	l.add	r11, r11, r11		#		q *= 2;
4:	l.jr	r9			#	return q;
	l.nop
5:	l.ori	r11, r3, 0		# identity:
	l.jr	r9			#	return x;
	l.ori	r12, r0, 0		#	r = 0;
endfunc __udivsi3			# }

func __umodsi3
	l.sw	-4(r1), r9
	l.jal	__udivsi3
	l.addi	r1, r1, -4
	l.addi	r1, r1, 4
	l.lwz	r9, -4(r1)
	l.jr	r9
	l.ori	r11, r12, 0
endfunc __umodsi3
