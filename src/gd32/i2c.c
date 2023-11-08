#include "autoconf.h" // CONFIG_MACH_STM32F1
#include "board/misc.h" // timer_is_before
#include "command.h" // shutdown
#include "gpio.h" // i2c_setup
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown
#include "board/irq.h" //irq_disable

struct i2c_cfg
{
	uint32_t i2cx;
	uint32_t  per_rst;
	uint32_t  per_clk;
	uint8_t	  scl;
	uint8_t   sda;
};

struct i2c_cfg	i2c_cfg_info[] = 
{
};

//DECL_ENUMERATION("i2c_bus", "i2c1", 0);
//DECL_CONSTANT_STR("BUS_PINS_i2c1", "PB10,PB11");

struct i2c_config
i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr)
{
	i2c_config ret = {0};

	return ret;	
}

static int32_t 
i2c_flag_get(uint32_t i2cx, uint32_t flag)
{
	return 0;
}

static void 
i2c_flag_clear(uint32_t i2cx, uint32_t flag)
{  
	return 0;
}


static int32_t 
i2c_wait(uint32_t i2cx, uint32_t flag, uint32_t check, uint32_t timeout)
{
	return 0;	
}

static int32_t 
i2c_master_write_addr(uint32_t i2cx, uint16_t address, uint32_t timeout)
{
  	return 0;
}

void
i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write)
{
  	return 0;
}

static int32_t 
i2c_master_read_addr(uint32_t i2cx, uint16_t address, uint32_t timeout)
{
  	return 0;
}

void
i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg
         , uint8_t read_len, uint8_t *read)
{
  	return 0;
}
