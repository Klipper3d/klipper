// Support for Linux "gs_usb" CANbus adapter emulation
//
// Copyright (C) 2018-2025  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memmove
#include "autoconf.h" // CONFIG_USB_VENDOR_ID
#include "board/canbus.h" // canbus_notify_tx
#include "board/canserial.h" // canserial_notify_tx
#include "board/io.h" // readl
#include "board/misc.h" // console_sendf
#include "board/pgm.h" // PROGMEM
#include "board/usb_cdc_ep.h" // USB_CDC_EP_BULK_IN
#include "byteorder.h" // cpu_to_le16
#include "command.h" // DECL_CONSTANT
#include "generic/usbstd.h" // struct usb_device_descriptor
#include "sched.h" // sched_wake_task
#include "usb_cdc.h" // usb_notify_ep0

DECL_CONSTANT("CANBUS_BRIDGE", 1);


/****************************************************************
 * Linux "gs_usb" definitions
 ****************************************************************/

#define USB_GSUSB_1_VENDOR_ID      0x1d50
#define USB_GSUSB_1_PRODUCT_ID     0x606f

enum gs_usb_breq {
    GS_USB_BREQ_HOST_FORMAT = 0,
    GS_USB_BREQ_BITTIMING,
    GS_USB_BREQ_MODE,
    GS_USB_BREQ_BERR,
    GS_USB_BREQ_BT_CONST,
    GS_USB_BREQ_DEVICE_CONFIG,
    GS_USB_BREQ_TIMESTAMP,
    GS_USB_BREQ_IDENTIFY,
    GS_USB_BREQ_GET_USER_ID,
    GS_USB_BREQ_SET_USER_ID,
    GS_USB_BREQ_DATA_BITTIMING,
    GS_USB_BREQ_BT_CONST_EXT,
};

struct gs_host_config {
    uint32_t byte_order;
} __packed;

struct gs_device_config {
    uint8_t reserved1;
    uint8_t reserved2;
    uint8_t reserved3;
    uint8_t icount;
    uint32_t sw_version;
    uint32_t hw_version;
} __packed;

struct gs_device_bt_const {
    uint32_t feature;
    uint32_t fclk_can;
    uint32_t tseg1_min;
    uint32_t tseg1_max;
    uint32_t tseg2_min;
    uint32_t tseg2_max;
    uint32_t sjw_max;
    uint32_t brp_min;
    uint32_t brp_max;
    uint32_t brp_inc;
} __packed;

struct gs_device_bittiming {
    uint32_t prop_seg;
    uint32_t phase_seg1;
    uint32_t phase_seg2;
    uint32_t sjw;
    uint32_t brp;
} __packed;

struct gs_device_mode {
    uint32_t mode;
    uint32_t flags;
} __packed;

struct gs_host_frame {
    uint32_t echo_id;
    uint32_t can_id;

    uint8_t can_dlc;
    uint8_t channel;
    uint8_t flags;
    uint8_t reserved;

    union {
        uint8_t data[8];
        uint32_t data32[2];
    };
} __packed;


/****************************************************************
 * Main usbcan task (read requests from usb and send msgs to usb)
 ****************************************************************/

// Global storage
static struct usbcan_data {
    struct task_wake wake;

    // Canbus data routed locally
    uint8_t notify_local, usb_send_busy;
    uint32_t assigned_id;

    // State tracking for messages to be sent from host to canbus
    uint32_t bus_send_discard_time;
    uint8_t bus_send_state;

    // Canbus data from host
    uint8_t host_status;
    uint32_t host_pull_pos, host_push_pos;
    struct gs_host_frame host_frames[16];

    // Data from physical canbus interface
    uint32_t canhw_pull_pos, canhw_push_pos;
    struct canbus_msg canhw_queue[32];
} UsbCan;

enum {
    BSS_READY = 0, BSS_BLOCKING, BSS_DISCARDING
};

enum {
    HS_TX_ECHO = 1,
    HS_TX_HW = 2,
    HS_TX_LOCAL = 4,
};

// Send a message to the Linux host
static int
send_frame(struct canbus_msg *msg)
{
    struct gs_host_frame gs = {};
    gs.echo_id = 0xffffffff;
    gs.can_id = msg->id;
    gs.can_dlc = msg->dlc;
    gs.data32[0] = msg->data32[0];
    gs.data32[1] = msg->data32[1];
    return usb_send_bulk_in(&gs, sizeof(gs));
}

// Send any pending messages read from canbus hw to host
static void
drain_canhw_queue(void)
{
    uint32_t pull_pos = UsbCan.canhw_pull_pos;
    for (;;) {
        uint32_t push_pos = readl(&UsbCan.canhw_push_pos);
        if (push_pos == pull_pos) {
            // No more data to send
            UsbCan.usb_send_busy = 0;
            return;
        }
        uint32_t pos = pull_pos % ARRAY_SIZE(UsbCan.canhw_queue);
        int ret = send_frame(&UsbCan.canhw_queue[pos]);
        if (ret < 0) {
            // USB is busy - retry later
            UsbCan.usb_send_busy = 1;
            return;
        }
        UsbCan.canhw_pull_pos = pull_pos = pull_pos + 1;
    }
}

// Fill local queue with any USB messages read from host
static void
drain_usb_host_messages(void)
{
    uint32_t pull_pos = UsbCan.host_pull_pos, push_pos = UsbCan.host_push_pos;
    for (;;) {
        if (push_pos - pull_pos >= ARRAY_SIZE(UsbCan.host_frames))
            // No more space in queue
            break;
        uint32_t pushp = push_pos % ARRAY_SIZE(UsbCan.host_frames);
        struct gs_host_frame *gs = &UsbCan.host_frames[pushp];
        int ret = usb_read_bulk_out(gs, sizeof(*gs));
        if (ret <= 0)
            // No more messages ready
            break;
        UsbCan.host_push_pos = push_pos = push_pos + 1;
    }
}

// Report bus stall state
static void
note_discard_state(uint32_t discard)
{
    sendf("usb_canbus_state discard=%u", discard);
}

// Check if canbus queue has gotten stuck
static int
check_need_discard(void)
{
    if (UsbCan.bus_send_state != BSS_BLOCKING)
        return 0;
    return timer_is_before(UsbCan.bus_send_discard_time, timer_read_time());
}

// Attempt to send a message on the canbus
static int
try_canmsg_send(struct canbus_msg *msg)
{
    int ret = canhw_send(msg);
    if (ret >= 0) {
        // Success
        if (UsbCan.bus_send_state == BSS_DISCARDING)
            note_discard_state(0);
        UsbCan.bus_send_state = BSS_READY;
        return ret;
    }

    // Unable to send message
    if (check_need_discard()) {
        // The canbus is stalled - start discarding messages
        note_discard_state(1);
        UsbCan.bus_send_state = BSS_DISCARDING;
    }
    if (UsbCan.bus_send_state == BSS_DISCARDING)
        // Queue is stalled - just discard the message
        return 0;
    if (UsbCan.bus_send_state == BSS_READY) {
        // Just starting to block - setup stall detection after 50ms
        UsbCan.bus_send_state = BSS_BLOCKING;
        UsbCan.bus_send_discard_time = timer_read_time() + timer_from_us(50000);
    }
    return ret;
}

// Process new requests arriving from the host
static void
drain_host_queue(void)
{
    uint32_t pull_pos = UsbCan.host_pull_pos, push_pos = UsbCan.host_push_pos;
    for (;;) {
        uint32_t pullp = pull_pos % ARRAY_SIZE(UsbCan.host_frames);
        struct gs_host_frame *gs = &UsbCan.host_frames[pullp];
        uint_fast8_t host_status = UsbCan.host_status;

        // Extract next frame from host
        if (! host_status) {
            if (pull_pos == push_pos)
                // No frame available - no more work to be done
                break;
            uint32_t id = gs->can_id;
            host_status = HS_TX_ECHO | HS_TX_HW;
            if (id == CANBUS_ID_ADMIN)
                host_status = HS_TX_ECHO | HS_TX_HW | HS_TX_LOCAL;
            else if (UsbCan.assigned_id && UsbCan.assigned_id == id)
                host_status = HS_TX_ECHO | HS_TX_LOCAL;
            UsbCan.host_status = host_status;
        }

        // Send echo frames back to host
        if (host_status & HS_TX_ECHO) {
            if (UsbCan.notify_local || UsbCan.usb_send_busy)
                // Don't send echo frame until other traffic is sent
                break;
            int ret = usb_send_bulk_in(gs, sizeof(*gs));
            if (ret < 0)
                break;
            UsbCan.host_status = host_status = host_status & ~HS_TX_ECHO;
        }

        // See if host frame needs to be transmitted
        struct canbus_msg msg;
        msg.id = gs->can_id;
        msg.dlc = gs->can_dlc;
        msg.data32[0] = gs->data32[0];
        msg.data32[1] = gs->data32[1];
        if (host_status & HS_TX_LOCAL) {
            canserial_process_data(&msg);
            UsbCan.host_status = host_status = host_status & ~HS_TX_LOCAL;
        }
        if (host_status & HS_TX_HW) {
            int ret = try_canmsg_send(&msg);
            if (ret < 0)
                break;
            UsbCan.host_status = host_status = host_status & ~HS_TX_HW;
        }

        // Note message fully processed
        UsbCan.host_pull_pos = pull_pos = pull_pos + 1;
    }
}

// Main message routing task
void
usbcan_task(void)
{
    if (!sched_check_wake(&UsbCan.wake) && !check_need_discard())
        return;

    // Send messages read from canbus hardware to host
    drain_canhw_queue();

    // Fill local queue with any USB messages arriving from host
    drain_usb_host_messages();

    // Route messages received from host
    drain_host_queue();

    // Wake up local message response handling (if usb is not busy)
    if (UsbCan.notify_local && !UsbCan.usb_send_busy)
        canserial_notify_tx();
}
DECL_TASK(usbcan_task);

// Helper function to wake usbcan_task()
static void
wake_usbcan_task(void)
{
    sched_wake_task(&UsbCan.wake);
}


/****************************************************************
 * Interface to canbus hardware (read canbus hw msgs and tx notifications)
 ****************************************************************/

void
canbus_notify_tx(void)
{
    wake_usbcan_task();
}

// Handle incoming data from hw canbus interface (called from IRQ handler)
void
canbus_process_data(struct canbus_msg *msg)
{
    // Add to admin command queue
    uint32_t pushp = UsbCan.canhw_push_pos;
    if (pushp - UsbCan.canhw_pull_pos >= ARRAY_SIZE(UsbCan.canhw_queue))
        // No space - drop message
        return;
    if (UsbCan.assigned_id && (msg->id & ~1) == UsbCan.assigned_id)
        // Id reserved for local
        return;
    uint32_t pos = pushp % ARRAY_SIZE(UsbCan.canhw_queue);
    memcpy(&UsbCan.canhw_queue[pos], msg, sizeof(*msg));
    UsbCan.canhw_push_pos = pushp + 1;
    wake_usbcan_task();
}


/****************************************************************
 * Handle messages routed locally (canserial.c interface)
 ****************************************************************/

int
canbus_send(struct canbus_msg *msg)
{
    if (UsbCan.usb_send_busy)
        goto retry_later;
    int ret = send_frame(msg);
    if (ret < 0)
        goto retry_later;
    if (UsbCan.host_status)
        wake_usbcan_task();
    UsbCan.notify_local = 0;
    return msg->dlc;
retry_later:
    UsbCan.notify_local = 1;
    return -1;
}

void
canbus_set_filter(uint32_t id)
{
    UsbCan.assigned_id = id;
}


/****************************************************************
 * USB bulk wakeup interface
 ****************************************************************/

void
usb_notify_bulk_out(void)
{
    wake_usbcan_task();
}

void
usb_notify_bulk_in(void)
{
    wake_usbcan_task();
}


/****************************************************************
 * USB descriptors
 ****************************************************************/

#define CONCAT1(a, b) a ## b
#define CONCAT(a, b) CONCAT1(a, b)
#define USB_STR_MANUFACTURER u"Klipper"
#define USB_STR_PRODUCT CONCAT(u,CONFIG_MCU)
#define USB_STR_SERIAL CONCAT(u,CONFIG_USB_SERIAL_NUMBER)

// String descriptors
enum {
    USB_STR_ID_MANUFACTURER = 1, USB_STR_ID_PRODUCT, USB_STR_ID_SERIAL,
};

#define SIZE_cdc_string_langids (sizeof(cdc_string_langids) + 2)

static const struct usb_string_descriptor cdc_string_langids PROGMEM = {
    .bLength = SIZE_cdc_string_langids,
    .bDescriptorType = USB_DT_STRING,
    .data = { cpu_to_le16(USB_LANGID_ENGLISH_US) },
};

#define SIZE_cdc_string_manufacturer \
    (sizeof(cdc_string_manufacturer) + sizeof(USB_STR_MANUFACTURER) - 2)

static const struct usb_string_descriptor cdc_string_manufacturer PROGMEM = {
    .bLength = SIZE_cdc_string_manufacturer,
    .bDescriptorType = USB_DT_STRING,
    .data = USB_STR_MANUFACTURER,
};

#define SIZE_cdc_string_product \
    (sizeof(cdc_string_product) + sizeof(USB_STR_PRODUCT) - 2)

static const struct usb_string_descriptor cdc_string_product PROGMEM = {
    .bLength = SIZE_cdc_string_product,
    .bDescriptorType = USB_DT_STRING,
    .data = USB_STR_PRODUCT,
};

#define SIZE_cdc_string_serial \
    (sizeof(cdc_string_serial) + sizeof(USB_STR_SERIAL) - 2)

static const struct usb_string_descriptor cdc_string_serial PROGMEM = {
    .bLength = SIZE_cdc_string_serial,
    .bDescriptorType = USB_DT_STRING,
    .data = USB_STR_SERIAL,
};

// Device descriptor
static const struct usb_device_descriptor gs_device_descriptor PROGMEM = {
    .bLength = sizeof(gs_device_descriptor),
    .bDescriptorType = USB_DT_DEVICE,
    .bcdUSB = cpu_to_le16(0x0200),
    .bMaxPacketSize0 = USB_CDC_EP0_SIZE,
    .idVendor = cpu_to_le16(USB_GSUSB_1_VENDOR_ID),
    .idProduct = cpu_to_le16(USB_GSUSB_1_PRODUCT_ID),
    .iManufacturer = USB_STR_ID_MANUFACTURER,
    .iProduct = USB_STR_ID_PRODUCT,
    .iSerialNumber = USB_STR_ID_SERIAL,
    .bNumConfigurations = 1,
};

// Config descriptor
static const struct config_s {
    struct usb_config_descriptor config;
    struct usb_interface_descriptor iface0;
    struct usb_endpoint_descriptor ep1;
    struct usb_endpoint_descriptor ep2;
} PACKED gs_config_descriptor PROGMEM = {
    .config = {
        .bLength = sizeof(gs_config_descriptor.config),
        .bDescriptorType = USB_DT_CONFIG,
        .wTotalLength = cpu_to_le16(sizeof(gs_config_descriptor)),
        .bNumInterfaces = 1,
        .bConfigurationValue = 1,
        .bmAttributes = 0xC0,
        .bMaxPower = 50,
    },
    .iface0 = {
        .bLength = sizeof(gs_config_descriptor.iface0),
        .bDescriptorType = USB_DT_INTERFACE,
        .bInterfaceNumber = 0,
        .bNumEndpoints = 2,
        .bInterfaceClass = 255,
        .bInterfaceSubClass = 255,
        .bInterfaceProtocol = 255,
    },
    .ep1 = {
        .bLength = sizeof(gs_config_descriptor.ep1),
        .bDescriptorType = USB_DT_ENDPOINT,
        .bEndpointAddress = USB_CDC_EP_BULK_OUT,
        .bmAttributes = USB_ENDPOINT_XFER_BULK,
        .wMaxPacketSize = cpu_to_le16(USB_CDC_EP_BULK_OUT_SIZE),
    },
    .ep2 = {
        .bLength = sizeof(gs_config_descriptor.ep2),
        .bDescriptorType = USB_DT_ENDPOINT,
        .bEndpointAddress = USB_CDC_EP_BULK_IN | USB_DIR_IN,
        .bmAttributes = USB_ENDPOINT_XFER_BULK,
        .wMaxPacketSize = cpu_to_le16(USB_CDC_EP_BULK_IN_SIZE),
    },
};

// List of available descriptors
static const struct descriptor_s {
    uint_fast16_t wValue;
    uint_fast16_t wIndex;
    const void *desc;
    uint_fast8_t size;
} usb_descriptors[] PROGMEM = {
    { USB_DT_DEVICE<<8, 0x0000,
      &gs_device_descriptor, sizeof(gs_device_descriptor) },
    { USB_DT_CONFIG<<8, 0x0000,
      &gs_config_descriptor, sizeof(gs_config_descriptor) },
    { USB_DT_STRING<<8, 0x0000,
      &cdc_string_langids, SIZE_cdc_string_langids },
    { (USB_DT_STRING<<8) | USB_STR_ID_MANUFACTURER, USB_LANGID_ENGLISH_US,
      &cdc_string_manufacturer, SIZE_cdc_string_manufacturer },
    { (USB_DT_STRING<<8) | USB_STR_ID_PRODUCT, USB_LANGID_ENGLISH_US,
      &cdc_string_product, SIZE_cdc_string_product },
#if !CONFIG_USB_SERIAL_NUMBER_CHIPID
    { (USB_DT_STRING<<8) | USB_STR_ID_SERIAL, USB_LANGID_ENGLISH_US,
      &cdc_string_serial, SIZE_cdc_string_serial },
#endif
};

// Fill in a USB serial string descriptor from a chip id
void
usb_fill_serial(struct usb_string_descriptor *desc, int strlen, void *id)
{
    desc->bLength = sizeof(*desc) + strlen * sizeof(desc->data[0]);
    desc->bDescriptorType = USB_DT_STRING;

    uint8_t *src = id;
    int i;
    for (i = 0; i < strlen; i++) {
        uint8_t c = i & 1 ? src[i/2] & 0x0f : src[i/2] >> 4;
        desc->data[i] = c < 10 ? c + '0' : c - 10 + 'A';
    }
}


/****************************************************************
 * USB endpoint 0 control message handling
 ****************************************************************/

// State tracking
enum {
    UX_READ = 1<<0, UX_SEND = 1<<1, UX_SEND_PROGMEM = 1<<2, UX_SEND_ZLP = 1<<3
};

static void *usb_xfer_data;
static uint8_t usb_xfer_size, usb_xfer_flags;

// Set the USB "stall" condition
static void
usb_do_stall(void)
{
    usb_stall_ep0();
    usb_xfer_flags = 0;
}

// Transfer data on the usb endpoint 0
static void
usb_do_xfer(void *data, uint_fast8_t size, uint_fast8_t flags)
{
    for (;;) {
        uint_fast8_t xs = size;
        if (xs > USB_CDC_EP0_SIZE)
            xs = USB_CDC_EP0_SIZE;
        int_fast8_t ret;
        if (flags & UX_READ)
            ret = usb_read_ep0(data, xs);
        else if (NEED_PROGMEM && flags & UX_SEND_PROGMEM)
            ret = usb_send_ep0_progmem(data, xs);
        else
            ret = usb_send_ep0(data, xs);
        if (ret == xs) {
            // Success
            data += xs;
            size -= xs;
            if (!size) {
                // Entire transfer completed successfully
                if (flags & UX_READ) {
                    // Send status packet at end of read
                    flags = UX_SEND;
                    continue;
                }
                if (xs == USB_CDC_EP0_SIZE && flags & UX_SEND_ZLP)
                    // Must send zero-length-packet
                    continue;
                usb_xfer_flags = 0;
                usb_notify_ep0();
                return;
            }
            continue;
        }
        if (ret == -1) {
            // Interface busy - retry later
            usb_xfer_data = data;
            usb_xfer_size = size;
            usb_xfer_flags = flags;
            return;
        }
        // Error
        usb_do_stall();
        return;
    }
}

static void
usb_req_get_descriptor(struct usb_ctrlrequest *req)
{
    if (req->bRequestType != USB_DIR_IN)
        goto fail;
    void *desc = NULL;
    uint_fast8_t flags, size, i;
    for (i=0; i<ARRAY_SIZE(usb_descriptors); i++) {
        const struct descriptor_s *d = &usb_descriptors[i];
        if (READP(d->wValue) == req->wValue
            && READP(d->wIndex) == req->wIndex) {
            flags = NEED_PROGMEM ? UX_SEND_PROGMEM : UX_SEND;
            size = READP(d->size);
            desc = (void*)READP(d->desc);
        }
    }
    if (CONFIG_USB_SERIAL_NUMBER_CHIPID
        && req->wValue == ((USB_DT_STRING<<8) | USB_STR_ID_SERIAL)
        && req->wIndex == USB_LANGID_ENGLISH_US) {
            struct usb_string_descriptor *usbserial_serialid;
            usbserial_serialid = usbserial_get_serialid();
            flags = UX_SEND;
            size = usbserial_serialid->bLength;
            desc = (void*)usbserial_serialid;
    }
    if (desc) {
        if (size > req->wLength)
            size = req->wLength;
        else if (size < req->wLength)
            flags |= UX_SEND_ZLP;
        usb_do_xfer(desc, size, flags);
        return;
    }
fail:
    usb_do_stall();
}

static void
usb_req_set_address(struct usb_ctrlrequest *req)
{
    if (req->bRequestType || req->wIndex || req->wLength) {
        usb_do_stall();
        return;
    }
    usb_set_address(req->wValue);
}

static void
usb_req_set_configuration(struct usb_ctrlrequest *req)
{
    if (req->bRequestType || req->wValue != 1 || req->wIndex || req->wLength) {
        usb_do_stall();
        return;
    }
    usb_set_configure();
    wake_usbcan_task();
    usb_do_xfer(NULL, 0, UX_SEND);
}

struct gs_host_config host_config;

static void
gs_breq_host_format(struct usb_ctrlrequest *req)
{
    // Like candlightfw, little-endian is always used.  Read and ignore value.
    usb_do_xfer(&host_config, sizeof(host_config), UX_READ);
}

static const struct gs_device_config device_config PROGMEM = {
    .sw_version = 2,
    .hw_version = 1,
};

static void
gs_breq_device_config(struct usb_ctrlrequest *req)
{
    usb_do_xfer((void*)&device_config, sizeof(device_config), UX_SEND);
}

static const struct gs_device_bt_const bt_const PROGMEM = {
    // These are just dummy values for now
    .feature = 0,
    .fclk_can = 48000000,
    .tseg1_min = 1,
    .tseg1_max = 16,
    .tseg2_min = 1,
    .tseg2_max = 8,
    .sjw_max = 4,
    .brp_min = 1,
    .brp_max = 1024,
    .brp_inc = 1,
};

static void
gs_breq_bt_const(struct usb_ctrlrequest *req)
{
    usb_do_xfer((void*)&bt_const, sizeof(bt_const), UX_SEND);
}

struct gs_device_bittiming device_bittiming;

static void
gs_breq_bittiming(struct usb_ctrlrequest *req)
{
    // Bit timing is ignored for now
    usb_do_xfer(&device_bittiming, sizeof(device_bittiming), UX_READ);
}

struct gs_device_mode device_mode;

static void
gs_breq_mode(struct usb_ctrlrequest *req)
{
    // Mode is ignored for now
    usb_do_xfer(&device_mode, sizeof(device_mode), UX_READ);
}

static void
usb_state_ready(void)
{
    struct usb_ctrlrequest req;
    int_fast8_t ret = usb_read_ep0_setup(&req, sizeof(req));
    if (ret != sizeof(req))
        return;
    uint32_t req_type = req.bRequestType & USB_TYPE_MASK;
    if (req_type == USB_TYPE_STANDARD) {
        switch (req.bRequest) {
        case USB_REQ_GET_DESCRIPTOR: usb_req_get_descriptor(&req); break;
        case USB_REQ_SET_ADDRESS: usb_req_set_address(&req); break;
        case USB_REQ_SET_CONFIGURATION: usb_req_set_configuration(&req); break;
        default: usb_do_stall(); break;
        }
    } else if (req_type == USB_TYPE_VENDOR) {
        switch (req.bRequest) {
        case GS_USB_BREQ_HOST_FORMAT: gs_breq_host_format(&req); break;
        case GS_USB_BREQ_DEVICE_CONFIG: gs_breq_device_config(&req); break;
        case GS_USB_BREQ_BT_CONST: gs_breq_bt_const(&req); break;
        case GS_USB_BREQ_BITTIMING: gs_breq_bittiming(&req); break;
        case GS_USB_BREQ_MODE: gs_breq_mode(&req); break;
        default: usb_do_stall(); break;
        }
    } else {
        usb_do_stall();
    }
}

// State tracking dispatch
static struct task_wake usb_ep0_wake;

void
usb_notify_ep0(void)
{
    sched_wake_task(&usb_ep0_wake);
}

void
usb_ep0_task(void)
{
    if (!sched_check_wake(&usb_ep0_wake))
        return;
    if (usb_xfer_flags)
        usb_do_xfer(usb_xfer_data, usb_xfer_size, usb_xfer_flags);
    else
        usb_state_ready();
}
DECL_TASK(usb_ep0_task);

void
usb_shutdown(void)
{
    wake_usbcan_task();
    usb_notify_ep0();
}
DECL_SHUTDOWN(usb_shutdown);
