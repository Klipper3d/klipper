/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_host_core.c
 **
 ** A detailed description is available at
 ** @link
        This file implements the functions for the core state machine process
    the enumeration and the control transfer process
    @endlink
 **
 **   - 2021-03-29  Linsq First version for USB demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <stdio.h>
#include "usb_host_ctrltrans.h"
#include "usb_bsp.h"
#include "usb_host_cfgch.h"
#include "usb_host_stdreq.h"
#include "usb_host_core.h"
#include "usb_host_int.h"
#include "usb_host_user.h"

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
HOST_STATUS usb_host_ctrlprocess(usb_core_instance *pdev, USBH_HOST *phost);
HOST_STATUS usb_host_enumprocess(usb_core_instance *pdev, USBH_HOST *phost);


/**
 *******************************************************************************
 ** \brief  initialization for the host application
 ** \param  pdev : device instance
 ** \param  phost: host state set
 ** \param  class_cbk: the call back function for the class application
 ** \param  user_cbk: the call back function for user
 ** \retval none
 ******************************************************************************/
void usb_host_init(usb_core_instance *pdev,
                           USBH_HOST *phost,
        usb_host_class_callback_func *class_cbk,
         usb_host_user_callback_func *user_cbk)
{
    hd_usb_bsp_init(pdev);
    hd_usb_bsp_cfgvbus(pdev);
    usb_host_deinit(pdev, phost);
    phost->class_callbk = class_cbk;
    phost->user_callbk  = user_cbk;
    host_driver_init(pdev);
    phost->user_callbk->huser_init();
    usb_bsp_nvic();
}

/**
 *******************************************************************************
 ** \brief  deinitialize the host application
 ** \param  pdev : device instance
 ** \param  phost: host state set
 ** \retval none
 ******************************************************************************/
void usb_host_deinit(usb_core_instance *pdev, USBH_HOST *phost)
{
    phost->host_state = HOST_IDLE;
    phost->host_state_backup = HOST_IDLE;
    phost->enum_state = ENUM_IDLE;
    phost->req_state = REQ_CMD_TX;

    phost->ctrlparam.ctrl_state = CTRL_SETUP;
    phost->ctrlparam.ctrlmaxsize = USB_MAX_EP0_SIZE;

    phost->device_prop.devaddr = DEV_DEFAULT_ADDRESS;
    phost->device_prop.devspeed = PRTSPD_FULL_SPEED;

    usb_host_freech(pdev, phost->ctrlparam.hc_num_in);
    usb_host_freech(pdev, phost->ctrlparam.hc_num_out);
}

/**
 *******************************************************************************
 ** \brief  This is the main process function for the host core, it will process
 **         the main machine, such as connect,disconnect, emunation etc.
 ** \param  pdev : device instance
 ** \param  *phost: host state set
 ** \retval none
 ******************************************************************************/
void usb_host_mainprocess(usb_core_instance *pdev , USBH_HOST *phost)
{
    __IO HOST_STATUS tmp_status;
    tmp_status = HSTATUS_FAIL;
    HOST_HANDLE_STATE tmp_host_state;

    if ((host_driver_ifdevconnected(pdev) == 0ul)&& (phost->host_state != HOST_IDLE))
    {
        if(phost->host_state != HOST_DEV_DISCONNECTED)
        {
            phost->host_state = HOST_DEV_DISCONNECTED;
        }
    }

    if((host_driver_ifdevconnected(pdev) == 0ul) && (phost->host_state == HOST_IDLE) && (host_driver_getvbusdrivestate(pdev) == 0ul))
    {
        phost->host_state = HOST_DEV_DISCONNECTED;
    }

    tmp_host_state = phost->host_state;
    if(tmp_host_state == HOST_IDLE)
    {
        if (host_driver_ifdevconnected(pdev))
        {
            phost->host_state = HOST_DEV_CONNECTED;
            hd_usb_mdelay(50ul);
        }
    }
    else if(tmp_host_state == HOST_DEV_CONNECTED)
    {
        #ifdef MSC_HID_COMPOSITE
        if(host_driver_getcurrentspd(pdev) == 2)
        {
            host_driver_init(pdev);
        }
        #endif //MSC_HID_COMPOSITE
        phost->user_callbk->huser_devattached();
        phost->ctrlparam.hc_num_out = usb_host_distrch(pdev, 0x00u);
        phost->ctrlparam.hc_num_in = usb_host_distrch(pdev, 0x80u);
        host_driver_portrst(pdev);
        phost->user_callbk->huser_devreset();
        phost->device_prop.devspeed = (uint8_t)host_driver_getcurrentspd(pdev);
        phost->host_state = HOST_ENUM;
        phost->user_callbk->huser_devspddetected(phost->device_prop.devspeed);
        usb_host_chopen(pdev,
                        phost->ctrlparam.hc_num_in,
                        phost->device_prop.devaddr,
                        phost->device_prop.devspeed,
                        EP_TYPE_CTRL,
                        (uint16_t)phost->ctrlparam.ctrlmaxsize);
        usb_host_chopen(pdev,
                        phost->ctrlparam.hc_num_out,
                        phost->device_prop.devaddr,
                        phost->device_prop.devspeed,
                        EP_TYPE_CTRL,
                        (uint16_t)phost->ctrlparam.ctrlmaxsize);
    }
    else if(tmp_host_state == HOST_ENUM)
    {
        if( usb_host_enumprocess(pdev , phost) == HSTATUS_OK)
        {
            phost->user_callbk->huser_enumcompl();
            phost->host_state  = HOST_USER_INPUT;
        }
    }
    else if(tmp_host_state == HOST_USER_INPUT)
    {
        if((phost->class_callbk->host_class_init(pdev, phost)) == HSTATUS_OK)
        {
            phost->host_state  = HOST_CLASS_REQ;
        }
    }
    else if(tmp_host_state == HOST_CLASS_REQ)
    {
        tmp_status = phost->class_callbk->host_class_request(pdev, phost);
        if(tmp_status == HSTATUS_OK)
        {
            phost->host_state  = HOST_CLASS_PROCESS;
        }
        else
        {
            usb_host_errorprocess(phost, tmp_status);
        }
    }
    else if(tmp_host_state == HOST_CLASS_PROCESS)
    {
        tmp_status = phost->class_callbk->host_class_process(pdev, phost);
        usb_host_errorprocess(phost, tmp_status);
    }
    else if(tmp_host_state == HOST_CTRL_TRANSMIT)
    {
        usb_host_ctrlprocess(pdev, phost);
    }
    else if(tmp_host_state == HOST_ERROR_STATE)
    {
        usb_host_deinit(pdev, phost);
        phost->user_callbk->huser_deinit();
        phost->class_callbk->host_class_deinit(pdev);
    }
    else if(tmp_host_state == HOST_DEV_DISCONNECTED)
    {
        phost->user_callbk->huser_devdisconn();
        usb_host_deinit(pdev, phost);
        phost->user_callbk->huser_deinit();
        phost->class_callbk->host_class_deinit(pdev);
        usb_host_dedistrallch(pdev);
        phost->host_state = HOST_IDLE;

        host_driver_init(pdev);
    }
    else
    {
        ;
    }
}

/**
 *******************************************************************************
 ** \brief  process the status when related error status happens.
 ** \param  *phost: host state set
 ** \param  errType : host status
 ** \retval none
 ******************************************************************************/
void usb_host_errorprocess(USBH_HOST *phost, HOST_STATUS errType)
{
    switch(errType)
    {
       case HSTATUS_APP_DEINIT:
            phost->host_state = HOST_ERROR_STATE;
            phost->user_callbk->huser_init();
            break;
       case HSTATUS_SPEED_UNKNOWN:
       case HSTATUS_UNRECOVERED_ERROR:
            phost->user_callbk->huser_unrecoverederror();
            phost->host_state = HOST_ERROR_STATE;
            break;
       default:
            break;
    }
}

/**
 *******************************************************************************
 ** \brief  this function process all the emunation steps.
 ** \param  pdev : device instance
 ** \param  phost: host state set
 ** \retval status
 ******************************************************************************/
HOST_STATUS usb_host_enumprocess(usb_core_instance *pdev, USBH_HOST *phost)
{
    HOST_STATUS tmp_status;
    uint8_t Local_Buffer[64];
    ENUM_HANDLE_STATE tmp_enum_state;
    tmp_status = HSTATUS_BUSY;
    tmp_enum_state = phost->enum_state;
    if(tmp_enum_state == ENUM_IDLE)
    {
        if ( usb_host_getdevdesc(pdev , phost, 8u) == HSTATUS_OK)
        {
            phost->ctrlparam.ctrlmaxsize = phost->device_prop.devdesc.bMaxPacketSize0;
            host_driver_portrst(pdev);
            phost->enum_state = ENUM_GET_FULL_DEVDESC;
            usb_host_mdfch (pdev,
                                    phost->ctrlparam.hc_num_out,
                                    0u,
                                    0u,
                                    0u,
                                    (uint16_t)phost->ctrlparam.ctrlmaxsize);
            usb_host_mdfch (pdev,
                                    phost->ctrlparam.hc_num_in,
                                    0u,
                                    0u,
                                    0u,
                                    (uint16_t)phost->ctrlparam.ctrlmaxsize);
        }
    }
    if(tmp_enum_state == ENUM_GET_FULL_DEVDESC)
    {
      if ( usb_host_getdevdesc(pdev, phost, USB_DEVICE_DESC_SIZE) == HSTATUS_OK)
      {
            phost->user_callbk->huser_devdescavailable(&phost->device_prop.devdesc);
            phost->enum_state = ENUM_SET_DEVADDR;
      }
    }
    if(tmp_enum_state == ENUM_SET_DEVADDR)
    {
      if ( usb_host_setdevaddr(pdev, phost, DEV_ASSIGNED_ADDRESS) == HSTATUS_OK)
      {
            hd_usb_mdelay(2ul);
            phost->device_prop.devaddr = DEV_ASSIGNED_ADDRESS;
            phost->user_callbk->huser_devaddrdistributed();
            phost->enum_state = ENUM_GET_CFGDESC;
            usb_host_mdfch (pdev,
                                    phost->ctrlparam.hc_num_in,
                                    phost->device_prop.devaddr,
                                    0u,
                                    0u,
                                    0u);

            usb_host_mdfch (pdev,
                                    phost->ctrlparam.hc_num_out,
                                    phost->device_prop.devaddr,
                                    0u,
                                    0u,
                                    0u);
      }
    }
    if(tmp_enum_state == ENUM_GET_CFGDESC)
    {
      if ( usb_host_getcfgdesc(pdev, phost, USB_CONFIGURATION_DESC_SIZE) == HSTATUS_OK)
      {
            phost->enum_state = ENUM_GET_FULL_CFGDESC;
      }
    }
    if(tmp_enum_state == ENUM_GET_FULL_CFGDESC)
    {
      if (usb_host_getcfgdesc(pdev, phost, phost->device_prop.devcfgdesc.wTotalLength) == HSTATUS_OK)
      {
            phost->user_callbk->huser_cfgdescavailable(&phost->device_prop.devcfgdesc,
                              phost->device_prop.devitfdesc,
                              phost->device_prop.devepdesc[0]);
            phost->enum_state = ENUM_GET_MFCSTRINGDESC;
      }
    }
    if(tmp_enum_state == ENUM_GET_MFCSTRINGDESC)
    {
      if (phost->device_prop.devdesc.iManufacturer != (uint8_t)0)
      {
            if (usb_host_getstringdesc(pdev,
                                       phost,
                                       phost->device_prop.devdesc.iManufacturer,
                                       Local_Buffer ,
                                       0xffu) == HSTATUS_OK)
            {
               phost->user_callbk->huser_mfcstring(Local_Buffer);
               phost->enum_state = ENUM_GET_PRODUCT_STRINGDESC;
            }
      }
      else
      {
            phost->user_callbk->huser_mfcstring("N/A");
            phost->enum_state = ENUM_GET_PRODUCT_STRINGDESC;
      }
    }
    if(tmp_enum_state == ENUM_GET_PRODUCT_STRINGDESC)
    {
      if (phost->device_prop.devdesc.iProduct != (uint8_t)0)
      {
            if (usb_host_getstringdesc(pdev,
                                       phost,
                                       phost->device_prop.devdesc.iProduct,
                                       Local_Buffer,
                                       0xffu) == HSTATUS_OK)
            {
               phost->user_callbk->huser_productstring(Local_Buffer);
               phost->enum_state = ENUM_GET_SERIALNUM_STRINGDESC;
            }
      }
      else
      {
            phost->user_callbk->huser_productstring("N/A");
            phost->enum_state = ENUM_GET_SERIALNUM_STRINGDESC;
      }
    }
    if(tmp_enum_state == ENUM_GET_SERIALNUM_STRINGDESC)
    {
      if (phost->device_prop.devdesc.iSerialNumber != (uint8_t)0)
      {
            if (usb_host_getstringdesc(pdev,
                                       phost,
                                       phost->device_prop.devdesc.iSerialNumber,
                                       Local_Buffer,
                                       0xffu) == HSTATUS_OK)
            {
               phost->user_callbk->huser_serialnum(Local_Buffer);
               phost->enum_state = ENUM_SET_CFG;
            }
      }
      else
      {
            phost->user_callbk->huser_serialnum("N/A");
            phost->enum_state = ENUM_SET_CFG;
      }
    }
    if(tmp_enum_state == ENUM_SET_CFG)
    {
      if (usb_host_setconfig(pdev, phost,
                        (uint16_t)phost->device_prop.devcfgdesc.bConfigurationValue) == HSTATUS_OK)
      {
            phost->enum_state = ENUM_DEV_CFG_OVER;
      }
    }
    if(tmp_enum_state == ENUM_DEV_CFG_OVER)
    {
      tmp_status = HSTATUS_OK;
    }
    else
    {
      ;
    }
    return tmp_status;
}


/**
 *******************************************************************************
 ** \brief  process the state machine of control transfer
 ** \param  pdev : device instance
 ** \retval status
 ******************************************************************************/
HOST_STATUS usb_host_ctrlprocess(usb_core_instance *pdev, USBH_HOST *phost)
{
    uint8_t direction;
    CTRL_HANDLE_STATE tmp_ctrl_state;
    static uint16_t timeout = 0;
    HOST_STATUS status = HSTATUS_OK;
    HOST_CH_XFER_STATE URB_Status = HOST_CH_XFER_IDLE;

    phost->ctrlparam.ctrl_status = CTRL_START;
    tmp_ctrl_state = phost->ctrlparam.ctrl_state;

   if(tmp_ctrl_state == CTRL_SETUP)
   {
      /* transmit a setup packet to the device */
      usb_host_sendctrlsetup(pdev, phost->ctrlparam.setup.d8, phost->ctrlparam.hc_num_out);
      phost->ctrlparam.ctrl_state = CTRL_SETUP_WAIT;
      timeout = DATA_STAGE_TIMEOUT * 6u;
      phost->ctrlparam.sof_num = (uint16_t)host_driver_getcurrentfrm(pdev);
   }
   else if(tmp_ctrl_state == CTRL_SETUP_WAIT)
   {
      URB_Status = host_driver_getxferstate(pdev , phost->ctrlparam.hc_num_out);
      /* case SETUP packet sent successfully */
      if(URB_Status == HOST_CH_XFER_DONE)
      {
            /* parse the direction of the request from the setup just sent */
            direction = (phost->ctrlparam.setup.b.bmRequestType & USB_REQ_DIR_MASK);
            /* judge if there is a data stage, if wLength is not zero, there may be a in or out
               data stage */
            if (phost->ctrlparam.setup.b.wLength.w != 0u )
            {
               timeout = DATA_STAGE_TIMEOUT;
               if (direction == USB_D2H)
               {
                  /* Data Direction is IN, device should send data in */
                  phost->ctrlparam.ctrl_state = CTRL_DATA_IN;
               }
               else
               {
                  /* Data Direction is OUT, host will send data out for device */
                  phost->ctrlparam.ctrl_state = CTRL_DATA_OUT;
               }
            }
            /* No DATA stage */
            else
            {
               timeout = NODATA_STAGE_TIMEOUT;
               /* If there is No Data Transfer Stage */
               if (direction == USB_D2H)
               {
                  /* Data Direction is IN */
                  phost->ctrlparam.ctrl_state = CTRL_STATUS_OUT;
               }
               else
               {
                  /* Data Direction is OUT */
                  phost->ctrlparam.ctrl_state = CTRL_STATUS_IN;
               }
            }
            /* Set the delay timer to enable timeout for data stage completion */
            phost->ctrlparam.sof_num = (uint16_t)host_driver_getcurrentfrm(pdev);
      }
      else if(URB_Status == HOST_CH_XFER_ERROR)
      {
            phost->ctrlparam.ctrl_state = CTRL_ERROR;
            phost->ctrlparam.ctrl_status = CTRL_XACTERR;
      }
      else if ((host_driver_getcurrentfrm(pdev)- phost->ctrlparam.sof_num) > timeout)
      {
            DDL_Printf("Device not responding\n");
      }
      else
      {
            ;
      }
   }
   else if(tmp_ctrl_state == CTRL_DATA_IN)
   {
      /* Issue an IN token */
      usb_host_recvctrldata(pdev, phost->ctrlparam.buff, phost->ctrlparam.length, phost->ctrlparam.hc_num_in);
      phost->ctrlparam.ctrl_state = CTRL_DATA_IN_WAIT;
   }
   else if(tmp_ctrl_state == CTRL_DATA_IN_WAIT)
   {
      URB_Status = host_driver_getxferstate(pdev , phost->ctrlparam.hc_num_in);
      /* check is DATA packet transfered successfully */
      if  (URB_Status == HOST_CH_XFER_DONE)
      {
            phost->ctrlparam.ctrl_state = CTRL_STATUS_OUT;
      }
      /* manage error cases*/
      if  (URB_Status == HOST_CH_XFER_STALL)
      {
            /* In stall case, return to previous machine state*/
            phost->host_state =   phost->host_state_backup;
      }
      else if (URB_Status == HOST_CH_XFER_ERROR)
      {
            /* Device error */
            phost->ctrlparam.ctrl_state = CTRL_ERROR;
      }
      else if ((host_driver_getcurrentfrm(pdev)- phost->ctrlparam.sof_num) > timeout)
      {
            /* timeout for IN transfer */
            phost->ctrlparam.ctrl_state = CTRL_ERROR;
      }
      else
      {
            ;
      }
   }
   else if(tmp_ctrl_state == CTRL_DATA_OUT)
   {
      /* Start DATA out transfer (only one DATA packet)*/
      pdev->host.hc[phost->ctrlparam.hc_num_out].out_toggle = 1;

      usb_host_sendctrldata (pdev,
      phost->ctrlparam.buff,
      phost->ctrlparam.length ,
      phost->ctrlparam.hc_num_out);
      phost->ctrlparam.ctrl_state = CTRL_DATA_OUT_WAIT;
   }
   else if(tmp_ctrl_state == CTRL_DATA_OUT_WAIT)
   {
      URB_Status = host_driver_getxferstate(pdev , phost->ctrlparam.hc_num_out);
      switch(URB_Status)
      {
         case HOST_CH_XFER_DONE:
         phost->ctrlparam.ctrl_state = CTRL_STATUS_IN;
         break;
         case HOST_CH_XFER_STALL:
         phost->host_state =   phost->host_state_backup;
         phost->ctrlparam.ctrl_state = CTRL_STALLED;
         break;
         case HOST_CH_XFER_UNREADY:
         phost->ctrlparam.ctrl_state = CTRL_DATA_OUT;
         break;
         case HOST_CH_XFER_ERROR:
         phost->ctrlparam.ctrl_state = CTRL_ERROR;
         break;
         default:
         break;
      }
   }
   else if(tmp_ctrl_state == CTRL_STATUS_IN)
   {
      /* receive a packet with 0 byte */
      usb_host_recvctrldata (pdev, 0u, 0u, phost->ctrlparam.hc_num_in);
      phost->ctrlparam.ctrl_state = CTRL_STATUS_IN_WAIT;
   }
   else if(tmp_ctrl_state == CTRL_STATUS_IN_WAIT)
   {
      URB_Status = host_driver_getxferstate(pdev , phost->ctrlparam.hc_num_in);
      if  ( URB_Status == HOST_CH_XFER_DONE)
      { /* Control transfers completed, Exit the State Machine */
            phost->host_state =   phost->host_state_backup;
            phost->ctrlparam.ctrl_state = CTRL_COMPLETE;
      }
      else if (URB_Status == HOST_CH_XFER_ERROR)
      {
            phost->ctrlparam.ctrl_state = CTRL_ERROR;
      }
      else if((host_driver_getcurrentfrm(pdev) - phost->ctrlparam.sof_num) > timeout)
      {
            phost->ctrlparam.ctrl_state = CTRL_ERROR;
      }
      else if(URB_Status == HOST_CH_XFER_STALL)
      {
            /* Control transfers completed, Exit the State Machine */
            phost->host_state =   phost->host_state_backup;
            phost->ctrlparam.ctrl_status = CTRL_STALL;
            status = HSTATUS_UNSUPPORTED;
      }
      else
      {
            ;
      }
   }
   else if(tmp_ctrl_state == CTRL_STATUS_OUT)
   {
      pdev->host.hc[phost->ctrlparam.hc_num_out].out_toggle ^= 1;
      usb_host_sendctrldata (pdev, 0u, 0u, phost->ctrlparam.hc_num_out);
      phost->ctrlparam.ctrl_state = CTRL_STATUS_OUT_WAIT;
   }
   else if(tmp_ctrl_state == CTRL_STATUS_OUT_WAIT)
   {
      URB_Status = host_driver_getxferstate(pdev , phost->ctrlparam.hc_num_out);
      switch(URB_Status)
      {
         case HOST_CH_XFER_DONE:
                           phost->host_state =   phost->host_state_backup;
            phost->ctrlparam.ctrl_state = CTRL_COMPLETE;
         break;
         case HOST_CH_XFER_UNREADY:
         phost->ctrlparam.ctrl_state = CTRL_STATUS_OUT;
         break;
         case HOST_CH_XFER_ERROR:
         phost->ctrlparam.ctrl_state = CTRL_ERROR;
         break;
         default:
         break;
      }
   }
   else if(tmp_ctrl_state == CTRL_ERROR)
   {
      if (++ phost->ctrlparam.err_cnt <= HOST_MAX_ERROR_CNT)
      {
            /* re-start the transmission, starting from SETUP packet */
            phost->ctrlparam.ctrl_state = CTRL_SETUP;
      }
      else
      {
            phost->ctrlparam.ctrl_status = CTRL_FAIL;
            phost->host_state =   phost->host_state_backup;

            status = HSTATUS_FAIL;
      }
   }
   else
   {
      ;
   }
    return status;
}

/*******************************************************************************
 ** EOF
 ******************************************************************************/
