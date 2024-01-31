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

#define HX711S_SAMPLE_PERIOD_TICKS			(CONFIG_CLOCK_FREQ / 10000 * 125)
#define HX711S_SAMPLE_REST_TICKS			(HX711S_SAMPLE_PERIOD_TICKS / 2)

struct hx711s
{
	struct timer hx711s_timer;
    uint32_t oid; 
 	uint32_t rest_ticks;
	uint8_t flags;	
    uint32_t hx711_count;
    int32_t  times_read;
    struct gpio_out clks[4];
    struct gpio_in sdos[4];
};

struct task_wake hx711s_wake;

enum {HX711S_SAMPLE_START = 1 << 0};

static uint_fast8_t
hx711s_sample_event(struct timer* t)
{
	sched_wake_task(&hx711s_wake);

	struct hx711s* h = container_of(t,struct hx711s,hx711s_timer);

	h->hx711s_timer.waketime += h->rest_ticks;

	if(h->flags & HX711S_SAMPLE_START)
	{
		return SF_RESCHEDULE;
	}

	return SF_DONE;
}


void 
command_config_hx711s(uint32_t *args)
{
    struct hx711s* h = oid_alloc(args[0], command_config_hx711s, sizeof(*h));
    h->oid = args[0];
    h->hx711_count = args[1];
    if (h->hx711_count > 4)
        shutdown("Max of 4 hx711");
	h->flags = 0;
	h->rest_ticks = HX711S_SAMPLE_REST_TICKS;
	h->hx711s_timer.func = hx711s_sample_event;
    sendf("debug_hx711s oid=%c arg[0]=%u arg[1]=%u arg[2]=%u arg[3]=%u", (int)args[0], (int)args[0], (int)args[1], 0, 0);
}
DECL_COMMAND(command_config_hx711s, "config_hx711s oid=%c hx711_count=%c");

static int32_t 
get_hx711s(struct hx711s* h)
{
    static uint64_t last_tick = 0;    
    int32_t outVals[4] = {0, 0, 0, 0}, is_data_valid = 0;

    for (int j = 0; j < h->hx711_count; j++)
        is_data_valid |= (gpio_in_read(h->sdos[j]) << j);

    uint64_t now_tick = timer_read_time();
    now_tick += (now_tick < last_tick ? 0xFFFFFFFF : 0);
    uint64_t now_inter_ms = (now_tick - last_tick) * 1000.0f / CONFIG_CLOCK_FREQ;

    //if (is_data_valid == 0 || now_inter_ms >= 25)
    if (is_data_valid == 0)
    {
        for (int j = 0; j < h->hx711_count; j++)
            gpio_out_write(h->clks[j], 0);

        for (int i = 0; i < 24; i++)
        {
            for (int j = 0; j < h->hx711_count; j++)
                gpio_out_write(h->clks[j], 1);
            for (int j = 0; j < h->hx711_count; j++)
                outVals[j] = outVals[j] << 1;
            for (int j = 0; j < h->hx711_count; j++)
                gpio_out_write(h->clks[j], 0);
            for (int j = 0; j < h->hx711_count; j++)
                outVals[j] += (gpio_in_read(h->sdos[j]) > 0 ? 1 : 0);
        }
        for (int j = 0; j < h->hx711_count; j++)
            gpio_out_write(h->clks[j], 1);
        for (int j = 0; j < h->hx711_count; j++)
        {
            outVals[j] |= ((outVals[j] & 0x00800000) != 0 ? 0xFF000000 : 0);
        }
        for (int j = 0; j < h->hx711_count; j++)
            gpio_out_write(h->clks[j], 0);

        last_tick = timer_read_time();
        sendf("result_hx711s oid=%c vd=%c it=%c tr=%hu nt=%u v0=%i v1=%i v2=%i v3=%i", 
                             (uint8_t)h->oid, (uint8_t)is_data_valid, (uint8_t)now_inter_ms, (uint16_t)h->times_read, (uint32_t)now_tick,  (int32_t)outVals[0], (int32_t)outVals[1], (int32_t)outVals[2], (int32_t)outVals[3]);
        return 1;  
    }
    return 0;
}


void 
command_add_hx711s(uint32_t *args)
{
	uint8_t oid = args[0];
    uint8_t index = args[1];
	struct hx711s* h = oid_lookup(oid,command_config_hx711s);	
    if (index >= h->hx711_count)
        shutdown("Set hx711 past maximum count");

    h->clks[index] = gpio_out_setup(args[2], 0);
    h->sdos[index] = gpio_in_setup(args[3], 0);

    sendf("debug_hx711s oid=%c arg[0]=%u arg[1]=%u arg[2]=%u arg[3]=%u", (int)args[0], (int)args[0], (int)args[1], (int)args[2], (int)args[3]);
}
DECL_COMMAND(command_add_hx711s, "add_hx711s oid=%c index=%c clk_pin=%u sdo_pin=%u");

void 
command_query_hx711s(uint32_t *args)
{
	uint8_t oid = args[0];
	struct hx711s* h = oid_lookup(oid,command_config_hx711s);	
	h->times_read = args[1];
	h->flags |= HX711S_SAMPLE_START;
	sched_del_timer(&h->hx711s_timer);
	irq_disable();
	h->hx711s_timer.waketime = timer_read_time() + h->rest_ticks;
	sched_add_timer(&h->hx711s_timer);
	irq_enable();
}
DECL_COMMAND(command_query_hx711s, "query_hx711s oid=%c times_read=%hu");


void 
hx711s_task(void)
{
	uint8_t oid;
	struct hx711s* h;

	if(!sched_check_wake(&hx711s_wake))
	{
		return;
	}

	foreach_oid(oid,h,command_config_hx711s)
	{
    	if (!(h->flags & HX711S_SAMPLE_START) || h->times_read <= 0)
        {
			return;
		}		

    	if(get_hx711s(h) > 0)
		{
        	h->times_read--;
			if(h->times_read == 0)
			{
				h->flags &= ~HX711S_SAMPLE_START;
				//sched_del_timer(&h->hx711s_timer);	
			}
		}
	}
}
DECL_TASK(hx711s_task);

void 
hx711s_shutdown(void)
{
	uint8_t oid;
	struct hx711s* h;
	foreach_oid(oid,h,command_config_hx711s)
	{
		h->times_read = 0;
		h->flags &= ~HX711S_SAMPLE_START;
	}
	return;	
}

DECL_SHUTDOWN(hx711s_shutdown);

