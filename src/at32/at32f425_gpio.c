#include <string.h>
#include "board/irq.h"
#include "command.h"
#include "gpio.h"
#include "internal.h"
#include "sched.h"

DECL_ENUMERATION_RANGE("pin","PA0",GPIO('A',0),16);
DECL_ENUMERATION_RANGE("pin","PB0",GPIO('B',0),16);
DECL_ENUMERATION_RANGE("pin","PC0",GPIO('C',0),16);
DECL_ENUMERATION_RANGE("pin","PD0",GPIO('D',0),16);
DECL_ENUMERATION_RANGE("pin","PF0",GPIO('F',0),16);

#define PERIPHERAL_CLOCK	(CONFIG_CLOCK_FREQ)

#define GPIOE 				((gpio_type *)0)

#define PCLK_GPIOE 			(0)

gpio_type *gpio_port_base[] = {GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF};

uint32_t gpio_pclk[] = {PCLK_GPIOA,PCLK_GPIOB,PCLK_GPIOC,PCLK_GPIOD,PCLK_GPIOE,PCLK_GPIOF};


uint32_t get_pclock_frequency(uint32_t periph_base)
{
	return PERIPHERAL_CLOCK;
}

uint32_t is_enable_pclock(uint32_t pclk)
{
	return !!(CRM_REG(pclk) & CRM_REG_BIT(pclk));
}

void enable_pclock(uint32_t pclk)
{
	CRM_REG(pclk) |= CRM_REG_BIT(pclk);
	return;
}

void gpio_mux_cfg(uint32_t gpio, gpio_pins_source_type gpio_pin_source, gpio_mux_sel_type gpio_mux)
{
	gpio_type* gpio_x = gpio_port_base[GPIO2PORT(gpio)];

    uint32_t temp = 0x00;
    uint32_t temp_2 = 0x00;
  
    temp = ((uint32_t)(gpio_mux) << ((uint32_t)((uint32_t)gpio_pin_source & (uint32_t)0x07) * 4));
    
	if(gpio_pin_source >> 0x03)
    {
      gpio_x->muxh &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)gpio_pin_source & (uint32_t)0x07) * 4));
      
	  temp_2 = gpio_x->muxh | temp;
      
	  gpio_x->muxh = temp_2;
    }
    else
    {
      gpio_x->muxl &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)gpio_pin_source & (uint32_t)0x07) * 4));
      
	  temp_2 = gpio_x->muxl | temp;
      
	  gpio_x->muxl = temp_2;
    }
	
	return;
}

void gpio_peripheral(uint32_t gpio, gpio_init_type* gpio_init_struct, uint32_t pullup)
{
	gpio_type* gpio_x = gpio_port_base[GPIO2PORT(gpio)];
	
	uint16_t pinx_value = GPIO2BIT(gpio);
	
	uint16_t pin_index = 0;
	
	enable_pclock(gpio_pclk[GPIO2PORT(gpio)]);
	
    while(pinx_value > 0)
    {
        if(pinx_value & 0x01)
        {
            gpio_x->cfgr  &= (uint32_t)~(0x03 << (pin_index * 2));
            
			gpio_x->cfgr  |= (uint32_t)(gpio_init_struct->gpio_mode << (pin_index * 2));
      
            gpio_x->omode &= (uint32_t)~(0x01 << (pin_index));
            
			gpio_x->omode |= (uint32_t)(gpio_init_struct->gpio_out_type << (pin_index));
      
            gpio_x->odrvr &= (uint32_t)~(0x03 << (pin_index * 2));
            
			gpio_x->odrvr |= (uint32_t)(gpio_init_struct->gpio_drive_strength << (pin_index * 2));
      
            gpio_x->pull  &= (uint32_t)~(0x03 << (pin_index * 2));
            
			gpio_x->pull  |= (uint32_t)(gpio_init_struct->gpio_pull << (pin_index * 2));
        }

        pinx_value >>= 1;
        
		pin_index++;
    }
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
	gpio_init_type gpio_init_struct = GPIO_INIT_STRUCT_DEF;

	gpio_type *gpio_regs = (gpio_type*)g.regs;

	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;

	irqstatus_t flag = irq_save();

	gpio_peripheral(g.pin, &gpio_init_struct, 2);

	if(!val)
	{
		gpio_regs->clr |= GPIO2BIT(g.pin);
	}
	else
	{
		gpio_regs->scr |= GPIO2BIT(g.pin);
	}

	irq_restore(flag);

	return;
}

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
	if(GPIO2PORT(pin) >= sizeof(gpio_port_base) / sizeof(gpio_port_base[0]))
		goto fail;
	
	struct gpio_out g = {.pin = pin, .regs = gpio_port_base[GPIO2PORT(pin)]};

	gpio_out_reset(g,val);

	return g;
fail:
	shutdown("Not an output pin");
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
	gpio_type *gpio_regs = (gpio_type*)g.regs;

	gpio_regs->odt ^= GPIO2BIT(g.pin);

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
	gpio_type *gpio_regs = (gpio_type*)g.regs;
	if(!val)
	{
		gpio_regs->clr |= GPIO2BIT(g.pin);
	}
	else
	{
		gpio_regs->scr |= GPIO2BIT(g.pin);
	}
}

struct gpio_in
gpio_in_setup(uint8_t pin, uint8_t pull_up)
{

	if(GPIO2PORT(pin) >= sizeof(gpio_port_base) / sizeof(gpio_port_base[0]))
		goto fail;
	
	struct gpio_in g = {.pin = pin, .regs = gpio_port_base[GPIO2PORT(pin)]};

	gpio_in_reset(g,pull_up);

	return g;
fail:
	shutdown("Not an input pin");
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
	
	gpio_init_type gpio_init_struct = GPIO_INIT_STRUCT_DEF; 

	gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;

	irqstatus_t flag = irq_save();

	gpio_peripheral(g.pin,&gpio_init_struct, pull_up);
	
	irq_restore(flag);
}

uint8_t
gpio_in_read(struct gpio_in g)
{
	gpio_type* gpio_regs = (gpio_type*)g.regs;

	return(!!(gpio_regs->idt & GPIO2BIT(g.pin)));
}

