// Serial over classic CAN for GD32F303 CAN0.
//
// Copyright (C) 2026  Xiaoyue Cui <2508041672@qq.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h"
#include "board/irq.h"
#include "board/misc.h"
#include "command.h"
#include "generic/armcm_boot.h"
#include "generic/canbus.h"
#include "generic/canserial.h"
#include "internal.h"
#include "sched.h"

#define SOC_CAN CAN0

#if CONFIG_GD32_CANBUS_PA11_PA12
DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PA11,PA12");
#define CAN_RX_PIN GPIO('A', 11)
#define CAN_TX_PIN GPIO('A', 12)
#define CAN_REMAP_VALUE 0U
#elif CONFIG_GD32_CANBUS_PB8_PB9
DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB8,PB9");
#define CAN_RX_PIN GPIO('B', 8)
#define CAN_TX_PIN GPIO('B', 9)
#define CAN_REMAP_VALUE 2U
#elif CONFIG_GD32_CANBUS_PD0_PD1
DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PD0,PD1");
#define CAN_RX_PIN GPIO('D', 0)
#define CAN_TX_PIN GPIO('D', 1)
#define CAN_REMAP_VALUE 3U
#else
#error "Unknown GD32F303 CAN pin mapping"
#endif

#define CAN_TX_IRQn USBD_HP_CAN0_TX_IRQn
#define CAN_RX0_IRQn USBD_LP_CAN0_RX0_IRQn
#define CAN_SCE_IRQn CAN0_EWMC_IRQn

static struct {
    uint32_t rx_error, tx_error;
} can_errors;

int
canhw_send(struct canbus_msg *msg)
{
    uint32_t tstat = CAN_TSTAT(SOC_CAN);
    if (!(tstat & (CAN_TSTAT_TME0 | CAN_TSTAT_TME1 | CAN_TSTAT_TME2))) {
        irq_disable();
        CAN_INTEN(SOC_CAN) |= CAN_INTEN_TMEIE;
        irq_enable();
        return -1;
    }
    uint_fast8_t mailbox = 2;
    if (tstat & CAN_TSTAT_TME0)
        mailbox = 0;
    else if (tstat & CAN_TSTAT_TME1)
        mailbox = 1;

    CAN_TMP(SOC_CAN, mailbox) = msg->dlc & 0x0f;
    CAN_TMDATA0(SOC_CAN, mailbox) = msg->data32[0];
    CAN_TMDATA1(SOC_CAN, mailbox) = msg->data32[1];
    uint32_t ident;
    if (msg->id & CANMSG_ID_EFF)
        ident = ((msg->id & 0x1fffffff) << 3) | CAN_TMI_FF;
    else
        ident = (msg->id & 0x7ff) << 21;
    if (msg->id & CANMSG_ID_RTR)
        ident |= CAN_TMI_FT;
    CAN_TMI(SOC_CAN, mailbox) = ident | CAN_TMI_TEN;
    return CANMSG_DATA_LEN(msg);
}

void
canhw_set_filter(uint32_t id)
{
    uint32_t active = 1U;
    CAN_FCTL(SOC_CAN) |= CAN_FCTL_FLD;
    CAN_FW(SOC_CAN) = 0;
    CAN_FMCFG(SOC_CAN) = 0;       // identifier-mask mode
    CAN_FAFIFO(SOC_CAN) = 0;      // all filters to FIFO0

    if (CONFIG_CANBUS_FILTER) {
        uint32_t mask = (0x7ffU << 21) | CAN_TMI_FF | CAN_TMI_FT;
        CAN_FDATA0(SOC_CAN, 0) = CANBUS_ID_ADMIN << 21;
        CAN_FDATA1(SOC_CAN, 0) = mask;
        CAN_FDATA0(SOC_CAN, 1) = (id + 1) << 21;
        CAN_FDATA1(SOC_CAN, 1) = mask;
        CAN_FDATA0(SOC_CAN, 2) = id << 21;
        CAN_FDATA1(SOC_CAN, 2) = mask;
        if (id)
            active |= (1U << 1) | (1U << 2);
    } else {
        CAN_FDATA0(SOC_CAN, 0) = 0;
        CAN_FDATA1(SOC_CAN, 0) = 0;
    }
    CAN_FSCFG(SOC_CAN) = (1U << 0) | (1U << 1) | (1U << 2);
    CAN_FW(SOC_CAN) = active;
    CAN_FCTL(SOC_CAN) &= ~CAN_FCTL_FLD;
}

void
canhw_get_status(struct canbus_status *status)
{
    irqstatus_t flags = irq_save();
    uint32_t err = CAN_ERR(SOC_CAN);
    status->rx_error = can_errors.rx_error;
    status->tx_error = can_errors.tx_error;
    irq_restore(flags);
    if (err & CAN_ERR_BOERR)
        status->bus_state = CANBUS_STATE_OFF;
    else if (err & CAN_ERR_PERR)
        status->bus_state = CANBUS_STATE_PASSIVE;
    else if (err & CAN_ERR_WERR)
        status->bus_state = CANBUS_STATE_WARN;
    else
        status->bus_state = 0;
}

void
CAN_IRQHandler(void)
{
    if (CAN_RFIFO0(SOC_CAN) & CAN_RFIFO0_RFL0) {
        uint32_t ident = CAN_RFIFOMI(SOC_CAN, 0);
        struct canbus_msg msg;
        if (ident & CAN_RFIFOMI_FF)
            msg.id = ((ident >> 3) & 0x1fffffff) | CANMSG_ID_EFF;
        else
            msg.id = (ident >> 21) & 0x7ff;
        if (ident & CAN_RFIFOMI_FT)
            msg.id |= CANMSG_ID_RTR;
        msg.dlc = CAN_RFIFOMP(SOC_CAN, 0) & CAN_RFIFOMP_DLENC;
        msg.data32[0] = CAN_RFIFOMDATA0(SOC_CAN, 0);
        msg.data32[1] = CAN_RFIFOMDATA1(SOC_CAN, 0);
        CAN_RFIFO0(SOC_CAN) = CAN_RFIFO0_RFD0;
        canbus_process_data(&msg);
    }

    uint32_t inten = CAN_INTEN(SOC_CAN);
    if ((inten & CAN_INTEN_TMEIE)
        && (CAN_TSTAT(SOC_CAN)
            & (CAN_TSTAT_MTF0 | CAN_TSTAT_MTF1 | CAN_TSTAT_MTF2))) {
        CAN_INTEN(SOC_CAN) = inten & ~CAN_INTEN_TMEIE;
        canbus_notify_tx();
    }

    if (CAN_STAT(SOC_CAN) & CAN_STAT_ERRIF) {
        uint32_t err = CAN_ERR(SOC_CAN);
        uint32_t code = GET_ERR_ERRN(err);
        if (code && code != 7) {
            CAN_ERR(SOC_CAN) = ERR_ERRN(7);
            if (code >= 3 && code <= 5)
                can_errors.tx_error++;
            else
                can_errors.rx_error++;
        }
        CAN_STAT(SOC_CAN) = CAN_STAT_ERRIF;
    }
}

static uint32_t
compute_btr(uint32_t pclock, uint32_t bitrate)
{
    if (!bitrate || pclock % bitrate)
        shutdown("Invalid CAN bitrate");
    uint32_t bit_clocks = pclock / bitrate;
    uint32_t quanta;
    for (quanta = 18; quanta > 9; quanta--)
        if (!(bit_clocks % quanta))
            break;
    if (quanta <= 9)
        shutdown("Unsupported CAN bitrate");
    uint32_t brp = bit_clocks / quanta;
    if (!brp || brp > 1024)
        shutdown("Unsupported CAN prescaler");
    uint32_t seg2 = quanta / 8;
    // SJW is 2TQ, so BS2 must be at least 2TQ as well.
    if (seg2 < 2)
        seg2 = 2;
    uint32_t seg1 = quanta - 1 - seg2;
    return ((2U - 1U) << 24) | ((seg1 - 1U) << 16)
           | ((seg2 - 1U) << 20) | (brp - 1U);
}

static void
can_wait_state(uint32_t mask, uint8_t want_set)
{
    uint32_t timeout = timer_read_time() + timer_from_us(100000);
    while (!!(CAN_STAT(SOC_CAN) & mask) != want_set) {
        if (!timer_is_before(timer_read_time(), timeout))
            shutdown("CAN initialization timeout");
    }
}

void
sched_can_init(void)
{
    enable_pclock(RCU_AF);
    enable_pclock(RCU_CAN0);
    AFIO_PCF0 = (AFIO_PCF0 & ~AFIO_PCF0_CAN_REMAP)
                | (CAN_REMAP_VALUE << 13);
    gpio_peripheral(CAN_RX_PIN, 1, 1);
    gpio_peripheral(CAN_TX_PIN, 3, 0);

    CAN_CTL(SOC_CAN) = CAN_CTL_IWMOD;
    can_wait_state(CAN_STAT_IWS, 1);
    CAN_BT(SOC_CAN) = compute_btr(APB1_FREQ, CONFIG_CANBUS_FREQUENCY);
    CAN_CTL(SOC_CAN) = CAN_CTL_TFO | CAN_CTL_ABOR;
    can_wait_state(CAN_STAT_IWS, 0);

    canhw_set_filter(0);
    armcm_enable_irq(CAN_IRQHandler, CAN_RX0_IRQn, 0);
    armcm_enable_irq(CAN_IRQHandler, CAN_TX_IRQn, 0);
    armcm_enable_irq(CAN_IRQHandler, CAN_SCE_IRQn, 0);
    CAN_INTEN(SOC_CAN) = CAN_INTEN_RFNEIE0 | CAN_INTEN_ERRIE
                         | CAN_INTEN_ERRNIE;
}
DECL_INIT(sched_can_init);
