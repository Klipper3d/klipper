// Support for plasma cutter control
//
// Copyright (C) 2020  Lucas Felix <lucas.felix0738@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK
#include "board/gpio.h" // i2c_setup, i2c_write, i2c_read
#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/misc.h" // timer_read_time

#define PLASMA_OFF 0
#define PLASMA_ON  1

// Frequency at which plasma state is monitored to detect arc losses.
// Plasma cutter internal transfer relay shown bounces as low as 20Hz, thus
// prefer frequency lower than this to avoid false positive.
#define MONITOR_FREQ 5
#define SEND_STATUS_FREQ 5

#define MSG_STATUS_BIT 0
#define MSG_ERROR_BIT  1

#define STATUS_OFF 0
#define STATUS_ON  1

#define ERROR_NONE          0
#define ERROR_NO_TRANSFER   1
#define ERROR_TRANSFER_LOST 2
#define ERROR_UNDEFINED     3 // free for future use

enum plasma_cmd {TURN_ON, TURN_OFF};

struct plasma {
    uint8_t status, error, msg, seq;
    enum plasma_cmd cmd;

    struct timer setup_timer;
    struct timer monitor_timer;
    struct timer send_status_timer;

    struct gpio_out start_pin;
    struct gpio_in transfer_pin;
    uint8_t transfer_invert;

    uint32_t ticks_to_timeout;
};

static struct task_wake start_wake;
static struct task_wake send_status_wake;

// loop timer for sending messages
static uint_fast8_t
send_status_event(struct timer *timer)
{
    timer->waketime += CONFIG_CLOCK_FREQ / SEND_STATUS_FREQ;
    sched_wake_task(&send_status_wake);
    return SF_RESCHEDULE;
}

// enable message sending until ack is receive
void
send_status(struct plasma *p)
{
    sched_del_timer(&p->send_status_timer);

    p->msg = p->status << MSG_STATUS_BIT | p->error << MSG_ERROR_BIT;
    sched_wake_task(&send_status_wake);

    p->send_status_timer.func = send_status_event;
    p->send_status_timer.waketime = timer_read_time()
        + CONFIG_CLOCK_FREQ / SEND_STATUS_FREQ;
    sched_add_timer(&p->send_status_timer);
}

static uint_fast8_t
monitor_event(struct timer *timer)
{
    struct plasma *p = container_of(timer, struct plasma, monitor_timer);
    if (!!(gpio_in_read(p->transfer_pin)) != p->transfer_invert) {
        timer->waketime += CONFIG_CLOCK_FREQ / MONITOR_FREQ;
        return SF_RESCHEDULE;
    }
    else {
        gpio_out_write(p->start_pin, PLASMA_OFF);
        p->error = ERROR_TRANSFER_LOST;
        send_status(p);
        return SF_DONE;
    }
}

void start_plasma(struct plasma *p)
{
    if(p->status != STATUS_OFF) {
        shutdown("Prevent plasma starting twice.");
    }
    p->status = STATUS_ON;
    p->error = ERROR_NONE;

    // turn plasma on
    gpio_out_write(p->start_pin, PLASMA_ON);

    // freeze time while waiting for arc transfer
    uint8_t transfer = 0;
    time_freeze(p->ticks_to_timeout);
    while(!transfer && is_time_frozen()) {
        time_frozen_idle();
        transfer = !!(gpio_in_read(p->transfer_pin)) != p->transfer_invert;
    }
    uint32_t clock_drift = time_unfreeze();
    sendf("clock_drift clock=%u", clock_drift);

    // check if wait has timed out or not
    if(!transfer) { // stop plasma and set error flag
        gpio_out_write(p->start_pin, PLASMA_OFF);
        p->error = ERROR_NO_TRANSFER;
        send_status(p);
    }
    else { // start plasma monitoring
        p->monitor_timer.func = monitor_event;
        p->monitor_timer.waketime = timer_read_time() +
                                    CONFIG_CLOCK_FREQ / MONITOR_FREQ;
        sched_add_timer(&p->monitor_timer);
    }
}

void stop_plasma(struct plasma *p)
{
        gpio_out_write(p->start_pin, PLASMA_OFF);
        sched_del_timer(&p->monitor_timer);
        p->status = STATUS_OFF;
        send_status(p);
}

static uint_fast8_t
setup_event(struct timer *timer)
{
    struct plasma *p = container_of(timer, struct plasma, setup_timer);
    if(p->cmd == TURN_ON)
    {
        // start has to be ran into task to ensure
        // no other task is running when time freeze
        sched_wake_task(&start_wake);
    }
    else // if(p->cmd == TURN_OFF)
    {
        stop_plasma(p);
    }
    return SF_DONE;
}

void
command_config_plasma(uint32_t *args)
{
    struct plasma *p = oid_alloc(args[0], command_config_plasma, sizeof(*p));
    p->start_pin = gpio_out_setup(args[1], PLASMA_OFF);
    p->transfer_pin = gpio_in_setup(args[2], args[3]);
    p->transfer_invert = !!(args[4]);
    p->ticks_to_timeout = timer_from_us(1000) * args[5];

    p->status = STATUS_OFF;
    p->error = ERROR_NONE;
    p->seq = 255;
}
DECL_COMMAND(command_config_plasma,
             "config_plasma oid=%c start_pin=%u transfer_pin=%u"
             " transfer_pullup=%c transfer_invert=%c transfer_timeout_ms=%u");

void
command_plasma_start(uint32_t *args)
{
    struct plasma *p = oid_lookup(args[0], command_config_plasma);
    p->cmd = TURN_ON;
    sched_del_timer(&p->setup_timer);
    p->setup_timer.func = setup_event;
    p->setup_timer.waketime = args[1];
    sched_add_timer(&p->setup_timer);
}
DECL_COMMAND(command_plasma_start, "plasma_start oid=%c clock=%u");

void
command_plasma_stop(uint32_t *args)
{
    struct plasma *p = oid_lookup(args[0], command_config_plasma);
    p->cmd = TURN_OFF;
    sched_del_timer(&p->setup_timer);
    p->setup_timer.func = setup_event;
    p->setup_timer.waketime = args[1];
    sched_add_timer(&p->setup_timer);
}
DECL_COMMAND(command_plasma_stop, "plasma_stop oid=%c clock=%u");

void
command_plasma_ack(uint32_t *args)
{
    struct plasma *p = oid_lookup(args[0], command_config_plasma);

    // stop response loop if current message acked
    if(args[1] == p->seq) {
        sched_del_timer(&p->send_status_timer);
    }
}
DECL_COMMAND(command_plasma_ack, "plasma_status_ack oid=%c seq=%c");

void
start_task(void)
{
    if (!sched_check_wake(&start_wake))
        return;

    uint8_t oid;
    struct plasma *p;
    // TODO only for concerned oid
    foreach_oid(oid, p, command_config_plasma) {
        start_plasma(p);
    }
}
DECL_TASK(start_task);

void
send_status_task(void)
{
    if (!sched_check_wake(&send_status_wake))
        return;

    uint8_t oid;
    struct plasma *p;
    // TODO only for concerned oid
    foreach_oid(oid, p, command_config_plasma) {
        p->seq++;
        sendf("plasma_status seq=%c status=%c", p->seq, p->msg);
    }
}
DECL_TASK(send_status_task);

void
plasma_shutdown(void)
{
    uint8_t oid;
    struct plasma *p;
    foreach_oid(oid, p, command_config_plasma) {
        gpio_out_write(p->start_pin, PLASMA_OFF);
    }
}
DECL_SHUTDOWN(plasma_shutdown);
