
// Code for Bed Distance Sensor
//
// Mark <niujl123@sina.com> https://github.com/markniu/Bed_Distance_sensor
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include<string.h>
#include <stdlib.h>

#include "autoconf.h" // CONFIG_MACH_AVR
#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_poll
#include "board/misc.h" // timer_from_us
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_SHUTDOWN


#define BYTE_CHECK_OK     0x01
#define BYTE_CHECK_ERR    0x00
#define BD_setLow(x)  gpio_out_write(x,0)
#define BD_setHigh(x) gpio_out_write(x,1)


uint32_t sda_pin=0,scl_pin=0,delay_m=200;
uint16_t BD_Data=0;
uint16_t BD_read_flag=1018,BD_read_lock=0;

struct gpio_out sda_gpio, scl_gpio;
struct gpio_in sda_gpio_in;

uint16_t BD_i2c_read(void);


uint16_t Get_Distane_data(void)
{
	BD_read_flag=1018;
	return BD_Data;

}



int BD_i2c_init(uint32_t _sda,uint32_t _scl,uint32_t delays) {
	sda_pin=_sda;
	scl_pin =_scl;
	if(delays>0)
		delay_m=delays;
	sda_gpio=gpio_out_setup(sda_pin, 0);//floting mode
	scl_gpio=gpio_out_setup(scl_pin, 0);
	///
	gpio_out_write(sda_gpio, 1);
	gpio_out_write(scl_gpio, 1);
	return 1;
}

uint32_t nsecs_to_ticks_bd(uint32_t ns)
{
    return timer_from_us(ns * 1000) / 1000000;
}

void ndelay_bd(uint32_t nsecs)
{
    if (CONFIG_MACH_AVR)
        // Slower MCUs don't require a delay
        return;
    uint32_t end = timer_read_time() + nsecs_to_ticks_bd(nsecs);
    while (timer_is_before(timer_read_time(), end))
        irq_poll();
        
}

unsigned short BD_Add_OddEven(unsigned short byte)
{
	unsigned char i;
	unsigned char n; 
	unsigned short r; 
	n =0;
  for(i=0;i<10;i++)
	{
	  if(((byte >>i)&0x01) == 0x01)
		{
		   n++;
		}	
	}
	if((n&0x01) == 0x01)
	{
		r = byte | 0x400; 
	}
	else
	{
	  r = byte | 0x00;  
	}
	return r;
}

unsigned short BD_Check_OddEven(unsigned short byte)
{
	unsigned char i;
	unsigned char n;  
	unsigned char r;  
	n =0;
    for(i=0;i<10;i++) 
	{
		if(((byte >>i)&0x01) == 0x01)
		{
		   n++;  
		}	
	}
	if((byte>>10) == (n&0x01)) 
	{
		r = BYTE_CHECK_OK;  
	}
	else
	{
		r = BYTE_CHECK_ERR;  
	}
	return r;
}	

void BD_I2C_start(void)
{
	sda_gpio=gpio_out_setup(sda_pin, 1);
	scl_gpio=gpio_out_setup(scl_pin, 1);

	BD_setHigh(scl_gpio);
	BD_setHigh(sda_gpio);
	ndelay_bd(delay_m);
	BD_setLow(sda_gpio);
	ndelay_bd(delay_m);
	BD_setLow(scl_gpio);
	ndelay_bd(delay_m);
}
void  BD_i2c_stop(void) {
  ndelay_bd(delay_m);
  sda_gpio=gpio_out_setup(sda_pin, 1);
  BD_setLow(sda_gpio);
  ndelay_bd(delay_m);
  scl_gpio=gpio_out_setup(scl_pin, 1);
  BD_setHigh(scl_gpio);
  ndelay_bd(delay_m);

  BD_setHigh(sda_gpio);
  ndelay_bd(delay_m);
}



uint16_t BD_i2c_read(void)
{
	uint16_t b = 1024;

 // if(BD_read_lock==1) 
 // 	return b;
	BD_read_lock=1;
	BD_I2C_start();
	//// read
	BD_setHigh(sda_gpio);
	BD_setHigh(scl_gpio);
	ndelay_bd(delay_m);
	BD_setLow(scl_gpio);
	///
	ndelay_bd(delay_m);
	b = 0;
	BD_setHigh(sda_gpio);
	sda_gpio_in=gpio_in_setup(sda_pin, 1);
	for (unsigned char i = 0; i <= 10; i++) {
	b <<= 1;
	ndelay_bd(delay_m);
	BD_setHigh(scl_gpio);
		
	if (gpio_in_read(sda_gpio_in)) 
		b |= 1;
	ndelay_bd(delay_m);
	BD_setLow(scl_gpio);
	}
	BD_i2c_stop();

		  
	if (BD_Check_OddEven(b) && (b & 0x3FF) < 1020)	  
		  // float BD_z = (tmp & 0x3FF) / 100.0f;
		b = (b & 0x3FF);
	 

	if(b>1024)
		b=1024;
	BD_read_lock=0;
	return b;

}

void BD_i2c_write(unsigned int addr)
{
 
	BD_I2C_start();
	//// write
	BD_setLow(sda_gpio);
	BD_setHigh(scl_gpio);
	ndelay_bd(delay_m);
	BD_setLow(scl_gpio);
	addr=BD_Add_OddEven(addr);
	///write address
	ndelay_bd(delay_m);
	for (int i=10; i >=0; i--) 
	{
	if ((addr>>i)&0x01) {BD_setHigh(sda_gpio);} else  BD_setLow(sda_gpio); 
	//if (addr &curr) {set_force_High(sda_gpio);} else  setLow(sda_gpio); 
	BD_setHigh(scl_gpio);
	ndelay_bd(delay_m);
	BD_setLow(scl_gpio);
	ndelay_bd(delay_m);
	}
	////////////
	BD_i2c_stop();
  

}

uint32_t INT_to_String(uint32_t BD_z1,uint8_t*data)
{
    uint32_t BD_z=BD_z1;
	//spidev_transfer(spi, 1, data_len, data);
	
	  //sprintf(data,"%.3f",BD_z);
	  uint32_t len=0,j=0; 
	  if(BD_z>1000)
	  {
		  j=BD_z/1000;
		  data[len++] = '0'+j;
		  BD_z-=1000*j;
		  data[len]='0';
		  data[len+1]='0';
		  data[len+2]='0';
	  }
	  if(BD_z>100)
	  {
		  j=BD_z/100;
		  data[len++] = '0'+j;
		  BD_z-=100*j;
		  data[len]='0';
		  data[len+1]='0';
	  }
	  else if(data[len])
		  len++;
	  if(BD_z>10)
	  {
		  j=BD_z/10;
		  data[len++] = '0'+j;
		  BD_z-=10*j;
		  data[len]='0';
	  }
	  else if(data[len])
		  len++;
	  j=BD_z;
	  data[len++] = '0'+j;
	return len;
	
}

//for gcode command
void 
command_I2C_BD_receive(uint32_t *args)
{
	if(BD_read_flag==1018)
		return;
	uint8_t oid = args[0];
	uint8_t data[8];
	uint16_t BD_z=BD_i2c_read();//BD_Data;
	memset(data,0,8);
//	output("mcuoid=%c BD_z=%u",oid, BD_z);
	uint32_t len=0,j=0; 

    ///////////same as function itoa()
	if(BD_z>=1000)
	{
		j=BD_z/1000;
		data[len++] = '0'+j;
		BD_z-=1000*j;
		data[len]='0';
		data[len+1]='0';
		data[len+2]='0';
	}
	if(BD_z>=100)
	{
		j=BD_z/100;
		data[len++] = '0'+j;
		BD_z-=100*j;
		data[len]='0';
		data[len+1]='0';
	}
	else if(data[len])
		len++;
	if(BD_z>=10)
	{
		j=BD_z/10;
		data[len++] = '0'+j;
		BD_z-=10*j;
		data[len]='0';
	}
	else if(data[len])
		len++;
	j=BD_z;
	data[len++] = '0'+j;
	////////////////
	sendf("I2C_BD_receive_response oid=%c response=%*s", oid,len,data);	
}

DECL_COMMAND(command_I2C_BD_receive, "I2C_BD_receive oid=%c data=%*s");

//for display command
void 
command_I2C_BD_receive2(uint32_t *args)
{
	if(BD_read_flag!=1018)
		return;
	uint8_t oid = args[0];
	uint8_t data[8];
	uint16_t BD_z=BD_i2c_read();//BD_Data;
	memset(data,0,8);
	///////////same as function itoa()
	uint32_t len=0,j=0; 
	if(BD_z>=1000)
	{
		j=BD_z/1000;
		data[len++] = '0'+j;
		BD_z-=1000*j;
		data[len]='0';
		data[len+1]='0';
		data[len+2]='0';
	}
	if(BD_z>=100)
	{
		j=BD_z/100;
		data[len++] = '0'+j;
		BD_z-=100*j;
		data[len]='0';
		data[len+1]='0';
	}
	else if(data[len])
		len++;
	if(BD_z>=10)
	{
		j=BD_z/10;
		data[len++] = '0'+j;
		BD_z-=10*j;
		data[len]='0';
	}
	else if(data[len])
		len++;
	j=BD_z;
	data[len++] = '0'+j;
	///////////////////////
	
	sendf("I2C_BD_receive2_response oid=%c response=%*s", oid,len,data); 
}



DECL_COMMAND(command_I2C_BD_receive2, "I2C_BD_receive2 oid=%c data=%*s");



void 
command_I2C_BD_send(uint32_t *args)
{	
	int addr=atoi((char *)args[2]);
	BD_read_flag=addr;
	if(addr>1021||addr==1015)
		return;
	BD_i2c_write(addr);

}

DECL_COMMAND(command_I2C_BD_send, "I2C_BD_send oid=%c data=%*s");


void
command_config_I2C_BD(uint32_t *args)
{
    BD_i2c_init(args[1],args[2],args[3]);
	 
}
DECL_COMMAND(command_config_I2C_BD,
             "config_I2C_BD oid=%c sda_pin=%u scl_pin=%u delay=%u");




 void
 bd_sensor_task(void)
 {
	if(BD_read_flag!=1018)
		return;

	if(sda_pin==0||scl_pin==0)
		return;
	
	uint16_t tm=BD_i2c_read();
	if(tm<1024)
		BD_Data=tm;
	//output("mcuoid=%c tmd=%u", oid,tm);
 }
 DECL_TASK(bd_sensor_task);


