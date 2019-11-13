#include "LPC17xx.h" // __disable_irq
#include "uid.h"

#define IAP_LOCATION        0x1fff1ff1

#define IAP_CMD_READ_UID    58

typedef void (*IAP)(uint32_t *, uint32_t *);

void
platform_get_uid(uint32_t *serial) {
    uint32_t iap_cmd_uid[5] = {IAP_CMD_READ_UID, 0, 0, 0, 0};
    uint32_t iap_resp[5];
    uint8_t i;

    IAP iap_entry = (IAP)IAP_LOCATION;
    __disable_irq();
    iap_entry(iap_cmd_uid, iap_resp);
    __enable_irq();

    for (i = 0; i < 4; i++)
        serial[i] = iap_resp[i+1];
}
