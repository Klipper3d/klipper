#ifndef __CANBUS_H__
#define __CANBUS_H__

#include <stdint.h> // uint32_t

#define CANBUS_ID_UUID 0x321
#define CANBUS_ID_SET 0x322
#define CANBUS_ID_UUID_RESP 0x323
#define CANBUS_UUID_LEN 6

// callbacks provided by board specific code
int canbus_read(uint32_t *id, uint8_t *data);
int canbus_send(uint32_t id, uint32_t len, uint8_t *data);
void canbus_set_filter(uint32_t id);
void canbus_reboot(void);

// canbus.c
void canbus_notify_tx(void);
void canbus_notify_rx(void);
void canbus_set_uuid(void *data);

#endif // canbus.h
