// Handling of timers on linux systems
//
// Copyright (C) 2017-2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <time.h> // struct timespec
#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/io.h" // readl
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_from_us
#include "command.h" // DECL_CONSTANT
#include "internal.h" // console_sleep
#include "sched.h" // DECL_INIT

// Global storage for timer handling
static struct {
    // Last time reported by timer_read_time()
    uint32_t last_read_time;
    // Fields for converting from a systime to ticks
    time_t start_sec;
    // Flags for tracking irq_enable()/irq_disable()
    uint32_t must_wake_timers;
    // Time of next software timer (also used to convert from ticks to systime)
    uint32_t next_wake_counter;
    struct timespec next_wake;
    // Unix signal tracking
    timer_t t_alarm;
    sigset_t ss_alarm, ss_sleep;
} TimerInfo;


/****************************************************************
 * Timespec helpers
 ****************************************************************/

// Convert a 'struct timespec' to a counter value
static inline uint32_t
timespec_to_time(struct timespec ts)
{
    return ((ts.tv_sec - TimerInfo.start_sec) * CONFIG_CLOCK_FREQ
            + ts.tv_nsec / NSECS_PER_TICK);
}

// Convert an internal time counter to a 'struct timespec'
static inline struct timespec
timespec_from_time(uint32_t time)
{
    int32_t counter_diff = time - TimerInfo.next_wake_counter;
    struct timespec ts;
    ts.tv_sec = TimerInfo.next_wake.tv_sec;
    ts.tv_nsec = TimerInfo.next_wake.tv_nsec + counter_diff * NSECS_PER_TICK;
    if ((unsigned long)ts.tv_nsec >= NSECS) {
        if (ts.tv_nsec < 0) {
            ts.tv_sec--;
            ts.tv_nsec += NSECS;
        } else {
            ts.tv_sec++;
            ts.tv_nsec -= NSECS;
        }
    }
    return ts;
}

// Return the current time
static struct timespec
timespec_read(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts;
}


/****************************************************************
 * Timers
 ****************************************************************/

DECL_CONSTANT("CLOCK_FREQ", CONFIG_CLOCK_FREQ);

// Check if a given time has past
int
timer_check_periodic(uint32_t *ts)
{
    uint32_t lrt = TimerInfo.last_read_time;
    if (timer_is_before(lrt, *ts))
        return 0;
    *ts = lrt + timer_from_us(2000000);
    return 1;
}

// Return the number of clock ticks for a given number of microseconds
uint32_t
timer_from_us(uint32_t us)
{
    return us * (CONFIG_CLOCK_FREQ / 1000000);
}

// Return true if time1 is before time2.  Always use this function to
// compare times as regular C comparisons can fail if the counter
// rolls over.
uint8_t
timer_is_before(uint32_t time1, uint32_t time2)
{
    return (int32_t)(time1 - time2) < 0;
}

// Return the current time (in clock ticks)
uint32_t
timer_read_time(void)
{
    uint32_t t = timespec_to_time(timespec_read());
    TimerInfo.last_read_time = t;
    return t;
}

// Activate timer dispatch as soon as possible
void
timer_kick(void)
{
    struct itimerspec it = { .it_interval = {0, 0}, .it_value = {0, 1} };
    timer_settime(TimerInfo.t_alarm, TIMER_ABSTIME, &it, NULL);
}

#define TIMER_IDLE_REPEAT_COUNT 100
#define TIMER_REPEAT_COUNT 20

#define TIMER_MIN_TRY_TICKS timer_from_us(2)

// Invoke timers
static void
timer_dispatch(void)
{
    uint32_t repeat_count = TIMER_REPEAT_COUNT, next;
    for (;;) {
        // Run the next software timer
        next = sched_timer_dispatch();

        repeat_count--;
        uint32_t lrt = TimerInfo.last_read_time;
        if (!timer_is_before(lrt, next) && repeat_count)
            // Can run next timer without overhead of calling timer_read_time()
            continue;

        uint32_t now = timer_read_time();
        int32_t diff = next - now;
        if (diff > (int32_t)TIMER_MIN_TRY_TICKS)
            // Schedule next timer normally.
            break;

        if (unlikely(!repeat_count)) {
            // Check if there are too many repeat timers
            if (diff < (int32_t)(-timer_from_us(100000)))
                try_shutdown("Rescheduled timer in the past");
            if (sched_check_set_tasks_busy())
                return;
            repeat_count = TIMER_IDLE_REPEAT_COUNT;
        }

        // Next timer in the past or near future - wait for it to be ready
        while (unlikely(diff > 0))
            diff = next - timer_read_time();
    }

    // Schedule SIGALRM signal
    struct itimerspec it;
    it.it_interval = (struct timespec){0, 0};
    TimerInfo.next_wake = it.it_value = timespec_from_time(next);
    TimerInfo.next_wake_counter = next;
    TimerInfo.must_wake_timers = 0;
    timer_settime(TimerInfo.t_alarm, TIMER_ABSTIME, &it, NULL);
}

// OS signal handler
static void
timer_signal(int signal)
{
    TimerInfo.must_wake_timers = 1;
}

void
timer_init(void)
{
    // Initialize ss_alarm signal set
    int ret = sigemptyset(&TimerInfo.ss_alarm);
    if (ret < 0) {
        report_errno("sigemptyset", ret);
        return;
    }
    ret = sigaddset(&TimerInfo.ss_alarm, SIGALRM);
    if (ret < 0) {
        report_errno("sigaddset", ret);
        return;
    }
    // Initialize ss_sleep signal set
    ret = sigprocmask(0, NULL, &TimerInfo.ss_sleep);
    if (ret < 0) {
        report_errno("sigprocmask ss_sleep", ret);
        return;
    }
    ret = sigdelset(&TimerInfo.ss_sleep, SIGALRM);
    if (ret < 0) {
        report_errno("sigdelset", ret);
        return;
    }
    // Initialize timespec_to_time() and timespec_from_time()
    struct timespec curtime = timespec_read();
    TimerInfo.start_sec = curtime.tv_sec + 1;
    TimerInfo.next_wake = curtime;
    TimerInfo.next_wake_counter = timespec_to_time(curtime);
    // Initialize t_alarm signal based timer
    ret = timer_create(CLOCK_MONOTONIC, NULL, &TimerInfo.t_alarm);
    if (ret < 0) {
        report_errno("timer_create", ret);
        return;
    }
    struct sigaction act = {.sa_handler = timer_signal, .sa_flags = SA_RESTART};
    ret = sigaction(SIGALRM, &act, NULL);
    if (ret < 0) {
        report_errno("sigaction", ret);
        return;
    }
    timer_kick();
}
DECL_INIT(timer_init);

// Block SIGALRM signal
void
timer_disable_signals(void)
{
    sigprocmask(SIG_BLOCK, &TimerInfo.ss_alarm, NULL);
}

// Restore reception of SIGALRM signal
void
timer_enable_signals(void)
{
    sigprocmask(SIG_UNBLOCK, &TimerInfo.ss_alarm, NULL);
}


/****************************************************************
 * Interrupt wrappers
 ****************************************************************/

void
irq_disable(void)
{
}

void
irq_enable(void)
{
}

irqstatus_t
irq_save(void)
{
    return 0;
}

void
irq_restore(irqstatus_t flag)
{
}

void
irq_wait(void)
{
    // Must atomically sleep until signaled
    if (!readl(&TimerInfo.must_wake_timers)) {
        timer_disable_signals();
        if (!TimerInfo.must_wake_timers)
            console_sleep(&TimerInfo.ss_sleep);
        timer_enable_signals();
    }
    irq_poll();
}

void
irq_poll(void)
{
    if (readl(&TimerInfo.must_wake_timers))
        timer_dispatch();
}
