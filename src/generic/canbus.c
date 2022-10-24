// Wrapper functions connecting canserial.c to low-level can hardware
//
// Copyright (C) 2022  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CANBUS_FREQUENCY
#include "canbus.h" // canbus_send
#include "canserial.h" // canserial_send
#include "command.h" // DECL_CONSTANT

DECL_CONSTANT("CANBUS_FREQUENCY", CONFIG_CANBUS_FREQUENCY);

int
canserial_send(struct canbus_msg *msg)
{
    return canbus_send(msg);
}

void
canserial_set_filter(uint32_t id)
{
    canbus_set_filter(id);
}

void
canbus_notify_tx(void)
{
    canserial_notify_tx();
}

void
canbus_process_data(struct canbus_msg *msg)
{
    canserial_process_data(msg);
}
