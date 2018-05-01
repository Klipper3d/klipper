#include "autoconf.h"

#include "board/gpio.h"   // gpio_out_write
#include "board/irq.h"    // irq_disable
#include "generic/spi.h"

#include "basecmd.h"      // oid_alloc
#include "command.h"      // DECL_COMMAND
#include "sched.h"        // DECL_TASK

struct spibus_t {
    struct spi_config spi_config;
    struct gpio_out pin;
};

#define BUFF_SIZE 32u
static uint8_t spirespbuff[BUFF_SIZE] = {0};

/* Config slave select pin and SPI bus */
void command_config_spibus_ss_pin(uint32_t *args) {
    struct spibus_t *spi =
        oid_alloc(args[0],
                  command_config_spibus_ss_pin,
                  sizeof(*spi));
    spi->pin        = gpio_out_setup(args[1], 1); // CS pin
    spi->spi_config = spi_get_config(args[2], args[3]);
}
DECL_COMMAND(command_config_spibus_ss_pin,
             "config_spibus_ss_pin oid=%c pin=%u spi_mode=%u spi_speed=%u");

/* write value to SPI slave */
void command_spibus_write(uint32_t *args) {
    struct spibus_t *spi =
        oid_lookup(args[0], command_config_spibus_ss_pin);

    uint8_t iter = 0;
    uint8_t status = 1;
    uint8_t cmd = args[1];
    uint8_t len = args[2];

    /* Write data to SPI slave */
    uint8_t *msg = (uint8_t*)(size_t)args[3];
    if (1 <= len) {
        while (!spi_set_config(spi->spi_config));
        gpio_out_write(spi->pin, 0); // Enable slave
        spirespbuff[iter++] = spi_transfer(cmd);
        while (len--) {
            spirespbuff[iter++] =
                spi_transfer((uint8_t)(*msg++));
        }
        spi_set_ready();
        gpio_out_write(spi->pin, 1); // Disable slave
        status = 0;
    }
    sendf("spibus_write_resp oid=%c status=%i data=%*s",
          args[0], status, iter, spirespbuff);
}
DECL_COMMAND(command_spibus_write,
             "spibus_write oid=%c cmd=%c cfg=%*s");

/* read from SPI slave */
void command_spibus_read(uint32_t *args) {
    struct spibus_t *spi =
        oid_lookup(args[0], command_config_spibus_ss_pin);

    uint8_t iter = 0;
    uint8_t status = 1;
    uint8_t cmd = args[1];
    uint8_t len = args[2];

    /* Write data to SPI slave */
    if (len && len <= BUFF_SIZE) {
        while (!spi_set_config(spi->spi_config));
        gpio_out_write(spi->pin, 0); // Enable slave
        spirespbuff[iter++] = spi_transfer(cmd);
        do {
            spirespbuff[iter++] = spi_transfer(0x00);
        } while (--len);
        spi_set_ready();
        gpio_out_write(spi->pin, 1); // Disable slave
        status = 0;
    }
    sendf("spibus_read_resp oid=%c status=%i data=%*s",
          args[0], status, iter, spirespbuff);
}
DECL_COMMAND(command_spibus_read,
             "spibus_read oid=%c cmd=%c len=%c");

/* Shutdown task */
void spibus_shutdown(void) {
    uint8_t oid;
    struct spibus_t *spi;
    foreach_oid(oid, spi, command_config_spibus_ss_pin) {
        gpio_out_write(spi->pin, 1); // Disable slaves
    }
}
DECL_SHUTDOWN(spibus_shutdown);
