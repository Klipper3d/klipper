// Standard 8N1 UART bitbang driver for Modbus RTU
// Licensed under the GNU GPLv3 license
#include <string.h>
#include "board/gpio.h"
#include "board/irq.h"
#include "board/misc.h"
#include "basecmd.h"
#include "command.h"
#include "sched.h"

#define MODBUS_UART_DEBUG 0
#if MODBUS_UART_DEBUG
#include <stdio.h>
#define DBG_PRINT(fmt, ...) printf("[DBG] " fmt "\n", ##__VA_ARGS__)
#else
#define DBG_PRINT(fmt, ...) do {} while(0)
#endif

/****************************************************************
 * Data structure definition
 ****************************************************************/
struct modbus_uart_s;
// Bus state bit flags
enum {
    MU_LINE_HIGH    = 1<<0,  // UART bus idle high level
    MU_ACTIVE       = 1<<1,  // Transfer in progress flag
    MU_RX_SYNC      = 1<<2,  // Rx sync ready flag
    MU_REPORT       = 1<<3,  // Flag to notify host of received data
    MU_PULLUP       = 1<<4,  // Rx pin internal pull-up enable
    MU_SINGLE_WIRE  = 1<<5   // Single-wire half-duplex mode flag
};

// Per-instance UART state storage
struct modbus_uart_s {
    struct timer timer;             // Scheduler timer for bit timing
    struct gpio_out tx_pin;         // Transmit output pin handle
    struct gpio_in rx_pin;          // Receive input pin handle
    uint8_t flags;                  // Combined state bitmask
    uint8_t tx_byte_idx;            // Current transmit byte index
    uint8_t tx_bit_idx;             // Current bit position inside transmit byte
    uint32_t bit_time;              // Clock ticks per single UART bit
    uint8_t tx_total;               // Total transmit byte count of current frame
    uint8_t rx_total;               // Expected receive byte count of current frame
    uint8_t rx_bit_count;           // Total sampled data bits received
    uint8_t data[16];               // Shared tx/rx data buffer (max 16 bytes)
    uint16_t sync_counter;          // Timeout counter for rx sync detection
};
/****************************************************************
 * Forward function prototypes
 ****************************************************************/
static void modbus_uart_reset_line(struct modbus_uart_s *m);
static uint_fast8_t modbus_uart_finalize(struct modbus_uart_s *m);
static uint_fast8_t modbus_uart_rx_sync_event(struct timer *timer);
static uint_fast8_t modbus_uart_rx_event(struct timer *timer);
static uint_fast8_t modbus_uart_tx_event(struct timer *timer);
static struct task_wake modbus_uart_wake;
/****************************************************************
 * Base utility functions
 ****************************************************************/
/**
 * Restore UART bus to idle high state
 * @param m Pointer to modbus_uart_s instance
 */
static void
modbus_uart_reset_line(struct modbus_uart_s *m)
{
    if (m->flags & MU_SINGLE_WIRE)
        gpio_out_reset(m->tx_pin, 1);
    else
        gpio_out_write(m->tx_pin, 1);
    m->flags = (m->flags & (MU_PULLUP | MU_SINGLE_WIRE)) | MU_LINE_HIGH;
}
/**
 * Complete current transfer, mark data ready for host report
 * @param m Pointer to modbus_uart_s instance
 * @return SF_DONE Scheduler flag to terminate timer callback
 */
static uint_fast8_t
modbus_uart_finalize(struct modbus_uart_s *m)
{
    __attribute__((unused)) uint8_t actual_bytes = m->rx_bit_count / 8;
    DBG_PRINT("finalize: expected=%d bytes, actual=%d bytes", m->rx_total, actual_bytes);
#if MODBUS_UART_DEBUG
    if (actual_bytes > 0) {
        printf("[DBG] rx hex: ");
        for (int i = 0; i < actual_bytes; i++)
            printf("%02x ", m->data[i]);
        printf("\n");
    }
#endif
    modbus_uart_reset_line(m);
    m->flags |= MU_REPORT;
    sched_wake_task(&modbus_uart_wake);
    return SF_DONE;
}
/****************************************************************
 * Receive state machine - Sync detection stage
 ****************************************************************/
/**
 * Rx sync state handler: Detect falling edge of start bit
 * @param timer Scheduler timer pointer
 * @return SF_RESCHEDULE/SF_DONE Scheduler execution flag
 */
static uint_fast8_t
modbus_uart_rx_sync_event(struct timer *timer)
{
    struct modbus_uart_s *m = container_of(timer, struct modbus_uart_s, timer);
    uint8_t v = gpio_in_read(m->rx_pin);
    if (v) {
        // Bus level high, mark sync ready for falling edge capture
        m->flags |= MU_RX_SYNC;
    } else if (m->flags & MU_RX_SYNC) {
        // Valid high->low falling edge detected: start bit received
        DBG_PRINT("sync: start bit detected, counter=%d", m->sync_counter);
        // Reset receive buffer and counters for new frame
        m->rx_bit_count = 0;
        memset(m->data, 0, sizeof(m->data));
        // Switch state machine to data bit sampling
        m->timer.func = modbus_uart_rx_event;
        // Delay 1.5 bit periods to sample at data bit center
        m->timer.waketime += m->bit_time + m->bit_time / 2;
        return SF_RESCHEDULE;
    }
    m->sync_counter++;
    // Terminate receive if sync timeout reached (300 bit periods)
    if (m->sync_counter >= 300) {
        DBG_PRINT("sync timeout after %d bits", m->sync_counter);
        return modbus_uart_finalize(m);
    }
    m->timer.waketime += m->bit_time;
    return SF_RESCHEDULE;
}
/****************************************************************
 * Receive state machine - Data bit sampling stage
 ****************************************************************/
/**
 * Rx data sampling handler: Sample 8 data bits per byte (LSB first)
 * @param timer Scheduler timer pointer
 * @return SF_RESCHEDULE/SF_DONE Scheduler execution flag
 */
static uint_fast8_t
modbus_uart_rx_event(struct timer *timer)
{
    struct modbus_uart_s *m = container_of(timer, struct modbus_uart_s, timer);
    uint8_t v = gpio_in_read(m->rx_pin);
    uint8_t bit_pos = m->rx_bit_count;
    // Calculate target byte and bit offset inside byte
    uint8_t byte_idx = bit_pos / 8;
    uint8_t bit_in_byte = bit_pos % 8;
    if (v && byte_idx < sizeof(m->data))
        m->data[byte_idx] |= (1 << bit_in_byte);
    m->rx_bit_count++;
    // Stop sampling if total expected data bits are captured
    if (m->rx_bit_count >= m->rx_total * 8) {
        DBG_PRINT("rx all done: %d bits = %d bytes", m->rx_bit_count, m->rx_total);
        return modbus_uart_finalize(m);
    }
    // After each full byte: skip stop bit + next start bit (2 extra bit periods)
    if (m->rx_bit_count % 8 == 0) {
        m->timer.waketime += m->bit_time * 3; // Current 1bit + skip 2bits
    } else {
        m->timer.waketime += m->bit_time;
    }
    return SF_RESCHEDULE;
}
/****************************************************************
 * Transmit state machine
 ****************************************************************/
/**
 * Tx bit generation handler: Generate 8N1 UART bit stream
 * @param timer Scheduler timer pointer
 * @return SF_RESCHEDULE/SF_DONE Scheduler execution flag
 */
static uint_fast8_t
modbus_uart_tx_event(struct timer *timer)
{
    struct modbus_uart_s *m = container_of(timer, struct modbus_uart_s, timer);
    uint8_t bit_val;
    uint8_t bit_idx = m->tx_bit_idx;
    uint8_t byte_idx = m->tx_byte_idx;
    if (bit_idx == 0) {
        bit_val = 0; // Start bit (logic low)
    } else if (bit_idx <= 8) {
        bit_val = (m->data[byte_idx] >> (bit_idx - 1)) & 0x01; // Data bits LSB first
    } else {
        bit_val = 1; // Stop bit (logic high)
    }
    uint8_t line_state = !!(m->flags & MU_LINE_HIGH);
    if (bit_val != line_state) {
        gpio_out_toggle_noirq(m->tx_pin);
        m->flags ^= MU_LINE_HIGH;
    }
    bit_idx++;
    if (bit_idx >= 10) {
        byte_idx++;
        if (byte_idx >= m->tx_total) {
            DBG_PRINT("tx done: %d bytes", m->tx_total);
            if (m->rx_total == 0)
                return modbus_uart_finalize(m);
            // Switch pin to input mode for single-wire receive
            if (m->flags & MU_SINGLE_WIRE)
                gpio_in_reset(m->rx_pin, m->flags & MU_PULLUP);
            m->sync_counter = 0;
            m->flags &= ~MU_RX_SYNC;
            m->timer.func = modbus_uart_rx_sync_event;
            // Adjust post-transmit delay for single/double wire modes
            uint32_t rx_delay = (m->flags & MU_SINGLE_WIRE) ?
                                m->bit_time * 20 : m->bit_time * 12;
            m->timer.waketime += rx_delay;
            return SF_RESCHEDULE;
        }
        bit_idx = 0;
    }
    m->tx_bit_idx = bit_idx;
    m->tx_byte_idx = byte_idx;
    m->timer.waketime += m->bit_time;
    return SF_RESCHEDULE;
}
/****************************************************************
 * Host command registration & handlers
 ****************************************************************/
/**
 * OID configuration command: Initialize modbus uart instance
 * @param args Command argument array from host
 */
void
command_config_modbus_uart(uint32_t *args)
{
    struct modbus_uart_s *m = oid_alloc(args[0], command_config_modbus_uart
                                        , sizeof(*m));
    uint8_t pull_up = args[2];
    uint32_t rx_pin = args[1];
    uint32_t tx_pin = args[3];
    m->rx_pin = gpio_in_setup(rx_pin, !!pull_up);
    m->tx_pin = gpio_out_setup(tx_pin, 1);
    m->bit_time = args[4];
    m->flags = (MU_LINE_HIGH | (pull_up ? MU_PULLUP : 0)
                | (rx_pin == tx_pin ? MU_SINGLE_WIRE : 0));
    DBG_PRINT("config: oid=%d rx=%u tx=%u pullup=%d bit_time=%u",
              args[0], rx_pin, tx_pin, pull_up, m->bit_time);
}
DECL_COMMAND(command_config_modbus_uart,
             "config_modbus_uart oid=%c rx_pin=%u pull_up=%c"
             " tx_pin=%u bit_time=%u");
/**
 * Transfer command: Trigger transmit + optional receive cycle
 * @param args Command argument array from host
 */
void
command_modbus_uart_send(uint32_t *args)
{
    struct modbus_uart_s *m = oid_lookup(args[0], command_config_modbus_uart);
    if (m->flags & MU_ACTIVE) {
        DBG_PRINT("send: busy drop");
        return;
    }
    uint8_t write_len = args[1];
    uint8_t *write = command_decode_ptr(args[2]);
    uint8_t read_len = args[3];
    DBG_PRINT("send cmd: wr=%d rd=%d", write_len, read_len);
    if (write_len > sizeof(m->data) || read_len > sizeof(m->data))
        shutdown("modbus_uart data too large");
    memcpy(m->data, write, write_len);
    m->tx_total = write_len;
    m->rx_total = read_len;
    m->tx_byte_idx = 0;
    m->tx_bit_idx = 0;
    m->rx_bit_count = 0;
    m->flags = (m->flags & (MU_LINE_HIGH|MU_PULLUP|MU_SINGLE_WIRE)) | MU_ACTIVE;
    m->timer.func = modbus_uart_tx_event;
    irq_disable();
    m->timer.waketime = timer_read_time() + timer_from_us(200);
    sched_add_timer(&m->timer);
    irq_enable();
}
DECL_COMMAND(command_modbus_uart_send,
             "modbus_uart_send oid=%c write=%*s read=%c");
/****************************************************************
 * Background report task
 ****************************************************************/
/**
 * Wakeup task: Send received frame data back to host
 */
void
modbus_uart_task(void)
{
    if (!sched_check_wake(&modbus_uart_wake))
        return;
    uint8_t oid;
    struct modbus_uart_s *m;
    foreach_oid(oid, m, command_config_modbus_uart) {
        if (!(m->flags & MU_REPORT))
            continue;
        irq_disable();
        m->flags &= ~MU_REPORT;
        irq_enable();
        uint8_t actual_bytes = m->rx_bit_count / 8;
        sendf("modbus_uart_response oid=%c read=%*s",
              oid, actual_bytes, m->data);
    }
}
DECL_TASK(modbus_uart_task);
/****************************************************************
 * System shutdown handler
 ****************************************************************/
/**
 * Shutdown callback: Reset all UART buses to idle state
 */
void
modbus_uart_shutdown(void)
{
    uint8_t i;
    struct modbus_uart_s *m;
    foreach_oid(i, m, command_config_modbus_uart) {
        modbus_uart_reset_line(m);
    }
}
DECL_SHUTDOWN(modbus_uart_shutdown);
