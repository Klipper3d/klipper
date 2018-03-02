#include "autoconf.h"

#include "board/gpio.h"   // gpio_out_write
#include "board/irq.h"    // irq_disable
#include "generic/spi.h"

#include "basecmd.h"      // oid_alloc
#include "command.h"      // DECL_COMMAND
#include "sched.h"        // DECL_TASK


struct thermocouple_spi {
    struct timer timer;
    uint32_t rest_time;
    uint32_t next_begin_time; // Start time
    uint32_t read_interval;
    uint32_t fault_mask;  // Fault check mask
    uint32_t value;       // Stores the thermocouple/RTD ADC output
    uint32_t min_value;   // Min allowed ADC value
    uint32_t max_value;   // Max allowed ADC value
    SPI_t    spi_config;
    uint8_t  read_cmd;    // SPI command to get ADC result
    uint8_t  read_bytes;  // Num of bytes to be read
    uint8_t  fault_cmd;   // SPI command to get fault register (0 = disabled)
    uint8_t  fault_value; // fault register value
    struct gpio_out pin;
};

static struct task_wake thermocouple_wake;

static uint_fast8_t thermocouple_event(struct timer *timer) {
    uint8_t iter;
    struct thermocouple_spi *spi = container_of(timer,
                                                struct thermocouple_spi,
                                                timer);
    spi_set_config(spi->spi_config);
    gpio_out_write(spi->pin, 0); // Enable slave
    spi_transfer(spi->read_cmd, 0);
    spi->value = spi_transfer(SPI_READ_CMD, 0);
    for (iter = 1; iter < spi->read_bytes; iter++) {
        spi->value <<= 8;
        spi->value |= spi_transfer(SPI_READ_CMD,
                                   (iter == (spi->read_bytes - 1)));
    }
    if (spi->fault_cmd) {
        spi_transfer(spi->fault_cmd, 0);
        spi->fault_value = spi_transfer(0x00, 1);
    }
    gpio_out_write(spi->pin, 1); // Disable slave

    if ( spi->value & spi->fault_mask ) {
        shutdown("Thermocouple reader fault");
        //spi->timer.waketime += spi->read_interval;
        //return SF_RESCHEDULE;
    }


    /* check the result and stop if below or above allowed range */
    if (spi->value < spi->min_value || spi->value > spi->max_value) {
        shutdown("Thermocouple ADC out of range");
    }

    sched_wake_task(&thermocouple_wake);
    /* Order next read */
    spi->next_begin_time += spi->rest_time;
    spi->timer.waketime = spi->next_begin_time;
    return SF_RESCHEDULE;//SF_DONE;
}

/* Configure thermocouple / RTD reader and let it do the task */
void command_config_thermocouple(uint32_t *args) {
    struct thermocouple_spi *spi =
        oid_lookup(args[0], command_config_thermocouple);

    spi->read_cmd        = args[1];
    spi->next_begin_time = args[2];
    spi->read_interval   = args[3];

    spi->min_value       = args[4];
    spi->max_value       = args[5];
    spi->read_bytes      = args[6];
    spi->fault_mask      = args[7];
    spi->rest_time       = args[8];
    spi->fault_cmd       = args[9];

    /* Skip if read interval is not set */
    if (!spi->read_interval || !spi->next_begin_time) {
        shutdown("Thermocouple configuration error!");
        return;
    }

    /* Configure reader here... */
    uint8_t len = args[10];
    uint8_t *msg = (uint8_t*)(size_t)args[11];
    if (*msg != 0 && 1 < len) {
        spi_set_config(spi->spi_config);
        gpio_out_write(spi->pin, 0); // Enable slave
        while (len--) {
            uint8_t const data = (uint8_t)(*msg++);
            spi_transfer(data, (len == 0));
        }
        gpio_out_write(spi->pin, 1); // Disable slave
    }

    /* Configure timer */
    sched_del_timer(&spi->timer);
    spi->timer.waketime = spi->next_begin_time;

    /* Enable read timer */
    sched_add_timer(&spi->timer);
}
DECL_COMMAND(command_config_thermocouple,
             "config_thermocouple oid=%c cmd=%c clock=%u interval=%u"
             " min_value=%u max_value=%u read_bytes=%c fault_mask=%u"
             " rest_ticks=%u fault_cmd=%c cfg=%*s");

/* Config slave select pin */
void command_config_thermocouple_ss_pin(uint32_t *args) {
    struct thermocouple_spi *spi =
        oid_alloc(args[0],
                  command_config_thermocouple,
                  sizeof(*spi));
    spi->timer.func = thermocouple_event;
    spi->pin        = gpio_out_setup(args[1], 1); // CS pin
    spi->spi_config = spi_get_config(args[2], args[3]);
}
DECL_COMMAND(command_config_thermocouple_ss_pin,
             "config_thermocouple_ss_pin oid=%c pin=%u spi_mode=%u spi_speed=%u");

/* task to send response */
void thermocouple_task(void) {
    if (!sched_check_wake(&thermocouple_wake))
        return;
    uint8_t oid;
    struct thermocouple_spi *spi;
    foreach_oid(oid, spi, command_config_thermocouple) {
        irq_disable();
        uint32_t const value           = spi->value;
        uint32_t const next_begin_time = spi->next_begin_time;
        uint8_t  const fault           = spi->fault_value;
        irq_enable();
        sendf("thermocouple_result oid=%c next_clock=%u value=%u fault=%c",
              oid, next_begin_time, value, fault);
    }
}
DECL_TASK(thermocouple_task);

/* Shutdown task */
void thermocouple_shutdown(void) {
    uint8_t oid;
    struct thermocouple_spi *spi;
    foreach_oid(oid, spi, command_config_thermocouple) {
        gpio_out_write(spi->pin, 1); // Disable slaves
    }
}
DECL_SHUTDOWN(thermocouple_shutdown);
