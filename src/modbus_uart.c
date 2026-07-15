// Standard 8N1 UART bitbang for Modbus RTU
// 修复：每字节采样后自动跳过停止位+起始位，保证数据位采样点始终居中
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdio.h>
#include <string.h>
#include "board/gpio.h"
#include "board/irq.h"
#include "board/misc.h"
#include "basecmd.h"
#include "command.h"
#include "sched.h"

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
    uint8_t rx_total;        // 期望接收的总字节数
    uint8_t rx_bit_count;    // 已采样的数据位总数（仅计数数据位，不含控制位）
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
    // 调试日志：接收结果
    printf("[DBG] finalize: rx_expected=%d bytes, rx_data_bits=%d\n",
           m->rx_total, m->rx_bit_count);
    uint8_t actual_bytes = m->rx_bit_count / 8;
    if (actual_bytes > 0) {
        printf("[DBG] rx data hex: ");
        for (int i = 0; i < actual_bytes; i++)
            printf("%02x ", m->data[i]);
        printf("\n");
    }

    modbus_uart_reset_line(m);
    m->flags |= MU_REPORT;
    sched_wake_task(&modbus_uart_wake);
    return SF_DONE;
}

/****************************************************************
 * 接收逻辑：单次同步 + 每字节自动跳过控制位（核心修复点）
 ****************************************************************/

/**
 * @brief  同步状态：只检测一次整帧起始位
 */
static uint_fast8_t
modbus_uart_rx_sync_event(struct timer *timer)
{
    struct modbus_uart_s *m = container_of(timer, struct modbus_uart_s, timer);
    uint8_t v = gpio_in_read(m->rx_pin);

    if (v) {
        // 先检测到高电平，标记同步就绪
        m->flags |= MU_RX_SYNC;
    } else if (m->flags & MU_RX_SYNC) {
        // 检测到高→低下降沿：第一个字节的起始位
        printf("[DBG] rx sync: start bit detected, sync_counter=%d\n", m->sync_counter);

        // 初始化接收状态
        m->rx_bit_count = 0;
        memset(m->data, 0, sizeof(m->data));

        // 切换到连续采样状态
        m->timer.func = modbus_uart_rx_event;
        // 偏移1.5位时间：跳过起始位，对准第一个数据位的中心
        m->timer.waketime += m->bit_time + m->bit_time / 2;
        return SF_RESCHEDULE;
    }

    m->sync_counter++;
    if (m->sync_counter >= 200) {
        printf("[DBG] rx sync timeout after %d counts\n", m->sync_counter);
        return modbus_uart_finalize(m);
    }

    m->timer.waketime += m->bit_time;
    return SF_RESCHEDULE;
}

/**
 * @brief  连续采样状态：每采完1字节自动跳过停止位+下一起始位
 * @note   保证每个数据位的采样点都精准落在电平中心，消除逐字节累积偏移
 */
static uint_fast8_t
modbus_uart_rx_event(struct timer *timer)
{
    struct modbus_uart_s *m = container_of(timer, struct modbus_uart_s, timer);
    uint8_t v = gpio_in_read(m->rx_pin);
    uint8_t bit_pos = m->rx_bit_count;

    // 计算当前位属于第几个字节、第几位，写入对应比特（LSB 在前）
    uint8_t byte_idx = bit_pos / 8;
    uint8_t bit_in_byte = bit_pos % 8;
    if (v && byte_idx < sizeof(m->data))
        m->data[byte_idx] |= (1 << bit_in_byte);

    m->rx_bit_count++;

    // 收满所有数据位，结束接收
    if (m->rx_bit_count >= m->rx_total * 8) {
        printf("[DBG] rx all done: %d bits = %d bytes\n",
               m->rx_bit_count, m->rx_total);
        return modbus_uart_finalize(m);
    }

    // ========== 核心修复：刚采完一个完整字节，跳过停止位和下一个起始位 ==========
    if (m->rx_bit_count % 8 == 0) {
        // 正常步进1位 + 额外跳过2位（停止位 + 下一字节起始位） = 总共步进3位时间
        // 确保下一个采样点直接对准下一字节数据位0的中心
        m->timer.waketime += m->bit_time * 3;
    } else {
        // 普通数据位，步进1位时间
        m->timer.waketime += m->bit_time;
    }

    return SF_RESCHEDULE;
}

/****************************************************************
 * 发送逻辑（保持不变，已验证正确）
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
        bit_val = (m->data[byte_idx] >> (bit_idx - 1)) & 0x01; // 数据位 LSB 在前
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
            printf("[DBG] tx all done: tx_total=%d, rx_total=%d\n",
                   m->tx_total, m->rx_total);

            if (m->rx_total == 0)
                return modbus_uart_finalize(m);

            if (m->flags & MU_SINGLE_WIRE)
                gpio_in_reset(m->rx_pin, m->flags & MU_PULLUP);

            m->sync_counter = 0;
            m->flags &= ~MU_RX_SYNC;
            m->timer.func = modbus_uart_rx_sync_event;
            // 发送结束后延迟20位再开始接收，给芯片处理时间
            m->timer.waketime += m->bit_time * 20;
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

    printf("[DBG] config: oid=%d rx=%u tx=%u pullup=%d bit_time=%u\n",
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
        printf("[DBG] send: busy, drop\n");
        return;
    }

    uint8_t write_len = args[1];
    uint8_t *write = command_decode_ptr(args[2]);
    uint8_t read_len = args[3];

    printf("[DBG] send cmd: write_len=%d read_len=%d\n", write_len, read_len);
    printf("[DBG] write hex: ");
    for (int i = 0; i < write_len; i++)
        printf("%02x ", write[i]);
    printf("\n");

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
        printf("[DBG] report to host: oid=%d bytes=%d\n", oid, actual_bytes);

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
