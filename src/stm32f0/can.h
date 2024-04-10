#ifndef CORE_INC_CAN_H_
#define CORE_INC_CAN_H_

// Read UUID  (6bytes)
#define PKT_ID_UUID (0x321)
// Set address (2bytes) to UUID (6b)
#define PKT_ID_SET (0x322)
// UUID response from slave  (6bytes)
#define PKT_ID_UUID_RESP (0x323)

#define STM32_UUID_ADDR (0x1FFFF7AC)
#define SHORT_UUID_LEN (6)

void CanInit(void);
void CanTransmit(uint32_t id, uint32_t dlc, uint8_t *pkt);

#endif /* CORE_INC_CAN_H_ */
