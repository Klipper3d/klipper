#ifndef TEST_MISC_H
#define TEST_MISC_H
static uint32_t now;
static uint32_t timer_read_time(void) { return now; }
static int timer_is_before(uint32_t a, uint32_t b) {
    return (int32_t)(a - b) < 0;
}
#endif
