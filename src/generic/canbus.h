#ifndef __CANBUS_H__
#define __CANBUS_H__

#include <stdint.h> // uint32_t

#define CANBUS_ID_ADMIN 0x3f0
#define CANBUS_ID_ADMIN_RESP 0x3f1

struct canbus_msg {
    uint32_t id;
    uint32_t dlc;
    union {
        uint8_t data[8];
        uint32_t data32[2];
    };
};

#define CANMSG_ID_RTR (1<<30)
#define CANMSG_ID_EFF (1<<31)

#define CANMSG_DATA_LEN(msg) ((msg)->dlc > 8 ? 8 : (msg)->dlc)

// callbacks provided by board specific code
int canbus_send(struct canbus_msg *msg);
void canbus_set_filter(uint32_t id);

// canbus.c
void canbus_notify_tx(void);
void canbus_process_data(struct canbus_msg *msg);
void canbus_set_uuid(uint8_t *raw_uuid, uint32_t raw_uuid_len);

#endif // canbus.h
