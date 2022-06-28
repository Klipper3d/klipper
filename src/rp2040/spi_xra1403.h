/*  Interface defintion for SPI XRA GPIO Expansion device
 *
 *  Copyright (C) 2022  Kevin Peck <kevindpeck@gmail.com>
 *
 *  This file may be distributed under the terms of the GNU GPLv3 license.
 */
#ifndef __SPI_XRA_H
#define __SPI_XRA_H

/* SPI Conventions for device
 *
 *  CS : Low is selected, only one message per toggle
 * CLK :
 *      CPOL=0 - Idle at zero, pulse at one
 *      CPHA=0 - Leading edge of clock pulse triggers R/W
 *      MAX_CLK - 26Mhz
 *
 * Data packet:
 * - 16 bit word/message defines all data transfers for send/receive
 * - Communication is half-duplex, only leading edge of clock pulse is read
 *      or write
 * - Write operation : first 8 bits are control, second 8 bits are out data
 *      , all return values are ignored
 * - Read operation : first 8 bits are control, second 8 bits are read data
 */

/* XRA registers */
#define XRA_GSR   0x00 /* GPIO State - Read-Only (0x00)*/
#define XRA_OCR   0x04 /* Output Control - R/W (0x02) Ouptput value Hi/Lo */
#define XRA_PIR   0x08 /* Input Polarity Inversion - R/W (0x04)*/
#define XRA_GCR   0x0C /* GPIO Config - R/W (0x06) Hi = Input*/
#define XRA_PUR   0x10 /* Input Int Pull-up En/Dis - R/W (0x80) Lo = No Pullup*/
#define XRA_IER   0x14 /* Input Interrupt En. - R/W (0x0A) Hi = Enabled */
#define XRA_TSCR  0x18 /* Output Three-State Control - R/W (0x0C)*/
#define XRA_ISR   0x1C /* Input Interrupt Status - Read-Only (0x0E)*/
#define XRA_REIR  0x20 /* Input Rising Edge Interrupt Enable - R/W (0x10)*/
#define XRA_FEIR  0x24 /* Input Falling Edge Interrupt Enable - R/W (0x12)*/
#define XRA_IFR   0x28 /* Input Filter Enable/Disable - R/W (0x14)*/

#define XRA_READ 0x01
#define XRA_WRITE 0x00

#define XRA_OUTPUT 0x00
#define XRA_INPUT 0x01

#define XRA_INDISABLE 0x00
#define XRA_INENABLE 0x01

#define XRA_PULDIS 0x00
#define XRA_PULEN 0x01

struct egpio_msg {
    uint8_t cmd;
    uint8_t dat;
};

#define XRA_BANKS 2
struct egpio_cfg {
    uint8_t io; // 1 - input, 0 - output
    uint8_t pul; // 1 - pullup, 0 - none
    uint8_t ien; // 1 - enabled, 0 - none
    uint8_t val;
};

/* Bit for Pin */
#define XRA_BIT_PIN(pin) (0x01 << ((pin) % 8))

/* Return bank number from provided pin number P0-15
 */
#define XRA_PIN2BANK(pin) ((pin) > 7)

/* Update just one pin in data P0-15 */
#define XRA_SET_PIN(data,pin,val)  \
    (data) = ( ((val) & 0x01) ? (data) | (XRA_BIT_PIN(pin)) \
        : (data) & ~(XRA_BIT_PIN(pin)) )

/* Get value of just one pin in data P0-15 */
#define XRA_GET_PIN(data,pin) ((data) & (XRA_BIT_PIN(pin)))

/* Prepare packet for XRA - 16 bit
 *      rw: 0 - write, 1 - read
 *    bank: 0 - P0-1, 1 - P8-15
 * control: Command/control code - see enum XRA_CMD
 *    data: 8 bit / unsigned char
 */
#define XRA_MSG(rw, bank, control, data) \
    (((rw)==XRA_WRITE ? 0x00 : 0x80) | ((control) & 0x3C) \
        | ((bank)>0 ? 0x02 : 0x00)),(data)

/* Two byte message length with read16/write16
 */
#define XRA_MSG_SIZE 2
#define XRA_MSG_BITS 8

/* Clock speed XRA
 */
#define MAX_XRA_CLK 26E6 /* 26 Mhz is max */

/* Board specific implementations of a GPIO Expander
 */
#if true
    // Board : PickSix
    #define SPI0_INTCS10_PIN 0
    #define SPI0_TX_PIN 3
    #define SPI0_SCK_PIN 2
    #define SPI0_RX_PIN 4
    #define SPI0_CS10_PIN 5

    #define SPI0_PICKSIX_MODE 0
    #define SPI0_PICKSIX_BUS_OID 7
#else
#endif

#endif // spi_xra1403.h
