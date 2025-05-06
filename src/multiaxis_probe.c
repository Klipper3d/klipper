// Handling of multiaxis probe.

#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // struct gpio
#include "board/irq.h" // irq_disable
#include "command.h" // DECL_COMMAND
#include "sched.h" // struct timer
#include "stepper.h" // stepper_stop_all

struct multiaxis_probe {
    struct timer time;
    struct gpio_in pin;
    uint32_t rest_time, nextwake;
    uint8_t pin_value, sample_count;
};

static uint_fast8_t multiaxis_probe_event(struct timer *t)
{
    struct multiaxis_probe *e = container_of(t, struct multiaxis_probe, time);
    uint8_t val = gpio_in_read(e->pin);
    uint32_t nextwake = e->time.waketime + e->rest_time;
    if(val ? ~e->pin_value : e->pin_value){
        // Probe not trigger, reschedule for the next attempt
        e->time.waketime = nextwake;
        return SF_RESCHEDULE;
    }
    // Probe triggered
    stepper_stop_all();
    return SF_DONE;
}

void command_config_multiaxis_probe(uint32_t *args)
{
    struct multiaxis_probe *e = oid_alloc(args[0], command_config_multiaxis_probe, sizeof(*e));
    e->pin = gpio_in_setup(args[1], args[2]);
}
DECL_COMMAND(command_config_multiaxis_probe, "config_multiaxis_probe oid=%c pin=%c pull_up=%c");

void command_multiaxis_probe_clear(uint32_t *args)
{
    struct multiaxis_probe *e = oid_lookup(args[0], command_config_multiaxis_probe);
    sched_del_timer(&e->time);
}
DECL_COMMAND(command_multiaxis_probe_clear, "multiaxis_probe_clear oid=%c");

void command_multiaxis_probe_start(uint32_t *args)
{
    struct multiaxis_probe *e = oid_lookup(args[0], command_config_multiaxis_probe);
    sched_del_timer(&e->time);
    e->time.waketime = args[1];
    e->time.func = multiaxis_probe_event;

    e->rest_time = args[2];
    e->pin_value = args[3];
    sched_add_timer(&e->time);
}
DECL_COMMAND(command_multiaxis_probe_start, "multiaxis_probe_start oid=%c clock=%u rest_ticks=%u pin_value=%c");

void command_multiaxis_probe_query_state(uint32_t *args)
{
    uint8_t oid = args[0];
    struct multiaxis_probe *e = oid_lookup(oid, command_config_multiaxis_probe);

    irq_disable();
    uint32_t nextwake = e->nextwake;
    irq_enable();

    sendf("multiaxis_probe_state oid=%c next_clock=%u pin_value=%c"
          , oid, nextwake, gpio_in_read(e->pin));
}
DECL_COMMAND(command_multiaxis_probe_query_state, "multiaxis_probe_query_state oid=%c");
