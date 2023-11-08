#include "autoconf.h" // CONFIG_MACH_STM32F1
#include "board/misc.h" // timer_is_before
#include "command.h" // shutdown
#include "gpio.h" // i2c_setup
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown
#include "board/irq.h" //irq_disable

struct i2c_cfg
{
	i2c_type* i2cx;
	uint32_t  per_rst;
	uint32_t  per_clk;
	uint8_t	  scl;
	uint8_t   sda;
};

struct i2c_cfg	i2c_cfg_info[] = 
{
	{.i2cx = I2C1, .per_rst = CRM_I2C1_PERIPH_RESET, .per_clk = CRM_I2C1_PERIPH_CLOCK, .scl = GPIO('B',10), .sda = GPIO('B',11)},
	{.i2cx = I2C2, .per_rst = CRM_I2C2_PERIPH_RESET, .per_clk = CRM_I2C2_PERIPH_CLOCK, .scl = GPIO('A',8), .sda = GPIO('C',9)},
	{.i2cx = I2C3, .per_rst = CRM_I2C3_PERIPH_RESET, .per_clk = CRM_I2C3_PERIPH_CLOCK, .scl = GPIO('B',6), .sda = GPIO('B',7)}
};

DECL_ENUMERATION("i2c_bus", "i2c1", 0);
DECL_CONSTANT_STR("BUS_PINS_i2c1", "PB10,PB11");

DECL_ENUMERATION("i2c_bus", "i2c2", 1);
DECL_CONSTANT_STR("BUS_PINS_i2c2", "PA8,PC9");

DECL_ENUMERATION("i2c_bus", "i2c3", 2);
DECL_CONSTANT_STR("BUS_PINS_i2c3", "PB6,PB7");

struct i2c_config
i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr)
{
	i2c_config ret = {.i2c = i2c_cfg_info[bus].i2cx, .addr = addr};

	i2c_type* i2cx = (i2c_type*)ret.i2c;

	gpio_init_type i2c_port;

	uint16_t temp = 0;

	uint16_t freq_mhz = 0;
	
	CRM_REG(i2c_cfg_info[bus].per_rst) |= (CRM_REG_BIT(i2c_cfg_info[bus].per_rst));

	CRM_REG(i2c_cfg_info[bus].per_rst) &= ~(CRM_REG_BIT(i2c_cfg_info[bus].per_rst));

	enable_pclock(i2c_cfg_info[bus].per_clk);

	i2c_port.gpio_out_type = GPIO_OUTPUT_OPEN_DRIAN;

	i2c_port.gpio_mode = GPIO_OMDE_MUX;

	i2c_port.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;	

	gpio_peripheral(i2c_cfg_info[bus].scl, &i2c_port, 1);
   	
	gpio_peripheral(i2c_cfg_info[bus].sda, &i2c_port, 1);

	i2cx->ctrl1_bit.i2cen = 0;

	freq_mhz = (get_pclock_frequency(1) / 1000000);
	
	i2cx->ctrl2_bit.clkfreq = freq_mhz;

	temp = (uint16_t)(get_pclock_frequency(1) / (100000 << 1));

	if(temp < 0x04)
	{
		temp = 0x04;
	}	
	
	i2cx->clkctrl_bit.speed = temp;

	i2cx->clkctrl_bit.speedmode = 0;

	if((freq_mhz + 1) > 0x3F)
	{
		i2cx->tmrise_bit.risetime = 0x3F;
	}
	else
	{
		i2cx->tmrise_bit.risetime = (freq_mhz + 1);
	}

	i2cx->oaddr1_bit.addrimode = I2C_ADDRESS_MODE_7BIT;	

	i2cx->ctrl1_bit.i2cen = 1;

	return ret;	
}

static int32_t 
i2c_flag_get(i2c_type* i2cx, uint32_t flag)
{
	volatile uint32_t reg = 0, value = 0;

	reg = flag >> 28;

	flag &= (uint32_t)0x00FFFFFF;

	if(reg == 0)
	{
		value = i2cx->sts1;
	}
  	else
  	{
    	flag = (uint32_t)(flag >> 16);

    	value = i2cx->sts2;
  	}

	if((value & flag) != (uint32_t)1)
  	{
    	return 1;
  	}
  	else
  	{
    	return 0;
  	}
}

static void 
i2c_flag_clear(i2c_type *i2cx, uint32_t flag)
{  
	i2cx->sts1 = (uint16_t)~(flag & (uint32_t)0x0000DF00);
  
	if(i2cx->sts1 & I2C_ADDR7F_FLAG)
	{
    	(void)(i2cx->sts2);
  	}
  
  	if(i2cx->sts1 & I2C_STOPF_FLAG)
	{
    	i2cx->ctrl1_bit.i2cen = 1;
  	}
}


static int32_t 
i2c_wait(i2c_type* i2cx, uint32_t flag, uint32_t check, uint32_t timeout)
{
	if(flag == I2C_BUSYF_FLAG)
  	{
    	while(i2c_flag_get(i2cx, flag) != RESET)
    	{
      		/* check timeout */
      		//if((timeout--) == 0)
			if(!timer_is_before(timer_read_time(), timeout))
      		{
        		shutdown("I2c timeout");
				//return I2C_ERR_TIMEOUT;
      		}
    	}
	}
  	else
  	{
    	while(i2c_flag_get(i2cx, flag) == RESET)
    	{
      		/* check the ack fail flag */
      		if(event_check & I2C_EVENT_CHECK_ACKFAIL)
      		{
        		if(i2c_flag_get(i2cx, I2C_ACKFAIL_FLAG) != RESET)
        		{
          			/* generate stop condtion */
          			//i2c_stop_generate(i2cx);
					i2cx->ctrl1_bit.genstop = 1;

          			/* clear ack fail flag */
          			i2c_flag_clear(i2cx, I2C_ACKFAIL_FLAG);

          			return I2C_ERR_ACKFAIL;
        		}
      		}

      		/* check the stop flag */
      		if(event_check & I2C_EVENT_CHECK_STOP)
      		{
        		if(i2c_flag_get(i2cx, I2C_STOPF_FLAG) != RESET)
        		{
          			/* clear stop flag */
          			i2c_flag_clear(i2cx, I2C_STOPF_FLAG);

          			return I2C_ERR_STOP;
        		}
      		}

      		/* check timeout */
      		//if((timeout--) == 0)
			if(!timer_is_before(timer_read_time(), timeout))
      		{
        		shutdown("I2c timeout");
				//return I2C_ERR_TIMEOUT;
      		}
    	}
  	}

	return 0;	
}

static int32_t 
i2c_master_write_addr(i2c_type* i2cx, uint16_t address, uint32_t timeout)
{
	/* generate start condtion */
	//i2c_start_generate(i2cx);
	i2cx->ctrl1_bit.genstart = 1;

  	/* wait for the start flag to be set */
  	if(i2c_wait_flag(i2cx, I2C_STARTF_FLAG, I2C_EVENT_CHECK_NONE, timeout) != 0)
  	{
    	return I2C_ERR_START;
  	}

  	if(i2cx->oaddr1_bit.addr1mode == I2C_ADDRESS_MODE_7BIT)
  	{
    	/* send slave address */
    	//i2c_7bit_address_send(i2cx, address, I2C_DIRECTION_TRANSMIT);
		i2cx->dt = address & 0xFE;
  	}
  	else
  	{
    	/* send slave 10-bit address header */
    	//i2c_data_send(i2cx, (uint8_t)((address & 0x0300) >> 7) | 0xF0);
		i2cx->dt = (uint8_t)((address & 0x0300) >> 7) | 0xF0;

    	/* wait for the addrh flag to be set */
    	if(i2c_wait_flag(i2cx, I2C_ADDRHF_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != 0)
    	{
      		return I2C_ERR_ADDR10;
    	}

    	/* send slave address */
    	//i2c_data_send(i2cx, (uint8_t)(address & 0x00FF));
		i2cx->dt = (uint8_t)(address & 0x00FF);
  	}

  	/* wait for the addr7 flag to be set */
  	if(i2c_wait_flag(i2cx, I2C_ADDR7F_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != 0)
  	{
    	return I2C_ERR_ADDR;
  	}

  	return 0;
}

void
i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write)
{
	i2c_type* i2cx = (i2c_type*)config.i2c;

	uint8_t address = config.addr;

	uint8_t size = write_len;

	uint8_t* pdata = write;
	
	uint32_t timeout = timer_read_time() + timer_from_us(5000);
  	/* wait for the busy flag to be reset */
  	if(i2c_wait_flag(i2cx, I2C_BUSYF_FLAG, I2C_EVENT_CHECK_NONE, timeout) != 0)
  	{
    	shutdown("i2c error");
		//return I2C_ERR_STEP_1;
  	}

  	/* ack acts on the current byte */
  	//i2c_master_receive_ack_set(i2cx, I2C_MASTER_ACK_CURRENT);
	i2cx->ctrl1_bit.mackctrl = I2C_MASTER_ACK_CURRENT;

  	/* send slave address */
  	if(i2c_master_write_addr(i2cx, address, timeout) != 0)
  	{
    	/* generate stop condtion */
    	//i2c_stop_generate(i2cx);
		i2cx->ctrl1_bit.genstop = 1;

    	shutdown("i2c error");
   		//return I2C_ERR_STEP_2;
  	}

  	/* clear addr flag */
  	i2c_flag_clear(i2cx, I2C_ADDR7F_FLAG);

  	while(size > 0)
  	{
    	/* wait for the tdbe flag to be set */
    	if(i2c_wait_flag(i2cx, I2C_TDBE_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != 0)
    	{
      		/* generate stop condtion */
      		//i2c_stop_generate(i2cx);
			i2cx->ctrl1_bit.genstop = 1;
			
			shutdown("i2c error");
      		//return I2C_ERR_STEP_3;
    	}

    	/* write data */
    	//i2c_data_send(i2cx, (*pdata++));
		i2c_x->dt = *pdata++;

    	size--;
  	}

  	/* wait for the tdc flag to be set */
  	if(i2c_wait_flag(i2cx, I2C_TDC_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != 0)
  	{
    	/* generate stop condtion */
    	//i2c_stop_generate(i2cx);
		i2cx->ctrl1_bit.genstop = 1;

		shutdown("i2c error");
    	//return I2C_ERR_STEP_4;
  	}

  	/* generate stop condtion */
  	//i2c_stop_generate(i2cx);
	i2cx->ctrl1_bit.genstop = 1;

  	return 0;
}

static int32_t 
i2c_master_read_addr(i2c_type *i2cx, uint16_t address, uint32_t timeout)
{
	/* enable ack */
  	//i2c_ack_enable(hi2c->i2cx, TRUE);
	
	i2cx->ctrl1_bit.acken = 1;

  	/* generate start condtion */
  	//i2c_start_generate(i2cx);
	i2cx->ctrl1_bit.genstart = 1;

	/* wait for the start flag to be set */
	if(i2c_wait_flag(i2cx, I2C_STARTF_FLAG, I2C_EVENT_CHECK_NONE, timeout) != 0)
  	{
    	shutdown("i2c read addr error");
	}

  	if(i2cx->oaddr1_bit.addr1mode == I2C_ADDRESS_MODE_7BIT)
  	{
    	/* send slave address */
    	//i2c_7bit_address_send(hi2c->i2cx, address, I2C_DIRECTION_RECEIVE);
		i2cx->dt = address | 0x01;
  	}
	else
  	{
    	/* send slave 10-bit address header */
    	//i2c_data_send(hi2c->i2cx, (uint8_t)((address & 0x0300) >> 7) | 0xF0);
		i2cx->dt = (uint8_t)((address & 0x0300) >> 7) | 0xF0;

    	/* wait for the addrh flag to be set */
    	if(i2c_wait_flag(i2cx, I2C_ADDRHF_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != 0)
    	{
			shutdown("i2c read addr error");
      		//return I2C_ERR_ADDR10;
    	}

    	/* send slave address */
    	//i2c_data_send(i2cx, (uint8_t)(address & 0x00FF));
		i2cx->dt = (uint8_t)(address & 0x00FF);

    	/* wait for the addr7 flag to be set */
    	if(i2c_wait_flag(i2cx, I2C_ADDR7F_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != 0)
    	{
			shutdown("i2c read addr error");
      		//return I2C_ERR_ADDR;
    	}

    	/* clear addr flag */
    	i2c_flag_clear(i2cx, I2C_ADDR7F_FLAG);

    	/* generate restart condtion */
    	//i2c_start_generate(hi2c->i2cx);
		i2cx->ctrl1_bit.genstart = TRUE;

    	/* wait for the start flag to be set */
    	if(i2c_wait_flag(i2cx, I2C_STARTF_FLAG, I2C_EVENT_CHECK_NONE, timeout) != 0)
    	{
			shutdown("i2c read error");
      		//return I2C_ERR_START;
    	}

    	/* send slave 10-bit address header */
    	//i2c_data_send(hi2c->i2cx, (uint8_t)((address & 0x0300) >> 7) | 0xF1);
		i2cx->dt = (uint8_t)((address & 0x0300) >> 7) | 0xF1;
  	}

  	/* wait for the addr7 flag to be set */
  	if(i2c_wait_flag(i2cx, I2C_ADDR7F_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != 0)
  	{
		shutdown("i2c read addr error");
    	//return I2C_ERR_ADDR;
  	}

  	return 0;
}

void
i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg
         , uint8_t read_len, uint8_t *read)
{
	uint8_t size = read_len;

	uint8_t* pdata = read;

	uint8_t address = config.addr;

	uint32_t timeout = timer_read_time() + timer_from_us(5000);
	
	/* wait for the busy flag to be reset */
	if(i2c_wait_flag(i2cx, I2C_BUSYF_FLAG, I2C_EVENT_CHECK_NONE, timeout) != 0)
	{
		shutdown("i2c read error");
    	//return I2C_ERR_STEP_1;
	}

  	/* ack acts on the current byte */
  	//i2c_master_receive_ack_set(hi2c->i2cx, I2C_MASTER_ACK_CURRENT);
	i2cx->ctrl1_bit.mackctrl = I2C_MASTER_ACK_CURRENT;

  	/* send slave address */
  	if(i2c_master_write_addr(i2cx, address, timeout) != 0)
  	{
    	/* generate stop condtion */
    	//i2c_stop_generate(hi2c->i2cx);
		i2cx->ctrl1_bit.genstop = 1;
		
		shutdown("i2c read error");
    	//return I2C_ERR_STEP_2;
  	}


	//write slave register
  	/* clear addr flag */
  	i2c_flag_clear(i2cx, I2C_ADDR7F_FLAG);

  	while(reg_len > 0)
  	{
    	/* wait for the tdbe flag to be set */
    	if(i2c_wait_flag(i2cx, I2C_TDBE_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != 0)
    	{
      		/* generate stop condtion */
      		//i2c_stop_generate(i2cx);
			i2cx->ctrl1_bit.genstop = 1;
			
			shutdown("i2c error");
      		//return I2C_ERR_STEP_3;
    	}

    	/* write data */
    	//i2c_data_send(i2cx, (*pdata++));
		i2c_x->dt = *reg++;

    	reg_len--;
  	}

	/* wait for the tdc flag to be set */
  	if(i2c_wait_flag(i2cx, I2C_TDC_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != 0)
  	{
    	/* generate stop condtion */
    	//i2c_stop_generate(hi2c->i2cx);
			
		i2cx->ctrl1_bit.genstop = 1;
			
		shutdown("i2c error");

    	//return I2C_ERR_STEP_4;
  	}

  	/* enable ack */
  	//i2c_ack_enable(hi2c->i2cx, TRUE);
	i2cx->ctrl1_bit.acken = 1;

  	/* send slave address */
  	if(i2c_master_read_addr(i2cx, address, timeout) != 0)
  	{
    	/* generate stop condtion */
    	//i2c_stop_generate(hi2c->i2cx);
		i2cx->ctrl1_bit.genstop = 1;
		
		shutdown("i2c read error");
    	//return I2C_ERR_STEP_2;
  	}

  	if(size == 1)
  	{
    	/* disable ack */
    	//i2c_ack_enable(hi2c->i2cx, FALSE);
		i2cx->ctrl1_bit.acken = 0;

    	/* clear addr flag */
    	i2c_flag_clear(i2cx, I2C_ADDR7F_FLAG);

    	/* generate stop condtion */
    	//i2c_stop_generate(i2cx);
		i2cx->ctrl1_bit.genstop = 1;
  	}
  	else if(size == 2)
  	{
    	/* ack acts on the next byte */
    	//i2c_master_receive_ack_set(i2cx, I2C_MASTER_ACK_NEXT);
		i2cx->ctrl1_bit.mackctrl = I2C_MASTER_ACK_NEXT;

    	/* disable ack */
    	//i2c_ack_enable(i2cx, FALSE);
		i2cx->ctrl1_bit.acken = 0;

    	/* clear addr flag */
    	i2c_flag_clear(i2cx, I2C_ADDR7F_FLAG);
  	}
  	else
  	{
    	/* enable ack */
    	//i2c_ack_enable(i2cx, TRUE);
		i2cx->ctrl1_bit.acken = 1;

    	/* clear addr flag */
    	i2c_flag_clear(i2cx, I2C_ADDR7F_FLAG);
  	}

  	while(size > 0)
  	{
    	if(size <= 3)
    	{
      		/* 1 byte */
      		if(size == 1)
      		{
        		/* wait for the rdbf flag to be set */
        		if(i2c_wait_flag(i2cx, I2C_RDBF_FLAG, I2C_EVENT_CHECK_NONE, timeout) != 0)
        		{
					/* generate stop condtion */
        
					//i2c_stop_generate(i2cx);
					i2cx->ctrl1_bit.genstop = 1;

					shutdown("i2c read error");
					//return I2C_ERR_STEP_3;
        		}

        		/* read data */
        		//(*pdata++) = i2c_data_receive(i2cx);
				(*pdata++) = (uint8_t)i2cx->dt;
        
				size--;
			}
    
			/* 2 bytes */
			else if(size == 2)
			{
    			/* wait for the tdc flag to be set */
        		if(i2c_wait_flag(i2cx, I2C_TDC_FLAG, I2C_EVENT_CHECK_NONE, timeout) != 0)
        		{
        			/* generate stop condtion */
        			//i2c_stop_generate(i2cx);
					i2cx->ctrl1_bit.genstop = 1;
					
					shutdown("i2c read error");
          			//return I2C_ERR_STEP_4;
        		}

        		/* generate stop condtion */
        		//i2c_stop_generate(i2cx);
				i2cx->ctrl1_bit.genstop = 1;

        		/* read data */
        		//(*pdata++) = i2c_data_receive(i2cx);
				(*pdata++) = (uint8_t)i2cx->dt;
        
				size--;

        		/* read data */
        		//(*pdata++) = i2c_data_receive(i2cx);
				(*pdata++) = (uint8_t)i2cx->dt;
        
				size--;
			}
    
			/* 3 last bytes */
			else
    		{
    			/* wait for the tdc flag to be set */
        		if(i2c_wait_flag(i2cx, I2C_TDC_FLAG, I2C_EVENT_CHECK_NONE, timeout) != 0)
        		{
        			/* generate stop condtion */
       				//i2c_stop_generate(i2cx);
					i2cx->ctrl1_bit.genstop = 1;

					shutdown("i2c read error");
          			//return I2C_ERR_STEP_5;
        		}

        		/* disable ack */
        		//i2c_ack_enable(hi2c->i2cx, FALSE);
				i2cx->ctrl1_bit.acken = 0;

        		/* read data */
        		//(*pdata++) = i2c_data_receive(hi2c->i2cx);
				(*pdata++) = (uint8_t)i2cx->dt;
        
				size--;

        		/* wait for the tdc flag to be set */
        		if(i2c_wait_flag(i2cx, I2C_TDC_FLAG, I2C_EVENT_CHECK_NONE, timeout) != 0)
        		{
        			/* generate stop condtion */
        			//i2c_stop_generate(i2cx);
					i2cx->ctrl1_bit.genstop = 1;

					shutdown("i2c read error");
        			//return I2C_ERR_STEP_6;
        		}

        		/* generate stop condtion */
        		//i2c_stop_generate(hi2c->i2cx);
				i2cx->ctrl1_bit.genstop = 1;

        		/* read data */
        		//(*pdata++) = i2c_data_receive(hi2c->i2cx);
				(*pdata++) = (uint8_t)i2cx->dt;
        
				size--;

        		/* read data */
        		//(*pdata++) = i2c_data_receive(hi2c->i2cx);
				(*pdata++) = (uint8_t)i2cx->dt;
        
				size--;
			}
		}
    	else
    	{
      		/* wait for the rdbf flag to be set */
      		if(i2c_wait_flag(i2cx, I2C_RDBF_FLAG, I2C_EVENT_CHECK_NONE, timeout) != 0)
      		{
        		/* generate stop condtion */
        		//i2c_stop_generate(hi2c->i2cx);
				i2cx->ctrl1_bit.genstop = 1;

				shutdown("i2c read error");
        		//return I2C_ERR_STEP_7;
      		}

      		/* read data */
      		//(*pdata++) = i2c_data_receive(i2cx);
			(*pdata++) = (uint8_t)i2cx->dt;
      		
			size--;
    	}
  	}

  	return 0;
}
