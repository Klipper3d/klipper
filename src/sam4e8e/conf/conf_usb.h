/*
 * conf_usb.h
 *
 *  Created on: 17 Mar 2016
 *      Author: David
 */

#ifndef CONF_USB_H_
#define CONF_USB_H_

#include "compiler.h"

/**
 * USB Device Configuration
 * @{
 */

//! Device definition (mandatory)

#if SAM3XA
# ifdef __ALLIGATOR__
#  define  USB_DEVICE_VENDOR_ID			0x1209			// pid.codes VID from which PIDs are allocated to open source projects
#  define  USB_DEVICE_PRODUCT_ID		0xAB3D			// PID allocated for Alligator board
#  define  USB_DEVICE_POWER				200				// Consumption on Vbus line (mA)
# else
#  define  USB_DEVICE_VENDOR_ID			0x1209			// pid.codes VID from which PIDs are allocated to open source projects
#  define  USB_DEVICE_PRODUCT_ID		0xD3D8			// PID allocated to us for older Duets
#  define  USB_DEVICE_POWER				200				// Consumption on Vbus line (mA)
# endif
#endif

#if SAM4E
#define  USB_DEVICE_VENDOR_ID			0x1D50			// openmoku.org VID from which PIDs are allocated to open source projects
#define  USB_DEVICE_PRODUCT_ID			0x60EC			// PID allocated to us for Duet NG series
#define  USB_DEVICE_POWER				200				// Consumption on Vbus line (mA)
#endif

#if SAM4S
#define  USB_DEVICE_VENDOR_ID			0x1D50			// openmoku.org VID from which PIDs are allocated to open source projects
#define  USB_DEVICE_PRODUCT_ID			0x60ED
#define  USB_DEVICE_POWER				200				// Consumption on Vbus line (mA)
#endif

#if SAME70
#define  USB_DEVICE_VENDOR_ID			0x1D50			// pid.codes VID from which PIDs are allocated to open source projects
#define  USB_DEVICE_PRODUCT_ID			0x60EE
#define  USB_DEVICE_POWER				200				// Consumption on Vbus line (mA)
#endif

#define  USB_DEVICE_MAJOR_VERSION         1
#define  USB_DEVICE_MINOR_VERSION         0
#define  USB_DEVICE_ATTR                  (USB_CONFIG_ATTR_SELF_POWERED)

//! USB Device string definitions (Optional)
#if defined(__ALLIGATOR__)
# define  USB_DEVICE_MANUFACTURE_NAME      "3DArtists"
# define  USB_DEVICE_PRODUCT_NAME          "Alligator"
#else
# define  USB_DEVICE_MANUFACTURE_NAME      "Duet3D"
# define  USB_DEVICE_PRODUCT_NAME          "Duet"
#endif

/**
 * USB Device Callbacks definitions (Optional)
 * @{
 */
// #define  UDC_VBUS_EVENT(b_vbus_high)      user_callback_vbus_action(b_vbus_high)
// extern void user_callback_vbus_action(bool b_vbus_high);
// #define  UDC_SOF_EVENT()                  user_callback_sof_action()
// extern void user_callback_sof_action(void);
// #define  UDC_SUSPEND_EVENT()              user_callback_suspend_action()
// extern void user_callback_suspend_action(void);
// #define  UDC_RESUME_EVENT()               user_callback_resume_action()
// extern void user_callback_resume_action(void);
// Mandatory when USB_DEVICE_ATTR authorizes remote wakeup feature
// #define  UDC_REMOTEWAKEUP_ENABLE()        user_callback_remotewakeup_enable()
// extern void user_callback_remotewakeup_enable(void);
// #define  UDC_REMOTEWAKEUP_DISABLE()       user_callback_remotewakeup_disable()
// extern void user_callback_remotewakeup_disable(void);
// When a extra string descriptor must be supported
// other than manufacturer, product and serial string
// #define  UDC_GET_EXTRA_STRING()
//@}

//@}
/**
 * Configuration of CDC interface
 * @{
 */

//! Define two USB communication ports
#define  UDI_CDC_PORT_NB 1

//! Interface callback definition
#define  UDI_CDC_ENABLE_EXT(port)         core_cdc_enable(port)
#define  UDI_CDC_DISABLE_EXT(port)        core_cdc_disable(port)
#define  UDI_CDC_RX_NOTIFY(port)          core_cdc_rx_notify(port)
#define  UDI_CDC_TX_EMPTY_NOTIFY(port)    core_cdc_tx_empty_notify(port)
#define  UDI_CDC_SET_CODING_EXT(port,cfg) core_cdc_set_coding_ext(port,cfg)
#define  UDI_CDC_SET_DTR_EXT(port,set)    core_cdc_set_dtr(port,set)
#define  UDI_CDC_SET_RTS_EXT(port,set)    core_cdc_set_rts(port,set)

//! Define it when the transfer CDC Device to Host is a low rate (<512000 bauds)
//! to reduce CDC buffers size
// #define  UDI_CDC_LOW_RATE

//! Default configuration of communication port
#define  UDI_CDC_DEFAULT_RATE             250000
#define  UDI_CDC_DEFAULT_STOPBITS         CDC_STOP_BITS_1
#define  UDI_CDC_DEFAULT_PARITY           CDC_PAR_NONE
#define  UDI_CDC_DEFAULT_DATABITS         8
//@}
//@}


/**
 * USB Device Driver Configuration
 * @{
 */
//@}

//! The includes of classes and other headers must be done at the end of this file to avoid compile error
#include "udi_cdc_conf.h"

// Callback functions, all called from the USB ISR. See file USBSerial for the definitions.

#ifdef __cplusplus
extern "C" {
#endif

// This is called when we are plugged in and connect to a host
bool core_cdc_enable(uint8_t port);

// This is called when we get disconnected from the host
void core_cdc_disable(uint8_t port);

// This is called when data has been received
void core_cdc_rx_notify(uint8_t port);

// This is called when the transmit buffer has been emptied
void core_cdc_tx_empty_notify(uint8_t port);

// This is called when the host ask to raise or lower DTR
inline void core_cdc_set_dtr(uint8_t port, bool b_enable) {}

// This is called when the host ask to raise or lower RTS
inline void core_cdc_set_rts(uint8_t port, bool b_enable) {}

// This is called when the hot asks to change the port speed, data bits etc.
inline void core_cdc_set_coding_ext(uint8_t port, usb_cdc_line_coding_t *cfg) {}

#ifdef __cplusplus
}
#endif

#endif /* CONF_USB_H_ */