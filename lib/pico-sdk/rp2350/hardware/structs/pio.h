// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_PIO_H
#define _HARDWARE_STRUCTS_PIO_H

/**
 * \file rp2350/pio.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/pio.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_pio
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/pio.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(PIO_SM0_CLKDIV_OFFSET) // PIO_SM0_CLKDIV
    // Clock divisor register for state machine 0 +
    // 0xffff0000 [31:16] INT          (0x0001) Effective frequency is sysclk/(int + frac/256)
    // 0x0000ff00 [15:8]  FRAC         (0x00) Fractional part of clock divisor
    io_rw_32 clkdiv;
 
    _REG_(PIO_SM0_EXECCTRL_OFFSET) // PIO_SM0_EXECCTRL
    // Execution/behavioural settings for state machine 0
    // 0x80000000 [31]    EXEC_STALLED (0) If 1, an instruction written to SMx_INSTR is stalled,...
    // 0x40000000 [30]    SIDE_EN      (0) If 1, the MSB of the Delay/Side-set instruction field is...
    // 0x20000000 [29]    SIDE_PINDIR  (0) If 1, side-set data is asserted to pin directions,...
    // 0x1f000000 [28:24] JMP_PIN      (0x00) The GPIO number to use as condition for JMP PIN
    // 0x00f80000 [23:19] OUT_EN_SEL   (0x00) Which data bit to use for inline OUT enable
    // 0x00040000 [18]    INLINE_OUT_EN (0) If 1, use a bit of OUT data as an auxiliary write enable +
    // 0x00020000 [17]    OUT_STICKY   (0) Continuously assert the most recent OUT/SET to the pins
    // 0x0001f000 [16:12] WRAP_TOP     (0x1f) After reaching this address, execution is wrapped to wrap_bottom
    // 0x00000f80 [11:7]  WRAP_BOTTOM  (0x00) After reaching wrap_top, execution is wrapped to this address
    // 0x00000060 [6:5]   STATUS_SEL   (0x0) Comparison used for the MOV x, STATUS instruction
    // 0x0000001f [4:0]   STATUS_N     (0x00) Comparison level or IRQ index for the MOV x, STATUS instruction
    io_rw_32 execctrl;
 
    _REG_(PIO_SM0_SHIFTCTRL_OFFSET) // PIO_SM0_SHIFTCTRL
    // Control behaviour of the input/output shift registers for state machine 0
    // 0x80000000 [31]    FJOIN_RX     (0) When 1, RX FIFO steals the TX FIFO's storage, and...
    // 0x40000000 [30]    FJOIN_TX     (0) When 1, TX FIFO steals the RX FIFO's storage, and...
    // 0x3e000000 [29:25] PULL_THRESH  (0x00) Number of bits shifted out of OSR before autopull, or...
    // 0x01f00000 [24:20] PUSH_THRESH  (0x00) Number of bits shifted into ISR before autopush, or...
    // 0x00080000 [19]    OUT_SHIFTDIR (1) 1 = shift out of output shift register to right
    // 0x00040000 [18]    IN_SHIFTDIR  (1) 1 = shift input shift register to right (data enters from left)
    // 0x00020000 [17]    AUTOPULL     (0) Pull automatically when the output shift register is emptied, i
    // 0x00010000 [16]    AUTOPUSH     (0) Push automatically when the input shift register is filled, i
    // 0x00008000 [15]    FJOIN_RX_PUT (0) If 1, disable this state machine's RX FIFO, make its...
    // 0x00004000 [14]    FJOIN_RX_GET (0) If 1, disable this state machine's RX FIFO, make its...
    // 0x0000001f [4:0]   IN_COUNT     (0x00) Set the number of pins which are not masked to 0 when...
    io_rw_32 shiftctrl;
 
    _REG_(PIO_SM0_ADDR_OFFSET) // PIO_SM0_ADDR
    // Current instruction address of state machine 0
    // 0x0000001f [4:0]   SM0_ADDR     (0x00) 
    io_ro_32 addr;
 
    _REG_(PIO_SM0_INSTR_OFFSET) // PIO_SM0_INSTR
    // Read to see the instruction currently addressed by state machine 0's program counter +
    // 0x0000ffff [15:0]  SM0_INSTR    (-) 
    io_rw_32 instr;
 
    _REG_(PIO_SM0_PINCTRL_OFFSET) // PIO_SM0_PINCTRL
    // State machine pin control
    // 0xe0000000 [31:29] SIDESET_COUNT (0x0) The number of MSBs of the Delay/Side-set instruction...
    // 0x1c000000 [28:26] SET_COUNT    (0x5) The number of pins asserted by a SET
    // 0x03f00000 [25:20] OUT_COUNT    (0x00) The number of pins asserted by an OUT PINS, OUT PINDIRS...
    // 0x000f8000 [19:15] IN_BASE      (0x00) The pin which is mapped to the least-significant bit of...
    // 0x00007c00 [14:10] SIDESET_BASE (0x00) The lowest-numbered pin that will be affected by a...
    // 0x000003e0 [9:5]   SET_BASE     (0x00) The lowest-numbered pin that will be affected by a SET...
    // 0x0000001f [4:0]   OUT_BASE     (0x00) The lowest-numbered pin that will be affected by an OUT...
    io_rw_32 pinctrl;
} pio_sm_hw_t;

typedef struct {
    _REG_(PIO_IRQ0_INTE_OFFSET) // PIO_IRQ0_INTE
    // Interrupt Enable for irq0
    // 0x00008000 [15]    SM7          (0) 
    // 0x00004000 [14]    SM6          (0) 
    // 0x00002000 [13]    SM5          (0) 
    // 0x00001000 [12]    SM4          (0) 
    // 0x00000800 [11]    SM3          (0) 
    // 0x00000400 [10]    SM2          (0) 
    // 0x00000200 [9]     SM1          (0) 
    // 0x00000100 [8]     SM0          (0) 
    // 0x00000080 [7]     SM3_TXNFULL  (0) 
    // 0x00000040 [6]     SM2_TXNFULL  (0) 
    // 0x00000020 [5]     SM1_TXNFULL  (0) 
    // 0x00000010 [4]     SM0_TXNFULL  (0) 
    // 0x00000008 [3]     SM3_RXNEMPTY (0) 
    // 0x00000004 [2]     SM2_RXNEMPTY (0) 
    // 0x00000002 [1]     SM1_RXNEMPTY (0) 
    // 0x00000001 [0]     SM0_RXNEMPTY (0) 
    io_rw_32 inte;
 
    _REG_(PIO_IRQ0_INTF_OFFSET) // PIO_IRQ0_INTF
    // Interrupt Force for irq0
    // 0x00008000 [15]    SM7          (0) 
    // 0x00004000 [14]    SM6          (0) 
    // 0x00002000 [13]    SM5          (0) 
    // 0x00001000 [12]    SM4          (0) 
    // 0x00000800 [11]    SM3          (0) 
    // 0x00000400 [10]    SM2          (0) 
    // 0x00000200 [9]     SM1          (0) 
    // 0x00000100 [8]     SM0          (0) 
    // 0x00000080 [7]     SM3_TXNFULL  (0) 
    // 0x00000040 [6]     SM2_TXNFULL  (0) 
    // 0x00000020 [5]     SM1_TXNFULL  (0) 
    // 0x00000010 [4]     SM0_TXNFULL  (0) 
    // 0x00000008 [3]     SM3_RXNEMPTY (0) 
    // 0x00000004 [2]     SM2_RXNEMPTY (0) 
    // 0x00000002 [1]     SM1_RXNEMPTY (0) 
    // 0x00000001 [0]     SM0_RXNEMPTY (0) 
    io_rw_32 intf;
 
    _REG_(PIO_IRQ0_INTS_OFFSET) // PIO_IRQ0_INTS
    // Interrupt status after masking & forcing for irq0
    // 0x00008000 [15]    SM7          (0) 
    // 0x00004000 [14]    SM6          (0) 
    // 0x00002000 [13]    SM5          (0) 
    // 0x00001000 [12]    SM4          (0) 
    // 0x00000800 [11]    SM3          (0) 
    // 0x00000400 [10]    SM2          (0) 
    // 0x00000200 [9]     SM1          (0) 
    // 0x00000100 [8]     SM0          (0) 
    // 0x00000080 [7]     SM3_TXNFULL  (0) 
    // 0x00000040 [6]     SM2_TXNFULL  (0) 
    // 0x00000020 [5]     SM1_TXNFULL  (0) 
    // 0x00000010 [4]     SM0_TXNFULL  (0) 
    // 0x00000008 [3]     SM3_RXNEMPTY (0) 
    // 0x00000004 [2]     SM2_RXNEMPTY (0) 
    // 0x00000002 [1]     SM1_RXNEMPTY (0) 
    // 0x00000001 [0]     SM0_RXNEMPTY (0) 
    io_ro_32 ints;
} pio_irq_ctrl_hw_t;

typedef struct {
    _REG_(PIO_CTRL_OFFSET) // PIO_CTRL
    // PIO control register
    // 0x04000000 [26]    NEXTPREV_CLKDIV_RESTART (0) Write 1 to restart the clock dividers of state machines...
    // 0x02000000 [25]    NEXTPREV_SM_DISABLE (0) Write 1 to disable state machines in neighbouring PIO...
    // 0x01000000 [24]    NEXTPREV_SM_ENABLE (0) Write 1 to enable state machines in neighbouring PIO...
    // 0x00f00000 [23:20] NEXT_PIO_MASK (0x0) A mask of state machines in the neighbouring...
    // 0x000f0000 [19:16] PREV_PIO_MASK (0x0) A mask of state machines in the neighbouring...
    // 0x00000f00 [11:8]  CLKDIV_RESTART (0x0) Restart a state machine's clock divider from an initial...
    // 0x000000f0 [7:4]   SM_RESTART   (0x0) Write 1 to instantly clear internal SM state which may...
    // 0x0000000f [3:0]   SM_ENABLE    (0x0) Enable/disable each of the four state machines by...
    io_rw_32 ctrl;
 
    _REG_(PIO_FSTAT_OFFSET) // PIO_FSTAT
    // FIFO status register
    // 0x0f000000 [27:24] TXEMPTY      (0xf) State machine TX FIFO is empty
    // 0x000f0000 [19:16] TXFULL       (0x0) State machine TX FIFO is full
    // 0x00000f00 [11:8]  RXEMPTY      (0xf) State machine RX FIFO is empty
    // 0x0000000f [3:0]   RXFULL       (0x0) State machine RX FIFO is full
    io_ro_32 fstat;
 
    _REG_(PIO_FDEBUG_OFFSET) // PIO_FDEBUG
    // FIFO debug register
    // 0x0f000000 [27:24] TXSTALL      (0x0) State machine has stalled on empty TX FIFO during a...
    // 0x000f0000 [19:16] TXOVER       (0x0) TX FIFO overflow (i
    // 0x00000f00 [11:8]  RXUNDER      (0x0) RX FIFO underflow (i
    // 0x0000000f [3:0]   RXSTALL      (0x0) State machine has stalled on full RX FIFO during a...
    io_rw_32 fdebug;
 
    _REG_(PIO_FLEVEL_OFFSET) // PIO_FLEVEL
    // FIFO levels
    // 0xf0000000 [31:28] RX3          (0x0) 
    // 0x0f000000 [27:24] TX3          (0x0) 
    // 0x00f00000 [23:20] RX2          (0x0) 
    // 0x000f0000 [19:16] TX2          (0x0) 
    // 0x0000f000 [15:12] RX1          (0x0) 
    // 0x00000f00 [11:8]  TX1          (0x0) 
    // 0x000000f0 [7:4]   RX0          (0x0) 
    // 0x0000000f [3:0]   TX0          (0x0) 
    io_ro_32 flevel;
 
    // (Description copied from array index 0 register PIO_TXF0 applies similarly to other array indexes)
    _REG_(PIO_TXF0_OFFSET) // PIO_TXF0
    // Direct write access to the TX FIFO for this state machine
    // 0xffffffff [31:0]  TXF0         (0x00000000) 
    io_wo_32 txf[4];
 
    // (Description copied from array index 0 register PIO_RXF0 applies similarly to other array indexes)
    _REG_(PIO_RXF0_OFFSET) // PIO_RXF0
    // Direct read access to the RX FIFO for this state machine
    // 0xffffffff [31:0]  RXF0         (-) 
    io_ro_32 rxf[4];
 
    _REG_(PIO_IRQ_OFFSET) // PIO_IRQ
    // State machine IRQ flags register
    // 0x000000ff [7:0]   IRQ          (0x00) 
    io_rw_32 irq;
 
    _REG_(PIO_IRQ_FORCE_OFFSET) // PIO_IRQ_FORCE
    // Writing a 1 to each of these bits will forcibly assert the corresponding IRQ
    // 0x000000ff [7:0]   IRQ_FORCE    (0x00) 
    io_wo_32 irq_force;
 
    _REG_(PIO_INPUT_SYNC_BYPASS_OFFSET) // PIO_INPUT_SYNC_BYPASS
    // There is a 2-flipflop synchronizer on each GPIO input, which protects PIO logic from metastabilities
    // 0xffffffff [31:0]  INPUT_SYNC_BYPASS (0x00000000) 
    io_rw_32 input_sync_bypass;
 
    _REG_(PIO_DBG_PADOUT_OFFSET) // PIO_DBG_PADOUT
    // Read to sample the pad output values PIO is currently driving to the GPIOs
    // 0xffffffff [31:0]  DBG_PADOUT   (0x00000000) 
    io_ro_32 dbg_padout;
 
    _REG_(PIO_DBG_PADOE_OFFSET) // PIO_DBG_PADOE
    // Read to sample the pad output enables (direction) PIO is currently driving to the GPIOs
    // 0xffffffff [31:0]  DBG_PADOE    (0x00000000) 
    io_ro_32 dbg_padoe;
 
    _REG_(PIO_DBG_CFGINFO_OFFSET) // PIO_DBG_CFGINFO
    // The PIO hardware has some free parameters that may vary between chip products
    // 0xf0000000 [31:28] VERSION      (0x1) Version of the core PIO hardware
    // 0x003f0000 [21:16] IMEM_SIZE    (-) The size of the instruction memory, measured in units of...
    // 0x00000f00 [11:8]  SM_COUNT     (-) The number of state machines this PIO instance is equipped with
    // 0x0000003f [5:0]   FIFO_DEPTH   (-) The depth of the state machine TX/RX FIFOs, measured in words
    io_ro_32 dbg_cfginfo;
 
    // (Description copied from array index 0 register PIO_INSTR_MEM0 applies similarly to other array indexes)
    _REG_(PIO_INSTR_MEM0_OFFSET) // PIO_INSTR_MEM0
    // Write-only access to instruction memory location 0
    // 0x0000ffff [15:0]  INSTR_MEM0   (0x0000) 
    io_wo_32 instr_mem[32];
 
    pio_sm_hw_t sm[4];
 
    // (Description copied from array index 0 register PIO_RXF0_PUTGET0 applies similarly to other array indexes)
    _REG_(PIO_RXF0_PUTGET0_OFFSET) // PIO_RXF0_PUTGET0
    // Direct read/write access to the RX FIFO on all SMs, if SHIFTCTRL_FJOIN_RX_PUT xor SHIFTCTRL_FJOIN_RX_GET is set
    // 0xffffffff [31:0]  RXF0_PUTGET0 (0x00000000) 
    io_rw_32 rxf_putget[4][4];
 
    _REG_(PIO_GPIOBASE_OFFSET) // PIO_GPIOBASE
    // Relocate GPIO 0 (from PIO's point of view) in the system GPIO numbering, to access more than 32...
    // 0x00000010 [4]     GPIOBASE     (0) 
    io_rw_32 gpiobase;
 
    _REG_(PIO_INTR_OFFSET) // PIO_INTR
    // Raw Interrupts
    // 0x00008000 [15]    SM7          (0) 
    // 0x00004000 [14]    SM6          (0) 
    // 0x00002000 [13]    SM5          (0) 
    // 0x00001000 [12]    SM4          (0) 
    // 0x00000800 [11]    SM3          (0) 
    // 0x00000400 [10]    SM2          (0) 
    // 0x00000200 [9]     SM1          (0) 
    // 0x00000100 [8]     SM0          (0) 
    // 0x00000080 [7]     SM3_TXNFULL  (0) 
    // 0x00000040 [6]     SM2_TXNFULL  (0) 
    // 0x00000020 [5]     SM1_TXNFULL  (0) 
    // 0x00000010 [4]     SM0_TXNFULL  (0) 
    // 0x00000008 [3]     SM3_RXNEMPTY (0) 
    // 0x00000004 [2]     SM2_RXNEMPTY (0) 
    // 0x00000002 [1]     SM1_RXNEMPTY (0) 
    // 0x00000001 [0]     SM0_RXNEMPTY (0) 
    io_ro_32 intr;
 
    union {
        struct {
            _REG_(PIO_IRQ0_INTE_OFFSET) // PIO_IRQ0_INTE
            // Interrupt Enable for irq0
            // 0x00000800 [11]    SM3          (0) 
            // 0x00000400 [10]    SM2          (0) 
            // 0x00000200 [9]     SM1          (0) 
            // 0x00000100 [8]     SM0          (0) 
            // 0x00000080 [7]     SM3_TXNFULL  (0) 
            // 0x00000040 [6]     SM2_TXNFULL  (0) 
            // 0x00000020 [5]     SM1_TXNFULL  (0) 
            // 0x00000010 [4]     SM0_TXNFULL  (0) 
            // 0x00000008 [3]     SM3_RXNEMPTY (0) 
            // 0x00000004 [2]     SM2_RXNEMPTY (0) 
            // 0x00000002 [1]     SM1_RXNEMPTY (0) 
            // 0x00000001 [0]     SM0_RXNEMPTY (0) 
            io_rw_32 inte0;

            _REG_(PIO_IRQ0_INTF_OFFSET) // PIO_IRQ0_INTF
            // Interrupt Force for irq0
            // 0x00000800 [11]    SM3          (0) 
            // 0x00000400 [10]    SM2          (0) 
            // 0x00000200 [9]     SM1          (0) 
            // 0x00000100 [8]     SM0          (0) 
            // 0x00000080 [7]     SM3_TXNFULL  (0) 
            // 0x00000040 [6]     SM2_TXNFULL  (0) 
            // 0x00000020 [5]     SM1_TXNFULL  (0) 
            // 0x00000010 [4]     SM0_TXNFULL  (0) 
            // 0x00000008 [3]     SM3_RXNEMPTY (0) 
            // 0x00000004 [2]     SM2_RXNEMPTY (0) 
            // 0x00000002 [1]     SM1_RXNEMPTY (0) 
            // 0x00000001 [0]     SM0_RXNEMPTY (0) 
            io_rw_32 intf0;

            _REG_(PIO_IRQ0_INTS_OFFSET) // PIO_IRQ0_INTS
            // Interrupt status after masking & forcing for irq0
            // 0x00000800 [11]    SM3          (0) 
            // 0x00000400 [10]    SM2          (0) 
            // 0x00000200 [9]     SM1          (0) 
            // 0x00000100 [8]     SM0          (0) 
            // 0x00000080 [7]     SM3_TXNFULL  (0) 
            // 0x00000040 [6]     SM2_TXNFULL  (0) 
            // 0x00000020 [5]     SM1_TXNFULL  (0) 
            // 0x00000010 [4]     SM0_TXNFULL  (0) 
            // 0x00000008 [3]     SM3_RXNEMPTY (0) 
            // 0x00000004 [2]     SM2_RXNEMPTY (0) 
            // 0x00000002 [1]     SM1_RXNEMPTY (0) 
            // 0x00000001 [0]     SM0_RXNEMPTY (0) 
            io_ro_32 ints0;

            _REG_(PIO_IRQ1_INTE_OFFSET) // PIO_IRQ1_INTE
            // Interrupt Enable for irq1
            // 0x00000800 [11]    SM3          (0) 
            // 0x00000400 [10]    SM2          (0) 
            // 0x00000200 [9]     SM1          (0) 
            // 0x00000100 [8]     SM0          (0) 
            // 0x00000080 [7]     SM3_TXNFULL  (0) 
            // 0x00000040 [6]     SM2_TXNFULL  (0) 
            // 0x00000020 [5]     SM1_TXNFULL  (0) 
            // 0x00000010 [4]     SM0_TXNFULL  (0) 
            // 0x00000008 [3]     SM3_RXNEMPTY (0) 
            // 0x00000004 [2]     SM2_RXNEMPTY (0) 
            // 0x00000002 [1]     SM1_RXNEMPTY (0) 
            // 0x00000001 [0]     SM0_RXNEMPTY (0) 
            io_rw_32 inte1;

            _REG_(PIO_IRQ1_INTF_OFFSET) // PIO_IRQ1_INTF
            // Interrupt Force for irq1
            // 0x00000800 [11]    SM3          (0) 
            // 0x00000400 [10]    SM2          (0) 
            // 0x00000200 [9]     SM1          (0) 
            // 0x00000100 [8]     SM0          (0) 
            // 0x00000080 [7]     SM3_TXNFULL  (0) 
            // 0x00000040 [6]     SM2_TXNFULL  (0) 
            // 0x00000020 [5]     SM1_TXNFULL  (0) 
            // 0x00000010 [4]     SM0_TXNFULL  (0) 
            // 0x00000008 [3]     SM3_RXNEMPTY (0) 
            // 0x00000004 [2]     SM2_RXNEMPTY (0) 
            // 0x00000002 [1]     SM1_RXNEMPTY (0) 
            // 0x00000001 [0]     SM0_RXNEMPTY (0) 
            io_rw_32 intf1;

            _REG_(PIO_IRQ1_INTS_OFFSET) // PIO_IRQ1_INTS
            // Interrupt status after masking & forcing for irq1
            // 0x00000800 [11]    SM3          (0) 
            // 0x00000400 [10]    SM2          (0) 
            // 0x00000200 [9]     SM1          (0) 
            // 0x00000100 [8]     SM0          (0) 
            // 0x00000080 [7]     SM3_TXNFULL  (0) 
            // 0x00000040 [6]     SM2_TXNFULL  (0) 
            // 0x00000020 [5]     SM1_TXNFULL  (0) 
            // 0x00000010 [4]     SM0_TXNFULL  (0) 
            // 0x00000008 [3]     SM3_RXNEMPTY (0) 
            // 0x00000004 [2]     SM2_RXNEMPTY (0) 
            // 0x00000002 [1]     SM1_RXNEMPTY (0) 
            // 0x00000001 [0]     SM0_RXNEMPTY (0) 
            io_ro_32 ints1;
        };
        pio_irq_ctrl_hw_t irq_ctrl[2];
    };
} pio_hw_t;

#define pio0_hw ((pio_hw_t *)PIO0_BASE)
#define pio1_hw ((pio_hw_t *)PIO1_BASE)
#define pio2_hw ((pio_hw_t *)PIO2_BASE)
static_assert(sizeof (pio_hw_t) == 0x0188, "");

#endif // _HARDWARE_STRUCTS_PIO_H

