/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file w25qxx.c
 **
 ** A detailed description is available at
 ** @link W25QXXGroup W25Q64 description @endlink
 **
 **   - 2019-05-15 CDT First version for Device Driver Library of W25QXX.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"
#include "w25qxx.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* QSPCK Port/Pin definition */
#define QSPCK_PORT                      (PortC)
#define QSPCK_PIN                       (Pin06)

/* QSNSS Port/Pin definition */
#define QSNSS_PORT                      (PortC)
#define QSNSS_PIN                       (Pin07)

/* QSIO0 Port/Pin definition */
#define QSIO0_PORT                      (PortD)
#define QSIO0_PIN                       (Pin08)

/* QSIO1 Port/Pin definition */
#define QSIO1_PORT                      (PortD)
#define QSIO1_PIN                       (Pin09)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint16_t W25QXX_TYPE = W25Q64;
uint8_t W25QXX_BUFFER[4096];

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief W25QXX IO initialize
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void W25QXX_Init(void)
{
    stc_qspi_init_t stcQspiInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcQspiInit);

    /* Configuration peripheral clock */
    PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_QSPI, Enable);

    /* Configuration QSPI pin */
    PORT_SetFunc(QSPCK_PORT, QSPCK_PIN, Func_Qspi, Disable);
    PORT_SetFunc(QSNSS_PORT, QSNSS_PIN, Func_Qspi, Disable);
    PORT_SetFunc(QSIO0_PORT, QSIO0_PIN, Func_Qspi, Disable);
    PORT_SetFunc(QSIO1_PORT, QSIO1_PIN, Func_Qspi, Disable);

    /* Configuration QSPI structure */
    stcQspiInit.enClkDiv = QspiHclkDiv2;
    stcQspiInit.enSpiMode = QspiSpiMode0;
    stcQspiInit.enBusCommMode = QspiBusModeRomAccess;
    stcQspiInit.enPrefetchMode = QspiPrefetchStopComplete;
    stcQspiInit.enPrefetchFuncEn = Disable;
    stcQspiInit.enQssnValidExtendTime = QspiQssnValidExtendNot;
    stcQspiInit.enQssnIntervalTime = QspiQssnIntervalQsck8;
    stcQspiInit.enQsckDutyCorr = QspiQsckDutyCorrNot;
    stcQspiInit.enVirtualPeriod = QspiVirtualPeriodQsck8;
    stcQspiInit.enWpPinLevel = QspiWpPinOutputLow;
    stcQspiInit.enQssnSetupDelayTime = QspiQssnSetupDelayHalfQsck;
    stcQspiInit.enQssnHoldDelayTime = QspiQssnHoldDelayHalfQsck;
    stcQspiInit.enFourByteAddrReadEn = Disable;
    stcQspiInit.enAddrWidth = QspiAddressByteThree;
    stcQspiInit.stcCommProtocol.enReadMode = QspiReadModeStandard;
    stcQspiInit.stcCommProtocol.enTransInstrProtocol = QspiProtocolExtendSpi;
    stcQspiInit.stcCommProtocol.enTransAddrProtocol = QspiProtocolExtendSpi;
    stcQspiInit.stcCommProtocol.enReceProtocol = QspiProtocolExtendSpi;
    stcQspiInit.u8RomAccessInstr = QSPI_3BINSTR_STANDARD_READ;
    QSPI_Init(&stcQspiInit);
    /* Read Flash ID */
    W25QXX_TYPE = W25QXX_ReadID();
}

/**
 *******************************************************************************
 ** \brief W25QXX Read Status Register
 **
 ** \param  None
 **
 ** \retval SR value
 **
 ******************************************************************************/
uint8_t W25QXX_ReadSR(void)
{
    uint8_t regSta = 0u;

    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(W25X_ReadStatusReg);
    regSta = QSPI_ReadDirectCommValue();
    QSPI_ExitDirectCommMode();

    return regSta;
}

/**
 *******************************************************************************
 ** \brief W25QXX Write Status Register
 **
 ** \param [in] sr
 **
 ** \retval None
 **
 ******************************************************************************/
void W25QXX_Write_SR(uint8_t sr)
{
    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(W25X_WriteStatusReg);
    QSPI_WriteDirectCommValue(sr);
    QSPI_ExitDirectCommMode();
}

/**
 *******************************************************************************
 ** \brief W25QXX Write enable
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void W25QXX_Write_Enable(void)
{
    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(W25X_WriteEnable);
    QSPI_ExitDirectCommMode();
}

/**
 *******************************************************************************
 ** \brief W25QXX Write disable
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void W25QXX_Write_Disable(void)
{
    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(W25X_WriteDisable);
    QSPI_ExitDirectCommMode();
}

/**
 *******************************************************************************
 ** \brief W25QXX read flash ID
 **
 ** \param  None
 **
 ** \retval u16FlashID
 **  0XEF13: W25Q80
 **  0XEF14: W25Q16
 **  0XEF15: W25Q32
 **  0XEF16: W25Q64
 **  0XEF17: W25Q128
 **
 ******************************************************************************/
uint16_t W25QXX_ReadID(void)
{
    uint16_t u16FlashID = 0u;

    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(W25X_ManufactDeviceID);
    QSPI_WriteDirectCommValue(0x00u);
    QSPI_WriteDirectCommValue(0x00u);
    QSPI_WriteDirectCommValue(0x00u);
    u16FlashID |= (uint16_t)((uint16_t)QSPI_ReadDirectCommValue() << (int8_t)8);
    u16FlashID |= QSPI_ReadDirectCommValue();
    QSPI_ExitDirectCommMode();
    return u16FlashID;
}

/**
 *******************************************************************************
 ** \brief W25QXX read flash content
 **
 ** \param [in] ReadAddr                Address to be read
 ** \param [in] NumByteToRead           Number to be read, (MAX. 65535)
 ** \param [out] pBuffer                Read data buffer
 **
 ** \retval None
 **
 ******************************************************************************/
void W25QXX_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    uint16_t i;

    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(W25X_ReadData);
    QSPI_WriteDirectCommValue((uint8_t)((ReadAddr) >> 16u));
    QSPI_WriteDirectCommValue((uint8_t)((ReadAddr) >> 8u));
    QSPI_WriteDirectCommValue((uint8_t)ReadAddr);
    for (i = 0u; i < NumByteToRead; i++)
    {
        pBuffer[i] = QSPI_ReadDirectCommValue();
    }
    QSPI_ExitDirectCommMode();
}

/**
 *******************************************************************************
 ** \brief W25QXX page write
 **
 ** \param [in] pBuffer                 data buffer to be written
 ** \param [in] WriteAddr               Address to be written
 ** \param [in] NumByteToWrite          Number to be written, (MAX. 256)
 **
 ** \retval None
 **
 ******************************************************************************/
void W25QXX_Write_Page(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    W25QXX_Write_Enable();
    /* Send data to flash */
    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(W25X_PageProgram);
    QSPI_WriteDirectCommValue((uint8_t)((WriteAddr & 0xFF0000ul) >> 16u));
    QSPI_WriteDirectCommValue((uint8_t)((WriteAddr & 0xFF00u) >> 8u));
    QSPI_WriteDirectCommValue((uint8_t)(WriteAddr & 0xFFu));
    while (NumByteToWrite--)
    {
        QSPI_WriteDirectCommValue(*pBuffer++);
    }
    QSPI_ExitDirectCommMode();
    /* Wait for flash idle */
    W25QXX_Wait_Busy();
}

/**
 *******************************************************************************
 ** \brief W25QXX flash write
 **
 ** \param [in] pBuffer                 data buffer to be written
 ** \param [in] WriteAddr               Address to be written
 ** \param [in] NumByteToWrite          Number to be written, (MAX. 65535)
 **
 ** \retval None
 **
 ******************************************************************************/
void W25QXX_Write_NoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t pageremain;
    pageremain = (uint16_t)(256u - WriteAddr % 256u);
    if (NumByteToWrite <= pageremain)
    {
        pageremain = NumByteToWrite;
    }
    while (1)
    {
        W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
        if (NumByteToWrite == pageremain)
        {
            break;
        }
        else //NumByteToWrite>pageremain
        {
            pBuffer        += pageremain;
            WriteAddr      += pageremain;

            NumByteToWrite -= pageremain;
            if (NumByteToWrite > 256u)
            {
                pageremain = 256u;
            }
            else
            {
                pageremain = NumByteToWrite;
            }
        }
    }
}

/**
 *******************************************************************************
 ** \brief W25QXX flash write API for MSC
 **
 ** \param [in] pBuffer                 data buffer to be written
 ** \param [in] WriteAddr               Address to be written
 ** \param [in] NumByteToWrite          Number to be written, (MAX. 65535)
 **
 ** \retval None
 **
 ******************************************************************************/
void W25QXX_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t * W25QXX_BUF;
    W25QXX_BUF = W25QXX_BUFFER;
    secpos     = WriteAddr / 4096u;
    secoff     = (uint16_t)(WriteAddr % 4096u);
    secremain  = 4096u - secoff;
//    DDL_Printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);           // for test
    if (NumByteToWrite <= secremain)
    {
        secremain = NumByteToWrite;                                // less than 4K
    }
    while (1)
    {
        W25QXX_Read(W25QXX_BUF, secpos * 4096u, 4096u);              // read one sector content
        for (i = 0u; i < secremain; i++)                            // check if blank sector
        {
            if (W25QXX_BUF[secoff + i] != (uint8_t)0XFF)
            {
                break;
            }
        }
        if (i < secremain)
        {
            W25QXX_Erase_Sector(secpos);                           // not blank, need erase
            for (i = 0u; i < secremain; i++)                        // backup first
            {
                W25QXX_BUF[i + secoff] = pBuffer[i];
            }
            W25QXX_Write_NoCheck(W25QXX_BUF, secpos * 4096u, 4096u); // write back after erase

        }
        else
        {
            W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain);
        }
        if (NumByteToWrite == secremain)
        {
            break;
        }
        else
        {
            secpos++;                                              // next sector
            secoff          = 0u;

            pBuffer        += secremain;
            WriteAddr      += secremain;
            NumByteToWrite -= secremain;
            if (NumByteToWrite > 4096u)
            {
                secremain = 4096u;
            }
            else
            {
                secremain = NumByteToWrite;
            }
        }
    }
    ;
}

/**
 *******************************************************************************
 ** \brief W25QXX flash chip erase
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void W25QXX_Erase_Chip(void)
{
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();
    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(W25X_ChipErase);
    QSPI_ExitDirectCommMode();
    W25QXX_Wait_Busy();
}

/**
 *******************************************************************************
 ** \brief W25QXX flash sector erase
 **
 ** \param [in] Dst_Addr address
 **
 ** \retval None
 **
 ******************************************************************************/
void W25QXX_Erase_Sector(uint32_t Dst_Addr)
{
    Dst_Addr *= 4096u;
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();
    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(W25X_SectorErase);
    QSPI_WriteDirectCommValue((uint8_t)((Dst_Addr) >> 16u));
    QSPI_WriteDirectCommValue((uint8_t)((Dst_Addr) >> 8u));
    QSPI_WriteDirectCommValue((uint8_t)Dst_Addr);
    QSPI_ExitDirectCommMode();
    W25QXX_Wait_Busy();
}

/**
 *******************************************************************************
 ** \brief W25QXX flash wait SR.BUSY
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void W25QXX_Wait_Busy(void)
{
    while ((W25QXX_ReadSR() & 0x01u) == 0x01u)            // Wait SR.BUSY = 0
    {
        ;
    }
}

/**
 *******************************************************************************
 ** \brief W25QXX enter power down mode
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void W25QXX_PowerDown(void)
{
    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(W25X_PowerDown);
    QSPI_ExitDirectCommMode();
    Ddl_Delay1ms(1ul);
}

/**
 *******************************************************************************
 ** \brief W25QXX wake up
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void W25QXX_WAKEUP(void)
{
    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(W25X_ReleasePowerDown);
    QSPI_ExitDirectCommMode();
    Ddl_Delay1ms(1ul);
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
