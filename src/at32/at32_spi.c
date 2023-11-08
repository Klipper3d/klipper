#include "autoconf.h" // CONFIG_MACH_STM32F1
#include "board/misc.h" // timer_is_before
#include "command.h" // shutdown
#include "gpio.h" // spi_setup
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown
#include "board/irq.h" //irq_disable


struct spi_info {
    
	spi_typedef *spi;
    
	uint8_t miso_pin;
    
	uint8_t	mosi_pin;
    
	uint8_t	sck_pin;
   	
	uint32_t spi_clk;
};

DECL_ENUMERATION("spi_bus", "spi1", 0);
DECL_CONSTANT_STR("BUS_PINS_spi1", "PA6,PA7,PA5");

DECL_ENUMERATION("spi_bus", "spi2", 1);
DECL_CONSTANT_STR("BUS_PINS_spi2", "PB14,PB15,PB13");

DECL_ENUMERATION("spi_bus", "spi3", 2);
DECL_CONSTANT_STR("BUS_PINS_spi3", "PB4,PB5,PB3");

struct spi_info spi_infos[] = {
	
	{SPI1, GPIO('A',6), GPIO('A',7), GPIO('A',5), CRM_SPI1_PERIPH_CLOCK},
	
	{SPI2, GPIO('B',14), GPIO('B',15), GPIO('B',13), CRM_SPI2_PERIPH_CLOCK},
	
	{SPI3, GPIO('B',4), GPIO('B',5), GPIO('B',3), CRM_SPI3_PERIPH_CLOCK},
};


struct spi_config {
    
	void *spi;
    
	uint32_t cr0, cpsr;
};

struct spi_config spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
	struct spi_config ret = {0};

	uint8_t pol = SPI_CLOCK_POLARITY_LOW;

	uint8_t pha = SPI_CLOCK_PHASE_1EDGE;

	gpio_init_type gpio_initstructure;	

	SPI_TypeDef *spi = NULL;

	if(bus > sizeof(spi_infos)/sizeof(spi_infos[0]))
	{
		shutdown("spi bus slot error");
	}

	ret.spi = spi_infos[bus].spi;
	
	enable_pclock(spi_infos[bus].spi_clk);

	gpio_initstructure.gpio_out_type = GPIO_OUTPUT_PUSH;
	
	gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
	
	gpio_initstructure.gpio_mode = GPIO_MODE_MUX;

	gpio_peripheral(gpio_infos[bus].sck_pin, gpio_initstructure, 0);
	
	gpio_peripheral(gpio_infos[bus].mosi_pin, gpio_initstructure, 1);

	gpio_initstructure.gpio_mode = GPIO_MODE_INPUT;
	
	gpio_peripheral(gpio_infos[bus].miso_pin, gpio_initstructure, 1);

	spi_infos[bus].spi->ctrl1_bit.slben = 0;

    spi_infos[bus].spi->ctrl1_bit.slbtd = 0;

    spi_infos[bus].spi->ctrl1_bit.ora = 0;

	spi_infos[bus].spi->ctrl1_bit.swcsen = SPI_CS_SOFTWARE_MODE;

	spi_infos[bus].spi->ctrl1_bit.swcsil = 1;

	spi_infos[bus].spi->ctrl1_bit.msten = SPI_MODE_MASTER;

	spi_infos[bus].spi->ctrl2_bit.mdiv_h = 0;

    spi_infos[bus].spi->ctrl1_bit.mdiv_l = SPI_MCLK_DIV_8;

	spi_infos[bus].spi->ctrl1_bit.ltf = SPI_FIRST_BIT_LSB; //SPI_FIRST_BIT_MSB
  
	spi_infos[bus].spi->ctrl1_bit.fbn = SPI_FRAME_8BIT;

	switch(mode)
	{
		case 0:
				break;
		case 1:
				pha = SPI_CLOCK_PHASE_2EDGE;
				break;
		case 2:
				pol = SPI_CLOCK_POLARITY_HIGH; 
				break;
		case 3:
				pol = SPI_CLOCK_POLARITY_HIGH; 
				pha = SPI_CLOCK_PHASE_2EDGE;
				break;
		default :
				break;
	}
  
	spi_infos[bus].spi->ctrl1_bit.clkpol = pol;
  
	spi_infos[bus].spi->ctrl1_bit.clkpha = pha;

	spi_infos[bus].spi->ctrl1_bit.spien = 1;

	return ret;
}

void spi_prepare(struct spi_config config)
{

}

static int32_t
spi_flag_get(spi_type* spi, uint32_t spi_flag)
{
	int32_t status = 0;

	if ((spi->sts & spi_flag) == 0)
	{
		status = 0;
	}
	else
	{
    	status = 1;
	}

	return status;
}

void spi_transfer(struct spi_config config, uint8_t receive_data
                  , uint8_t len, uint8_t *data)
{
	spi_typedef *spi = config.spi;

	while(len--)
	{
		while(spi_flag_get(spi,SPI_I2S_TDBE_FLAG) == 0); 
		
		spi->dt = *data;

		if(receive_data)
		{
			while(spi_flag_get(spi, SPI_I2S_RDBF_FLAG) == 0);

    		*data = spi->dt;
		}

		data++;
	}

	return;
}
