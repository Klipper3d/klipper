// Commands for sending messages on an SPI bus
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/gpio.h" // gpio_out_write
#include "command.h" // DECL_COMMAND
#include "generic/spi.h"

void
command_send_spi_message(uint32_t *args)
{
    // For now, this only implements enough to program an ad5206 digipot
    uint8_t len = args[1];
    char *msg = (void*)(size_t)args[2];

    while (!spi_set_config(spi_basic_config));
    struct gpio_out pin = gpio_out_setup(args[0], 0);
    spi_transfer_len(msg, len);
    spi_set_ready();
    gpio_out_write(pin, 1);
    sendf("spi_response response=%*s", len, msg);
}
DECL_COMMAND(command_send_spi_message, "send_spi_message pin=%u msg=%*s");
