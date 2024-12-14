// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_USB_H
#define _HARDWARE_STRUCTS_USB_H

/**
 * \file rp2350/usb.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/usb.h"
#include "hardware/structs/usb_dpram.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_usb
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/usb.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(USB_ADDR_ENDP_OFFSET) // USB_ADDR_ENDP
    // Device address and endpoint control
    // 0x000f0000 [19:16] ENDPOINT     (0x0) Device endpoint to send data to
    // 0x0000007f [6:0]   ADDRESS      (0x00) In device mode, the address that the device should respond to
    io_rw_32 dev_addr_ctrl;
 
    // (Description copied from array index 0 register USB_ADDR_ENDP1 applies similarly to other array indexes)
    _REG_(USB_ADDR_ENDP1_OFFSET) // USB_ADDR_ENDP1
    // Interrupt endpoint 1
    // 0x04000000 [26]    INTEP_PREAMBLE (0) Interrupt EP requires preamble (is a low speed device on...
    // 0x02000000 [25]    INTEP_DIR    (0) Direction of the interrupt endpoint
    // 0x000f0000 [19:16] ENDPOINT     (0x0) Endpoint number of the interrupt endpoint
    // 0x0000007f [6:0]   ADDRESS      (0x00) Device address
    io_rw_32 int_ep_addr_ctrl[15];
 
    _REG_(USB_MAIN_CTRL_OFFSET) // USB_MAIN_CTRL
    // Main control register
    // 0x80000000 [31]    SIM_TIMING   (0) Reduced timings for simulation
    // 0x00000004 [2]     PHY_ISO      (1) Isolates USB phy after controller power-up +
    // 0x00000002 [1]     HOST_NDEVICE (0) Device mode = 0, Host mode = 1
    // 0x00000001 [0]     CONTROLLER_EN (0) Enable controller
    io_rw_32 main_ctrl;
 
    _REG_(USB_SOF_WR_OFFSET) // USB_SOF_WR
    // Set the SOF (Start of Frame) frame number in the host controller
    // 0x000007ff [10:0]  COUNT        (0x000) 
    io_wo_32 sof_wr;
 
    _REG_(USB_SOF_RD_OFFSET) // USB_SOF_RD
    // Read the last SOF (Start of Frame) frame number seen
    // 0x000007ff [10:0]  COUNT        (0x000) 
    io_ro_32 sof_rd;
 
    _REG_(USB_SIE_CTRL_OFFSET) // USB_SIE_CTRL
    // SIE control register
    // 0x80000000 [31]    EP0_INT_STALL (0) Device: Set bit in EP_STATUS_STALL_NAK when EP0 sends a STALL
    // 0x40000000 [30]    EP0_DOUBLE_BUF (0) Device: EP0 single buffered = 0, double buffered = 1
    // 0x20000000 [29]    EP0_INT_1BUF (0) Device: Set bit in BUFF_STATUS for every buffer completed on EP0
    // 0x10000000 [28]    EP0_INT_2BUF (0) Device: Set bit in BUFF_STATUS for every 2 buffers...
    // 0x08000000 [27]    EP0_INT_NAK  (0) Device: Set bit in EP_STATUS_STALL_NAK when EP0 sends a NAK
    // 0x04000000 [26]    DIRECT_EN    (0) Direct bus drive enable
    // 0x02000000 [25]    DIRECT_DP    (0) Direct control of DP
    // 0x01000000 [24]    DIRECT_DM    (0) Direct control of DM
    // 0x00080000 [19]    EP0_STOP_ON_SHORT_PACKET (0) Device: Stop EP0 on a short packet
    // 0x00040000 [18]    TRANSCEIVER_PD (0) Power down bus transceiver
    // 0x00020000 [17]    RPU_OPT      (0) Device: Pull-up strength (0=1K2, 1=2k3)
    // 0x00010000 [16]    PULLUP_EN    (0) Device: Enable pull up resistor
    // 0x00008000 [15]    PULLDOWN_EN  (1) Host: Enable pull down resistors
    // 0x00002000 [13]    RESET_BUS    (0) Host: Reset bus
    // 0x00001000 [12]    RESUME       (0) Device: Remote wakeup
    // 0x00000800 [11]    VBUS_EN      (0) Host: Enable VBUS
    // 0x00000400 [10]    KEEP_ALIVE_EN (0) Host: Enable keep alive packet (for low speed bus)
    // 0x00000200 [9]     SOF_EN       (0) Host: Enable SOF generation (for full speed bus)
    // 0x00000100 [8]     SOF_SYNC     (0) Host: Delay packet(s) until after SOF
    // 0x00000040 [6]     PREAMBLE_EN  (0) Host: Preable enable for LS device on FS hub
    // 0x00000010 [4]     STOP_TRANS   (0) Host: Stop transaction
    // 0x00000008 [3]     RECEIVE_DATA (0) Host: Receive transaction (IN to host)
    // 0x00000004 [2]     SEND_DATA    (0) Host: Send transaction (OUT from host)
    // 0x00000002 [1]     SEND_SETUP   (0) Host: Send Setup packet
    // 0x00000001 [0]     START_TRANS  (0) Host: Start transaction
    io_rw_32 sie_ctrl;
 
    _REG_(USB_SIE_STATUS_OFFSET) // USB_SIE_STATUS
    // SIE status register
    // 0x80000000 [31]    DATA_SEQ_ERROR (0) Data Sequence Error
    // 0x40000000 [30]    ACK_REC      (0) ACK received
    // 0x20000000 [29]    STALL_REC    (0) Host: STALL received
    // 0x10000000 [28]    NAK_REC      (0) Host: NAK received
    // 0x08000000 [27]    RX_TIMEOUT   (0) RX timeout is raised by both the host and device if an...
    // 0x04000000 [26]    RX_OVERFLOW  (0) RX overflow is raised by the Serial RX engine if the...
    // 0x02000000 [25]    BIT_STUFF_ERROR (0) Bit Stuff Error
    // 0x01000000 [24]    CRC_ERROR    (0) CRC Error
    // 0x00800000 [23]    ENDPOINT_ERROR (0) An endpoint has encountered an error
    // 0x00080000 [19]    BUS_RESET    (0) Device: bus reset received
    // 0x00040000 [18]    TRANS_COMPLETE (0) Transaction complete
    // 0x00020000 [17]    SETUP_REC    (0) Device: Setup packet received
    // 0x00010000 [16]    CONNECTED    (0) Device: connected
    // 0x00001000 [12]    RX_SHORT_PACKET (0) Device or Host has received a short packet
    // 0x00000800 [11]    RESUME       (0) Host: Device has initiated a remote resume
    // 0x00000400 [10]    VBUS_OVER_CURR (0) VBUS over current detected
    // 0x00000300 [9:8]   SPEED        (0x0) Host: device speed
    // 0x00000010 [4]     SUSPENDED    (0) Bus in suspended state
    // 0x0000000c [3:2]   LINE_STATE   (0x0) USB bus line state
    // 0x00000001 [0]     VBUS_DETECTED (0) Device: VBUS Detected
    io_rw_32 sie_status;
 
    _REG_(USB_INT_EP_CTRL_OFFSET) // USB_INT_EP_CTRL
    // interrupt endpoint control register
    // 0x0000fffe [15:1]  INT_EP_ACTIVE (0x0000) Host: Enable interrupt endpoint 1 -> 15
    io_rw_32 int_ep_ctrl;
 
    _REG_(USB_BUFF_STATUS_OFFSET) // USB_BUFF_STATUS
    // Buffer status register
    // 0x80000000 [31]    EP15_OUT     (0) 
    // 0x40000000 [30]    EP15_IN      (0) 
    // 0x20000000 [29]    EP14_OUT     (0) 
    // 0x10000000 [28]    EP14_IN      (0) 
    // 0x08000000 [27]    EP13_OUT     (0) 
    // 0x04000000 [26]    EP13_IN      (0) 
    // 0x02000000 [25]    EP12_OUT     (0) 
    // 0x01000000 [24]    EP12_IN      (0) 
    // 0x00800000 [23]    EP11_OUT     (0) 
    // 0x00400000 [22]    EP11_IN      (0) 
    // 0x00200000 [21]    EP10_OUT     (0) 
    // 0x00100000 [20]    EP10_IN      (0) 
    // 0x00080000 [19]    EP9_OUT      (0) 
    // 0x00040000 [18]    EP9_IN       (0) 
    // 0x00020000 [17]    EP8_OUT      (0) 
    // 0x00010000 [16]    EP8_IN       (0) 
    // 0x00008000 [15]    EP7_OUT      (0) 
    // 0x00004000 [14]    EP7_IN       (0) 
    // 0x00002000 [13]    EP6_OUT      (0) 
    // 0x00001000 [12]    EP6_IN       (0) 
    // 0x00000800 [11]    EP5_OUT      (0) 
    // 0x00000400 [10]    EP5_IN       (0) 
    // 0x00000200 [9]     EP4_OUT      (0) 
    // 0x00000100 [8]     EP4_IN       (0) 
    // 0x00000080 [7]     EP3_OUT      (0) 
    // 0x00000040 [6]     EP3_IN       (0) 
    // 0x00000020 [5]     EP2_OUT      (0) 
    // 0x00000010 [4]     EP2_IN       (0) 
    // 0x00000008 [3]     EP1_OUT      (0) 
    // 0x00000004 [2]     EP1_IN       (0) 
    // 0x00000002 [1]     EP0_OUT      (0) 
    // 0x00000001 [0]     EP0_IN       (0) 
    io_rw_32 buf_status;
 
    _REG_(USB_BUFF_CPU_SHOULD_HANDLE_OFFSET) // USB_BUFF_CPU_SHOULD_HANDLE
    // Which of the double buffers should be handled
    // 0x80000000 [31]    EP15_OUT     (0) 
    // 0x40000000 [30]    EP15_IN      (0) 
    // 0x20000000 [29]    EP14_OUT     (0) 
    // 0x10000000 [28]    EP14_IN      (0) 
    // 0x08000000 [27]    EP13_OUT     (0) 
    // 0x04000000 [26]    EP13_IN      (0) 
    // 0x02000000 [25]    EP12_OUT     (0) 
    // 0x01000000 [24]    EP12_IN      (0) 
    // 0x00800000 [23]    EP11_OUT     (0) 
    // 0x00400000 [22]    EP11_IN      (0) 
    // 0x00200000 [21]    EP10_OUT     (0) 
    // 0x00100000 [20]    EP10_IN      (0) 
    // 0x00080000 [19]    EP9_OUT      (0) 
    // 0x00040000 [18]    EP9_IN       (0) 
    // 0x00020000 [17]    EP8_OUT      (0) 
    // 0x00010000 [16]    EP8_IN       (0) 
    // 0x00008000 [15]    EP7_OUT      (0) 
    // 0x00004000 [14]    EP7_IN       (0) 
    // 0x00002000 [13]    EP6_OUT      (0) 
    // 0x00001000 [12]    EP6_IN       (0) 
    // 0x00000800 [11]    EP5_OUT      (0) 
    // 0x00000400 [10]    EP5_IN       (0) 
    // 0x00000200 [9]     EP4_OUT      (0) 
    // 0x00000100 [8]     EP4_IN       (0) 
    // 0x00000080 [7]     EP3_OUT      (0) 
    // 0x00000040 [6]     EP3_IN       (0) 
    // 0x00000020 [5]     EP2_OUT      (0) 
    // 0x00000010 [4]     EP2_IN       (0) 
    // 0x00000008 [3]     EP1_OUT      (0) 
    // 0x00000004 [2]     EP1_IN       (0) 
    // 0x00000002 [1]     EP0_OUT      (0) 
    // 0x00000001 [0]     EP0_IN       (0) 
    io_ro_32 buf_cpu_should_handle;
 
    _REG_(USB_EP_ABORT_OFFSET) // USB_EP_ABORT
    // Device only: Can be set to ignore the buffer control register for this endpoint in case you...
    // 0x80000000 [31]    EP15_OUT     (0) 
    // 0x40000000 [30]    EP15_IN      (0) 
    // 0x20000000 [29]    EP14_OUT     (0) 
    // 0x10000000 [28]    EP14_IN      (0) 
    // 0x08000000 [27]    EP13_OUT     (0) 
    // 0x04000000 [26]    EP13_IN      (0) 
    // 0x02000000 [25]    EP12_OUT     (0) 
    // 0x01000000 [24]    EP12_IN      (0) 
    // 0x00800000 [23]    EP11_OUT     (0) 
    // 0x00400000 [22]    EP11_IN      (0) 
    // 0x00200000 [21]    EP10_OUT     (0) 
    // 0x00100000 [20]    EP10_IN      (0) 
    // 0x00080000 [19]    EP9_OUT      (0) 
    // 0x00040000 [18]    EP9_IN       (0) 
    // 0x00020000 [17]    EP8_OUT      (0) 
    // 0x00010000 [16]    EP8_IN       (0) 
    // 0x00008000 [15]    EP7_OUT      (0) 
    // 0x00004000 [14]    EP7_IN       (0) 
    // 0x00002000 [13]    EP6_OUT      (0) 
    // 0x00001000 [12]    EP6_IN       (0) 
    // 0x00000800 [11]    EP5_OUT      (0) 
    // 0x00000400 [10]    EP5_IN       (0) 
    // 0x00000200 [9]     EP4_OUT      (0) 
    // 0x00000100 [8]     EP4_IN       (0) 
    // 0x00000080 [7]     EP3_OUT      (0) 
    // 0x00000040 [6]     EP3_IN       (0) 
    // 0x00000020 [5]     EP2_OUT      (0) 
    // 0x00000010 [4]     EP2_IN       (0) 
    // 0x00000008 [3]     EP1_OUT      (0) 
    // 0x00000004 [2]     EP1_IN       (0) 
    // 0x00000002 [1]     EP0_OUT      (0) 
    // 0x00000001 [0]     EP0_IN       (0) 
    io_rw_32 abort;
 
    _REG_(USB_EP_ABORT_DONE_OFFSET) // USB_EP_ABORT_DONE
    // Device only: Used in conjunction with `EP_ABORT`
    // 0x80000000 [31]    EP15_OUT     (0) 
    // 0x40000000 [30]    EP15_IN      (0) 
    // 0x20000000 [29]    EP14_OUT     (0) 
    // 0x10000000 [28]    EP14_IN      (0) 
    // 0x08000000 [27]    EP13_OUT     (0) 
    // 0x04000000 [26]    EP13_IN      (0) 
    // 0x02000000 [25]    EP12_OUT     (0) 
    // 0x01000000 [24]    EP12_IN      (0) 
    // 0x00800000 [23]    EP11_OUT     (0) 
    // 0x00400000 [22]    EP11_IN      (0) 
    // 0x00200000 [21]    EP10_OUT     (0) 
    // 0x00100000 [20]    EP10_IN      (0) 
    // 0x00080000 [19]    EP9_OUT      (0) 
    // 0x00040000 [18]    EP9_IN       (0) 
    // 0x00020000 [17]    EP8_OUT      (0) 
    // 0x00010000 [16]    EP8_IN       (0) 
    // 0x00008000 [15]    EP7_OUT      (0) 
    // 0x00004000 [14]    EP7_IN       (0) 
    // 0x00002000 [13]    EP6_OUT      (0) 
    // 0x00001000 [12]    EP6_IN       (0) 
    // 0x00000800 [11]    EP5_OUT      (0) 
    // 0x00000400 [10]    EP5_IN       (0) 
    // 0x00000200 [9]     EP4_OUT      (0) 
    // 0x00000100 [8]     EP4_IN       (0) 
    // 0x00000080 [7]     EP3_OUT      (0) 
    // 0x00000040 [6]     EP3_IN       (0) 
    // 0x00000020 [5]     EP2_OUT      (0) 
    // 0x00000010 [4]     EP2_IN       (0) 
    // 0x00000008 [3]     EP1_OUT      (0) 
    // 0x00000004 [2]     EP1_IN       (0) 
    // 0x00000002 [1]     EP0_OUT      (0) 
    // 0x00000001 [0]     EP0_IN       (0) 
    io_rw_32 abort_done;
 
    _REG_(USB_EP_STALL_ARM_OFFSET) // USB_EP_STALL_ARM
    // Device: this bit must be set in conjunction with the `STALL` bit in the buffer control register...
    // 0x00000002 [1]     EP0_OUT      (0) 
    // 0x00000001 [0]     EP0_IN       (0) 
    io_rw_32 ep_stall_arm;
 
    _REG_(USB_NAK_POLL_OFFSET) // USB_NAK_POLL
    // Used by the host controller
    // 0xf0000000 [31:28] RETRY_COUNT_HI (0x0) Bits 9:6 of nak_retry count
    // 0x08000000 [27]    EPX_STOPPED_ON_NAK (0) EPX polling has stopped because a nak was received
    // 0x04000000 [26]    STOP_EPX_ON_NAK (0) Stop polling epx when a nak is received
    // 0x03ff0000 [25:16] DELAY_FS     (0x010) NAK polling interval for a full speed device
    // 0x0000fc00 [15:10] RETRY_COUNT_LO (0x00) Bits 5:0 of nak_retry_count
    // 0x000003ff [9:0]   DELAY_LS     (0x010) NAK polling interval for a low speed device
    io_rw_32 nak_poll;
 
    _REG_(USB_EP_STATUS_STALL_NAK_OFFSET) // USB_EP_STATUS_STALL_NAK
    // Device: bits are set when the `IRQ_ON_NAK` or `IRQ_ON_STALL` bits are set
    // 0x80000000 [31]    EP15_OUT     (0) 
    // 0x40000000 [30]    EP15_IN      (0) 
    // 0x20000000 [29]    EP14_OUT     (0) 
    // 0x10000000 [28]    EP14_IN      (0) 
    // 0x08000000 [27]    EP13_OUT     (0) 
    // 0x04000000 [26]    EP13_IN      (0) 
    // 0x02000000 [25]    EP12_OUT     (0) 
    // 0x01000000 [24]    EP12_IN      (0) 
    // 0x00800000 [23]    EP11_OUT     (0) 
    // 0x00400000 [22]    EP11_IN      (0) 
    // 0x00200000 [21]    EP10_OUT     (0) 
    // 0x00100000 [20]    EP10_IN      (0) 
    // 0x00080000 [19]    EP9_OUT      (0) 
    // 0x00040000 [18]    EP9_IN       (0) 
    // 0x00020000 [17]    EP8_OUT      (0) 
    // 0x00010000 [16]    EP8_IN       (0) 
    // 0x00008000 [15]    EP7_OUT      (0) 
    // 0x00004000 [14]    EP7_IN       (0) 
    // 0x00002000 [13]    EP6_OUT      (0) 
    // 0x00001000 [12]    EP6_IN       (0) 
    // 0x00000800 [11]    EP5_OUT      (0) 
    // 0x00000400 [10]    EP5_IN       (0) 
    // 0x00000200 [9]     EP4_OUT      (0) 
    // 0x00000100 [8]     EP4_IN       (0) 
    // 0x00000080 [7]     EP3_OUT      (0) 
    // 0x00000040 [6]     EP3_IN       (0) 
    // 0x00000020 [5]     EP2_OUT      (0) 
    // 0x00000010 [4]     EP2_IN       (0) 
    // 0x00000008 [3]     EP1_OUT      (0) 
    // 0x00000004 [2]     EP1_IN       (0) 
    // 0x00000002 [1]     EP0_OUT      (0) 
    // 0x00000001 [0]     EP0_IN       (0) 
    io_rw_32 ep_nak_stall_status;
 
    _REG_(USB_USB_MUXING_OFFSET) // USB_USB_MUXING
    // Where to connect the USB controller
    // 0x80000000 [31]    SWAP_DPDM    (0) Swap the USB PHY DP and DM pins and all related controls...
    // 0x00000010 [4]     USBPHY_AS_GPIO (0) Use the usb DP and DM pins as GPIO pins instead of...
    // 0x00000008 [3]     SOFTCON      (0) 
    // 0x00000004 [2]     TO_DIGITAL_PAD (0) 
    // 0x00000002 [1]     TO_EXTPHY    (0) 
    // 0x00000001 [0]     TO_PHY       (1) 
    io_rw_32 muxing;
 
    _REG_(USB_USB_PWR_OFFSET) // USB_USB_PWR
    // Overrides for the power signals in the event that the VBUS signals are not hooked up to GPIO
    // 0x00000020 [5]     OVERCURR_DETECT_EN (0) 
    // 0x00000010 [4]     OVERCURR_DETECT (0) 
    // 0x00000008 [3]     VBUS_DETECT_OVERRIDE_EN (0) 
    // 0x00000004 [2]     VBUS_DETECT  (0) 
    // 0x00000002 [1]     VBUS_EN_OVERRIDE_EN (0) 
    // 0x00000001 [0]     VBUS_EN      (0) 
    io_rw_32 pwr;
 
    _REG_(USB_USBPHY_DIRECT_OFFSET) // USB_USBPHY_DIRECT
    // This register allows for direct control of the USB phy
    // 0x02000000 [25]    RX_DM_OVERRIDE (0) Override rx_dm value into controller
    // 0x01000000 [24]    RX_DP_OVERRIDE (0) Override rx_dp value into controller
    // 0x00800000 [23]    RX_DD_OVERRIDE (0) Override rx_dd value into controller
    // 0x00400000 [22]    DM_OVV       (0) DM over voltage
    // 0x00200000 [21]    DP_OVV       (0) DP over voltage
    // 0x00100000 [20]    DM_OVCN      (0) DM overcurrent
    // 0x00080000 [19]    DP_OVCN      (0) DP overcurrent
    // 0x00040000 [18]    RX_DM        (0) DPM pin state
    // 0x00020000 [17]    RX_DP        (0) DPP pin state
    // 0x00010000 [16]    RX_DD        (0) Differential RX
    // 0x00008000 [15]    TX_DIFFMODE  (0) TX_DIFFMODE=0: Single ended mode +
    // 0x00004000 [14]    TX_FSSLEW    (0) TX_FSSLEW=0: Low speed slew rate +
    // 0x00002000 [13]    TX_PD        (0) TX power down override (if override enable is set)
    // 0x00001000 [12]    RX_PD        (0) RX power down override (if override enable is set)
    // 0x00000800 [11]    TX_DM        (0) Output data
    // 0x00000400 [10]    TX_DP        (0) Output data
    // 0x00000200 [9]     TX_DM_OE     (0) Output enable
    // 0x00000100 [8]     TX_DP_OE     (0) Output enable
    // 0x00000040 [6]     DM_PULLDN_EN (0) DM pull down enable
    // 0x00000020 [5]     DM_PULLUP_EN (0) DM pull up enable
    // 0x00000010 [4]     DM_PULLUP_HISEL (0) Enable the second DM pull up resistor
    // 0x00000004 [2]     DP_PULLDN_EN (0) DP pull down enable
    // 0x00000002 [1]     DP_PULLUP_EN (0) DP pull up enable
    // 0x00000001 [0]     DP_PULLUP_HISEL (0) Enable the second DP pull up resistor
    io_rw_32 phy_direct;
 
    _REG_(USB_USBPHY_DIRECT_OVERRIDE_OFFSET) // USB_USBPHY_DIRECT_OVERRIDE
    // Override enable for each control in usbphy_direct
    // 0x00040000 [18]    RX_DM_OVERRIDE_EN (0) 
    // 0x00020000 [17]    RX_DP_OVERRIDE_EN (0) 
    // 0x00010000 [16]    RX_DD_OVERRIDE_EN (0) 
    // 0x00008000 [15]    TX_DIFFMODE_OVERRIDE_EN (0) 
    // 0x00001000 [12]    DM_PULLUP_OVERRIDE_EN (0) 
    // 0x00000800 [11]    TX_FSSLEW_OVERRIDE_EN (0) 
    // 0x00000400 [10]    TX_PD_OVERRIDE_EN (0) 
    // 0x00000200 [9]     RX_PD_OVERRIDE_EN (0) 
    // 0x00000100 [8]     TX_DM_OVERRIDE_EN (0) 
    // 0x00000080 [7]     TX_DP_OVERRIDE_EN (0) 
    // 0x00000040 [6]     TX_DM_OE_OVERRIDE_EN (0) 
    // 0x00000020 [5]     TX_DP_OE_OVERRIDE_EN (0) 
    // 0x00000010 [4]     DM_PULLDN_EN_OVERRIDE_EN (0) 
    // 0x00000008 [3]     DP_PULLDN_EN_OVERRIDE_EN (0) 
    // 0x00000004 [2]     DP_PULLUP_EN_OVERRIDE_EN (0) 
    // 0x00000002 [1]     DM_PULLUP_HISEL_OVERRIDE_EN (0) 
    // 0x00000001 [0]     DP_PULLUP_HISEL_OVERRIDE_EN (0) 
    io_rw_32 phy_direct_override;
 
    _REG_(USB_USBPHY_TRIM_OFFSET) // USB_USBPHY_TRIM
    // Used to adjust trim values of USB phy pull down resistors
    // 0x00001f00 [12:8]  DM_PULLDN_TRIM (0x1f) Value to drive to USB PHY +
    // 0x0000001f [4:0]   DP_PULLDN_TRIM (0x1f) Value to drive to USB PHY +
    io_rw_32 phy_trim;
 
    _REG_(USB_LINESTATE_TUNING_OFFSET) // USB_LINESTATE_TUNING
    // Used for debug only
    // 0x00000f00 [11:8]  SPARE_FIX    (0x0) 
    // 0x00000080 [7]     DEV_LS_WAKE_FIX (1) Device - exit suspend on any non-idle signalling, not...
    // 0x00000040 [6]     DEV_RX_ERR_QUIESCE (1) Device - suppress repeated errors until the device FSM...
    // 0x00000020 [5]     SIE_RX_CHATTER_SE0_FIX (1) RX - when recovering from line chatter or bitstuff...
    // 0x00000010 [4]     SIE_RX_BITSTUFF_FIX (1) RX - when a bitstuff error is signalled by rx_dasm,...
    // 0x00000008 [3]     DEV_BUFF_CONTROL_DOUBLE_READ_FIX (1) Device - the controller FSM performs two reads of the...
    // 0x00000004 [2]     MULTI_HUB_FIX (0) Host - increase inter-packet and turnaround timeouts to...
    // 0x00000002 [1]     LINESTATE_DELAY (0) Device/Host - add an extra 1-bit debounce of linestate sampling
    // 0x00000001 [0]     RCV_DELAY    (0) Device - register the received data to account for hub...
    io_rw_32 linestate_tuning;
 
    _REG_(USB_INTR_OFFSET) // USB_INTR
    // Raw Interrupts
    // 0x00800000 [23]    EPX_STOPPED_ON_NAK (0) Source: NAK_POLL
    // 0x00400000 [22]    DEV_SM_WATCHDOG_FIRED (0) Source: DEV_SM_WATCHDOG
    // 0x00200000 [21]    ENDPOINT_ERROR (0) Source: SIE_STATUS
    // 0x00100000 [20]    RX_SHORT_PACKET (0) Source: SIE_STATUS
    // 0x00080000 [19]    EP_STALL_NAK (0) Raised when any bit in EP_STATUS_STALL_NAK is set
    // 0x00040000 [18]    ABORT_DONE   (0) Raised when any bit in ABORT_DONE is set
    // 0x00020000 [17]    DEV_SOF      (0) Set every time the device receives a SOF (Start of Frame) packet
    // 0x00010000 [16]    SETUP_REQ    (0) Device
    // 0x00008000 [15]    DEV_RESUME_FROM_HOST (0) Set when the device receives a resume from the host
    // 0x00004000 [14]    DEV_SUSPEND  (0) Set when the device suspend state changes
    // 0x00002000 [13]    DEV_CONN_DIS (0) Set when the device connection state changes
    // 0x00001000 [12]    BUS_RESET    (0) Source: SIE_STATUS
    // 0x00000800 [11]    VBUS_DETECT  (0) Source: SIE_STATUS
    // 0x00000400 [10]    STALL        (0) Source: SIE_STATUS
    // 0x00000200 [9]     ERROR_CRC    (0) Source: SIE_STATUS
    // 0x00000100 [8]     ERROR_BIT_STUFF (0) Source: SIE_STATUS
    // 0x00000080 [7]     ERROR_RX_OVERFLOW (0) Source: SIE_STATUS
    // 0x00000040 [6]     ERROR_RX_TIMEOUT (0) Source: SIE_STATUS
    // 0x00000020 [5]     ERROR_DATA_SEQ (0) Source: SIE_STATUS
    // 0x00000010 [4]     BUFF_STATUS  (0) Raised when any bit in BUFF_STATUS is set
    // 0x00000008 [3]     TRANS_COMPLETE (0) Raised every time SIE_STATUS
    // 0x00000004 [2]     HOST_SOF     (0) Host: raised every time the host sends a SOF (Start of Frame)
    // 0x00000002 [1]     HOST_RESUME  (0) Host: raised when a device wakes up the host
    // 0x00000001 [0]     HOST_CONN_DIS (0) Host: raised when a device is connected or disconnected (i
    io_ro_32 intr;
 
    _REG_(USB_INTE_OFFSET) // USB_INTE
    // Interrupt Enable
    // 0x00800000 [23]    EPX_STOPPED_ON_NAK (0) Source: NAK_POLL
    // 0x00400000 [22]    DEV_SM_WATCHDOG_FIRED (0) Source: DEV_SM_WATCHDOG
    // 0x00200000 [21]    ENDPOINT_ERROR (0) Source: SIE_STATUS
    // 0x00100000 [20]    RX_SHORT_PACKET (0) Source: SIE_STATUS
    // 0x00080000 [19]    EP_STALL_NAK (0) Raised when any bit in EP_STATUS_STALL_NAK is set
    // 0x00040000 [18]    ABORT_DONE   (0) Raised when any bit in ABORT_DONE is set
    // 0x00020000 [17]    DEV_SOF      (0) Set every time the device receives a SOF (Start of Frame) packet
    // 0x00010000 [16]    SETUP_REQ    (0) Device
    // 0x00008000 [15]    DEV_RESUME_FROM_HOST (0) Set when the device receives a resume from the host
    // 0x00004000 [14]    DEV_SUSPEND  (0) Set when the device suspend state changes
    // 0x00002000 [13]    DEV_CONN_DIS (0) Set when the device connection state changes
    // 0x00001000 [12]    BUS_RESET    (0) Source: SIE_STATUS
    // 0x00000800 [11]    VBUS_DETECT  (0) Source: SIE_STATUS
    // 0x00000400 [10]    STALL        (0) Source: SIE_STATUS
    // 0x00000200 [9]     ERROR_CRC    (0) Source: SIE_STATUS
    // 0x00000100 [8]     ERROR_BIT_STUFF (0) Source: SIE_STATUS
    // 0x00000080 [7]     ERROR_RX_OVERFLOW (0) Source: SIE_STATUS
    // 0x00000040 [6]     ERROR_RX_TIMEOUT (0) Source: SIE_STATUS
    // 0x00000020 [5]     ERROR_DATA_SEQ (0) Source: SIE_STATUS
    // 0x00000010 [4]     BUFF_STATUS  (0) Raised when any bit in BUFF_STATUS is set
    // 0x00000008 [3]     TRANS_COMPLETE (0) Raised every time SIE_STATUS
    // 0x00000004 [2]     HOST_SOF     (0) Host: raised every time the host sends a SOF (Start of Frame)
    // 0x00000002 [1]     HOST_RESUME  (0) Host: raised when a device wakes up the host
    // 0x00000001 [0]     HOST_CONN_DIS (0) Host: raised when a device is connected or disconnected (i
    io_rw_32 inte;
 
    _REG_(USB_INTF_OFFSET) // USB_INTF
    // Interrupt Force
    // 0x00800000 [23]    EPX_STOPPED_ON_NAK (0) Source: NAK_POLL
    // 0x00400000 [22]    DEV_SM_WATCHDOG_FIRED (0) Source: DEV_SM_WATCHDOG
    // 0x00200000 [21]    ENDPOINT_ERROR (0) Source: SIE_STATUS
    // 0x00100000 [20]    RX_SHORT_PACKET (0) Source: SIE_STATUS
    // 0x00080000 [19]    EP_STALL_NAK (0) Raised when any bit in EP_STATUS_STALL_NAK is set
    // 0x00040000 [18]    ABORT_DONE   (0) Raised when any bit in ABORT_DONE is set
    // 0x00020000 [17]    DEV_SOF      (0) Set every time the device receives a SOF (Start of Frame) packet
    // 0x00010000 [16]    SETUP_REQ    (0) Device
    // 0x00008000 [15]    DEV_RESUME_FROM_HOST (0) Set when the device receives a resume from the host
    // 0x00004000 [14]    DEV_SUSPEND  (0) Set when the device suspend state changes
    // 0x00002000 [13]    DEV_CONN_DIS (0) Set when the device connection state changes
    // 0x00001000 [12]    BUS_RESET    (0) Source: SIE_STATUS
    // 0x00000800 [11]    VBUS_DETECT  (0) Source: SIE_STATUS
    // 0x00000400 [10]    STALL        (0) Source: SIE_STATUS
    // 0x00000200 [9]     ERROR_CRC    (0) Source: SIE_STATUS
    // 0x00000100 [8]     ERROR_BIT_STUFF (0) Source: SIE_STATUS
    // 0x00000080 [7]     ERROR_RX_OVERFLOW (0) Source: SIE_STATUS
    // 0x00000040 [6]     ERROR_RX_TIMEOUT (0) Source: SIE_STATUS
    // 0x00000020 [5]     ERROR_DATA_SEQ (0) Source: SIE_STATUS
    // 0x00000010 [4]     BUFF_STATUS  (0) Raised when any bit in BUFF_STATUS is set
    // 0x00000008 [3]     TRANS_COMPLETE (0) Raised every time SIE_STATUS
    // 0x00000004 [2]     HOST_SOF     (0) Host: raised every time the host sends a SOF (Start of Frame)
    // 0x00000002 [1]     HOST_RESUME  (0) Host: raised when a device wakes up the host
    // 0x00000001 [0]     HOST_CONN_DIS (0) Host: raised when a device is connected or disconnected (i
    io_rw_32 intf;
 
    _REG_(USB_INTS_OFFSET) // USB_INTS
    // Interrupt status after masking & forcing
    // 0x00800000 [23]    EPX_STOPPED_ON_NAK (0) Source: NAK_POLL
    // 0x00400000 [22]    DEV_SM_WATCHDOG_FIRED (0) Source: DEV_SM_WATCHDOG
    // 0x00200000 [21]    ENDPOINT_ERROR (0) Source: SIE_STATUS
    // 0x00100000 [20]    RX_SHORT_PACKET (0) Source: SIE_STATUS
    // 0x00080000 [19]    EP_STALL_NAK (0) Raised when any bit in EP_STATUS_STALL_NAK is set
    // 0x00040000 [18]    ABORT_DONE   (0) Raised when any bit in ABORT_DONE is set
    // 0x00020000 [17]    DEV_SOF      (0) Set every time the device receives a SOF (Start of Frame) packet
    // 0x00010000 [16]    SETUP_REQ    (0) Device
    // 0x00008000 [15]    DEV_RESUME_FROM_HOST (0) Set when the device receives a resume from the host
    // 0x00004000 [14]    DEV_SUSPEND  (0) Set when the device suspend state changes
    // 0x00002000 [13]    DEV_CONN_DIS (0) Set when the device connection state changes
    // 0x00001000 [12]    BUS_RESET    (0) Source: SIE_STATUS
    // 0x00000800 [11]    VBUS_DETECT  (0) Source: SIE_STATUS
    // 0x00000400 [10]    STALL        (0) Source: SIE_STATUS
    // 0x00000200 [9]     ERROR_CRC    (0) Source: SIE_STATUS
    // 0x00000100 [8]     ERROR_BIT_STUFF (0) Source: SIE_STATUS
    // 0x00000080 [7]     ERROR_RX_OVERFLOW (0) Source: SIE_STATUS
    // 0x00000040 [6]     ERROR_RX_TIMEOUT (0) Source: SIE_STATUS
    // 0x00000020 [5]     ERROR_DATA_SEQ (0) Source: SIE_STATUS
    // 0x00000010 [4]     BUFF_STATUS  (0) Raised when any bit in BUFF_STATUS is set
    // 0x00000008 [3]     TRANS_COMPLETE (0) Raised every time SIE_STATUS
    // 0x00000004 [2]     HOST_SOF     (0) Host: raised every time the host sends a SOF (Start of Frame)
    // 0x00000002 [1]     HOST_RESUME  (0) Host: raised when a device wakes up the host
    // 0x00000001 [0]     HOST_CONN_DIS (0) Host: raised when a device is connected or disconnected (i
    io_ro_32 ints;
 
    uint32_t _pad0[25];
 
    _REG_(USB_SOF_TIMESTAMP_RAW_OFFSET) // USB_SOF_TIMESTAMP_RAW
    // Device only
    // 0x001fffff [20:0]  SOF_TIMESTAMP_RAW (0x000000) 
    io_ro_32 sof_timestamp_raw;
 
    _REG_(USB_SOF_TIMESTAMP_LAST_OFFSET) // USB_SOF_TIMESTAMP_LAST
    // Device only
    // 0x001fffff [20:0]  SOF_TIMESTAMP_LAST (0x000000) 
    io_ro_32 sof_timestamp_last;
 
    _REG_(USB_SM_STATE_OFFSET) // USB_SM_STATE
    // 0x00000f00 [11:8]  RX_DASM      (0x0) 
    // 0x000000e0 [7:5]   BC_STATE     (0x0) 
    // 0x0000001f [4:0]   STATE        (0x00) 
    io_ro_32 sm_state;
 
    _REG_(USB_EP_TX_ERROR_OFFSET) // USB_EP_TX_ERROR
    // TX error count for each endpoint
    // 0xc0000000 [31:30] EP15         (0x0) 
    // 0x30000000 [29:28] EP14         (0x0) 
    // 0x0c000000 [27:26] EP13         (0x0) 
    // 0x03000000 [25:24] EP12         (0x0) 
    // 0x00c00000 [23:22] EP11         (0x0) 
    // 0x00300000 [21:20] EP10         (0x0) 
    // 0x000c0000 [19:18] EP9          (0x0) 
    // 0x00030000 [17:16] EP8          (0x0) 
    // 0x0000c000 [15:14] EP7          (0x0) 
    // 0x00003000 [13:12] EP6          (0x0) 
    // 0x00000c00 [11:10] EP5          (0x0) 
    // 0x00000300 [9:8]   EP4          (0x0) 
    // 0x000000c0 [7:6]   EP3          (0x0) 
    // 0x00000030 [5:4]   EP2          (0x0) 
    // 0x0000000c [3:2]   EP1          (0x0) 
    // 0x00000003 [1:0]   EP0          (0x0) 
    io_rw_32 ep_tx_error;
 
    _REG_(USB_EP_RX_ERROR_OFFSET) // USB_EP_RX_ERROR
    // RX error count for each endpoint
    // 0x80000000 [31]    EP15_SEQ     (0) 
    // 0x40000000 [30]    EP15_TRANSACTION (0) 
    // 0x20000000 [29]    EP14_SEQ     (0) 
    // 0x10000000 [28]    EP14_TRANSACTION (0) 
    // 0x08000000 [27]    EP13_SEQ     (0) 
    // 0x04000000 [26]    EP13_TRANSACTION (0) 
    // 0x02000000 [25]    EP12_SEQ     (0) 
    // 0x01000000 [24]    EP12_TRANSACTION (0) 
    // 0x00800000 [23]    EP11_SEQ     (0) 
    // 0x00400000 [22]    EP11_TRANSACTION (0) 
    // 0x00200000 [21]    EP10_SEQ     (0) 
    // 0x00100000 [20]    EP10_TRANSACTION (0) 
    // 0x00080000 [19]    EP9_SEQ      (0) 
    // 0x00040000 [18]    EP9_TRANSACTION (0) 
    // 0x00020000 [17]    EP8_SEQ      (0) 
    // 0x00010000 [16]    EP8_TRANSACTION (0) 
    // 0x00008000 [15]    EP7_SEQ      (0) 
    // 0x00004000 [14]    EP7_TRANSACTION (0) 
    // 0x00002000 [13]    EP6_SEQ      (0) 
    // 0x00001000 [12]    EP6_TRANSACTION (0) 
    // 0x00000800 [11]    EP5_SEQ      (0) 
    // 0x00000400 [10]    EP5_TRANSACTION (0) 
    // 0x00000200 [9]     EP4_SEQ      (0) 
    // 0x00000100 [8]     EP4_TRANSACTION (0) 
    // 0x00000080 [7]     EP3_SEQ      (0) 
    // 0x00000040 [6]     EP3_TRANSACTION (0) 
    // 0x00000020 [5]     EP2_SEQ      (0) 
    // 0x00000010 [4]     EP2_TRANSACTION (0) 
    // 0x00000008 [3]     EP1_SEQ      (0) 
    // 0x00000004 [2]     EP1_TRANSACTION (0) 
    // 0x00000002 [1]     EP0_SEQ      (0) 
    // 0x00000001 [0]     EP0_TRANSACTION (0) 
    io_rw_32 ep_rx_error;
 
    _REG_(USB_DEV_SM_WATCHDOG_OFFSET) // USB_DEV_SM_WATCHDOG
    // Watchdog that forces the device state machine to idle and raises an interrupt if the device...
    // 0x00100000 [20]    FIRED        (0) 
    // 0x00080000 [19]    RESET        (0) Set to 1 to forcibly reset the device state machine on...
    // 0x00040000 [18]    ENABLE       (0) 
    // 0x0003ffff [17:0]  LIMIT        (0x00000) 
    io_rw_32 dev_sm_watchdog;
} usb_hw_t;

#define usb_hw ((usb_hw_t *)USBCTRL_REGS_BASE)
static_assert(sizeof (usb_hw_t) == 0x0118, "");

#endif // _HARDWARE_STRUCTS_USB_H

