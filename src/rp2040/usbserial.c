// Hardware interface to USB on rp2040
//
// Copyright (C) 2021-2024  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "board/armcm_boot.h" // armcm_enable_irq
#include "board/io.h" // writeb
#include "board/misc.h" // timer_read_time
#include "board/usb_cdc.h" // usb_notify_ep0
#include "board/usb_cdc_ep.h" // USB_CDC_EP_BULK_IN
#include "board/usbstd.h" // USB_ENDPOINT_XFER_INT
#include "hardware/regs/sysinfo.h" // SYSINFO_CHIP_ID_OFFSET
#include "hardware/structs/iobank0.h" // iobank0_hw
#include "hardware/structs/padsbank0.h" // padsbank0_hw
#include "hardware/structs/resets.h" // RESETS_RESET_USBCTRL_BITS
#include "hardware/structs/usb.h" // usb_hw
#include "internal.h" // enable_pclock
#include "sched.h" // DECL_INIT


/****************************************************************
 * USB transfer memory
 ****************************************************************/

#define DPBUF_SIZE 64

// Get the offset of a given endpoint's base buffer
static uint32_t
usb_buf_offset(uint32_t ep)
{
    return 0x100 + ep * DPBUF_SIZE * 2;
}

// Obtain a pointer to an endpoint buffer
static void*
usb_buf_addr(uint32_t ep, int bufnum)
{
    return (void*)usb_dpram + usb_buf_offset(ep) + bufnum * DPBUF_SIZE;
}

// Return a pointer to the ep_buf_ctrl register for an endpoint
static volatile uint16_t *
lookup_epbufctrl(uint32_t ep, int is_rx, int bufnum)
{
    volatile uint16_t *epbp;
    if (is_rx)
        epbp = (void*)&usb_dpram->ep_buf_ctrl[ep].out;
    else
        epbp = (void*)&usb_dpram->ep_buf_ctrl[ep].in;
    return &epbp[bufnum];
}

// Determine the next transfer PID id from the last PID
static uint32_t
next_data_pid(uint32_t epb)
{
    return (epb ^ USB_BUF_CTRL_DATA1_PID) & USB_BUF_CTRL_DATA1_PID;
}

// Extract the number of bytes in an rx buffer
static uint32_t
get_rx_count(uint32_t epb, uint32_t max_len)
{
    uint32_t c = epb & USB_BUF_CTRL_LEN_MASK;
    if (c > max_len)
        c = max_len;
    return c;
}

// Memcpy using 8-bit read/write (system memcpy may make unaligned accesses)
static void
dpram_memcpy(void *dest, const void *src, size_t n)
{
    const uint8_t *s = src;
    volatile uint8_t *d = dest;
    while (n) {
        *d++ = *s++;
        n -= sizeof(*d);
    }
}

// Memset using only 8-bit writes (system memcpy may make unaligned accesses)
static void
dpram_memset(void *s, int c, size_t n)
{
    volatile uint8_t *p = s;
    while (n) {
        *p++ = c;
        n -= sizeof(*p);
    }
}


/****************************************************************
 * Interface
 ****************************************************************/

static uint32_t bulk_out_push_count;

int_fast8_t
usb_read_bulk_out(void *data, uint_fast8_t max_len)
{
    // Check if there is a packet ready
    uint32_t bopc = bulk_out_push_count, bufnum = bopc & 1;
    uint32_t ep = USB_CDC_EP_BULK_OUT;
    volatile uint16_t *epbp = lookup_epbufctrl(ep, 1, bufnum);
    uint32_t epb = *epbp;
    if ((epb & (USB_BUF_CTRL_AVAIL|USB_BUF_CTRL_FULL)) != USB_BUF_CTRL_FULL)
        return -1;
    // Determine the next packet header
    bulk_out_push_count = bopc + 1;
    uint32_t pid = bufnum ? USB_BUF_CTRL_DATA1_PID : 0;
    uint32_t new_epb = USB_BUF_CTRL_LAST | pid | DPBUF_SIZE;
    *epbp = new_epb;
    barrier();
    // Copy the packet to the given buffer
    uint32_t c = get_rx_count(epb, max_len);
    dpram_memcpy(data, usb_buf_addr(ep, bufnum), c);
    // Notify the USB hardware that the space is now available
    barrier();
    *epbp = new_epb | USB_BUF_CTRL_AVAIL;
    return c;
}

static uint32_t bulk_in_pop_count;

int_fast8_t
usb_send_bulk_in(void *data, uint_fast8_t len)
{
    // Check if there is room for this packet
    uint32_t bipc = bulk_in_pop_count, bufnum = bipc & 1;
    uint32_t ep = USB_CDC_EP_BULK_IN;
    volatile uint16_t *epbp = lookup_epbufctrl(ep, 0, bufnum);
    uint32_t epb = *epbp;
    if (epb & (USB_BUF_CTRL_AVAIL|USB_BUF_CTRL_FULL))
        return -1;
    // Determine the next packet header
    bulk_in_pop_count = bipc + 1;
    uint32_t pid = bufnum ? USB_BUF_CTRL_DATA1_PID : 0;
    uint32_t new_epb = USB_BUF_CTRL_FULL | USB_BUF_CTRL_LAST | pid | len;
    *epbp = new_epb;
    barrier();
    // Copy the packet to the hw buffer
    dpram_memcpy(usb_buf_addr(ep, bufnum), data, len);
    // Inform the USB hardware of the available packet
    barrier();
    *epbp = new_epb | USB_BUF_CTRL_AVAIL;
    return len;
}

int_fast8_t
usb_read_ep0_setup(void *data, uint_fast8_t max_len)
{
    if (!(usb_hw->intr & USB_INTR_SETUP_REQ_BITS)) {
        usb_hw->inte = (USB_INTE_BUFF_STATUS_BITS | USB_INTE_SETUP_REQ_BITS
                        | USB_INTE_BUS_RESET_BITS);
        return -1;
    }
    usb_dpram->ep_buf_ctrl[0].in = 0;
    usb_dpram->ep_buf_ctrl[0].out = (USB_BUF_CTRL_DATA1_PID | USB_BUF_CTRL_LAST
                                     | USB_BUF_CTRL_AVAIL | DPBUF_SIZE);
    usb_hw->sie_status = USB_SIE_STATUS_SETUP_REC_BITS;
    barrier();
    dpram_memcpy(data, (void*)usb_dpram->setup_packet, max_len);
    barrier();
    if (usb_hw->intr & USB_INTR_SETUP_REQ_BITS) {
        // Raced with next setup packet
        usb_notify_ep0();
        return -1;
    }
    return max_len;
}

int_fast8_t
usb_read_ep0(void *data, uint_fast8_t max_len)
{
    // Check if there is a packet ready
    uint32_t ep = 0;
    if (usb_hw->intr & USB_INTR_SETUP_REQ_BITS)
        // Early end of transmission
        return -2;
    volatile uint16_t *epbp = lookup_epbufctrl(ep, 1, 0);
    uint32_t epb = *epbp;
    if ((epb & (USB_BUF_CTRL_AVAIL|USB_BUF_CTRL_FULL)) != USB_BUF_CTRL_FULL)
        return -1;
    // Determine the next packet header
    uint32_t new_epb = USB_BUF_CTRL_LAST | next_data_pid(epb) | DPBUF_SIZE;
    *epbp = new_epb;
    barrier();
    // Copy the packet to the given buffer
    uint32_t c = get_rx_count(epb, max_len);
    dpram_memcpy(data, usb_buf_addr(ep, 0), c);
    // Notify the USB hardware that the space is now available
    barrier();
    *epbp = new_epb | USB_BUF_CTRL_AVAIL;
    return c;
}

int_fast8_t
usb_send_ep0(const void *data, uint_fast8_t len)
{
    // Check if there is room for this packet
    uint32_t ep = 0;
    if (usb_hw->intr & USB_INTR_SETUP_REQ_BITS || usb_hw->buf_status & 2)
        // Early end of transmission
        return -2;
    volatile uint16_t *epbp = lookup_epbufctrl(ep, 0, 0);
    uint32_t epb = *epbp;
    if (epb & (USB_BUF_CTRL_AVAIL|USB_BUF_CTRL_FULL))
        return -1;
    // Determine the next packet header
    uint32_t pid = next_data_pid(epb);
    uint32_t new_epb = USB_BUF_CTRL_FULL | USB_BUF_CTRL_LAST | pid | len;
    *epbp = new_epb;
    barrier();
    // Copy the packet to the hw buffer
    dpram_memcpy(usb_buf_addr(ep, 0), data, len);
    // Inform the USB hardware of the available packet
    barrier();
    *epbp = new_epb | USB_BUF_CTRL_AVAIL;
    return len;
}

void
usb_stall_ep0(void)
{
    usb_dpram->ep_buf_ctrl[0].in = 0;
    usb_dpram->ep_buf_ctrl[0].out = 0;
    usb_hw->ep_stall_arm = USB_EP_STALL_ARM_BITS;
    usb_dpram->ep_buf_ctrl[0].in = USB_BUF_CTRL_STALL;
    usb_dpram->ep_buf_ctrl[0].out = USB_BUF_CTRL_STALL;
    usb_notify_ep0();
}

static uint8_t set_address;

void
usb_set_address(uint_fast8_t addr)
{
    writeb(&set_address, addr);
    usb_send_ep0(NULL, 0);
}

void
usb_set_configure(void)
{
    bulk_in_pop_count = 0;
    usb_dpram->ep_buf_ctrl[USB_CDC_EP_BULK_IN].in = 0;

    bulk_out_push_count = 0;
    uint32_t epb0 = USB_BUF_CTRL_AVAIL | USB_BUF_CTRL_LAST | DPBUF_SIZE;
    uint32_t epb1 = epb0 | USB_BUF_CTRL_DATA1_PID;
    usb_dpram->ep_buf_ctrl[USB_CDC_EP_BULK_OUT].out = epb0 | (epb1 << 16);
}


/****************************************************************
 * USB Errata workaround
 ****************************************************************/

// The rp2040 USB has an errata causing it to sometimes not connect
// after a reset.  The following code has extracts from the PICO SDK.

static uint8_t need_errata;
static struct task_wake usb_errata_wake;

// Workaround for rp2040-e5 errata
void
usb_errata_task(void)
{
    if (!sched_check_wake(&usb_errata_wake))
        return;

    if (usb_hw->sie_status & USB_SIE_STATUS_CONNECTED_BITS)
        // Already connected - workaround not needed
        return;

    // Wait for not in SE0 state
    if (!(usb_hw->sie_status & USB_SIE_STATUS_LINE_STATE_BITS)) {
        sched_wake_task(&usb_errata_wake);
        return;
    }

    // Backup GPIO15 pad state
    uint32_t dp = 15;
    uint32_t gpio_ctrl_prev = iobank0_hw->io[dp].ctrl;
    uint32_t pad_ctrl_prev = padsbank0_hw->io[dp];

    // Enable bus keep
    hw_write_masked(&padsbank0_hw->io[dp],
                    PADS_BANK0_GPIO15_PUE_BITS | PADS_BANK0_GPIO15_PDE_BITS,
                    PADS_BANK0_GPIO15_PUE_BITS | PADS_BANK0_GPIO15_PDE_BITS);
    // Disable pad output
    hw_write_masked(&iobank0_hw->io[dp].ctrl,
                    0x2 << IO_BANK0_GPIO15_CTRL_OEOVER_LSB,
                    IO_BANK0_GPIO15_CTRL_OEOVER_BITS);
    // Enable USB debug muxing function
    hw_write_masked(&iobank0_hw->io[dp].ctrl,
                    8 << IO_BANK0_GPIO15_CTRL_FUNCSEL_LSB,
                    IO_BANK0_GPIO15_CTRL_FUNCSEL_BITS);
    // Set input override
    hw_write_masked(&iobank0_hw->io[dp].ctrl,
                    0x3 << IO_BANK0_GPIO15_CTRL_INOVER_LSB,
                    IO_BANK0_GPIO15_CTRL_INOVER_BITS);
    // PHY pullups need to stay on
    hw_set_alias(usb_hw)->phy_direct = USB_USBPHY_DIRECT_DP_PULLUP_EN_BITS;
    hw_set_alias(usb_hw)->phy_direct_override =
        USB_USBPHY_DIRECT_OVERRIDE_DP_PULLUP_EN_OVERRIDE_EN_BITS;
    // Switch from USB PHY to GPIO PHY, now with J forced
    usb_hw->muxing = (USB_USB_MUXING_TO_DIGITAL_PAD_BITS
                      | USB_USB_MUXING_SOFTCON_BITS);

    // Wait 1ms
    uint32_t endtime = timer_read_time() + timer_from_us(1000);
    while (timer_is_before(timer_read_time(), endtime))
        ;

    // Verify in connected state
    endtime += timer_from_us(1000);
    for (;;) {
        if (usb_hw->sie_status & USB_SIE_STATUS_CONNECTED_BITS)
            break;
        if (timer_is_before(endtime, timer_read_time()))
            // Something went wrong - restore state and continue anyway
            break;
    }

    // Switch back to USB phy
    usb_hw->muxing = USB_USB_MUXING_TO_PHY_BITS | USB_USB_MUXING_SOFTCON_BITS;
    // Unset PHY pullup overrides
    hw_clear_alias(usb_hw)->phy_direct_override =
        USB_USBPHY_DIRECT_OVERRIDE_DP_PULLUP_EN_OVERRIDE_EN_BITS;

    // Restore GPIO control states
    iobank0_hw->io[dp].ctrl = gpio_ctrl_prev;
    padsbank0_hw->io[dp] = pad_ctrl_prev;
}
DECL_TASK(usb_errata_task);


/****************************************************************
 * Setup and interrupts
 ****************************************************************/

void
USB_Handler(void)
{
    uint32_t ints = usb_hw->ints;
    if (ints & USB_INTS_SETUP_REQ_BITS) {
        usb_hw->inte = USB_INTE_BUFF_STATUS_BITS | USB_INTE_BUS_RESET_BITS;
        usb_notify_ep0();
    }
    if (ints & USB_INTS_BUFF_STATUS_BITS) {
        uint32_t buf_status = usb_hw->buf_status;
        usb_hw->buf_status = buf_status;
        if (buf_status & (1 << (USB_CDC_EP_BULK_OUT*2 + 1)))
            usb_notify_bulk_out();
        if (buf_status & (1 << (USB_CDC_EP_BULK_IN*2)))
            usb_notify_bulk_in();
        if (buf_status & 3) {
            usb_notify_ep0();
            if (buf_status & 1 && set_address) {
                usb_hw->dev_addr_ctrl = set_address;
                set_address = 0;
            }
        }
    }
    if (ints & USB_INTS_BUS_RESET_BITS) {
        usb_hw->dev_addr_ctrl = 0;
        usb_hw->sie_status = USB_SIE_STATUS_BUS_RESET_BITS;
        if (need_errata)
            sched_wake_task(&usb_errata_wake);
    }
}

static void
endpoint_setup(void)
{
    // ACM
    uint32_t ep_acm = (EP_CTRL_ENABLE_BITS | usb_buf_offset(USB_CDC_EP_ACM)
                       | (USB_ENDPOINT_XFER_INT << EP_CTRL_BUFFER_TYPE_LSB));
    usb_dpram->ep_ctrl[USB_CDC_EP_ACM-1].in = ep_acm;
    // BULK
    uint32_t ep_out = (EP_CTRL_ENABLE_BITS | usb_buf_offset(USB_CDC_EP_BULK_OUT)
                       | EP_CTRL_DOUBLE_BUFFERED_BITS
                       | EP_CTRL_INTERRUPT_PER_BUFFER
                       | (USB_ENDPOINT_XFER_BULK << EP_CTRL_BUFFER_TYPE_LSB));
    usb_dpram->ep_ctrl[USB_CDC_EP_BULK_OUT-1].out = ep_out;
    uint32_t ep_in = (EP_CTRL_ENABLE_BITS | usb_buf_offset(USB_CDC_EP_BULK_IN)
                      | EP_CTRL_DOUBLE_BUFFERED_BITS
                      | EP_CTRL_INTERRUPT_PER_BUFFER
                      | (USB_ENDPOINT_XFER_BULK << EP_CTRL_BUFFER_TYPE_LSB));
    usb_dpram->ep_ctrl[USB_CDC_EP_BULK_IN-1].in = ep_in;
    usb_dpram->ep_buf_ctrl[USB_CDC_EP_BULK_IN].in = USB_BUF_CTRL_FULL;
}

void
usbserial_init(void)
{
    // Configure usb clock
    enable_pclock(RESETS_RESET_USBCTRL_BITS);

    // Setup shared memory area
    dpram_memset(usb_dpram, 0, sizeof(*usb_dpram));
    endpoint_setup();

    // Enable USB in device mode
    usb_hw->muxing = USB_USB_MUXING_TO_PHY_BITS | USB_USB_MUXING_SOFTCON_BITS;
    usb_hw->pwr = (USB_USB_PWR_VBUS_DETECT_BITS
                   | USB_USB_PWR_VBUS_DETECT_OVERRIDE_EN_BITS);
    usb_hw->main_ctrl = USB_MAIN_CTRL_CONTROLLER_EN_BITS;

    // Check if usb errata workaround needed
    enable_pclock(RESETS_RESET_SYSINFO_BITS);
    uint32_t chip_id = *((io_ro_32*)(SYSINFO_BASE + SYSINFO_CHIP_ID_OFFSET));
    uint32_t version = ((chip_id & SYSINFO_CHIP_ID_REVISION_BITS)
                        >> SYSINFO_CHIP_ID_REVISION_LSB);
    need_errata = (version == 1);

    // Enable irqs
    usb_hw->sie_ctrl = USB_SIE_CTRL_EP0_INT_1BUF_BITS;
    usb_hw->inte = (USB_INTE_BUFF_STATUS_BITS | USB_INTE_SETUP_REQ_BITS
                    | USB_INTE_BUS_RESET_BITS);
    armcm_enable_irq(USB_Handler, USBCTRL_IRQ_IRQn, 1);

    // Enable USB pullup
    usb_hw->sie_ctrl = (USB_SIE_CTRL_EP0_INT_1BUF_BITS
                        | USB_SIE_CTRL_PULLUP_EN_BITS);
}
DECL_INIT(usbserial_init);
