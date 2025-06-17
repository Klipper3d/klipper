/*
#################################################################
Klipper Touch Sensor Module @ Albatross
Description: This module provides low level kllipper interface for spi-based touch sensor (MCP3462R-ADC)
Date: 2025-06-05
Version: 0.1
Owner: Mo
#################################################################
*/
#include "board/gpio.h" // irq_disable
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK
#include "spicmds.h" // spidev_transfer


#define READ_CMD 0b01000011 // Read command for MCP3462R

struct mcp3462r_adc {
    uint8_t oid; // Object ID for this ADC instance
    struct spidev_s *spi;
    struct gpio_in adc_ready_pin;
    struct gpio_out trigger_out_pin;
    uint32_t rest_ticks, timeout_cycles;
    struct timer timer;
    uint8_t active_session_flag, configured_flag;
    uint8_t msg[2];
};

static struct mcp3462r_adc *mcp_adc_ptr = NULL;

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
    mcp_adc_ptr->msg = {0x00, 0x00};
}

DECL_COMMAND(command_cfg_ts_adc, "cfg_ts_adc oid=%c spi_oid=%c adc_int_pin=%u trigger_out_pin=%u");


int8_t
mcp3462r_is_data_ready(struct mcp3462r_adc *mcp3462r)
{
    return gpio_in_read(mcp3462r->adc_ready_pin) == 0;
}


static uint_fast8_t
mcp3462r_event(struct timer *t)
{
    // struct mcp3462r_adc *mcp_adc_ptr = container_of(t, struct mcp3462r_adc, timer);
    if (mcp3462r_is_data_ready(mcp_adc_ptr)) 
    {
        // Read ADC data
        mcp_adc_ptr->msg = {0x00, 0x00}; // Clear previous data
        spidev_transfer(mcp_adc_ptr->spi, 1, 2, msg);
        // Process the raw values here
    }
    else
    {
        mcp_adc_ptr->timer.waketime += mcp_adc_ptr->rest_ticks;
        if (--mcp_adc_ptr->timeout_cycles == 0) {
            // Timeout reached, stop the task
            mcp_adc_ptr->active_session_flag = 0;
            // sched_del_timer(&mcp_adc_ptr->timer);
            return SF_DONE;
        }

    }
    
    return SF_RESCHEDULE;
}

/*
// This function is the event that is initiated by the user to walk 
// the a timered tasl for reading the value and  doing the low level logic
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
    if (!mcp_adc_ptr->timeout_cycles || !mcp_adc_ptr->rest_ticks) 
        shutdown("Timeout cycles and rest ticks must be greater than 0");
    
    // Send a reading command to the MCP3462R ADC
    mcp_adc_ptr->msg[0] = READ_CMD;
    spidev_transfer(mcp_adc_ptr->spi, 0, 1, mcp_adc_ptr->msg[0]);

    // Start the periodic event to listen for data ready
    sched_del_timer(&mcp_adc_ptr->timer);
    irq_disable();
    mcp_adc_ptr->timer.waketime = timer_read_time() + mcp_adc_ptr->rest_ticks;
    sched_add_timer(&mcp_adc_ptr->timer);
    irq_enable();
}

DECL_COMMAND(command_start_touch_sensing_session,
             "start_ts_session oid=%c timeout_cycles=%u rest_ticks=%u");

