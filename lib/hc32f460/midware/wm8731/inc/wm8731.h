/*****************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file wm8731.h
 **
 ** A detailed description is available at
 ** @link Wm8731Group WM8731 description @endlink
 **
 **   - 2018-10-13  CDT First version for Device Driver Library.
 **
 ******************************************************************************/
#ifndef __WM8731_H__
#define __WM8731_H__


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
 ** \defgroup Wm8731Group Wm8731 CODECs with an integrated headphone driver
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*****************************************************************************/
/* Global pre-processor symbols/macros ('define')                            */
/*****************************************************************************/
#define WM8731_ADDRESS             (0x1Au)      // WM8731 chip address on I2C bus

#define WM8731_REG_LLINE_IN        (0x00u)      // Left Channel Line Input Volume Control
#define WM8731_REG_RLINE_IN        (0x01u)      // Right Channel Line Input Volume Control
#define WM8731_REG_LHPHONE_OUT     (0x02u)      // Left Channel Headphone Output Volume Control
#define WM8731_REG_RHPHONE_OUT     (0x03u)      // Right Channel Headphone Output Volume Control
#define WM8731_REG_ANALOG_PATH     (0x04u)      // Analog Audio Path Control
#define WM8731_REG_DIGITAL_PATH    (0x05u)      // Digital Audio Path Control
#define WM8731_REG_PDOWN_CTRL      (0x06u)      // Power Down Control Register
#define WM8731_REG_DIGITAL_IF      (0x07u)      // Digital Audio Interface Format
#define WM8731_REG_SAMPLING_CTRL   (0x08u)      // Sampling Control Register
#define WM8731_REG_ACTIVE_CTRL     (0x09u)      // Active Control
#define WM8731_REG_RESET           (0x0Fu)      // Reset register

/******************************************************************************
 * WM8731 registers bit field
 ******************************************************************************/
typedef struct stc_wm8731_llin_field
{
    uint16_t LINVOL   : 5;
    uint16_t RESERVED : 2;
    uint16_t LINMUTE  : 1;
    uint16_t LRINBOTH : 1;

}stc_wm8731_llin_field_t;

typedef struct stc_wm8731_rlin_field
{
    uint16_t RINVOL   : 5;
    uint16_t RESERVED : 2;
    uint16_t RINMUTE  : 1;
    uint16_t RINBOTH  : 1;

}stc_wm8731_rlin_field_t;

typedef struct stc_wm8731_lhout_field
{
    uint16_t LHPVOL   : 7;
    uint16_t LZCEN    : 1;
    uint16_t LRHPBOTH : 1;

}stc_wm8731_lhout_field_t;

typedef struct stc_wm8731_rhout_field
{
    uint16_t RHPVOL   : 7;
    uint16_t RZCEN    : 1;
    uint16_t RLHPBOTH : 1;

}stc_wm8731_rhout_field_t;

typedef struct stc_wm8731_aapc_field
{
    uint16_t MICBOOST   : 1;
    uint16_t MUTEMIC    : 1;
    uint16_t INSEL      : 1;
    uint16_t BYPASS     : 1;
    uint16_t DACSEL     : 1;
    uint16_t SIDETONE   : 1;
    uint16_t SIDEATT    : 2;

}stc_wm8731_aapc_field_t;

typedef struct stc_wm8731_dapc_field
{
    uint16_t ADCHPD   : 1;
    uint16_t DEEMP    : 2;
    uint16_t DACMU    : 1;
    uint16_t HPOR     : 1;

}stc_wm8731_dapc_field_t;

typedef struct stc_wm8731_pdc_field
{
    uint16_t LINEINPD   : 1;
    uint16_t MICPD      : 1;
    uint16_t ADCPD      : 1;
    uint16_t DACPD      : 1;
    uint16_t OUTPD      : 1;
    uint16_t OSCPD      : 1;
    uint16_t CLKOUTPD   : 1;
    uint16_t POWEROFF   : 1;

}stc_wm8731_pdc_field_t;

typedef struct stc_wm8731_daif_field
{
    uint16_t FORMAT     : 2;
    uint16_t IWL        : 2;
    uint16_t LRP        : 1;
    uint16_t LRSWAP     : 1;
    uint16_t MS         : 1;
    uint16_t BCLKINV    : 1;

}stc_wm8731_daif_field_t;

typedef struct stc_wm8731_sc_field
{
    uint16_t NORMAL_USB : 1;
    uint16_t BOSR       : 1;
    uint16_t SR         : 4;
    uint16_t CLKDIV2    : 1;
    uint16_t CLKODIV2   : 1;
}stc_wm8731_sc_field_t;

typedef struct stc_wm8731_ac_field
{
    uint16_t ACTIVE : 1;

}stc_wm8731_ac_field_t;

/******************************************************************************
 * WM8731 registers structure
 ******************************************************************************/
typedef struct stc_wm8731_reg
{
  union {
    __IO  uint16_t LLIN;                    ///< Left line in register
    stc_wm8731_llin_field_t LLIN_f;
  };

  union {
    __IO  uint16_t RLIN;                   ///< Right line in register
    stc_wm8731_rlin_field_t RLIN_f;
  };

  union {
    __IO  uint16_t LHOUT;                 ///< Left headphone out register
    stc_wm8731_lhout_field_t LHOUT_f;
  };

  union {
    __IO  uint16_t RHOUT;                 ///< Right headphone out register
    stc_wm8731_rhout_field_t RHOUT_f;
  };

  union {
    __IO  uint16_t AAPC;                  ///< Analog audio path control register
    stc_wm8731_aapc_field_t AAPC_f;
  };

  union {
    __IO  uint16_t DAPC;                 ///< Digital audio path control register
    stc_wm8731_dapc_field_t DAPC_f;
  };

  union {
    __IO  uint16_t PDC;                  ///< Power down control register
    stc_wm8731_pdc_field_t PDC_f;
  };

  union {
    __IO  uint16_t DAIF;                 ///< Digital audio interface format register
    stc_wm8731_daif_field_t DAIF_f;
  };

  union {
    __IO  uint16_t SC;                   ///< Sampling control register
    stc_wm8731_sc_field_t SC_f;
  };

  union {
    __IO  uint16_t AC;                   ///< Active control register
    stc_wm8731_ac_field_t AC_f;
  };

  __IO  uint16_t RESET;                ///< Reset register

}stc_wm8731_reg_t;


/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
en_result_t WM8731_Init(M4_I2C_TypeDef* pstcI2c, stc_wm8731_reg_t* pstcReg);
en_result_t WM8731_SetHpVolume(M4_I2C_TypeDef* pstcI2c, uint16_t u16LeftVol, uint16_t u16RightVol);

//@} // Wm8731Group

#ifdef __cplusplus
}
#endif


#endif /* __WM8731_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
