#include <string.h>
#include "board/irq.h"
#include "command.h"
#include "gpio.h"
#include "internal.h"
#include "sched.h"

#if CONFIG_MACH_GD32F303XB
DECL_ENUMERATION_RANGE("pin","PA0",GPIO('A',0),16);
DECL_ENUMERATION_RANGE("pin","PB0",GPIO('B',0),16);
DECL_ENUMERATION_RANGE("pin","PC0",GPIO('C',0),16);
//DECL_ENUMERATION_RANGE("pin","PD0",GPIO('D',0),16);
//DECL_ENUMERATION_RANGE("pin","PE0",GPIO('E',0),16);
//DECL_ENUMERATION_RANGE("pin","PF0",GPIO('F',0),16);
//DECL_ENUMERATION_RANGE("pin","PG0",GPIO('G',0),16);
#elif CONFIG_MACH_GD32F303XE
DECL_ENUMERATION_RANGE("pin","PA0",GPIO('A',0),16);
DECL_ENUMERATION_RANGE("pin","PB0",GPIO('B',0),16);
DECL_ENUMERATION_RANGE("pin","PC0",GPIO('C',0),16);
DECL_ENUMERATION_RANGE("pin","PD0",GPIO('D',0),16);
//DECL_ENUMERATION_RANGE("pin","PE0",GPIO('E',0),16);
//DECL_ENUMERATION_RANGE("pin","PF0",GPIO('F',0),16);
//DECL_ENUMERATION_RANGE("pin","PG0",GPIO('G',0),16);
#endif

uint32_t gpio_port_base[] = {GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG};

uint32_t gpio_pclk[] = {RCU_GPIOA,RCU_GPIOB,RCU_GPIOC,RCU_GPIOD,RCU_GPIOE,RCU_GPIOF,RCU_GPIOG};

uint32_t
get_pclock_frequency(uint32_t periph_base)
{
	if(periph_base == 0)
	{
		return AHB_FREQ >> 1; 
	}
	else
	{
		return APB2_ADC_FREQ;
	}
}

uint32_t is_enable_pclock(uint32_t pclk)
{
	return RCU_REG_VAL(pclk) & BIT(RCU_BIT_POS(pclk));
}

void enable_pclock(uint32_t pclk)
{
	RCU_REG_VAL(pclk) |= BIT(RCU_BIT_POS(pclk));

	return;
}

/*
 * @gpio 
 * @otype 0=output;1=input;2=analog;3=AFIO
*/
void gpio_peripheral(uint32_t gpio, uint32_t otype, uint32_t pull_up)
{
	uint16_t i;

    uint32_t temp_mode = 0U;

    uint32_t reg = 0U;

	uint32_t speed = GPIO_OSPEED_50MHZ;

	uint32_t pinmode = 0;

	uint32_t port =  GPIO2PORT(gpio);
	
	uint16_t pin = GPIO2BIT(gpio);
		
	uint32_t gpio_periph = gpio_port_base[port];
	
	enable_pclock(gpio_pclk[port]);

	if(otype == 0)
	{
		if(pull_up == 1)
		{
			pinmode = GPIO_MODE_OUT_PP;
		}
		else
		{
			pinmode = GPIO_MODE_OUT_OD;
		}
	}
	else if(otype == 1)
	{
		if(pull_up == 0)
		{
			pinmode = GPIO_MODE_IPD;
		}
		else if(pull_up == 1)
		{
			pinmode = GPIO_MODE_IPU;	
		}
		else
		{
			pinmode = GPIO_MODE_IN_FLOATING;
		}
	}
	else if(otype == 2)
	{
		pinmode = GPIO_MODE_AIN;	
	}
	else if(otype == 3)
	{
		if(pull_up == 0)
		{
			pinmode = GPIO_MODE_AF_OD;	
		}
		else
		{
			pinmode = GPIO_MODE_AF_PP;	
		}
	}
	else
	{
		shutdown("gpio initialize fail");
	}

    /* GPIO mode configuration */
    temp_mode = (uint32_t)(pinmode & ((uint32_t)0x0FU));
    
    /* GPIO speed configuration */
    if(((uint32_t)0x00U) != ((uint32_t)pinmode & ((uint32_t)0x10U)))
	{
        /* output mode max speed */
        if(GPIO_OSPEED_MAX == (uint32_t)speed)
		{
            temp_mode |= (uint32_t)0x03U;
            /* set the corresponding SPD bit */
            GPIOx_SPD(gpio_periph) |= (uint32_t)pin ;
        }
		else
		{
            /* output mode max speed:10MHz,2MHz,50MHz */
            temp_mode |= (uint32_t)speed;
        }
    }

    /* configure the eight low port pins with GPIO_CTL0 */
    for(i = 0U;i < 8U;i++)
	{
        if((1U << i) & pin)
		{
            reg = GPIO_CTL0(gpio_periph);
            
            /* clear the specified pin mode bits */
            reg &= ~GPIO_MODE_MASK(i);
            
			/* set the specified pin mode bits */
            reg |= GPIO_MODE_SET(i, temp_mode);
            
            /* set IPD or IPU */
            if(GPIO_MODE_IPD == pinmode)
			{
                /* reset the corresponding OCTL bit */
                GPIO_BC(gpio_periph) = (uint32_t)((1U << i) & pin);
            }
			else
			{
                /* set the corresponding OCTL bit */
                if(GPIO_MODE_IPU == pinmode)
				{
                    GPIO_BOP(gpio_periph) = (uint32_t)((1U << i) & pin);
                }
            }
            /* set GPIO_CTL0 register */
            GPIO_CTL0(gpio_periph) = reg;
        }
    }

    /* configure the eight high port pins with GPIO_CTL1 */
    for(i = 8U;i < 16U;i++)
	{
        if((1U << i) & pin)
		{
            reg = GPIO_CTL1(gpio_periph);
            
            /* clear the specified pin mode bits */
            reg &= ~GPIO_MODE_MASK(i - 8U);
            
			/* set the specified pin mode bits */
            reg |= GPIO_MODE_SET(i - 8U, temp_mode);
            
            /* set IPD or IPU */
            if(GPIO_MODE_IPD == pinmode)
			{
                /* reset the corresponding OCTL bit */
                GPIO_BC(gpio_periph) = (uint32_t)((1U << i) & pin);
            }
			else
			{
                /* set the corresponding OCTL bit */
                if(GPIO_MODE_IPU == pinmode)
				{
                    GPIO_BOP(gpio_periph) = (uint32_t)((1U << i) & pin);
                }
            }

            /* set GPIO_CTL1 register */
            GPIO_CTL1(gpio_periph) = reg;
        }
	}
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
	uint32_t port = GPIO2PORT(g.pin);

	uint32_t pin = GPIO2BIT(g.pin);
		
	irqstatus_t flag = irq_save();

	gpio_peripheral(g.pin, 0, 1);

	if(!val)
	{
		GPIO_BC(gpio_port_base[port]) = (uint32_t)pin;
	}
	else
	{
		GPIO_BOP(gpio_port_base[port]) = (uint32_t)pin;
	}

	irq_restore(flag);

	return;
}

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
	if(GPIO2PORT(pin) >= sizeof(gpio_port_base) / sizeof(gpio_port_base[0]))
	{
		goto fail;
	}

	struct gpio_out g = {.pin = pin};

	gpio_out_reset(g,val);

	return g;
fail:
	shutdown("not an output pin");
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
	uint32_t port = GPIO2PORT(g.pin);

	uint32_t pin = GPIO2BIT(g.pin);
	
	if(GPIO_OCTL(gpio_port_base[port]) & (pin))
	{
		GPIO_BC(gpio_port_base[port]) = (uint32_t)pin;
	}
	else
	{
		GPIO_BOP(gpio_port_base[port]) = (uint32_t)pin;
	}	
	
	return;	
}

void 
gpio_out_toggle(struct gpio_out g)
{
	irqstatus_t flag = irq_save();
	
	gpio_out_toggle_noirq(g);

	irq_restore(flag);

	return;
}

void 
gpio_out_write(struct gpio_out g, uint8_t val)
{
	uint32_t port = GPIO2PORT(g.pin);

	uint32_t pin = GPIO2BIT(g.pin);
	
	if(!val)
	{
		GPIO_BC(gpio_port_base[port]) = (uint32_t)pin;
	}
	else
	{
		GPIO_BOP(gpio_port_base[port]) = (uint32_t)pin;
	}

	return;
}

struct gpio_in
gpio_in_setup(uint8_t pin, uint8_t pull_up)
{

	if(GPIO2PORT(pin) >= sizeof(gpio_port_base) / sizeof(gpio_port_base[0]))
	{
		goto fail;
	}

	struct gpio_in g = {.pin = pin};

	gpio_in_reset(g,pull_up);

	return g;
fail:
	shutdown("Not an input pin");
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
	irqstatus_t flag = irq_save();

	gpio_peripheral(g.pin, 1, pull_up);
	
	irq_restore(flag);
}

uint8_t
gpio_in_read(struct gpio_in g)
{
	uint32_t port = GPIO2PORT(g.pin);

	uint32_t pin = GPIO2BIT(g.pin);

	return (!!(GPIO_ISTAT(gpio_port_base[port]) & pin));
}


