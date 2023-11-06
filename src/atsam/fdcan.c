// CANbus support on atsame70 chips
//
// Copyright (C) 2021-2022  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2019 Eug Krashtan <eug.krashtan@gmail.com>
// Copyright (C) 2020 Pontus Borg <glpontus@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // DECL_CONSTANT_STR
#include "generic/armcm_boot.h" // armcm_enable_irq
#include "generic/canbus.h" // canbus_notify_tx
#include "generic/canserial.h" // CANBUS_ID_ADMIN
#include "internal.h" // enable_pclock
#include "sched.h" // DECL_INIT


/****************************************************************
 * Pin configuration
 ****************************************************************/

#if CONFIG_ATSAM_CANBUS_PB3_PB2
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PB3,PB2");
 #define GPIO_Rx GPIO('B', 3)
 #define GPIO_Tx GPIO('B', 2)
 #define CANx_GCLK_ID MCAN0_CLOCK_ID
#elif CONFIG_ATSAM_CANBUS_PC12_PD12
 DECL_CONSTANT_STR("RESERVE_PINS_CAN", "PC12,PD12");
 #define GPIO_Rx GPIO('C', 12)
 #define GPIO_Tx GPIO('D', 12)
 #define CANx_GCLK_ID MCAN1_CLOCK_ID
#endif

#if CANx_GCLK_ID == MCAN0_CLOCK_ID
 #define CAN_FUNCTION_Rx 'A'
 #define CAN_FUNCTION_Tx 'A'
 #define CANx MCAN0
 #define CANx_IRQn MCAN0_INT0_IRQn
 #define CCFG_CANxDMABA MATRIX->CCFG_CAN0
 #define CCFG_CANxDMABA_Msk CCFG_CAN0_CAN0DMABA_Msk
 #define CCFG_CANxDMABA_Pos CCFG_CAN0_CAN0DMABA_Pos
#else
 #define CAN_FUNCTION_Rx 'C'
 #define CAN_FUNCTION_Tx 'B'
 #define CANx MCAN1
 #define CANx_IRQn MCAN1_INT0_IRQn
 #define CCFG_CANxDMABA MATRIX->CCFG_SYSIO
 #define CCFG_CANxDMABA_Msk CCFG_SYSIO_CAN1DMABA_Msk
 #define CCFG_CANxDMABA_Pos CCFG_SYSIO_CAN1DMABA_Pos
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

#define FDCAN_IE_TC        (MCAN_IE_TCE | MCAN_IE_TCFE | MCAN_IE_TFEE)

// Transmit a packet
int
canhw_send(struct canbus_msg *msg)
{
    uint32_t txfqs = CANx->MCAN_TXFQS;
    if (txfqs & MCAN_TXFQS_TFQF)
        // No space in transmit fifo - wait for irq
        return -1;

    uint32_t w_index = ((txfqs & MCAN_TXFQS_TFQPI_Msk) >> MCAN_TXFQS_TFQPI_Pos);
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
    CANx->MCAN_TXBAR;
    CANx->MCAN_TXBAR = ((uint32_t)1 << w_index);
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
    CANx->MCAN_CCCR |= MCAN_CCCR_INIT;
    /* Wait the acknowledge */
    while (!(CANx->MCAN_CCCR & MCAN_CCCR_INIT))
        ;
    /* Enable configuration change */
    CANx->MCAN_CCCR |= MCAN_CCCR_CCE;

    // Load filter
    can_filter(0, CANBUS_ID_ADMIN);
    can_filter(1, id);
    can_filter(2, id + 1);

    uint32_t flssa = (uint32_t)MSG_RAM.FLS
        - (CCFG_CANxDMABA_Msk & CCFG_CANxDMABA);
    CANx->MCAN_SIDFC = flssa | ((id ? 3 : 1) << MCAN_SIDFC_LSS_Pos);
    CANx->MCAN_GFC = 0x02 << MCAN_GFC_ANFS_Pos;

    /* Leave the initialisation mode for the filter */
    barrier();
    CANx->MCAN_CCCR &= ~MCAN_CCCR_CCE;
    CANx->MCAN_CCCR &= ~MCAN_CCCR_INIT;
}

// This function handles CAN global interrupts
void
CAN_IRQHandler(void)
{
    uint32_t ir = CANx->MCAN_IR;

    if (ir & MCAN_IE_RF0NE) {
        CANx->MCAN_IR = MCAN_IE_RF0NE;

        uint32_t rxf0s = CANx->MCAN_RXF0S;
        if (rxf0s & MCAN_RXF0S_F0FL_Msk) {
            // Read and ack data packet
            uint32_t idx = (rxf0s & MCAN_RXF0S_F0GI_Msk) >> MCAN_RXF0S_F0GI_Pos;
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
            CANx->MCAN_RXF0A = idx;

            // Process packet
            canbus_process_data(&msg);
        }
    }
    if (ir & FDCAN_IE_TC) {
        // Tx
        CANx->MCAN_IR = FDCAN_IE_TC;
        canbus_notify_tx();
    }
}

static inline const uint32_t
make_btr(uint32_t sjw,       // Sync jump width, ... hmm
         uint32_t time_seg1, // time segment before sample point, 1 .. 16
         uint32_t time_seg2, // time segment after sample point, 1 .. 8
         uint32_t brp)       // Baud rate prescaler, 1 .. 1024
{
    return (((uint32_t)(sjw-1)) << MCAN_NBTP_NSJW_Pos
            | ((uint32_t)(time_seg1-1)) << MCAN_NBTP_NTSEG1_Pos
            | ((uint32_t)(time_seg2-1)) << MCAN_NBTP_NTSEG2_Pos
            | ((uint32_t)(brp - 1)) << MCAN_NBTP_NBRP_Pos);
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
    if (!CONFIG_USBCANBUS) {
        // Configure UPLL for USB and CAN
        PMC->CKGR_UCKR = CKGR_UCKR_UPLLCOUNT(3) | CKGR_UCKR_UPLLEN;
        while (!(PMC->PMC_SR & PMC_SR_LOCKU))
            ;
    }

    // Configure PCK5 for CAN use
    PMC->PMC_PCK[5] = PMC_PCK_CSS_UPLL_CLK | PMC_PCK_PRES(5);
    while (!(PMC->PMC_SR & PMC_SR_PCKRDY5))
        ;
    PMC->PMC_SCER |= PMC_SCER_PCK5;

    enable_pclock(CANx_GCLK_ID);

    gpio_peripheral(GPIO_Rx, CAN_FUNCTION_Rx, 1);
    gpio_peripheral(GPIO_Tx, CAN_FUNCTION_Tx, 0);

    uint32_t pclock = get_pclock_frequency(CANx_GCLK_ID);

    uint32_t btr = compute_btr(pclock, CONFIG_CANBUS_FREQUENCY);

    /*##-1- Configure the CAN #######################################*/

    /* Exit from sleep mode */
    CANx->MCAN_CCCR &= ~MCAN_CCCR_CSR;
    /* Wait the acknowledge */
    while (CANx->MCAN_CCCR & MCAN_CCCR_CSA)
        ;
    /* Request initialization */
    CANx->MCAN_CCCR |= MCAN_CCCR_INIT;
    /* Wait the acknowledge */
    while (!(CANx->MCAN_CCCR & MCAN_CCCR_INIT))
        ;
    /* Enable configuration change */
    CANx->MCAN_CCCR |= MCAN_CCCR_CCE;

    /* Disable protocol exception handling */
    CANx->MCAN_CCCR |= MCAN_CCCR_PXHD;

    CANx->MCAN_NBTP = btr;

    /* Setup message RAM addresses */
    uint32_t ccfg = (CCFG_CANxDMABA & ~CCFG_CANxDMABA_Msk);
    CCFG_CANxDMABA = (ccfg | (((uint32_t)&MSG_RAM)
        & CCFG_CANxDMABA_Msk));
    uint32_t f0sa = (uint32_t)MSG_RAM.RXF0
        - (CCFG_CANxDMABA_Msk & CCFG_CANxDMABA);
    CANx->MCAN_RXF0C = f0sa
        | (ARRAY_SIZE(MSG_RAM.RXF0) << MCAN_RXF0C_F0S_Pos);
    CANx->MCAN_RXESC = (7 << MCAN_RXESC_F1DS_Pos)
        | (7 << MCAN_RXESC_F0DS_Pos);
    uint32_t tbsa = (uint32_t)MSG_RAM.TXFIFO
        - (CCFG_CANxDMABA_Msk & CCFG_CANxDMABA);
    CANx->MCAN_TXBC = tbsa
        | (ARRAY_SIZE(MSG_RAM.TXFIFO) << MCAN_TXBC_TFQS_Pos);
    CANx->MCAN_TXESC = 7 << MCAN_TXESC_TBDS_Pos;

    /* Leave the initialisation mode */
    CANx->MCAN_CCCR &= ~MCAN_CCCR_CCE;
    CANx->MCAN_CCCR &= ~MCAN_CCCR_INIT;

    /*##-2- Configure the CAN Filter #######################################*/
    canhw_set_filter(0);

    /*##-3- Configure Interrupts #################################*/
    armcm_enable_irq(CAN_IRQHandler, CANx_IRQn, 1);
    CANx->MCAN_ILE = MCAN_ILE_EINT0;
    CANx->MCAN_IE = MCAN_IE_RF0NE | FDCAN_IE_TC;
}
DECL_INIT(can_init);
