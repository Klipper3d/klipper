#inlcude "internal.h"

#define EP0IN	0x80
#define EP0OUT	0x00
#define EP1IN	0x81
#define EP1OUT	0x01
#define EP2IN	0x82

#define EPT0_TX_ADDR                     0x40    /*!< usb endpoint 0 tx buffer address offset */
#define EPT0_RX_ADDR                     0x80    /*!< usb endpoint 0 rx buffer address offset */

#define EPT1_TX_ADDR                     0xC0    /*!< usb endpoint 1 tx buffer address offset */
#define EPT1_RX_ADDR                     0x100   /*!< usb endpoint 1 rx buffer address offset */

#define EPT2_TX_ADDR                     0x140   /*!< usb endpoint 2 tx buffer address offset */

static int_fast8_t
usb_write_packet(uint32_t ep, const void *data, uint_fast8_t len);

static int_fast8_t
usb_read_packet(uint32_t ep, void *data, uint_fast8_t max_len);

int_fast8_t
usb_read_bulk_out(void *data, uint_fast8_t max_len)
{
    return usb_read_packet(EP2OUT, data, max_len);
}

int_fast8_t
usb_send_bulk_in(void *data, uint_fast8_t len)
{
    return usb_write_packet(EP5IN, data, len);
}

int_fast8_t
usb_read_ep0(void *data, uint_fast8_t max_len)
{
    return usb_read_packet(EP0OUT, data, max_len);
}

int_fast8_t
usb_read_ep0_setup(void *data, uint_fast8_t max_len)
{
    return usb_read_ep0(data, max_len);
}

int_fast8_t
usb_send_ep0(const void *data, uint_fast8_t len)
{
    return usb_write_packet(EP0IN, data, len);
}

void
usb_stall_ep0(void)
{
//	void usb_ept_stall(usbd_type *usbx, usb_ept_info *ept_info)

  	if(ept_info->inout == DATA_TRANS_IN)
  	{
    	USB_SET_TXSTS(ept_info->eptn, USB_TX_STALL)
  	}
  	else
  	{
    	USB_SET_RXSTS(ept_info->eptn, USB_RX_STALL)
  	}
}

void
usb_set_address(uint_fast8_t addr)
{
    usb_irq_disable();
    sie_cmd_write(SIE_CMD_SET_ADDRESS, addr | (1<<7));
    usb_irq_enable();
    usb_send_ep0(NULL, 0);
}


void
usb_set_configure(void)
{
    usb_irq_disable();
    realize_endpoint(EP1IN, USB_CDC_EP_ACM_SIZE);
    realize_endpoint(EP2OUT, USB_CDC_EP_BULK_OUT_SIZE);
    realize_endpoint(EP5IN, USB_CDC_EP_BULK_IN_SIZE);
    sie_cmd_write(SIE_CMD_CONFIGURE, 1);
    usb_irq_enable();
}

void
usb_request_bootloader(void)
{
    if (!CONFIG_SMOOTHIEWARE_BOOTLOADER)
        return;
    // Disable USB and pause for 5ms so host recognizes a disconnect
    irq_disable();
    sie_cmd_write(SIE_CMD_SET_DEVICE_STATUS, 0);
    udelay(5000);
    // The "LPC17xx-DFU-Bootloader" will enter the bootloader if the
    // watchdog timeout flag is set.
    LPC_WDT->WDMOD = 0x07;
    NVIC_SystemReset();
}

void
USBFS_L_CAN1_RX0_IRQHandler(void)
{
	switch(USB->intsts_bit.pet_num)
	{
		case 0:
				if(USB->intsts_bit.inout == 1)
				{
					//ep0 out event
					if((USB->ept_bit[0].rxtc == 1) && (USB->ept_bit[0].setuptc == 0))
					{
						//ep0 out event
					}
					else
					{
						//epo setup event
					}
				}
				else
				{
					//ep0 in event
				}
				break;
		case 1:
				
				if(USB->intsts_bit.inout == 1)
				{
					//ep1 out event
				}
				else
				{
					//ep1 in event
				}

				break;
		case 2:
				if(USB->intsts_bit.inout == 0)
				{
					//ep2 in event
				}
				break;
		default:
				break;
	}
}


void
usbserial_init(void)
{
	//config usb clock. using hick from acc 48MHz
	CRM->misc1_bit.hickdiv = CRM_HICK48_NODIV;

	CRM->misc3_bit.hick_to_sclk = CRM_HICK_SCLK_48MHZ;

	CRM->misc3_bit.hick_to_usb = CRM_USB_CLOCK_SOURCE_HICK;

	//enable the acc clock
	CRM_REG(CRM_ACC_PERIPH_CLOCK) |= CRM_REG_BIT(1);

	//update the c1\c2\c3 value
	ACC->c1 = 7980;

	ACC->c2 = 8000;

	ACC->c3 = 8020;

	//open acc calibartion
	ACC->ctrl1_bit.entrim = ACC_CAL_HICKTRIM;

	ACC->ctrl1_bit.calon = 1;

	//enable usb clock
	CRM_REG(CRM_USB_PERIPH_CLOCK) |= CRM_REG_BIT(CRM_USB_PERIPH_CLOCK);

	//cfg EP0IN
	USB_SET_EPT_ADDRESS(EP0IN & 0x0F, EP0IN); 		/*!< set ep0in addr.*/

	USB_SET_TRANS_TYPE(EP0IN & 0x0F, EPT_CONTROL_TYPE);	/*!< set ep0in type.*/

	/* set in endpoint tx offset address */
    //USB_SET_TX_ADDRESS(EP0IN & 0x0F, EPT0_TX_ADDR);
	(*(uint32_t *)((USB->buftbl) * 2 + USB_PACKET_BUFFER_ADDRESS) = EPTO_TX_ADDR;
    /* clear in endpoint data toggle */
    USB_CLEAR_TXDTS(EP0IN & 0x0F);

    /* set endpoint transmission status: nak */
    USB_SET_TXSTS(EP0IN & 0x0F, USB_TX_NAK);
	
	//cfg EP0OUT
	USB_SET_EPT_ADDRESS(EP0OUT & 0x0F, EP0OUT); 		/*!< set ep0in addr.*/

	USB_SET_TRANS_TYPE(EP0OUT & 0x0F, EPT_CONTROL_TYPE);	/*!< set ep0in type.*/

	/* set out endpoint rx offset address */
    //USB_SET_RX_ADDRESS(EP0OUT & 0x0F, EPT0_RX_ADDR);
	*(uint32_t *)((USB->buftbl + 4) * 2 + USB_PACKET_BUFFER_ADDRESS) = EPT0_RX_ADDR;
    /* clear out endpoint data toggle */
    USB_CLEAR_RXDTS(EP0OUT & 0x0F);

	USB_SET_RXLEN(EP0OUT & 0x0F, 0x40);

    /* set endpoint transmission status: nak */
    USB_SET_RXSTS(EP0OUT & 0x0F, USB_RX_VALLD);

	//enable usb interrupt
	USB->ctrl_bit.csrst = 0;

	USB->intsts = 0;

	USB->buftbl = USB_BUFFER_TABLE_ADDRESS;

	USB->devaddr = 0x80;

	USB->ctrl |= (USB_SOF_INT | USB_RST_INT | USB_SP_INT | USB_WK_INT | USB_TC_INT);

	armcm_enable_irq(USBFS_L_CAN1_RX0_IRQHandler, USBFS_L_CAN1_RX0_IRQn, 1);

	//connect usb device
	USB->ctrl_bit.disusb = 0;

	USB->cfg_bit.puo = 0;
}
DECL_INIT(usbserial_init);

void
usbserial_shutdown(void)
{
	USB->ctrl &= ~(USB_SOF_INT | USB_RST_INT | USB_SP_INT | USB_WK_INT | USB_TC_INT);
}
DECL_SHUTDOWN(usbserial_shutdown);

