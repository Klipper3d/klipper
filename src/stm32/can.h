#ifndef __STM32_CAN_H__
#define __STM32_CAN_H__

// Read UUID  (6bytes)
#define PKT_ID_UUID (0x321)
// Set address (2bytes) to UUID (6b)
#define PKT_ID_SET (0x322)
// UUID response from slave  (6bytes)
#define PKT_ID_UUID_RESP (0x323)

#define SHORT_UUID_LEN (6)

#endif /* __STM32_CAN_H__*/
