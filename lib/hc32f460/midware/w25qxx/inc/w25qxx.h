/*****************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file w25qxx.h
 **
 ** A detailed description is available at
 ** @link W25QXXGroup W25Q64 description @endlink
 **
 **   - 2019-05-15 CDT First version for Device Driver Library.
 **
 ******************************************************************************/
#ifndef __W25QXX_H__
#define __W25QXX_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
/**
 *******************************************************************************
 ** \defgroup W25QXXGroup W25Q64 SPI NOR Flash driver
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*****************************************************************************/
/* Global pre-processor symbols/macros ('define')                            */
/*****************************************************************************/
/* W25QXX Flash ID */
#define W25Q80  0XEF13u
#define W25Q16  0XEF14u
#define W25Q32  0XEF15u
#define W25Q64  0XEF16u
#define W25Q128 0XEF17u

/* W25QXX Command list */
#define W25X_WriteEnable        0x06u
#define W25X_WriteDisable       0x04u
#define W25X_ReadStatusReg      0x05u
#define W25X_WriteStatusReg     0x01u
#define W25X_ReadData           0x03u
#define W25X_FastReadData       0x0Bu
#define W25X_FastReadDual       0x3Bu
#define W25X_PageProgram        0x02u
#define W25X_BlockErase         0xD8u
#define W25X_SectorErase        0x20u
#define W25X_ChipErase          0xC7u
#define W25X_PowerDown          0xB9u
#define W25X_ReleasePowerDown   0xABu
#define W25X_DeviceID           0xABu
#define W25X_ManufactDeviceID   0x90u
#define W25X_JedecDeviceID      0x9Fu

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern uint16_t W25QXX_TYPE;

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
void W25QXX_Init(void);
uint16_t  W25QXX_ReadID(void);
uint8_t   W25QXX_ReadSR(void);
void W25QXX_Write_SR(uint8_t sr);
void W25QXX_Write_Enable(void);
void W25QXX_Write_Disable(void);
void W25QXX_Write_NoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void W25QXX_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void W25QXX_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void W25QXX_Erase_Chip(void);
void W25QXX_Erase_Sector(uint32_t Dst_Addr);
void W25QXX_Wait_Busy(void);
void W25QXX_PowerDown(void);
void W25QXX_WAKEUP(void);

//@} // W25QXXGroup

#ifdef __cplusplus
}
#endif

#endif /* __W25QXX_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
