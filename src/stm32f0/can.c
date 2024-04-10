/*
 * Serial over CAN emulation for STM32F042 boards.
 *
 *  Copyright (C) 2019 Eug Krashtan <eug.krashtan@gmail.com>
 *  This file may be distributed under the terms of the GNU GPLv3 license.
 *
 */

#include "stm32f0xx_hal.h"
#include <string.h>
#include "can.h"
#include "command.h" // encoder
#include "board/serial_irq.h" // serial_enable_tx_irq
#include "sched.h" // DECL_INIT
#include "log.h"

CAN_HandleTypeDef hcan;
CanTxMsgTypeDef    TxMessage;
CanRxMsgTypeDef    RxMessage;
static uint16_t MyCanId = 0;


void CanTransmit(uint32_t id, uint32_t dlc, uint8_t *pkt)
{
    memcpy(hcan.pTxMsg->Data, pkt, dlc);
    hcan.pTxMsg->StdId = id;
    hcan.pTxMsg->DLC = dlc;
    HAL_CAN_Transmit_IT(&hcan);
}

// Convert Unique 96-bit value into 48 bit representation
static void PackUuid(uint8_t* u)
{
    for(int i=0; i<SHORT_UUID_LEN; i++) {
        u[i] = *((uint8_t*)(STM32_UUID_ADDR+i)) ^
                *((uint8_t*)(STM32_UUID_ADDR+i+SHORT_UUID_LEN));
    }
}

static void CanUUIDResp(void)
{
    uint8_t short_uuid[SHORT_UUID_LEN];
    PackUuid(short_uuid);
    CanTransmit(PKT_ID_UUID_RESP, SHORT_UUID_LEN, short_uuid);
}

void CanInit(void)
{
    // Master reset
    //SET_BIT(CAN->MCR, CAN_MCR_RESET);

    hcan.Instance = CAN;
    hcan.Init.Prescaler = 12;
    hcan.Init.Mode = CAN_MODE_NORMAL;
    hcan.Init.SJW = CAN_SJW_1TQ;
    hcan.Init.BS1 = CAN_BS1_5TQ;
    hcan.Init.BS2 = CAN_BS2_2TQ;
    hcan.Init.TTCM = DISABLE;
    hcan.Init.ABOM = DISABLE;
    hcan.Init.AWUM = DISABLE;
    hcan.Init.NART = ENABLE;
    hcan.Init.RFLM = DISABLE;
    hcan.Init.TXFP = DISABLE;
    HAL_CAN_Init(&hcan);
    hcan.pTxMsg = &TxMessage;
    hcan.pRxMsg = &RxMessage;

    /*##-2- Configure the CAN Filter #######################################*/
    CAN_FilterConfTypeDef sFilterConfig;
    sFilterConfig.FilterNumber = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = PKT_ID_UUID<<5;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = PKT_ID_SET<<5;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);

    /*##-3- Configure Transmission process #################################*/
    hcan.pTxMsg->ExtId = 0;
    HAL_CAN_Receive_IT(&hcan, CAN_FIFO0);

    /* Request for port */
    CanUUIDResp();
}
DECL_INIT(CanInit);

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* h) {
    int i;

    if(!MyCanId) { // If serial not assigned yet
        if(h->pRxMsg->StdId == PKT_ID_UUID && h->pRxMsg->DLC == 0) {
            // Just inform host about my UUID
            CanUUIDResp();
        } else if (h->pRxMsg->StdId == PKT_ID_SET) {
            // compare my UUID with packet to check if this packet mine
            uint8_t short_uuid[SHORT_UUID_LEN];
            PackUuid(short_uuid);
            if (memcmp(&(h->pRxMsg->Data[2]), short_uuid,
                    SHORT_UUID_LEN) == 0) {
                memcpy(&MyCanId, h->pRxMsg->Data, sizeof(uint16_t));
                CAN_FilterConfTypeDef sFilterConfig;
                sFilterConfig.FilterNumber = 0;
                sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
                sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
                // Personal ID
                sFilterConfig.FilterIdHigh = MyCanId<<5;
                sFilterConfig.FilterIdLow = 0x0000;
                // Catch reset command
                sFilterConfig.FilterMaskIdHigh = PKT_ID_UUID<<5;
                sFilterConfig.FilterMaskIdLow = 0x0000;
                sFilterConfig.FilterFIFOAssignment = CAN_FIFO0;
                sFilterConfig.FilterActivation = ENABLE;
                // Disable 'set address' filter and enable only my packets
                HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);
            }
        }
    } else {
        if(h->pRxMsg->StdId == MyCanId) {
            if(h->pRxMsg->DLC == 0) {
                // empty packet == ping request
                hcan.pTxMsg->StdId = MyCanId+1;
                hcan.pTxMsg->DLC = 0;
                HAL_CAN_Transmit_IT(&hcan);
            } else {
                for(i=0; i < h->pRxMsg->DLC; i++ ) {
                    serial_rx_byte(h->pRxMsg->Data[i]);
                }
            }
        } else if (h->pRxMsg->StdId == PKT_ID_UUID && h->pRxMsg->DLC > 0) {
            if (memcmp(&(h->pRxMsg->Data), &MyCanId, 2) == 0) {
                // Reset from host
                NVIC_SystemReset();
            }
        }
    }
    HAL_CAN_Receive_IT(&hcan, CAN_FIFO0);
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *h)
{
    // Overrun handling - drop damaged packet
    HAL_CAN_Receive_IT(&hcan, CAN_FIFO0);
}

/**
  * @brief This function handles HDMI-CEC and CAN global interrupts /
  *  HDMI-CEC wake-up interrupt through EXTI line 27.
  */
void CEC_CAN_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&hcan);
}

void
serial_enable_tx_irq(void)
{
    if(MyCanId == 0)
        // Serial port not initialized
        return;

    int i=0;
    for (;i<8;)
    {
        if(serial_get_tx_byte(&(hcan.pTxMsg->Data[i])) == -1) {
            break;
        }
        i++;
    }
    if (i>0) {
        hcan.pTxMsg->StdId = MyCanId+1;
        hcan.pTxMsg->DLC = i;
        HAL_CAN_Transmit_IT(&hcan);
    }
}

void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef* hcan)
{
    // if we still have some data to transmit
    serial_enable_tx_irq();
}

/*
 * @brief CAN MSP Initialization
 * @param h: CAN handle pointer
 * @retval None
 */
void HAL_CAN_MspInit(CAN_HandleTypeDef* h)
{

    __HAL_REMAP_PIN_ENABLE(HAL_REMAP_PA11_PA12);

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(h->Instance==CAN)
    {
        /* Peripheral clock enable */
        __HAL_RCC_CAN1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**CAN GPIO Configuration
        PA11     ------> CAN_RX
        PA12     ------> CAN_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_CAN;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* CAN interrupt Init */
        HAL_NVIC_SetPriority(CEC_CAN_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CEC_CAN_IRQn);
    }
}
