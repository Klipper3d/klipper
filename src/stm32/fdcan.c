// Serial over CAN emulation for STM32 boards.
//
// Copyright (C) 2019 Eug Krashtan <eug.krashtan@gmail.com>
// Copyright (C) 2020 Pontus Borg <glpontus@gmail.com>
// Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "autoconf.h" // CONFIG_MACH_STM32F1
#include "board/irq.h" // irq_disable
#include "command.h" // DECL_CONSTANT_STR
#include "fasthash.h" // fasthash64
#include "generic/armcm_boot.h" // armcm_enable_irq
#include "generic/canbus.h" // canbus_notify_tx
#include "generic/serial_irq.h" // serial_rx_byte
#include "internal.h" // enable_pclock
#include "sched.h" // DECL_INIT

/*
 FDCAN max date length = 64bytes
 data_len[] is the data length & DLC mapping table
 Required when the data length exceeds 64bytes
 */
uint8_t data_len[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};

typedef struct
{
       uint32_t RESERVED0 : 18;
  __IO uint32_t ID : 11;
  __IO uint32_t RTR : 1;
  __IO uint32_t XTD : 1;
  __IO uint32_t ESI : 1;
  __IO uint32_t RXTS : 16;
  __IO uint32_t DLC : 4;
  __IO uint32_t BRS : 1;
  __IO uint32_t FDF : 1;
       uint32_t RESERVED1 : 2;
  __IO uint32_t FIDX : 7;
  __IO uint32_t ANMF : 1;
  __IO uint8_t  data[64];
}FDCAN_RX_FIFO_TypeDef;

typedef struct
{
  __IO uint32_t id_section;
  __IO uint32_t dlc_section;
  __IO uint32_t data[64 / 4];
}FDCAN_TX_FIFO_TypeDef;

typedef struct
{
  __IO uint32_t FLS[28]; // Filter list standard
  __IO uint32_t FLE[16]; // Filter list extended
  FDCAN_RX_FIFO_TypeDef RXF0[3];
  FDCAN_RX_FIFO_TypeDef RXF1[3];
  __IO uint32_t TEF[6]; // Tx event FIFO
  FDCAN_TX_FIFO_TypeDef TXFIFO[3];
}FDCAN_MSG_RAM_TypeDef;

typedef struct
{
  FDCAN_MSG_RAM_TypeDef fdcan1;
  FDCAN_MSG_RAM_TypeDef fdcan2;
}FDCAN_RAM_TypeDef;

FDCAN_RAM_TypeDef *fdcan_ram = (FDCAN_RAM_TypeDef *)(SRAMCAN_BASE);

#define FDCAN_IE_RX_FIFO0  (FDCAN_IE_RF0NE | FDCAN_IE_RF0FE | FDCAN_IE_RF0LE)
#define FDCAN_IE_RX_FIFO1  (FDCAN_IE_RF1NE | FDCAN_IE_RF1FE | FDCAN_IE_RF1LE)
#define FDCAN_IE_TC        (FDCAN_IE_TCE | FDCAN_IE_TCFE | FDCAN_IE_TFEE)

#if CONFIG_STM32_CANBUS_PB0_PB1
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB0,PB1");
 #define GPIO_Rx GPIO('B', 0)
 #define GPIO_Tx GPIO('B', 1)
#endif

#if CONFIG_MACH_STM32G0
 #if CONFIG_STM32_CANBUS_PB0_PB1
  #define SOC_CAN FDCAN2
  #define MSG_RAM fdcan_ram->fdcan2
 #else
  #error Uknown pins for STMF32G0 CAN
 #endif

 #define CAN_IT0_IRQn  TIM16_FDCAN_IT0_IRQn
 #define CAN_IT1_IRQn  TIM17_FDCAN_IT1_IRQn
 #define CAN_FUNCTION  GPIO_FUNCTION(3) // Alternative function mapping number
#endif

#ifndef SOC_CAN
 #error No known CAN device for configured MCU
#endif

// Read the next CAN packet
int
canbus_read(uint32_t *id, uint8_t *data)
{
    if (!(SOC_CAN->RXF0S & FDCAN_RXF0S_F0FL)) {
        // All rx mboxes empty, enable wake on rx IRQ
        irq_disable();
        SOC_CAN->IE |= FDCAN_IE_RF0NE;
        irq_enable();
        return -1;
    }

    // Read and ack packet
    uint32_t r_index = ((SOC_CAN->RXF0S & FDCAN_RXF0S_F0GI)
                        >> FDCAN_RXF0S_F0GI_Pos);
    FDCAN_RX_FIFO_TypeDef *rxf0 = &MSG_RAM.RXF0[r_index];
    uint32_t dlc = rxf0->DLC;
    *id = rxf0->ID;
    for (uint8_t i = 0; i < dlc; i++) {
        data[i] = rxf0->data[i];
    }
    SOC_CAN->RXF0A = r_index;

    return dlc;
}

// Transmit a packet
int
canbus_send(uint32_t id, uint32_t len, uint8_t *data)
{
    uint32_t txfqs = SOC_CAN->TXFQS;
    if (txfqs & FDCAN_TXFQS_TFQF) {
        // No space in transmit fifo - enable tx irq
        irq_disable();
        SOC_CAN->IE |= FDCAN_IE_TC;
        irq_enable();
        return -1;
    }

    uint32_t w_index = ((txfqs & FDCAN_TXFQS_TFQPI) >> FDCAN_TXFQS_TFQPI_Pos);
    FDCAN_TX_FIFO_TypeDef *txfifo = &MSG_RAM.TXFIFO[w_index];
    txfifo->id_section = id << 18;
    txfifo->dlc_section = len << 16;
    if (len) {
        txfifo->data[0] = (((uint32_t)data[3] << 24)
                           | ((uint32_t)data[2] << 16)
                           | ((uint32_t)data[1] << 8)
                           | ((uint32_t)data[0] << 0));
        txfifo->data[1] = (((uint32_t)data[7] << 24)
                           | ((uint32_t)data[6] << 16)
                           | ((uint32_t)data[5] << 8)
                           | ((uint32_t)data[4] << 0));
    }
    SOC_CAN->TXBAR = ((uint32_t)1 << w_index);
    return len;
}

void can_filter(uint32_t id, uint8_t index)
{
    MSG_RAM.FLS[index] = ((0x2 << 30) // Classic filter
                          | (0x1 << 27) // Store in Rx FIFO 0 if filter matches
                          | (id << 16)
                          | 0x7FF); // mask all enabled
}

// Setup the receive packet filter
void
canbus_set_filter(uint32_t id)
{
    /* Request initialisation */
    SOC_CAN->CCCR |= FDCAN_CCCR_INIT;
    /* Wait the acknowledge */
    while (!(SOC_CAN->CCCR & FDCAN_CCCR_INIT))
        ;
    /* Enable configuration change */
    SOC_CAN->CCCR |= FDCAN_CCCR_CCE;

    can_filter(CANBUS_ID_ADMIN, 0);

    /*  List size standard */
    SOC_CAN->RXGFC &= ~(FDCAN_RXGFC_LSS);
    SOC_CAN->RXGFC |= 1 << FDCAN_RXGFC_LSS_Pos;

    /* Filter remote frames with 11-bit standard IDs
       Non-matching frames standard reject or accept in Rx FIFO 1 */
    SOC_CAN->RXGFC &= ~(FDCAN_RXGFC_RRFS | FDCAN_RXGFC_ANFS);
    SOC_CAN->RXGFC |= ((0 << FDCAN_RXGFC_RRFS_Pos)
                       | ((id ? 0x01 : 0x02) << FDCAN_RXGFC_ANFS_Pos));

    /* Leave the initialisation mode for the filter */
    SOC_CAN->CCCR &= ~FDCAN_CCCR_CCE;
    SOC_CAN->CCCR &= ~FDCAN_CCCR_INIT;
}

// This function handles CAN global interrupts
void
CAN_IRQHandler(void)
{
    uint32_t ir = SOC_CAN->IR;
    uint32_t ie = SOC_CAN->IE;

    if (ir & FDCAN_IE_RX_FIFO1 && ie & FDCAN_IE_RX_FIFO1) {
        SOC_CAN->IR = FDCAN_IE_RX_FIFO1;

        if (SOC_CAN->RXF1S & FDCAN_RXF1S_F1FL) {
            // Read and ack data packet
            uint32_t r_index = ((SOC_CAN->RXF1S & FDCAN_RXF1S_F1GI)
                                >> FDCAN_RXF1S_F1GI_Pos);
            FDCAN_RX_FIFO_TypeDef *rxf1 = &MSG_RAM.RXF1[r_index];

            uint32_t rir_id = rxf1->ID;
            uint32_t dlc = rxf1->DLC;
            uint8_t data[8];
            for (uint8_t i = 0; i < dlc; i++) {
                data[i] = rxf1->data[i];
            }
            SOC_CAN->RXF1A = r_index;

            // Process packet
            canbus_process_data(rir_id, dlc, data);
        }
    }
    if (ie & FDCAN_IE_RX_FIFO0 && ir & FDCAN_IE_RX_FIFO0) {
        // Admin Rx
        SOC_CAN->IR = FDCAN_IE_RX_FIFO0;
        canbus_notify_rx();
    }
    if (ie & FDCAN_IE_TC && ir & FDCAN_IE_TC) {
        // Tx
        SOC_CAN->IR = FDCAN_IE_TC;
        canbus_notify_tx();
    }
}

static inline const uint32_t
make_btr(uint32_t sjw,       // Sync jump width, ... hmm
         uint32_t time_seg1, // time segment before sample point, 1 .. 16
         uint32_t time_seg2, // time segment after sample point, 1 .. 8
         uint32_t brp)       // Baud rate prescaler, 1 .. 1024
{
    return (((uint32_t)(sjw-1)) << FDCAN_NBTP_NSJW_Pos
            | ((uint32_t)(time_seg1-1)) << FDCAN_NBTP_NTSEG1_Pos
            | ((uint32_t)(time_seg2-1)) << FDCAN_NBTP_NTSEG2_Pos
            | ((uint32_t)(brp - 1)) << FDCAN_NBTP_NBRP_Pos);
}

static inline const uint32_t
compute_btr(uint32_t pclock, uint32_t bitrate)
{
    /*
        Some equations:
        Tpclock = 1 / pclock
        Tq      = brp * Tpclock
        Tbs1    = Tq * TS1
        Tbs2    = Tq * TS2
        NominalBitTime = Tq + Tbs1 + Tbs2
        BaudRate = 1/NominalBitTime
        Bit value sample point is after Tq+Tbs1. Ideal sample point
        is at 87.5% of NominalBitTime
        Use the lowest brp where ts1 and ts2 are in valid range
     */

    uint32_t bit_clocks = pclock / bitrate; // clock ticks per bit

    uint32_t sjw = 2;
    uint32_t qs;
    // Find number of time quantas that gives us the exact wanted bit time
    for (qs = 18; qs > 9; qs--) {
        // check that bit_clocks / quantas is an integer
        uint32_t brp_rem = bit_clocks % qs;
        if (brp_rem == 0)
            break;
    }
    uint32_t brp       = bit_clocks / qs;
    uint32_t time_seg2 = qs / 8; // sample at ~87.5%
    uint32_t time_seg1 = qs - (1 + time_seg2);

    return make_btr(sjw, time_seg1, time_seg2, brp);
}

void
can_init(void)
{
    enable_pclock((uint32_t)SOC_CAN);

    gpio_peripheral(GPIO_Rx, CAN_FUNCTION, 1);
    gpio_peripheral(GPIO_Tx, CAN_FUNCTION, 0);

    uint32_t pclock = get_pclock_frequency((uint32_t)SOC_CAN);

    uint32_t btr = compute_btr(pclock, CONFIG_CANBUS_FREQUENCY);

    /*##-1- Configure the CAN #######################################*/

    /* Exit from sleep mode */
    SOC_CAN->CCCR &= ~FDCAN_CCCR_CSR;
    /* Wait the acknowledge */
    while (SOC_CAN->CCCR & FDCAN_CCCR_CSA)
        ;
    /* Request initialisation */
    SOC_CAN->CCCR |= FDCAN_CCCR_INIT;
    /* Wait the acknowledge */
    while (!(SOC_CAN->CCCR & FDCAN_CCCR_INIT))
        ;
    /* Enable configuration change */
    SOC_CAN->CCCR |= FDCAN_CCCR_CCE;

    if (SOC_CAN == FDCAN1)
        FDCAN_CONFIG->CKDIV = 0;

    /* Disable protocol exception handling */
    SOC_CAN->CCCR |= FDCAN_CCCR_PXHD;

    SOC_CAN->NBTP = btr;

    /* Leave the initialisation mode */
    SOC_CAN->CCCR &= ~FDCAN_CCCR_CCE;
    SOC_CAN->CCCR &= ~FDCAN_CCCR_INIT;

    /*##-2- Configure the CAN Filter #######################################*/
    canbus_set_filter(0);

    /*##-3- Configure Interrupts #################################*/
    armcm_enable_irq(CAN_IRQHandler, CAN_IT0_IRQn, 0);
    if (CAN_IT0_IRQn != CAN_IT1_IRQn)
        armcm_enable_irq(CAN_IRQHandler, CAN_IT1_IRQn, 0);
    SOC_CAN->ILE |= 0x03;
    SOC_CAN->IE |= FDCAN_IE_RX_FIFO0 | FDCAN_IE_RX_FIFO1;

    // Convert unique 96-bit chip id into 48 bit representation
    uint64_t hash = fasthash64((uint8_t*)UID_BASE, 12, 0xA16231A7);
    canbus_set_uuid(&hash);
}
DECL_INIT(can_init);
