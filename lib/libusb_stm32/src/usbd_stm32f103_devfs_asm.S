/* This file is the part of the Lightweight USB device Stack for STM32 microcontrollers
 *
 * Copyright ©2016 Dmitry Filimonchuk <dmitrystu[at]gmail[dot]com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !defined (__ASSEMBLER__)
    #define __ASSEMBLER__
#endif

#include "usb.h"
#if defined(USBD_STM32F103)
#include "memmap.inc"

#define EP_SETUP    0x0800
#define EP_TYPE     0x0600
#define EP_KIND     0x0100
#define EP_ADDR     0x000F

#define EP_RX_CTR   0x8000
#define EP_RX_DTOG  0x4000
#define EP_RX_STAT  0x3000
#define EP_RX_SWBUF 0x0040

#define EP_RX_DIS   0x0000
#define EP_RX_STAL  0x1000
#define EP_RX_NAK   0x2000
#define EP_RX_VAL   0x3000

#define EP_TX_CTR   0x0080
#define EP_TX_DTOG  0x0040
#define EP_TX_STAT  0x0030
#define EP_TX_SWBUF 0x4000

#define EP_TX_DIS   0x0000
#define EP_TX_STAL  0x0010
#define EP_TX_NAK   0x0020
#define EP_TX_VAL   0x0030

#if defined(STM32F302x8) || defined(STM32F302xE) || defined(STM32F303xE)
    #define RXADDR0     0x00
    #define RXCOUNT0    0x02
    #define RXADDR1     0x04
    #define RXCOUNT1    0x06
    #define USB_PMASZ   0x300
    #define USB_PMASTP  0x02

    .macro GetEPT
    add     r4, r4, r0, LSR #25     //*EPT -> R4
    .endm

    .macro GetPMA
    ldr     r4, =#USB_PMABASE
    adds    r5, r4          // PMA BUFFER -> R5
    .endm

#else
    #define RXADDR0     0x00
    #define RXCOUNT0    0x04
    #define RXADDR1     0x08
    #define RXCOUNT1    0x0C
    #define USB_PMASZ   0x200
    #define USB_PMASTP  0x04

    .macro GetEPT
    add     r4, r4, r0, LSR #24     //*EPT -> R4
    .endm

    .macro GetPMA
    ldr     r4, =#USB_PMABASE
    lsls    r5, #1
    adds    r5, r4          // PMA BUFFER -> R5
    .endm

#endif

#define TXADDR0     RXADDR0
#define TXCOUNT0    RXCOUNT0
#define TXADDR1     RXADDR1
#define TXCOUNT1    RXCOUNT1

#define TXADDR      RXADDR0
#define TXCOUNT     RXCOUNT0
#define RXADDR      RXADDR1
#define RXCOUNT     RXCOUNT1

#define EP_NOTOG    (EP_RX_CTR | EP_TX_CTR | EP_SETUP | EP_TYPE | EP_KIND | EP_ADDR)

#define TGL_SET(mask, bits)  ((EP_NOTOG | (mask))<<16 | (bits))

#define TX_STALL    TGL_SET(EP_TX_STAT,                            EP_TX_STAL)
#define RX_STALL    TGL_SET(EP_RX_STAT,                            EP_RX_STAL)
#define TX_USTALL   TGL_SET(EP_TX_STAT | EP_TX_DTOG,               EP_TX_NAK)
#define RX_USTALL   TGL_SET(EP_RX_STAT | EP_RX_DTOG,               EP_RX_VAL)
#define DTX_USTALL  TGL_SET(EP_TX_STAT | EP_TX_DTOG | EP_TX_SWBUF, EP_TX_VAL)
#define DRX_USTALL  TGL_SET(EP_RX_STAT | EP_RX_DTOG | EP_RX_SWBUF, EP_RX_VAL | EP_RX_SWBUF)


#if (USBD_DP_PORT == GPIOA)
    #define RCC_GPIOxEN RCC_GPIOAEN
#elif (USBD_DP_PORT == GPIOB)
    #define RCC_GPIOxEN RCC_GPIOBEN
#elif (USBD_DP_PORT == GPIOC)
    #define RCC_GPIOxEN RCC_GPIOCEN
#elif (USBD_DP_PORT == GPIOD)
    #define RCC_GPIOxEN RCC_GPIODEN
#elif (USBD_DP_PORT == GPIOE)
    #define RCC_GPIOxEN RCC_GPIOEEN
#elif (USBD_DP_PORT == GPIOF)
    #define RCC_GPIOxEN RCC_GPIOFEN
#elif (USBD_DP_PORT == GPIOG)
    #define RCC_GPIOxEN RCC_GPIOGEN
#elif (USBD_DP_PORT == GPIOH)
    #define RCC_GPIOxEN RCC_GPIOHEN
#else
    #warning "No USB DP control used for F303"
    #undef USBD_DP_PORT
    #undef USBD_DP_PIN
#endif

    .syntax unified
    .cpu cortex-m3
    .thumb

    .section .rodata.usbd_devfs_asm
    .align  4
    .globl  usbd_devfs_asm
usbd_devfs_asm:
    .long   _getinfo
    .long   _enable
    .long   _connect
    .long   _setaddr
    .long   _ep_config
    .long   _ep_deconfig
    .long   _ep_read
    .long   _ep_write
    .long   _ep_setstall
    .long   _ep_isstalled
    .long   _evt_poll
    .long   _get_frame
    .long   _get_serial_desc
    .size   usbd_devfs_asm, . - usbd_devfs_asm

    .text
    .align 2
    .thumb_func
    .type _get_serial_desc, %function

/*  uint16_t get_serial_desc (void *buffer)
 *  R0 <- buffer for the string descriptor
 *  descrpitor size -> R0
 */
_get_serial_desc:
    push    {r4, r5, lr}
    movs    r1, #18             //descriptor size 18 bytes
    strb    r1, [r0]
    movs    r1, #0x03           //DTYPE_STRING
    strb    r1, [r0, #0x01]
    ldr     r5, .L_uid_base     //UID3 this is the serial number
    ldr     r4, .L_fnv1a_offset //FNV1A offset
    ldr     r2, [r5, 0x00]      //UID0
    bl      .L_fnv1a
    ldr     r2, [r5, 0x04]      //UID1
    bl      .L_fnv1a
    ldr     r2, [r5, 0x08]      //UID2
    bl      .L_fnv1a
    movs    r3, #28
.L_gsn_loop:
    lsrs    r1, r4, r3
    and     r1, #0x0F
    cmp     r1, #0x09
    ite     gt
    addgt   r1, #55
    addle   r1, #48
.L_gsn_store:
    adds    r0, #0x02
    strb    r1, [r0]
    lsrs    r1, #0x08
    strb    r1, [r0, #0x01]
    subs    r3, #0x04
    bpl     .L_gsn_loop
    movs    r0, #18
    pop     {r4, r5, pc}

.L_fnv1a:
    movs    r3, #0x04
.L_fnv1a_loop:
    uxtb    r1, r2
    eors    r4, r1
    ldr     r1, .L_fnv1a_prime       //FNV1A prime
    muls    r4, r1
    lsrs    r2, #0x08
    subs    r3, #0x01
    bne     .L_fnv1a_loop
    bx      lr

    .align 2
.L_uid_base:        .long   UID_BASE
.L_fnv1a_offset:    .long   2166136261
.L_fnv1a_prime:     .long   16777619

    .size _get_serial_desc, . - _get_serial_desc

    .thumb_func
    .type   _connect, %function
_connect:
#if defined(USBD_DP_PORT) && defined(USBD_DP_PIN) && defined(STM32F1)
    #if (USBD_DP_PIN < 8)
    #define GPIO_CRx    GPIO_CRL
    #define DP_PIN      (4 * USBD_DP_PIN)
    #else
    #define GPIO_CRx    GPIO_CRH
    #define DP_PIN      (4 * (USBD_DP_PIN - 8))
    #endif
    ldr     r3, =#USBD_DP_PORT
    ldr     r2, [r3, #GPIO_CRx]
    movs    r1, #0x0F
    bics    r2, r2, r1, LSL #DP_PIN
    movs    r1, #0x04
    cbz     r0, .L_store
    movs    r1, #0x01
    lsls    r1, #USBD_DP_PIN
    str     r1, [r3, #GPIO_BSRR]
    movs    r1, #0x02
.L_store:
    orrs    r2, r2, r1, LSL #DP_PIN
    str     r2, [r3, #GPIO_CRx]
#elif defined(USBD_DP_PORT) && defined(USBD_DP_PIN) && defined(STM32F3)
    ldr     r3, =#USBD_DP_PORT
    ldr     r2, [r3, #GPIO_MODER]
    movs    r1, #0x03
    bics    r2, r2, r1, LSL #(2 * USBD_DP_PIN)
    movs    r1, #0x01
    cbz     r0, .L_store
    orrs    r2, r2, r1, LSL #(2 * USBD_DP_PIN)
    lsls    r1, #USBD_DP_PIN
    str     r1, [r3, #GPIO_BSRR]
.L_store:
    str     r2, [r3, #GPIO_MODER]
#endif
    movs    r0, #usbd_lane_unk
    bx      lr
    .size   _connect, . - _connect

    .thumb_func
    .type   _setaddr, %function
_setaddr:
    ldr     r1, =#USB_REGBASE
    adds    r0, #0x80
    strh    r0, [r1, #USB_DADDR]     //USB->DADDR
    bx      lr
    .size   _setaddr, . - _setaddr

    .thumb_func
    .type   _get_frame, %function
_get_frame:
    ldr     r0, =#USB_REGBASE
    ldrh    r0, [r0, #USB_FNR]     //FNR
    lsls    r0, #21
    lsrs    r0, #21
    bx      lr
    .size   _get_frame, . - _get_frame

    .thumb_func
    .type   _enable, %function
_enable:
    ldr     r2, =#RCC_BASE        //RCC
    movs    r1, #0x01
    lsls    r3, r1, #23           //USBEN or USBRST
    cbz     r0, .L_disable
.L_enable:
/* enabling and resetting USB peripheral */
/* enabling DP control GPIO port */
#if defined(USBD_DP_PORT) && defined(USBD_DP_PIN) && defined(STM32F3)
    lsls    r1, #RCC_GPIOxEN
    ldr     r0, [r2, #RCC_AHBENR]
    orrs    r0, r1
    str     r0, [r2, #RCC_AHBENR]
#elif defined(USBD_DP_PORT) && defined(USBD_DP_PIN)
    lsls    r1, #RCC_GPIOxEN
    ldr     r0, [r2, #RCC_APB2ENR]
    orrs    r0, r1
    str     r0, [r2, #RCC_APB2ENR]
#endif
    ldr     r1, =#USB_REGBASE
    ldr     r0, [r2, #RCC_APB1ENR]
    orrs    r0, r3
    str     r0, [r2, #RCC_APB1ENR]     //RCC->APB1ENR |= USBEN
    ldr     r0, [r2, #RCC_APB1RSTR]
    orrs    r0, r3
    str     r0, [r2, #RCC_APB1RSTR]     //RCC->APB1RSTR |= USBRST
    bics    r0, r3
    str     r0, [r2, #RCC_APB1RSTR]     //RCC->APB1RSTR &= ~USBRST
/* setting up USB CNTR */
#if !defined(USBD_SOF_DISABLED)
    movs    r0, #0xBE                   // CTRM | ERRM | WKUPM | SUSPM | RESETM | SOFM
#else
    movs    r0, #0xBC                   // CTRM | ERRM | WKUPM | SUSPM | RESETM
#endif
    lsls    r0, #0x08
    strh    r0, [r1, #USB_CNTR]         //set USB->CNTR
    bx      lr
.L_disable:
    ldr     r0, [r2, #RCC_APB1ENR]
    tst     r0, r3
    beq     .L_enable_end               // usb is already disabled
/* disabling USB peripheral */
    bics    r0, r3
    str     r0, [r2, #RCC_APB1ENR]
    movs    r0, #0
    b       _connect                    // jump to disconnect subroutine
.L_enable_end:
    bx      lr
    .size   _enable, . - _enable

    .thumb_func
    .type   _getinfo, %function
_getinfo:
    movs    r0, #0
    ldr     r2, =#RCC_BASE
    ldr     r1, [r2, #RCC_APB1ENR]
    lsrs    r1, #24                     //USBEN -> CF
    bcc     .L_getinfo_end
    adds    r0, #USBD_HW_ENABLED
#if defined(USBD_DP_PORT) && defined(USBD_DP_PIN)
    ldr     r2, =#USBD_DP_PORT
    ldr     r1, [r2, #GPIO_IDR]
    lsrs    r1, #USBD_DP_PIN            //USBD_DP_PIN -> CF
    bcc     .L_getinfo_end
#endif
    adds    r0, #USBD_HW_SPEED_FS
.L_getinfo_end:
    bx      lr
    .size   _getinfo, . - _getinfo

    .thumb_func
    .type   _ep_setstall, %function
/*void ep_settall(uint8_t ep, bool stall)
 * in  R0 <- endpoint number
 * in  R1 <- 0 if unstall, !0 if stall
 */
_ep_setstall:
    push    {r4, lr}
    lsls    r2, r0, #28
    lsrs    r2, #26
    ldr     r3, =#USB_EPBASE
    adds    r3, r2          // epr -> r3
    movs    r2, 0x30        // TX_STAT_MASK -> r2
    ldrh    r4, [r3]
    lsls    r4, #21
    lsrs    r4, #29         // EP_TYPE | EP_KIND -> R4 LSB
    cmp     r4, #0x04       // ISO ?
    beq     .L_eps_exit
    cmp     r0, #0x80
    blo     .L_eps_rx
.L_eps_tx:
    ldr     r0, =#TX_STALL   //stall TX
    cmp     r1, #0x00
    bne     .L_eps_reg_set
.L_eps_tx_unstall:
    ldr     r0, =#DTX_USTALL //unstall dblbulk or iso TX (VALID and clr DTOG_TX & SWBUF_TX)
    cmp     r4, #0x01       // if doublebuffered bulk endpoint
    beq     .L_eps_reg_set
    ldr     r0, =#TX_USTALL  // unstall other TX (NAKED + clr DTOG_TX)
    b       .L_eps_reg_set
.L_eps_rx:
    lsls    r2, #8          // RX_STAT_MASK -> R2
    ldr     r0,=#RX_STALL    //stall RX
    cmp     r1, #0x00
    bne     .L_eps_reg_set
.L_eps_rx_unstall:
    ldr     r0, =#DRX_USTALL //unstall dblbulk or iso (VALID. clr DTOG_RX set SWBUF_RX)
    cmp     r4, #0x01       // if dblbulk
    beq     .L_eps_reg_set
    ldr     r0, =#RX_USTALL  // unstall other RX (VALID + clr
/* R0 - mask and toggle bits
 * R2 - mask for STAT bits
 * R3 - endpoint register pointer
 */
.L_eps_reg_set:
    ldrh    r1, [r3]        // *epr -> r1
    ands    r2, r1          // check if endpoint disabled
    beq     .L_eps_exit     // do nothing
    eors    r1, r0
    lsrs    r0, #16
    ands    r1, r0
    strh    r1, [r3]
.L_eps_exit:
    pop     {r4, pc}
    .size   _ep_setstall, . - _ep_setstall


    .thumb_func
    .type   _ep_isstalled, %function
/* bool ep_isstalled(uint8t ep) */
_ep_isstalled:
    ldr     r1, =#USB_EPBASE
    lsls    r2, r0, #28
    lsrs    r2, #26
    ldr     r1, [r1, r2]
    lsls    r1, #17
    cmp     r0, #0x80
    bhs     .L_eis_check
    lsls    r1, #8
.L_eis_check:
    lsrs    r1, r1, #28
    subs    r1, #0x01
    subs    r0, r1, #0x01
    sbcs    r1, r1
    rsbs    r0, r1, #0
    bx      lr
    .size  _ep_isstalled, . - _ep_isstalled


    .thumb_func
    .type       _ep_read, %function
/* int32_t _ep_read(uint8_t ep, void *buf, uint16_t blen)
 * in  R0 <- endpoint
 * in  R1 <- *buffer
 * in  R2 <- length of the buffer
 * out length of the recieved data -> R0 or 0 on error
 */
_ep_read:
    push    {r4, r5, lr}
    ldr     r3, =#USB_EPBASE
    ldr     r4, =#USB_PMABASE
    lsls    r0, #28
    add     r3, r3, r0, LSR #26     //*EPR -> R3
    GetEPT                          //*EPT -> R4
    ldrh    r5, [r3]                // reading epr
/* validating endpoint */
    movs    r0, #0x37
    ands    r0, r0, r5, LSR #8
    cmp     r0, #0x34       // (OK) RX_VALID + ISO
    beq     .L_epr_iso
    cmp     r0, #0x31       // (OK) RX_VALID + DBLBULK
    beq     .L_epr_dbl
    cmp     r0, #0x20       // (OK) RX_NAKED + BULK
    beq     .L_epr_sngl
    cmp     r0, #0x22       // (OK) RX_NAKED + CTRL
    beq     .L_epr_sngl
    cmp     r0, #0x26       // (OK) RX_NAKED + INTR
    beq     .L_epr_sngl
    movs    r0, #0xFF       // endpoint contains no valid data
    sxtb    r0, r0
    b       .L_epr_exit
/* processing */
.L_epr_dbl:
    eors    r0, r5, r5, LSR #8
    lsrs    r0, #7          // SW_RX ^ DTOG_RX -> CF
    bcs     .L_epr_notog    // jmp if SW_RX != DTOG_RX (VALID)
    ldr     r0, =#EP_NOTOG
    ands    r5, r0
    adds    r5, #EP_RX_SWBUF
    strh    r5, [r3]        // toggling SW_RX
.L_epr_notog:
    ldrh    r5, [r3]
    lsls    r5, #8          // shift SW_RX to DTOG_RX
.L_epr_iso:
    lsrs    r5, #15         // DTOG_RX -> CF
    bcs     .L_epr_sngl
    subs    r4, #0x08       // set RXADDR0
.L_epr_sngl:
    ldrh    r0, [r4, #RXCOUNT]
    lsrs    r5, r0, #0x0A
    lsls    r5, #0x0A       // r5 = r5 & ~0x03FF
    strh    r5, [r4, #RXCOUNT]
    lsls    r0, #22
    lsrs    r0, #22         // r0 &= 0x3FF (RX count)
    ldrh    r5, [r4, #RXADDR]
    GetPMA
    cmp     r2, r0
    blo     .L_epr_read
    mov     r2, r0          // if buffer is larger
.L_epr_read:
    cmp     r2, #1
    blo     .L_epr_read_end
    ldrh    r4, [r5]
    strb    r4, [r1]
    beq     .L_epr_read_end
    lsrs    r4, #8
    strb    r4, [r1, #1]
    adds    r1, #2
    adds    r5, #USB_PMASTP
    subs    r2, #2
    bhi     .L_epr_read
.L_epr_read_end:
    ldrh    r5, [r3]        // reload EPR
    lsls    r1, r5, #21
    lsrs    r1, #29
    cmp     r1, #0x04
    beq     .L_epr_exit     // ep is iso. no needs to set it to valid
    cmp     r1, #0x01
    beq     .L_epr_exit     // ep is dblbulk. no needs to set it to valid
    ldr     r2, =#TGL_SET(EP_RX_STAT , EP_RX_VAL)
    eors    r5, r2
    and     r5, r5, r2, LSR #16
    strh    r5, [r3]        // set ep to VALID state
.L_epr_exit:
    pop     {r4, r5, pc}
    .size   _ep_read, . - _ep_read


    .thumb_func
    .type   _ep_write, %function
/* int32_t ep_write(uint8_t ep, void *buf, uint16_t blen)
 * R0 -> endpoint
 * R1 -> *buffer
 * R2 -> data length
 * result -> R0
 */
_ep_write:
    push    {r4, r5, r6, lr}
    ldr     r3, =#USB_EPBASE
    ldr     r4, =#USB_PMABASE
    lsls    r0, #28
    add     r3, r3, r0, LSR #26     //*EPR -> R3
    GetEPT
    ldrh    r5, [r3]        // reading epr
    movs    r0, #0x73
    and     r0, r0, r5, LSR #4
    cmp     r0, #0x43       // (OK) TX_VALID + ISO
    beq     .L_epw_iso
    cmp     r0, #0x12       // (OK) TX_NAK + DBLBULK
    beq     .L_epw_dbl
    cmp     r0, #0x02       // (OK) TX_NAK + BULK
    beq     .L_epw_sngl
    cmp     r0, #0x22       // (OK) TX_NAK + CONTROL
    beq     .L_epw_sngl
    cmp     r0, #0x62       // (OK) TX_NAK + INTERRUPT
    beq     .L_epw_sngl
    movs    r0, #0xFF       // -1 error
    sxtb    r0, r0
    b       .L_epw_exit
.L_epw_dbl:
    mvns    r5, r5
    lsrs    r5, #8          // ~SWBUF_TX -> DTOG_TX
.L_epw_iso:
    lsrs    r5, #7          // DTOG_TX -> CF
    bcs     .L_epw_sngl
    adds    r4, #8          // TXADDR1 -> R4
.L_epw_sngl:
    strh    r2, [r4, #TXCOUNT]
    mov     r0, r2          // save count for return
    ldrh    r5, [r4, #TXADDR]
    GetPMA
.L_epw_write:
    cmp     r2, #1
    blo     .L_epw_write_end
    ldrb    r4, [r1]
    beq     .L_epw_store
    ldrb    r6, [r1, #1]
    lsls    r6, #8
    orrs    r4, r6
.L_epw_store:
    strh    r4, [r5]
    adds    r5, #USB_PMASTP
    adds    r1, #2
    subs    r2, #2
    bhi     .L_epw_write
.L_epw_write_end:
    ldrh    r5, [r3]        // reload EPR
    lsls    r1, r5, #21
    lsrs    r1, #29
    cmp     r1, #0x04
    beq     .L_epw_exit     // isochronous ep. do nothing
    ldr     r2, =#TGL_SET(EP_TX_STAT, EP_TX_VAL)
    cmp     r1, #0x01
    bne     .L_epw_setstate // NOT a doublebuffered bulk
    ldr     r2, =#TGL_SET(EP_TX_SWBUF, EP_TX_SWBUF)
    bics    r5, r2          // clear TX_SWBUF
.L_epw_setstate:
    eors    r5, r2
    and     r5, r5, r2, LSR #16
    strh    r5, [r3]
.L_epw_exit:
    pop     {r4, r5, r6, pc}
    .size   _ep_write, .- _ep_write



/* internal function */
/* requester size passed in R2 */
/* result returns in R0 CF=1 if OK*/

_get_next_pma:
    push    {r1, r3, r4, lr}
    movs    r1, #16
    ldr     r3, =#USB_PMASZ
    ldr     r0, =#USB_PMABASE
.L_gnp_chkaddr:
    ldrh    r4, [r0, #0]    //txaddr
    tst     r4, r4
    beq     .L_gnp_nxtaddr
    cmp     r3, r4
    blo     .L_gnp_nxtaddr
    mov     r3, r4
.L_gnp_nxtaddr:
    adds    r0, #8
    subs    r1, #1
    bne     .L_gnp_chkaddr
    subs    r0, r3, r2
    blo     .L_gnp_exit
    cmp     r0, #0x20       //check for the pma table overlap
.L_gnp_exit:
    pop     {r1, r3, r4, pc}



    .size   _get_next_pma, . - _get_next_pma

    .thumb_func
    .type   _ep_config, %function
/* bool ep_config(uint8_t ep, uint8_t eptype, uint16_t epsize)
 * R0 <- ep
 * R1 <- eptype
 * R2 <- epsize
 * result -> R0
 */
_ep_config:
    push    {r4, r5, lr}
    movs    r3, 0x01
    ands    r3, r2
    adds    r2, r3      //R2 -> halfword aligned epsize
    movs    r3, #0x00   //BULK
    cmp     r1, #0x02   // is eptype bulk ?
    beq     .L_epc_settype
    movs    r3, #0x01   //DBLBULK
    cmp     r1, #0x06
    beq     .L_epc_settype
    movs    r3, #0x02   //CONTROL
    cmp     r1, #0x00
    beq     .L_epc_settype
    movs    r3, #0x04   //ISO
    cmp     r1, #0x01
    beq     .L_epc_settype
    movs    r3, #0x06   //INTERRUPT
.L_epc_settype:
    lsls    r3, #8
    lsls    r4, r0, #28
    lsrs    r4, #28
    orrs    r3, r4
    lsls    r4, #2
    ldr     r5, =#USB_EPBASE
    strh    r3, [r5, r4]    //setup EPTYPE EPKIND EPADDR
    cmp     r1, #0x00       // is a control ep ?
    beq     .L_epc_setuptx
    cmp     r0, #0x80
    blo     .L_epc_setuprx
.L_epc_setuptx:
    ldr     r5, =#USB_PMABASE
    lsls    r4, #2
    adds    r5, r4
    bl      _get_next_pma
    bcc     .L_epc_fail
    strh    r0, [r5, #TXADDR]    //store txaddr or txaddr0
    movs    r0, #0x00
    strh    r0, [r5, #TXCOUNT]    //store txcnt
    cmp     r1, #0x06       // is DBLBULK
    beq     .L_epc_txdbl
    ldr     r3, =#TX_USTALL  //set state NAKED , clr DTOG_TX
    cmp     r1, #0x01       // is ISO
    bne     .L_epc_txsetstate   //
.L_epc_txdbl:
    ldr     r3, =#DTX_USTALL //set state VALID clr DTOG_TX & SWBUF_TX
    bl      _get_next_pma
    bcc     .L_epc_fail
    strh    r0, [r5, #TXADDR1]    //store txaddr1
    movs    r0, #0x00
    strh    r0, [r5, #TXCOUNT1]    //store txcnt
.L_epc_txsetstate:
    ldr     r5, =#USB_EPBASE
    lsrs    r4, #2
    ldrh    r0, [r5, r4]
    eors    r0, r3
    lsrs    r3, #16
    ands    r0, r3
    strh    r0, [r5, r4]
    cmp     r1, #0x00       //is a control ep ?
    bne     .L_epc_exit
.L_epc_setuprx:
    mov     r3, r2
    cmp     r2, #62
    bls     .L_epc_rxbb
    movs    r3, #0x1F
    ands    r3, r2
    bne     .L_epc_rxaa
    subs    r2, #0x20
.L_epc_rxaa:
    bics    r2, r3
    lsrs    r3, r2, #4
    adds    r3, #0x40
    adds    r2, #0x20
.L_epc_rxbb:
    lsls    r3, #9
    ldr     r5, =#USB_PMABASE
    lsls    r4, #2
    adds    r5, r4
/* RX or RX1 */
    bl      _get_next_pma
    bcc     .L_epc_fail
    strh    r0, [r5, #RXADDR]
    strh    r3, [r5, #RXCOUNT]
    ldr     r0, =#RX_USTALL
/* check if doublebuffered */
    cmp     r1, 0x06    //if dblbulk
    beq     .L_epc_rxdbl
    cmp     r1, 0x01    // iso
    bne     .L_epc_rxsetstate
.L_epc_rxdbl:
    bl      _get_next_pma
    bcc     .L_epc_fail
    strh    r0, [r5, #RXADDR0]    //store rxaddr0
    strh    r3, [r5, #RXCOUNT0]    //store rxcnt0
    ldr     r0, =#DRX_USTALL
.L_epc_rxsetstate:
    ldr     r5, =#USB_EPBASE
    lsrs    r4, #2
    ldrh    r3, [r5, r4]
    eors    r3, r0
    lsrs    r0, #16
    ands    r3, r0
    strh    r3, [r5, r4]
.L_epc_exit:
    movs    r0, #0x01
    pop     {r4, r5, pc}
.L_epc_fail:
    movs    r0, #0x00
    pop     {r4, r5, pc}

    .size   _ep_config, . - _ep_config

    .thumb_func
    .type   _ep_deconfig, %function


/* void ep_deconfig( uint8_t ep)
 * R0 <- ep
 */
_ep_deconfig:
    lsls    r1, r0, #28
    lsrs    r1, #26
    ldr     r2, =#USB_EPBASE
    ldr     r3, =#USB_PMABASE
    adds    r2, r1
    lsls    r1, #1
    adds    r3, r1
/* clearing endpoint register */
    ldr     r1, =#EP_NOTOG
    ldrh    r0, [r2]
    bics    r0, r1
    strh    r0, [r2]
/* clearing PMA data */
    movs    r0, #0x00
    strh    r0, [r3, #TXADDR]
    strh    r0, [r3, #TXCOUNT]
    strh    r0, [r3, #RXADDR]
    strh    r0, [r3, #RXCOUNT]
    bx      lr

    .size   _ep_deconfig, . - _ep_config




#define ISTRSHIFT   8
#define ISTRBIT(bit) ((1 << bit) >> ISTRSHIFT)


    .thumb_func
    .type     _evt_poll, %function
/*void evt_poll(usbd_device *dev, usbd_evt_callback callback)*/
_evt_poll:
    push    {r0, r1, r4, r5}
    ldr     r3, =#USB_REGBASE
    ldrh    r0, [r3, #4]        //USB->ISTR -> R2
/* ep_index -> R2 */
    movs    r2, 0x07
    ands    r2, r0
/* checking USB->ISTR for events */
#if !defined(USBD_SOF_DISABLED)
    lsrs    r1, r0, #10         //SOFM -> CF
    bcs     .L_ep_sofm
#endif
    lsrs    r1, r0, #11         //RESETM -> CF
    bcs     .L_ep_resetm
    lsrs    r1, r0, #16         //CTRM -> CF
    bcs     .L_ep_ctrm
    lsrs    r1, r0, #14         //ERRM -> CF
    bcs     .L_ep_errm
    lsrs    r1, r0, #13         //WKUPM -> CF
    bcs     .L_ep_wkupm
    lsrs    r1, r0, #12         //SUSPM -> CF
    bcs     .L_ep_suspm
    /* exit with no callback */
    pop     {r0, r1, r4 , r5}
    bx      lr

.L_ep_ctrm:
    movs    r5, #0x80           // CTR_TX mask to R5
    ldr     r0,=#USB_EPBASE
    add     r0, r0, r2, LSL #2  // R0 ep register address
    ldrh    r4, [r0]            // R4 EPR valur
    lsrs    r3, r4, #8          // CTR_TX -> CF
    bcc     .L_ep_ctr_rx
/* CTR_TX event */
    movs    r1, #usbd_evt_eptx
    orrs    r2, r5              // set endpoint tx
    b       .L_ep_clr_ctr
.L_ep_ctr_rx:
/* CTR_RX  RX or SETUP */
    lsls    r5, #0x08           // set mask to CRT_RX
    movs    r1, #usbd_evt_eprx
    lsls    r3, r4, #21         //SETUP -> CF
    bcc     .L_ep_clr_ctr
    movs    r1, #usbd_evt_epsetup
.L_ep_clr_ctr:
    bics    r4, r5              //clear CTR flag
    ldr     r5, =#EP_NOTOG
    ands    r4, r5
    strh    r4, [r0]            // store
    b       .L_ep_callback
.L_ep_errm:
    movs    r1, #usbd_evt_error
    movs    r4, #ISTRBIT(13)
    b      .L_ep_clristr
#if !defined(USBD_SOF_DISABLED)
.L_ep_sofm:
    movs    r1, #usbd_evt_sof
    movs    r4, #ISTRBIT(9)
    b       .L_ep_clristr
#endif
.L_ep_wkupm:
    ldrh    r1, [r3, #0]            //R1 USB->CNTR
    movs    r5, #0x08
    bics    r1, r5                  //clr FSUSP
    strh    r1, [r3, #0]            //USB->CNTR R2
    movs    r1, #usbd_evt_wkup
    movs    r4, #ISTRBIT(12)
    b       .L_ep_clristr

.L_ep_suspm:
    ldrh    r1, [r3, #0]            //R1 USB->CNTR
    movs    r5, #0x08
    orrs    r1, r5                  //set FSUSP
    strh    r1, [r3, #0]            //USB->CNTR R2
    movs    r1, #usbd_evt_susp
    movs    r4, #ISTRBIT(11)
    b       .L_ep_clristr

/* do reset routine */
.L_ep_resetm:
    movs    r1, #7
    ldr     r2, =#USB_EPBASE
    ldr     r0, =#USB_PMABASE
    ldr     r5, =#EP_NOTOG
.L_ep_reset_loop:
    ldrh    r4, [r2]
    bics    r4, r5
    strh    r4, [r2]
    movs    r4, #0
    strh    r4, [r0, #TXADDR]
    strh    r4, [r0, #TXCOUNT]
    strh    r4, [r0, #RXADDR]
    strh    r4, [r0, #RXCOUNT]
    adds    r2, #0x04
    adds    r0, #(0x04 * USB_PMASTP)
    subs    r1, #1
    bpl     .L_ep_reset_loop
    movs    r2, #0x00
    strh    r2, [r3, #0x10]     // 0 -> USB->BTABLE
    movs    r1, #usbd_evt_reset
    movs    r4, #ISTRBIT(10)
.L_ep_clristr:
    lsls    r4, #ISTRSHIFT
    ldrh    r0, [r3, #4]
    bics    r0, r4
    strh    r0, [r3, #4]
.L_ep_callback:
    pop     {r0, r3, r4, r5 }
    bx      r3

    .size   _evt_poll, . - _evt_poll

    .pool

   .end

#endif //USBD_STM32F103
