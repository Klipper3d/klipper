/*!
    \file    gd32f30x_dbg.h
    \brief   definitions for the DBG

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
    \version 2020-09-30, V2.1.0, firmware for GD32F30x
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef GD32F30X_DBG_H
#define GD32F30X_DBG_H

#include "gd32f30x.h"

/* DBG definitions */
#define DBG                      DBG_BASE

/* registers definitions */
#define DBG_ID                   REG32(DBG + 0x00U)         /*!< DBG_ID code register */
#define DBG_CTL0                 REG32(DBG + 0x04U)         /*!< DBG control register 0 */

/* bits definitions */
/* DBG_ID */
#define DBG_ID_ID_CODE           BITS(0,31)                 /*!< DBG ID code values */

/* DBG_CTL0 */
#define DBG_CTL0_SLP_HOLD        BIT(0)                     /*!< keep debugger connection during sleep mode */
#define DBG_CTL0_DSLP_HOLD       BIT(1)                     /*!< keep debugger connection during deepsleep mode */
#define DBG_CTL0_STB_HOLD        BIT(2)                     /*!< keep debugger connection during standby mode */
#define DBG_CTL0_TRACE_IOEN      BIT(5)                     /*!< enable trace pin assignment */
#define DBG_CTL0_FWDGT_HOLD      BIT(8)                     /*!< debug FWDGT kept when core is halted */
#define DBG_CTL0_WWDGT_HOLD      BIT(9)                     /*!< debug WWDGT kept when core is halted */
#define DBG_CTL0_TIMER0_HOLD     BIT(10)                    /*!< hold TIMER0 counter when core is halted */
#define DBG_CTL0_TIMER1_HOLD     BIT(11)                    /*!< hold TIMER1 counter when core is halted */
#define DBG_CTL0_TIMER2_HOLD     BIT(12)                    /*!< hold TIMER2 counter when core is halted */
#define DBG_CTL0_TIMER3_HOLD     BIT(13)                    /*!< hold TIMER3 counter when core is halted */
#define DBG_CTL0_CAN0_HOLD       BIT(14)                    /*!< debug CAN0 kept when core is halted */
#define DBG_CTL0_I2C0_HOLD       BIT(15)                    /*!< hold I2C0 smbus when core is halted */
#define DBG_CTL0_I2C1_HOLD       BIT(16)                    /*!< hold I2C1 smbus when core is halted */
#define DBG_CTL0_TIMER7_HOLD     BIT(17)                    /*!< hold TIMER7 counter when core is halted */
#define DBG_CTL0_TIMER4_HOLD     BIT(18)                    /*!< hold TIMER4 counter when core is halted */
#define DBG_CTL0_TIMER5_HOLD     BIT(19)                    /*!< hold TIMER5 counter when core is halted */
#define DBG_CTL0_TIMER6_HOLD     BIT(20)                    /*!< hold TIMER6 counter when core is halted */
#ifdef GD32F30X_CL
#define DBG_CTL0_CAN1_HOLD       BIT(21)                    /*!< debug CAN1 kept when core is halted */
#endif /* GD32F30X_CL */
#ifndef GD32F30X_HD
#define DBG_CTL0_TIMER11_HOLD    BIT(25)                    /*!< hold TIMER11 counter when core is halted */
#define DBG_CTL0_TIMER12_HOLD    BIT(26)                    /*!< hold TIMER12 counter when core is halted */
#define DBG_CTL0_TIMER13_HOLD    BIT(27)                    /*!< hold TIMER13 counter when core is halted */
#define DBG_CTL0_TIMER8_HOLD     BIT(28)                    /*!< hold TIMER8 counter when core is halted */
#define DBG_CTL0_TIMER9_HOLD     BIT(29)                    /*!< hold TIMER9 counter when core is halted */
#define DBG_CTL0_TIMER10_HOLD    BIT(30)                    /*!< hold TIMER10 counter when core is halted */
#endif /* GD32F30X_HD */

/* constants definitions */
#define DBG_LOW_POWER_SLEEP      DBG_CTL0_SLP_HOLD          /*!< keep debugger connection during sleep mode */
#define DBG_LOW_POWER_DEEPSLEEP  DBG_CTL0_DSLP_HOLD         /*!< keep debugger connection during deepsleep mode */
#define DBG_LOW_POWER_STANDBY    DBG_CTL0_STB_HOLD          /*!< keep debugger connection during standby mode */

/* define the peripheral debug hold bit position and its register index offset */
#define DBG_REGIDX_BIT(regidx, bitpos)      (((regidx) << 6) | (bitpos))
#define DBG_REG_VAL(periph)                 (REG32(DBG + ((uint32_t)(periph) >> 6)))
#define DBG_BIT_POS(val)                    ((uint32_t)(val) & 0x1FU)

/* register index */
enum dbg_reg_idx
{
    DBG_IDX_CTL0  = 0x04U
};

typedef enum
{
    DBG_FWDGT_HOLD             = DBG_REGIDX_BIT(DBG_IDX_CTL0, 8U),                    /*!< debug FWDGT kept when core is halted */
    DBG_WWDGT_HOLD             = DBG_REGIDX_BIT(DBG_IDX_CTL0, 9U),                    /*!< debug WWDGT kept when core is halted */
    DBG_TIMER0_HOLD            = DBG_REGIDX_BIT(DBG_IDX_CTL0, 10U),                   /*!< hold TIMER0 counter when core is halted */
    DBG_TIMER1_HOLD            = DBG_REGIDX_BIT(DBG_IDX_CTL0, 11U),                   /*!< hold TIMER1 counter when core is halted */
    DBG_TIMER2_HOLD            = DBG_REGIDX_BIT(DBG_IDX_CTL0, 12U),                   /*!< hold TIMER2 counter when core is halted */
    DBG_TIMER3_HOLD            = DBG_REGIDX_BIT(DBG_IDX_CTL0, 13U),                   /*!< hold TIMER3 counter when core is halted */
    DBG_CAN0_HOLD              = DBG_REGIDX_BIT(DBG_IDX_CTL0, 14U),                   /*!< debug CAN0 kept when core is halted */
    DBG_I2C0_HOLD              = DBG_REGIDX_BIT(DBG_IDX_CTL0, 15U),                   /*!< hold I2C0 smbus when core is halted */
    DBG_I2C1_HOLD              = DBG_REGIDX_BIT(DBG_IDX_CTL0, 16U),                   /*!< hold I2C1 smbus when core is halted */
    DBG_TIMER7_HOLD            = DBG_REGIDX_BIT(DBG_IDX_CTL0, 17U),                   /*!< hold TIMER7 counter when core is halted */
    DBG_TIMER4_HOLD            = DBG_REGIDX_BIT(DBG_IDX_CTL0, 18U),                   /*!< hold TIMER4 counter when core is halted */
    DBG_TIMER5_HOLD            = DBG_REGIDX_BIT(DBG_IDX_CTL0, 19U),                   /*!< hold TIMER5 counter when core is halted */
    DBG_TIMER6_HOLD            = DBG_REGIDX_BIT(DBG_IDX_CTL0, 20U),                   /*!< hold TIMER6 counter when core is halted */
#ifdef GD32F30X_CL
    DBG_CAN1_HOLD              = DBG_REGIDX_BIT(DBG_IDX_CTL0, 21U),                   /*!< debug CAN1 kept when core is halted */
#endif /* GD32F30X_CL */
#ifndef GD32F30X_HD
    DBG_TIMER11_HOLD           = DBG_REGIDX_BIT(DBG_IDX_CTL0, 25U),                   /*!< hold TIMER11 counter when core is halted */
    DBG_TIMER12_HOLD           = DBG_REGIDX_BIT(DBG_IDX_CTL0, 26U),                   /*!< hold TIMER12 counter when core is halted */
    DBG_TIMER13_HOLD           = DBG_REGIDX_BIT(DBG_IDX_CTL0, 27U),                   /*!< hold TIMER13 counter when core is halted */
    DBG_TIMER8_HOLD            = DBG_REGIDX_BIT(DBG_IDX_CTL0, 28U),                   /*!< hold TIMER8 counter when core is halted */
    DBG_TIMER9_HOLD            = DBG_REGIDX_BIT(DBG_IDX_CTL0, 29U),                   /*!< hold TIMER9 counter when core is halted */
    DBG_TIMER10_HOLD           = DBG_REGIDX_BIT(DBG_IDX_CTL0, 30U),                   /*!< hold TIMER10 counter when core is halted */
#endif /* GD32F30X_HD */
}dbg_periph_enum;

/* function declarations */
/* deinitialize the DBG */
void dbg_deinit(void);
/* read DBG_ID code register */
uint32_t dbg_id_get(void);

/* enable low power behavior when the MCU is in debug mode */
void dbg_low_power_enable(uint32_t dbg_low_power);
/* disable low power behavior when the MCU is in debug mode */
void dbg_low_power_disable(uint32_t dbg_low_power);

/* enable peripheral behavior when the MCU is in debug mode */
void dbg_periph_enable(dbg_periph_enum dbg_periph);
/* disable peripheral behavior when the MCU is in debug mode */
void dbg_periph_disable(dbg_periph_enum dbg_periph);

/* enable trace pin assignment */
void dbg_trace_pin_enable(void);
/* disable trace pin assignment */
void dbg_trace_pin_disable(void);

#endif /* GD32F30X_DBG_H */
