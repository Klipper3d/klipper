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
#include "generic/armcm_boot.h" // armcm_enable_irq
#include "generic/canbus.h" // canbus_notify_tx
#include "generic/canserial.h" // CANBUS_ID_ADMIN
#include "generic/serial_irq.h" // serial_rx_byte
#include "internal.h" // enable_pclock
#include "sched.h" // DECL_INIT

typedef struct
{
  __IO uint32_t id_section;
  __IO uint32_t dlc_section;
  __IO uint32_t data[64 / 4];
}FDCAN_FIFO_TypeDef;

#define FDCAN_XTD (1<<30)
#define FDCAN_RTR (1<<29)

typedef struct
{
  __IO uint32_t FLS[28]; // Filter list standard
  __IO uint32_t FLE[16]; // Filter list extended
  FDCAN_FIFO_TypeDef RXF0[3];
  FDCAN_FIFO_TypeDef RXF1[3];
  __IO uint32_t TEF[6]; // Tx event FIFO
  FDCAN_FIFO_TypeDef TXFIFO[3];
}FDCAN_MSG_RAM_TypeDef;

typedef struct
{
  FDCAN_MSG_RAM_TypeDef fdcan1;
  FDCAN_MSG_RAM_TypeDef fdcan2;
}FDCAN_RAM_TypeDef;

FDCAN_RAM_TypeDef *fdcan_ram = (FDCAN_RAM_TypeDef *)(SRAMCAN_BASE);

#define FDCAN_IE_TC        (FDCAN_IE_TCE | FDCAN_IE_TCFE | FDCAN_IE_TFEE)

#if CONFIG_STM32_CANBUS_PA11_PA12
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PA11,PA12");
 #define GPIO_Rx GPIO('A', 11)
 #define GPIO_Tx GPIO('A', 12)
#elif CONFIG_STM32_CANBUS_PB8_PB9
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB8,PB9");
 #define GPIO_Rx GPIO('B', 8)
 #define GPIO_Tx GPIO('B', 9)
#elif CONFIG_STM32_CANBUS_PB0_PB1
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB0,PB1");
 #define GPIO_Rx GPIO('B', 0)
 #define GPIO_Tx GPIO('B', 1)
#endif

#if !CONFIG_STM32_CANBUS_PB0_PB1
 #define SOC_CAN FDCAN1
 #define MSG_RAM fdcan_ram->fdcan1
#else
 #define SOC_CAN FDCAN2
 #define MSG_RAM fdcan_ram->fdcan2
#endif

#define CAN_IT0_IRQn  TIM16_FDCAN_IT0_IRQn
#define CAN_FUNCTION  GPIO_FUNCTION(3) // Alternative function mapping number

#ifndef SOC_CAN
 #error No known CAN device for configured MCU
#endif

// Transmit a packet
int
canbus_send(struct canbus_msg *msg)
{
    uint32_t txfqs = SOC_CAN->TXFQS;
    if (txfqs & FDCAN_TXFQS_TFQF)
        // No space in transmit fifo - wait for irq
        return -1;

    uint32_t w_index = ((txfqs & FDCAN_TXFQS_TFQPI) >> FDCAN_TXFQS_TFQPI_Pos);
    FDCAN_FIFO_TypeDef *txfifo = &MSG_RAM.TXFIFO[w_index];
    uint32_t ids;
    if (msg->id & CANMSG_ID_EFF)
        ids = (msg->id & 0x1fffffff) | FDCAN_XTD;
    else
        ids = (msg->id & 0x7ff) << 18;
    ids |= msg->id & CANMSG_ID_RTR ? FDCAN_RTR : 0;
    txfifo->id_section = ids;
    txfifo->dlc_section = (msg->dlc & 0x0f) << 16;
    txfifo->data[0] = msg->data32[0];
    txfifo->data[1] = msg->data32[1];
    SOC_CAN->TXBAR = ((uint32_t)1 << w_index);
    return CANMSG_DATA_LEN(msg);
}

static void
can_filter(uint32_t index, uint32_t id)
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
    if (!CONFIG_CANBUS_FILTER)
        return;
    /* Request initialisation */
    SOC_CAN->CCCR |= FDCAN_CCCR_INIT;
    /* Wait the acknowledge */
    while (!(SOC_CAN->CCCR & FDCAN_CCCR_INIT))
        ;
    /* Enable configuration change */
    SOC_CAN->CCCR |= FDCAN_CCCR_CCE;

    // Load filter
    can_filter(0, CANBUS_ID_ADMIN);
    can_filter(1, id);
    can_filter(2, id + 1);
    SOC_CAN->RXGFC = ((id ? 3 : 1) << FDCAN_RXGFC_LSS_Pos
                      | 0x02 << FDCAN_RXGFC_ANFS_Pos);

    /* Leave the initialisation mode for the filter */
    SOC_CAN->CCCR &= ~FDCAN_CCCR_CCE;
    SOC_CAN->CCCR &= ~FDCAN_CCCR_INIT;
}

// This function handles CAN global interrupts
void
CAN_IRQHandler(void)
{
    uint32_t ir = SOC_CAN->IR;

    if (ir & FDCAN_IE_RF0NE) {
        SOC_CAN->IR = FDCAN_IE_RF0NE;

        uint32_t rxf0s = SOC_CAN->RXF0S;
        if (rxf0s & FDCAN_RXF0S_F0FL) {
            // Read and ack data packet
            uint32_t idx = (rxf0s & FDCAN_RXF0S_F0GI) >> FDCAN_RXF0S_F0GI_Pos;
            FDCAN_FIFO_TypeDef *rxf0 = &MSG_RAM.RXF0[idx];
            uint32_t ids = rxf0->id_section;
            struct canbus_msg msg;
            if (ids & FDCAN_XTD)
                msg.id = (ids & 0x1fffffff) | CANMSG_ID_EFF;
            else
                msg.id = (ids >> 18) & 0x7ff;
            msg.id |= ids & FDCAN_RTR ? CANMSG_ID_RTR : 0;
            msg.dlc = (rxf0->dlc_section >> 16) & 0x0f;
            msg.data32[0] = rxf0->data[0];
            msg.data32[1] = rxf0->data[1];
            SOC_CAN->RXF0A = idx;

            // Process packet
            canbus_process_data(&msg);
        }
    }
    if (ir & FDCAN_IE_TC) {
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
    SOC_CAN->ILE = FDCAN_ILE_EINT0;
    SOC_CAN->IE = FDCAN_IE_RF0NE | FDCAN_IE_TC;
}
DECL_INIT(can_init);
