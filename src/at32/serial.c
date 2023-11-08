#include "autoconf.h"
#include "board/armcm_boot.h"
#include "board/serial_irq.h"
#include "command.h"
#include "internal.h"
#include "sched.h"

#if CONFIG_AT32_SERIAL_USART2_PA2_PA3
	#define GPIO_Rx 			GPIO('A',3)
	#define GPIO_Tx 			GPIO('A',2)
	#define USARTx 				USART2
	#define USARTx_IRQn 		USART2_IRQn
	#define USARTx_PCLK			CRM_USART2_PERIPH_CLOCK
	#define USARTx_PIN_PCLK		CRM_GPIOA_PERIPH_CLOCK
	#define USARTx_IRQHandler	USART2_IRQHandler
	DECL_CONSTANT_STR("RESERVE_PINS_serial", "PA2,PA3");
#elif CONFIG_AT32_SERIAL_USART2_PD5_PD6
	#define GPIO_Rx 			GPIO('D',6)
	#define GPIO_Tx 			GPIO('D',5)
	#define USARTx 				USART2
	#define USARTx_IRQn 		USART2_IRQn
	#define USARTx_PCLK			CRM_USART2_PERIPH_CLOCK
	#define USARTx_PIN_PCLK		CRM_GPIOD_PERIPH_CLOCK
	#define USARTx_IRQHandler	USART2_IRQHandler
	DECL_CONSTANT_STR("RESERVE_PINS_serial", "PD5,PD6");
#elif CONFIG_AT32_SERIAL_USART1_PA9_PA10
	#define GPIO_Rx 			GPIO('A',10)
	#define GPIO_Tx 			GPIO('A',9)
	#define USARTx 				USART1
	#define USARTx_IRQn 		USART1_IRQn
	#define USARTx_PCLK			CRM_USART1_PERIPH_CLOCK
	#define USARTx_PIN_PCLK		CRM_GPIOA_PERIPH_CLOCK
	#define USARTx_IRQHandler	USART1_IRQHandler
	DECL_CONSTANT_STR("RESERVE_PINS_serial", "PA9,PA10");
#else
#error "at32 serial configuration error"
#endif


#define USART_IDLE_INT 	MAKE_VALUE(0x0C,0x04)
#define USART_RDBF_INT	MAKE_VALUE(0x0C,0x05)
#define USART_TDC_INT	MAKE_VALUE(0x0C,0x06)
#define USART_TDBE_INT	MAKE_VALUE(0x0C,0x07)

#define USART_IDLEF_FLAG	((uint32_t)0x00000010)
#define USART_RDBF_FLAG		((uint32_t)0x00000020)
#define USART_TDC_FLAG		((uint32_t)0x00000040)
#define USART_TDBE_FLAG		((uint32_t)0x00000080)

//test
#define PRINT_UART                       USART1
#define PRINT_UART_CRM_CLK               CRM_USART1_PERIPH_CLOCK
#define PRINT_UART_TX_PIN                GPIO_PINS_9
#define PRINT_UART_TX_GPIO               GPIOA
#define PRINT_UART_TX_GPIO_CRM_CLK       CRM_GPIOA_PERIPH_CLOCK

void uart_print_init(uint32_t baudrate);
//test

extern void at32f403a_407_gpio_remap(uint32_t gpio_remap, uint8_t new_state);

void
USARTx_IRQHandler(void)
{
	uint8_t data;

	uint8_t ret;

	if(USARTx->sts & USART_RDBF_FLAG)
   	{
		serial_rx_byte(USARTx->dt);
	}

	if(USARTx->sts & USART_TDBE_FLAG)
	{
        ret = serial_get_tx_byte(&data);
       
		if (ret)
		{
			if(USARTx->sts_bit.tdc)	
			{
				USARTx->sts_bit.tdc = 0;

				PERIPH_REG((uint32_t)USARTx, USART_TDBE_INT) &= ~PERIPH_REG_BIT(USART_TDBE_INT);

				USARTx->ctrl1_bit.ten = 1;
		
				USARTx->ctrl1_bit.ren = 1; 
		
				USARTx->ctrl1_bit.uen = 1;

			}
		}
       	else
		{
           	USARTx->dt = data;
		}
	}
}


void
serial_enable_tx_irq(void)
{
	PERIPH_REG((uint32_t)USARTx,USART_TDBE_INT) |= PERIPH_REG_BIT(USART_TDBE_INT);

	return;
}

void 
serial_init(void)
{
	uint32_t temp_val = 0;
	
	gpio_init_type gpio_init_struct;

  	/* enable the uart and gpio clock */
	CRM_REG(USARTx_PCLK) |= CRM_REG_BIT(USARTx_PCLK);
	
	CRM_REG(USARTx_PIN_PCLK) |= CRM_REG_BIT(USARTx_PIN_PCLK);

  	/* configure the uart tx pin */
  	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;

  	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	
	gpio_peripheral(GPIO_Tx,&gpio_init_struct,2);
#if (AT32F403AVGT7 || AT32F415CCU7)
	gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
#else
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
#endif	
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	
	gpio_peripheral(GPIO_Rx,&gpio_init_struct,2);

#if (AT32F425F8P7 || AT32F421F6P7)
	gpio_mux_cfg(GPIO_Rx, GPIO_Rx % 16, GPIO_MUX_1);
	gpio_mux_cfg(GPIO_Tx, GPIO_Tx % 16, GPIO_MUX_1);
#endif

  	armcm_enable_irq(USARTx_IRQHandler,USARTx_IRQn,0);
  
	PERIPH_REG((uint32_t)USARTx, USART_RDBF_INT) |= PERIPH_REG_BIT(USART_RDBF_INT);
  
  	/* configure uart param */
	temp_val = (get_pclock_frequency(1) * 10 / CONFIG_SERIAL_BAUD);
 	
	if((temp_val % 10) < 5)
  	{
    	temp_val = (temp_val / 10);
  	}
  	else
  	{
    	temp_val = (temp_val / 10) + 1;
  	}
	
	USARTx->baudr_bit.div = temp_val;

#if !(AT32F425F8P7)	
	USARTx->ctrl1_bit.dbn = 0;
#else
	USARTx->ctrl1_bit.dbn_h = 0;
	
	USARTx->ctrl1_bit.dbn_l = 0;
#endif

	USARTx->ctrl2_bit.stopbn = 0;
	
	USARTx->ctrl1_bit.ten = 1;
  	
	USARTx->ctrl1_bit.ren = 1;
 	
	USARTx->ctrl1_bit.uen = 1;
}		
		

DECL_INIT(serial_init);
	

