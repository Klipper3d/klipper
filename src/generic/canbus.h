#ifndef __CANBUS_H__
#define __CANBUS_H__

#include <stdint.h> // uint32_t

#define CANBUS_ID_ADMIN 0x3f0
#define CANBUS_ID_ADMIN_RESP 0x3f1
#define CANBUS_UUID_LEN 6

// callbacks provided by board specific code
int canbus_read(uint32_t *id, uint8_t *data);
int canbus_send(uint32_t id, uint32_t len, uint8_t *data);
void canbus_set_filter(uint32_t id);

// canbus.c
void canbus_notify_tx(void);
void canbus_notify_rx(void);
void canbus_set_uuid(void *data);

#endif // canbus.h
