/* ---------------------------------------------------------------------------- */
/*                  Atmel Microcontroller Software Support                      */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) %copyright_year%, Atmel Corporation                                        */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Atmel's name may not be used to endorse or promote products derived from     */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR   */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE   */
/* DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,      */
/* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,  */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    */
/* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING         */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, */
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                           */
/* ---------------------------------------------------------------------------- */

#include "sam4e.h"

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

/* Clock Settings (120MHz) */
#define SYS_BOARD_OSCOUNT   (CKGR_MOR_MOSCXTST(0x8U))
#define SYS_BOARD_PLLAR     (CKGR_PLLAR_ONE \
							| CKGR_PLLAR_MULA(0x13U) \
							| CKGR_PLLAR_PLLACOUNT(0x3fU) \
							| CKGR_PLLAR_DIVA(0x1U))
#define SYS_BOARD_MCKR      (PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK)

#define SYS_CKGR_MOR_KEY_VALUE	CKGR_MOR_KEY(0x37) /* Key to unlock MOR register */

uint32_t SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;

/**
 * \brief Setup the microcontroller system.
 * Initialize the System and update the SystemFrequency variable.
 */
void SystemInit( void )
{
	/* Set FWS according to SYS_BOARD_MCKR configuration */
	EFC->EEFC_FMR = EEFC_FMR_FWS(5);


	/* Initialize main oscillator */
	if ( !(PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) )
  {
		PMC->CKGR_MOR = SYS_CKGR_MOR_KEY_VALUE | SYS_BOARD_OSCOUNT | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;

		while ( !(PMC->PMC_SR & PMC_SR_MOSCXTS) )
    {
		}
	}

	/* Switch to 3-20MHz Xtal oscillator */
	PMC->CKGR_MOR = SYS_CKGR_MOR_KEY_VALUE | SYS_BOARD_OSCOUNT | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCSEL;

	while ( !(PMC->PMC_SR & PMC_SR_MOSCSELS) )
  {
	}

	PMC->PMC_MCKR = (PMC->PMC_MCKR & ~(uint32_t)PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;

	while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) )
  {
  }

	/* Initialize PLLA */
	PMC->CKGR_PLLAR = SYS_BOARD_PLLAR;
	while ( !(PMC->PMC_SR & PMC_SR_LOCKA) )
  {
	}

	/* Switch to main clock */
	PMC->PMC_MCKR = (SYS_BOARD_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
	while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) )
  {
  }

	/* Switch to PLLA */
	PMC->PMC_MCKR = SYS_BOARD_MCKR;
	while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) )
  {
  }

	SystemCoreClock = CHIP_FREQ_CPU_MAX;
}

void SystemCoreClockUpdate( void )
{
	/* Determine clock frequency according to clock register values */
	switch (PMC->PMC_MCKR & (uint32_t) PMC_MCKR_CSS_Msk)
  {
	  case PMC_MCKR_CSS_SLOW_CLK:	/* Slow clock */
      if ( SUPC->SUPC_SR & SUPC_SR_OSCSEL )
      {
        SystemCoreClock = CHIP_FREQ_XTAL_32K;
      }
      else
      {
        SystemCoreClock = CHIP_FREQ_SLCK_RC;
      }
		break;

    case PMC_MCKR_CSS_MAIN_CLK:	/* Main clock */
		if ( PMC->CKGR_MOR & CKGR_MOR_MOSCSEL )
    {
			SystemCoreClock = CHIP_FREQ_XTAL_12M;
		}
    else
    {
			SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;

			switch ( PMC->CKGR_MOR & CKGR_MOR_MOSCRCF_Msk )
      {
        case CKGR_MOR_MOSCRCF_4_MHz:
				break;

        case CKGR_MOR_MOSCRCF_8_MHz:
          SystemCoreClock *= 2U;
				break;

        case CKGR_MOR_MOSCRCF_12_MHz:
          SystemCoreClock *= 3U;
				break;

        default:
				break;
			}
		}
		break;

    case PMC_MCKR_CSS_PLLA_CLK:	/* PLLA clock */
      if ( PMC->CKGR_MOR & CKGR_MOR_MOSCSEL )
      {
        SystemCoreClock = CHIP_FREQ_XTAL_12M ;
      }
      else
      {
        SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;

        switch ( PMC->CKGR_MOR & CKGR_MOR_MOSCRCF_Msk )
        {
          case CKGR_MOR_MOSCRCF_4_MHz:
          break;

          case CKGR_MOR_MOSCRCF_8_MHz:
            SystemCoreClock *= 2U;
          break;

          case CKGR_MOR_MOSCRCF_12_MHz:
            SystemCoreClock *= 3U;
          break;

          default:
          break;
        }
      }

      if ((uint32_t) (PMC->PMC_MCKR & (uint32_t) PMC_MCKR_CSS_Msk) == PMC_MCKR_CSS_PLLA_CLK)
      {
        SystemCoreClock *= ((((PMC->CKGR_PLLAR) & CKGR_PLLAR_MULA_Msk) >> CKGR_PLLAR_MULA_Pos) + 1U);
        SystemCoreClock /= ((((PMC->CKGR_PLLAR) & CKGR_PLLAR_DIVA_Msk) >> CKGR_PLLAR_DIVA_Pos));
      }
		break;

    default:
		break;
	}

	if ((PMC->PMC_MCKR & PMC_MCKR_PRES_Msk) == PMC_MCKR_PRES_CLK_3)
  {
		SystemCoreClock /= 3U;
	}
  else
  {
		SystemCoreClock >>= ((PMC->PMC_MCKR & PMC_MCKR_PRES_Msk) >> PMC_MCKR_PRES_Pos);
	}
}

/**
 * Initialize flash.
 */
void system_init_flash( uint32_t ul_clk )
{
	/* Set FWS for embedded Flash access according to operating frequency */
	if ( ul_clk < CHIP_FREQ_FWS_0 )
  {
		EFC->EEFC_FMR = EEFC_FMR_FWS(0);
	}
  else
  {
    if (ul_clk < CHIP_FREQ_FWS_1)
    {
		  EFC->EEFC_FMR = EEFC_FMR_FWS(1);
	  }
    else
    {
      if (ul_clk < CHIP_FREQ_FWS_2)
      {
		    EFC->EEFC_FMR = EEFC_FMR_FWS(2);
	    }
      else
      {
        if ( ul_clk < CHIP_FREQ_FWS_3 )
        {
		      EFC->EEFC_FMR = EEFC_FMR_FWS(3);
	      }
        else
        {
          if ( ul_clk < CHIP_FREQ_FWS_4 )
          {
		        EFC->EEFC_FMR = EEFC_FMR_FWS(4);
	        }
          else
          {
            EFC->EEFC_FMR = EEFC_FMR_FWS(5);
          }
        }
      }
    }
  }
}

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */
