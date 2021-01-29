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

#if CONFIG_CAN_PINS_PA11_PA12
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PA11,PA12");
 #define GPIO_Rx GPIO('A', 11)
 #define GPIO_Tx GPIO('A', 12)
#endif
#if CONFIG_CAN_PINS_PB8_PB9
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB8,PB9");
 #define GPIO_Rx GPIO('B', 8)
 #define GPIO_Tx GPIO('B', 9)
#endif
#if CONFIG_CAN_PINS_PI8_PH13
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PI8,PH13");
 #define GPIO_Rx GPIO('I', 8)
 #define GPIO_Tx GPIO('H', 13)
#endif
#if CONFIG_CAN_PINS_PB5_PB6
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB5,PB6");
 #define GPIO_Rx GPIO('B', 5)
 #define GPIO_Tx GPIO('B', 6)
#endif
#if CONFIG_CAN_PINS_PB12_PB13
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB12,PB13");
 #define GPIO_Rx GPIO('B', 12)
 #define GPIO_Tx GPIO('B', 13)
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
 #warning CAN on STM32F4 is untested
 #if (CONFIG_CAN_PINS_PA11_PA12 || CONFIG_CAN_PINS_PB8_PB9 \
      || CONFIG_CAN_PINS_PI8_PH13)
  #define SOC_CAN CAN1
  #define CAN_RX0_IRQn  CAN1_RX0_IRQn
  #define CAN_RX1_IRQn  CAN1_RX1_IRQn
  #define CAN_TX_IRQn   CAN1_TX_IRQn
  #define CAN_SCE_IRQn  CAN1_SCE_IRQn
 #elif CONFIG_CAN_PINS_PB5_PB6 || CONFIG_CAN_PINS_PB12_PB13
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

// Read the next CAN packet
int
canbus_read(uint32_t *id, uint8_t *data)
{
    if (!(SOC_CAN->RF0R & CAN_RF0R_FMP0)) {
        // All rx mboxes empty, enable wake on rx IRQ
        irq_disable();
        SOC_CAN->IER |= CAN_IER_FMPIE0;
        irq_enable();
        return -1;
    }

    // Read and ack packet
    CAN_FIFOMailBox_TypeDef *mb = &SOC_CAN->sFIFOMailBox[0];
    uint32_t rir_id = (mb->RIR >> CAN_RI0R_STID_Pos) & 0x7FF;
    uint32_t dlc = mb->RDTR & CAN_RDT0R_DLC;
    uint32_t rdlr = mb->RDLR, rdhr = mb->RDHR;
    SOC_CAN->RF0R = CAN_RF0R_RFOM0;

    // Return packet
    *id = rir_id;
    data[0] = (rdlr >>  0) & 0xff;
    data[1] = (rdlr >>  8) & 0xff;
    data[2] = (rdlr >> 16) & 0xff;
    data[3] = (rdlr >> 24) & 0xff;
    data[4] = (rdhr >>  0) & 0xff;
    data[5] = (rdhr >>  8) & 0xff;
    data[6] = (rdhr >> 16) & 0xff;
    data[7] = (rdhr >> 24) & 0xff;
    return dlc;
}

// Transmit a packet
int
canbus_send(uint32_t id, uint32_t len, uint8_t *data)
{
    uint32_t tsr = SOC_CAN->TSR;
    if (!(tsr & (CAN_TSR_TME0|CAN_TSR_TME1|CAN_TSR_TME2))) {
        // No space in transmit fifo - enable tx irq
        irq_disable();
        SOC_CAN->IER |= CAN_IER_TMEIE;
        irq_enable();
        return -1;
    }
    int mbox = (tsr & CAN_TSR_CODE) >> CAN_TSR_CODE_Pos;
    CAN_TxMailBox_TypeDef *mb = &SOC_CAN->sTxMailBox[mbox];

    /* Set up the DLC */
    mb->TDTR = (mb->TDTR & 0xFFFFFFF0) | (len & 0x0F);

    /* Set up the data field */
    if (len) {
        mb->TDLR = (((uint32_t)data[3] << 24)
                    | ((uint32_t)data[2] << 16)
                    | ((uint32_t)data[1] << 8)
                    | ((uint32_t)data[0] << 0));
        mb->TDHR = (((uint32_t)data[7] << 24)
                    | ((uint32_t)data[6] << 16)
                    | ((uint32_t)data[5] << 8)
                    | ((uint32_t)data[4] << 0));
    }

    /* Request transmission */
    mb->TIR = (id << CAN_TI0R_STID_Pos) | CAN_TI0R_TXRQ;
    return len;
}

#define CAN_FILTER_NUMBER 0

// Setup the receive packet filter
static void
can_set_filter(uint32_t id1, uint32_t id2)
{
    uint32_t filternbrbitpos = 1 << CAN_FILTER_NUMBER;

    /* Select the start slave bank */
    SOC_CAN->FMR |= CAN_FMR_FINIT;
    /* Initialisation mode for the filter */
    SOC_CAN->FA1R = 0;

    SOC_CAN->sFilterRegister[CAN_FILTER_NUMBER].FR1 = id1 << (5 + 16);
    SOC_CAN->sFilterRegister[CAN_FILTER_NUMBER].FR2 = id2 << (5 + 16);

    /* Identifier list mode for the filter */
    SOC_CAN->FM1R = filternbrbitpos;
    /* 32-bit scale for the filter */
    SOC_CAN->FS1R = filternbrbitpos;

    /* FIFO 0 assigned for the filter */
    SOC_CAN->FFA1R = 0;

    /* Filter activation */
    SOC_CAN->FA1R = filternbrbitpos;
    /* Leave the initialisation mode for the filter */
    SOC_CAN->FMR &= ~CAN_FMR_FINIT;
}

void
canbus_set_dataport(uint32_t id)
{
    can_set_filter(CANBUS_ID_UUID, id);
}

void
canbus_reboot(void)
{
    NVIC_SystemReset();
}

// This function handles CAN global interrupts
void
CAN_IRQHandler(void)
{
    uint32_t ier = SOC_CAN->IER;
    if (ier & CAN_IER_FMPIE0 && SOC_CAN->RF0R & CAN_RF0R_FMP0) {
        // Rx
        SOC_CAN->IER = ier = ier & ~CAN_IER_FMPIE0;
        canbus_notify_rx();
    }
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

    uint32_t sjw =  2;
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
    can_set_filter(CANBUS_ID_UUID, CANBUS_ID_SET);

    /*##-3- Configure Interrupts #################################*/

    SOC_CAN->IER = CAN_IER_FMPIE0; // RX mailbox IRQ

    armcm_enable_irq(CAN_IRQHandler, CAN_RX0_IRQn, 0);
    if (CAN_RX0_IRQn != CAN_RX1_IRQn)
        armcm_enable_irq(CAN_IRQHandler, CAN_RX1_IRQn, 0);
    if (CAN_RX0_IRQn != CAN_TX_IRQn)
        armcm_enable_irq(CAN_IRQHandler, CAN_TX_IRQn, 0);

    // Convert unique 96-bit chip id into 48 bit representation
    uint64_t hash = fasthash64((uint8_t*)UID_BASE, 12, 0xA16231A7);
    canbus_set_uuid(&hash);
}
DECL_INIT(can_init);
