#include "board/irq.h" // irq_save
#include "board/misc.h" // timer_from_us
#include "command.h" // shutdown
#include "compiler.h" // ARRAY_SIZE
#include "generic/armcm_timer.h" // udelay
#include "gpio.h" // gpio_adc_setup
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

DECL_CONSTANT("ADC_MAX",4095);
#define ADC_TEMPERATURE_PIN 0xfe
DECL_ENUMERATION("pin", "ADC_TEMPERATURE", ADC_TEMPERATURE_PIN);
#if CONFIG_MACH_GD32E230X8
/* 
static const uint8_t adc_pin[] = {
	GPIO('A',0),GPIO('A',1),GPIO('A',2),GPIO('A',3),
	GPIO('A',4),GPIO('A',5),GPIO('A',6),GPIO('A',7),
	0xff,GPIO('B',1)};
*/

static const uint8_t adc_pin[] = {
    GPIO('A',0),GPIO('A',1),GPIO('A',2),GPIO('A',3),
    GPIO('A',4),GPIO('A',5),GPIO('A',6),GPIO('A',7),
    GPIO('B',0),GPIO('B',1),GPIO('C',0),GPIO('C',1),
    GPIO('C',2),GPIO('C',3),GPIO('C',4),GPIO('C',5),
    ADC_TEMPERATURE_PIN
};


#define _ADC_STAT 	ADC_STAT
#define _ADC_CTL0	ADC_CTL0
#define _ADC_CTL1	ADC_CTL1
#define _ADC_SAMPT0	ADC_SAMPT0
#define _ADC_SAMPT1	ADC_SAMPT1
#define _ADC_IOFF0	ADC_IOFF0
#define _ADC_IOFF1	ADC_IOFF1
#define _ADC_IOFF2	ADC_IOFF2
#define _ADC_IOFF3	ADC_IOFF3
#define _ADC_WDHT	ADC_WDHT
#define _ADC_WDLT	ADC_WDLT
#define _ADC_RSQ0	ADC_RSQ0
#define _ADC_RSQ1	ADC_RSQ1
#define _ADC_RSQ2	ADC_RSQ2
#define _ADC_ISQ	ADC_ISQ
#define _ADC_IDATA0	ADC_IDATA0
#define _ADC_IDATA1	ADC_IDATA1
#define _ADC_IDATA2	ADC_IDATA2
#define _ADC_IDATA3	ADC_IDATA3
#define _ADC_RDATA	ADC_RDATA
#define _ADC_OVSAMPCTL	ADC_OVSAMPCTL

#elif CONFIG_MACH_GD32F303XX
static const uint8_t adc_pin[] = {
	GPIO('A',0),GPIO('A',1),GPIO('A',2),GPIO('A',3),
	GPIO('A',4),GPIO('A',5),GPIO('A',6),GPIO('A',7),
	GPIO('B',0),GPIO('B',1),GPIO('C',0),GPIO('C',1),
	GPIO('C',2),GPIO('C',3),GPIO('C',4),GPIO('C',5),
	ADC_TEMPERATURE_PIN
};

#define ADCx		ADC0
#define _ADC_STAT 	ADC_STAT(ADCx)
#define _ADC_CTL0	ADC_CTL0(ADCx)
#define _ADC_CTL1	ADC_CTL1(ADCx)
#define _ADC_SAMPT0	ADC_SAMPT0(ADCx)
#define _ADC_SAMPT1	ADC_SAMPT1(ADCx)
#define _ADC_IOFF0	ADC_IOFF0(ADCx)
#define _ADC_IOFF1	ADC_IOFF1(ADCx)
#define _ADC_IOFF2	ADC_IOFF2(ADCx)
#define _ADC_IOFF3	ADC_IOFF3(ADCx)
#define _ADC_WDHT	ADC_WDHT(ADCx)
#define _ADC_WDLT	ADC_WDLT(ADCx)
#define _ADC_RSQ0	ADC_RSQ0(ADCx)
#define _ADC_RSQ1	ADC_RSQ1(ADCx)
#define _ADC_RSQ2	ADC_RSQ2(ADCx)
#define _ADC_ISQ	ADC_ISQ(ADCx)
#define _ADC_IDATA0	ADC_IDATA0(ADCx)
#define _ADC_IDATA1	ADC_IDATA1(ADCx)
#define _ADC_IDATA2	ADC_IDATA2(ADCx)
#define _ADC_IDATA3	ADC_IDATA3(ADCx)
#define _ADC_RDATA	ADC_RDATA(ADCx)
#define _ADC_OVSAMPCTL	ADC_OVSAMPCTL(ADCx)

#define ADC_EXTTRIG_REGULAR_NONE	ADC0_1_2_EXTTRIG_REGULAR_NONE
#define RCU_ADC 					RCU_ADC0
#define RCU_ADC_PSC_OFFSET          ((uint32_t)14U)
#else
#error "Invalid MCU type"
#endif


extern uint32_t gpio_port_base[];
  
extern uint32_t gpio_pclk[];

static void
adcClockConfig(void)
{
#if CONFIG_MACH_GD32E230X8

	RCU_CFG0 &= ~RCU_CFG0_ADCPSC;

    RCU_CFG2 &= ~(RCU_CFG2_ADCSEL | RCU_CFG2_IRC28MDIV | RCU_CFG2_ADCPSC2);

	RCU_CFG0 |= RCU_ADC_CKAPB2_DIV6;
        
	RCU_CFG2 |= RCU_CFG2_ADCSEL;

#elif CONFIG_MACH_GD32F303XX

	uint32_t reg0 = RCU_CFG0;
	
	uint32_t reg1 = RCU_CFG1;
	
	reg0 &= ~(RCU_CFG0_ADCPSC_2 | RCU_CFG0_ADCPSC);

	reg0 |= (RCU_CKADC_CKAPB2_DIV6 << RCU_ADC_PSC_OFFSET);

	reg1 &= ~RCU_CFG1_ADCPSC_3;

	RCU_CFG0 = reg0;

	RCU_CFG1 = reg1;

#endif
}


static void
adcRegularChannelConfig(uint8_t rank, uint8_t channel, uint32_t sample_time)
{
	uint32_t rsq,sampt;
    
    /* configure ADC regular sequence */
    if(rank < 6U)
	{
        rsq = _ADC_RSQ2;
        
		rsq &=  ~((uint32_t)(ADC_RSQX_RSQN << (5U*rank)));
        
		rsq |= ((uint32_t)channel << (5U*rank));
        
		_ADC_RSQ2 = rsq;
	}
	else if(rank < 12U)
	{
        rsq = _ADC_RSQ1;
        
		rsq &= ~((uint32_t)(ADC_RSQX_RSQN << (5U*(rank-6U))));
        
		rsq |= ((uint32_t)channel << (5U*(rank-6U)));
        
		_ADC_RSQ1 = rsq;
    }
	else if(rank < 16U)
	{
        rsq = _ADC_RSQ0;
        
		rsq &= ~((uint32_t)(ADC_RSQX_RSQN << (5U*(rank-12U))));
        
		rsq |= ((uint32_t)channel << (5U*(rank-12U)));
        
		_ADC_RSQ0 = rsq;
    }
	else
	{
    }
    
    /* configure ADC sampling time */
    if(channel < 10U)
	{
        sampt = _ADC_SAMPT1;
        
		sampt &= ~((uint32_t)(ADC_SAMPTX_SPTN << (3U*channel)));
        
		sampt |= (uint32_t)(sample_time << (3U*channel));
        
		_ADC_SAMPT1 = sampt;
    }
	else if(channel < 19U)
	{
        sampt = _ADC_SAMPT0;
        
		sampt &= ~((uint32_t)(ADC_SAMPTX_SPTN << (3U*(channel-10U))));
        
		sampt |= (uint32_t)(sample_time << (3U*(channel-10U)));
        
		_ADC_SAMPT0 = sampt;
    }
	else
	{
        /* illegal parameters */
    }
}

struct gpio_adc gpio_adc_setup(uint8_t pin)
{
	uint32_t chan;
	
	uint32_t delay = 0xfffff;
	
	struct gpio_adc gpio_adc_rt;

	for(chan = 0; ; chan++)
	{
		if(chan >= sizeof(adc_pin)/sizeof(adc_pin[0]))
		{
			shutdown("Not a valid ADC pin");
		}

		if(adc_pin[chan] == pin)
		{
			break;
		}
	}

	gpio_adc_rt.chan = chan;

	//enable_pclock(gpio_pclk[GPIO2PORT(pin)]);
	
	enable_pclock(RCU_ADC);

#if CONFIG_MACH_GD32E230X8

	if(pin == ADC_TEMPERATURE_PIN)
	{
		_ADC_CTL1 = ((uint32_t)ADC_CTL1_TSVREN);  //mcu temp
	}
	else
	{	
		gpio_init_mode_set(pin, GPIO_MODE_ANALOG, GPIO_PUPD_NONE);
	}

#elif CONFIG_MACH_GD32F303XX
	if(pin == ADC_TEMPERATURE_PIN)
	{
		_ADC_CTL1 = ((uint32_t)ADC_CTL1_TSVREN);  //mcu temp
	}
	else
	{	
		gpio_peripheral(pin, 2, 0);
	}
	
	_ADC_CTL0 |= ADC_MODE_FREE;

#endif

	adcClockConfig();

	_ADC_CTL1 &= ~((uint32_t)ADC_CTL1_DAL);  //dataalign_right

	_ADC_RSQ0 &= ~((uint32_t)ADC_RSQ0_RL);  //channel_length
        
	_ADC_RSQ0 |= RSQ0_RL((uint32_t)(0U));

	_ADC_CTL1 &= ~((uint32_t)ADC_CTL1_ETSRC); //trigger_source_cfg
        
	_ADC_CTL1 |= (uint32_t)ADC_EXTTRIG_REGULAR_NONE; //soft trigger

    _ADC_CTL1 |= ADC_CTL1_ETERC;	//enable soft trigger
   
    _ADC_CTL1 |= (uint32_t)ADC_CTL1_ADCON; //adc_enable
	
	//delay 1ms
	for(;delay > 0; delay--);
	
    _ADC_CTL1 |= (uint32_t) ADC_CTL1_RSTCLB; //calibration_reset
    
	while((_ADC_CTL1 & ADC_CTL1_RSTCLB));

    _ADC_CTL1 |= ADC_CTL1_CLB; //calibration_en
    
	while((_ADC_CTL1 & ADC_CTL1_CLB));

	return gpio_adc_rt;
}


uint32_t 
gpio_adc_sample(struct gpio_adc g)
{
	if(_ADC_STAT & ADC_FLAG_STRC) //start of group conversion flag
	{
		if(_ADC_STAT & ADC_FLAG_EOC) //end of group conversion flag
		{
			return 0;
		}

		goto delaytime;
	} 

	adcRegularChannelConfig(0, g.chan, ADC_SAMPLETIME_7POINT5);

	_ADC_CTL1 |= ADC_CTL1_SWRCST; //software trigger enable

delaytime:
	
	return timer_from_us(20);
}

uint16_t
gpio_adc_read(struct gpio_adc g)
{
	_ADC_STAT &= ~((uint32_t)ADC_FLAG_EOC); //clear end fo conversion flag
	
	_ADC_STAT &= ~((uint32_t)ADC_FLAG_STRC); //clear start fo conversion flag

	return ((uint16_t)_ADC_RDATA); //adc data 
}

void
gpio_adc_cancel_sample(struct gpio_adc g)
{
	irqstatus_t flag = irq_save();

	if(_ADC_STAT & ADC_FLAG_EOC) //end of group conversion flag
	{
		gpio_adc_read(g);
	}

	irq_restore(flag);
}
