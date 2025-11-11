#pragma once

#include <stdint.h>

void timer_init();
uint32_t timer_read_time();
void timer_kick();
