#include <string.h>
#include "board/irq.h"
#include "command.h"
#include "gpio.h"
#include "internal.h"
#include "sched.h"

DECL_ENUMERATION_RANGE("pin","PA0",GPIO('A',0),16);
DECL_ENUMERATION_RANGE("pin","PB0",GPIO('B',0),16);
DECL_ENUMERATION_RANGE("pin","PC0",GPIO('C',0),16);

#if !defined(AT32F421F6P7)
DECL_ENUMERATION_RANGE("pin","PD0",GPIO('D',0),16);
#endif

#if (!defined(AT32F415CCU7) && !defined(AT32F425F8P7) && !defined(AT32F421F6P7))
DECL_ENUMERATION_RANGE("pin","PE0",GPIO('E',0),16);
#endif

DECL_ENUMERATION_RANGE("pin","PF0",GPIO('F',0),16);

#define PERIPHERAL_CLOCK	(CONFIG_CLOCK_FREQ / 2)


gpio_type *gpio_port_base[] = 
{
		GPIOA,
		GPIOB,
		GPIOC,
#if !defined(AT32F421F6P7)
		GPIOD,
#endif

#if (!defined(AT32F415CCU7) && !defined(AT32F425F8P7) && !defined(AT32F421F6P7))
		GPIOE,
#endif
		GPIOF
};

uint32_t gpio_pclk[] = 
{
		PCLK_GPIOA,
		PCLK_GPIOB,
		PCLK_GPIOC,
#if !defined(AT32F421F6P7)
		PCLK_GPIOD,
#endif

#if (!defined(AT32F415CCU7) && !defined(AT32F425F8P7) && !defined(AT32F421F6P7))
		PCLK_GPIOE,
#endif
		PCLK_GPIOF
};


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

void gpio_peripheral(uint32_t gpio, gpio_init_type* gpio_init_struct, uint32_t pullup)
{
	gpio_type* gpio_x = gpio_port_base[GPIO2PORT(gpio)];
	
	uint32_t temp;
	
	uint16_t pinx_value;
	
	uint16_t pin_index = GPIO2BIT(gpio);
	
	enable_pclock(gpio_pclk[GPIO2PORT(gpio)]);
	
	if(1 == pullup)
	{
		gpio_init_struct->gpio_pull = GPIO_PULL_UP;
	}
	else if(0 == pullup)
	{
		gpio_init_struct->gpio_pull = GPIO_PULL_DOWN;
	}
	else
	{
		gpio_init_struct->gpio_pull = GPIO_PULL_NONE;
	}
#if (AT32F421F6P7)
	
  pinx_value = (uint16_t)pin_index;

	pin_index = 0;

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
#else
	if(gpio_init_struct->gpio_mode == GPIO_MODE_ANALOG)
	{
		pinx_value = 0x00;
	}
	else if(gpio_init_struct->gpio_mode == GPIO_MODE_INPUT)
	{
		pinx_value = gpio_init_struct->gpio_pull & 0x0F;

	   	if(gpio_init_struct->gpio_pull == GPIO_PULL_UP)
	   	 {
	     		 gpio_x->scr = pin_index;
	   	 }
	   	 else if(gpio_init_struct->gpio_pull == GPIO_PULL_DOWN)
	   	 {
	     		 gpio_x->clr = pin_index;
	   	 }
	}
	else
	{
		pinx_value = (gpio_init_struct->gpio_mode & 0x08) | (gpio_init_struct->gpio_out_type & 0x04) | \
		(gpio_init_struct->gpio_drive_strength & 0x03);
       	}

		/* pin 0~7 config */
		if(((uint32_t)pin_index & ((uint32_t)0x00FF)) != 0x00)
		{
			for(temp = 0; temp < 0x08; temp++)
	   		{
	     		if((1 << temp) & pin_index)
	     		{
				gpio_x->cfglr &= (uint32_t)~(0x0F << (temp * 4));
	
				gpio_x->cfglr |= (uint32_t)(pinx_value << (temp * 4));
	     		}
	    	}
		}

		/* pin 8~15 config */
		if(pin_index > 0x00ff)
		{
			pin_index = pin_index >> 8;
	
			for(temp = 0; temp < 0x8; temp++)
	       	{
	     		if((1 << temp) & pin_index)
	      		{
				gpio_x->cfghr &= (uint32_t)~(0xf << (temp * 4));
	
				gpio_x->cfghr |= (uint32_t)(pinx_value << (temp * 4));
	      		}
	    	}
	  	}
#endif	
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

void at32f403a_407_gpio_remap(uint32_t gpio_remap, uint8_t new_state)
{
	uint32_t reg_addr, remap_mask;

	uint8_t bit_offset, bit_num, bit_val;

	/* get register address, bit offset, bit number and remap value */
   	reg_addr = IOMUX_BASE + (gpio_remap >> 24);

	bit_offset = (gpio_remap >> 16) & 0xFF;

	bit_num = (gpio_remap >> 8) & 0xFF;

	bit_val = gpio_remap & 0xFF;

	/* get remap mask value */

	remap_mask = 0xFFFFFFFF << (32 - bit_num - bit_offset);
   
	remap_mask = remap_mask >> (32 - bit_num - bit_offset);

  	remap_mask = remap_mask >> bit_offset;
  
	remap_mask = remap_mask << bit_offset;

 
   	/* clear remap value */
 
   	*(uint32_t*)reg_addr &= ~remap_mask;

   	if(new_state != 0)
   	{
		/* set remap value */
		*(uint32_t*)reg_addr |= (uint32_t)(bit_val << bit_offset);
   	}
}
