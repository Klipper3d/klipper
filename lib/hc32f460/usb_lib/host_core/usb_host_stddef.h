/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_host_stddef.h
 **
 ** A detailed description is available at
 ** @link Definitions used in the USB host library @endlink
 **
 **   - 2018-12-26  CDT First version for USB demo.
 **
 ******************************************************************************/

#ifndef __USB_HOST_STDDEF_H__
#define __USB_HOST_STDDEF_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "stdint.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/*  This Union is copied from usb_core.h  */
typedef union
{
    uint16_t w;
    struct BW
    {
        uint8_t msb;
        uint8_t lsb;
    }
    bw;
}uint16_t_uint8_t;
/* standard setup packet defination */
typedef union 
{
    uint8_t d8[8];
    struct _SetupPkt_Struc
    {
        uint8_t           bmRequestType;
        uint8_t           bRequest;
        uint16_t_uint8_t  wValue;
        uint16_t_uint8_t  wIndex;
        uint16_t_uint8_t  wLength;
    } b;
} usb_setup_typedef;

typedef  struct  
{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
} USB_HOST_DescHeader_TypeDef;

/* Standard Device Descriptor */
typedef struct 
{
    uint8_t   bLength;           //Size of this descriptor in bytes
    uint8_t   bDescriptorType;   //Device descriptor type
    uint16_t  bcdUSB;            //USB Specification Release Numbrer in Binary-Coded Decimal(i.e.,2.10 is 210H)
    uint8_t   bDeviceClass;      //Class code(assigned by the USB-IF)
    uint8_t   bDeviceSubClass;   //Subclass code(assigned by the USB-IF) this code is qualified by the value of the bDeviceClass field.
    uint8_t   bDeviceProtocol;   //Protocol code(assigned by the USB-IF), this code is qualified by the value of the bDeviceClass and the bDeviceSubClass fields.
    uint8_t   bMaxPacketSize0;   //Maximum packet size for EP0
    uint16_t  idVendor;          // Vendor ID (assigned by the USB-IF)
    uint16_t  idProduct;         // Product ID (assigned by manufacturer)
    uint16_t  bcdDevice;         // Device Release Number in binary-coded decimal
    uint8_t   iManufacturer;     // Index of string descriptor describing manufacturer
    uint8_t   iProduct;          // Index of string descriptor describing product
    uint8_t   iSerialNumber;     // Index of string descriptor describing the device's serial number
    uint8_t   bNumConfigurations;// Number of possible configurations
}usb_host_devdesc_typedef;

/* Standard Configuration Descriptor */
typedef struct 
{
    uint8_t   bLength;             //Size of this descriptor in bytes
    uint8_t   bDescriptorType;     //CONFIGURATION descriptor type
    uint16_t  wTotalLength;        // Total length of data returned for this configuration
    uint8_t   bNumInterfaces;      // Number of interfaces supported by this configuration
    uint8_t   bConfigurationValue; // Value to use as an argument to the SetConfiguration() request to select this configuration
    uint8_t   iConfiguration;      // Index of string descriptor describing this configuration
    uint8_t   bmAttributes;        // Configuration characteristics: D7:Reserved(set to one) D6:Self-powered D5:Remote Wakeup D4..0 Reserved(set to zero)
    uint8_t   bMaxPower;           // Maximum power consumption of the device from the bus in this specific configuration when the device is fully operational
}usb_host_cfgdesc_typedef;


typedef struct 
{
    uint8_t   bLength;
    uint8_t   bDescriptorType;
    uint16_t  bcdHID;               /* indicates what endpoint this descriptor is describing */
    uint8_t   bCountryCode;        /* specifies the transfer type. */
    uint8_t   bNumDescriptors;     /* specifies the transfer type. */
    uint8_t   bReportDescriptorType;    /* Maximum Packet Size this endpoint is capable of sending or receiving */
    uint16_t  wItemLength;          /* is used to specify the polling interval of certain transfers. */
}USB_HOST_HDIDesc_TypeDef;

/* Standard Interface Descriptor */
typedef struct 
{
    uint8_t bLength;              //Size of this descriptor in bytes
    uint8_t bDescriptorType;      //INTERFACE Descriptor Type
    uint8_t bInterfaceNumber;     //Number of this interface
    uint8_t bAlternateSetting;    // Value used to select this alternate setting for the interface identifiled in the prior field
    uint8_t bNumEndpoints;        // Number of Endpoints used by this interface
    uint8_t bInterfaceClass;      // Class code (assigned by the USB-IF) 
    uint8_t bInterfaceSubClass;   // Subclass code (assigned by the USB-IF)
    uint8_t bInterfaceProtocol;   // Protocol code (assigned by the USB)
    uint8_t iInterface;           // Index of string descriptor describing this interface 
}usb_host_itfdesc_typedef;

/* Standard Endpoint Descriptor */
typedef struct 
{
    uint8_t   bLength;           // Size of this descriptor in bytes
    uint8_t   bDescriptorType;   // ENDPOINT descriptor type
    uint8_t   bEndpointAddress;  // The address of the endpoint on the device described by this descriptor
    uint8_t   bmAttributes;      // refer to the related standard of USB 2.0
    uint16_t  wMaxPacketSize;    // Maximum Packet Size this endpoint is capable of sending or receiving when this configuration is selected
    uint8_t   bInterval;         // Interval for servicing the endpoint for data transfers
}USB_HOST_EPDesc_TypeDef;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

#ifndef USBH_NULL
#define USBH_NULL ((void *)0u)
#endif

#ifndef FALSE
#define FALSE 0u
#endif

#ifndef TRUE
#define TRUE 1u
#endif
//Get a 16bits data from buffer in little end mode.
#define  SMALL_END(addr)   (((uint16_t)(*((uint8_t *)(addr)))) + (((uint16_t)(*(((uint8_t *)(addr)) + 1u))) << 8u))

#define  USB_LEN_CFG_DESC                               (0x09u)

/* bmRequestType
D7: Data transfer direction
    0 = Host-to-device
    1 = Device-to-host
*/
#define  USB_REQ_DIR_MASK                               (0x80u)
#define  USB_H2D                                        (0x00u)
#define  USB_D2H                                        (0x80u)

/* bmRequestType 
D6...5: Type
        0 = Standard
        1 = Class
        2 = Vendor
        3 = Reserved
*/
#define  USB_REQ_TYPE_STANDARD                          (0x00u)
#define  USB_REQ_TYPE_CLASS                             (0x20u)
#define  USB_REQ_TYPE_VENDOR                            (0x40u)
#define  USB_REQ_TYPE_RESERVED                          (0x60u)

/* bmRequestType 
D4...0:  Recipient
         0 = Device
         1 = Interface
         2 = Endpoint
         3 = Other
         4...31 = Reserved
*/
#define  USB_REQ_RECIPIENT_DEVICE                       (0x00u)
#define  USB_REQ_RECIPIENT_INTERFACE                    (0x01u)
#define  USB_REQ_RECIPIENT_ENDPOINT                     (0x02u)
#define  USB_REQ_RECIPIENT_OTHER                        (0x03u)

/* Table 9-4. Standard Request Codes [USB Specification] */
         /*      bRequest                                Value    */
#define  USB_REQ_GET_STATUS                             (0x00u)
#define  USB_REQ_CLEAR_FEATURE                          (0x01u)
#define  USB_REQ_SET_FEATURE                            (0x03u)
#define  USB_REQ_SET_ADDRESS                            (0x05u)
#define  USB_REQ_GET_DESCRIPTOR                         (0x06u)
#define  USB_REQ_SET_DESCRIPTOR                         (0x07u)
#define  USB_REQ_GET_CONFIGURATION                      (0x08u)
#define  USB_REQ_SET_CONFIGURATION                      (0x09u)
#define  USB_REQ_GET_INTERFACE                          (0x0Au)
#define  USB_REQ_SET_INTERFACE                          (0x0Bu)
#define  USB_REQ_SYNCH_FRAME                            (0x0Cu)

/* Table 9-5. Descriptor Types  [USB Specification]  */
      /*   Descriptor Types                               Value    */
#define  USB_DESC_TYPE_DEVICE                              (1u)
#define  USB_DESC_TYPE_CONFIGURATION                       (2u)
#define  USB_DESC_TYPE_STRING                              (3u)
#define  USB_DESC_TYPE_INTERFACE                           (4u)
#define  USB_DESC_TYPE_ENDPOINT                            (5u)
#define  USB_DESC_TYPE_DEVICE_QUALIFIER                    (6u)
#define  USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION           (7u)
#define  USB_DESC_TYPE_INTERFACE_POWER                     (8u)
#define  USB_DESC_TYPE_HID                                 (0x21u)
#define  USB_DESC_TYPE_HID_REPORT                          (0x22u)

#define USB_DEVICE_DESC_SIZE                               (18u)
#define USB_CONFIGURATION_DESC_SIZE                        (9u)
#define USB_HID_DESC_SIZE                                  (9u)
#define USB_INTERFACE_DESC_SIZE                            (9u)
#define USB_ENDPOINT_DESC_SIZE                             (7u)

/* Descriptor Type and Descriptor Index  */
/* Use the following values when calling the function usb_host_getdesc  */
#define  USB_DESC_DEVICE                    (((uint16_t)USB_DESC_TYPE_DEVICE << 8u) & 0xFF00u)
#define  USB_DESC_CONFIGURATION             (((uint16_t)USB_DESC_TYPE_CONFIGURATION << 8u) & 0xFF00u)
#define  USB_DESC_STRING                    (((uint16_t)USB_DESC_TYPE_STRING << 8u) & 0xFF00u)
#define  USB_DESC_INTERFACE                 (((uint16_t)USB_DESC_TYPE_INTERFACE << 8u) & 0xFF00u)
#define  USB_DESC_ENDPOINT                  (((uint16_t)USB_DESC_TYPE_INTERFACE << 8u) & 0xFF00u)
#define  USB_DESC_DEVICE_QUALIFIER          (((uint16_t)USB_DESC_TYPE_DEVICE_QUALIFIER << 8u) & 0xFF00u)
#define  USB_DESC_OTHER_SPEED_CONFIGURATION (((uint16_t)USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION << 8u) & 0xFF00u)
#define  USB_DESC_INTERFACE_POWER           (((uint16_t)USB_DESC_TYPE_INTERFACE_POWER << 8u) & 0xFF00u)
#define  USB_DESC_HID_REPORT                (((uint16_t)USB_DESC_TYPE_HID_REPORT << 8u) & 0xFF00u)
#define  USB_DESC_HID                       (((uint16_t)USB_DESC_TYPE_HID << 8u) & 0xFF00u)


#define  USB_EP_TYPE_CTRL                               (0x00u)
#define  USB_EP_TYPE_ISOC                               (0x01u)
#define  USB_EP_TYPE_BULK                               (0x02u)
#define  USB_EP_TYPE_INTR                               (0x03u)

#define  USB_EP_DIR_OUT                                 (0x00u)
#define  USB_EP_DIR_IN                                  (0x80u)
#define  USB_EP_DIR_MSK                                 (0x80u)

/* supported classes */
#define USB_MSC_CLASS                                   (0x08u)
#define USB_HID_CLASS                                   (0x03u)

/* Interface Descriptor field values for HID Boot Protocol */
#define HID_BOOT_CODE                                  (0x01u)
#define HID_KEYBRD_BOOT_CODE                           (0x01u)
#define HID_MOUSE_BOOT_CODE                            (0x02u)

/* As per USB specs 9.2.6.4 :Standard request with data request timeout: 5sec
   Standard request with no data stage timeout : 50ms */
#define DATA_STAGE_TIMEOUT                              (5000u)
#define NODATA_STAGE_TIMEOUT                            (50u)

#define USBH_CONFIGURATION_DESCRIPTOR_SIZE (USB_CONFIGURATION_DESC_SIZE \
                                           + USB_INTERFACE_DESC_SIZE\
                                           + (USBH_MAX_NUM_ENDPOINTS * USB_ENDPOINT_DESC_SIZE))


#define CONFIG_DESC_wTOTAL_LENGTH (ConfigurationDescriptorData.ConfigDescfield.\
                                          ConfigurationDescriptor.wTotalLength)
/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/

#endif /* __USB_HOST_STDDEF_H__ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
