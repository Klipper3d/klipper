// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _INTCTRL_H
#define _INTCTRL_H

/**
 * \file rp2350/intctrl.h
 */

#ifdef __ASSEMBLER__
#define TIMER0_IRQ_0 0
#define TIMER0_IRQ_1 1
#define TIMER0_IRQ_2 2
#define TIMER0_IRQ_3 3
#define TIMER1_IRQ_0 4
#define TIMER1_IRQ_1 5
#define TIMER1_IRQ_2 6
#define TIMER1_IRQ_3 7
#define PWM_IRQ_WRAP_0 8
#define PWM_IRQ_WRAP_1 9
#define DMA_IRQ_0 10
#define DMA_IRQ_1 11
#define DMA_IRQ_2 12
#define DMA_IRQ_3 13
#define USBCTRL_IRQ 14
#define PIO0_IRQ_0 15
#define PIO0_IRQ_1 16
#define PIO1_IRQ_0 17
#define PIO1_IRQ_1 18
#define PIO2_IRQ_0 19
#define PIO2_IRQ_1 20
#define IO_IRQ_BANK0 21
#define IO_IRQ_BANK0_NS 22
#define IO_IRQ_QSPI 23
#define IO_IRQ_QSPI_NS 24
#define SIO_IRQ_FIFO 25
#define SIO_IRQ_BELL 26
#define SIO_IRQ_FIFO_NS 27
#define SIO_IRQ_BELL_NS 28
#define SIO_IRQ_MTIMECMP 29
#define CLOCKS_IRQ 30
#define SPI0_IRQ 31
#define SPI1_IRQ 32
#define UART0_IRQ 33
#define UART1_IRQ 34
#define ADC_IRQ_FIFO 35
#define I2C0_IRQ 36
#define I2C1_IRQ 37
#define OTP_IRQ 38
#define TRNG_IRQ 39
#define PROC0_IRQ_CTI 40
#define PROC1_IRQ_CTI 41
#define PLL_SYS_IRQ 42
#define PLL_USB_IRQ 43
#define POWMAN_IRQ_POW 44
#define POWMAN_IRQ_TIMER 45
#define SPAREIRQ_IRQ_0 46
#define SPAREIRQ_IRQ_1 47
#define SPAREIRQ_IRQ_2 48
#define SPAREIRQ_IRQ_3 49
#define SPAREIRQ_IRQ_4 50
#define SPAREIRQ_IRQ_5 51
#else
/**
 * \brief Interrupt numbers on RP2350 (used as typedef \ref irq_num_t)
 * \ingroup hardware_irq
 */
typedef enum irq_num_rp2350 {
    TIMER0_IRQ_0 = 0, ///< Select TIMER0's IRQ 0 output
    TIMER0_IRQ_1 = 1, ///< Select TIMER0's IRQ 1 output
    TIMER0_IRQ_2 = 2, ///< Select TIMER0's IRQ 2 output
    TIMER0_IRQ_3 = 3, ///< Select TIMER0's IRQ 3 output
    TIMER1_IRQ_0 = 4, ///< Select TIMER1's IRQ 0 output
    TIMER1_IRQ_1 = 5, ///< Select TIMER1's IRQ 1 output
    TIMER1_IRQ_2 = 6, ///< Select TIMER1's IRQ 2 output
    TIMER1_IRQ_3 = 7, ///< Select TIMER1's IRQ 3 output
    PWM_IRQ_WRAP_0 = 8, ///< Select PWM's IRQ_WRAP 0 output
    PWM_IRQ_WRAP_1 = 9, ///< Select PWM's IRQ_WRAP 1 output
    DMA_IRQ_0 = 10, ///< Select DMA's IRQ 0 output
    DMA_IRQ_1 = 11, ///< Select DMA's IRQ 1 output
    DMA_IRQ_2 = 12, ///< Select DMA's IRQ 2 output
    DMA_IRQ_3 = 13, ///< Select DMA's IRQ 3 output
    USBCTRL_IRQ = 14, ///< Select USBCTRL's IRQ output
    PIO0_IRQ_0 = 15, ///< Select PIO0's IRQ 0 output
    PIO0_IRQ_1 = 16, ///< Select PIO0's IRQ 1 output
    PIO1_IRQ_0 = 17, ///< Select PIO1's IRQ 0 output
    PIO1_IRQ_1 = 18, ///< Select PIO1's IRQ 1 output
    PIO2_IRQ_0 = 19, ///< Select PIO2's IRQ 0 output
    PIO2_IRQ_1 = 20, ///< Select PIO2's IRQ 1 output
    IO_IRQ_BANK0 = 21, ///< Select IO_BANK0's IRQ output
    IO_IRQ_BANK0_NS = 22, ///< Select IO_BANK0_NS's IRQ output
    IO_IRQ_QSPI = 23, ///< Select IO_QSPI's IRQ output
    IO_IRQ_QSPI_NS = 24, ///< Select IO_QSPI_NS's IRQ output
    SIO_IRQ_FIFO = 25, ///< Select SIO's IRQ_FIFO output
    SIO_IRQ_BELL = 26, ///< Select SIO's IRQ_BELL output
    SIO_IRQ_FIFO_NS = 27, ///< Select SIO_NS's IRQ_FIFO output
    SIO_IRQ_BELL_NS = 28, ///< Select SIO_NS's IRQ_BELL output
    SIO_IRQ_MTIMECMP = 29, ///< Select SIO_IRQ_MTIMECMP's IRQ output
    CLOCKS_IRQ = 30, ///< Select CLOCKS's IRQ output
    SPI0_IRQ = 31, ///< Select SPI0's IRQ output
    SPI1_IRQ = 32, ///< Select SPI1's IRQ output
    UART0_IRQ = 33, ///< Select UART0's IRQ output
    UART1_IRQ = 34, ///< Select UART1's IRQ output
    ADC_IRQ_FIFO = 35, ///< Select ADC's IRQ_FIFO output
    I2C0_IRQ = 36, ///< Select I2C0's IRQ output
    I2C1_IRQ = 37, ///< Select I2C1's IRQ output
    OTP_IRQ = 38, ///< Select OTP's IRQ output
    TRNG_IRQ = 39, ///< Select TRNG's IRQ output
    PROC0_IRQ_CTI = 40, ///< Select PROC0's IRQ_CTI output
    PROC1_IRQ_CTI = 41, ///< Select PROC1's IRQ_CTI output
    PLL_SYS_IRQ = 42, ///< Select PLL_SYS's IRQ output
    PLL_USB_IRQ = 43, ///< Select PLL_USB's IRQ output
    POWMAN_IRQ_POW = 44, ///< Select POWMAN's IRQ_POW output
    POWMAN_IRQ_TIMER = 45, ///< Select POWMAN's IRQ_TIMER output
    SPARE_IRQ_0 = 46, ///< Select SPARE IRQ 0
    SPARE_IRQ_1 = 47, ///< Select SPARE IRQ 1
    SPARE_IRQ_2 = 48, ///< Select SPARE IRQ 2
    SPARE_IRQ_3 = 49, ///< Select SPARE IRQ 3
    SPARE_IRQ_4 = 50, ///< Select SPARE IRQ 4
    SPARE_IRQ_5 = 51, ///< Select SPARE IRQ 5
    IRQ_COUNT
} irq_num_t;
#endif

#define isr_timer0_0 isr_irq0
#define isr_timer0_1 isr_irq1
#define isr_timer0_2 isr_irq2
#define isr_timer0_3 isr_irq3
#define isr_timer1_0 isr_irq4
#define isr_timer1_1 isr_irq5
#define isr_timer1_2 isr_irq6
#define isr_timer1_3 isr_irq7
#define isr_pwm_wrap_0 isr_irq8
#define isr_pwm_wrap_1 isr_irq9
#define isr_dma_0 isr_irq10
#define isr_dma_1 isr_irq11
#define isr_dma_2 isr_irq12
#define isr_dma_3 isr_irq13
#define isr_usbctrl isr_irq14
#define isr_pio0_0 isr_irq15
#define isr_pio0_1 isr_irq16
#define isr_pio1_0 isr_irq17
#define isr_pio1_1 isr_irq18
#define isr_pio2_0 isr_irq19
#define isr_pio2_1 isr_irq20
#define isr_io_bank0 isr_irq21
#define isr_io_bank0_ns isr_irq22
#define isr_io_qspi isr_irq23
#define isr_io_qspi_ns isr_irq24
#define isr_sio_fifo isr_irq25
#define isr_sio_bell isr_irq26
#define isr_sio_fifo_ns isr_irq27
#define isr_sio_bell_ns isr_irq28
#define isr_sio_mtimecmp isr_irq29
#define isr_clocks isr_irq30
#define isr_spi0 isr_irq31
#define isr_spi1 isr_irq32
#define isr_uart0 isr_irq33
#define isr_uart1 isr_irq34
#define isr_adc_fifo isr_irq35
#define isr_i2c0 isr_irq36
#define isr_i2c1 isr_irq37
#define isr_otp isr_irq38
#define isr_trng isr_irq39
#define isr_proc0_cti isr_irq40
#define isr_proc1_cti isr_irq41
#define isr_pll_sys isr_irq42
#define isr_pll_usb isr_irq43
#define isr_powman_pow isr_irq44
#define isr_powman_timer isr_irq45
#define isr_spare_0 isr_irq46
#define isr_spare_1 isr_irq47
#define isr_spare_2 isr_irq48
#define isr_spare_3 isr_irq49
#define isr_spare_4 isr_irq50
#define isr_spare_5 isr_irq51

#endif // _INTCTRL_H

