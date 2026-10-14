#ifndef TEST_GPIO_H
#define TEST_GPIO_H
struct gpio_in { unsigned pin; };
struct gpio_out { unsigned pin; };
static unsigned levels[32];
static struct gpio_in gpio_in_setup(unsigned pin, int pullup) {
    return (struct gpio_in){pin};
}
static struct gpio_out gpio_out_setup(unsigned pin, unsigned value) {
    levels[pin] = value;
    return (struct gpio_out){pin};
}
static unsigned gpio_in_read(struct gpio_in pin) { return levels[pin.pin]; }
static void gpio_out_write(struct gpio_out pin, unsigned value) {
    levels[pin.pin] = value;
}
#endif
