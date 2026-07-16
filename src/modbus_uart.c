// Standard 8N1 UART bitbang for Modbus RTU
// 稳定基线版：单次同步 + 逐字节对齐，适配Linux用户态软串口
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h>
#include "board/gpio.h"
#include "board/irq.h"
#include "board/misc.h"
#include "basecmd.h"
#include "command.h"
#include "sched.h"

// 调试开关：需要看日志时改成1，正常使用改成0，printf会加剧抖动
#define MODBUS_UART_DEBUG 0

#if MODBUS_UART_DEBUG
#include <stdio.h>
#define DBG_PRINT(fmt, ...) printf("[DBG] " fmt "\n", ##__VA_ARGS__)
#else
#define DBG_PRINT(fmt, ...) do {} while(0)
#endif

/****************************************************************
 * 数据结构定义
 ****************************************************************/
struct modbus_uart_s;

enum {
    MU_LINE_HIGH    = 1<<0,
    MU_ACTIVE       = 1<<1,
    MU_RX_SYNC      = 1<<2,
    MU_REPORT       = 1<<3,
    MU_PULLUP       = 1<<4,
    MU_SINGLE_WIRE  = 1<<5
};

struct modbus_uart_s {
    struct timer timer;
    struct gpio_out tx_pin;
    struct gpio_in rx_pin;
    uint8_t flags;
    uint8_t tx_byte_idx;
    uint8_t tx_bit_idx;
    uint32_t bit_time;
    uint8_t tx_total;
    uint8_t rx_total;
    uint8_t rx_bit_count;    // 已采样的数据位总数（仅数据位）
    uint8_t data[16];
    uint16_t sync_counter;
};

/****************************************************************
 * 函数前置声明
 ****************************************************************/
static void modbus_uart_reset_line(struct modbus_uart_s *m);
static uint_fast8_t modbus_uart_finalize(struct modbus_uart_s *m);
static uint_fast8_t modbus_uart_rx_sync_event(struct timer *timer);
static uint_fast8_t modbus_uart_rx_event(struct timer *timer);
static uint_fast8_t modbus_uart_tx_event(struct timer *timer);

static struct task_wake modbus_uart_wake;

/****************************************************************
 * 基础辅助函数
 ****************************************************************/
static void
modbus_uart_reset_line(struct modbus_uart_s *m)
{
    if (m->flags & MU_SINGLE_WIRE)
        gpio_out_reset(m->tx_pin, 1);
    else
        gpio_out_write(m->tx_pin, 1);
    m->flags = (m->flags & (MU_PULLUP | MU_SINGLE_WIRE)) | MU_LINE_HIGH;
}

static uint_fast8_t
modbus_uart_finalize(struct modbus_uart_s *m)
{
    uint8_t actual_bytes = m->rx_bit_count / 8;
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
 * 接收逻辑：单次同步 + 每字节跳2位（核心时序和成功版本完全一致）
 ****************************************************************/
static uint_fast8_t
modbus_uart_rx_sync_event(struct timer *timer)
{
    struct modbus_uart_s *m = container_of(timer, struct modbus_uart_s, timer);
    uint8_t v = gpio_in_read(m->rx_pin);

    if (v) {
        // 高电平：标记就绪，等待下降沿
        m->flags |= MU_RX_SYNC;
    } else if (m->flags & MU_RX_SYNC) {
        // 检测到高→低跳变：确认起始位
        DBG_PRINT("sync: start bit detected, counter=%d", m->sync_counter);

        // 初始化接收状态
        m->rx_bit_count = 0;
        memset(m->data, 0, sizeof(m->data));

        // 切换到数据采样状态
        m->timer.func = modbus_uart_rx_event;
        // 偏移1.5位时间：跳过起始位，对准第一个数据位中心
        // 这是经过验证的正确相位，绝对不要随便改
        m->timer.waketime += m->bit_time + m->bit_time / 2;
        return SF_RESCHEDULE;
    }

    m->sync_counter++;
    // 超时从200位加到300位，约7.8ms@38400，留足应答时间
    if (m->sync_counter >= 300) {
        DBG_PRINT("sync timeout after %d bits", m->sync_counter);
        return modbus_uart_finalize(m);
    }

    m->timer.waketime += m->bit_time;
    return SF_RESCHEDULE;
}

static uint_fast8_t
modbus_uart_rx_event(struct timer *timer)
{
    struct modbus_uart_s *m = container_of(timer, struct modbus_uart_s, timer);
    uint8_t v = gpio_in_read(m->rx_pin);
    uint8_t bit_pos = m->rx_bit_count;

    // 写入当前位（LSB 在前）
    uint8_t byte_idx = bit_pos / 8;
    uint8_t bit_in_byte = bit_pos % 8;
    if (v && byte_idx < sizeof(m->data))
        m->data[byte_idx] |= (1 << bit_in_byte);

    m->rx_bit_count++;

    // 收满所有数据位，结束
    if (m->rx_bit_count >= m->rx_total * 8) {
        DBG_PRINT("rx all done: %d bits = %d bytes", m->rx_bit_count, m->rx_total);
        return modbus_uart_finalize(m);
    }

    // 每采完1字节，额外跳2位时间（停止位 + 下一字节起始位）
    // 保证下一个采样点对准下一字节数据位中心
    if (m->rx_bit_count % 8 == 0) {
        m->timer.waketime += m->bit_time * 3; // 本步1位 + 跳2位 = 共3位
    } else {
        m->timer.waketime += m->bit_time;
    }

    return SF_RESCHEDULE;
}

/****************************************************************
 * 发送逻辑（保持不变，仅微调收发延迟）
 ****************************************************************/
static uint_fast8_t
modbus_uart_tx_event(struct timer *timer)
{
    struct modbus_uart_s *m = container_of(timer, struct modbus_uart_s, timer);
    uint8_t bit_val;
    uint8_t bit_idx = m->tx_bit_idx;
    uint8_t byte_idx = m->tx_byte_idx;

    if (bit_idx == 0) {
        bit_val = 0; // 起始位
    } else if (bit_idx <= 8) {
        bit_val = (m->data[byte_idx] >> (bit_idx - 1)) & 0x01; // LSB在前
    } else {
        bit_val = 1; // 停止位
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

            if (m->flags & MU_SINGLE_WIRE)
                gpio_in_reset(m->rx_pin, m->flags & MU_PULLUP);

            m->sync_counter = 0;
            m->flags &= ~MU_RX_SYNC;
            m->timer.func = modbus_uart_rx_sync_event;

            // 收发延迟微调：双线模式12位，单线模式20位
            // 既不早到错过应答，也不晚到漏检起始位
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
 * 主机命令接口
 ****************************************************************/
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
 * 结果上报与关机
 ****************************************************************/
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
