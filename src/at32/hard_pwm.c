#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/irq.h" // irq_save
#include "command.h" // shutdown
#include "gpio.h" // gpio_pwm_write
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

#define MAX_PWM	255

DECL_CONSTANT("PWM_MAX",MAX_PWM);

struct gpio_pwm_info
{
	tmr_type* 	tmr_base;
	uint8_t   	chan;
	uint8_t		pin;
	uint32_t 	mux;
};

static struct gpio_pwm_info gpio_pwm_infos[] =
{
	{TMR1, TMR_SELECT_CHANNEL_1, GPIO('A',8),  0},
	{TMR1, TMR_SELECT_CHANNEL_2, GPIO('A',9),  0},
	{TMR1, TMR_SELECT_CHANNEL_3, GPIO('A',10), 0},
	{TMR1, TMR_SELECT_CHANNEL_4, GPIO('A',11), 0},
#ifndef CONFIG_MACH_AT32F415
	{TMR1, TMR_SELECT_CHANNEL_1, GPIO('E',9),  TMR1_MUX_11},
	{TMR1, TMR_SELECT_CHANNEL_2, GPIO('E',11), TMR1_MUX_11},
	{TMR1, TMR_SELECT_CHANNEL_3, GPIO('E',13), TMR1_MUX_11},
	{TMR1, TMR_SELECT_CHANNEL_4, GPIO('E',14), TMR1_MUX_11},
#endif	
	{TMR2, TMR_SELECT_CHANNEL_1, GPIO('A',0),  0},
	{TMR2, TMR_SELECT_CHANNEL_2, GPIO('A',1),  0},
	{TMR2, TMR_SELECT_CHANNEL_3, GPIO('A',2),  0},
	{TMR2, TMR_SELECT_CHANNEL_4, GPIO('A',3),  0},
	{TMR2, TMR_SELECT_CHANNEL_1, GPIO('A',15), TMR2_MUX_01},
	{TMR2, TMR_SELECT_CHANNEL_2, GPIO('B',3),  TMR2_MUX_01},
	{TMR2, TMR_SELECT_CHANNEL_3, GPIO('A',2),  TMR2_MUX_01},
	{TMR2, TMR_SELECT_CHANNEL_4, GPIO('A',3),  TMR2_MUX_01},
	{TMR2, TMR_SELECT_CHANNEL_3, GPIO('B',10), TMR2_MUX_10},
	{TMR2, TMR_SELECT_CHANNEL_4, GPIO('B',11), TMR2_MUX_10},
	{TMR3, TMR_SELECT_CHANNEL_1, GPIO('A',6),  0},
	{TMR3, TMR_SELECT_CHANNEL_2, GPIO('A',7),  0},
	{TMR3, TMR_SELECT_CHANNEL_3, GPIO('B',0),  0},
	{TMR3, TMR_SELECT_CHANNEL_4, GPIO('B',1),  0},
	{TMR3, TMR_SELECT_CHANNEL_1, GPIO('B',4),  TMR3_MUX_10},
	{TMR3, TMR_SELECT_CHANNEL_2, GPIO('B',5),  TMR3_MUX_10},
	{TMR3, TMR_SELECT_CHANNEL_1, GPIO('C',6),  TMR3_MUX_11},
	{TMR3, TMR_SELECT_CHANNEL_2, GPIO('C',7),  TMR3_MUX_11},
	{TMR3, TMR_SELECT_CHANNEL_3, GPIO('C',8),  TMR3_MUX_11},
	{TMR3, TMR_SELECT_CHANNEL_4, GPIO('C',9),  TMR3_MUX_11}
};

static void
tmr_output_channel_cfg(	tmr_type *tmr_x, 
						
						tmr_channel_select_type tmr_channel,
                        
						tmr_output_config_type *tmr_output_struct)
{
	uint16_t channel_index = 0;
   
	uint16_t channel_c_index = 0;
			
	uint16_t channel = 0;

	channel_index = (uint16_t)(tmr_output_struct->oc_idle_state << (8 + tmr_channel));

	channel_c_index = (uint16_t)(tmr_output_struct->occ_idle_state << (9 + tmr_channel));

  
	if((tmr_x == TMR1) 
#ifndef CONFIG_MACH_AT32F415	
		||	(tmr_x == TMR8)
#endif	
	)
	{
    	tmr_x->ctrl2 &= ~channel_c_index;
    	
		tmr_x->ctrl2 |= channel_c_index;
  	}

  	tmr_x->ctrl2 &= ~channel_index;
  
	tmr_x->ctrl2 |= channel_index;

  	channel = tmr_channel;

  	switch(channel)
  	{
    	case TMR_SELECT_CHANNEL_1:
      		
			tmr_x->cm1_output_bit.c1octrl = tmr_output_struct->oc_mode;
      		
			break;

    	case TMR_SELECT_CHANNEL_2:
      		
			tmr_x->cm1_output_bit.c2octrl = tmr_output_struct->oc_mode;
      		
			break;

    	case TMR_SELECT_CHANNEL_3:
      		
			tmr_x->cm2_output_bit.c3octrl = tmr_output_struct->oc_mode;
      		
			break;

    	case TMR_SELECT_CHANNEL_4:
      		
			tmr_x->cm2_output_bit.c4octrl = tmr_output_struct->oc_mode;
      		
			break;

    	default:
      		
			break;
  }

  	channel_index = (uint16_t)(tmr_output_struct->oc_polarity << ((tmr_channel * 2) + 1));

  	channel_c_index = (uint16_t)(tmr_output_struct->occ_polarity << ((tmr_channel * 2) + 3));

  	if((tmr_x == TMR1)
#ifndef CONFIG_MACH_AT32F415	
		|| (tmr_x == TMR8)
#endif
	)
  	{
    	tmr_x->cctrl &= ~channel_c_index;
    	
		tmr_x->cctrl |= channel_c_index;
  	}

  	tmr_x->cctrl &= ~channel_index;
  
	tmr_x->cctrl |= channel_index;

  	channel_index = (uint16_t)(tmr_output_struct->oc_output_state << (tmr_channel * 2));

  	channel_c_index = (uint16_t)(tmr_output_struct->occ_output_state << ((tmr_channel * 2) + 2));

  	if((tmr_x == TMR1)
#ifndef CONFIG_MACH_AT32F415	
		|| (tmr_x == TMR8)
#endif
	)
  	{
    	tmr_x->cctrl &= ~channel_c_index;
    	
		tmr_x->cctrl |= channel_c_index;
  	}

  	tmr_x->cctrl &= ~channel_index;
  	
	tmr_x->cctrl |= channel_index;
}

struct gpio_pwm
gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val) 
{
	struct gpio_pwm rt_pwm;
		
	struct gpio_pwm_info tmp_pwm_info = {.chan = 0};
	
	uint8_t idx = 0;

	uint32_t reg_addr;
	
	uint32_t remap_mask;
  
	uint8_t bit_offset;
			
	uint8_t bit_num;
	
	uint8_t bit_val;
	
	uint8_t size = sizeof(gpio_pwm_infos)/sizeof(gpio_pwm_infos[0]);
	
	uint16_t prescaler_value = (uint16_t)(CONFIG_CLOCK_FREQ / cycle_time) - 1;

	tmr_type* tmr_reg = 0;

	tmr_output_config_type tmr_oc_init_structure;

	for(idx = 0; idx < size; idx++)
	{
		if(pin == gpio_pwm_infos[idx].pin)
		{
			tmp_pwm_info = gpio_pwm_infos[idx];
			
			break;
		}
	}

	if(tmp_pwm_info.chan == 0)
	{
		//shutdown("Don't find timer channel");
	}
	
	gpio_init_type gpio_init_struct = GPIO_INIT_STRUCT_DEF;

	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;

	gpio_peripheral(tmp_pwm_info.pin, &gpio_init_struct, 2);

	//config mux
	
#if 1
	if(tmp_pwm_info.mux != 0)
	{
		enable_pclock(CRM_IOMUX_PERIPH_CLOCK);
  		
		reg_addr = IOMUX_BASE + (tmp_pwm_info.mux >> 24);
  
		bit_offset = (tmp_pwm_info.mux >> 16) & 0xFF;
  
		bit_num = (tmp_pwm_info.mux >> 8) & 0xFF;
  
		bit_val = tmp_pwm_info.mux & 0xFF;

  		remap_mask = 0xFFFFFFFF << (32 - bit_num - bit_offset);
  
		remap_mask = remap_mask >> (32 - bit_num - bit_offset);
  
		remap_mask = remap_mask >> bit_offset;
  
		remap_mask = remap_mask << bit_offset;

  		*(uint32_t*)reg_addr &= ~remap_mask;

    	*(uint32_t*)reg_addr |= (uint32_t)(bit_val << bit_offset);
	}
#endif
	
	tmr_reg = tmp_pwm_info.tmr_base;

	if(tmr_reg == TMR1)
	{
		enable_pclock(CRM_TMR1_PERIPH_CLOCK);
	}
	else if(tmr_reg == TMR2)
	{
		enable_pclock(CRM_TMR2_PERIPH_CLOCK);
	}
	else if(tmr_reg == TMR3)
	{
		enable_pclock(CRM_TMR3_PERIPH_CLOCK);
	}
	else
	{
		//add more TMRx
	}


	tmr_reg->pr = MAX_PWM - 1;

	tmr_reg->div = prescaler_value;

	tmr_reg->swevt_bit.ovfswtr = 1;

	tmr_reg->ctrl1_bit.cnt_dir = TMR_COUNT_UP;

	tmr_reg->ctrl1_bit.clkdiv = TMR_CLOCK_DIV2;

	tmr_oc_init_structure.occ_idle_state = FALSE;
  	
	tmr_oc_init_structure.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  
	tmr_oc_init_structure.occ_output_state = FALSE;
	
  	tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  
	tmr_oc_init_structure.oc_idle_state = FALSE;
  
	tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  
	tmr_oc_init_structure.oc_output_state = TRUE;

	tmr_output_channel_cfg(tmr_reg,tmp_pwm_info.chan,&tmr_oc_init_structure);

	switch(tmp_pwm_info.chan)
	{
		case TMR_SELECT_CHANNEL_1:
			
			tmr_reg->c1dt = val;
			
			tmr_reg->cm1_output_bit.c1oben = 1;

			break;
		
		case TMR_SELECT_CHANNEL_2:
			
			tmr_reg->c2dt = val;
			
			tmr_reg->cm1_output_bit.c1oben = 1;

			break;
		
		case TMR_SELECT_CHANNEL_3:
			
			tmr_reg->c3dt = val;
			
			tmr_reg->cm1_output_bit.c1oben = 1;

			break;
		
		case TMR_SELECT_CHANNEL_4:
			
			tmr_reg->c4dt = val;

			tmr_reg->cm1_output_bit.c1oben = 1;
			
			break;

		default:
			
			break;
	}
	
	tmr_reg->ctrl1_bit.prben = 1;

	tmr_reg->ctrl1_bit.tmren = 1;

	rt_pwm.regs = tmr_reg;

	rt_pwm.chan = tmp_pwm_info.chan;

	return rt_pwm;
}

void
gpio_pwm_write(struct gpio_pwm g, uint32_t val) 
{
	tmr_type* regs = (tmr_type*)g.regs;

	switch(g.chan)
  	{
    	case TMR_SELECT_CHANNEL_1:
      		
			regs->c1dt = val;
      		
			break;

    	case TMR_SELECT_CHANNEL_2:
      
			regs->c2dt = val;
      
			break;

    	case TMR_SELECT_CHANNEL_3:
      		
			regs->c3dt = val;
      
			break;

    	case TMR_SELECT_CHANNEL_4:
      
			regs->c4dt = val;
      
			break;
    
		default:
      		
			break;
  }
}
