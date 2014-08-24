#ifndef __AVR_SERIAL_H
#define __AVR_SERIAL_H

#include <stdint.h>

uint8_t serial_canpop(void);
char serial_pop(void);
void serial_push(char data);

#endif // serial.h
