/*
 * Serial over CAN emulation for STM32 boards.
 *
 *  Copyright (C) 2019 Eug Krashtan <eug.krashtan@gmail.com>
 *  Copyright (C) 2020 Pontus Borg <glpontus@gmail.com>
 *  This file may be distributed under the terms of the GNU GPLv3 license.
 *
 */

#include "autoconf.h" //
#include "board/armcm_boot.h" // armcm_enable_irq
#include "board/serial_irq.h" // serial_rx_byte
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // enable_pclock
#include "sched.h" // DECL_INIT
#include <string.h>
#include "can.h"
#include <fasthash.h>

#if (CONFIG_CAN_PINS_PA11_PA12)
DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PA11,PA12");
#define GPIO_Rx GPIO('A', 11)
#define GPIO_Tx GPIO('A', 12)
#endif
#if (CONFIG_CAN_PINS_PB8_PB9)
DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB8,PB9");
#define GPIO_Rx GPIO('B', 8)
#define GPIO_Tx GPIO('B', 9)
#endif
#if (CONFIG_CAN_PINS_PI8_PH13)
DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PI8,PH13");
#define GPIO_Rx GPIO('I', 8)
#define GPIO_Tx GPIO('H', 13)
#endif
#if (CONFIG_CAN_PINS_PB5_PB6)
DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB5,PB6");
#define GPIO_Rx GPIO('B', 5)
#define GPIO_Tx GPIO('B', 6)
#endif
#if (CONFIG_CAN_PINS_PB12_PB13)
DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB12,PB13");
#define GPIO_Rx GPIO('B', 12)
#define GPIO_Tx GPIO('B', 13)
#endif

#if    (CONFIG_MACH_STM32F0)
#define SOC_CAN CAN
#define CAN_RX0_IRQn  CEC_CAN_IRQn
#define CAN_RX1_IRQn  CEC_CAN_IRQn
#define CAN_TX_IRQn   CEC_CAN_IRQn
#define CAN_SCE_IRQn  CEC_CAN_IRQn
#define CAN_FUNCTION  GPIO_FUNCTION(4)  // Alternative function mapping number
#endif

#if (CONFIG_MACH_STM32F1)
#define SOC_CAN CAN1
#define CAN_RX0_IRQn  CAN1_RX0_IRQn
#define CAN_RX1_IRQn  CAN1_RX1_IRQn
#define CAN_TX_IRQn   CAN1_TX_IRQn
#define CAN_SCE_IRQn  CAN1_SCE_IRQn
#define CAN_FUNCTION  GPIO_FUNCTION(9)  // Alternative function mapping number
#endif


#if (CONFIG_MACH_STM32F4)
#warning CAN on STM32F4 is untested
#if (CONFIG_CAN_PINS_PA11_PA12 ||
     CONFIG_CAN_PINS_PB8_PB9 ||
     CONFIG_CAN_PINS_PI8_PH13)
#define SOC_CAN CAN1
#define CAN_RX0_IRQn  CAN1_RX0_IRQn
#define CAN_RX1_IRQn  CAN1_RX1_IRQn
#define CAN_TX_IRQn   CAN1_TX_IRQn
#define CAN_SCE_IRQn  CAN1_SCE_IRQn
#elsif ((CONFIG_CAN_PINS_PB5_PB6 || CONFIG_CAN_PINS_PB12_PB13)
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


// TXFP makes packets posted to the TX mboxes transmit in chronologcal order
// ABOM makes the hardware automatically leave bus-off state
#define MCR_FLAGS (CAN_MCR_TXFP | CAN_MCR_ABOM)

#define CAN_FILTER_NUMBER 0

static uint16_t MyCanId = 0;

static int can_find_empty_tx_mbox(void) {
    uint32_t tsr = SOC_CAN->TSR;
    if(tsr & CAN_TSR_TME0) return 0;
    if(tsr & CAN_TSR_TME1) return 1;
    if(tsr & CAN_TSR_TME2) return 2;
    return -1;
}

static void can_transmit_mbox(uint32_t id, int mbox, uint32_t dlc, uint8_t *pkt)
{
     CAN_TxMailBox_TypeDef *mb = &SOC_CAN->sTxMailBox[mbox];
    /* Set up the Id */
    mb->TIR &= CAN_TI0R_TXRQ;
    mb->TIR |= (id << CAN_TI0R_STID_Pos);

    /* Set up the DLC */
    mb->TDTR &= 0xFFFFFFF0U;
    mb->TDTR |= (dlc & 0xFU);

    /* Set up the data field */
    if(pkt) {
      mb->TDLR = ((uint32_t)pkt[3] << 24) |
                 ((uint32_t)pkt[2] << 16) |
                 ((uint32_t)pkt[1] << 8) |
                 ((uint32_t)pkt[0] << 0);
      mb->TDHR = ((uint32_t)pkt[7] << 24) |
                 ((uint32_t)pkt[6] << 16) |
                 ((uint32_t)pkt[5] << 8) |
                 ((uint32_t)pkt[4] << 0);
    }

     /* Request transmission */
    __sync_synchronize(); // disable write optimization
    mb->TIR |= CAN_TI0R_TXRQ;

}

// Blocking transmit function, it can race with the IRQ driven TX handler.
// This should(tm) not happen
static void can_transmit(uint32_t id, uint32_t dlc, uint8_t *pkt)
{
    int mbox = -1;

    do {
      mbox = can_find_empty_tx_mbox();
    } while(mbox < 0);

    can_transmit_mbox(id, mbox, dlc, pkt);
}

// Convert Unique 96-bit value into 48 bit representation
static void pack_uuid(uint8_t* u)
{
    uint64_t hash = fasthash64((uint8_t*)UID_BASE, 12, 0xA16231A7);
    memcpy(u, &hash, SHORT_UUID_LEN);
}

static void can_uuid_resp(void)
{
    uint8_t short_uuid[SHORT_UUID_LEN];
    pack_uuid(short_uuid);
    can_transmit(PKT_ID_UUID_RESP, SHORT_UUID_LEN, short_uuid);
}

static void get_rx_data(uint8_t* buf, unsigned int mbox)
{
    uint32_t rdlr = SOC_CAN->sFIFOMailBox[mbox].RDLR;
    buf[0] = (rdlr >>  0) & 0xff;
    buf[1] = (rdlr >>  8) & 0xff;
    buf[2] = (rdlr >> 16) & 0xff;
    buf[3] = (rdlr >> 24) & 0xff;
    uint32_t rdhr = SOC_CAN->sFIFOMailBox[mbox].RDHR;
    buf[4] = (rdhr >>  0) & 0xff;
    buf[5] = (rdhr >>  8) & 0xff;
    buf[6] = (rdhr >> 16) & 0xff;
    buf[7] = (rdhr >> 24) & 0xff;
}

// Return true if more data is available to send or mailboxes are full
int CAN_TxIrq(void) {
    int txdata = 1;

    // TODO: We need some kind of error handling?

    while(txdata) {
        int mbox = can_find_empty_tx_mbox();
        if(mbox < 0) {
            // All mboxes full, wait for next IRQ
            return 1;
        }
        int i=0;
        uint8_t databuf[8];
        for (;i<8;i++)
        {
            if(serial_get_tx_byte(&(databuf[i])) == -1) {
                txdata = 0;
                break;
            }
        }
        if (i>0) {
            can_transmit_mbox(MyCanId+1, mbox, i, databuf);
        }
    }
    return txdata;
}

void CAN_RxCpltCallback(unsigned int mbox)
{
    CAN_FIFOMailBox_TypeDef* mb = &SOC_CAN->sFIFOMailBox[mbox];
    uint32_t id = (mb->RIR >> CAN_RI0R_STID_Pos) & 0x7FF;
    uint8_t dlc = mb->RDTR & CAN_RDT0R_DLC;
    uint8_t databuf[8];

    if(!MyCanId) { // If serial not assigned yet
        if(id==PKT_ID_UUID && dlc == 0) {
            // Just inform host about my UUID
            can_uuid_resp();
        } else if (id == PKT_ID_SET) {
            uint8_t short_uuid[SHORT_UUID_LEN];
            pack_uuid(short_uuid);

            // compare my UUID with packet to check if this packet mine
            get_rx_data(databuf, mbox);
            if (memcmp(&(databuf[2]), short_uuid, SHORT_UUID_LEN) == 0) {
                memcpy(&MyCanId, databuf, sizeof(uint16_t));
                /* Set new filter values */
                uint32_t filternbrbitpos = (1U) << CAN_FILTER_NUMBER;
                SOC_CAN->FA1R &= ~(filternbrbitpos);
                /* Personal ID */
                SOC_CAN->sFilterRegister[CAN_FILTER_NUMBER].FR1 =
                        ((uint32_t)(MyCanId<<5) << 16U);
                /* Catch reset command */
                SOC_CAN->sFilterRegister[CAN_FILTER_NUMBER].FR2 =
                        ((uint32_t)(PKT_ID_UUID<<5) << 16U);
                /* Filter activation */
                SOC_CAN->FA1R |= filternbrbitpos;
                /* Leave the initialisation mode for the filter */
                SOC_CAN->FMR &= ~(CAN_FMR_FINIT);
            }
        }
    }  else {
        if (id == MyCanId) {
            // compare my UUID with packet to check if this packet mine
            if(dlc == 0) {
                // empty packet == ping request
                can_transmit(MyCanId+1, 0, NULL);
            } else {
                get_rx_data(databuf, mbox);
                for(int i=0; i < dlc; i++ ) {
                    serial_rx_byte(databuf[i]);
                }
            }
        }
        else if (id == PKT_ID_UUID && dlc > 0)
        {
            get_rx_data(databuf, mbox);
            if (memcmp(databuf, &MyCanId, 2) == 0)
            {
                // Reset from host
                NVIC_SystemReset();
            }
        }
    }
}

/**
  * @brief This function handles CAN global interrupts
  */
void
CAN_IRQHandler(void)
{
    // RX
    if (SOC_CAN->RF0R & CAN_RF0R_FMP0) {
        // Mailbox 0
        while(SOC_CAN->RF0R & CAN_RF0R_FMP0) {
            CAN_RxCpltCallback(0);
            SOC_CAN->RF0R |= CAN_RF0R_RFOM0;
        }
    }
    if (SOC_CAN->RF1R & CAN_RF1R_FMP1) {
        // Mailbox 1
        while(SOC_CAN->RF1R & CAN_RF1R_FMP1) {
            CAN_RxCpltCallback(1);
            SOC_CAN->RF1R |= CAN_RF1R_RFOM1;
        }
    }

    /* Check Overrun flag for FIFO0 */
    if(SOC_CAN->RF0R & CAN_RF0R_FOVR0)
    {
        /* Clear FIFO0 Overrun Flag */
        SOC_CAN->RF0R |= CAN_RF0R_FOVR0;
    }
    /* Check Overrun flag for FIFO1 */
    if(SOC_CAN->RF1R & CAN_RF1R_FOVR1)
    {
        /* Clear FIFO1 Overrun Flag */
        SOC_CAN->RF1R |= CAN_RF1R_FOVR1;
    }

    // TX
    if(SOC_CAN->IER & CAN_IER_TMEIE) {  // TX IRQ enabled
      if(!CAN_TxIrq())
          SOC_CAN->IER &= ~CAN_IER_TMEIE; // Disable TXIRQ
    }
}

static inline const uint32_t
make_btr(uint32_t sjw,       // Sync jump width, ... hmm
         uint32_t time_seg1, // time segment before sample point, 1 .. 16
         uint32_t time_seg2, // time segment after sample point, 1 .. 8
         uint32_t brp)       // Baud rate prescaler, 1 .. 1024
{
    return
        ((uint32_t)(sjw-1)) << CAN_BTR_SJW_Pos
        | ((uint32_t)(time_seg1-1)) << CAN_BTR_TS1_Pos
        | ((uint32_t)(time_seg2-1)) << CAN_BTR_TS2_Pos
        | ((uint32_t)(brp - 1)) << CAN_BTR_BRP_Pos;
}


static inline const uint32_t
compute_btr(uint32_t pclock, uint32_t bitrate) {

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
    for(qs = 18; qs > 9; qs --) {
        // check that bit_clocks / quantas is an integer
        uint32_t brp_rem = bit_clocks % qs;
        if(brp_rem == 0)
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

    uint32_t btr = compute_btr(pclock, CONFIG_SERIAL_BAUD);

    /*##-1- Configure the CAN #######################################*/

    /* Exit from sleep mode */
    SOC_CAN->MCR &= ~(CAN_MCR_SLEEP);
    /* Request initialisation */
    SOC_CAN->MCR |= CAN_MCR_INRQ;
    /* Wait the acknowledge */
    while( !(SOC_CAN->MSR & CAN_MSR_INAK) );

    SOC_CAN->MCR |= MCR_FLAGS;
    SOC_CAN->BTR = btr;

    /* Request leave initialisation */
    SOC_CAN->MCR &= ~(CAN_MCR_INRQ);
    /* Wait the acknowledge */
    while( SOC_CAN->MSR & CAN_MSR_INAK );

    /*##-2- Configure the CAN Filter #######################################*/
    uint32_t filternbrbitpos = (1U) << CAN_FILTER_NUMBER;

    /* Select the start slave bank */
    SOC_CAN->FMR |= CAN_FMR_FINIT;
    /* Initialisation mode for the filter */
    SOC_CAN->FA1R &= ~(filternbrbitpos);

    SOC_CAN->sFilterRegister[CAN_FILTER_NUMBER].FR1 =
            ((uint32_t)(PKT_ID_UUID<<5) << 16U);
    SOC_CAN->sFilterRegister[CAN_FILTER_NUMBER].FR2 =
            ((uint32_t)(PKT_ID_SET<<5) << 16U);

    /*Identifier list mode for the filter*/
    SOC_CAN->FM1R |= filternbrbitpos;
    /* 32-bit scale for the filter */
    SOC_CAN->FS1R |= filternbrbitpos;

    /* FIFO 0 assignation for the filter */
    SOC_CAN->FFA1R &= ~(filternbrbitpos);

    /* Filter activation */
    SOC_CAN->FA1R |= filternbrbitpos;
    /* Leave the initialisation mode for the filter */
    SOC_CAN->FMR &= ~(CAN_FMR_FINIT);

    /*##-3- Configure Interrupts #################################*/

    SOC_CAN->IER |= (CAN_IER_FMPIE0 | CAN_IER_FMPIE1); // RX mailbox IRQ

    armcm_enable_irq(CAN_IRQHandler, CAN_RX0_IRQn, 0);
    if(CAN_RX0_IRQn != CAN_RX1_IRQn)
      armcm_enable_irq(CAN_IRQHandler, CAN_RX1_IRQn, 0);
    if(CAN_RX0_IRQn != CAN_TX_IRQn)
      armcm_enable_irq(CAN_IRQHandler, CAN_TX_IRQn, 0);
    // TODO: CAN_SCE_IRQ?n


    /*##-4- Say Hello #################################*/
    can_uuid_resp();
}
DECL_INIT(can_init);

void
serial_enable_tx_irq(void)
{
    if(MyCanId == 0)
        // Serial port not initialized
        return;

    SOC_CAN->IER |= CAN_IER_TMEIE; // TX mailbox IRQ
}
