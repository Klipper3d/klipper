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
#include "internal.h" // enable_pclock
#include "sched.h" // DECL_INIT

#if CONFIG_STM32_CANBUS_PA11_PA12 || CONFIG_STM32_CANBUS_PA11_PA12_REMAP
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PA11,PA12");
 #define GPIO_Rx GPIO('A', 11)
 #define GPIO_Tx GPIO('A', 12)
#endif
#if CONFIG_STM32_CANBUS_PB8_PB9
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB8,PB9");
 #define GPIO_Rx GPIO('B', 8)
 #define GPIO_Tx GPIO('B', 9)
#endif
#if CONFIG_STM32_CANBUS_PI9_PH13
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PI9,PH13");
 #define GPIO_Rx GPIO('I', 9)
 #define GPIO_Tx GPIO('H', 13)
#endif
#if CONFIG_STM32_CANBUS_PB5_PB6
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB5,PB6");
 #define GPIO_Rx GPIO('B', 5)
 #define GPIO_Tx GPIO('B', 6)
#endif
#if CONFIG_STM32_CANBUS_PB12_PB13
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB12,PB13");
 #define GPIO_Rx GPIO('B', 12)
 #define GPIO_Tx GPIO('B', 13)
#endif
#if CONFIG_STM32_CANBUS_PD0_PD1
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PD0,PD1");
 #define GPIO_Rx GPIO('D', 0)
 #define GPIO_Tx GPIO('D', 1)
#endif

#if CONFIG_MACH_STM32F0
 #define SOC_CAN CAN
 #define CAN_RX0_IRQn  CEC_CAN_IRQn
 #define CAN_RX1_IRQn  CEC_CAN_IRQn
 #define CAN_TX_IRQn   CEC_CAN_IRQn
 #define CAN_SCE_IRQn  CEC_CAN_IRQn
 #define CAN_FUNCTION  GPIO_FUNCTION(4)  // Alternative function mapping number
#endif

#if CONFIG_MACH_STM32F1
 #define SOC_CAN CAN1
 #define CAN_RX0_IRQn  CAN1_RX0_IRQn
 #define CAN_RX1_IRQn  CAN1_RX1_IRQn
 #define CAN_TX_IRQn   CAN1_TX_IRQn
 #define CAN_SCE_IRQn  CAN1_SCE_IRQn
 #define CAN_FUNCTION  GPIO_FUNCTION(9)  // Alternative function mapping number
#endif

#if CONFIG_MACH_STM32F4
 #if (CONFIG_STM32_CANBUS_PA11_PA12 || CONFIG_STM32_CANBUS_PB8_PB9 \
     || CONFIG_STM32_CANBUS_PD0_PD1 || CONFIG_STM32_CANBUS_PI9_PH13)
  #define SOC_CAN CAN1
  #define CAN_RX0_IRQn  CAN1_RX0_IRQn
  #define CAN_RX1_IRQn  CAN1_RX1_IRQn
  #define CAN_TX_IRQn   CAN1_TX_IRQn
  #define CAN_SCE_IRQn  CAN1_SCE_IRQn
 #elif CONFIG_STM32_CANBUS_PB5_PB6 || CONFIG_STM32_CANBUS_PB12_PB13
  #define SOC_CAN CAN2
  #define CAN_RX0_IRQn  CAN2_RX0_IRQn
  #define CAN_RX1_IRQn  CAN2_RX1_IRQn
  #define CAN_TX_IRQn   CAN2_TX_IRQn
  #define CAN_SCE_IRQn  CAN2_SCE_IRQn
 #else
  #error Uknown pins for STMF32F4 CAN
 #endif

 #define CAN_FUNCTION  GPIO_FUNCTION(9) // Alternative function mapping number
#endif

#ifndef SOC_CAN
 #error No known CAN device for configured MCU
#endif

// Transmit a packet
int
canbus_send(struct canbus_msg *msg)
{
    uint32_t tsr = SOC_CAN->TSR;
    if (!(tsr & (CAN_TSR_TME0|CAN_TSR_TME1|CAN_TSR_TME2))) {
        // No space in transmit fifo - enable tx irq
        irq_disable();
        SOC_CAN->IER |= CAN_IER_TMEIE;
        irq_enable();
        return -1;
    }
    int mbox = 2;
    if (tsr & CAN_TSR_TME0)
        mbox = 0;
    else if (tsr & CAN_TSR_TME1)
        mbox = 1;
    CAN_TxMailBox_TypeDef *mb = &SOC_CAN->sTxMailBox[mbox];

    /* Set up the DLC */
    mb->TDTR = (mb->TDTR & 0xFFFFFFF0) | (msg->dlc & 0x0F);

    /* Set up the data field */
    mb->TDLR = msg->data32[0];
    mb->TDHR = msg->data32[1];

    /* Request transmission */
    uint32_t tir;
    if (msg->id & CANMSG_ID_EFF)
        tir = ((msg->id & 0x1fffffff) << CAN_TI0R_EXID_Pos) | CAN_TI0R_IDE;
    else
        tir = (msg->id & 0x7ff) << CAN_TI0R_STID_Pos;
    tir |= msg->id & CANMSG_ID_RTR ? CAN_TI0R_RTR : 0;
    mb->TIR = (msg->id << CAN_TI0R_STID_Pos) | CAN_TI0R_TXRQ;
    return CANMSG_DATA_LEN(msg);
}

// Setup the receive packet filter
void
canbus_set_filter(uint32_t id)
{
    /* Select the start slave bank */
    SOC_CAN->FMR |= CAN_FMR_FINIT;
    /* Initialisation mode for the filter */
    SOC_CAN->FA1R = 0;

    if (CONFIG_CANBUS_FILTER) {
        uint32_t mask = CAN_TI0R_STID | CAN_TI0R_IDE | CAN_TI0R_RTR;
        SOC_CAN->sFilterRegister[0].FR1 = CANBUS_ID_ADMIN << CAN_RI0R_STID_Pos;
        SOC_CAN->sFilterRegister[0].FR2 = mask;
        SOC_CAN->sFilterRegister[1].FR1 = (id + 1) << CAN_RI0R_STID_Pos;
        SOC_CAN->sFilterRegister[1].FR2 = mask;
        SOC_CAN->sFilterRegister[2].FR1 = id << CAN_RI0R_STID_Pos;
        SOC_CAN->sFilterRegister[2].FR2 = mask;
    } else {
        SOC_CAN->sFilterRegister[0].FR1 = 0;
        SOC_CAN->sFilterRegister[0].FR2 = 0;
        id = 0;
    }

    /* 32-bit scale for the filter */
    SOC_CAN->FS1R = (1<<0) | (1<<1) | (1<<2);

    /* Filter activation */
    SOC_CAN->FA1R = (1<<0) | (id ? (1<<1) | (1<<2) : 0);
    /* Leave the initialisation mode for the filter */
    SOC_CAN->FMR &= ~CAN_FMR_FINIT;
}

// This function handles CAN global interrupts
void
CAN_IRQHandler(void)
{
    if (SOC_CAN->RF0R & CAN_RF0R_FMP0) {
        // Read and ack data packet
        CAN_FIFOMailBox_TypeDef *mb = &SOC_CAN->sFIFOMailBox[0];
        uint32_t rir = mb->RIR;
        struct canbus_msg msg;
        if (rir & CAN_RI0R_IDE)
            msg.id = ((rir >> CAN_RI0R_EXID_Pos) & 0x1fffffff) | CANMSG_ID_EFF;
        else
            msg.id = (rir >> CAN_RI0R_STID_Pos) & 0x7ff;
        msg.id |= rir & CAN_RI0R_RTR ? CANMSG_ID_RTR : 0;
        msg.dlc = mb->RDTR & CAN_RDT0R_DLC;
        msg.data32[0] = mb->RDLR;
        msg.data32[1] = mb->RDHR;
        SOC_CAN->RF0R = CAN_RF0R_RFOM0;

        // Process packet
        canbus_process_data(&msg);
    }
    uint32_t ier = SOC_CAN->IER;
    if (ier & CAN_IER_TMEIE
        && SOC_CAN->TSR & (CAN_TSR_RQCP0|CAN_TSR_RQCP1|CAN_TSR_RQCP2)) {
        // Tx
        SOC_CAN->IER = ier & ~CAN_IER_TMEIE;
        canbus_notify_tx();
    }
}

static inline const uint32_t
make_btr(uint32_t sjw,       // Sync jump width, ... hmm
         uint32_t time_seg1, // time segment before sample point, 1 .. 16
         uint32_t time_seg2, // time segment after sample point, 1 .. 8
         uint32_t brp)       // Baud rate prescaler, 1 .. 1024
{
    return (((uint32_t)(sjw-1)) << CAN_BTR_SJW_Pos
            | ((uint32_t)(time_seg1-1)) << CAN_BTR_TS1_Pos
            | ((uint32_t)(time_seg2-1)) << CAN_BTR_TS2_Pos
            | ((uint32_t)(brp - 1)) << CAN_BTR_BRP_Pos);
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

    /* Request initialisation */
    SOC_CAN->MCR = CAN_MCR_INRQ;
    /* Wait the acknowledge */
    while (!(SOC_CAN->MSR & CAN_MSR_INAK))
        ;

    SOC_CAN->BTR = btr;

    // TXFP makes packets posted to the TX mboxes transmit in chronologcal order
    // ABOM makes the hardware automatically leave bus-off state
    SOC_CAN->MCR = CAN_MCR_TXFP | CAN_MCR_ABOM;
    /* Wait the acknowledge */
    while (SOC_CAN->MSR & CAN_MSR_INAK)
        ;

    /*##-2- Configure the CAN Filter #######################################*/
    canbus_set_filter(0);

    /*##-3- Configure Interrupts #################################*/
    armcm_enable_irq(CAN_IRQHandler, CAN_RX0_IRQn, 0);
    if (CAN_RX0_IRQn != CAN_RX1_IRQn)
        armcm_enable_irq(CAN_IRQHandler, CAN_RX1_IRQn, 0);
    if (CAN_RX0_IRQn != CAN_TX_IRQn)
        armcm_enable_irq(CAN_IRQHandler, CAN_TX_IRQn, 0);
    SOC_CAN->IER = CAN_IER_FMPIE0;
}
DECL_INIT(can_init);
