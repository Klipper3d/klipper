#include "autoconf.h"
#include "board/armcm_boot.h"
#include "board/armcm_reset.h"
#include "board/irq.h"
#include "board/usb_cdc.h"
#include "internal.h"
#include "sched.h"


void
watchdog_task(void)
{
	WDT->cmd = WDT_CMD_RELOAD;
}

DECL_TASK(watchdog_task);

void watchdog_init(void)
{
	CRM->ctrlsts_bit.rstfc = 1;

    while(CRM->ctrlsts_bit.rstfc == 1){}

	WDT->cmd = WDT_CMD_UNLOCK;

	WDT->div_bit.div = WDT_CLK_DIV_8;

	WDT->rld = 3000 - 1; //Watchdog reset period in [400ms,800ms]

	WDT->cmd = WDT_CMD_ENABLE;
}

DECL_INIT(watchdog_init);


/**
  * @brief  system clock config program
  * @note   the system clock is configured as follow:
  *         - system clock        = hext * pll_mult
  *         - system clock source = pll (hext)
  *         - hext                = 8000000
  *         - sclk                = 144000000
  *         - ahbdiv              = 1
  *         - ahbclk              = 144000000
  *         - apb1div             = 2
  *         - apb1clk             = 72000000
  *         - apb2div             = 2
  *         - apb2clk             = 72000000
  *         - pll_mult            = 18
  *         - flash_wtcyc         = 4 cycle
  * @param  none
  * @retval none
  */
static void
systemclockconfig(void)
{
  	/* config flash psr register */
	FLASH->psr |= (uint32_t)(0x150 | FLASH_WAIT_CYCLE_4);
 
  	/* reset crm */
  	crm_reset();

  	/* enable hext */
  	crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);

   	/* wait till hext is ready */
  	while(crm_hext_stable_wait() == ERROR){}

 	/* config pll clock resource */
  	crm_pll_config(CRM_PLL_SOURCE_HEXT, CRM_PLL_MULT_18);

  	/* enable pll */
  	crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

  	/* wait till pll is ready */
  	while(crm_flag_get(CRM_PLL_STABLE_FLAG) != SET){}

  	/* config ahbclk */
  	crm_ahb_div_set(CRM_AHB_DIV_1);

  	/* config apb2clk */
  	crm_apb2_div_set(CRM_APB2_DIV_2);

  	/* config apb1clk */
  	crm_apb1_div_set(CRM_APB1_DIV_2);

  	/* enable auto step mode */
  	crm_auto_step_mode_enable(TRUE);

  	/* select pll as system clock source */
  	crm_sysclk_switch(CRM_SCLK_PLL);

  	/* wait till pll is used as system clock source */
  	while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL){}

  	/* disable auto step mode */
  	crm_auto_step_mode_enable(FALSE);

  	/* update system_core_clock global variable */
  	system_core_clock_update();
}


void
armcm_main(void)
{
	SystemInit();

	SCB->VTOR = (uint32_t)VectorTable;

	systemclockconfig();

	sched_main();
}
