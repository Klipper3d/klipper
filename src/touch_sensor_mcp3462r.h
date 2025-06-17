#define READ_CMD 0b01000011 // Read command for MCP3462R

struct mcp3462r_adc {
    uint8_t oid; // Object ID for this ADC instance
    struct spidev_s *spi;
    struct gpio_in adc_ready_pin;
    struct gpio_out trigger_out_pin;
    uint32_t rest_ticks, timeout_cycles;
    struct timer timer;
    uint8_t active_session_flag, configured_flag;
    uint8_t msg[2];
    uint16_t sensitivity; 
};