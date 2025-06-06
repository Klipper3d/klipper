#ifndef __STM32_FLASH_H
#define __STM32_FLASH_H

#include <stdint.h>

void flash_erase_page(uint32_t page_address);

#endif
