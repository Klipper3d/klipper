#pragma once

#include "driver/gptimer_types.h"

#define U64_MASK_LO (0x00000000FFFFFFFFull)
#define U64_MASK_HI (0xFFFFFFFF00000000ull)

// Todo: ordenar!

void timer_init();
static bool timer_alarm(gptimer_handle_t timer, const gptimer_alarm_event_data_t* edata, void* user_ctx);
static uint64_t timer_read_time_u64();
static void timer_set_alarm(uint32_t next_alarm);
