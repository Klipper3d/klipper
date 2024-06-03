#include "autoconf.h" // CONFIG_CLOCK_REF_FREQ
#include "board/armcm_boot.h" // armcm_main
#include "board/armcm_reset.h" // try_request_canboot
#include "board/irq.h" // irq_disable
#include "board/misc.h" // bootloader_request
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // enable_pclock
#include "sched.h" // sched_main


#define RCU_MODIFY(__delay)     do{                                     \
                                    volatile uint32_t i;                \
                                    if(0 != __delay){                   \
                                        RCU_CFG0 |= RCU_AHB_CKSYS_DIV2; \
                                        for(i=0; i<__delay; i++){       \
                                        }                               \
                                        RCU_CFG0 |= RCU_AHB_CKSYS_DIV4; \
                                        for(i=0; i<__delay; i++){       \
                                        }                               \
                                    }                                   \
                                }while(0)

#define SEL_IRC8M       0x00U
#define SEL_HXTAL       0x01U
#define SEL_PLL         0x02U

#define OSC_STARTUP_TIMEOUT         ((uint32_t)0x000FFFFFU)
#define LXTAL_STARTUP_TIMEOUT       ((uint32_t)0x03FFFFFFU)

#include "gpio.h"
struct gpio_out led1,led2;

void
watchdog_task(void)
{
	FWDGT_CTL = FWDGT_KEY_RELOAD;	
}

DECL_TASK(watchdog_task);

void
watchdog_init(void)
{
	uint32_t timeout = FWDGT_PSC_TIMEOUT;
   
	uint32_t flag_status = RESET;

	uint16_t reload_value = 2500;  			//500ms

	uint8_t prescaler_div = FWDGT_PSC_DIV8;
	
	uint32_t stb_cnt = 0U;

	FlagStatus osci_stat = RESET;

	RCU_REG_VAL(RCU_IRC40K) |= BIT(RCU_BIT_POS(RCU_IRC40K));

	while((RESET == osci_stat) && (OSC_STARTUP_TIMEOUT != stb_cnt))
	{
		osci_stat = (RCU_REG_VAL(RCU_FLAG_IRC40KSTB) & BIT(RCU_BIT_POS(RCU_FLAG_IRC40KSTB)));
            
		stb_cnt++;
	}
     
	 /* check whether flag is set or not */
    if(RESET == (RCU_REG_VAL(RCU_FLAG_IRC40KSTB) & BIT(RCU_BIT_POS(RCU_FLAG_IRC40KSTB))))
	{
        shutdown("watchdog initialize fail");
    }
  
    FWDGT_CTL = FWDGT_WRITEACCESS_ENABLE;
  
    do
	{
        flag_status = FWDGT_STAT & FWDGT_STAT_PUD;
    
	}while((--timeout > 0U) && ((uint32_t)RESET != flag_status));
    
    if ((uint32_t)RESET != flag_status)
	{
        shutdown("watchdog initialize fail");
    }
    
    FWDGT_PSC = (uint32_t)prescaler_div;       

    timeout = FWDGT_RLD_TIMEOUT;   
	
	flag_status = RESET;
    
	do
	{
        flag_status = FWDGT_STAT & FWDGT_STAT_RUD;
    
	}while((--timeout > 0U) && ((uint32_t)RESET != flag_status));
   
    if ((uint32_t)RESET != flag_status)
	{
        shutdown("watchdog initialize fail");
    }
	
    FWDGT_RLD = RLD_RLD(reload_value);
    
    FWDGT_CTL = FWDGT_KEY_RELOAD;

	FWDGT_CTL = FWDGT_KEY_ENABLE;

	return;
}

DECL_INIT(watchdog_init);


uint32_t SystemCoreClock = SYSTEM_CLOCK_PLL_HXTAL;

static void systemClock72mHxtal(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;

    /* enable HXTAL */
    RCU_CTL0 |= RCU_CTL0_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do{
        timeout++;
        stab_flag = (RCU_CTL0 & RCU_CTL0_HXTALSTB);
    }
    while((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));
    /* if fail */
    if(0U == (RCU_CTL0 & RCU_CTL0_HXTALSTB)){
        while(1){
        }
    }
    
    FMC_WS = (FMC_WS & (~FMC_WS_WSCNT)) | WS_WSCNT_2;
    
    /* HXTAL is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV1;

    /* PLL = HXTAL * 9 = 72 MHz */
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF | RCU_CFG0_PLLDV);
    RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_PLL_MUL9);

    /* enable PLL */
    RCU_CTL0 |= RCU_CTL0_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL0 & RCU_CTL0_PLLSTB)){
    }

    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLL;

    /* wait until PLL is selected as system clock */
    while(RCU_SCSS_PLL != (RCU_CFG0 & RCU_CFG0_SCSS)){
    }
}


void systemInit (void)
{
    /* enable IRC8M */
    RCU_CTL0 |= RCU_CTL0_IRC8MEN;
    while(0U == (RCU_CTL0 & RCU_CTL0_IRC8MSTB)){
    }

    RCU_MODIFY(0x80);
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CTL0 &= ~(RCU_CTL0_HXTALEN | RCU_CTL0_CKMEN | RCU_CTL0_PLLEN | RCU_CTL0_HXTALBPS);
    /* reset RCU */
    RCU_CFG0 &= ~(RCU_CFG0_SCS | RCU_CFG0_AHBPSC | RCU_CFG0_APB1PSC | RCU_CFG0_APB2PSC |\
                  RCU_CFG0_ADCPSC | RCU_CFG0_CKOUTSEL | RCU_CFG0_CKOUTDIV | RCU_CFG0_PLLDV);
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF | RCU_CFG0_PLLMF4 | RCU_CFG0_PLLDV);
    RCU_CFG1 &= ~(RCU_CFG1_PREDV);
    RCU_CFG2 &= ~(RCU_CFG2_USART0SEL | RCU_CFG2_ADCSEL);
    RCU_CFG2 &= ~RCU_CFG2_IRC28MDIV;
    RCU_CFG2 &= ~RCU_CFG2_ADCPSC2;
    RCU_CTL1 &= ~RCU_CTL1_IRC28MEN;
    RCU_INT = 0x00000000U;

    /* configure system clock */
	systemClock72mHxtal();
}

void
armcm_main(void)
{
	systemInit();

	SCB->VTOR = (uint32_t)VectorTable;

	enable_pclock(RCU_GPIOA);			//why? I don't know.

	sched_main();
}

