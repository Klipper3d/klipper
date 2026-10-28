// Serial over CAN emulation for LPC176x boards.
//
// Copyright (C) 2026  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CANBUS_FREQUENCY
#include "board/armcm_boot.h" // armcm_enable_irq
#include "board/irq.h" // irq_save
#include "command.h" // DECL_CONSTANT_STR
#include "compiler.h" // DIV_ROUND_CLOSEST
#include "generic/canbus.h" // canbus_notify_tx
#include "internal.h" // enable_pclock
#include "sched.h" // DECL_INIT

#if CONFIG_LPC_CANBUS_P0_0_P0_1
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "P0.0,P0.1");
 #define LPC_CANx LPC_CAN1
 #define PCLK_CANx PCLK_CAN1
 #define GPIO_Rx GPIO(0, 0)
 #define GPIO_Tx GPIO(0, 1)
 #define CAN_FUNCTION_Rx 1
 #define CAN_FUNCTION_Tx 1
#elif CONFIG_LPC_CANBUS_P0_21_P0_22
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "P0.21,P0.22");
 #define LPC_CANx LPC_CAN1
 #define PCLK_CANx PCLK_CAN1
 #define GPIO_Rx GPIO(0, 21)
 #define GPIO_Tx GPIO(0, 22)
 #define CAN_FUNCTION_Rx 2
 #define CAN_FUNCTION_Tx 2
#elif CONFIG_LPC_CANBUS_P0_4_P0_5
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "P0.4,P0.5");
 #define LPC_CANx LPC_CAN2
 #define PCLK_CANx PCLK_CAN2
 #define GPIO_Rx GPIO(0, 4)
 #define GPIO_Tx GPIO(0, 5)
 #define CAN_FUNCTION_Rx 2
 #define CAN_FUNCTION_Tx 2
#elif CONFIG_LPC_CANBUS_P2_7_P2_8
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "P2.7,P2.8");
 #define LPC_CANx LPC_CAN2
 #define PCLK_CANx PCLK_CAN2
 #define GPIO_Rx GPIO(2, 7)
 #define GPIO_Tx GPIO(2, 8)
 #define CAN_FUNCTION_Rx 1
 #define CAN_FUNCTION_Tx 1
#else
 #error No known CAN pins configured for LPC176x
#endif

#define CAN_MOD_RM             (1<<0)

#define CAN_CMR_TR             (1<<0)
#define CAN_CMR_RRB            (1<<2)
#define CAN_CMR_CDO            (1<<3)
#define CAN_CMR_STB1           (1<<5)

#define CAN_GSR_ES             (1<<6)
#define CAN_GSR_BS             (1<<7)
#define CAN_GSR_RXERR_Pos      16
#define CAN_GSR_TXERR_Pos      24

#define CAN_IER_RIE            (1<<0)
#define CAN_IER_TIE1           (1<<1)
#define CAN_IER_EIE            (1<<2)
#define CAN_IER_DOIE           (1<<3)
#define CAN_IER_EPIE           (1<<5)
#define CAN_IER_BEIE           (1<<7)

#define CAN_ICR_RI             (1<<0)
#define CAN_ICR_TI1            (1<<1)
#define CAN_ICR_EI             (1<<2)
#define CAN_ICR_DOI            (1<<3)
#define CAN_ICR_EPI            (1<<5)
#define CAN_ICR_BEI            (1<<7)
#define CAN_ICR_ERRDIR_RECEIVE (1<<21)

#define CAN_SR_RBS             (1<<0)
#define CAN_SR_TBS1            (1<<2)

#define CAN_TFI_DLC_Pos        16
#define CAN_TFI_RTR            (1u<<30)
#define CAN_TFI_FF             (1u<<31)

#define CAN_RFS_DLC_Pos        16
#define CAN_RFS_RTR            (1u<<30)
#define CAN_RFS_FF             (1u<<31)

#define CAN_BTR_SJW_Pos        14
#define CAN_BTR_TSEG1_Pos      16
#define CAN_BTR_TSEG2_Pos      20

#define CANAF_AFMR_ACCBP       (1<<1)

#define CAN_ERROR_PASSIVE_LIMIT 128
#define CAN_ERROR_WARNING_LIMIT 96

static struct {
    uint32_t rx_error, tx_error;
} CAN_Errors;

static uint32_t
make_btr(uint32_t sjw, uint32_t time_seg1, uint32_t time_seg2, uint32_t brp)
{
    return (((brp - 1) & 0x3ff)
            | ((sjw - 1) << CAN_BTR_SJW_Pos)
            | ((time_seg1 - 1) << CAN_BTR_TSEG1_Pos)
            | ((time_seg2 - 1) << CAN_BTR_TSEG2_Pos));
}

static uint32_t
compute_btr(uint32_t pclock, uint32_t bitrate)
{
    uint32_t bit_clocks = pclock / bitrate;
    uint32_t best_sample_error = ~0, best_brp = bit_clocks / 10;
    uint32_t time_seg1 = 7, time_seg2 = 2;
    int qs;
    for (qs = 25; qs >= 8; qs--) {
        if (bit_clocks % qs)
            continue;
        uint32_t brp = bit_clocks / qs;
        if (!brp || brp > 1024)
            continue;

        uint32_t cur_time_seg2 = DIV_ROUND_CLOSEST(qs, 8);
        if (cur_time_seg2 < 2)
            cur_time_seg2 = 2;
        if (cur_time_seg2 > 8)
            cur_time_seg2 = 8;

        uint32_t cur_time_seg1 = qs - (1 + cur_time_seg2);
        if (cur_time_seg1 > 16) {
            cur_time_seg1 = 16;
            cur_time_seg2 = qs - (1 + cur_time_seg1);
        }
        if (!cur_time_seg1 || cur_time_seg2 < 2 || cur_time_seg2 > 8)
            continue;

        uint32_t sample_point = DIV_ROUND_CLOSEST(
            (1 + cur_time_seg1) * 1000, qs);
        uint32_t sample_error = sample_point > 875
                                ? sample_point - 875 : 875 - sample_point;
        if (sample_error < best_sample_error) {
            best_sample_error = sample_error;
            best_brp = brp;
            time_seg1 = cur_time_seg1;
            time_seg2 = cur_time_seg2;
        }
    }

    return make_btr(2, time_seg1, time_seg2, best_brp);
}

// Transmit a packet
int
canhw_send(struct canbus_msg *msg)
{
    if (!(LPC_CANx->SR & CAN_SR_TBS1)) {
        irqstatus_t flag = irq_save();
        LPC_CANx->IER |= CAN_IER_TIE1;
        if (LPC_CANx->SR & CAN_SR_TBS1) {
            LPC_CANx->IER &= ~CAN_IER_TIE1;
            irq_restore(flag);
            canbus_notify_tx();
        } else {
            irq_restore(flag);
        }
        return -1;
    }

    uint32_t tfi = (CANMSG_DATA_LEN(msg) << CAN_TFI_DLC_Pos);
    if (msg->id & CANMSG_ID_EFF)
        tfi |= CAN_TFI_FF;
    if (msg->id & CANMSG_ID_RTR)
        tfi |= CAN_TFI_RTR;

    LPC_CANx->TFI1 = tfi;
    LPC_CANx->TID1 = msg->id & (msg->id & CANMSG_ID_EFF ? 0x1fffffff : 0x7ff);
    LPC_CANx->TDA1 = msg->data32[0];
    LPC_CANx->TDB1 = msg->data32[1];
    LPC_CANx->CMR = CAN_CMR_STB1 | CAN_CMR_TR;
    return CANMSG_DATA_LEN(msg);
}

// Setup the receive packet filter
void
canhw_set_filter(uint32_t id)
{
    // Keep the LPC acceptance filter in bypass mode; canserial filters in
    // software and this avoids a fragile AF LUT rewrite while the bus is live.
}

// Report interface status
void
canhw_get_status(struct canbus_status *status)
{
    irqstatus_t flag = irq_save();
    uint32_t gsr = LPC_CANx->GSR;
    uint32_t rx_error = CAN_Errors.rx_error, tx_error = CAN_Errors.tx_error;
    irq_restore(flag);

    uint32_t rx_count = (gsr >> CAN_GSR_RXERR_Pos) & 0xff;
    uint32_t tx_count = (gsr >> CAN_GSR_TXERR_Pos) & 0xff;
    status->rx_error = rx_error;
    status->tx_error = tx_error;
    if (gsr & CAN_GSR_BS)
        status->bus_state = CANBUS_STATE_OFF;
    else if (rx_count >= CAN_ERROR_PASSIVE_LIMIT
             || tx_count >= CAN_ERROR_PASSIVE_LIMIT)
        status->bus_state = CANBUS_STATE_PASSIVE;
    else if (gsr & CAN_GSR_ES)
        status->bus_state = CANBUS_STATE_WARN;
    else
        status->bus_state = CANBUS_STATE_ACTIVE;
}

static void
can_process_rx(void)
{
    while (LPC_CANx->SR & CAN_SR_RBS) {
        uint32_t rfs = LPC_CANx->RFS;
        struct canbus_msg msg;
        if (rfs & CAN_RFS_FF)
            msg.id = (LPC_CANx->RID & 0x1fffffff) | CANMSG_ID_EFF;
        else
            msg.id = LPC_CANx->RID & 0x7ff;
        msg.id |= rfs & CAN_RFS_RTR ? CANMSG_ID_RTR : 0;
        msg.dlc = (rfs >> CAN_RFS_DLC_Pos) & 0x0f;
        msg.data32[0] = LPC_CANx->RDA;
        msg.data32[1] = LPC_CANx->RDB;
        LPC_CANx->CMR = CAN_CMR_RRB;

        canbus_process_data(&msg);
    }
}

// This function handles CAN global interrupts
void
CAN_IRQHandler(void)
{
    uint32_t icr = LPC_CANx->ICR;

    if (icr & (CAN_ICR_RI | CAN_ICR_DOI))
        can_process_rx();

    if (icr & CAN_ICR_DOI) {
        CAN_Errors.rx_error += 1;
        LPC_CANx->CMR = CAN_CMR_CDO;
    }

    if (icr & CAN_ICR_TI1) {
        LPC_CANx->IER &= ~CAN_IER_TIE1;
        canbus_notify_tx();
    }

    if (icr & CAN_ICR_BEI) {
        if (icr & CAN_ICR_ERRDIR_RECEIVE)
            CAN_Errors.rx_error += 1;
        else
            CAN_Errors.tx_error += 1;
    }

    if (icr & (CAN_ICR_EI | CAN_ICR_EPI)) {
        uint32_t gsr = LPC_CANx->GSR;
        if (((gsr >> CAN_GSR_RXERR_Pos) & 0xff)
            >= CAN_ERROR_WARNING_LIMIT)
            CAN_Errors.rx_error += 1;
        if (((gsr >> CAN_GSR_TXERR_Pos) & 0xff)
            >= CAN_ERROR_WARNING_LIMIT)
            CAN_Errors.tx_error += 1;
    }
}

void
can_init(void)
{
    // Enable clock and hold the CAN controller in reset while configuring it.
    enable_pclock(PCLK_CANx);
    LPC_CANx->MOD = CAN_MOD_RM;
    LPC_CANx->IER = 0;

    gpio_peripheral(GPIO_Rx, CAN_FUNCTION_Rx, 1);
    gpio_peripheral(GPIO_Tx, CAN_FUNCTION_Tx, 0);

    LPC_CANAF->AFMR = CANAF_AFMR_ACCBP;
    LPC_CANx->BTR = compute_btr(get_pclock_frequency(PCLK_CANx)
                                , CONFIG_CANBUS_FREQUENCY);
    LPC_CANx->EWL = CAN_ERROR_WARNING_LIMIT;

    // Clear any stale interrupt state and enter normal operating mode.
    uint32_t icr = LPC_CANx->ICR;
    (void)icr;
    LPC_CANx->MOD = 0;

    armcm_enable_irq(CAN_IRQHandler, CAN_IRQn, 0);
    LPC_CANx->IER = CAN_IER_RIE | CAN_IER_EIE | CAN_IER_DOIE
                    | CAN_IER_EPIE | CAN_IER_BEIE;
}
DECL_INIT(can_init);
