// Commands for controlling hardware based pulse-width-modulator pins
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // struct gpio_pwm
#include "board/irq.h" // irq_disable
#include "command.h" // DECL_COMMAND
#include "sched.h" // sched_add_timer

//FIXME: DEBUG
#define GPIO(PORT, NUM) (((PORT)-'A') * 8 + (NUM))

struct pwm_value {
	uint32_t waketime;
	uint16_t value;
	struct pwm_value *next;
};

struct pwm_out_s {
    struct timer timer;
    struct gpio_pwm pin;
    uint32_t max_duration;
    uint16_t default_value;
    struct pwm_value *first, **plast;
    //FIXME: debugging
    struct gpio_out debug5, debug7;
};


static uint_fast8_t
pwm_end_event(struct timer *timer)
{
    shutdown("Missed scheduling of next hard pwm event");
}

static uint_fast8_t
pwm_event(struct timer *timer)
{
    struct pwm_out_s *p = container_of(timer, struct pwm_out_s, timer);
    struct pwm_value *v = p->first;

    //fixme: Debug
    gpio_out_write(p->debug5, 1);

    if(!v)
    {
        // no next pwm value, queue is empty
    	// this should not happen because we check the v->next
        //fixme: Debug
        gpio_out_write(p->debug5, 0);
        return SF_DONE;
    }

    gpio_pwm_write(p->pin, v->value);

    p->first = v->next;		//v->next may be NULL if no further elements are queued

    if(p->first){
        //next event scheduled
        p->timer.waketime = p->first->waketime;

        //fixme: Debug
        gpio_out_write(p->debug7, 1);

    } else {
    	// no next event schedlued
        if (v->value == p->default_value || !p->max_duration) {
        	//We either have set the default value or there is no maximum duration
        	irq_disable();
            pwm_free(v);
            irq_enable();
            //fixme: Debug
            gpio_out_write(p->debug5, 0);
            return SF_DONE;
        }

        //nothing scheduled, so lets start the safety timeout
        p->timer.waketime += p->max_duration;
        p->timer.func = pwm_end_event;
    }

    //fixme: Debug
    gpio_out_write(p->debug5, 0);
    gpio_out_write(p->debug7, 0);

	irq_disable();
    pwm_free(v);
    irq_enable();

    return SF_RESCHEDULE;
}

void
command_config_pwm_out(uint32_t *args)
{
    struct gpio_pwm pin = gpio_pwm_setup(args[1], args[2], args[3]);
    struct pwm_out_s *p = oid_alloc(args[0], command_config_pwm_out
                                    , sizeof(*p));
    p->pin = pin;
    p->default_value = args[4];
    p->max_duration = args[5];
    p->first = NULL;
    p->plast = NULL;

    pwm_request_size(sizeof(struct pwm_value));


    //FIXME: debugging gpios
    p->debug5 = gpio_out_setup(GPIO('D', 5), 0);
    p->debug7 = gpio_out_setup(GPIO('D', 7), 0);
}
DECL_COMMAND(command_config_pwm_out,
             "config_pwm_out oid=%c pin=%u cycle_ticks=%u value=%hu"
             " default_value=%hu max_duration=%u");

void
command_schedule_pwm_out(uint32_t *args)
{

    struct pwm_out_s *p = oid_lookup(args[0], command_config_pwm_out);

    gpio_out_write(p->debug7, 1);
    gpio_out_write(p->debug5, 1);

    struct pwm_value* v = pwm_alloc();
    v->waketime = args[1];
    v->value = args[2];
    v->next = NULL;

    gpio_out_toggle_noirq(p->debug5);

    irq_disable();
    if(p->first) {
        // there exists an element in queue

        //fixme: debug
        gpio_out_toggle_noirq(p->debug5);

        //if there is a p->first, there has to be a p->plast
        //if there is no first, plast is invalid.
        *p->plast = v;	//enqueue new v into the last's "next" element
    }
    else {
        // no first element set
        p->first = v;
        p->timer.waketime = args[1];
        p->timer.func = pwm_event;
        sched_add_timer(&p->timer);
    }

    p->plast = &v->next;	//plast to point to this new element's next pointer
    irq_enable();

    //fixme: debug
    gpio_out_write(p->debug7, 0);
    gpio_out_write(p->debug5, 0);
}
DECL_COMMAND(command_schedule_pwm_out,
             "schedule_pwm_out oid=%c clock=%u value=%hu");

void
pwm_shutdown(void)
{
    uint8_t i;
    struct pwm_out_s *p;
    foreach_oid(i, p, command_config_pwm_out) {
        gpio_out_write(p->debug7, 1);
        gpio_out_write(p->debug5, 1);

        gpio_pwm_write(p->pin, p->default_value);

        gpio_out_write(p->debug7, 0);
        gpio_out_write(p->debug5, 0);
    }
}
DECL_SHUTDOWN(pwm_shutdown);

void
command_set_pwm_out(uint32_t *args)
{
    gpio_pwm_setup(args[0], args[1], args[2]);
}
DECL_COMMAND(command_set_pwm_out,
             "set_pwm_out pin=%u cycle_ticks=%u value=%hu");
