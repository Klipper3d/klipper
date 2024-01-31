#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "basecmd.h"        // oid_alloc
#include "board/gpio.h"     // struct gpio_in
#include "board/irq.h"      // irq_disable
#include "board/misc.h"     // timer_from_us
#include "board/internal.h" // gpio_peripheral
#include "command.h"        // DECL_COMMAND
#include "sched.h"          // struct timer
#include <stdio.h>
#include <string.h>

#define TICKS_LEN 32

struct dirzctl
{
    struct timer time;

    uint32_t oid;
    uint32_t count;
    int32_t dir_inverts[4];
    int32_t step_invrets[4];
    struct gpio_out dirs[4];
    struct gpio_out steps[4];
    struct gpio_in in_dirs[4];

    uint32_t r_steps;
    uint32_t r_steps_fix;
    uint32_t n_ticks;
    uint32_t o_dirs[4];

    uint32_t r_stop;

    uint32_t a_finish;
    uint32_t a_steps;
    uint32_t a_ticks;
};

struct task_wake dirzctl_wake;

//struct dirzctl *d = NULL;
void 
command_config_dirzctl(uint32_t *args);

static void 
deal_dirs(struct dirzctl *d, int32_t dir, int32_t is_save)
{
    if (is_save > 0)
    {
        for (int32_t i = 0; i < d->count; i++)
        {
            d->o_dirs[i] = gpio_in_read(d->in_dirs[i]);
            gpio_out_write(d->dirs[i], (dir != d->dir_inverts[i]));
        }
    }
    else
    {
        for (int32_t i = 0; i < d->count; i++)
            gpio_out_write(d->dirs[i], d->o_dirs[i]);
    }
}

static void 
deal_steps(struct dirzctl *d)
{
	for (int i = 0; i < d->count; i++)
        gpio_out_toggle_noirq(d->steps[i]);
}

static void 
send_status(struct dirzctl *d)
{
    d->a_ticks = timer_read_time();
    d->a_steps = (d->r_steps - (d->r_steps % 2)) / 2;
    d->a_finish = 1;
}

static uint_fast8_t 
dirzctl_event(struct timer *t)
{
	struct dirzctl *d = container_of(t,struct dirzctl,time);

    sched_wake_task(&dirzctl_wake);
	deal_steps(d);
    d->r_steps--;
    if((d->r_steps == 0) || ((d->r_steps % 2 == 0) && (d->r_stop == 1)))
    {
        sched_del_timer(&d->time);
        send_status(d);
        deal_dirs(d,0, 0);
        d->r_stop = 0;
        return SF_DONE;        
    }else if(d->r_steps == d->r_steps_fix - 1)
        send_status(d);
    d->time.waketime = timer_read_time() + d->n_ticks;
    return SF_RESCHEDULE;
}

void 
command_config_dirzctl(uint32_t *args)
{
    sendf("debug_dirzctl oid=%c arg[0]=%u arg[1]=%u arg[2]=%u arg[3]=%u arg[4]=%u arg[5]=%u", 
        (uint8_t)args[0], (uint32_t)args[0], (uint32_t)args[1], (uint32_t)args[2], (uint32_t)args[3], (uint32_t)args[4], (uint32_t)args[5]);
    struct dirzctl* d = oid_alloc(args[0], command_config_dirzctl, sizeof(*d));
    d->oid = args[0];
    d->count = args[1];
    d->a_finish = 0;
    d->r_stop = 0;
    if (d->count > 4)
        shutdown("Max of 4 Z AXIS");
    d->time.func = dirzctl_event;
}
DECL_COMMAND(command_config_dirzctl, "config_dirzctl oid=%c z_count=%c");

void 
command_add_dirzctl(uint32_t *args)
{
    uint8_t index = args[1];
	uint8_t oid = args[0];
	struct dirzctl* d = oid_lookup(oid,command_config_dirzctl);

    sendf("debug_dirzctl oid=%c arg[0]=%u arg[1]=%u arg[2]=%u arg[3]=%u arg[4]=%u arg[5]=%u", 
        (uint8_t)args[0], (uint32_t)args[0], (uint32_t)args[1], (uint32_t)args[2], (uint32_t)args[3], (uint32_t)args[4], (uint32_t)args[5]);
    if (index >= 4)
        shutdown("Set direct z ctl maximum count");
    d->dir_inverts[index] = args[4];
    d->step_invrets[index] = args[5];
    d->in_dirs[index] = gpio_in_setup(args[2], 0);
    d->dirs[index] = gpio_out_setup(args[2], 0);
    d->steps[index] = gpio_out_setup(args[3], args[5]);
}
DECL_COMMAND(command_add_dirzctl, "add_dirzctl oid=%c index=%c dir_pin=%u step_pin=%u dir_invert=%c step_invert=%c");

void 
command_run_dirzctl(uint32_t *args)
{
    //sendf("debug_dirzctl oid=%c arg[0]=%u arg[1]=%u arg[2]=%u arg[3]=%u arg[4]=%u arg[5]=%u", 
    //   (uint8_t)args[0], (uint32_t)args[0], (uint32_t)args[1], (uint32_t)args[2], (uint32_t)args[3], (uint32_t)args[4], (uint32_t)args[5]);
	uint8_t oid = args[0];
	struct dirzctl* d = oid_lookup(oid,command_config_dirzctl);
    
	if(args[3] == 0)
    {
        d->r_stop = 1;
        return;
    }    
    d->r_stop = 0;
    deal_dirs(d,args[1], 1);    
    d->n_ticks = (uint32_t)(((double)args[2] / 1000000) * CONFIG_CLOCK_FREQ) / 2; 
    d->r_steps = args[3] * 2;
    d->r_steps_fix = d->r_steps;
    d->time.waketime = timer_read_time() + d->n_ticks;
    sched_add_timer(&d->time);
}
DECL_COMMAND(command_run_dirzctl, "run_dirzctl oid=%c direct=%c step_us=%u step_cnt=%u");

void 
dirzctl_task(void)
{
	uint8_t oid;

	struct dirzctl* d;

	if(!sched_check_wake(&dirzctl_wake))
	{
		return;
	}
	
	foreach_oid(oid,d,command_config_dirzctl)
	{

		if(d->a_finish == 0)
		{
			return;
		}

		sendf("result_dirzctl oid=%c step=%u tick=%u", (uint8_t)d->oid, (uint16_t)d->a_steps, (uint32_t)d->a_ticks);
		
		d->a_finish = 0;
	}
	return;
}
DECL_TASK(dirzctl_task);

void 
dirzctl_shutdown(void)
{
	uint8_t oid;

	struct dirzctl* d;

	foreach_oid(oid,d,command_config_dirzctl)
	{
		d->a_finish = 0;
	}

	return;
}

DECL_SHUTDOWN(dirzctl_shutdown);

