#include "autoconf.h"
#include "board/armcm_boot.h"
#include "board/armcm_reset.h"
#include "board/irq.h"
#include "board/usb_cdc.h"
#include "internal.h"
#include "sched.h"
#include "at32f403a_407_board.h"

#include "gpio.h" //ONLY FOR TESTING GPIO????????????????????????????????

//extern void adc_config(void);	
/**********************************************************************************
 * Startup
 **********************************************************************************/
//Main entry point - called from armcm_boot.c:ResetHandler()
static void
at32f403a_swjatg_reamp(void)
{
	enable_pclock(CRM_IOMUX_PERIPH_CLOCK);

	enable_pclock(CRM_GPIOA_PERIPH_CLOCK);
			
	enable_pclock(CRM_GPIOB_PERIPH_CLOCK);

	at32f403a_407_gpio_remap(SWJTAG_MUX_100,1);
}

static void
systemclockconfig(void)
{
	crm_reset();
	
	crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT,1);

	while(crm_hext_stable_wait() == 0){}

	crm_pll_config(CRM_PLL_SOURCE_HEXT_DIV, CRM_PLL_MULT_60, CRM_PLL_OUTPUT_RANGE_GT72MHZ);

	crm_hext_clock_div_set(CRM_HEXT_DIV_2);

	crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, 1);

	while(crm_flag_get(CRM_PLL_STABLE_FLAG) != 1){}

	crm_ahb_div_set(CRM_AHB_DIV_1);

	crm_apb2_div_set(CRM_APB2_DIV_2);

	crm_apb1_div_set(CRM_APB1_DIV_2);

	crm_auto_step_mode_enable(TRUE);

	crm_sysclk_switch(CRM_SCLK_PLL);

	while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL){}

	crm_auto_step_mode_enable(0);

	system_core_clock_update();
}

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


/**************** define print uart ******************/
#define PRINT_UART                       USART1

#define PRINT_UART_CRM_CLK               CRM_USART1_PERIPH_CLOCK

#define PRINT_UART_TX_PIN                GPIO_PINS_9

#define PRINT_UART_TX_GPIO               GPIOA

#define PRINT_UART_TX_GPIO_CRM_CLK       CRM_GPIOA_PERIPH_CLOCK


int kprintf(char *pBuffer)
{
	uint32_t size = strlen(pBuffer);

	uint32_t i = 0;

	for(i = 0; i < size; i++)
	{
		while(!(USART1->sts & USART_TDBE_FLAG)){}
		USART1->dt = (pBuffer[i] & 0x01FF);
	}
	return size;
}

armcm_main(void)
{
	uint8_t i = 0;
	uint16_t convdata = 0;
	struct gpio_pwm gpio_pwm_if;
	struct gpio_out gpio_out_str;
	struct gpio_out gpio_out_str1;
	struct gpio_out gpio_out_str2;

	SystemInit();

	SCB->VTOR = (uint32_t)VectorTable;

	systemclockconfig();
#if 0
	delay_init();
	//at32f403a_swjatg_reamp();
	//uart_print_init(115200);
		
	gpio_out_str = gpio_out_setup(GPIO('D',13),0);
	
	gpio_out_str1 = gpio_out_setup(GPIO('D',14),1);
	
	gpio_out_str2 = gpio_out_setup(GPIO('D',15),0);
	
	struct gpio_adc gpio_adc_ch4 = gpio_adc_setup(GPIO('A',4));
	
	gpio_pwm_if = gpio_pwm_setup(GPIO('A',6),255,128);

  	gpio_pwm_write(gpio_pwm_if,64);	

	//serial_init();
	while(1)
	{
		while(gpio_adc_sample(gpio_adc_ch4) != 0)
		{
			kprintf("gpio_adc_sample no start or stop\r\n");
		}
		convdata = gpio_adc_read(gpio_adc_ch4);

		gpio_out_toggle(gpio_out_str);	
		gpio_out_toggle(gpio_out_str1);	
		gpio_out_toggle(gpio_out_str2);	

		delay_ms(convdata + 200);
	}

#else

	sched_main();

#endif
}
