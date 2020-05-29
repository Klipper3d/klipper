/*
 * Serial over CAN emulation for STM32F042 boards.
 *
 *  Copyright (C) 2019 Eug Krashtan <eug.krashtan@gmail.com>
 *  Copyright (C) 2020 Pontus Borg <glpontus@gmail.com>
 *  This file may be distributed under the terms of the GNU GPLv3 license.
 *
 */

#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "board/armcm_boot.h" // armcm_enable_irq
#include "board/serial_irq.h" // serial_rx_byte
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // enable_pclock
#include "sched.h" // DECL_INIT
#include <string.h>
#include "stm32f0_can.h"

DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PA11,PA12");
#define GPIO_Rx GPIO('A', 11)
#define GPIO_Tx GPIO('A', 12)

// TXFP makes packets posted to the TX mboxes transmit in chronologcal order
#define MCR_FLAGS (CAN_MCR_TXFP)

// TODO: Make portable to other STM32's.
// TODO: Use SERIAL_BAUD

/* BTR Register:
    BRP = baudrate prescaler;  (6 bits)
    SJW = synchronization jump width,  (2 bits)
    TS1 = time segment before sample point,  (4 bits)
    TS2 = time segment after sample point, (3 bits)
  Values below gives 500kbits/s on a 48Mhz clock
*/
#define BTR_FLAGS (  CAN_BTR_TS1_2 | CAN_BTR_TS2_0 | /* prescaler */ 11U )
#define CAN_FILTER_NUMBER 0

static uint16_t MyCanId = 0;

static int can_find_empty_tx_mbox(void) {
    uint32_t tsr = CAN->TSR;
    if(tsr & CAN_TSR_TME0) return 0;
    if(tsr & CAN_TSR_TME1) return 1;
    if(tsr & CAN_TSR_TME2) return 2;
    return -1;
}

static void can_transmit_mbox(uint32_t id, int mbox, uint32_t dlc, uint8_t *pkt)
{
     CAN_TxMailBox_TypeDef *mb = &CAN->sTxMailBox[mbox];
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
    for(int i=0; i<SHORT_UUID_LEN; i++) {
        u[i] = *((uint8_t*)(UID_BASE+i)) ^
                *((uint8_t*)(UID_BASE+i+SHORT_UUID_LEN));
    }
}

static void can_uuid_resp(void)
{
    uint8_t short_uuid[SHORT_UUID_LEN];
    pack_uuid(short_uuid);
    can_transmit(PKT_ID_UUID_RESP, SHORT_UUID_LEN, short_uuid);
}

static void get_rx_data(uint8_t* buf, unsigned int mbox)
{
    uint32_t rdlr = CAN->sFIFOMailBox[mbox].RDLR;
    buf[0] = (rdlr >>  0) & 0xff;
    buf[1] = (rdlr >>  8) & 0xff;
    buf[2] = (rdlr >> 16) & 0xff;
    buf[3] = (rdlr >> 24) & 0xff;
    uint32_t rdhr = CAN->sFIFOMailBox[mbox].RDHR;
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
    uint32_t id = (CAN->sFIFOMailBox[mbox].RIR >> CAN_RI0R_STID_Pos) & 0x7FF;
    uint8_t dlc = CAN->sFIFOMailBox[mbox].RDTR & CAN_RDT0R_DLC;
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
                CAN->FA1R &= ~(filternbrbitpos);
                /* Personal ID */
                CAN->sFilterRegister[CAN_FILTER_NUMBER].FR1 =
                        ((uint32_t)(MyCanId<<5) << 16U);
                /* Catch reset command */
                CAN->sFilterRegister[CAN_FILTER_NUMBER].FR2 =
                        ((uint32_t)(PKT_ID_UUID<<5) << 16U);
                /* Filter activation */
                CAN->FA1R |= filternbrbitpos;
                /* Leave the initialisation mode for the filter */
                CAN->FMR &= ~(CAN_FMR_FINIT);
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
  * @brief This function handles HDMI-CEC and CAN global interrupts /
  *  HDMI-CEC wake-up interrupt through EXTI line 27.
  */
void
CEC_CAN_IRQHandler(void)
{
    // RX
    if (CAN->RF0R & CAN_RF0R_FMP0) {
        // Mailbox 0
        while(CAN->RF0R & CAN_RF0R_FMP0) {
            CAN_RxCpltCallback(0);
            CAN->RF0R |= CAN_RF0R_RFOM0;
        }
    }
    if (CAN->RF1R & CAN_RF1R_FMP1) {
        // Mailbox 1
        while(CAN->RF1R & CAN_RF1R_FMP1) {
            CAN_RxCpltCallback(1);
            CAN->RF1R |= CAN_RF1R_RFOM1;
        }
    }

    /* Check Overrun flag for FIFO0 */
    if(CAN->RF0R & CAN_RF0R_FOVR0)
    {
        /* Clear FIFO0 Overrun Flag */
        CAN->RF0R |= CAN_RF0R_FOVR0;
    }
    /* Check Overrun flag for FIFO1 */
    if(CAN->RF1R & CAN_RF1R_FOVR1)
    {
        /* Clear FIFO1 Overrun Flag */
        CAN->RF1R |= CAN_RF1R_FOVR1;
    }

    // TX
    if(CAN->IER & CAN_IER_TMEIE) {  // TX IRQ enabled
      if(!CAN_TxIrq())
          CAN->IER &= ~CAN_IER_TMEIE; // Disable TXIRQ
    }
}

void
can_init(void)
{
    enable_pclock((uint32_t)CAN);
    gpio_peripheral(GPIO_Rx, GPIO_FUNCTION(4), 1);
    gpio_peripheral(GPIO_Tx, GPIO_FUNCTION(4), 0);

    /*##-1- Configure the CAN #######################################*/

    /* Exit from sleep mode */
    CAN->MCR &= ~(CAN_MCR_SLEEP);
    /* Request initialisation */
    CAN->MCR |= CAN_MCR_INRQ;
    /* Wait the acknowledge */
    while( !(CAN->MSR & CAN_MSR_INAK) );

    CAN->MCR |= MCR_FLAGS;
    CAN->BTR = BTR_FLAGS;

    /* Request leave initialisation */
    CAN->MCR &= ~(CAN_MCR_INRQ);
    /* Wait the acknowledge */
    while( CAN->MSR & CAN_MSR_INAK );

    /*##-2- Configure the CAN Filter #######################################*/
    uint32_t filternbrbitpos = (1U) << CAN_FILTER_NUMBER;

    /* Select the start slave bank */
    CAN->FMR |= CAN_FMR_FINIT;
    /* Initialisation mode for the filter */
    CAN->FA1R &= ~(filternbrbitpos);

    CAN->sFilterRegister[CAN_FILTER_NUMBER].FR1 =
            ((uint32_t)(PKT_ID_UUID<<5) << 16U);
    CAN->sFilterRegister[CAN_FILTER_NUMBER].FR2 =
            ((uint32_t)(PKT_ID_SET<<5) << 16U);

    /*Identifier list mode for the filter*/
    CAN->FM1R |= filternbrbitpos;
    /* 32-bit scale for the filter */
    CAN->FS1R |= filternbrbitpos;

    /* FIFO 0 assignation for the filter */
    CAN->FFA1R &= ~(filternbrbitpos);

    /* Filter activation */
    CAN->FA1R |= filternbrbitpos;
    /* Leave the initialisation mode for the filter */
    CAN->FMR &= ~(CAN_FMR_FINIT);

    /*##-3- Configure Interrupts #################################*/

    CAN->IER |= (CAN_IER_FMPIE0 | CAN_IER_FMPIE1); // RX mailbox IRQ

    armcm_enable_irq(CEC_CAN_IRQHandler, CEC_CAN_IRQn, 0);

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

    CAN->IER |= CAN_IER_TMEIE; // TX mailbox IRQ
}
