    .global start
    .global stack
    .global reset
    .global dst_addr
    .global src_addr
    .global words

    .text
    .thumb
    .align 0

start:
    ldr     r0, dst_addr
    ldr     r1, src_addr
    ldr     r2, words
    b       check
    
copy:
    ldmia   r1!, {r3}
    stmia   r0!, {r3}
    sub     r2, #1
    
check:
    cmp     r2, #0
    bne     copy
    
    @ Fix for SAM-BA stack bug
    ldr     r0, reset
    cmp     r0, #0
    bne     return
    ldr     r0, stack
    mov     sp, r0

return:
    bx      lr

    .align  0
stack:
    .word   0
reset:
    .word   0
dst_addr:
    .word   0
src_addr:
    .word   0
words:
    .word   0
