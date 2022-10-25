// Commands for sending messages on an SDIO bus
//
// Copyright (C) 2022 H. Gregor Molter <gregor.molter@secretlab.de>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "board/gpio.h" // gpio_in_setup
#include "board/sdio.h" // sdio_setup
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_SHUTDOWN

struct sdiodev_s {
    struct sdio_config sdio_config;
    uint32_t blocksize;
    uint32_t speed;
    uint8_t data_buffer[4096];
};

#define TIMEOUT_MSEC 500

void
command_config_sdio(uint32_t *args)
{
    struct sdiodev_s *sdio = oid_alloc(args[0], command_config_sdio
                                       , sizeof(*sdio));
    sdio->blocksize = args[1];
    sdio->speed = 400000; // Initial speed set to ~400khz
}
DECL_COMMAND(command_config_sdio, "config_sdio oid=%c blocksize=%u");

struct sdiodev_s *
sdiodev_oid_lookup(uint8_t oid)
{
    return oid_lookup(oid, command_config_sdio);
}

void
command_sdio_set_bus(uint32_t *args)
{
    struct sdiodev_s *sdio = sdiodev_oid_lookup(args[0]);
    sdio->sdio_config = sdio_setup(args[1]);
}
DECL_COMMAND(command_sdio_set_bus, "sdio_set_bus oid=%c sdio_bus=%u");

void
command_sdio_set_speed(uint32_t *args)
{
    struct sdiodev_s *sdio = sdiodev_oid_lookup(args[0]);
    sdio->speed = args[1];
    sdio_set_speed(sdio->sdio_config, sdio->speed);
}
DECL_COMMAND(command_sdio_set_speed, "sdio_set_speed oid=%c speed=%u");

void
command_sdio_send_command(uint32_t *args)
{
    uint8_t oid = args[0];
    uint8_t cmd = args[1];
    uint32_t argument = args[2];
    uint8_t wait = args[3];
    struct sdiodev_s *sdio = sdiodev_oid_lookup(oid);
    uint8_t response[16];
    uint8_t response_len = 0;
    uint8_t err = sdio_send_command(sdio->sdio_config, cmd, argument, wait
                                    , response, &response_len);
    sendf("sdio_send_command_response oid=%c error=%c response=%*s"
          , oid, err, response_len, response);
}
DECL_COMMAND(command_sdio_send_command
             , "sdio_send_command oid=%c cmd=%c argument=%u wait=%c");

void
command_sdio_read_data(uint32_t *args)
{
    uint8_t oid = args[0];
    uint8_t cmd = args[1];
    uint32_t argument = args[2];
    uint32_t data_len = 0;
    struct sdiodev_s *sdio = sdiodev_oid_lookup(oid);
    uint32_t timeout = TIMEOUT_MSEC*sdio->speed/1000;
    uint8_t err = sdio_prepare_data_transfer(sdio->sdio_config, 1, 1
                                             , sdio->blocksize, timeout);
    if (err == 0) {
        err = sdio_send_command(sdio->sdio_config, cmd, argument
                                , 1, NULL, NULL);
        if (err == 0) {
            data_len = sdio->blocksize;
            if (data_len <= sizeof(sdio->data_buffer)) {
                err = sdio_read_data(sdio->sdio_config, sdio->data_buffer
                                     , 1, sdio->blocksize);
            } else {
                data_len = 0;
            }
        }
    }
    sendf("sdio_read_data_response oid=%c error=%c read=%u"
          , oid, err, data_len);
}
DECL_COMMAND(command_sdio_read_data
             , "sdio_read_data oid=%c cmd=%c argument=%u");

void
command_sdio_write_data(uint32_t *args)
{
    uint8_t oid = args[0];
    uint8_t cmd = args[1];
    uint32_t argument = args[2];
    uint32_t data_len = 0;
    struct sdiodev_s *sdio = sdiodev_oid_lookup(oid);
    uint32_t timeout = TIMEOUT_MSEC*sdio->speed/1000;
    uint8_t err = sdio_prepare_data_transfer(sdio->sdio_config, 0, 1
                                             , sdio->blocksize, timeout);
    if (err == 0) {
        err = sdio_send_command(sdio->sdio_config, cmd, argument
                                , 1, NULL, NULL);
        if (err == 0) {
            data_len = sdio->blocksize;
            if (data_len <= sizeof(sdio->data_buffer)) {
                err = sdio_write_data(sdio->sdio_config, sdio->data_buffer
                                      , 1, sdio->blocksize);
            } else {
                data_len = 0;
            }
        }
    }
    sendf("sdio_write_data_response oid=%c error=%c write=%u"
          , oid, err, data_len);
}
DECL_COMMAND(command_sdio_write_data
             , "sdio_write_data oid=%c cmd=%c argument=%u");

void
command_sdio_read_data_buffer(uint32_t *args)
{
    uint8_t oid = args[0];
    uint32_t offset = args[1];
    uint8_t len = args[2];
    struct sdiodev_s *sdio = sdiodev_oid_lookup(oid);
    uint8_t *buf = &(sdio->data_buffer[offset]);

    if (offset + len > sizeof(sdio->data_buffer)) {
        len = 0;
    }
    sendf("sdio_read_data_buffer_response oid=%c data=%*s", oid, len, buf);
}
DECL_COMMAND(command_sdio_read_data_buffer
             , "sdio_read_data_buffer oid=%c offset=%u len=%c");

void
command_sdio_write_data_buffer(uint32_t *args)
{
    uint8_t oid = args[0];
    uint32_t offset = args[1];
    uint8_t write_data_len = args[2];
    uint8_t *write_data = command_decode_ptr(args[3]);
    struct sdiodev_s *sdio = sdiodev_oid_lookup(oid);
    uint8_t *buf = &(sdio->data_buffer[offset]);

    if (offset + write_data_len <= sizeof(sdio->data_buffer)) {
        memcpy(buf, write_data, write_data_len);
    }
}
DECL_COMMAND(command_sdio_write_data_buffer
             , "sdio_write_data_buffer oid=%c offset=%u data=%*s");
