// Support for Linux "gs_usb" CANbus adapter emulation
//
// Copyright (C) 2018-2022  Kevin O'Connor <kevin@koconnor.net>
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
#include "generic/usbstd.h" // struct usb_device_descriptor
#include "sched.h" // sched_wake_task
#include "usb_cdc.h" // usb_notify_ep0


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
 * Message sending
 ****************************************************************/

// Global storage
static struct usbcan_data {
    struct task_wake wake;

    // Canbus data from host
    union {
        struct gs_host_frame host_frame;
        uint8_t rx_frame_pad[USB_CDC_EP_BULK_OUT_SIZE];
    };
    uint8_t host_status;

    // Canbus data routed locally
    uint8_t notify_local;
    uint32_t assigned_id;

    // Data from physical canbus interface
    uint32_t pull_pos, push_pos;
    struct canbus_msg queue[8];
} UsbCan;

enum {
    HS_TX_ECHO = 1,
    HS_TX_HW = 2,
    HS_TX_LOCAL = 4,
};

void
canbus_notify_tx(void)
{
    sched_wake_task(&UsbCan.wake);
}

// Handle incoming data from hw canbus interface (called from IRQ handler)
void
canbus_process_data(struct canbus_msg *msg)
{
    // Add to admin command queue
    uint32_t pushp = UsbCan.push_pos;
    if (pushp - UsbCan.pull_pos >= ARRAY_SIZE(UsbCan.queue))
        // No space - drop message
        return;
    if (UsbCan.assigned_id && (msg->id & ~1) == UsbCan.assigned_id)
        // Id reserved for local
        return;
    uint32_t pos = pushp % ARRAY_SIZE(UsbCan.queue);
    memcpy(&UsbCan.queue[pos], msg, sizeof(*msg));
    UsbCan.push_pos = pushp + 1;
    usb_notify_bulk_out();
}

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

// Send any pending hw frames to host
static int
drain_hw_queue(void)
{
    for (;;) {
        uint32_t push_pos = readl(&UsbCan.push_pos);
        uint32_t pull_pos = UsbCan.pull_pos;
        if (push_pos != pull_pos) {
            uint32_t pos = pull_pos % ARRAY_SIZE(UsbCan.queue);
            int ret = send_frame(&UsbCan.queue[pos]);
            if (ret < 0)
                return -1;
            UsbCan.pull_pos = pull_pos + 1;
            continue;
        }
        return 0;
    }
}

void
usbcan_task(void)
{
    if (!sched_check_wake(&UsbCan.wake))
        return;
    for (;;) {
        // Send any pending hw frames to host
        int ret = drain_hw_queue();
        if (ret < 0)
            return;

        // See if previous host frame needs to be transmitted
        uint_fast8_t host_status = UsbCan.host_status;
        if (host_status & (HS_TX_HW | HS_TX_LOCAL)) {
            struct gs_host_frame *gs = &UsbCan.host_frame;
            struct canbus_msg msg;
            msg.id = gs->can_id;
            msg.dlc = gs->can_dlc;
            msg.data32[0] = gs->data32[0];
            msg.data32[1] = gs->data32[1];
            if (host_status & HS_TX_HW) {
                ret = canbus_send(&msg);
                if (ret < 0)
                    return;
                UsbCan.host_status = host_status = host_status & ~HS_TX_HW;
            }
            if (host_status & HS_TX_LOCAL) {
                ret = canserial_process_data(&msg);
                if (ret < 0) {
                    usb_notify_bulk_out();
                    return;
                }
                UsbCan.host_status = host_status & ~HS_TX_LOCAL;
            }
            continue;
        }

        // Send any previous echo frames
        if (host_status) {
            ret = usb_send_bulk_in(&UsbCan.host_frame
                                   , sizeof(UsbCan.host_frame));
            if (ret < 0)
                return;
            UsbCan.host_status = 0;
            continue;
        }

        // See if can read a new frame from host
        ret = usb_read_bulk_out(&UsbCan.host_frame, USB_CDC_EP_BULK_OUT_SIZE);
        if (ret > 0) {
            uint32_t id = UsbCan.host_frame.can_id;
            UsbCan.host_status = HS_TX_ECHO | HS_TX_HW;
            if (id == CANBUS_ID_ADMIN)
                UsbCan.host_status = HS_TX_ECHO | HS_TX_HW | HS_TX_LOCAL;
            else if (UsbCan.assigned_id && UsbCan.assigned_id == id)
                UsbCan.host_status = HS_TX_ECHO | HS_TX_LOCAL;
            continue;
        }

        // No more work to be done
        if (UsbCan.notify_local) {
            UsbCan.notify_local = 0;
            canserial_notify_tx();
        }
        return;
    }
}
DECL_TASK(usbcan_task);

int
canserial_send(struct canbus_msg *msg)
{
    int ret = drain_hw_queue();
    if (ret < 0)
        goto retry_later;
    ret = send_frame(msg);
    if (ret < 0)
        goto retry_later;
    UsbCan.notify_local = 0;
    return msg->dlc;
retry_later:
    UsbCan.notify_local = 1;
    return -1;
}

void
canserial_set_filter(uint32_t id)
{
    UsbCan.assigned_id = id;
}

void
usb_notify_bulk_out(void)
{
    canbus_notify_tx();
}

void
usb_notify_bulk_in(void)
{
    canbus_notify_tx();
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
    usb_notify_bulk_in();
    usb_notify_bulk_out();
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
    usb_notify_bulk_in();
    usb_notify_bulk_out();
    usb_notify_ep0();
}
DECL_SHUTDOWN(usb_shutdown);
