#include "timer.h"
#include "command.h"
#include "esp_attr.h"
#include "esp_cpu.h"
#include "esp_intr_alloc.h"
#include "esp_rom_sys.h"
#include "board/timer_irq.h"
#include "xtensa/core-macros.h"

/**
 * Klipper's ESP32 timer.
 *
 * This uses the clock cycle counter register (CCOUNT), that is automatically
 * incremented on each clock cycle. One tick is equal to 1s / clock_freq.
 *
 * It routes and enables an interrupt that triggers each time that CCOUNT is
 * equal to CCOMPAREn. The ISR takes care of dispatching klipper's timers.
 *
 * There are some "magic numbers" defined below. Those come from intr_alloc.c
 * and esp_intr_alloc.h, in particular get_available_int(). I have not been
 * able to find them in any technical document, but it is possible to enable
 * DEBUG_INT_ALLOC_DECISIONS in intr_alloc.c to debug and get them for any
 * other chip compatible with the SDK. If you're porting it to another MCU
 * and it does not work, check this!
 *
 * Care must be taken to ensure that no other code messes with this interrupt,
 * as ESP-IDF might want to reallocate it if using the drivers normally. I have
 * not followed it enough, but hey, Here be dragons!
 */

#define TIMER_CCOMPARE_NO       1
#define TIMER_CCOMP_INTR_SRC    (ETS_INTERNAL_TIMER1_INTR_SOURCE)
#define TIMER_CCOMP_INTR_NO     (15)                                // ETS_INTERNAL_TIMER1_INTR_NO

static void timer_set_ccompare(uint32_t next);

/**
 * Timer's ISR.
 * Dispatchs klipper's timers.
 */
static void IRAM_ATTR timer_isr()
{
    uint32_t next = timer_dispatch_many();
    timer_set_ccompare(next);
}

/**
 * Timer init.
 *
 * Initializes the timer and makes sure it is in a safe state before enabling
 * its corresponding interrupt.
 */
void timer_init()
{
    // Disable interrupt in case it's enabled
    ESP_INTR_DISABLE(TIMER_CCOMP_INTR_NO);

    /**
     * Route the interrupt source to the interrupt number and register the ISR.
     * The interrupt number is usually handled by get_available_int() in
     * intr_alloc.c, in this case we'll just hardcode it. Beware this is the
     * case for the ESP32 only.
     */
    esp_rom_route_intr_matrix(0, TIMER_CCOMP_INTR_SRC, TIMER_CCOMP_INTR_NO);
    esp_cpu_intr_set_handler(TIMER_CCOMP_INTR_NO, timer_isr, NULL);

    // Sometimes the registers end up in an unsafe state so we'll reset them
    XTHAL_SET_CCOUNT(0);
    timer_set_ccompare(0);

    // Kick the timer and enable the interrupt
    timer_kick();
    ESP_INTR_ENABLE(TIMER_CCOMP_INTR_NO);
}
DECL_INIT(timer_init);

/**
 * Get the current timestamp as a 32-bit field (it wraps around when overflown)
 */
uint32_t timer_read_time()
{
    return XTHAL_GET_CCOUNT();
}

/**
 * Set the CCOMPAREn register which triggers timer_isr when due.
 *
 * @param next next timestamp (in absolute clock cycles, wraps around)
 * @static internal only.
 */
static void timer_set_ccompare(uint32_t next)
{
    XTHAL_SET_CCOMPARE(TIMER_CCOMPARE_NO, next);
}

/**
 * Kick the timer.
 * Schedules the next interrupt 50us from now.
 */
void timer_kick()
{
    timer_set_ccompare(timer_read_time() + (esp_rom_get_cpu_ticks_per_us() * 50ul));
}
