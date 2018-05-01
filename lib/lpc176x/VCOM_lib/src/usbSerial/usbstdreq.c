/*
	LPCUSB, an USB device driver for LPC microcontrollers	
	Copyright (C) 2006 Bertrik Sikken (bertrik@sikken.nl)

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.
	3. The name of the author may not be used to endorse or promote products
	   derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/** @file
	Standard request handler.
	
	This modules handles the 'chapter 9' processing, specifically the
	standard device requests in table 9-3 from the universal serial bus
	specification revision 2.0
	
	Specific types of devices may specify additional requests (for example
	HID devices add a GET_DESCRIPTOR request for interfaces), but they
	will not be part of this module.

	@todo some requests have to return a request error if device not configured:
	@todo GET_INTERFACE, GET_STATUS, SET_INTERFACE, SYNCH_FRAME
	@todo this applies to the following if endpoint != 0:
	@todo SET_FEATURE, GET_FEATURE 
*/

// CodeRed - include the LPCUSB type.h file rather than NXP one directly
//#include "type.h"
#include "lpcusb_type.h"

#include "usbdebug.h"
#include "usbstruct.h"
#include "usbapi.h"

#define MAX_DESC_HANDLERS	4		/**< device, interface, endpoint, other */


/* general descriptor field offsets */
#define DESC_bLength					0	/**< length offset */
#define DESC_bDescriptorType			1	/**< descriptor type offset */	

/* config descriptor field offsets */
#define CONF_DESC_wTotalLength			2	/**< total length offset */
#define CONF_DESC_bConfigurationValue	5	/**< configuration value offset */	
#define CONF_DESC_bmAttributes			7	/**< configuration characteristics */

/* interface descriptor field offsets */
#define INTF_DESC_bAlternateSetting		3	/**< alternate setting offset */

/* endpoint descriptor field offsets */
#define ENDP_DESC_bEndpointAddress		2	/**< endpoint address offset */
#define ENDP_DESC_wMaxPacketSize		4	/**< maximum packet size offset */


/** Currently selected configuration */
static U8				bConfiguration = 0;
/** Installed custom request handler */
static TFnHandleRequest	*pfnHandleCustomReq = NULL;
/** Pointer to registered descriptors */
static const U8			*pabDescrip = NULL;


/**
	Registers a pointer to a descriptor block containing all descriptors
	for the device.

	@param [in]	pabDescriptors	The descriptor byte array
 */
void USBRegisterDescriptors(const U8 *pabDescriptors)
{
	pabDescrip = pabDescriptors;
}


/**
	Parses the list of installed USB descriptors and attempts to find
	the specified USB descriptor.
		
	@param [in]		wTypeIndex	Type and index of the descriptor
	@param [in]		wLangID		Language ID of the descriptor (currently unused)
	@param [out]	*piLen		Descriptor length
	@param [out]	*ppbData	Descriptor data
	
	@return TRUE if the descriptor was found, FALSE otherwise
 */
BOOL USBGetDescriptor(U16 wTypeIndex, U16 wLangID, int *piLen, U8 **ppbData)
{
	U8	bType, bIndex;
	U8	*pab;
	int iCurIndex;
	wLangID = wLangID;
	ASSERT(pabDescrip != NULL);

	bType = GET_DESC_TYPE(wTypeIndex);
	bIndex = GET_DESC_INDEX(wTypeIndex);
	
	pab = (U8 *)pabDescrip;
	iCurIndex = 0;
	
	while (pab[DESC_bLength] != 0) {
		if (pab[DESC_bDescriptorType] == bType) {
			if (iCurIndex == bIndex) {
				// set data pointer
				*ppbData = pab;
				// get length from structure
				if (bType == DESC_CONFIGURATION) {
					// configuration descriptor is an exception, length is at offset 2 and 3
					*piLen =	(pab[CONF_DESC_wTotalLength]) |
								(pab[CONF_DESC_wTotalLength + 1] << 8);
				}
				else {
					// normally length is at offset 0
					*piLen = pab[DESC_bLength];
				}
				return TRUE;
			}
			iCurIndex++;
		}
		// skip to next descriptor
		pab += pab[DESC_bLength];
	}
	// nothing found
	DBG("Desc %x not found!\n", wTypeIndex);
	return FALSE;
}


/**
	Configures the device according to the specified configuration index and
	alternate setting by parsing the installed USB descriptor list.
	A configuration index of 0 unconfigures the device.
		
	@param [in]		bConfigIndex	Configuration index
	@param [in]		bAltSetting		Alternate setting number
	
	@todo function always returns TRUE, add stricter checking?
	
	@return TRUE if successfully configured, FALSE otherwise
 */
static BOOL USBSetConfiguration(U8 bConfigIndex, U8 bAltSetting)
{
	U8	*pab;
	U8	bCurConfig, bCurAltSetting;
	U8	bEP;
	U16	wMaxPktSize;
	
	ASSERT(pabDescrip != NULL);

	if (bConfigIndex == 0) {
		// unconfigure device
		USBHwConfigDevice(FALSE);
	}
	else {
		// configure endpoints for this configuration/altsetting
		pab = (U8 *)pabDescrip;
		bCurConfig = 0xFF;
		bCurAltSetting = 0xFF;

		while (pab[DESC_bLength] != 0) {

			switch (pab[DESC_bDescriptorType]) {

			case DESC_CONFIGURATION:
				// remember current configuration index
				bCurConfig = pab[CONF_DESC_bConfigurationValue];
				break;

			case DESC_INTERFACE:
				// remember current alternate setting
				bCurAltSetting = pab[INTF_DESC_bAlternateSetting];
				break;

			case DESC_ENDPOINT:
				if ((bCurConfig == bConfigIndex) &&
					(bCurAltSetting == bAltSetting)) {
					// endpoint found for desired config and alternate setting
					bEP = pab[ENDP_DESC_bEndpointAddress];
					wMaxPktSize = 	(pab[ENDP_DESC_wMaxPacketSize]) |
									(pab[ENDP_DESC_wMaxPacketSize + 1] << 8);
					// configure endpoint
					USBHwEPConfig(bEP, wMaxPktSize);
				}
				break;

			default:
				break;
			}
			// skip to next descriptor
			pab += pab[DESC_bLength];
		}
		
		// configure device
		USBHwConfigDevice(TRUE);
	}

	return TRUE;
}


/**
	Local function to handle a standard device request
		
	@param [in]		pSetup		The setup packet
	@param [in,out]	*piLen		Pointer to data length
	@param [in,out]	ppbData		Data buffer.

	@return TRUE if the request was handled successfully
 */
static BOOL HandleStdDeviceReq(TSetupPacket *pSetup, int *piLen, U8 **ppbData)
{
	U8	*pbData = *ppbData;

	switch (pSetup->bRequest) {
	
	case REQ_GET_STATUS:
		// bit 0: self-powered
		// bit 1: remote wakeup = not supported
		pbData[0] = 0;
		pbData[1] = 0;
		*piLen = 2;
		break;
		
	case REQ_SET_ADDRESS:
		USBHwSetAddress(pSetup->wValue);
		break;

	case REQ_GET_DESCRIPTOR:
		DBG("D%x", pSetup->wValue);
		return USBGetDescriptor(pSetup->wValue, pSetup->wIndex, piLen, ppbData);

	case REQ_GET_CONFIGURATION:
		// indicate if we are configured
		pbData[0] = bConfiguration;
		*piLen = 1;
		break;

	case REQ_SET_CONFIGURATION:
		if (!USBSetConfiguration(pSetup->wValue & 0xFF, 0)) {
			DBG("USBSetConfiguration failed!\n");
			return FALSE;
		}
		// configuration successful, update current configuration
		bConfiguration = pSetup->wValue & 0xFF;	
		break;

	case REQ_CLEAR_FEATURE:
	case REQ_SET_FEATURE:
		if (pSetup->wValue == FEA_REMOTE_WAKEUP) {
			// put DEVICE_REMOTE_WAKEUP code here
		}
		if (pSetup->wValue == FEA_TEST_MODE) {
			// put TEST_MODE code here
		}
		return FALSE;

	case REQ_SET_DESCRIPTOR:
		DBG("Device req %d not implemented\n", pSetup->bRequest);
		return FALSE;

	default:
		DBG("Illegal device req %d\n", pSetup->bRequest);
		return FALSE;
	}
	
	return TRUE;
}


/**
	Local function to handle a standard interface request
		
	@param [in]		pSetup		The setup packet
	@param [in,out]	*piLen		Pointer to data length
	@param [in]		ppbData		Data buffer.

	@return TRUE if the request was handled successfully
 */
static BOOL HandleStdInterfaceReq(TSetupPacket	*pSetup, int *piLen, U8 **ppbData)
{
	U8	*pbData = *ppbData;

	switch (pSetup->bRequest) {

	case REQ_GET_STATUS:
		// no bits specified
		pbData[0] = 0;
		pbData[1] = 0;
		*piLen = 2;
		break;

	case REQ_CLEAR_FEATURE:
	case REQ_SET_FEATURE:
		// not defined for interface
		return FALSE;
	
	case REQ_GET_INTERFACE:	// TODO use bNumInterfaces
        // there is only one interface, return n-1 (= 0)
		pbData[0] = 0;
		*piLen = 1;
		break;
	
	case REQ_SET_INTERFACE:	// TODO use bNumInterfaces
		// there is only one interface (= 0)
		if (pSetup->wValue != 0) {
			return FALSE;
		}
		*piLen = 0;
		break;

	default:
		DBG("Illegal interface req %d\n", pSetup->bRequest);
		return FALSE;
	}

	return TRUE;
}


/**
	Local function to handle a standard endpoint request
		
	@param [in]		pSetup		The setup packet
	@param [in,out]	*piLen		Pointer to data length
	@param [in]		ppbData		Data buffer.

	@return TRUE if the request was handled successfully
 */
static BOOL HandleStdEndPointReq(TSetupPacket	*pSetup, int *piLen, U8 **ppbData)
{
	U8	*pbData = *ppbData;

	switch (pSetup->bRequest) {
	case REQ_GET_STATUS:
		// bit 0 = endpointed halted or not
		pbData[0] = (USBHwEPGetStatus(pSetup->wIndex) & EP_STATUS_STALLED) ? 1 : 0;
		pbData[1] = 0;
		*piLen = 2;
		break;
		
	case REQ_CLEAR_FEATURE:
		if (pSetup->wValue == FEA_ENDPOINT_HALT) {
			// clear HALT by unstalling
			USBHwEPStall(pSetup->wIndex, FALSE);
			break;
		}
		// only ENDPOINT_HALT defined for endpoints
		return FALSE;
	
	case REQ_SET_FEATURE:
		if (pSetup->wValue == FEA_ENDPOINT_HALT) {
			// set HALT by stalling
			USBHwEPStall(pSetup->wIndex, TRUE);
			break;
		}
		// only ENDPOINT_HALT defined for endpoints
		return FALSE;

	case REQ_SYNCH_FRAME:
		DBG("EP req %d not implemented\n", pSetup->bRequest);
		return FALSE;

	default:
		DBG("Illegal EP req %d\n", pSetup->bRequest);
		return FALSE;
	}
	
	return TRUE;
}


/**
	Default handler for standard ('chapter 9') requests
	
	If a custom request handler was installed, this handler is called first.
		
	@param [in]		pSetup		The setup packet
	@param [in,out]	*piLen		Pointer to data length
	@param [in]		ppbData		Data buffer.

	@return TRUE if the request was handled successfully
 */
BOOL USBHandleStandardRequest(TSetupPacket	*pSetup, int *piLen, U8 **ppbData)
{
	// try the custom request handler first
	if ((pfnHandleCustomReq != NULL) && pfnHandleCustomReq(pSetup, piLen, ppbData)) {
		return TRUE;
	}
	
	switch (REQTYPE_GET_RECIP(pSetup->bmRequestType)) {
	case REQTYPE_RECIP_DEVICE:		return HandleStdDeviceReq(pSetup, piLen, ppbData);
	case REQTYPE_RECIP_INTERFACE:	return HandleStdInterfaceReq(pSetup, piLen, ppbData);
	case REQTYPE_RECIP_ENDPOINT: 	return HandleStdEndPointReq(pSetup, piLen, ppbData);
	default: 						return FALSE;
	}
}


/**
	Registers a callback for custom device requests
	
	In USBHandleStandardRequest, the custom request handler gets a first
	chance at handling the request before it is handed over to the 'chapter 9'
	request handler.
	
	This can be used for example in HID devices, where a REQ_GET_DESCRIPTOR
	request is sent to an interface, which is not covered by the 'chapter 9'
	specification.
		
	@param [in]	pfnHandler	Callback function pointer
 */
void USBRegisterCustomReqHandler(TFnHandleRequest *pfnHandler)
{
	pfnHandleCustomReq = pfnHandler;
}

