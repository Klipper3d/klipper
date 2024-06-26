#ifndef __CANSERIAL_H__
#define __CANSERIAL_H__

#include <stdint.h> // uint32_t

#define CANBUS_ID_ADMIN 0x3f0
#define CANBUS_ID_ADMIN_RESP 0x3f1

// canserial.c
void canserial_notify_tx(void);
struct canbus_msg;
void canserial_process_data(struct canbus_msg *msg);
void canserial_set_uuid(uint8_t *raw_uuid, uint32_t raw_uuid_len);

#endif // canserial.h
