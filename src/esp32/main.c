#include "main.h"

#include "command.h"
#include "esp_private/startup_internal.h"
#include "esp_private/system_internal.h"
#include "esp_rom_sys.h"

/**
 * Hook into ESP secondary init functions to avoid initializing FreeRTOS [1].
 *
 * [1] https://github.com/fermino/esp-idf-example-no-freertos/blob/main/main/esp-no-freertos.c
 */
ESP_SYSTEM_INIT_FN(main_core0, SECONDARY, BIT(0), 1000)
{
    // Let UART finish sending init buffer's content
    esp_rom_delay_us(500 * 1000);

    sched_main();
    try_shutdown("Main stopped.");
    esp_restart_noos();
}

void command_reset(uint32_t* args) { esp_restart_noos(); }
DECL_COMMAND_FLAGS(command_reset, HF_IN_SHUTDOWN, "reset");

/**
 * Placeholder to avoid linker errors
 */
void app_main() { esp_restart_noos(); }
