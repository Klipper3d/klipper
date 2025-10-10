// CANbus support on atsame51 chips
//
// Copyright (C) 2021-2025  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2019 Eug Krashtan <eug.krashtan@gmail.com>
// Copyright (C) 2020 Pontus Borg <glpontus@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_save
#include "command.h" // DECL_CONSTANT_STR
#include "generic/armcm_boot.h" // armcm_enable_irq
#include "generic/canbus.h" // canbus_notify_tx
#include "generic/canserial.h" // CANBUS_ID_ADMIN
#include "internal.h" // enable_pclock
#include "sched.h" // DECL_INIT


/****************************************************************
 * Pin configuration
 ****************************************************************/

#if CONFIG_ATSAMD_CANBUS_PA23_PA22
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PA23,PA22");
 #define GPIO_Rx GPIO('A', 23)
 #define GPIO_Tx GPIO('A', 22)
 #define CANx_GCLK_ID CAN0_GCLK_ID
#elif CONFIG_ATSAMD_CANBUS_PA25_PA24
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PA25,PA24");
 #define GPIO_Rx GPIO('A', 25)
 #define GPIO_Tx GPIO('A', 24)
 #define CANx_GCLK_ID CAN0_GCLK_ID
#elif CONFIG_ATSAMD_CANBUS_PB11_PB10
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB11,PB10");
 #define GPIO_Rx GPIO('B', 11)
 #define GPIO_Tx GPIO('B', 10)
 #define CANx_GCLK_ID CAN1_GCLK_ID
#elif CONFIG_ATSAMD_CANBUS_PB13_PB12
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB13,PB12");
 #define GPIO_Rx GPIO('B', 13)
 #define GPIO_Tx GPIO('B', 12)
 #define CANx_GCLK_ID CAN1_GCLK_ID
#elif CONFIG_ATSAMD_CANBUS_PB15_PB14
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB15,PB14");
 #define GPIO_Rx GPIO('B', 15)
 #define GPIO_Tx GPIO('B', 14)
 #define CANx_GCLK_ID CAN1_GCLK_ID
#endif

#if CANx_GCLK_ID == CAN0_GCLK_ID && CONFIG_MACH_SAMC21
 #define CAN_FUNCTION 'G'
 #define CANx CAN0
 #define CANx_IRQn CAN0_IRQn
 #define MCLK_AHBMASK_CANx MCLK_AHBMASK_CAN0
#elif CANx_GCLK_ID == CAN1_GCLK_ID && CONFIG_MACH_SAMC21
 #define CAN_FUNCTION 'G'
 #define CANx CAN1
 #define CANx_IRQn CAN1_IRQn
 #define MCLK_AHBMASK_CANx MCLK_AHBMASK_CAN1
#elif CANx_GCLK_ID == CAN0_GCLK_ID
 #define CAN_FUNCTION 'I'
 #define CANx CAN0
 #define CANx_IRQn CAN0_IRQn
#else
 #define CAN_FUNCTION 'H'
 #define CANx CAN1
 #define CANx_IRQn CAN1_IRQn
#endif


/****************************************************************
 * Message ram layout
 ****************************************************************/

struct fdcan_fifo {
    uint32_t id_section;
    uint32_t dlc_section;
    uint32_t data[64 / 4];
};

#define FDCAN_XTD (1<<30)
#define FDCAN_RTR (1<<29)

struct fdcan_msg_ram {
    uint32_t FLS[28]; // Filter list standard
    uint32_t FLE[16]; // Filter list extended
    struct fdcan_fifo RXF0[3];
    struct fdcan_fifo RXF1[3];
    uint32_t TEF[6]; // Tx event FIFO
    struct fdcan_fifo TXFIFO[3];
};

// Message ram is in regular memory
static struct fdcan_msg_ram MSG_RAM;


/****************************************************************
 * CANbus code
 ****************************************************************/

#define FDCAN_IE_TC        (CAN_IE_TCE | CAN_IE_TCFE | CAN_IE_TFEE)

// Transmit a packet
int
canhw_send(struct canbus_msg *msg)
{
    uint32_t txfqs = CANx->TXFQS.reg;
    if (txfqs & CAN_TXFQS_TFQF)
        // No space in transmit fifo - wait for irq
        return -1;

    uint32_t w_index = ((txfqs & CAN_TXFQS_TFQPI_Msk) >> CAN_TXFQS_TFQPI_Pos);
    struct fdcan_fifo *txfifo = &MSG_RAM.TXFIFO[w_index];
    uint32_t ids;
    if (msg->id & CANMSG_ID_EFF)
        ids = (msg->id & 0x1fffffff) | FDCAN_XTD;
    else
        ids = (msg->id & 0x7ff) << 18;
    ids |= msg->id & CANMSG_ID_RTR ? FDCAN_RTR : 0;
    txfifo->id_section = ids;
    txfifo->dlc_section = (msg->dlc & 0x0f) << 16;
    txfifo->data[0] = msg->data32[0];
    txfifo->data[1] = msg->data32[1];
    __DMB();
    CANx->TXBAR.reg;
    CANx->TXBAR.reg = ((uint32_t)1 << w_index);
    return CANMSG_DATA_LEN(msg);
}

static void
can_filter(uint32_t index, uint32_t id)
{
    MSG_RAM.FLS[index] = ((0x2 << 30) // Classic filter
                          | (0x1 << 27) // Store in Rx FIFO 0 if filter matches
                          | (id << 16)
                          | 0x7FF); // mask all enabled
}

// Setup the receive packet filter
void
canhw_set_filter(uint32_t id)
{
    if (!CONFIG_CANBUS_FILTER)
        return;
    /* Request initialisation */
    CANx->CCCR.reg |= CAN_CCCR_INIT;
    /* Wait the acknowledge */
    while (!(CANx->CCCR.reg & CAN_CCCR_INIT))
        ;
    /* Enable configuration change */
    CANx->CCCR.reg |= CAN_CCCR_CCE;

    // Load filter
    can_filter(0, CANBUS_ID_ADMIN);
    can_filter(1, id);
    can_filter(2, id + 1);

    uint32_t flssa = (uint32_t)MSG_RAM.FLS - CAN0_MSG_RAM_ADDR;
    CANx->SIDFC.reg = flssa | ((id ? 3 : 1) << CAN_SIDFC_LSS_Pos);
    CANx->GFC.reg = 0x02 << CAN_GFC_ANFS_Pos;

    /* Leave the initialisation mode for the filter */
    barrier();
    CANx->CCCR.reg &= ~CAN_CCCR_CCE;
    CANx->CCCR.reg &= ~CAN_CCCR_INIT;
}

static struct {
    uint32_t rx_error, tx_error;
} CAN_Errors;

// Report interface status
void
canhw_get_status(struct canbus_status *status)
{
    irqstatus_t flag = irq_save();
    uint32_t psr = CANx->PSR.reg, lec = psr & CAN_PSR_LEC_Msk;
    if (lec && lec != 7) {
        // Reading PSR clears it - so update state here
        if (lec >= 3 && lec <= 5)
            CAN_Errors.tx_error += 1;
        else
            CAN_Errors.rx_error += 1;
    }
    uint32_t rx_error = CAN_Errors.rx_error, tx_error = CAN_Errors.tx_error;
    irq_restore(flag);

    status->rx_error = rx_error;
    status->tx_error = tx_error;
    if (psr & CAN_PSR_BO)
        status->bus_state = CANBUS_STATE_OFF;
    else if (psr & CAN_PSR_EP)
        status->bus_state = CANBUS_STATE_PASSIVE;
    else if (psr & CAN_PSR_EW)
        status->bus_state = CANBUS_STATE_WARN;
    else
        status->bus_state = 0;
}

// This function handles CAN global interrupts
void
CAN_IRQHandler(void)
{
    uint32_t ir = CANx->IR.reg;

    if (ir & CAN_IE_RF0NE) {
        CANx->IR.reg = CAN_IE_RF0NE;

        uint32_t rxf0s = CANx->RXF0S.reg;
        if (rxf0s & CAN_RXF0S_F0FL_Msk) {
            // Read and ack data packet
            uint32_t idx = (rxf0s & CAN_RXF0S_F0GI_Msk) >> CAN_RXF0S_F0GI_Pos;
            struct fdcan_fifo *rxf0 = &MSG_RAM.RXF0[idx];
            uint32_t ids = rxf0->id_section;
            struct canbus_msg msg;
            if (ids & FDCAN_XTD)
                msg.id = (ids & 0x1fffffff) | CANMSG_ID_EFF;
            else
                msg.id = (ids >> 18) & 0x7ff;
            msg.id |= ids & FDCAN_RTR ? CANMSG_ID_RTR : 0;
            msg.dlc = (rxf0->dlc_section >> 16) & 0x0f;
            msg.data32[0] = rxf0->data[0];
            msg.data32[1] = rxf0->data[1];
            barrier();
            CANx->RXF0A.reg = idx;

            // Process packet
            canbus_process_data(&msg);
        }
    }
    if (ir & FDCAN_IE_TC) {
        // Tx
        CANx->IR.reg = FDCAN_IE_TC;
        canbus_notify_tx();
    }
    if (ir & (CAN_IR_PED | CAN_IR_PEA)) {
        // Bus error
        uint32_t psr = CANx->PSR.reg;
        CANx->IR.reg = CAN_IR_PED | CAN_IR_PEA;
        uint32_t lec = psr & CAN_PSR_LEC_Msk;
        if (lec && lec != 7) {
            if (lec >= 3 && lec <= 5)
                CAN_Errors.tx_error += 1;
            else
                CAN_Errors.rx_error += 1;
        }
    }
}

static inline const uint32_t
make_btr(uint32_t sjw,       // Sync jump width, ... hmm
         uint32_t time_seg1, // time segment before sample point, 1 .. 16
         uint32_t time_seg2, // time segment after sample point, 1 .. 8
         uint32_t brp)       // Baud rate prescaler, 1 .. 1024
{
    return (((uint32_t)(sjw-1)) << CAN_NBTP_NSJW_Pos
            | ((uint32_t)(time_seg1-1)) << CAN_NBTP_NTSEG1_Pos
            | ((uint32_t)(time_seg2-1)) << CAN_NBTP_NTSEG2_Pos
            | ((uint32_t)(brp - 1)) << CAN_NBTP_NBRP_Pos);
}

static inline const uint32_t
compute_btr(uint32_t pclock, uint32_t bitrate)
{
    /*
        Some equations:
        Tpclock = 1 / pclock
        Tq      = brp * Tpclock
        Tbs1    = Tq * TS1
        Tbs2    = Tq * TS2
        NominalBitTime = Tq + Tbs1 + Tbs2
        BaudRate = 1/NominalBitTime
        Bit value sample point is after Tq+Tbs1. Ideal sample point
        is at 87.5% of NominalBitTime
        Use the lowest brp where ts1 and ts2 are in valid range
     */

    uint32_t bit_clocks = pclock / bitrate; // clock ticks per bit

    uint32_t sjw = 2;
    uint32_t qs;
    // Find number of time quantas that gives us the exact wanted bit time
    for (qs = 18; qs > 9; qs--) {
        // check that bit_clocks / quantas is an integer
        uint32_t brp_rem = bit_clocks % qs;
        if (brp_rem == 0)
            break;
    }
    uint32_t brp       = bit_clocks / qs;
    uint32_t time_seg2 = qs / 8; // sample at ~87.5%
    uint32_t time_seg1 = qs - (1 + time_seg2);

    return make_btr(sjw, time_seg1, time_seg2, brp);
}

void
can_init(void)
{
#if CONFIG_HAVE_SAMD_USB
    if (!CONFIG_USB) {
        // The FDCAN peripheral only seems to run if at least one
        // other peripheral is also enabled.
        enable_pclock(USB_GCLK_ID, ID_USB);
        USB->DEVICE.CTRLA.reg = USB_CTRLA_ENABLE;
    }
#endif

#if CONFIG_MACH_SAMC21
    MCLK->AHBMASK.reg |= MCLK_AHBMASK_CANx;
#endif
    enable_pclock(CANx_GCLK_ID, -1);

    gpio_peripheral(GPIO_Rx, CAN_FUNCTION, 1);
    gpio_peripheral(GPIO_Tx, CAN_FUNCTION, 0);

    uint32_t pclock = get_pclock_frequency(CANx_GCLK_ID);

    uint32_t btr = compute_btr(pclock, CONFIG_CANBUS_FREQUENCY);

    /*##-1- Configure the CAN #######################################*/

    /* Exit from sleep mode */
    CANx->CCCR.reg &= ~CAN_CCCR_CSR;
    /* Wait the acknowledge */
    while (CANx->CCCR.reg & CAN_CCCR_CSA)
        ;
    /* Request initialisation */
    CANx->CCCR.reg |= CAN_CCCR_INIT;
    /* Wait the acknowledge */
    while (!(CANx->CCCR.reg & CAN_CCCR_INIT))
        ;
    /* Enable configuration change */
    CANx->CCCR.reg |= CAN_CCCR_CCE;

    /* Disable protocol exception handling */
    CANx->CCCR.reg |= CAN_CCCR_PXHD;

    CANx->NBTP.reg = btr;

    /* Setup message RAM addresses */
    uint32_t f0sa = (uint32_t)MSG_RAM.RXF0 - CAN0_MSG_RAM_ADDR;
    CANx->RXF0C.reg = f0sa | (ARRAY_SIZE(MSG_RAM.RXF0) << CAN_RXF0C_F0S_Pos);
    CANx->RXESC.reg = (7 << CAN_RXESC_F1DS_Pos) | (7 << CAN_RXESC_F0DS_Pos);
    uint32_t tbsa = (uint32_t)MSG_RAM.TXFIFO - CAN0_MSG_RAM_ADDR;
    CANx->TXBC.reg = tbsa | (ARRAY_SIZE(MSG_RAM.TXFIFO) << CAN_TXBC_TFQS_Pos);
    CANx->TXESC.reg = 7 << CAN_TXESC_TBDS_Pos;

    /* Leave the initialisation mode */
    CANx->CCCR.reg &= ~CAN_CCCR_CCE;
    CANx->CCCR.reg &= ~CAN_CCCR_INIT;

    /*##-2- Configure the CAN Filter #######################################*/
    canhw_set_filter(0);

    /*##-3- Configure Interrupts #################################*/
    armcm_enable_irq(CAN_IRQHandler, CANx_IRQn, 1);
    CANx->ILE.reg = CAN_ILE_EINT0;
    CANx->IE.reg = CAN_IE_RF0NE | FDCAN_IE_TC | CAN_IE_PEDE | CAN_IE_PEAE;
}
DECL_INIT(can_init);
