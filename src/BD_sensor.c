// Code for Bed Distance Sensor
// Mark yue<niujl123@sina.com>
// This file may be distributed under the terms of the GNU GPLv3 license.

#include<string.h>
#include <stdlib.h>
#include "autoconf.h"
#include "board/gpio.h"
#include "board/irq.h"
#include "board/misc.h"
#include "command.h"
#include "sched.h"
#include "autoconf.h" // CONFIG_*
#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_is_before
#include "command.h" // DECL_COMMAND
#include "sched.h" // struct timer
#include "stepper.h" // stepper_event
#include "trsync.h" // trsync_add_signal


#define BYTE_CHECK_OK     0x01
#define BYTE_CHECK_ERR    0x00
#define BD_setLow(x)  gpio_out_write(x,0)
#define BD_setHigh(x) gpio_out_write(x,1)


uint32_t delay_m = 20, homing_pose = 0;
int sda_pin = -1, scl_pin = -1, z_ofset = 0;
uint16_t BD_Data;
//extern uint32_t timer_period_time;
uint16_t BD_read_flag=1018,BD_read_lock=0;
int switch_mode = 0; //1:in switch mode
struct gpio_out sda_gpio, scl_gpio;
struct gpio_in sda_gpio_in;
uint8_t oid_g,etrsync_oid,endstop_reason=0;
uint8_t z_oid[4];
uint32_t endtime_debug=0;
uint32_t timer_period_endstop=100;

///////////BDsensor as endstop
struct timer time_bd;
#include "autoconf.h"
struct endstop {
    struct timer time;
    uint32_t rest_time, sample_time, nextwake,pin_num;
    struct trsync *ts;
    uint8_t flags, sample_count, trigger_count, trigger_reason;
    struct gpio_in pin;
};

enum { ESF_PIN_HIGH=1<<0, ESF_HOMING=1<<1 };
static uint_fast8_t endstop_oversample_event(struct timer *t);
static struct endstop e ;
///////////////

int z_index=0;

enum { POSITION_BIAS=0x40000000 };

struct stepper_move {
    struct move_node node;
    uint32_t interval;
    int16_t add;
    uint16_t count;
    uint8_t flags;
};

struct stepper {
    struct timer time;
    uint32_t interval;
    int16_t add;
    uint32_t count;
    uint32_t next_step_time, step_pulse_ticks;
    struct gpio_out step_pin, dir_pin;
    uint32_t position;
    struct move_queue_head mq;
    struct trsync_signal stop_signal;
    // gcc (pre v6) does better optimization when uint8_t are bitfields
    uint8_t flags : 8;
};

struct step_adjust{
    uint32_t cur_z;
    int adj_z_range;
    int invert_dir;
    int steps_per_mm;
    int step_time;
    int zoid;//oid for all the z stepper
};

#define NUM_Z_MOTOR  6
struct step_adjust step_adj[NUM_Z_MOTOR];//x,y,z

struct _step_probe{
    int min_x;
    int max_x;
    int points;
    int steps_at_zero;
    int steps_per_mm;
    int xoid;//oid for x stepper
    int x_count;
    int x_dir;
    int y_dir;
    int kinematics;//0:cartesian,1:corexy,2:delta
    ////for y
    int min_y;
    int max_y;
    int y_steps_at_zero;
    int y_steps_per_mm;
    int y_oid;//oid for y stepper
    ////
    int x_data[64];
};

struct _step_probe stepx_probe;


void BD_i2c_write(unsigned int addr);
uint16_t BD_i2c_read(void);

int BD_i2c_init(uint32_t _sda,uint32_t _scl,
    uint32_t delays,uint32_t h_pose,int z_adjust)
{
    int i=0;
    sda_pin=_sda;
    scl_pin =_scl;
    homing_pose = h_pose;
    z_ofset = z_adjust;
    if (z_ofset > 500)
        z_ofset = 0;
    if(delays>0)
        delay_m=delays;
    sda_gpio=gpio_out_setup(sda_pin, 1);
    scl_gpio=gpio_out_setup(scl_pin, 1);

    gpio_out_write(sda_gpio, 1);
    gpio_out_write(scl_gpio, 1);
    for (i=0;i<NUM_Z_MOTOR;i++){
        step_adj[i].cur_z=0;
        step_adj[i].zoid=0;
        step_adj[i].adj_z_range=0;
    }
    stepx_probe.xoid=0;
    stepx_probe.y_oid=0;
    BD_i2c_write(1022); //reset BDsensor
    return 1;
}

uint32_t nsecs_to_ticks_bd(uint32_t ns)
{
    return ns * (CONFIG_CLOCK_FREQ / 1000000);
}


void ndelay_bd_c(uint32_t nsecs)
{
    if (CONFIG_MACH_AVR)
        return;
    uint32_t end = timer_read_time() + nsecs_to_ticks_bd(nsecs);
    while (timer_is_before(timer_read_time(), end))
        irq_poll();
}
void ndelay_bd(uint32_t nsecs)
{
    int i=1;
    while(i--)
        ndelay_bd_c(nsecs);
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
void  BD_i2c_stop(void)
{
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
   // if(BD_read_flag==1014)

    BD_read_lock=1;
    BD_I2C_start();

    BD_setHigh(sda_gpio);
    BD_setHigh(scl_gpio);
    ndelay_bd(delay_m);
    BD_setLow(scl_gpio);

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
    if (BD_Check_OddEven(b) && (b & 0x3FF) < 1020){
        b = (b & 0x3FF);
        if(BD_read_flag==1018&&(b<1000)){
            b = b - z_ofset;
            if(b>1024)
                b=0;
        }

    }
    else
        b=1024;

#if 0
    sda_gpio_in=gpio_in_setup(sda_pin, 1);
    b=0;
    b=gpio_in_read(sda_gpio_in)*300+1;
#endif
    BD_read_lock=0;
    return b;
}

void BD_i2c_write(unsigned int addr)
{
    BD_I2C_start();
    //// write
    BD_setLow(sda_gpio);
    BD_setHigh(scl_gpio);
    ndelay_bd(delay_m*2);
    BD_setLow(scl_gpio);
    addr=BD_Add_OddEven(addr);
    ///write address
    ndelay_bd(delay_m);
    for (int i=10; i >=0; i--)
    {
        if ((addr>>i)&0x01)
        {
            BD_setHigh(sda_gpio);
        }
        else
            BD_setLow(sda_gpio);
        ndelay_bd(delay_m);
        BD_setHigh(scl_gpio);
        ndelay_bd(delay_m*2);
        BD_setLow(scl_gpio);
        ndelay_bd(delay_m);
    }
    BD_i2c_stop();
}

uint32_t INT_to_String(uint32_t BD_z1,uint8_t*data)
{
    uint32_t BD_z=BD_z1;
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
    data[len]=0;
    return len;
}

//for gcode command
void
command_I2C_BD_receive(uint32_t *args)
{
    uint8_t oid = args[0];
    uint8_t data[8];
    uint16_t BD_z;

    //if(BD_read_flag==1018)
    //    BD_z=BD_Data;
    //else
    BD_z=BD_i2c_read();//BD_Data;
    BD_Data=BD_z;
    memset(data,0,8);
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

    sendf("I2C_BD_receive_response oid=%c response=%*s", oid,len,data);
}

DECL_COMMAND(command_I2C_BD_receive, "I2C_BD_receive oid=%c data=%*s");


void
command_I2C_BD_send(uint32_t *args)
{
    int addr=atoi((char *)args[2]);
    BD_read_flag=addr;
    if(addr==1015)
        return;
    BD_i2c_write(addr);
    if (addr==1023)
        switch_mode=1;
    else if(addr>1015)
        switch_mode=0;
    else if(switch_mode==1){//write switch value
        sda_gpio_in=gpio_in_setup(sda_pin, 1);
        BD_setLow(scl_gpio);
    }

}

DECL_COMMAND(command_I2C_BD_send, "I2C_BD_send oid=%c data=%*s");


void
command_config_I2C_BD(uint32_t *args)
{
    oid_g = args[0];
    BD_i2c_init(args[1],args[2],args[3],args[4],args[5]);
}
DECL_COMMAND(command_config_I2C_BD,
             "config_I2C_BD oid=%c sda_pin=%u scl_pin=%u"
             " delay=%u h_pos=%u z_adjust=%u");

int  read_endstop_pin(void)
{
    uint16_t tm;
    tm=BD_i2c_read();
    if(tm<1023)
        BD_Data=tm;
    else
        BD_Data=0;
    if(BD_Data<=homing_pose)
        BD_Data=0;
    return BD_Data?0:1;
}
// Timer callback for an end stop
static uint_fast8_t
endstop_event(struct timer *t)
{
    uint8_t val =0;
    if(e.pin_num!=sda_pin)
        val = gpio_in_read(e.pin);
    else if (switch_mode == 1){
        val = gpio_in_read(sda_gpio_in);
    }
    else
        val = read_endstop_pin();
    uint32_t nextwake = e.time.waketime + e.rest_time;
    if ((val ? ~e.flags : e.flags) & ESF_PIN_HIGH) {
        // No match - reschedule for the next attempt
        e.time.waketime = nextwake;
        return SF_RESCHEDULE;
    }
    e.nextwake = nextwake;
    e.time.func = endstop_oversample_event;
    return endstop_oversample_event(t);
}

// Timer callback for an end stop that is sampling extra times
static uint_fast8_t
endstop_oversample_event(struct timer *t)
{
    uint8_t val =0;
    if(e.pin_num!=sda_pin)
        val = gpio_in_read(e.pin);
    else if (switch_mode == 1){
        val = gpio_in_read(sda_gpio_in);
    }
    else{
        val = BD_Data?0:1;//read_endstop_pin();
    }
    if ((val ? ~e.flags : e.flags) & ESF_PIN_HIGH) {
        // No longer matching - reschedule for the next attempt
        e.time.func = endstop_event;
        e.time.waketime = e.nextwake;
        e.trigger_count = e.sample_count;
        return SF_RESCHEDULE;
    }
    uint8_t count = e.trigger_count - 1;
    if (!count) {
        trsync_do_trigger(e.ts, e.trigger_reason);
        step_adj[0].adj_z_range=0;
        return SF_DONE;
    }
    e.trigger_count = count;
    e.time.waketime += e.sample_time;
    return SF_RESCHEDULE;
}


// Home an axis
void
command_BDendstop_home(uint32_t *args)
{
    sched_del_timer(&e.time);
    e.time.waketime = args[1];
    e.sample_time = args[2];
    e.sample_count = args[3];
    if (!e.sample_count) {
        // Disable end stop checking
        e.ts = NULL;
        e.flags = 0;
        return;
    }
    e.rest_time = args[4];
    e.time.func = endstop_event;
    e.trigger_count = e.sample_count;
    e.flags = ESF_HOMING | (args[5] ? ESF_PIN_HIGH : 0);
    e.ts = trsync_oid_lookup(args[6]);
    e.trigger_reason = args[7];
    e.pin_num = args[8];
    e.pin =  gpio_in_setup(args[8], 1);
    sched_add_timer(&e.time);
}
DECL_COMMAND(command_BDendstop_home,
             "BDendstop_home oid=%c clock=%u sample_ticks=%u sample_count=%c"
             " rest_ticks=%u pin_value=%c trsync_oid=%c trigger_reason=%c"
             " endstop_pin=%c");
