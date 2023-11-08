#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/irq.h" // irq_save
#include "command.h" // shutdown
#include "gpio.h" // gpio_pwm_write
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

#define MAX_PWM 	65536	

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
	{TMR1, TMR_SELECT_CHANNEL_1, GPIO('F',0),  GPIO_MUX_0},
	{TMR1, TMR_SELECT_CHANNEL_2C, GPIO('F',1),  GPIO_MUX_0},
	{TMR1, TMR_SELECT_CHANNEL_1C, GPIO('A',7), GPIO_MUX_3},
	{TMR1, TMR_SELECT_CHANNEL_3C, GPIO('B',1),  GPIO_MUX_2},
	{TMR1, TMR_SELECT_CHANNEL_2, GPIO('A',9), GPIO_MUX_2},
	{TMR1, TMR_SELECT_CHANNEL_3, GPIO('A',10), GPIO_MUX_2},
	{TMR1, TMR_SELECT_CHANNEL_4, GPIO('A',11), GPIO_MUX_2},
	{TMR2, TMR_SELECT_CHANNEL_1, GPIO('A',0), GPIO_MUX_2},
	{TMR2, TMR_SELECT_CHANNEL_2, GPIO('A',1), GPIO_MUX_2},
	{TMR2, TMR_SELECT_CHANNEL_3, GPIO('A',2), GPIO_MUX_2},
	{TMR2, TMR_SELECT_CHANNEL_4, GPIO('A',3), GPIO_MUX_2},
	{TMR2, TMR_SELECT_CHANNEL_1, GPIO('A',5), GPIO_MUX_2},
	{TMR3, TMR_SELECT_CHANNEL_1, GPIO('A',6), GPIO_MUX_2},
	//{TMR3, TMR_SELECT_CHANNEL_2, GPIO('A',7), GPIO_MUX_2},
	//{TMR3, TMR_SELECT_CHANNEL_4, GPIO('B',1), GPIO_MUX_1},
	//{TMR13, TMR_SELECT_CHANNEL_1, GPIO('A',6), GPIO_MUX_8},
	{TMR14, TMR_SELECT_CHANNEL_1, GPIO('A',4), GPIO_MUX_6},
	//{TMR14, TMR_SELECT_CHANNEL_1, GPIO('B',1), GPIO_MUX_0},
	//{TMR15, TMR_SELECT_CHANNEL_1C, GPIO('A',1), GPIO_MUX_5},
	//{TMR15, TMR_SELECT_CHANNEL_1, GPIO('A',2), GPIO_MUX_0},
	//{TMR15, TMR_SELECT_CHANNEL_2, GPIO('A',3), GPIO_MUX_0},
	//{TMR16, TMR_SELECT_CHANNEL_1, GPIO('A',6), GPIO_MUX_6},
	//{TMR17, TMR_SELECT_CHANNEL_1, GPIO('A',7), GPIO_MUX_6},
};

static void
tmr_output_channel_cfg(	tmr_type *tmr_x, 
						
						tmr_channel_select_type tmr_channel,
                        
						tmr_output_config_type *tmr_output_struct)
{
	uint16_t channel_index = 0, channel_c_index = 0, channel = 0, chx_offset, chcx_offset;

    chx_offset = (8 + tmr_channel);
    chcx_offset = (9 + tmr_channel);
    
    /* get channel idle state bit position in ctrl2 register */
    channel_index = (uint16_t)(tmr_output_struct->oc_idle_state << chx_offset);
  
    /* get channel complementary idle state bit position in ctrl2 register */
    channel_c_index = (uint16_t)(tmr_output_struct->occ_idle_state << chcx_offset);
  
    /* set output channel complementary idle state */
    tmr_x->ctrl2 &= ~(1<<chcx_offset);
    tmr_x->ctrl2 |= channel_c_index;
  
    /* set output channel idle state */
    tmr_x->ctrl2 &= ~(1<<chx_offset);
    tmr_x->ctrl2 |= channel_index;
  
    /* set channel output mode */
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
  
    chx_offset = ((tmr_channel * 2) + 1);
    chcx_offset = ((tmr_channel * 2) + 3);
  
    /* get channel polarity bit position in cctrl register */
    channel_index = (uint16_t)(tmr_output_struct->oc_polarity << chx_offset);
  
    /* get channel complementary polarity bit position in cctrl register */
    channel_c_index = (uint16_t)(tmr_output_struct->occ_polarity << chcx_offset);
  
    /* set output channel complementary polarity */
    tmr_x->cctrl &= ~(1<<chcx_offset);
    tmr_x->cctrl |= channel_c_index;
  
    /* set output channel polarity */
    tmr_x->cctrl &= ~(1<<chx_offset);
    tmr_x->cctrl |= channel_index;
  
    chx_offset = (tmr_channel * 2);
    chcx_offset = ((tmr_channel * 2) + 2);
  
    /* get channel enable bit position in cctrl register */
    channel_index = (uint16_t)(tmr_output_struct->oc_output_state << (tmr_channel * 2));
  
    /* get channel complementary enable bit position in cctrl register */
    channel_c_index = (uint16_t)(tmr_output_struct->occ_output_state << ((tmr_channel * 2) + 2));
  
    /* set output channel complementary enable bit */
    tmr_x->cctrl &= ~(1<<chcx_offset);
    tmr_x->cctrl |= channel_c_index;
  
    /* set output channel enable bit */
    tmr_x->cctrl &= ~(1<<chx_offset);
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
	gpio_mux_cfg(tmp_pwm_info.pin, (gpio_pins_source_type)(tmp_pwm_info.pin % 16) , (gpio_mux_sel_type)(tmp_pwm_info.mux));
	
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
	else if(tmr_reg == TMR14)
	{
		enable_pclock(CRM_TMR14_PERIPH_CLOCK);
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
			
			break;
		
		case TMR_SELECT_CHANNEL_2:
			
			tmr_reg->c2dt = val;

			break;
		
		case TMR_SELECT_CHANNEL_3:
			
			tmr_reg->c3dt = val;

			break;
		
		case TMR_SELECT_CHANNEL_4:
			
			tmr_reg->c4dt = val;
			
			break;

		default:
			
			break;
	}

	tmr_reg->brk_bit.oen = 1;

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
