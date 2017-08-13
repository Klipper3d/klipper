// Wrappers for AVR usb serial.
//
// Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memmove
#include "../lib/pjrc_usb_serial/usb_serial.h"
#include "board/misc.h" // console_sendf
#include "command.h" // command_dispatch
#include "sched.h" // DECL_INIT

static char receive_buf[MESSAGE_MAX];
static uint8_t receive_pos;

void
usbserial_init(void)
{
    usb_init();
}
DECL_INIT(usbserial_init);

// Check for new incoming data
static void
console_check_input(void)
{
    for (;;) {
        if (receive_pos >= sizeof(receive_buf))
            break;
        int16_t ret = usb_serial_getchar();
        if (ret == -1)
            break;
        receive_buf[receive_pos++] = ret;
    }
}

// Remove from the receive buffer the given number of bytes
static void
console_pop_input(uint8_t len)
{
    uint8_t needcopy = receive_pos - len;
    if (needcopy) {
        memmove(receive_buf, &receive_buf[len], needcopy);
        sched_wake_tasks();
    }
    receive_pos = needcopy;
}

// Process any incoming commands
void
console_task(void)
{
    console_check_input();
    uint8_t pop_count;
    int8_t ret = command_find_block(receive_buf, receive_pos, &pop_count);
    if (ret > 0)
        command_dispatch(receive_buf, pop_count);
    if (ret)
        console_pop_input(pop_count);
}
DECL_TASK(console_task);

// Encode and transmit a "response" message
void
console_sendf(const struct command_encoder *ce, va_list args)
{
    // Generate message
    static char buf[MESSAGE_MAX];
    uint8_t msglen = command_encodef(buf, ce, args);
    command_add_frame(buf, msglen);

    // Transmit message
    usb_serial_write((void*)buf, msglen);
    usb_serial_flush_output();
}
