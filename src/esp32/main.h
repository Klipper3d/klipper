#pragma once

#include "sdkconfig.h"

#if (!defined(CONFIG_FREERTOS_UNICORE) || !CONFIG_FREERTOS_UNICORE)
    #error Klipper needs CONFIG_FREERTOS_UNICORE enabled
#endif

void app_main();

static void neutralize_freertos();
static void neutralize_freertos_twdt();
static void task_call_sched_main();
