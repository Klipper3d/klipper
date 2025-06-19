/*
#################################################################
Klipper Touch Sensor Module @ Albatross
Description: This module provides low level kllipper interface for spi-based touch sensor (MCP3462R-ADC)
Date: 2025-06-05
Version: 0.1
Owner: Mo
#################################################################
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "board/gpio.h" // irq_disable
#include "board/irq.h" // irq_disable

#include "basecmd.h" // oid_alloc
#include "board/misc.h" // timer_read_time
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK
#include "spicmds.h" // spidev_transfer
#include "touch_sensor_mcp3462r.h"

static struct mcp3462r_adc *mcp_adc_ptr = NULL;

int8_t
mcp3462r_is_data_ready(struct mcp3462r_adc *mcp3462r)
{
    return gpio_in_read(mcp3462r->adc_ready_pin) == 0;
}


static uint_fast8_t
mcp3462r_event(struct timer *t)
{
    uint16_t doneP =0, data =0;
    struct mcp3462r_adc *mcp_adc_ptr = container_of(t, struct mcp3462r_adc, timer);
    output("Touch sensor ADC event triggered at cycle= %u",  mcp_adc_ptr->timeout_cycles);
    if (mcp3462r_is_data_ready(mcp_adc_ptr)) 
    {
        // Read ADC data
        mcp_adc_ptr->msg[0] = 0x00;
        mcp_adc_ptr->msg[1] = 0x00;
        spidev_transfer(mcp_adc_ptr->spi, 1, 2, mcp_adc_ptr->msg);
        // Process the raw values here
        output("Got new raw ADC data: %u %u at cycle= %u", mcp_adc_ptr->msg[0], mcp_adc_ptr->msg[1], mcp_adc_ptr->timeout_cycles); 
        data = (mcp_adc_ptr->msg[0] << 8) | mcp_adc_ptr->msg[1];
        if (data < mcp_adc_ptr->sensitivity) 
        {
            // Trigger the touch event
            gpio_out_write(mcp_adc_ptr->trigger_out_pin, 1);
            // This is the last cycle
            mcp_adc_ptr->timeout_cycles =1;
            doneP = 1;

        } else {
            // No touch detected, reset the output pin
            gpio_out_write(mcp_adc_ptr->trigger_out_pin, 0);
        }
    }

    mcp_adc_ptr->timer.waketime += mcp_adc_ptr->rest_ticks;
    if (--mcp_adc_ptr->timeout_cycles == 0) {
        // Timeout reached, stop the task
        mcp_adc_ptr->active_session_flag = 0;
        // sched_del_timer(&mcp_adc_ptr->timer);
        sendf("Ts_session_result oid=%c status=%u lstValue=%u", mcp_adc_ptr->oid, doneP,data);
        output("Ts_session_result status=%u lstValue=%u", doneP,data);
        return SF_DONE;
    }
    return SF_RESCHEDULE;
}

void
command_cfg_ts_adc(uint32_t *args)
{
    mcp_adc_ptr = oid_alloc(args[0], command_cfg_ts_adc, sizeof(*mcp_adc_ptr));
    mcp_adc_ptr->oid = args[0];
    mcp_adc_ptr->spi = spidev_oid_lookup(args[1]);
    mcp_adc_ptr->adc_ready_pin = gpio_in_setup(args[2], 0);
    mcp_adc_ptr->trigger_out_pin = gpio_out_setup(args[3], 0);
    mcp_adc_ptr->timer.func = mcp3462r_event;
    mcp_adc_ptr->active_session_flag = 0;
    mcp_adc_ptr->configured_flag = 1;
    mcp_adc_ptr->msg[0] = 0x00;
    mcp_adc_ptr->msg[1] = 0x00;
    gpio_out_write(mcp_adc_ptr->trigger_out_pin, 0);
    output("Touch sensor ADC configured with OID=%c, SPI OID=%c, ADC ready pin=%u, Trigger out pin=%u",
         mcp_adc_ptr->oid, args[1], args[2], args[3]);
}

DECL_COMMAND(command_cfg_ts_adc, "cfg_ts_adc oid=%c spi_oid=%c adc_int_pin=%u trigger_out_pin=%u");



/*
// This function is the cmd that is initiated by the user to wake up
// a timered event for reading the value and  doing the low level logic
*/
void
command_start_touch_sensing_session(uint32_t *args)
{
    if (mcp_adc_ptr == NULL || !mcp_adc_ptr->configured_flag) 
        shutdown("Touch sensor ADC HW is not configured");
    if (mcp_adc_ptr->oid != args[0]) 
        shutdown("Touch sensor ADC OID does not match the configured OID");
    
    mcp_adc_ptr->timeout_cycles = args[1];
    mcp_adc_ptr->rest_ticks = args[2];
    mcp_adc_ptr->sensitivity = args[3];
    output("Starting touch sensing session with OID=%c, timeout_cycles=%u, rest_ticks=%u, sensitivity=%u\n",
         mcp_adc_ptr->oid, mcp_adc_ptr->timeout_cycles, mcp_adc_ptr->rest_ticks, mcp_adc_ptr->sensitivity);
    if (!mcp_adc_ptr->timeout_cycles || !mcp_adc_ptr->rest_ticks) 
        shutdown("Timeout cycles and rest ticks must be greater than 0");
    if (mcp_adc_ptr->sensitivity == 0) 
        shutdown("Sensitivity must be greater than 0");
    if (mcp_adc_ptr->active_session_flag) 
        shutdown("Touch sensing session is already active");
    // Send a reading command to the MCP3462R ADC
    mcp_adc_ptr->msg[0] = READ_CMD;
    spidev_transfer(mcp_adc_ptr->spi, 0, 1, &mcp_adc_ptr->msg[0]);
    mcp_adc_ptr->active_session_flag = 1;
    // Start the periodic event to listen for data ready
    sched_del_timer(&mcp_adc_ptr->timer);
    irq_disable();
    mcp_adc_ptr->timer.waketime = timer_read_time() + mcp_adc_ptr->rest_ticks;
    sched_add_timer(&mcp_adc_ptr->timer);
    irq_enable();
}

DECL_COMMAND(command_start_touch_sensing_session,
             "start_ts_session oid=%c timeout_cycles=%u rest_ticks=%u sensitivity=%u");

