#include "sched.h" // DECL_CONSTANT
#include "command.h" // shutdown

#include <lpc17xx_wdt.h>

// debug:
#include "pins_MKS.h"

// TODO FIXME : Watchdog is not working correctly!!

/****************************************************************
 * watchdog handler
 ****************************************************************/

void WDT_IRQHandler(void) {
    DEBUG_OUT("shutdown (Watchdog reset)\n");
    shutdown("Watchdog reset");
}

void
watchdog_reset(void)
{
    WDT_Feed();
}
DECL_TASK(watchdog_reset);

void
watchdog_init(void)
{
    //uint32_t const timeout = 500 * 1000;  // 500ms timeout in us
    //uint32_t const timeout = 5000000;  // 5sec timeout in us
    uint32_t const timeout = 2000000;  // 2sec timeout in us
    WDT_Init(WDT_CLKSRC_IRC, WDT_MODE_INT_ONLY /*WDT_MODE_RESET*/);
    //WDT_Start(timeout);
    DEBUG_OUT("watchdog_init (2s) done\n");
}
DECL_INIT(watchdog_init);
