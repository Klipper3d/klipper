/*****************************************************************************
 * Copyright (c) 2026, Nsing Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
*\*\file system_n32h7xx.c
*\*\author Nsing
*\*\version v1.0.0
*\*\copyright Copyright (c) 2026, Nsing Technologies Inc. All rights reserved.
 */

#include "n32h7xx.h"

#if !defined  (HSE_VALUE)
    #define HSE_VALUE             ((uint32_t)25000000UL) /* Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (CSI_VALUE)
    #define CSI_VALUE           ((uint32_t)4000000UL) /* Value of the Internal oscillator in Hz*/
#endif /* CSI_VALUE */

#if !defined  (HSI_VALUE)
    #define HSI_VALUE           ((uint32_t)64000000UL) /* Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

#define VECT_TAB_OFFSET       ((uint32_t)0x00000000UL) /* Vector Table base offset field.   */

#define FLASH_BANK1_BASE      (0x15000000U)
#define FLASH_BANK2_BASE      (0x15080000U)
#define AHBSRAM_ENDADDR       (0x30057FFFU)

#ifndef N32H7_SYSTEM_CLOCK_HZ
#define N32H7_SYSTEM_CLOCK_HZ 600000000U
#endif

uint32_t SystemCoreClock = N32H7_SYSTEM_CLOCK_HZ;

#ifndef TCM_SIZE_VALUE
#define TCM_SIZE_VALUE        (0x02) /*TCM_SIZE=0x02:768K ITCM;256K DTCM;128K AXI_SRAM2/3*/
#endif

/** Private_Functions  */

#define IS_RDPLEVEL_L1()      ((*(uint32_t (*)(void))0x1ff00011U)() == 1U)

/* defaule power supply is extern LDO. User can change the way of power supply*/
//#define PWR_SUPPLY_SELECTION       (PWR_LDO_SUPPLY)  /* External LDO Supply  */
#define PWR_SUPPLY_SELECTION       (PWR_DIRECT_SMPS_SUPPLY)  /* DCDC Supply  */
//#define PWR_SUPPLY_SELECTION       (PWR_EXTERNAL_SOURCE_SUPPLY)  /* VCAP Supply  */


#define READ_TCM_SIZE         (0x1ff00f01U)
#define OTP_TCM_SIZE_REG      (0x51105280U)

#define ITCM_SIZE(n) (  (n) <= 0x08U ? 8U - (n)          : \
                        (n) <= 0x10U ? 7U - ((n) - 9U)   : \
                        (n) <= 0x17U ? 6U - ((n) - 17U)  : \
                        (n) <= 0x1DU ? 5U - ((n) - 24U)  : \
                        (n) <= 0x22U ? 4U - ((n) - 30U)  : \
                        (n) <= 0x26U ? 3U - ((n) - 35U)  : \
                        (n) <= 0x29U ? 2U - ((n) - 39U)  : \
                        (n) <= 0x2BU ? 1U - ((n) - 42U)  : 0U \
)


/**
  \brief   System Reset
  \details Initiates a system reset request to reset the MCU.
 */
#if defined(__GNUC__)
    void  __attribute__((noinline, section(".text.reset")))  __Self_SystemReset(void)
#else
    void  __Self_SystemReset(void)
#endif
{
    __DSB();                                                          /* Ensure all outstanding memory accesses included
                                                                       buffered write are completed before reset */
    SCB->AIRCR  = ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
                   SCB_AIRCR_SYSRESETREQ_Msk);
    __DSB();                                                          /* Ensure completion of memory access */

    for (;;)                                                          /* wait until reset */
    {
        __NOP();
    }
}


/**
 *\*\name   ConfigTcmSize.
 *\*\fun    Config TCM_SIZE
 *\*\param  tcmSizeValue
 *\*\    for N32H76x and N32H78x (The input parameters must be the following values):
 *\*\           0x00 :1024KB ITCM,  0   KB DTCM,  0   KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x01 :896 KB ITCM,  128 KB DTCM,  0   KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x02 :768 KB ITCM,  256 KB DTCM,  0   KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x03 :640 KB ITCM,  384 KB DTCM,  0   KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x04 :512 KB ITCM,  512 KB DTCM,  0   KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x05 :384 KB ITCM,  640 KB DTCM,  0   KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x06 :256 KB ITCM,  768 KB DTCM,  0   KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x07 :128 KB ITCM,  896 KB DTCM,  0   KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x08 :0   KB ITCM,  1024KB DTCM,  0   KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x09 :896 KB ITCM,  0   KB DTCM,  128 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x0A :768 KB ITCM,  128 KB DTCM,  128 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x0B :640 KB ITCM,  256 KB DTCM,  128 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x0C :512 KB ITCM,  384 KB DTCM,  128 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x0D :384 KB ITCM,  512 KB DTCM,  128 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x0E :256 KB ITCM,  640 KB DTCM,  128 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x0F :128 KB ITCM,  768 KB DTCM,  128 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x10 :0   KB ITCM,  896 KB DTCM,  128 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x11 :768 KB ITCM,  0   KB DTCM,  256 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x12 :640 KB ITCM,  128 KB DTCM,  256 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x13 :512 KB ITCM,  256 KB DTCM,  256 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x14 :384 KB ITCM,  384 KB DTCM,  256 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x15 :256 KB ITCM,  512 KB DTCM,  256 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x16 :128 KB ITCM,  640 KB DTCM,  256 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x17 :0   KB ITCM,  768 KB DTCM,  256 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x18 :640 KB ITCM,  0   KB DTCM,  384 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x19 :512 KB ITCM,  128 KB DTCM,  384 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x1A :384 KB ITCM,  256 KB DTCM,  384 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x1B :256 KB ITCM,  384 KB DTCM,  384 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x1C :128 KB ITCM,  512 KB DTCM,  384 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x1D :0   KB ITCM,  640 KB DTCM,  384 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x1E :512 KB ITCM,  0   KB DTCM,  512 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x1F :384 KB ITCM,  128 KB DTCM,  512 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x20 :256 KB ITCM,  256 KB DTCM,  512 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x21 :128 KB ITCM,  384 KB DTCM,  512 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x22 :0   KB ITCM,  512 KB DTCM,  512 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x23 :384 KB ITCM,  0   KB DTCM,  640 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x24 :256 KB ITCM,  128 KB DTCM,  640 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x25 :128 KB ITCM,  256 KB DTCM,  640 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x26 :0   KB ITCM,  384 KB DTCM,  640 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x27 :256 KB ITCM,  0   KB DTCM,  768 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x28 :128 KB ITCM,  128 KB DTCM,  768 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x29 :0   KB ITCM,  256 KB DTCM,  768 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x2A :128 KB ITCM,  0   KB DTCM,  896 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x2B :0   KB ITCM,  128 KB DTCM,  896 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x2C~2F :0KB ITCM,  0   KB DTCM,  1024KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\    for N32H73x (The input parameters must be the following values):
 *\*\           0x1E :512 KB ITCM,  0   KB DTCM,  128 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x1F :384 KB ITCM,  128 KB DTCM,  128 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x20 :256 KB ITCM,  256 KB DTCM,  128 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x21 :128 KB ITCM,  384 KB DTCM,  128 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x22 :0   KB ITCM,  512 KB DTCM,  128 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x23 :384 KB ITCM,  0   KB DTCM,  256 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x24 :256 KB ITCM,  128 KB DTCM,  256 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x25 :128 KB ITCM,  256 KB DTCM,  256 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x26 :0   KB ITCM,  384 KB DTCM,  256 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x27 :256 KB ITCM,  0   KB DTCM,  384 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x28 :128 KB ITCM,  128 KB DTCM,  384 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x29 :0   KB ITCM,  256 KB DTCM,  384 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x2A :128 KB ITCM,  0   KB DTCM,  512 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x2B :0   KB ITCM,  128 KB DTCM,  512 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\           0x2C~2F :0KB ITCM,  0   KB DTCM,  640 KB AXI_SRAM2/3,  128KB AXI SRAM1
 *\*\return none
 */
#if defined(__GNUC__)
    void __attribute__((noinline, section(".text.reset"))) ConfigTcmSize(uint32_t tcmSizeValue)
#else
    void ConfigTcmSize(uint32_t tcmSizeValue)
#endif
{
    uint32_t currValue = (*(uint32_t(*)(void))READ_TCM_SIZE)();
    if ((currValue == 0x2c) && (currValue != tcmSizeValue))
    {
        *(__IO uint32_t*)OTP_TCM_SIZE_REG = tcmSizeValue;
        __Self_SystemReset();
    }
    /*Configure ITCM as a protected area When RDP level is L1*/
    if (IS_RDPLEVEL_L1())
    {
        /* when it configs success for the first time, this operation will triggers a system reset. */
        (*(uint32_t (*)(uint32_t, uint32_t))(0x1ff01601U))(ITCM_BASE, 0x20000U * ITCM_SIZE(tcmSizeValue));
    }
}
#ifdef CORE_CM7
/**
*\*\name    PWR_ConfigSupply.
*\*\fun     Configure the PWR supply.
*\*\param   SupplySource (The input parameters must be the following values):
*\*\          - PWR_LDO_SUPPLY               :External LDO  SUPPLY
*\*\          - PWR_DIRECT_SMPS_SUPPLY       :DCDC SUPPLY
*\*\          - PWR_EXTERNAL_SOURCE_SUPPLY   :External VCAP SUPPLY
*\*\return  none
**/
#if defined(__GNUC__)
    static void __attribute__((noinline, section(".text.reset"))) PWR_ConfigSupply(uint32_t SupplySource)
#else
    static void PWR_ConfigSupply(uint32_t SupplySource)
#endif
{
    __IO uint32_t tempreg;
    /* Get the old register value */
    tempreg = PWR->SYSCTRL4;
    /* Clear the old  value */
    tempreg &= (~PWR_SUPPLY_MODE_MASK);
    /* Set the new values */
    tempreg |= SupplySource;
    /* Set the power supply configuration */
    PWR->SYSCTRL4 = tempreg;

    /* Config NRST Filter Function */
    tempreg = PWR->SYSCTRL1;
    tempreg &= ~(PWR_RST_AGFBPEN_MAST | PWR_RST_DGF_CNT_MAST | PWR_RST_DGFBPEN_MAST);
    tempreg |= PWR_RST_DGF_CNT_DEFAULT;
    PWR->SYSCTRL1 = tempreg;
}
#endif
/**
 *\*\name   SystemInit.
 *\*\fun    Setup the microcontroller system.Initialize the FPU setting, vector table location and External memory configuration.
 *\*\param  none
 *\*\return none
 */
#if defined(__GNUC__)
    void __attribute__((section(".text.reset"))) SystemInit(void)
#else
    void SystemInit(void)
#endif
{
    /* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << (10 * 2)) | (3UL << (11 * 2))); /* set CP10 and CP11 Full Access */
#endif

    /*SEVONPEND enabled so that an interrupt coming from the CPU(n) interrupt signal is
     detectable by the CPU after a WFI/WFE instruction.*/
    SCB->SCR |= SCB_SCR_SEVONPEND_Msk;

#ifdef CORE_CM7

#endif /* CORE_CM7*/

#ifdef CORE_CM4

    /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
    SCB->VTOR = AHBSRAM_BASE; /* Vector Table Relocation in Internal SRAM */
#else
    SCB->VTOR = FLASH_BANK2_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif
    /*Configure AHB-SRAM as a protected area When RDP level is L1*/
    if (IS_RDPLEVEL_L1())
    {
        /* when it configs success for the first time, this operation will triggers a system reset. */
        (*(uint32_t (*)(uint32_t, uint32_t))(0x1ff01701))(AHBSRAM_BASE, AHBSRAM_ENDADDR);
    }
#else
#ifdef CORE_CM7

    /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
    SCB->VTOR = SRAM_BASE;        /* Vector Table Relocation in Internal SRAM */
#else
    SCB->VTOR = FLASH_BANK1_BASE | VECT_TAB_OFFSET;       /* Vector Table Relocation in Internal FLASH */
#endif

    /*defaule TCM_SIZE=0x2F,All TCMSRAM are AXI_SRAM,if you want to use ITCM/DTCM, define TCM_SIZE_VALUE*/
#ifdef USING_TCM
    ConfigTcmSize(TCM_SIZE_VALUE);
#endif
    /*User can change the way of power supply */
    PWR_ConfigSupply(PWR_SUPPLY_SELECTION);

#else
#error Please #define CORE_CM4 or CORE_CM7
#endif
#endif

}


/**
 *\*\name   N32SysTick_Handler.
 *\*\fun    This function handles N32SysTick Handler.
 *\*\param  none
 *\*\return none
 */

#if defined(__GNUC__)
    void __attribute__((noinline)) X32SysTick_Handler(void)
#else
    void N32SysTick_Handler(void)
#endif
{

}

