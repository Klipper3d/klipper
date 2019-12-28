/*
 * Serial over CAN emulation for STM32F042 boards.
 *
 *  Copyright (C) 2019 Eug Krashtan <eug.krashtan@gmail.com>
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
#define MCR_FLAGS (CAN_MCR_NART)
#define BTR_FLAGS (CAN_BTR_TS1_2 | CAN_BTR_TS2_0 | /* prescaler */ 11U )
#define CAN_FILTER_NUMBER 0
#define CAN_DATA3_Pos 24
#define CAN_DATA2_Pos 16
#define CAN_DATA1_Pos 8
#define CAN_DATA0_Pos 0

static uint16_t MyCanId = 0;

static void can_transmit(uint32_t id, uint32_t dlc, uint8_t *pkt)
{
    /* ToDo: implement serial_get_tx_byte() inside TXComplete interrupt */

    /* Use only Mailbox 0 to avoid message fragmentation */
    while( !(CAN->TSR & CAN_TSR_TME0) ) /* wait until previous TX completes */
        ;

    /* Set up the Id */
    CAN->sTxMailBox[0].TIR &= CAN_TI1R_TXRQ;
    CAN->sTxMailBox[0].TIR |= (id << CAN_TI1R_STID_Pos);

    /* Set up the DLC */
    CAN->sTxMailBox[0].TDTR &= 0xFFFFFFF0U;
    CAN->sTxMailBox[0].TDTR |= (dlc & 0xFU);

    /* Set up the data field */
    if(pkt) {
        CAN->sTxMailBox[0].TDLR = ((uint32_t)pkt[3] << CAN_DATA3_Pos) |
                        ((uint32_t)pkt[2] << CAN_DATA2_Pos) |
                        ((uint32_t)pkt[1] << CAN_DATA1_Pos) |
                        ((uint32_t)pkt[0] << CAN_DATA0_Pos);
        CAN->sTxMailBox[0].TDHR = ((uint32_t)pkt[7] << CAN_DATA3_Pos) |
                        ((uint32_t)pkt[6] << CAN_DATA2_Pos) |
                        ((uint32_t)pkt[5] << CAN_DATA1_Pos) |
                        ((uint32_t)pkt[4] << CAN_DATA0_Pos);
    }

     /* Request transmission */
    __sync_synchronize(); // disable write optimization
    CAN->sTxMailBox[0].TIR |= CAN_TI1R_TXRQ;
}

// Convert Unique 96-bit value into 48 bit representation
static void pack_uuid(uint8_t* u)
{
    for(int i=0; i<SHORT_UUID_LEN; i++) {
        u[i] = *((uint8_t*)(STM32_UUID_ADDR+i)) ^
                *((uint8_t*)(STM32_UUID_ADDR+i+SHORT_UUID_LEN));
    }
}

static void can_uuid_resp(void)
{
    uint8_t short_uuid[SHORT_UUID_LEN];
    pack_uuid(short_uuid);
    can_transmit(PKT_ID_UUID_RESP, SHORT_UUID_LEN, short_uuid);
}

static void get_data(uint8_t* buf, uint8_t mbox)
{
    for(int i=0; i < 8; i++ ) {
        if (i<4) {
            buf[i] = (CAN->sFIFOMailBox[mbox].RDLR >> (i*8)) & 0xFF;
        } else {
            buf[i] = (CAN->sFIFOMailBox[mbox].RDHR >> ((i-4)*8))& 0xFF;
        }
    }
}

void CAN_RxCpltCallback(uint8_t mbox)
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
            get_data(databuf, mbox);
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
                get_data(databuf, mbox);
                for(int i=0; i < dlc; i++ ) {
                    serial_rx_byte(databuf[i]);
                }
            }
        }
        else if (id == PKT_ID_UUID && dlc > 0)
        {
            get_data(databuf, mbox);
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

    /*##-3- Configure Transmission process #################################*/

    CAN->IER |= (CAN_IER_FMPIE0 | CAN_IER_FMPIE1);
    armcm_enable_irq(CEC_CAN_IRQHandler, CEC_CAN_IRQn, 0);

    /*##-4- Say Hello #################################*/
    can_uuid_resp();
}
DECL_INIT(can_init);

void
serial_enable_tx_irq(void)
{
    uint8_t databuf[8];
    if(MyCanId == 0)
        // Serial port not initialized
        return;
    uint8_t txdata =1;
    while(txdata) {
        int i=0;
        for (;i<8;)
        {
            if(serial_get_tx_byte(&(databuf[i])) == -1) {
                txdata = 0;
                break;
            }
            i++;
        }
        if (i>0) {
            can_transmit(MyCanId+1, i, databuf);
        }
    }
}
