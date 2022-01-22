/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_host_core.h
 **
 ** A detailed description is available at
 ** @link header file for the usbh_core.c @endlink
 **
 **   - 2021-03-29  Linsq First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_HOST_CORE_H__
#define __USB_HOST_CORE_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_host_driver.h"
#include "usb_host_stddef.h"
#include "usbh_conf.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief USBH_CORE_Exported_Types
 **
 ******************************************************************************/
 /* Host status */
typedef enum {
    HSTATUS_OK = 0,
    HSTATUS_BUSY,
    HSTATUS_FAIL,
    HSTATUS_UNSUPPORTED,
    HSTATUS_UNRECOVERED_ERROR,
    HSTATUS_SPEED_UNKNOWN,
    HSTATUS_APP_DEINIT
}HOST_STATUS;

/* states about the handle stages on the host side */
typedef enum {
    HOST_IDLE = 0,
    HOST_DEV_CONNECTED,
    HOST_DEV_DISCONNECTED,
    HOST_GET_DEVSPEED,
    HOST_ENUM,
    HOST_CLASS_REQ,
    HOST_CLASS_PROCESS,
    HOST_CTRL_TRANSMIT,
    HOST_USER_INPUT,
    HOST_SUSPENDED,
    HOST_ERROR_STATE
}HOST_HANDLE_STATE;

/* states of the enumeration stage on the host side */
typedef enum {
    ENUM_IDLE = 0,
    ENUM_GET_FULL_DEVDESC,
    ENUM_SET_DEVADDR,
    ENUM_GET_CFGDESC,
    ENUM_GET_FULL_CFGDESC,
    ENUM_GET_MFCSTRINGDESC,
    ENUM_GET_PRODUCT_STRINGDESC,
    ENUM_GET_SERIALNUM_STRINGDESC,
    ENUM_SET_CFG,
    ENUM_DEV_CFG_OVER
} ENUM_HANDLE_STATE;

/* states of the control stages on the host side */
typedef enum {
    CTRL_IDLE =0,
    CTRL_SETUP,
    CTRL_SETUP_WAIT,
    CTRL_DATA_IN,
    CTRL_DATA_IN_WAIT,
    CTRL_DATA_OUT,
    CTRL_DATA_OUT_WAIT,
    CTRL_STATUS_IN,
    CTRL_STATUS_IN_WAIT,
    CTRL_STATUS_OUT,
    CTRL_STATUS_OUT_WAIT,
    CTRL_ERROR,
    CTRL_STALLED,
    CTRL_COMPLETE
}CTRL_HANDLE_STATE;

/* Following states are state machine for the request transferring */
typedef enum {
    REQ_CMD_IDLE = 0,
    REQ_CMD_TX,
    REQ_CMD_WAIT
} REQ_HANDLE_STATE;

typedef enum {    
    USER_HAVE_RESP = 0,
    USER_NONE_RESP
}HOST_USER_STATUS;

typedef struct 
{
    uint8_t               hc_num_in;         //channel number for the IN EP
    uint8_t               hc_num_out;        //channel number for the OUT EP
    uint8_t               ctrlmaxsize;       //the max size of EP0 parsed from the device descriptor
    uint8_t               err_cnt;           //the error counter
    uint16_t              sof_num;           //the frame number for sof packet
    uint16_t              length;            //length of data in byte
    uint8_t               *buff;             //data buffer 
    CTRL_HANDLE_STATUS    ctrl_status;       //status of control pipe    
    CTRL_HANDLE_STATE     ctrl_state;        //running state of the control transfer
    usb_setup_typedef     setup;             //setup packet
} usb_host_ctrl_param;

/* Device information parsed from the related descriptors requested from the connected device
   the following data are all parsed from the data sent by the connnected device */
typedef struct 
{
    uint8_t                           devaddr;           //the address of the connected device
    uint8_t                           devspeed;          //the core speed of the connected device
    usb_host_devdesc_typedef          devdesc;           //the device descriptor parsed from the data sent by device
    usb_host_cfgdesc_typedef          devcfgdesc;        //the device configuration descriptor parsed from the data sent by device
    usb_host_itfdesc_typedef          devitfdesc[USBH_MAX_NUM_INTERFACES]; //the interface descritpor
    USB_HOST_EPDesc_TypeDef               devepdesc[USBH_MAX_NUM_INTERFACES][USBH_MAX_NUM_ENDPOINTS];  //the endpoint descriptor
    USB_HOST_HDIDesc_TypeDef              hiddesc;          //the hid descriptor
}usb_host_devinformation;

typedef struct 
{
    HOST_STATUS  (*host_class_init)   (usb_core_instance *pdev , void *phost);
    void         (*host_class_deinit) (usb_core_instance *pdev);
    HOST_STATUS  (*host_class_request)(usb_core_instance *pdev , void *phost);
    HOST_STATUS  (*host_class_process)(usb_core_instance *pdev , void *phost);
} usb_host_class_callback_func;

typedef struct 
{
    void (*huser_init)(void);       
    void (*huser_deinit)(void);       
    void (*huser_devattached)(void);          
    void (*huser_devreset)(void);
    void (*huser_devdisconn)(void);
    void (*huser_overcurrent)(void);
    void (*huser_devspddetected)(uint8_t DeviceSpeed);        
    void (*huser_devdescavailable)(void *);    
    void (*huser_devaddrdistributed)(void);   
    void (*huser_cfgdescavailable)(usb_host_cfgdesc_typedef *,
                                     usb_host_itfdesc_typedef *,
                                     USB_HOST_EPDesc_TypeDef *);
    /* Configuration Descriptor available */
    void (*huser_mfcstring)(void *);     
    void (*huser_productstring)(void *);          
    void (*huser_serialnum)(void *);       
    void (*huser_enumcompl)(void);          
    HOST_USER_STATUS (*huser_userinput)(void);
    int  (*huser_application) (void);
    void (*huser_devunsupported)(void); 
    void (*huser_unrecoverederror)(void);
}usb_host_user_callback_func;

typedef struct
{
    /* states for the host, enumeration, request */
    REQ_HANDLE_STATE                       req_state;          // value of state machine about the request
    ENUM_HANDLE_STATE                      enum_state;         // state machine while enumerating      
    HOST_HANDLE_STATE                      host_state_backup;  // backup value of state machine about the host 
    HOST_HANDLE_STATE                      host_state;         // value of state machine about the host    
    /* control informations */
    usb_host_ctrl_param                    ctrlparam;          // values about the control parameters   
    /* device information parsed from the descriptors from the device */
    usb_host_devinformation                device_prop;
    /* functions: call back functions for the class and user */
    usb_host_class_callback_func           *class_callbk;
    usb_host_user_callback_func            *user_callbk;
} USBH_HOST;
/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief USBH_CORE_Exported_Defines
 **
 ******************************************************************************/
#define MSC_CLASS                         (0x08u)
#define HID_CLASS                         (0x03u)
#define MSC_PROTOCOL                      (0x50u)
#define CBI_PROTOCOL                      (0x01u)


#define DEV_DEFAULT_ADDRESS               (0u)
#define DEV_ASSIGNED_ADDRESS              (1u)

#define HOST_MAX_ERROR_CNT                (2u)
/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
extern void usb_host_init(usb_core_instance *pdev,
                                  USBH_HOST *phost,
               usb_host_class_callback_func *class_cbk,
                usb_host_user_callback_func *user_cbk);
extern void usb_host_deinit(usb_core_instance *pdev, USBH_HOST *phost);
extern void usb_host_mainprocess(usb_core_instance *pdev, USBH_HOST *phost);
extern void usb_host_errorprocess(USBH_HOST *phost, HOST_STATUS errType);

#endif /* __USB_HOST_CORE_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
