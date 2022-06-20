#ifndef __RP2040_GPIO_H
#define __RP2040_GPIO_H

#include <stdint.h> // uint32_t
#include <stdbool.h>
#include "autoconf.h" // EGPIO

// PICO_CONFIG: PARAM_ASSERTIONS_ENABLED_GPIO, Enable/disable
// assertions in the GPIO module, type=bool, default=0, group=hardware_gpio
#ifndef PARAM_ASSERTIONS_ENABLED_GPIO
#define PARAM_ASSERTIONS_ENABLED_GPIO 0
#endif


/*! \brief  GPIO function definitions for use with function select
 *  \ingroup hardware_gpio
 * \brief GPIO function selectors
 *
 * Each GPIO can have one function selected at a time. Likewise,
 * each peripheral input (e.g. UART0 RX) should only be
 * selected on one GPIO at a time. If the same peripheral input is
 * connected to multiple GPIOs, the peripheral sees the logical
 * OR of these GPIO inputs.
 *
 * Please refer to the datsheet for more information on GPIO
 * function selection.
 */
enum gpio_function {
    GPIO_FUNC_XIP = 0,
    GPIO_FUNC_SPI = 1,
    GPIO_FUNC_UART = 2,
    GPIO_FUNC_I2C = 3,
    GPIO_FUNC_PWM = 4,
    GPIO_FUNC_SIO = 5,
    GPIO_FUNC_PIO0 = 6,
    GPIO_FUNC_PIO1 = 7,
    GPIO_FUNC_GPCK = 8,
    GPIO_FUNC_USB = 9,
    GPIO_FUNC_NULL = 0x1f,
};

// Convert a register and bit location back to an integer pin identifier
#if CONFIG_HAVE_EGPIO
#define SET_BIT(var,prop,pin) \
    if(unlikely(pin>31)) { \
        var.prop = 0x0; \
        var.prop##_hi =1<<(pin-32); \
    } else { var.prop =1<<pin; }
#define GET_BIT(var,prop,pin) \
    !!( ( (pin>31 ? var.prop##_hi : var.prop ) ) \
    & (1<<(pin - ( pin > 31 ? 32 : 0 ) )) )
#define MASK_TO_PIN(str,prop) \
    likely(ffs(str.prop)) ? ffs(str.prop)-1 : ffs(str.prop##_hi)+31
uint8_t xra_read(uint8_t bus, uint8_t reg);
void xra_write(uint8_t bus, uint8_t reg, uint8_t data);
#else
#define SET_BIT(var,prop,pin) var.prop =1<<pin
#define MASK_TO_PIN(str,prop) \
    ffs(str.prop)-1
#define GET_BIT(var,prop,pin) !!(var.prop & (1<<pin))
#endif

struct gpio_out {
#if CONFIG_HAVE_EGPIO
    uint32_t bit;
    uint32_t bit_hi;
#else
    uint32_t bit;
#endif
};
struct gpio_out gpio_out_setup(uint8_t pin, uint8_t val);
void gpio_out_reset(struct gpio_out g, uint8_t val);
void gpio_out_toggle_noirq(struct gpio_out g);
void gpio_out_toggle(struct gpio_out g);
void gpio_out_write(struct gpio_out g, uint8_t val);

struct gpio_in {
#if CONFIG_HAVE_EGPIO
    uint32_t bit;
    uint32_t bit_hi;
#else
    uint32_t bit;
#endif
};
struct gpio_in gpio_in_setup(uint8_t pin, int8_t pull_up);
void gpio_in_reset(struct gpio_in g, int8_t pull_up);
uint8_t gpio_in_read(struct gpio_in g);

struct gpio_pwm {
    void *reg;
    uint8_t shift;
    uint32_t mask;
};
struct gpio_pwm gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val);
void gpio_pwm_write(struct gpio_pwm g, uint32_t val);

struct gpio_adc {
    uint8_t chan;
    uint8_t pin;
};
typedef void (*callback_t)(void) ;
struct gpio_adc gpio_adc_setup(uint32_t pin);
void gpio_acknowledge_irq(uint32_t gpio, uint32_t events);
void gpio_in_setup_interrupt(uint32_t pin, uint8_t val);
uint32_t gpio_adc_sample(struct gpio_adc g);
uint16_t gpio_adc_read(struct gpio_adc g);
void gpio_adc_cancel_sample(struct gpio_adc g);

struct spi_config {
    void *spi;
    uint32_t cr0, cpsr;
};
struct spi_config spi_setup(uint32_t bus, uint8_t mode, uint32_t rate);
void spi_prepare(struct spi_config config);
void spi_transfer(struct spi_config config, uint8_t receive_data
                  , uint8_t len, uint8_t *data);

struct i2c_config {
    void *i2c;
    uint8_t addr;
};

struct i2c_config i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr);
void i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write);
void i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg
              , uint8_t read_len, uint8_t *read);


/*! \brief  GPIO Interrupt level definitions
 *  \ingroup hardware_gpio
 *  \brief GPIO Interrupt levels
 *
 * An interrupt can be generated for every GPIO pin in 4 scenarios:
 *
 * * Level High: the GPIO pin is a logical 1
 * * Level Low: the GPIO pin is a logical 0
 * * Edge High: the GPIO has transitioned from a logical 0 to a logical 1
 * * Edge Low: the GPIO has transitioned from a logical 1 to a logical 0
 *
 * The level interrupts are not latched. This means that if the pin is a
 * logical 1 and the level high interrupt is active, it will
 * become inactive as soon as the pin changes to a logical 0. The edge
 * interrupts are stored in the INTR register and can be
 * cleared by writing to the INTR register.
 */
enum gpio_irq_level {
    GPIO_IRQ_LEVEL_LOW = 0x1u,
    GPIO_IRQ_LEVEL_HIGH = 0x2u,
    GPIO_IRQ_EDGE_FALL = 0x4u,
    GPIO_IRQ_EDGE_RISE = 0x8u,
};

// --------------------------------------------------------------------------
// Pad Controls + IO Muxing
// --------------------------------------------------------------------------
// Declarations for gpio.c

/*! \brief Select GPIO function
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \param fn Which GPIO function select to use from list
 * \ref gpio_function
 */
void gpio_set_function(uint32_t gpio, enum gpio_function fn);

/*! \brief Determine current GPIO function
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \return Which GPIO function is currently selected from list
 * \ref gpio_function
 */
enum gpio_function gpio_get_function(uint32_t gpio);

/*! \brief Select up and down pulls on specific GPIO
 *  \ingroup hardware_gpio
 *
 * \param gpio GPIO number
 * \param up If true set a pull up on the GPIO
 * \param down If true set a pull down on the GPIO
 *
 * \note On the RP2040, setting both pulls enables a "bus keep" function,
 * i.e. a weak pull to whatever is current high/low state of GPIO.
 */
void gpio_set_pulls(uint32_t gpio, bool up, bool down);

#endif // gpio.h
