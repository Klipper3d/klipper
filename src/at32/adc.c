#include "board/irq.h" // irq_save
#include "board/misc.h" // timer_from_us
#include "command.h" // shutdown
#include "compiler.h" // ARRAY_SIZE
#include "generic/armcm_timer.h" // udelay
#include "gpio.h" // gpio_adc_setup
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

DECL_CONSTANT("ADC_MAX",4095);

static const uint8_t adc_pin[] = {
	GPIO('A',0),GPIO('A',1),GPIO('A',2),GPIO('A',3),
	GPIO('A',4),GPIO('A',5),GPIO('A',6),GPIO('A',7),
	GPIO('B',0),GPIO('B',1),GPIO('C',0),GPIO('C',1),
	GPIO('C',2),GPIO('C',3),GPIO('C',4),GPIO('C',5)
};

extern gpio_type *gpio_port_base[];
  
extern uint32_t gpio_pclk[];

static void
adc_ordinary_channel_cfg(adc_type *adc_x, adc_channel_select_type adc_channel, uint8_t adc_sequence, adc_sampletime_select_type adc_sampletime)
{
  switch(adc_channel)
  {
    case ADC_CHANNEL_0:
      adc_x->spt2_bit.cspt0 = adc_sampletime;
      break;
    case ADC_CHANNEL_1:
      adc_x->spt2_bit.cspt1 = adc_sampletime;
      break;
    case ADC_CHANNEL_2:
      adc_x->spt2_bit.cspt2 = adc_sampletime;
      break;
    case ADC_CHANNEL_3:
      adc_x->spt2_bit.cspt3 = adc_sampletime;
      break;
    case ADC_CHANNEL_4:
      adc_x->spt2_bit.cspt4 = adc_sampletime;
      break;
    case ADC_CHANNEL_5:
      adc_x->spt2_bit.cspt5 = adc_sampletime;
      break;
    case ADC_CHANNEL_6:
      adc_x->spt2_bit.cspt6 = adc_sampletime;
      break;
    case ADC_CHANNEL_7:
      adc_x->spt2_bit.cspt7 = adc_sampletime;
      break;
    case ADC_CHANNEL_8:
      adc_x->spt2_bit.cspt8 = adc_sampletime;
      break;
    case ADC_CHANNEL_9:
      adc_x->spt2_bit.cspt9 = adc_sampletime;
      break;
    case ADC_CHANNEL_10:
      adc_x->spt1_bit.cspt10 = adc_sampletime;
      break;
    case ADC_CHANNEL_11:
      adc_x->spt1_bit.cspt11 = adc_sampletime;
      break;
    case ADC_CHANNEL_12:
      adc_x->spt1_bit.cspt12 = adc_sampletime;
      break;
    case ADC_CHANNEL_13:
      adc_x->spt1_bit.cspt13 = adc_sampletime;
      break;
    case ADC_CHANNEL_14:
      adc_x->spt1_bit.cspt14 = adc_sampletime;
      break;
    case ADC_CHANNEL_15:
      adc_x->spt1_bit.cspt15 = adc_sampletime;
      break;
    case ADC_CHANNEL_16:
      adc_x->spt1_bit.cspt16 = adc_sampletime;
      break;
    case ADC_CHANNEL_17:
      adc_x->spt1_bit.cspt17 = adc_sampletime;
      break;
    default:
      break;
  }
  switch(adc_sequence)
  {
    case 1:
      adc_x->osq3_bit.osn1 = adc_channel;
      break;
    case 2:
      adc_x->osq3_bit.osn2 = adc_channel;
      break;
    case 3:
      adc_x->osq3_bit.osn3 = adc_channel;
      break;
    case 4:
      adc_x->osq3_bit.osn4 = adc_channel;
      break;
    case 5:
      adc_x->osq3_bit.osn5 = adc_channel;
      break;
    case 6:
      adc_x->osq3_bit.osn6 = adc_channel;
      break;
    case 7:
      adc_x->osq2_bit.osn7 = adc_channel;
      break;
    case 8:
      adc_x->osq2_bit.osn8 = adc_channel;
      break;
    case 9:
      adc_x->osq2_bit.osn9 = adc_channel;
      break;
    case 10:
      adc_x->osq2_bit.osn10 = adc_channel;
      break;
    case 11:
      adc_x->osq2_bit.osn11 = adc_channel;
      break;
    case 12:
      adc_x->osq2_bit.osn12 = adc_channel;
      break;
    case 13:
      adc_x->osq1_bit.osn13 = adc_channel;
      break;
    case 14:
      adc_x->osq1_bit.osn14 = adc_channel;
      break;
    case 15:
      adc_x->osq1_bit.osn15 = adc_channel;
      break;
    case 16:
      adc_x->osq1_bit.osn16 = adc_channel;
      break;
    default:
      break;
  }
}

struct gpio_adc gpio_adc_setup(uint8_t pin)
{
	int chan;
	
	struct gpio_adc gpio_adc_rt;

	for(chan = 0; ; chan++)
	{
		if(chan >= sizeof(adc_pin)/sizeof(adc_pin[0]))
		{
			shutdown("Not a valid ADC pin");
		}

		if(CONFIG_AT32_SERIAL_USART2_PA2_PA3)
		{
			if((pin == GPIO('A',2)) || (pin == GPIO('A',3)))
			{
				shutdown("Not a valid ADC pin");
			}
		}

		if(adc_pin[chan] == pin)
		{
			break;
		}
	}

	enable_pclock(gpio_pclk[GPIO2PORT(pin)]);
	
	enable_pclock(CRM_ADC1_PERIPH_CLOCK);
 
	CRM->cfg_bit.adcdiv_l = (CRM_ADC_DIV_6 & 0x03);
  	
	CRM->cfg_bit.adcdiv_h = ((CRM_ADC_DIV_6 >> 2) & 0x01);

	gpio_init_type gpio_init_struct;

	gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;

	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;

	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;

	gpio_peripheral(pin, &gpio_init_struct, 2); 

	ADC1->ctrl1_bit.sqen = 1;

	ADC1->ctrl2_bit.rpen = 0;

	ADC1->ctrl2_bit.dtalign = ADC_RIGHT_ALIGNMENT;

	ADC1->osq1_bit.oclen = 0;

	adc_ordinary_channel_cfg(ADC1, chan, 1, ADC_SAMPLETIME_239_5);

	ADC1->ctrl2_bit.octesel_h = 0;

	ADC1->ctrl2_bit.octesel_l = ADC12_ORDINARY_TRIG_SOFTWARE; //software trigge

	ADC1->ctrl2_bit.octen = 1;

	ADC1->ctrl2_bit.adcen = 1; //enable adc1

	ADC1->ctrl2_bit.adcalinit = 1; //init calibration

	while(ADC1->ctrl2_bit.adcalinit == 1){} //wait init cmplt
	
	ADC1->ctrl2_bit.adcal = 1; //start calibaration

	while(ADC1->ctrl2_bit.adcal == 1){}	//wait start cmplt

	ADC1->ctrl2_bit.ocswtrg = 1;

	gpio_adc_rt.regs = ADC1;

	gpio_adc_rt.chan = chan;

	return gpio_adc_rt;
}


uint32_t 
gpio_adc_sample(struct gpio_adc g)
{
	adc_type* adc = (adc_type*) g.regs;

	if( adc->sts & ADC_OCCS_FLAG)
	{
		if(adc->sts & ADC_CCE_FLAG)
		{
			return 0;
		}
			goto need_delay;
	}

	adc->ctrl2_bit.ocswtrg = 1;

need_delay:

	return timer_from_us(20);
}

uint16_t
gpio_adc_read(struct gpio_adc g)
{
	adc_type* adc = (adc_type*)g.regs;

	adc->sts = ~(ADC_CCE_FLAG | ADC_OCCS_FLAG);

	return adc->odt_bit.odt;
}

void
gpio_adc_cancel_sample(struct gpio_adc g)
{
	adc_type* adc = (adc_type*)g.regs;

	irqstatus_t flag = irq_save();

	if(adc->sts_bit.occs & ADC_OCCS_FLAG)
	{
		gpio_adc_read(g);
	}

	irq_restore(flag);
}
