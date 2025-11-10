#include "main.h"

#include <string.h>

#include "command.h"
#include "esp_log.h"
#include "esp_task.h"
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "sched.h"

void app_main()
{
    // @todo the stack depth is a wild guess, check it
    xTaskCreate(
        task_call_sched_main,
        "sched_main",
        8192,
        NULL,
        ESP_TASK_PRIO_MAX - 1,
        NULL
    );
}

static void task_call_sched_main()
{
    // Give time to the IDLE task to reap app_main's memory before start breaking stuff :)
    vTaskDelay(pdMS_TO_TICKS(1000));
    neutralize_freertos();

    // Main klipper loop
    sched_main();

    // If we're here, something went wrooong
    try_shutdown("main loop finished");
    // @todo implementar esp restart como tarea en el shutdown
    esp_restart();
    vTaskDelete(NULL);
}

/**
 * ESP-IDF by default initializes a set of tasks that include FreeRTOS
 * housekeeping (2x IDLEx), Inter-Processor Call (2x IPCx) and the main task
 * (app_main). Using CONFIG_FREERTOS_UNICORE we can get rid of the IPC stuff,
 * and because the idle task only takes care of freeing memory from other
 * tasks, we can starve it (per the docs) to effectively get rid of all
 * context switches in our main task.
 *
 * @todo see if we can reduce FreeRTOS' frequency and if it's effective at
 * avoiding interrupts
 *
 * As a reference, our current tasks look like this (vTaskList()):
 * Name     State   Priority    StackSize   Num     CoreID
 * *******************************************************
 * main     X       1           1220        1       0
 * IDLE     R       0           1240        2       0
 *
 * See also:
 * https://docs.espressif.com/projects/esp-idf/en/v5.5.1/esp32/api-reference/system/freertos.html#id3
 * https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/01-Tasks-and-co-routines/15-Idle-task
 */
static void neutralize_freertos()
{
    // Neutralize the TWDT for some tasks so that when we starve them we don't get an exception
    neutralize_freertos_twdt();
}

/**
 * Disables the Task Watchdog Timer (TWDT) for the tasks that we will later
 * starve. See the docs at neutralize_freertos().
 *
 * See also:
 * https://docs.espressif.com/projects/esp-idf/en/v5.5.1/esp32/api-reference/system/wdts.html#task-watchdog-timer-twdt
 */
static void neutralize_freertos_twdt()
{
    UBaseType_t tasks_n = uxTaskGetNumberOfTasks();
    TaskStatus_t tasks[tasks_n];
    uxTaskGetSystemState(tasks, tasks_n, NULL);

    for (size_t i = 0; i < tasks_n; i++)
    {
        if (strcmp(tasks[i].pcTaskName, "IDLE") == 0)
        {
            ESP_LOGW(__FILE__, "Stopping TWDT for task %s", tasks[i].pcTaskName);
            // @todo esp_task_wdt_deinit() ?
            esp_task_wdt_delete(tasks[i].xHandle);
            return;
        }
    }
}


// @todo: ??
DECL_COMMAND_FLAGS(command_reset, HF_IN_SHUTDOWN, "reset");

void command_reset(uint32_t* args)
{
    // timer_reset();
    // restore_data();
    // void *reset = (void *)RESET_VECTOR;
    // goto *reset;
}
