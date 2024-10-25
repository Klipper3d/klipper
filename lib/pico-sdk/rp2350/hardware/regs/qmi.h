// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : QMI
// Version        : 1
// Bus type       : apb
// Description    : QSPI Memory Interface.
//
//                  Provides a memory-mapped interface to up to two
//                  SPI/DSPI/QSPI flash or PSRAM devices. Also provides a serial
//                  interface for programming and configuration of the external
//                  device.
// =============================================================================
#ifndef _HARDWARE_REGS_QMI_H
#define _HARDWARE_REGS_QMI_H
// =============================================================================
// Register    : QMI_DIRECT_CSR
// Description : Control and status for direct serial mode
//
//               Direct serial mode allows the processor to send and receive raw
//               serial frames, for programming, configuration and control of
//               the external memory devices. Only SPI mode 0 (CPOL=0 CPHA=0) is
//               supported.
#define QMI_DIRECT_CSR_OFFSET _u(0x00000000)
#define QMI_DIRECT_CSR_BITS   _u(0xffdf7ccf)
#define QMI_DIRECT_CSR_RESET  _u(0x01800000)
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_CSR_RXDELAY
// Description : Delay the read data sample timing, in units of one half of a
//               system clock cycle. (Not necessarily half of an SCK cycle.)
#define QMI_DIRECT_CSR_RXDELAY_RESET  _u(0x0)
#define QMI_DIRECT_CSR_RXDELAY_BITS   _u(0xc0000000)
#define QMI_DIRECT_CSR_RXDELAY_MSB    _u(31)
#define QMI_DIRECT_CSR_RXDELAY_LSB    _u(30)
#define QMI_DIRECT_CSR_RXDELAY_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_CSR_CLKDIV
// Description : Clock divisor for direct serial mode. Divisors of 1..255 are
//               encoded directly, and the maximum divisor of 256 is encoded by
//               a value of CLKDIV=0.
//
//               The clock divisor can be changed on-the-fly by software,
//               without halting or otherwise coordinating with the serial
//               interface. The serial interface will sample the latest clock
//               divisor each time it begins the transmission of a new byte.
#define QMI_DIRECT_CSR_CLKDIV_RESET  _u(0x06)
#define QMI_DIRECT_CSR_CLKDIV_BITS   _u(0x3fc00000)
#define QMI_DIRECT_CSR_CLKDIV_MSB    _u(29)
#define QMI_DIRECT_CSR_CLKDIV_LSB    _u(22)
#define QMI_DIRECT_CSR_CLKDIV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_CSR_RXLEVEL
// Description : Current level of DIRECT_RX FIFO
#define QMI_DIRECT_CSR_RXLEVEL_RESET  _u(0x0)
#define QMI_DIRECT_CSR_RXLEVEL_BITS   _u(0x001c0000)
#define QMI_DIRECT_CSR_RXLEVEL_MSB    _u(20)
#define QMI_DIRECT_CSR_RXLEVEL_LSB    _u(18)
#define QMI_DIRECT_CSR_RXLEVEL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_CSR_RXFULL
// Description : When 1, the DIRECT_RX FIFO is currently full. The serial
//               interface will be stalled until data is popped; the interface
//               will not begin a new serial frame when the DIRECT_TX FIFO is
//               empty or the DIRECT_RX FIFO is full.
#define QMI_DIRECT_CSR_RXFULL_RESET  _u(0x0)
#define QMI_DIRECT_CSR_RXFULL_BITS   _u(0x00020000)
#define QMI_DIRECT_CSR_RXFULL_MSB    _u(17)
#define QMI_DIRECT_CSR_RXFULL_LSB    _u(17)
#define QMI_DIRECT_CSR_RXFULL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_CSR_RXEMPTY
// Description : When 1, the DIRECT_RX FIFO is currently empty. If the processor
//               attempts to read more data, the FIFO state is not affected, but
//               the value returned to the processor is undefined.
#define QMI_DIRECT_CSR_RXEMPTY_RESET  _u(0x0)
#define QMI_DIRECT_CSR_RXEMPTY_BITS   _u(0x00010000)
#define QMI_DIRECT_CSR_RXEMPTY_MSB    _u(16)
#define QMI_DIRECT_CSR_RXEMPTY_LSB    _u(16)
#define QMI_DIRECT_CSR_RXEMPTY_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_CSR_TXLEVEL
// Description : Current level of DIRECT_TX FIFO
#define QMI_DIRECT_CSR_TXLEVEL_RESET  _u(0x0)
#define QMI_DIRECT_CSR_TXLEVEL_BITS   _u(0x00007000)
#define QMI_DIRECT_CSR_TXLEVEL_MSB    _u(14)
#define QMI_DIRECT_CSR_TXLEVEL_LSB    _u(12)
#define QMI_DIRECT_CSR_TXLEVEL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_CSR_TXEMPTY
// Description : When 1, the DIRECT_TX FIFO is currently empty. Unless the
//               processor pushes more data, transmission will stop and BUSY
//               will go low once the current 8-bit serial frame completes.
#define QMI_DIRECT_CSR_TXEMPTY_RESET  _u(0x0)
#define QMI_DIRECT_CSR_TXEMPTY_BITS   _u(0x00000800)
#define QMI_DIRECT_CSR_TXEMPTY_MSB    _u(11)
#define QMI_DIRECT_CSR_TXEMPTY_LSB    _u(11)
#define QMI_DIRECT_CSR_TXEMPTY_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_CSR_TXFULL
// Description : When 1, the DIRECT_TX FIFO is currently full. If the processor
//               tries to write more data, that data will be ignored.
#define QMI_DIRECT_CSR_TXFULL_RESET  _u(0x0)
#define QMI_DIRECT_CSR_TXFULL_BITS   _u(0x00000400)
#define QMI_DIRECT_CSR_TXFULL_MSB    _u(10)
#define QMI_DIRECT_CSR_TXFULL_LSB    _u(10)
#define QMI_DIRECT_CSR_TXFULL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_CSR_AUTO_CS1N
// Description : When 1, automatically assert the CS1n chip select line whenever
//               the BUSY flag is set.
#define QMI_DIRECT_CSR_AUTO_CS1N_RESET  _u(0x0)
#define QMI_DIRECT_CSR_AUTO_CS1N_BITS   _u(0x00000080)
#define QMI_DIRECT_CSR_AUTO_CS1N_MSB    _u(7)
#define QMI_DIRECT_CSR_AUTO_CS1N_LSB    _u(7)
#define QMI_DIRECT_CSR_AUTO_CS1N_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_CSR_AUTO_CS0N
// Description : When 1, automatically assert the CS0n chip select line whenever
//               the BUSY flag is set.
#define QMI_DIRECT_CSR_AUTO_CS0N_RESET  _u(0x0)
#define QMI_DIRECT_CSR_AUTO_CS0N_BITS   _u(0x00000040)
#define QMI_DIRECT_CSR_AUTO_CS0N_MSB    _u(6)
#define QMI_DIRECT_CSR_AUTO_CS0N_LSB    _u(6)
#define QMI_DIRECT_CSR_AUTO_CS0N_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_CSR_ASSERT_CS1N
// Description : When 1, assert (i.e. drive low) the CS1n chip select line.
//
//               Note that this applies even when DIRECT_CSR_EN is 0.
#define QMI_DIRECT_CSR_ASSERT_CS1N_RESET  _u(0x0)
#define QMI_DIRECT_CSR_ASSERT_CS1N_BITS   _u(0x00000008)
#define QMI_DIRECT_CSR_ASSERT_CS1N_MSB    _u(3)
#define QMI_DIRECT_CSR_ASSERT_CS1N_LSB    _u(3)
#define QMI_DIRECT_CSR_ASSERT_CS1N_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_CSR_ASSERT_CS0N
// Description : When 1, assert (i.e. drive low) the CS0n chip select line.
//
//               Note that this applies even when DIRECT_CSR_EN is 0.
#define QMI_DIRECT_CSR_ASSERT_CS0N_RESET  _u(0x0)
#define QMI_DIRECT_CSR_ASSERT_CS0N_BITS   _u(0x00000004)
#define QMI_DIRECT_CSR_ASSERT_CS0N_MSB    _u(2)
#define QMI_DIRECT_CSR_ASSERT_CS0N_LSB    _u(2)
#define QMI_DIRECT_CSR_ASSERT_CS0N_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_CSR_BUSY
// Description : Direct mode busy flag. If 1, data is currently being shifted
//               in/out (or would be if the interface were not stalled on the RX
//               FIFO), and the chip select must not yet be deasserted.
//
//               The busy flag will also be set to 1 if a memory-mapped transfer
//               is still in progress when direct mode is enabled. Direct mode
//               blocks new memory-mapped transfers, but can't halt a transfer
//               that is already in progress. If there is a chance that memory-
//               mapped transfers may be in progress, the busy flag should be
//               polled for 0 before asserting the chip select.
//
//               (In practice you will usually discover this timing condition
//               through other means, because any subsequent memory-mapped
//               transfers when direct mode is enabled will return bus errors,
//               which are difficult to ignore.)
#define QMI_DIRECT_CSR_BUSY_RESET  _u(0x0)
#define QMI_DIRECT_CSR_BUSY_BITS   _u(0x00000002)
#define QMI_DIRECT_CSR_BUSY_MSB    _u(1)
#define QMI_DIRECT_CSR_BUSY_LSB    _u(1)
#define QMI_DIRECT_CSR_BUSY_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_CSR_EN
// Description : Enable direct mode.
//
//               In direct mode, software controls the chip select lines, and
//               can perform direct SPI transfers by pushing data to the
//               DIRECT_TX FIFO, and popping the same amount of data from the
//               DIRECT_RX FIFO.
//
//               Memory-mapped accesses will generate bus errors when direct
//               serial mode is enabled.
#define QMI_DIRECT_CSR_EN_RESET  _u(0x0)
#define QMI_DIRECT_CSR_EN_BITS   _u(0x00000001)
#define QMI_DIRECT_CSR_EN_MSB    _u(0)
#define QMI_DIRECT_CSR_EN_LSB    _u(0)
#define QMI_DIRECT_CSR_EN_ACCESS "RW"
// =============================================================================
// Register    : QMI_DIRECT_TX
// Description : Transmit FIFO for direct mode
#define QMI_DIRECT_TX_OFFSET _u(0x00000004)
#define QMI_DIRECT_TX_BITS   _u(0x001fffff)
#define QMI_DIRECT_TX_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_TX_NOPUSH
// Description : Inhibit the RX FIFO push that would correspond to this TX FIFO
//               entry.
//
//               Useful to avoid garbage appearing in the RX FIFO when pushing
//               the command at the beginning of a SPI transfer.
#define QMI_DIRECT_TX_NOPUSH_RESET  _u(0x0)
#define QMI_DIRECT_TX_NOPUSH_BITS   _u(0x00100000)
#define QMI_DIRECT_TX_NOPUSH_MSB    _u(20)
#define QMI_DIRECT_TX_NOPUSH_LSB    _u(20)
#define QMI_DIRECT_TX_NOPUSH_ACCESS "WF"
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_TX_OE
// Description : Output enable (active-high). For single width (SPI), this field
//               is ignored, and SD0 is always set to output, with SD1 always
//               set to input.
//
//               For dual and quad width (DSPI/QSPI), this sets whether the
//               relevant SDx pads are set to output whilst transferring this
//               FIFO record. In this case the command/address should have OE
//               set, and the data transfer should have OE set or clear
//               depending on the direction of the transfer.
#define QMI_DIRECT_TX_OE_RESET  _u(0x0)
#define QMI_DIRECT_TX_OE_BITS   _u(0x00080000)
#define QMI_DIRECT_TX_OE_MSB    _u(19)
#define QMI_DIRECT_TX_OE_LSB    _u(19)
#define QMI_DIRECT_TX_OE_ACCESS "WF"
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_TX_DWIDTH
// Description : Data width. If 0, hardware will transmit the 8 LSBs of the
//               DIRECT_TX DATA field, and return an 8-bit value in the 8 LSBs
//               of DIRECT_RX. If 1, the full 16-bit width is used. 8-bit and
//               16-bit transfers can be mixed freely.
#define QMI_DIRECT_TX_DWIDTH_RESET  _u(0x0)
#define QMI_DIRECT_TX_DWIDTH_BITS   _u(0x00040000)
#define QMI_DIRECT_TX_DWIDTH_MSB    _u(18)
#define QMI_DIRECT_TX_DWIDTH_LSB    _u(18)
#define QMI_DIRECT_TX_DWIDTH_ACCESS "WF"
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_TX_IWIDTH
// Description : Configure whether this FIFO record is transferred with
//               single/dual/quad interface width (0/1/2). Different widths can
//               be mixed freely.
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_DIRECT_TX_IWIDTH_RESET  _u(0x0)
#define QMI_DIRECT_TX_IWIDTH_BITS   _u(0x00030000)
#define QMI_DIRECT_TX_IWIDTH_MSB    _u(17)
#define QMI_DIRECT_TX_IWIDTH_LSB    _u(16)
#define QMI_DIRECT_TX_IWIDTH_ACCESS "WF"
#define QMI_DIRECT_TX_IWIDTH_VALUE_S _u(0x0)
#define QMI_DIRECT_TX_IWIDTH_VALUE_D _u(0x1)
#define QMI_DIRECT_TX_IWIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_DIRECT_TX_DATA
// Description : Data pushed here will be clocked out falling edges of SCK (or
//               before the very first rising edge of SCK, if this is the first
//               pulse). For each byte clocked out, the interface will
//               simultaneously sample one byte, on rising edges of SCK, and
//               push this to the DIRECT_RX FIFO.
//
//               For 16-bit data, the least-significant byte is transmitted
//               first.
#define QMI_DIRECT_TX_DATA_RESET  _u(0x0000)
#define QMI_DIRECT_TX_DATA_BITS   _u(0x0000ffff)
#define QMI_DIRECT_TX_DATA_MSB    _u(15)
#define QMI_DIRECT_TX_DATA_LSB    _u(0)
#define QMI_DIRECT_TX_DATA_ACCESS "WF"
// =============================================================================
// Register    : QMI_DIRECT_RX
// Description : Receive FIFO for direct mode
//               With each byte clocked out on the serial interface, one byte
//               will simultaneously be clocked in, and will appear in this
//               FIFO. The serial interface will stall when this FIFO is full,
//               to avoid dropping data.
//
//               When 16-bit data is pushed into the TX FIFO, the corresponding
//               RX FIFO push will also contain 16 bits of data. The least-
//               significant byte is the first one received.
#define QMI_DIRECT_RX_OFFSET _u(0x00000008)
#define QMI_DIRECT_RX_BITS   _u(0x0000ffff)
#define QMI_DIRECT_RX_RESET  _u(0x00000000)
#define QMI_DIRECT_RX_MSB    _u(15)
#define QMI_DIRECT_RX_LSB    _u(0)
#define QMI_DIRECT_RX_ACCESS "RF"
// =============================================================================
// Register    : QMI_M0_TIMING
// Description : Timing configuration register for memory address window 0.
#define QMI_M0_TIMING_OFFSET _u(0x0000000c)
#define QMI_M0_TIMING_BITS   _u(0xf3fff7ff)
#define QMI_M0_TIMING_RESET  _u(0x40000004)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_TIMING_COOLDOWN
// Description : Chip select cooldown period. When a memory transfer finishes,
//               the chip select remains asserted for 64 x COOLDOWN system clock
//               cycles, plus half an SCK clock period (rounded up for odd SCK
//               divisors). After this cooldown expires, the chip select is
//               always deasserted to save power.
//
//               If the next memory access arrives within the cooldown period,
//               the QMI may be able to append more SCK cycles to the currently
//               ongoing SPI transfer, rather than starting a new transfer. This
//               reduces access latency and increases bus throughput.
//
//               Specifically, the next access must be in the same direction
//               (read/write), access the same memory window (chip select 0/1),
//               and follow sequentially the address of the last transfer. If
//               any of these are false, the new access will first deassert the
//               chip select, then begin a new transfer.
//
//               If COOLDOWN is 0, the address alignment configured by PAGEBREAK
//               has been reached, or the total chip select assertion limit
//               MAX_SELECT has been reached, the cooldown period is skipped,
//               and the chip select will always be deasserted one half SCK
//               period after the transfer finishes.
#define QMI_M0_TIMING_COOLDOWN_RESET  _u(0x1)
#define QMI_M0_TIMING_COOLDOWN_BITS   _u(0xc0000000)
#define QMI_M0_TIMING_COOLDOWN_MSB    _u(31)
#define QMI_M0_TIMING_COOLDOWN_LSB    _u(30)
#define QMI_M0_TIMING_COOLDOWN_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M0_TIMING_PAGEBREAK
// Description : When page break is enabled, chip select will automatically
//               deassert when crossing certain power-of-2-aligned address
//               boundaries. The next access will always begin a new read/write
//               SPI burst, even if the address of the next access follows in
//               sequence with the last access before the page boundary.
//
//               Some flash and PSRAM devices forbid crossing page boundaries
//               with a single read/write transfer, or restrict the operating
//               frequency for transfers that do cross page a boundary. This
//               option allows the QMI to safely support those devices.
//
//               This field has no effect when COOLDOWN is disabled.
//               0x0 -> No page boundary is enforced
//               0x1 -> Break bursts crossing a 256-byte page boundary
//               0x2 -> Break bursts crossing a 1024-byte quad-page boundary
//               0x3 -> Break bursts crossing a 4096-byte sector boundary
#define QMI_M0_TIMING_PAGEBREAK_RESET  _u(0x0)
#define QMI_M0_TIMING_PAGEBREAK_BITS   _u(0x30000000)
#define QMI_M0_TIMING_PAGEBREAK_MSB    _u(29)
#define QMI_M0_TIMING_PAGEBREAK_LSB    _u(28)
#define QMI_M0_TIMING_PAGEBREAK_ACCESS "RW"
#define QMI_M0_TIMING_PAGEBREAK_VALUE_NONE _u(0x0)
#define QMI_M0_TIMING_PAGEBREAK_VALUE_256 _u(0x1)
#define QMI_M0_TIMING_PAGEBREAK_VALUE_1024 _u(0x2)
#define QMI_M0_TIMING_PAGEBREAK_VALUE_4096 _u(0x3)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_TIMING_SELECT_SETUP
// Description : Add up to one additional system clock cycle of setup between
//               chip select assertion and the first rising edge of SCK.
//
//               The default setup time is one half SCK period, which is usually
//               sufficient except for very high SCK frequencies with some flash
//               devices.
#define QMI_M0_TIMING_SELECT_SETUP_RESET  _u(0x0)
#define QMI_M0_TIMING_SELECT_SETUP_BITS   _u(0x02000000)
#define QMI_M0_TIMING_SELECT_SETUP_MSB    _u(25)
#define QMI_M0_TIMING_SELECT_SETUP_LSB    _u(25)
#define QMI_M0_TIMING_SELECT_SETUP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M0_TIMING_SELECT_HOLD
// Description : Add up to three additional system clock cycles of active hold
//               between the last falling edge of SCK and the deassertion of
//               this window's chip select.
//
//               The default hold time is one system clock cycle. Note that
//               flash datasheets usually give chip select active hold time from
//               the last *rising* edge of SCK, and so even zero hold from the
//               last falling edge would be safe.
//
//               Note that this is a minimum hold time guaranteed by the QMI:
//               the actual chip select active hold may be slightly longer for
//               read transfers with low clock divisors and/or high sample
//               delays. Specifically, if the point two cycles after the last RX
//               data sample is later than the last SCK falling edge, then the
//               hold time is measured from *this* point.
//
//               Note also that, in case the final SCK pulse is masked to save
//               energy (true for non-DTR reads when COOLDOWN is disabled or
//               PAGE_BREAK is reached), all of QMI's timing logic behaves as
//               though the clock pulse were still present. The SELECT_HOLD time
//               is applied from the point where the last SCK falling edge would
//               be if the clock pulse were not masked.
#define QMI_M0_TIMING_SELECT_HOLD_RESET  _u(0x0)
#define QMI_M0_TIMING_SELECT_HOLD_BITS   _u(0x01800000)
#define QMI_M0_TIMING_SELECT_HOLD_MSB    _u(24)
#define QMI_M0_TIMING_SELECT_HOLD_LSB    _u(23)
#define QMI_M0_TIMING_SELECT_HOLD_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M0_TIMING_MAX_SELECT
// Description : Enforce a maximum assertion duration for this window's chip
//               select, in units of 64 system clock cycles. If 0, the QMI is
//               permitted to keep the chip select asserted indefinitely when
//               servicing sequential memory accesses (see COOLDOWN).
//
//               This feature is required to meet timing constraints of PSRAM
//               devices, which specify a maximum chip select assertion so they
//               can perform DRAM refresh cycles. See also MIN_DESELECT, which
//               can enforce a minimum deselect time.
//
//               If a memory access is in progress at the time MAX_SELECT is
//               reached, the QMI will wait for the access to complete before
//               deasserting the chip select. This additional time must be
//               accounted for to calculate a safe MAX_SELECT value. In the
//               worst case, this may be a fully-formed serial transfer,
//               including command prefix and address, with a data payload as
//               large as one cache line.
#define QMI_M0_TIMING_MAX_SELECT_RESET  _u(0x00)
#define QMI_M0_TIMING_MAX_SELECT_BITS   _u(0x007e0000)
#define QMI_M0_TIMING_MAX_SELECT_MSB    _u(22)
#define QMI_M0_TIMING_MAX_SELECT_LSB    _u(17)
#define QMI_M0_TIMING_MAX_SELECT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M0_TIMING_MIN_DESELECT
// Description : After this window's chip select is deasserted, it remains
//               deasserted for half an SCK cycle (rounded up to an integer
//               number of system clock cycles), plus MIN_DESELECT additional
//               system clock cycles, before the QMI reasserts either chip
//               select pin.
//
//               Nonzero values may be required for PSRAM devices which enforce
//               a longer minimum CS deselect time, so that they can perform
//               internal DRAM refresh cycles whilst deselected.
#define QMI_M0_TIMING_MIN_DESELECT_RESET  _u(0x00)
#define QMI_M0_TIMING_MIN_DESELECT_BITS   _u(0x0001f000)
#define QMI_M0_TIMING_MIN_DESELECT_MSB    _u(16)
#define QMI_M0_TIMING_MIN_DESELECT_LSB    _u(12)
#define QMI_M0_TIMING_MIN_DESELECT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M0_TIMING_RXDELAY
// Description : Delay the read data sample timing, in units of one half of a
//               system clock cycle. (Not necessarily half of an SCK cycle.) An
//               RXDELAY of 0 means the sample is captured at the SDI input
//               registers simultaneously with the rising edge of SCK launched
//               from the SCK output register.
//
//               At higher SCK frequencies, RXDELAY may need to be increased to
//               account for the round trip delay of the pads, and the clock-
//               to-Q delay of the QSPI memory device.
#define QMI_M0_TIMING_RXDELAY_RESET  _u(0x0)
#define QMI_M0_TIMING_RXDELAY_BITS   _u(0x00000700)
#define QMI_M0_TIMING_RXDELAY_MSB    _u(10)
#define QMI_M0_TIMING_RXDELAY_LSB    _u(8)
#define QMI_M0_TIMING_RXDELAY_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M0_TIMING_CLKDIV
// Description : Clock divisor. Odd and even divisors are supported. Defines the
//               SCK clock period in units of 1 system clock cycle. Divisors
//               1..255 are encoded directly, and a divisor of 256 is encoded
//               with a value of CLKDIV=0.
//
//               The clock divisor can be changed on-the-fly, even when the QMI
//               is currently accessing memory in this address window. All other
//               parameters must only be changed when the QMI is idle.
//
//               If software is increasing CLKDIV in anticipation of an increase
//               in the system clock frequency, a dummy access to either memory
//               window (and appropriate processor barriers/fences) must be
//               inserted after the Mx_TIMING write to ensure the SCK divisor
//               change is in effect _before_ the system clock is changed.
#define QMI_M0_TIMING_CLKDIV_RESET  _u(0x04)
#define QMI_M0_TIMING_CLKDIV_BITS   _u(0x000000ff)
#define QMI_M0_TIMING_CLKDIV_MSB    _u(7)
#define QMI_M0_TIMING_CLKDIV_LSB    _u(0)
#define QMI_M0_TIMING_CLKDIV_ACCESS "RW"
// =============================================================================
// Register    : QMI_M0_RFMT
// Description : Read transfer format configuration for memory address window 0.
//
//               Configure the bus width of each transfer phase individually,
//               and configure the length or presence of the command prefix,
//               command suffix and dummy/turnaround transfer phases. Only
//               24-bit addresses are supported.
//
//               The reset value of the M0_RFMT register is configured to
//               support a basic 03h serial read transfer with no additional
//               configuration.
#define QMI_M0_RFMT_OFFSET _u(0x00000010)
#define QMI_M0_RFMT_BITS   _u(0x1007d3ff)
#define QMI_M0_RFMT_RESET  _u(0x00001000)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_RFMT_DTR
// Description : Enable double transfer rate (DTR) for read commands: address,
//               suffix and read data phases are active on both edges of SCK.
//               SDO data is launched centre-aligned on each SCK edge, and SDI
//               data is captured on the SCK edge that follows its launch.
//
//               DTR is implemented by halving the clock rate; SCK has a period
//               of 2 x CLK_DIV throughout the transfer. The prefix and dummy
//               phases are still single transfer rate.
//
//               If the suffix is quad-width, it must be 0 or 8 bits in length,
//               to ensure an even number of SCK edges.
#define QMI_M0_RFMT_DTR_RESET  _u(0x0)
#define QMI_M0_RFMT_DTR_BITS   _u(0x10000000)
#define QMI_M0_RFMT_DTR_MSB    _u(28)
#define QMI_M0_RFMT_DTR_LSB    _u(28)
#define QMI_M0_RFMT_DTR_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M0_RFMT_DUMMY_LEN
// Description : Length of dummy phase between command suffix and data phase, in
//               units of 4 bits. (i.e. 1 cycle for quad width, 2 for dual, 4
//               for single)
//               0x0 -> No dummy phase
//               0x1 -> 4 dummy bits
//               0x2 -> 8 dummy bits
//               0x3 -> 12 dummy bits
//               0x4 -> 16 dummy bits
//               0x5 -> 20 dummy bits
//               0x6 -> 24 dummy bits
//               0x7 -> 28 dummy bits
#define QMI_M0_RFMT_DUMMY_LEN_RESET  _u(0x0)
#define QMI_M0_RFMT_DUMMY_LEN_BITS   _u(0x00070000)
#define QMI_M0_RFMT_DUMMY_LEN_MSB    _u(18)
#define QMI_M0_RFMT_DUMMY_LEN_LSB    _u(16)
#define QMI_M0_RFMT_DUMMY_LEN_ACCESS "RW"
#define QMI_M0_RFMT_DUMMY_LEN_VALUE_NONE _u(0x0)
#define QMI_M0_RFMT_DUMMY_LEN_VALUE_4 _u(0x1)
#define QMI_M0_RFMT_DUMMY_LEN_VALUE_8 _u(0x2)
#define QMI_M0_RFMT_DUMMY_LEN_VALUE_12 _u(0x3)
#define QMI_M0_RFMT_DUMMY_LEN_VALUE_16 _u(0x4)
#define QMI_M0_RFMT_DUMMY_LEN_VALUE_20 _u(0x5)
#define QMI_M0_RFMT_DUMMY_LEN_VALUE_24 _u(0x6)
#define QMI_M0_RFMT_DUMMY_LEN_VALUE_28 _u(0x7)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_RFMT_SUFFIX_LEN
// Description : Length of post-address command suffix, in units of 4 bits.
//               (i.e. 1 cycle for quad width, 2 for dual, 4 for single)
//
//               Only values of 0 and 8 bits are supported.
//               0x0 -> No suffix
//               0x2 -> 8-bit suffix
#define QMI_M0_RFMT_SUFFIX_LEN_RESET  _u(0x0)
#define QMI_M0_RFMT_SUFFIX_LEN_BITS   _u(0x0000c000)
#define QMI_M0_RFMT_SUFFIX_LEN_MSB    _u(15)
#define QMI_M0_RFMT_SUFFIX_LEN_LSB    _u(14)
#define QMI_M0_RFMT_SUFFIX_LEN_ACCESS "RW"
#define QMI_M0_RFMT_SUFFIX_LEN_VALUE_NONE _u(0x0)
#define QMI_M0_RFMT_SUFFIX_LEN_VALUE_8 _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_RFMT_PREFIX_LEN
// Description : Length of command prefix, in units of 8 bits. (i.e. 2 cycles
//               for quad width, 4 for dual, 8 for single)
//               0x0 -> No prefix
//               0x1 -> 8-bit prefix
#define QMI_M0_RFMT_PREFIX_LEN_RESET  _u(0x1)
#define QMI_M0_RFMT_PREFIX_LEN_BITS   _u(0x00001000)
#define QMI_M0_RFMT_PREFIX_LEN_MSB    _u(12)
#define QMI_M0_RFMT_PREFIX_LEN_LSB    _u(12)
#define QMI_M0_RFMT_PREFIX_LEN_ACCESS "RW"
#define QMI_M0_RFMT_PREFIX_LEN_VALUE_NONE _u(0x0)
#define QMI_M0_RFMT_PREFIX_LEN_VALUE_8 _u(0x1)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_RFMT_DATA_WIDTH
// Description : The width used for the data transfer
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M0_RFMT_DATA_WIDTH_RESET  _u(0x0)
#define QMI_M0_RFMT_DATA_WIDTH_BITS   _u(0x00000300)
#define QMI_M0_RFMT_DATA_WIDTH_MSB    _u(9)
#define QMI_M0_RFMT_DATA_WIDTH_LSB    _u(8)
#define QMI_M0_RFMT_DATA_WIDTH_ACCESS "RW"
#define QMI_M0_RFMT_DATA_WIDTH_VALUE_S _u(0x0)
#define QMI_M0_RFMT_DATA_WIDTH_VALUE_D _u(0x1)
#define QMI_M0_RFMT_DATA_WIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_RFMT_DUMMY_WIDTH
// Description : The width used for the dummy phase, if any.
//
//               If width is single, SD0/MOSI is held asserted low during the
//               dummy phase, and SD1...SD3 are tristated. If width is
//               dual/quad, all IOs are tristated during the dummy phase.
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M0_RFMT_DUMMY_WIDTH_RESET  _u(0x0)
#define QMI_M0_RFMT_DUMMY_WIDTH_BITS   _u(0x000000c0)
#define QMI_M0_RFMT_DUMMY_WIDTH_MSB    _u(7)
#define QMI_M0_RFMT_DUMMY_WIDTH_LSB    _u(6)
#define QMI_M0_RFMT_DUMMY_WIDTH_ACCESS "RW"
#define QMI_M0_RFMT_DUMMY_WIDTH_VALUE_S _u(0x0)
#define QMI_M0_RFMT_DUMMY_WIDTH_VALUE_D _u(0x1)
#define QMI_M0_RFMT_DUMMY_WIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_RFMT_SUFFIX_WIDTH
// Description : The width used for the post-address command suffix, if any
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M0_RFMT_SUFFIX_WIDTH_RESET  _u(0x0)
#define QMI_M0_RFMT_SUFFIX_WIDTH_BITS   _u(0x00000030)
#define QMI_M0_RFMT_SUFFIX_WIDTH_MSB    _u(5)
#define QMI_M0_RFMT_SUFFIX_WIDTH_LSB    _u(4)
#define QMI_M0_RFMT_SUFFIX_WIDTH_ACCESS "RW"
#define QMI_M0_RFMT_SUFFIX_WIDTH_VALUE_S _u(0x0)
#define QMI_M0_RFMT_SUFFIX_WIDTH_VALUE_D _u(0x1)
#define QMI_M0_RFMT_SUFFIX_WIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_RFMT_ADDR_WIDTH
// Description : The transfer width used for the address. The address phase
//               always transfers 24 bits in total.
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M0_RFMT_ADDR_WIDTH_RESET  _u(0x0)
#define QMI_M0_RFMT_ADDR_WIDTH_BITS   _u(0x0000000c)
#define QMI_M0_RFMT_ADDR_WIDTH_MSB    _u(3)
#define QMI_M0_RFMT_ADDR_WIDTH_LSB    _u(2)
#define QMI_M0_RFMT_ADDR_WIDTH_ACCESS "RW"
#define QMI_M0_RFMT_ADDR_WIDTH_VALUE_S _u(0x0)
#define QMI_M0_RFMT_ADDR_WIDTH_VALUE_D _u(0x1)
#define QMI_M0_RFMT_ADDR_WIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_RFMT_PREFIX_WIDTH
// Description : The transfer width used for the command prefix, if any
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M0_RFMT_PREFIX_WIDTH_RESET  _u(0x0)
#define QMI_M0_RFMT_PREFIX_WIDTH_BITS   _u(0x00000003)
#define QMI_M0_RFMT_PREFIX_WIDTH_MSB    _u(1)
#define QMI_M0_RFMT_PREFIX_WIDTH_LSB    _u(0)
#define QMI_M0_RFMT_PREFIX_WIDTH_ACCESS "RW"
#define QMI_M0_RFMT_PREFIX_WIDTH_VALUE_S _u(0x0)
#define QMI_M0_RFMT_PREFIX_WIDTH_VALUE_D _u(0x1)
#define QMI_M0_RFMT_PREFIX_WIDTH_VALUE_Q _u(0x2)
// =============================================================================
// Register    : QMI_M0_RCMD
// Description : Command constants used for reads from memory address window 0.
//
//               The reset value of the M0_RCMD register is configured to
//               support a basic 03h serial read transfer with no additional
//               configuration.
#define QMI_M0_RCMD_OFFSET _u(0x00000014)
#define QMI_M0_RCMD_BITS   _u(0x0000ffff)
#define QMI_M0_RCMD_RESET  _u(0x0000a003)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_RCMD_SUFFIX
// Description : The command suffix bits following the address, if
//               Mx_RFMT_SUFFIX_LEN is nonzero.
#define QMI_M0_RCMD_SUFFIX_RESET  _u(0xa0)
#define QMI_M0_RCMD_SUFFIX_BITS   _u(0x0000ff00)
#define QMI_M0_RCMD_SUFFIX_MSB    _u(15)
#define QMI_M0_RCMD_SUFFIX_LSB    _u(8)
#define QMI_M0_RCMD_SUFFIX_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M0_RCMD_PREFIX
// Description : The command prefix bits to prepend on each new transfer, if
//               Mx_RFMT_PREFIX_LEN is nonzero.
#define QMI_M0_RCMD_PREFIX_RESET  _u(0x03)
#define QMI_M0_RCMD_PREFIX_BITS   _u(0x000000ff)
#define QMI_M0_RCMD_PREFIX_MSB    _u(7)
#define QMI_M0_RCMD_PREFIX_LSB    _u(0)
#define QMI_M0_RCMD_PREFIX_ACCESS "RW"
// =============================================================================
// Register    : QMI_M0_WFMT
// Description : Write transfer format configuration for memory address window
//               0.
//
//               Configure the bus width of each transfer phase individually,
//               and configure the length or presence of the command prefix,
//               command suffix and dummy/turnaround transfer phases. Only
//               24-bit addresses are supported.
//
//               The reset value of the M0_WFMT register is configured to
//               support a basic 02h serial write transfer. However, writes to
//               this window must first be enabled via the XIP_CTRL_WRITABLE_M0
//               bit, as XIP memory is read-only by default.
#define QMI_M0_WFMT_OFFSET _u(0x00000018)
#define QMI_M0_WFMT_BITS   _u(0x1007d3ff)
#define QMI_M0_WFMT_RESET  _u(0x00001000)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_WFMT_DTR
// Description : Enable double transfer rate (DTR) for write commands: address,
//               suffix and write data phases are active on both edges of SCK.
//               SDO data is launched centre-aligned on each SCK edge, and SDI
//               data is captured on the SCK edge that follows its launch.
//
//               DTR is implemented by halving the clock rate; SCK has a period
//               of 2 x CLK_DIV throughout the transfer. The prefix and dummy
//               phases are still single transfer rate.
//
//               If the suffix is quad-width, it must be 0 or 8 bits in length,
//               to ensure an even number of SCK edges.
#define QMI_M0_WFMT_DTR_RESET  _u(0x0)
#define QMI_M0_WFMT_DTR_BITS   _u(0x10000000)
#define QMI_M0_WFMT_DTR_MSB    _u(28)
#define QMI_M0_WFMT_DTR_LSB    _u(28)
#define QMI_M0_WFMT_DTR_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M0_WFMT_DUMMY_LEN
// Description : Length of dummy phase between command suffix and data phase, in
//               units of 4 bits. (i.e. 1 cycle for quad width, 2 for dual, 4
//               for single)
//               0x0 -> No dummy phase
//               0x1 -> 4 dummy bits
//               0x2 -> 8 dummy bits
//               0x3 -> 12 dummy bits
//               0x4 -> 16 dummy bits
//               0x5 -> 20 dummy bits
//               0x6 -> 24 dummy bits
//               0x7 -> 28 dummy bits
#define QMI_M0_WFMT_DUMMY_LEN_RESET  _u(0x0)
#define QMI_M0_WFMT_DUMMY_LEN_BITS   _u(0x00070000)
#define QMI_M0_WFMT_DUMMY_LEN_MSB    _u(18)
#define QMI_M0_WFMT_DUMMY_LEN_LSB    _u(16)
#define QMI_M0_WFMT_DUMMY_LEN_ACCESS "RW"
#define QMI_M0_WFMT_DUMMY_LEN_VALUE_NONE _u(0x0)
#define QMI_M0_WFMT_DUMMY_LEN_VALUE_4 _u(0x1)
#define QMI_M0_WFMT_DUMMY_LEN_VALUE_8 _u(0x2)
#define QMI_M0_WFMT_DUMMY_LEN_VALUE_12 _u(0x3)
#define QMI_M0_WFMT_DUMMY_LEN_VALUE_16 _u(0x4)
#define QMI_M0_WFMT_DUMMY_LEN_VALUE_20 _u(0x5)
#define QMI_M0_WFMT_DUMMY_LEN_VALUE_24 _u(0x6)
#define QMI_M0_WFMT_DUMMY_LEN_VALUE_28 _u(0x7)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_WFMT_SUFFIX_LEN
// Description : Length of post-address command suffix, in units of 4 bits.
//               (i.e. 1 cycle for quad width, 2 for dual, 4 for single)
//
//               Only values of 0 and 8 bits are supported.
//               0x0 -> No suffix
//               0x2 -> 8-bit suffix
#define QMI_M0_WFMT_SUFFIX_LEN_RESET  _u(0x0)
#define QMI_M0_WFMT_SUFFIX_LEN_BITS   _u(0x0000c000)
#define QMI_M0_WFMT_SUFFIX_LEN_MSB    _u(15)
#define QMI_M0_WFMT_SUFFIX_LEN_LSB    _u(14)
#define QMI_M0_WFMT_SUFFIX_LEN_ACCESS "RW"
#define QMI_M0_WFMT_SUFFIX_LEN_VALUE_NONE _u(0x0)
#define QMI_M0_WFMT_SUFFIX_LEN_VALUE_8 _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_WFMT_PREFIX_LEN
// Description : Length of command prefix, in units of 8 bits. (i.e. 2 cycles
//               for quad width, 4 for dual, 8 for single)
//               0x0 -> No prefix
//               0x1 -> 8-bit prefix
#define QMI_M0_WFMT_PREFIX_LEN_RESET  _u(0x1)
#define QMI_M0_WFMT_PREFIX_LEN_BITS   _u(0x00001000)
#define QMI_M0_WFMT_PREFIX_LEN_MSB    _u(12)
#define QMI_M0_WFMT_PREFIX_LEN_LSB    _u(12)
#define QMI_M0_WFMT_PREFIX_LEN_ACCESS "RW"
#define QMI_M0_WFMT_PREFIX_LEN_VALUE_NONE _u(0x0)
#define QMI_M0_WFMT_PREFIX_LEN_VALUE_8 _u(0x1)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_WFMT_DATA_WIDTH
// Description : The width used for the data transfer
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M0_WFMT_DATA_WIDTH_RESET  _u(0x0)
#define QMI_M0_WFMT_DATA_WIDTH_BITS   _u(0x00000300)
#define QMI_M0_WFMT_DATA_WIDTH_MSB    _u(9)
#define QMI_M0_WFMT_DATA_WIDTH_LSB    _u(8)
#define QMI_M0_WFMT_DATA_WIDTH_ACCESS "RW"
#define QMI_M0_WFMT_DATA_WIDTH_VALUE_S _u(0x0)
#define QMI_M0_WFMT_DATA_WIDTH_VALUE_D _u(0x1)
#define QMI_M0_WFMT_DATA_WIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_WFMT_DUMMY_WIDTH
// Description : The width used for the dummy phase, if any.
//
//               If width is single, SD0/MOSI is held asserted low during the
//               dummy phase, and SD1...SD3 are tristated. If width is
//               dual/quad, all IOs are tristated during the dummy phase.
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M0_WFMT_DUMMY_WIDTH_RESET  _u(0x0)
#define QMI_M0_WFMT_DUMMY_WIDTH_BITS   _u(0x000000c0)
#define QMI_M0_WFMT_DUMMY_WIDTH_MSB    _u(7)
#define QMI_M0_WFMT_DUMMY_WIDTH_LSB    _u(6)
#define QMI_M0_WFMT_DUMMY_WIDTH_ACCESS "RW"
#define QMI_M0_WFMT_DUMMY_WIDTH_VALUE_S _u(0x0)
#define QMI_M0_WFMT_DUMMY_WIDTH_VALUE_D _u(0x1)
#define QMI_M0_WFMT_DUMMY_WIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_WFMT_SUFFIX_WIDTH
// Description : The width used for the post-address command suffix, if any
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M0_WFMT_SUFFIX_WIDTH_RESET  _u(0x0)
#define QMI_M0_WFMT_SUFFIX_WIDTH_BITS   _u(0x00000030)
#define QMI_M0_WFMT_SUFFIX_WIDTH_MSB    _u(5)
#define QMI_M0_WFMT_SUFFIX_WIDTH_LSB    _u(4)
#define QMI_M0_WFMT_SUFFIX_WIDTH_ACCESS "RW"
#define QMI_M0_WFMT_SUFFIX_WIDTH_VALUE_S _u(0x0)
#define QMI_M0_WFMT_SUFFIX_WIDTH_VALUE_D _u(0x1)
#define QMI_M0_WFMT_SUFFIX_WIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_WFMT_ADDR_WIDTH
// Description : The transfer width used for the address. The address phase
//               always transfers 24 bits in total.
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M0_WFMT_ADDR_WIDTH_RESET  _u(0x0)
#define QMI_M0_WFMT_ADDR_WIDTH_BITS   _u(0x0000000c)
#define QMI_M0_WFMT_ADDR_WIDTH_MSB    _u(3)
#define QMI_M0_WFMT_ADDR_WIDTH_LSB    _u(2)
#define QMI_M0_WFMT_ADDR_WIDTH_ACCESS "RW"
#define QMI_M0_WFMT_ADDR_WIDTH_VALUE_S _u(0x0)
#define QMI_M0_WFMT_ADDR_WIDTH_VALUE_D _u(0x1)
#define QMI_M0_WFMT_ADDR_WIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_WFMT_PREFIX_WIDTH
// Description : The transfer width used for the command prefix, if any
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M0_WFMT_PREFIX_WIDTH_RESET  _u(0x0)
#define QMI_M0_WFMT_PREFIX_WIDTH_BITS   _u(0x00000003)
#define QMI_M0_WFMT_PREFIX_WIDTH_MSB    _u(1)
#define QMI_M0_WFMT_PREFIX_WIDTH_LSB    _u(0)
#define QMI_M0_WFMT_PREFIX_WIDTH_ACCESS "RW"
#define QMI_M0_WFMT_PREFIX_WIDTH_VALUE_S _u(0x0)
#define QMI_M0_WFMT_PREFIX_WIDTH_VALUE_D _u(0x1)
#define QMI_M0_WFMT_PREFIX_WIDTH_VALUE_Q _u(0x2)
// =============================================================================
// Register    : QMI_M0_WCMD
// Description : Command constants used for writes to memory address window 0.
//
//               The reset value of the M0_WCMD register is configured to
//               support a basic 02h serial write transfer with no additional
//               configuration.
#define QMI_M0_WCMD_OFFSET _u(0x0000001c)
#define QMI_M0_WCMD_BITS   _u(0x0000ffff)
#define QMI_M0_WCMD_RESET  _u(0x0000a002)
// -----------------------------------------------------------------------------
// Field       : QMI_M0_WCMD_SUFFIX
// Description : The command suffix bits following the address, if
//               Mx_WFMT_SUFFIX_LEN is nonzero.
#define QMI_M0_WCMD_SUFFIX_RESET  _u(0xa0)
#define QMI_M0_WCMD_SUFFIX_BITS   _u(0x0000ff00)
#define QMI_M0_WCMD_SUFFIX_MSB    _u(15)
#define QMI_M0_WCMD_SUFFIX_LSB    _u(8)
#define QMI_M0_WCMD_SUFFIX_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M0_WCMD_PREFIX
// Description : The command prefix bits to prepend on each new transfer, if
//               Mx_WFMT_PREFIX_LEN is nonzero.
#define QMI_M0_WCMD_PREFIX_RESET  _u(0x02)
#define QMI_M0_WCMD_PREFIX_BITS   _u(0x000000ff)
#define QMI_M0_WCMD_PREFIX_MSB    _u(7)
#define QMI_M0_WCMD_PREFIX_LSB    _u(0)
#define QMI_M0_WCMD_PREFIX_ACCESS "RW"
// =============================================================================
// Register    : QMI_M1_TIMING
// Description : Timing configuration register for memory address window 1.
#define QMI_M1_TIMING_OFFSET _u(0x00000020)
#define QMI_M1_TIMING_BITS   _u(0xf3fff7ff)
#define QMI_M1_TIMING_RESET  _u(0x40000004)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_TIMING_COOLDOWN
// Description : Chip select cooldown period. When a memory transfer finishes,
//               the chip select remains asserted for 64 x COOLDOWN system clock
//               cycles, plus half an SCK clock period (rounded up for odd SCK
//               divisors). After this cooldown expires, the chip select is
//               always deasserted to save power.
//
//               If the next memory access arrives within the cooldown period,
//               the QMI may be able to append more SCK cycles to the currently
//               ongoing SPI transfer, rather than starting a new transfer. This
//               reduces access latency and increases bus throughput.
//
//               Specifically, the next access must be in the same direction
//               (read/write), access the same memory window (chip select 0/1),
//               and follow sequentially the address of the last transfer. If
//               any of these are false, the new access will first deassert the
//               chip select, then begin a new transfer.
//
//               If COOLDOWN is 0, the address alignment configured by PAGEBREAK
//               has been reached, or the total chip select assertion limit
//               MAX_SELECT has been reached, the cooldown period is skipped,
//               and the chip select will always be deasserted one half SCK
//               period after the transfer finishes.
#define QMI_M1_TIMING_COOLDOWN_RESET  _u(0x1)
#define QMI_M1_TIMING_COOLDOWN_BITS   _u(0xc0000000)
#define QMI_M1_TIMING_COOLDOWN_MSB    _u(31)
#define QMI_M1_TIMING_COOLDOWN_LSB    _u(30)
#define QMI_M1_TIMING_COOLDOWN_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M1_TIMING_PAGEBREAK
// Description : When page break is enabled, chip select will automatically
//               deassert when crossing certain power-of-2-aligned address
//               boundaries. The next access will always begin a new read/write
//               SPI burst, even if the address of the next access follows in
//               sequence with the last access before the page boundary.
//
//               Some flash and PSRAM devices forbid crossing page boundaries
//               with a single read/write transfer, or restrict the operating
//               frequency for transfers that do cross page a boundary. This
//               option allows the QMI to safely support those devices.
//
//               This field has no effect when COOLDOWN is disabled.
//               0x0 -> No page boundary is enforced
//               0x1 -> Break bursts crossing a 256-byte page boundary
//               0x2 -> Break bursts crossing a 1024-byte quad-page boundary
//               0x3 -> Break bursts crossing a 4096-byte sector boundary
#define QMI_M1_TIMING_PAGEBREAK_RESET  _u(0x0)
#define QMI_M1_TIMING_PAGEBREAK_BITS   _u(0x30000000)
#define QMI_M1_TIMING_PAGEBREAK_MSB    _u(29)
#define QMI_M1_TIMING_PAGEBREAK_LSB    _u(28)
#define QMI_M1_TIMING_PAGEBREAK_ACCESS "RW"
#define QMI_M1_TIMING_PAGEBREAK_VALUE_NONE _u(0x0)
#define QMI_M1_TIMING_PAGEBREAK_VALUE_256 _u(0x1)
#define QMI_M1_TIMING_PAGEBREAK_VALUE_1024 _u(0x2)
#define QMI_M1_TIMING_PAGEBREAK_VALUE_4096 _u(0x3)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_TIMING_SELECT_SETUP
// Description : Add up to one additional system clock cycle of setup between
//               chip select assertion and the first rising edge of SCK.
//
//               The default setup time is one half SCK period, which is usually
//               sufficient except for very high SCK frequencies with some flash
//               devices.
#define QMI_M1_TIMING_SELECT_SETUP_RESET  _u(0x0)
#define QMI_M1_TIMING_SELECT_SETUP_BITS   _u(0x02000000)
#define QMI_M1_TIMING_SELECT_SETUP_MSB    _u(25)
#define QMI_M1_TIMING_SELECT_SETUP_LSB    _u(25)
#define QMI_M1_TIMING_SELECT_SETUP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M1_TIMING_SELECT_HOLD
// Description : Add up to three additional system clock cycles of active hold
//               between the last falling edge of SCK and the deassertion of
//               this window's chip select.
//
//               The default hold time is one system clock cycle. Note that
//               flash datasheets usually give chip select active hold time from
//               the last *rising* edge of SCK, and so even zero hold from the
//               last falling edge would be safe.
//
//               Note that this is a minimum hold time guaranteed by the QMI:
//               the actual chip select active hold may be slightly longer for
//               read transfers with low clock divisors and/or high sample
//               delays. Specifically, if the point two cycles after the last RX
//               data sample is later than the last SCK falling edge, then the
//               hold time is measured from *this* point.
//
//               Note also that, in case the final SCK pulse is masked to save
//               energy (true for non-DTR reads when COOLDOWN is disabled or
//               PAGE_BREAK is reached), all of QMI's timing logic behaves as
//               though the clock pulse were still present. The SELECT_HOLD time
//               is applied from the point where the last SCK falling edge would
//               be if the clock pulse were not masked.
#define QMI_M1_TIMING_SELECT_HOLD_RESET  _u(0x0)
#define QMI_M1_TIMING_SELECT_HOLD_BITS   _u(0x01800000)
#define QMI_M1_TIMING_SELECT_HOLD_MSB    _u(24)
#define QMI_M1_TIMING_SELECT_HOLD_LSB    _u(23)
#define QMI_M1_TIMING_SELECT_HOLD_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M1_TIMING_MAX_SELECT
// Description : Enforce a maximum assertion duration for this window's chip
//               select, in units of 64 system clock cycles. If 0, the QMI is
//               permitted to keep the chip select asserted indefinitely when
//               servicing sequential memory accesses (see COOLDOWN).
//
//               This feature is required to meet timing constraints of PSRAM
//               devices, which specify a maximum chip select assertion so they
//               can perform DRAM refresh cycles. See also MIN_DESELECT, which
//               can enforce a minimum deselect time.
//
//               If a memory access is in progress at the time MAX_SELECT is
//               reached, the QMI will wait for the access to complete before
//               deasserting the chip select. This additional time must be
//               accounted for to calculate a safe MAX_SELECT value. In the
//               worst case, this may be a fully-formed serial transfer,
//               including command prefix and address, with a data payload as
//               large as one cache line.
#define QMI_M1_TIMING_MAX_SELECT_RESET  _u(0x00)
#define QMI_M1_TIMING_MAX_SELECT_BITS   _u(0x007e0000)
#define QMI_M1_TIMING_MAX_SELECT_MSB    _u(22)
#define QMI_M1_TIMING_MAX_SELECT_LSB    _u(17)
#define QMI_M1_TIMING_MAX_SELECT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M1_TIMING_MIN_DESELECT
// Description : After this window's chip select is deasserted, it remains
//               deasserted for half an SCK cycle (rounded up to an integer
//               number of system clock cycles), plus MIN_DESELECT additional
//               system clock cycles, before the QMI reasserts either chip
//               select pin.
//
//               Nonzero values may be required for PSRAM devices which enforce
//               a longer minimum CS deselect time, so that they can perform
//               internal DRAM refresh cycles whilst deselected.
#define QMI_M1_TIMING_MIN_DESELECT_RESET  _u(0x00)
#define QMI_M1_TIMING_MIN_DESELECT_BITS   _u(0x0001f000)
#define QMI_M1_TIMING_MIN_DESELECT_MSB    _u(16)
#define QMI_M1_TIMING_MIN_DESELECT_LSB    _u(12)
#define QMI_M1_TIMING_MIN_DESELECT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M1_TIMING_RXDELAY
// Description : Delay the read data sample timing, in units of one half of a
//               system clock cycle. (Not necessarily half of an SCK cycle.) An
//               RXDELAY of 0 means the sample is captured at the SDI input
//               registers simultaneously with the rising edge of SCK launched
//               from the SCK output register.
//
//               At higher SCK frequencies, RXDELAY may need to be increased to
//               account for the round trip delay of the pads, and the clock-
//               to-Q delay of the QSPI memory device.
#define QMI_M1_TIMING_RXDELAY_RESET  _u(0x0)
#define QMI_M1_TIMING_RXDELAY_BITS   _u(0x00000700)
#define QMI_M1_TIMING_RXDELAY_MSB    _u(10)
#define QMI_M1_TIMING_RXDELAY_LSB    _u(8)
#define QMI_M1_TIMING_RXDELAY_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M1_TIMING_CLKDIV
// Description : Clock divisor. Odd and even divisors are supported. Defines the
//               SCK clock period in units of 1 system clock cycle. Divisors
//               1..255 are encoded directly, and a divisor of 256 is encoded
//               with a value of CLKDIV=0.
//
//               The clock divisor can be changed on-the-fly, even when the QMI
//               is currently accessing memory in this address window. All other
//               parameters must only be changed when the QMI is idle.
//
//               If software is increasing CLKDIV in anticipation of an increase
//               in the system clock frequency, a dummy access to either memory
//               window (and appropriate processor barriers/fences) must be
//               inserted after the Mx_TIMING write to ensure the SCK divisor
//               change is in effect _before_ the system clock is changed.
#define QMI_M1_TIMING_CLKDIV_RESET  _u(0x04)
#define QMI_M1_TIMING_CLKDIV_BITS   _u(0x000000ff)
#define QMI_M1_TIMING_CLKDIV_MSB    _u(7)
#define QMI_M1_TIMING_CLKDIV_LSB    _u(0)
#define QMI_M1_TIMING_CLKDIV_ACCESS "RW"
// =============================================================================
// Register    : QMI_M1_RFMT
// Description : Read transfer format configuration for memory address window 1.
//
//               Configure the bus width of each transfer phase individually,
//               and configure the length or presence of the command prefix,
//               command suffix and dummy/turnaround transfer phases. Only
//               24-bit addresses are supported.
//
//               The reset value of the M1_RFMT register is configured to
//               support a basic 03h serial read transfer with no additional
//               configuration.
#define QMI_M1_RFMT_OFFSET _u(0x00000024)
#define QMI_M1_RFMT_BITS   _u(0x1007d3ff)
#define QMI_M1_RFMT_RESET  _u(0x00001000)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_RFMT_DTR
// Description : Enable double transfer rate (DTR) for read commands: address,
//               suffix and read data phases are active on both edges of SCK.
//               SDO data is launched centre-aligned on each SCK edge, and SDI
//               data is captured on the SCK edge that follows its launch.
//
//               DTR is implemented by halving the clock rate; SCK has a period
//               of 2 x CLK_DIV throughout the transfer. The prefix and dummy
//               phases are still single transfer rate.
//
//               If the suffix is quad-width, it must be 0 or 8 bits in length,
//               to ensure an even number of SCK edges.
#define QMI_M1_RFMT_DTR_RESET  _u(0x0)
#define QMI_M1_RFMT_DTR_BITS   _u(0x10000000)
#define QMI_M1_RFMT_DTR_MSB    _u(28)
#define QMI_M1_RFMT_DTR_LSB    _u(28)
#define QMI_M1_RFMT_DTR_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M1_RFMT_DUMMY_LEN
// Description : Length of dummy phase between command suffix and data phase, in
//               units of 4 bits. (i.e. 1 cycle for quad width, 2 for dual, 4
//               for single)
//               0x0 -> No dummy phase
//               0x1 -> 4 dummy bits
//               0x2 -> 8 dummy bits
//               0x3 -> 12 dummy bits
//               0x4 -> 16 dummy bits
//               0x5 -> 20 dummy bits
//               0x6 -> 24 dummy bits
//               0x7 -> 28 dummy bits
#define QMI_M1_RFMT_DUMMY_LEN_RESET  _u(0x0)
#define QMI_M1_RFMT_DUMMY_LEN_BITS   _u(0x00070000)
#define QMI_M1_RFMT_DUMMY_LEN_MSB    _u(18)
#define QMI_M1_RFMT_DUMMY_LEN_LSB    _u(16)
#define QMI_M1_RFMT_DUMMY_LEN_ACCESS "RW"
#define QMI_M1_RFMT_DUMMY_LEN_VALUE_NONE _u(0x0)
#define QMI_M1_RFMT_DUMMY_LEN_VALUE_4 _u(0x1)
#define QMI_M1_RFMT_DUMMY_LEN_VALUE_8 _u(0x2)
#define QMI_M1_RFMT_DUMMY_LEN_VALUE_12 _u(0x3)
#define QMI_M1_RFMT_DUMMY_LEN_VALUE_16 _u(0x4)
#define QMI_M1_RFMT_DUMMY_LEN_VALUE_20 _u(0x5)
#define QMI_M1_RFMT_DUMMY_LEN_VALUE_24 _u(0x6)
#define QMI_M1_RFMT_DUMMY_LEN_VALUE_28 _u(0x7)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_RFMT_SUFFIX_LEN
// Description : Length of post-address command suffix, in units of 4 bits.
//               (i.e. 1 cycle for quad width, 2 for dual, 4 for single)
//
//               Only values of 0 and 8 bits are supported.
//               0x0 -> No suffix
//               0x2 -> 8-bit suffix
#define QMI_M1_RFMT_SUFFIX_LEN_RESET  _u(0x0)
#define QMI_M1_RFMT_SUFFIX_LEN_BITS   _u(0x0000c000)
#define QMI_M1_RFMT_SUFFIX_LEN_MSB    _u(15)
#define QMI_M1_RFMT_SUFFIX_LEN_LSB    _u(14)
#define QMI_M1_RFMT_SUFFIX_LEN_ACCESS "RW"
#define QMI_M1_RFMT_SUFFIX_LEN_VALUE_NONE _u(0x0)
#define QMI_M1_RFMT_SUFFIX_LEN_VALUE_8 _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_RFMT_PREFIX_LEN
// Description : Length of command prefix, in units of 8 bits. (i.e. 2 cycles
//               for quad width, 4 for dual, 8 for single)
//               0x0 -> No prefix
//               0x1 -> 8-bit prefix
#define QMI_M1_RFMT_PREFIX_LEN_RESET  _u(0x1)
#define QMI_M1_RFMT_PREFIX_LEN_BITS   _u(0x00001000)
#define QMI_M1_RFMT_PREFIX_LEN_MSB    _u(12)
#define QMI_M1_RFMT_PREFIX_LEN_LSB    _u(12)
#define QMI_M1_RFMT_PREFIX_LEN_ACCESS "RW"
#define QMI_M1_RFMT_PREFIX_LEN_VALUE_NONE _u(0x0)
#define QMI_M1_RFMT_PREFIX_LEN_VALUE_8 _u(0x1)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_RFMT_DATA_WIDTH
// Description : The width used for the data transfer
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M1_RFMT_DATA_WIDTH_RESET  _u(0x0)
#define QMI_M1_RFMT_DATA_WIDTH_BITS   _u(0x00000300)
#define QMI_M1_RFMT_DATA_WIDTH_MSB    _u(9)
#define QMI_M1_RFMT_DATA_WIDTH_LSB    _u(8)
#define QMI_M1_RFMT_DATA_WIDTH_ACCESS "RW"
#define QMI_M1_RFMT_DATA_WIDTH_VALUE_S _u(0x0)
#define QMI_M1_RFMT_DATA_WIDTH_VALUE_D _u(0x1)
#define QMI_M1_RFMT_DATA_WIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_RFMT_DUMMY_WIDTH
// Description : The width used for the dummy phase, if any.
//
//               If width is single, SD0/MOSI is held asserted low during the
//               dummy phase, and SD1...SD3 are tristated. If width is
//               dual/quad, all IOs are tristated during the dummy phase.
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M1_RFMT_DUMMY_WIDTH_RESET  _u(0x0)
#define QMI_M1_RFMT_DUMMY_WIDTH_BITS   _u(0x000000c0)
#define QMI_M1_RFMT_DUMMY_WIDTH_MSB    _u(7)
#define QMI_M1_RFMT_DUMMY_WIDTH_LSB    _u(6)
#define QMI_M1_RFMT_DUMMY_WIDTH_ACCESS "RW"
#define QMI_M1_RFMT_DUMMY_WIDTH_VALUE_S _u(0x0)
#define QMI_M1_RFMT_DUMMY_WIDTH_VALUE_D _u(0x1)
#define QMI_M1_RFMT_DUMMY_WIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_RFMT_SUFFIX_WIDTH
// Description : The width used for the post-address command suffix, if any
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M1_RFMT_SUFFIX_WIDTH_RESET  _u(0x0)
#define QMI_M1_RFMT_SUFFIX_WIDTH_BITS   _u(0x00000030)
#define QMI_M1_RFMT_SUFFIX_WIDTH_MSB    _u(5)
#define QMI_M1_RFMT_SUFFIX_WIDTH_LSB    _u(4)
#define QMI_M1_RFMT_SUFFIX_WIDTH_ACCESS "RW"
#define QMI_M1_RFMT_SUFFIX_WIDTH_VALUE_S _u(0x0)
#define QMI_M1_RFMT_SUFFIX_WIDTH_VALUE_D _u(0x1)
#define QMI_M1_RFMT_SUFFIX_WIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_RFMT_ADDR_WIDTH
// Description : The transfer width used for the address. The address phase
//               always transfers 24 bits in total.
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M1_RFMT_ADDR_WIDTH_RESET  _u(0x0)
#define QMI_M1_RFMT_ADDR_WIDTH_BITS   _u(0x0000000c)
#define QMI_M1_RFMT_ADDR_WIDTH_MSB    _u(3)
#define QMI_M1_RFMT_ADDR_WIDTH_LSB    _u(2)
#define QMI_M1_RFMT_ADDR_WIDTH_ACCESS "RW"
#define QMI_M1_RFMT_ADDR_WIDTH_VALUE_S _u(0x0)
#define QMI_M1_RFMT_ADDR_WIDTH_VALUE_D _u(0x1)
#define QMI_M1_RFMT_ADDR_WIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_RFMT_PREFIX_WIDTH
// Description : The transfer width used for the command prefix, if any
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M1_RFMT_PREFIX_WIDTH_RESET  _u(0x0)
#define QMI_M1_RFMT_PREFIX_WIDTH_BITS   _u(0x00000003)
#define QMI_M1_RFMT_PREFIX_WIDTH_MSB    _u(1)
#define QMI_M1_RFMT_PREFIX_WIDTH_LSB    _u(0)
#define QMI_M1_RFMT_PREFIX_WIDTH_ACCESS "RW"
#define QMI_M1_RFMT_PREFIX_WIDTH_VALUE_S _u(0x0)
#define QMI_M1_RFMT_PREFIX_WIDTH_VALUE_D _u(0x1)
#define QMI_M1_RFMT_PREFIX_WIDTH_VALUE_Q _u(0x2)
// =============================================================================
// Register    : QMI_M1_RCMD
// Description : Command constants used for reads from memory address window 1.
//
//               The reset value of the M1_RCMD register is configured to
//               support a basic 03h serial read transfer with no additional
//               configuration.
#define QMI_M1_RCMD_OFFSET _u(0x00000028)
#define QMI_M1_RCMD_BITS   _u(0x0000ffff)
#define QMI_M1_RCMD_RESET  _u(0x0000a003)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_RCMD_SUFFIX
// Description : The command suffix bits following the address, if
//               Mx_RFMT_SUFFIX_LEN is nonzero.
#define QMI_M1_RCMD_SUFFIX_RESET  _u(0xa0)
#define QMI_M1_RCMD_SUFFIX_BITS   _u(0x0000ff00)
#define QMI_M1_RCMD_SUFFIX_MSB    _u(15)
#define QMI_M1_RCMD_SUFFIX_LSB    _u(8)
#define QMI_M1_RCMD_SUFFIX_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M1_RCMD_PREFIX
// Description : The command prefix bits to prepend on each new transfer, if
//               Mx_RFMT_PREFIX_LEN is nonzero.
#define QMI_M1_RCMD_PREFIX_RESET  _u(0x03)
#define QMI_M1_RCMD_PREFIX_BITS   _u(0x000000ff)
#define QMI_M1_RCMD_PREFIX_MSB    _u(7)
#define QMI_M1_RCMD_PREFIX_LSB    _u(0)
#define QMI_M1_RCMD_PREFIX_ACCESS "RW"
// =============================================================================
// Register    : QMI_M1_WFMT
// Description : Write transfer format configuration for memory address window
//               1.
//
//               Configure the bus width of each transfer phase individually,
//               and configure the length or presence of the command prefix,
//               command suffix and dummy/turnaround transfer phases. Only
//               24-bit addresses are supported.
//
//               The reset value of the M1_WFMT register is configured to
//               support a basic 02h serial write transfer. However, writes to
//               this window must first be enabled via the XIP_CTRL_WRITABLE_M1
//               bit, as XIP memory is read-only by default.
#define QMI_M1_WFMT_OFFSET _u(0x0000002c)
#define QMI_M1_WFMT_BITS   _u(0x1007d3ff)
#define QMI_M1_WFMT_RESET  _u(0x00001000)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_WFMT_DTR
// Description : Enable double transfer rate (DTR) for write commands: address,
//               suffix and write data phases are active on both edges of SCK.
//               SDO data is launched centre-aligned on each SCK edge, and SDI
//               data is captured on the SCK edge that follows its launch.
//
//               DTR is implemented by halving the clock rate; SCK has a period
//               of 2 x CLK_DIV throughout the transfer. The prefix and dummy
//               phases are still single transfer rate.
//
//               If the suffix is quad-width, it must be 0 or 8 bits in length,
//               to ensure an even number of SCK edges.
#define QMI_M1_WFMT_DTR_RESET  _u(0x0)
#define QMI_M1_WFMT_DTR_BITS   _u(0x10000000)
#define QMI_M1_WFMT_DTR_MSB    _u(28)
#define QMI_M1_WFMT_DTR_LSB    _u(28)
#define QMI_M1_WFMT_DTR_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M1_WFMT_DUMMY_LEN
// Description : Length of dummy phase between command suffix and data phase, in
//               units of 4 bits. (i.e. 1 cycle for quad width, 2 for dual, 4
//               for single)
//               0x0 -> No dummy phase
//               0x1 -> 4 dummy bits
//               0x2 -> 8 dummy bits
//               0x3 -> 12 dummy bits
//               0x4 -> 16 dummy bits
//               0x5 -> 20 dummy bits
//               0x6 -> 24 dummy bits
//               0x7 -> 28 dummy bits
#define QMI_M1_WFMT_DUMMY_LEN_RESET  _u(0x0)
#define QMI_M1_WFMT_DUMMY_LEN_BITS   _u(0x00070000)
#define QMI_M1_WFMT_DUMMY_LEN_MSB    _u(18)
#define QMI_M1_WFMT_DUMMY_LEN_LSB    _u(16)
#define QMI_M1_WFMT_DUMMY_LEN_ACCESS "RW"
#define QMI_M1_WFMT_DUMMY_LEN_VALUE_NONE _u(0x0)
#define QMI_M1_WFMT_DUMMY_LEN_VALUE_4 _u(0x1)
#define QMI_M1_WFMT_DUMMY_LEN_VALUE_8 _u(0x2)
#define QMI_M1_WFMT_DUMMY_LEN_VALUE_12 _u(0x3)
#define QMI_M1_WFMT_DUMMY_LEN_VALUE_16 _u(0x4)
#define QMI_M1_WFMT_DUMMY_LEN_VALUE_20 _u(0x5)
#define QMI_M1_WFMT_DUMMY_LEN_VALUE_24 _u(0x6)
#define QMI_M1_WFMT_DUMMY_LEN_VALUE_28 _u(0x7)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_WFMT_SUFFIX_LEN
// Description : Length of post-address command suffix, in units of 4 bits.
//               (i.e. 1 cycle for quad width, 2 for dual, 4 for single)
//
//               Only values of 0 and 8 bits are supported.
//               0x0 -> No suffix
//               0x2 -> 8-bit suffix
#define QMI_M1_WFMT_SUFFIX_LEN_RESET  _u(0x0)
#define QMI_M1_WFMT_SUFFIX_LEN_BITS   _u(0x0000c000)
#define QMI_M1_WFMT_SUFFIX_LEN_MSB    _u(15)
#define QMI_M1_WFMT_SUFFIX_LEN_LSB    _u(14)
#define QMI_M1_WFMT_SUFFIX_LEN_ACCESS "RW"
#define QMI_M1_WFMT_SUFFIX_LEN_VALUE_NONE _u(0x0)
#define QMI_M1_WFMT_SUFFIX_LEN_VALUE_8 _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_WFMT_PREFIX_LEN
// Description : Length of command prefix, in units of 8 bits. (i.e. 2 cycles
//               for quad width, 4 for dual, 8 for single)
//               0x0 -> No prefix
//               0x1 -> 8-bit prefix
#define QMI_M1_WFMT_PREFIX_LEN_RESET  _u(0x1)
#define QMI_M1_WFMT_PREFIX_LEN_BITS   _u(0x00001000)
#define QMI_M1_WFMT_PREFIX_LEN_MSB    _u(12)
#define QMI_M1_WFMT_PREFIX_LEN_LSB    _u(12)
#define QMI_M1_WFMT_PREFIX_LEN_ACCESS "RW"
#define QMI_M1_WFMT_PREFIX_LEN_VALUE_NONE _u(0x0)
#define QMI_M1_WFMT_PREFIX_LEN_VALUE_8 _u(0x1)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_WFMT_DATA_WIDTH
// Description : The width used for the data transfer
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M1_WFMT_DATA_WIDTH_RESET  _u(0x0)
#define QMI_M1_WFMT_DATA_WIDTH_BITS   _u(0x00000300)
#define QMI_M1_WFMT_DATA_WIDTH_MSB    _u(9)
#define QMI_M1_WFMT_DATA_WIDTH_LSB    _u(8)
#define QMI_M1_WFMT_DATA_WIDTH_ACCESS "RW"
#define QMI_M1_WFMT_DATA_WIDTH_VALUE_S _u(0x0)
#define QMI_M1_WFMT_DATA_WIDTH_VALUE_D _u(0x1)
#define QMI_M1_WFMT_DATA_WIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_WFMT_DUMMY_WIDTH
// Description : The width used for the dummy phase, if any.
//
//               If width is single, SD0/MOSI is held asserted low during the
//               dummy phase, and SD1...SD3 are tristated. If width is
//               dual/quad, all IOs are tristated during the dummy phase.
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M1_WFMT_DUMMY_WIDTH_RESET  _u(0x0)
#define QMI_M1_WFMT_DUMMY_WIDTH_BITS   _u(0x000000c0)
#define QMI_M1_WFMT_DUMMY_WIDTH_MSB    _u(7)
#define QMI_M1_WFMT_DUMMY_WIDTH_LSB    _u(6)
#define QMI_M1_WFMT_DUMMY_WIDTH_ACCESS "RW"
#define QMI_M1_WFMT_DUMMY_WIDTH_VALUE_S _u(0x0)
#define QMI_M1_WFMT_DUMMY_WIDTH_VALUE_D _u(0x1)
#define QMI_M1_WFMT_DUMMY_WIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_WFMT_SUFFIX_WIDTH
// Description : The width used for the post-address command suffix, if any
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M1_WFMT_SUFFIX_WIDTH_RESET  _u(0x0)
#define QMI_M1_WFMT_SUFFIX_WIDTH_BITS   _u(0x00000030)
#define QMI_M1_WFMT_SUFFIX_WIDTH_MSB    _u(5)
#define QMI_M1_WFMT_SUFFIX_WIDTH_LSB    _u(4)
#define QMI_M1_WFMT_SUFFIX_WIDTH_ACCESS "RW"
#define QMI_M1_WFMT_SUFFIX_WIDTH_VALUE_S _u(0x0)
#define QMI_M1_WFMT_SUFFIX_WIDTH_VALUE_D _u(0x1)
#define QMI_M1_WFMT_SUFFIX_WIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_WFMT_ADDR_WIDTH
// Description : The transfer width used for the address. The address phase
//               always transfers 24 bits in total.
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M1_WFMT_ADDR_WIDTH_RESET  _u(0x0)
#define QMI_M1_WFMT_ADDR_WIDTH_BITS   _u(0x0000000c)
#define QMI_M1_WFMT_ADDR_WIDTH_MSB    _u(3)
#define QMI_M1_WFMT_ADDR_WIDTH_LSB    _u(2)
#define QMI_M1_WFMT_ADDR_WIDTH_ACCESS "RW"
#define QMI_M1_WFMT_ADDR_WIDTH_VALUE_S _u(0x0)
#define QMI_M1_WFMT_ADDR_WIDTH_VALUE_D _u(0x1)
#define QMI_M1_WFMT_ADDR_WIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_WFMT_PREFIX_WIDTH
// Description : The transfer width used for the command prefix, if any
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define QMI_M1_WFMT_PREFIX_WIDTH_RESET  _u(0x0)
#define QMI_M1_WFMT_PREFIX_WIDTH_BITS   _u(0x00000003)
#define QMI_M1_WFMT_PREFIX_WIDTH_MSB    _u(1)
#define QMI_M1_WFMT_PREFIX_WIDTH_LSB    _u(0)
#define QMI_M1_WFMT_PREFIX_WIDTH_ACCESS "RW"
#define QMI_M1_WFMT_PREFIX_WIDTH_VALUE_S _u(0x0)
#define QMI_M1_WFMT_PREFIX_WIDTH_VALUE_D _u(0x1)
#define QMI_M1_WFMT_PREFIX_WIDTH_VALUE_Q _u(0x2)
// =============================================================================
// Register    : QMI_M1_WCMD
// Description : Command constants used for writes to memory address window 1.
//
//               The reset value of the M1_WCMD register is configured to
//               support a basic 02h serial write transfer with no additional
//               configuration.
#define QMI_M1_WCMD_OFFSET _u(0x00000030)
#define QMI_M1_WCMD_BITS   _u(0x0000ffff)
#define QMI_M1_WCMD_RESET  _u(0x0000a002)
// -----------------------------------------------------------------------------
// Field       : QMI_M1_WCMD_SUFFIX
// Description : The command suffix bits following the address, if
//               Mx_WFMT_SUFFIX_LEN is nonzero.
#define QMI_M1_WCMD_SUFFIX_RESET  _u(0xa0)
#define QMI_M1_WCMD_SUFFIX_BITS   _u(0x0000ff00)
#define QMI_M1_WCMD_SUFFIX_MSB    _u(15)
#define QMI_M1_WCMD_SUFFIX_LSB    _u(8)
#define QMI_M1_WCMD_SUFFIX_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_M1_WCMD_PREFIX
// Description : The command prefix bits to prepend on each new transfer, if
//               Mx_WFMT_PREFIX_LEN is nonzero.
#define QMI_M1_WCMD_PREFIX_RESET  _u(0x02)
#define QMI_M1_WCMD_PREFIX_BITS   _u(0x000000ff)
#define QMI_M1_WCMD_PREFIX_MSB    _u(7)
#define QMI_M1_WCMD_PREFIX_LSB    _u(0)
#define QMI_M1_WCMD_PREFIX_ACCESS "RW"
// =============================================================================
// Register    : QMI_ATRANS0
// Description : Configure address translation for XIP virtual addresses
//               0x000000 through 0x3fffff (a 4 MiB window starting at +0 MiB).
//
//               Address translation allows a program image to be executed in
//               place at multiple physical flash addresses (for example, a
//               double-buffered flash image for over-the-air updates), without
//               the overhead of position-independent code.
//
//               At reset, the address translation registers are initialised to
//               an identity mapping, so that they can be ignored if address
//               translation is not required.
//
//               Note that the XIP cache is fully virtually addressed, so a
//               cache flush is required after changing the address translation.
#define QMI_ATRANS0_OFFSET _u(0x00000034)
#define QMI_ATRANS0_BITS   _u(0x07ff0fff)
#define QMI_ATRANS0_RESET  _u(0x04000000)
// -----------------------------------------------------------------------------
// Field       : QMI_ATRANS0_SIZE
// Description : Translation aperture size for this virtual address range, in
//               units of 4 kiB (one flash sector).
//
//               Bits 21:12 of the virtual address are compared to SIZE. Offsets
//               greater than SIZE return a bus error, and do not cause a QSPI
//               access.
#define QMI_ATRANS0_SIZE_RESET  _u(0x400)
#define QMI_ATRANS0_SIZE_BITS   _u(0x07ff0000)
#define QMI_ATRANS0_SIZE_MSB    _u(26)
#define QMI_ATRANS0_SIZE_LSB    _u(16)
#define QMI_ATRANS0_SIZE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_ATRANS0_BASE
// Description : Physical address base for this virtual address range, in units
//               of 4 kiB (one flash sector).
//
//               Taking a 24-bit virtual address, firstly bits 23:22 (the two
//               MSBs) are masked to zero, and then BASE is added to bits 23:12
//               (the upper 12 bits) to form the physical address. Translation
//               wraps on a 16 MiB boundary.
#define QMI_ATRANS0_BASE_RESET  _u(0x000)
#define QMI_ATRANS0_BASE_BITS   _u(0x00000fff)
#define QMI_ATRANS0_BASE_MSB    _u(11)
#define QMI_ATRANS0_BASE_LSB    _u(0)
#define QMI_ATRANS0_BASE_ACCESS "RW"
// =============================================================================
// Register    : QMI_ATRANS1
// Description : Configure address translation for XIP virtual addresses
//               0x400000 through 0x7fffff (a 4 MiB window starting at +4 MiB).
//
//               Address translation allows a program image to be executed in
//               place at multiple physical flash addresses (for example, a
//               double-buffered flash image for over-the-air updates), without
//               the overhead of position-independent code.
//
//               At reset, the address translation registers are initialised to
//               an identity mapping, so that they can be ignored if address
//               translation is not required.
//
//               Note that the XIP cache is fully virtually addressed, so a
//               cache flush is required after changing the address translation.
#define QMI_ATRANS1_OFFSET _u(0x00000038)
#define QMI_ATRANS1_BITS   _u(0x07ff0fff)
#define QMI_ATRANS1_RESET  _u(0x04000400)
// -----------------------------------------------------------------------------
// Field       : QMI_ATRANS1_SIZE
// Description : Translation aperture size for this virtual address range, in
//               units of 4 kiB (one flash sector).
//
//               Bits 21:12 of the virtual address are compared to SIZE. Offsets
//               greater than SIZE return a bus error, and do not cause a QSPI
//               access.
#define QMI_ATRANS1_SIZE_RESET  _u(0x400)
#define QMI_ATRANS1_SIZE_BITS   _u(0x07ff0000)
#define QMI_ATRANS1_SIZE_MSB    _u(26)
#define QMI_ATRANS1_SIZE_LSB    _u(16)
#define QMI_ATRANS1_SIZE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_ATRANS1_BASE
// Description : Physical address base for this virtual address range, in units
//               of 4 kiB (one flash sector).
//
//               Taking a 24-bit virtual address, firstly bits 23:22 (the two
//               MSBs) are masked to zero, and then BASE is added to bits 23:12
//               (the upper 12 bits) to form the physical address. Translation
//               wraps on a 16 MiB boundary.
#define QMI_ATRANS1_BASE_RESET  _u(0x400)
#define QMI_ATRANS1_BASE_BITS   _u(0x00000fff)
#define QMI_ATRANS1_BASE_MSB    _u(11)
#define QMI_ATRANS1_BASE_LSB    _u(0)
#define QMI_ATRANS1_BASE_ACCESS "RW"
// =============================================================================
// Register    : QMI_ATRANS2
// Description : Configure address translation for XIP virtual addresses
//               0x800000 through 0xbfffff (a 4 MiB window starting at +8 MiB).
//
//               Address translation allows a program image to be executed in
//               place at multiple physical flash addresses (for example, a
//               double-buffered flash image for over-the-air updates), without
//               the overhead of position-independent code.
//
//               At reset, the address translation registers are initialised to
//               an identity mapping, so that they can be ignored if address
//               translation is not required.
//
//               Note that the XIP cache is fully virtually addressed, so a
//               cache flush is required after changing the address translation.
#define QMI_ATRANS2_OFFSET _u(0x0000003c)
#define QMI_ATRANS2_BITS   _u(0x07ff0fff)
#define QMI_ATRANS2_RESET  _u(0x04000800)
// -----------------------------------------------------------------------------
// Field       : QMI_ATRANS2_SIZE
// Description : Translation aperture size for this virtual address range, in
//               units of 4 kiB (one flash sector).
//
//               Bits 21:12 of the virtual address are compared to SIZE. Offsets
//               greater than SIZE return a bus error, and do not cause a QSPI
//               access.
#define QMI_ATRANS2_SIZE_RESET  _u(0x400)
#define QMI_ATRANS2_SIZE_BITS   _u(0x07ff0000)
#define QMI_ATRANS2_SIZE_MSB    _u(26)
#define QMI_ATRANS2_SIZE_LSB    _u(16)
#define QMI_ATRANS2_SIZE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_ATRANS2_BASE
// Description : Physical address base for this virtual address range, in units
//               of 4 kiB (one flash sector).
//
//               Taking a 24-bit virtual address, firstly bits 23:22 (the two
//               MSBs) are masked to zero, and then BASE is added to bits 23:12
//               (the upper 12 bits) to form the physical address. Translation
//               wraps on a 16 MiB boundary.
#define QMI_ATRANS2_BASE_RESET  _u(0x800)
#define QMI_ATRANS2_BASE_BITS   _u(0x00000fff)
#define QMI_ATRANS2_BASE_MSB    _u(11)
#define QMI_ATRANS2_BASE_LSB    _u(0)
#define QMI_ATRANS2_BASE_ACCESS "RW"
// =============================================================================
// Register    : QMI_ATRANS3
// Description : Configure address translation for XIP virtual addresses
//               0xc00000 through 0xffffff (a 4 MiB window starting at +12 MiB).
//
//               Address translation allows a program image to be executed in
//               place at multiple physical flash addresses (for example, a
//               double-buffered flash image for over-the-air updates), without
//               the overhead of position-independent code.
//
//               At reset, the address translation registers are initialised to
//               an identity mapping, so that they can be ignored if address
//               translation is not required.
//
//               Note that the XIP cache is fully virtually addressed, so a
//               cache flush is required after changing the address translation.
#define QMI_ATRANS3_OFFSET _u(0x00000040)
#define QMI_ATRANS3_BITS   _u(0x07ff0fff)
#define QMI_ATRANS3_RESET  _u(0x04000c00)
// -----------------------------------------------------------------------------
// Field       : QMI_ATRANS3_SIZE
// Description : Translation aperture size for this virtual address range, in
//               units of 4 kiB (one flash sector).
//
//               Bits 21:12 of the virtual address are compared to SIZE. Offsets
//               greater than SIZE return a bus error, and do not cause a QSPI
//               access.
#define QMI_ATRANS3_SIZE_RESET  _u(0x400)
#define QMI_ATRANS3_SIZE_BITS   _u(0x07ff0000)
#define QMI_ATRANS3_SIZE_MSB    _u(26)
#define QMI_ATRANS3_SIZE_LSB    _u(16)
#define QMI_ATRANS3_SIZE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_ATRANS3_BASE
// Description : Physical address base for this virtual address range, in units
//               of 4 kiB (one flash sector).
//
//               Taking a 24-bit virtual address, firstly bits 23:22 (the two
//               MSBs) are masked to zero, and then BASE is added to bits 23:12
//               (the upper 12 bits) to form the physical address. Translation
//               wraps on a 16 MiB boundary.
#define QMI_ATRANS3_BASE_RESET  _u(0xc00)
#define QMI_ATRANS3_BASE_BITS   _u(0x00000fff)
#define QMI_ATRANS3_BASE_MSB    _u(11)
#define QMI_ATRANS3_BASE_LSB    _u(0)
#define QMI_ATRANS3_BASE_ACCESS "RW"
// =============================================================================
// Register    : QMI_ATRANS4
// Description : Configure address translation for XIP virtual addresses
//               0x1000000 through 0x13fffff (a 4 MiB window starting at +16
//               MiB).
//
//               Address translation allows a program image to be executed in
//               place at multiple physical flash addresses (for example, a
//               double-buffered flash image for over-the-air updates), without
//               the overhead of position-independent code.
//
//               At reset, the address translation registers are initialised to
//               an identity mapping, so that they can be ignored if address
//               translation is not required.
//
//               Note that the XIP cache is fully virtually addressed, so a
//               cache flush is required after changing the address translation.
#define QMI_ATRANS4_OFFSET _u(0x00000044)
#define QMI_ATRANS4_BITS   _u(0x07ff0fff)
#define QMI_ATRANS4_RESET  _u(0x04000000)
// -----------------------------------------------------------------------------
// Field       : QMI_ATRANS4_SIZE
// Description : Translation aperture size for this virtual address range, in
//               units of 4 kiB (one flash sector).
//
//               Bits 21:12 of the virtual address are compared to SIZE. Offsets
//               greater than SIZE return a bus error, and do not cause a QSPI
//               access.
#define QMI_ATRANS4_SIZE_RESET  _u(0x400)
#define QMI_ATRANS4_SIZE_BITS   _u(0x07ff0000)
#define QMI_ATRANS4_SIZE_MSB    _u(26)
#define QMI_ATRANS4_SIZE_LSB    _u(16)
#define QMI_ATRANS4_SIZE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_ATRANS4_BASE
// Description : Physical address base for this virtual address range, in units
//               of 4 kiB (one flash sector).
//
//               Taking a 24-bit virtual address, firstly bits 23:22 (the two
//               MSBs) are masked to zero, and then BASE is added to bits 23:12
//               (the upper 12 bits) to form the physical address. Translation
//               wraps on a 16 MiB boundary.
#define QMI_ATRANS4_BASE_RESET  _u(0x000)
#define QMI_ATRANS4_BASE_BITS   _u(0x00000fff)
#define QMI_ATRANS4_BASE_MSB    _u(11)
#define QMI_ATRANS4_BASE_LSB    _u(0)
#define QMI_ATRANS4_BASE_ACCESS "RW"
// =============================================================================
// Register    : QMI_ATRANS5
// Description : Configure address translation for XIP virtual addresses
//               0x1400000 through 0x17fffff (a 4 MiB window starting at +20
//               MiB).
//
//               Address translation allows a program image to be executed in
//               place at multiple physical flash addresses (for example, a
//               double-buffered flash image for over-the-air updates), without
//               the overhead of position-independent code.
//
//               At reset, the address translation registers are initialised to
//               an identity mapping, so that they can be ignored if address
//               translation is not required.
//
//               Note that the XIP cache is fully virtually addressed, so a
//               cache flush is required after changing the address translation.
#define QMI_ATRANS5_OFFSET _u(0x00000048)
#define QMI_ATRANS5_BITS   _u(0x07ff0fff)
#define QMI_ATRANS5_RESET  _u(0x04000400)
// -----------------------------------------------------------------------------
// Field       : QMI_ATRANS5_SIZE
// Description : Translation aperture size for this virtual address range, in
//               units of 4 kiB (one flash sector).
//
//               Bits 21:12 of the virtual address are compared to SIZE. Offsets
//               greater than SIZE return a bus error, and do not cause a QSPI
//               access.
#define QMI_ATRANS5_SIZE_RESET  _u(0x400)
#define QMI_ATRANS5_SIZE_BITS   _u(0x07ff0000)
#define QMI_ATRANS5_SIZE_MSB    _u(26)
#define QMI_ATRANS5_SIZE_LSB    _u(16)
#define QMI_ATRANS5_SIZE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_ATRANS5_BASE
// Description : Physical address base for this virtual address range, in units
//               of 4 kiB (one flash sector).
//
//               Taking a 24-bit virtual address, firstly bits 23:22 (the two
//               MSBs) are masked to zero, and then BASE is added to bits 23:12
//               (the upper 12 bits) to form the physical address. Translation
//               wraps on a 16 MiB boundary.
#define QMI_ATRANS5_BASE_RESET  _u(0x400)
#define QMI_ATRANS5_BASE_BITS   _u(0x00000fff)
#define QMI_ATRANS5_BASE_MSB    _u(11)
#define QMI_ATRANS5_BASE_LSB    _u(0)
#define QMI_ATRANS5_BASE_ACCESS "RW"
// =============================================================================
// Register    : QMI_ATRANS6
// Description : Configure address translation for XIP virtual addresses
//               0x1800000 through 0x1bfffff (a 4 MiB window starting at +24
//               MiB).
//
//               Address translation allows a program image to be executed in
//               place at multiple physical flash addresses (for example, a
//               double-buffered flash image for over-the-air updates), without
//               the overhead of position-independent code.
//
//               At reset, the address translation registers are initialised to
//               an identity mapping, so that they can be ignored if address
//               translation is not required.
//
//               Note that the XIP cache is fully virtually addressed, so a
//               cache flush is required after changing the address translation.
#define QMI_ATRANS6_OFFSET _u(0x0000004c)
#define QMI_ATRANS6_BITS   _u(0x07ff0fff)
#define QMI_ATRANS6_RESET  _u(0x04000800)
// -----------------------------------------------------------------------------
// Field       : QMI_ATRANS6_SIZE
// Description : Translation aperture size for this virtual address range, in
//               units of 4 kiB (one flash sector).
//
//               Bits 21:12 of the virtual address are compared to SIZE. Offsets
//               greater than SIZE return a bus error, and do not cause a QSPI
//               access.
#define QMI_ATRANS6_SIZE_RESET  _u(0x400)
#define QMI_ATRANS6_SIZE_BITS   _u(0x07ff0000)
#define QMI_ATRANS6_SIZE_MSB    _u(26)
#define QMI_ATRANS6_SIZE_LSB    _u(16)
#define QMI_ATRANS6_SIZE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_ATRANS6_BASE
// Description : Physical address base for this virtual address range, in units
//               of 4 kiB (one flash sector).
//
//               Taking a 24-bit virtual address, firstly bits 23:22 (the two
//               MSBs) are masked to zero, and then BASE is added to bits 23:12
//               (the upper 12 bits) to form the physical address. Translation
//               wraps on a 16 MiB boundary.
#define QMI_ATRANS6_BASE_RESET  _u(0x800)
#define QMI_ATRANS6_BASE_BITS   _u(0x00000fff)
#define QMI_ATRANS6_BASE_MSB    _u(11)
#define QMI_ATRANS6_BASE_LSB    _u(0)
#define QMI_ATRANS6_BASE_ACCESS "RW"
// =============================================================================
// Register    : QMI_ATRANS7
// Description : Configure address translation for XIP virtual addresses
//               0x1c00000 through 0x1ffffff (a 4 MiB window starting at +28
//               MiB).
//
//               Address translation allows a program image to be executed in
//               place at multiple physical flash addresses (for example, a
//               double-buffered flash image for over-the-air updates), without
//               the overhead of position-independent code.
//
//               At reset, the address translation registers are initialised to
//               an identity mapping, so that they can be ignored if address
//               translation is not required.
//
//               Note that the XIP cache is fully virtually addressed, so a
//               cache flush is required after changing the address translation.
#define QMI_ATRANS7_OFFSET _u(0x00000050)
#define QMI_ATRANS7_BITS   _u(0x07ff0fff)
#define QMI_ATRANS7_RESET  _u(0x04000c00)
// -----------------------------------------------------------------------------
// Field       : QMI_ATRANS7_SIZE
// Description : Translation aperture size for this virtual address range, in
//               units of 4 kiB (one flash sector).
//
//               Bits 21:12 of the virtual address are compared to SIZE. Offsets
//               greater than SIZE return a bus error, and do not cause a QSPI
//               access.
#define QMI_ATRANS7_SIZE_RESET  _u(0x400)
#define QMI_ATRANS7_SIZE_BITS   _u(0x07ff0000)
#define QMI_ATRANS7_SIZE_MSB    _u(26)
#define QMI_ATRANS7_SIZE_LSB    _u(16)
#define QMI_ATRANS7_SIZE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : QMI_ATRANS7_BASE
// Description : Physical address base for this virtual address range, in units
//               of 4 kiB (one flash sector).
//
//               Taking a 24-bit virtual address, firstly bits 23:22 (the two
//               MSBs) are masked to zero, and then BASE is added to bits 23:12
//               (the upper 12 bits) to form the physical address. Translation
//               wraps on a 16 MiB boundary.
#define QMI_ATRANS7_BASE_RESET  _u(0xc00)
#define QMI_ATRANS7_BASE_BITS   _u(0x00000fff)
#define QMI_ATRANS7_BASE_MSB    _u(11)
#define QMI_ATRANS7_BASE_LSB    _u(0)
#define QMI_ATRANS7_BASE_ACCESS "RW"
// =============================================================================
#endif // _HARDWARE_REGS_QMI_H

