#include "timer.h"
#include "sched.h"
#include "autoconf.h"
#include "timer_irq.h"
#include "irq.h"
#include "esp_err.h"
#include "driver/gptimer.h"
#include "esp_attr.h"
#include "generic/misc.h"

static gptimer_handle_t gptimer = NULL;

/**
 * Init a GPTimer to use as a base for Klipper timer events.
 * If the current performance is not enough, it should be possible to bypass
 * the driver layer and use the HAL/LL layer directly, which I think should
 * save us a couple cycles as it would be just the ISR and not a callback.
 * I think it should be enough though. Time will tell :)
 *
 * Since all ESP32 timers are 64 bits, we will just use a freerunning timer
 * and
 *CONFIG_INLINE_STEPPER_HACK
    // @todo llevar reloj del CPU a 240MHz
    // @todo funcion de shutdown
    // @todo ver si podemos volar la interrupción de freertos para evitar jitter
 * See also:
 * https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/gptimer.html
 */
DECL_INIT(timer_init);
void timer_init()
{
    ESP_ERROR_CHECK(gptimer_new_timer(
        &(gptimer_config_t) {
            .clk_src = GPTIMER_CLK_SRC_DEFAULT,
            .direction = GPTIMER_COUNT_UP,
            .resolution_hz = CONFIG_CLOCK_FREQ,
            .intr_priority = 3, // Max priority
        },
        &gptimer
    ));

    ESP_ERROR_CHECK(gptimer_register_event_callbacks(
        gptimer,
        &(gptimer_event_callbacks_t) {
            .on_alarm = timer_alarm
        },
        NULL
    ));

    timer_set_alarm(0);

    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));

    timer_kick();
}

// @todo OJO con posibles cache misses pueden retrasar operaciones
// @todo ver https://esp32.com/viewtopic.php?t=30399
// @todo ver CONFIG_APP_BUILD_TYPE_ELF_RAM
static bool IRAM_ATTR timer_alarm(gptimer_handle_t timer, const gptimer_alarm_event_data_t* edata, void* user_ctx)
{
    irq_disable();
    uint32_t next = timer_dispatch_many();
    timer_set_alarm(next);
    irq_enable();

    /*
    * The returned value tells the scheduler if it has to wake a high priority
    * task. As we're trying to avoid FreeRTOS as much as possible, we'll just
    * avoid the context-switching attempt.
    */
    return false;
}

// Interfaces that must be provided for the core code
// Todo rewrite?
void timer_kick()
{
    timer_set_alarm(timer_read_time() + 50);
}

/**
 * Returns the current timestamp as a 32-bit value.
 *
 * Internally ESP timers are 64 bits, but we want to emulate 32 bit behavior
 * so that wrap-around is preserved.
 */
uint32_t timer_read_time()
{
    return (uint32_t)(timer_read_time_u64() & U64_MASK_LO);
}

/**
 * Returns the current timestamp as a 64-bit value.
 *
 * @static Internal only, as klipper uses 32-bit timers.
 */
static uint64_t timer_read_time_u64()
{
    uint64_t count;
    ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &count));
    return count;
}

/**
 * Set the next gptimer alarm (64-bit) based on a 32-bit timestamp.
 *
 * Instead of relying on different timers to emulate 32-bit behavior we let the
 * gptimer run in a 64-bit domain and add the "32-bit epoch" to the alarm.
 * See https://esp32.com/viewtopic.php?p=52247#p52247
 */
static void timer_set_alarm(const uint32_t next_alarm)
{
    const uint64_t current_time = timer_read_time_u64();

    // Reconstruct u64 from u32: add current epoch + 1 if we wrapped around
    uint64_t next_alarm_u64 = (current_time & U64_MASK_HI) + (next_alarm & U64_MASK_LO);
    if (next_alarm < (uint32_t)(current_time & U64_MASK_LO)) {
        next_alarm_u64 += (1ull << 32);
    }

    ESP_ERROR_CHECK(gptimer_set_alarm_action(
        gptimer,
        &(gptimer_alarm_config_t) {
            .alarm_count = next_alarm_u64,
            .flags.auto_reload_on_alarm = false,
        }
    ));
}
