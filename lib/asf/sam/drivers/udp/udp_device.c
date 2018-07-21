/**
 * \file
 *
 * \brief USB Device Driver for UDP. Compliant with common UDD driver.
 *
 * Copyright (c) 2012-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include "conf_usb.h"
#include "sysclk.h"
#if SAMG55
#include "matrix.h"
#endif
#include "udd.h"
#include "udp_device.h"
#include <string.h>

#ifndef UDD_NO_SLEEP_MGR
#  include "sleep.h"
#  include "sleepmgr.h"
#endif

#if !(SAM3S || SAM4S || SAM4E || SAMG55)
#  error The current UDP Device Driver supports only SAM3S, SAM4S, SAM4E and SAMG55 devices.
#endif

#ifndef UDD_USB_INT_LEVEL
#  define UDD_USB_INT_LEVEL 5 // By default USB interrupt have low priority
#endif

/**
 * \ingroup udd_group
 * \defgroup udd_udp_group USB Device Port Driver
 *
 * \section UDP_CONF UDP Custom configuration
 * The following UDP driver configuration must be included in the conf_usb.h
 * file of the application.
 *
 * UDD_USB_INT_LEVEL<br>
 * Option to change the interrupt priority (0 to 15) by default 5 (recommended).
 *
 * UDD_USB_INT_FUN<br>
 * Option to fit interrupt function to what defined in exception table.
 *
 * UDD_NO_SLEEP_MGR<br>
 * Feature to work without sleep manager module.
 * Default not defined.
 * Note that with this feature defined sleep manager must not be used in
 * application.
 *
 * \section Callbacks management
 * The USB driver is fully managed by interrupt and does not request periodic
 * task. Thereby, the USB events use callbacks to transfer the information.
 * The callbacks are declared in static during compilation or in variable during
 * code execution.
 *
 * Static declarations defined in conf_usb.h:
 * - UDC_VBUS_EVENT(bool b_present)<br>
 *   To signal Vbus level change
 * - UDC_SUSPEND_EVENT()<br>
 *   Called when USB bus enter in suspend mode
 * - UDC_RESUME_EVENT()<br>
 *   Called when USB bus is wakeup
 * - UDC_SOF_EVENT()<br>
 *   Called for each received SOF, Note: Each 1ms in HS/FS mode only.
 *
 * Dynamic callbacks, called "endpoint job" , are registered
 * in udd_ep_job_t structure via the following functions:
 * - udd_ep_run()<br>
 *   To call it when a transfer is finish
 * - udd_ep_wait_stall_clear()<br>
 *   To call it when a endpoint halt is disabled
 *
 * \section Power mode management
 * The Sleep modes authorized :
 * - in USB IDLE state, the UDP needs of USB clock and authorizes up to sleep mode WFI.
 * - in USB SUSPEND state, the UDP no needs USB clock and authorizes up to sleep mode WAIT.
 * @{
 */

// Check USB Device configuration
#ifndef USB_DEVICE_EP_CTRL_SIZE
#  error USB_DEVICE_EP_CTRL_SIZE not defined
#endif
#ifndef USB_DEVICE_MAX_EP
#  error USB_DEVICE_MAX_EP not defined
#endif
#if USB_DEVICE_MAX_EP > (UDD_MAX_PEP_NB-1) // USB_DEVICE_MAX_EP does not include control endpoint
#  error USB_DEVICE_MAX_EP is too high and not supported by this part
#endif
#ifdef USB_DEVICE_HS_SUPPORT
#  error The High speed mode is not supported on this part, please remove USB_DEVICE_HS_SUPPORT in conf_usb.h
#endif
#ifdef USB_DEVICE_LOW_SPEED
#  error The Low speed mode is not supported on this part, please remove USB_DEVICE_LOW_SPEED in conf_usb.h
#endif

#ifndef UDD_USB_INT_FUN
#  define UDD_USB_INT_FUN UDP_Handler
#endif

#ifndef UDC_VBUS_EVENT
#  define UDC_VBUS_EVENT(present)
#endif

/**
 * \name Power management routine.
 */
//@{

#ifndef UDD_NO_SLEEP_MGR

//! Definition of sleep levels
#define UDP_SLEEP_MODE_USB_SUSPEND  SLEEPMGR_ACTIVE
#if SAMG55
#define UDP_SLEEP_MODE_USB_IDLE     SLEEPMGR_ACTIVE
#else
#define UDP_SLEEP_MODE_USB_IDLE     SLEEPMGR_SLEEP_WFI
#endif

//! State of USB line
static bool udd_b_idle;


/*! \brief Authorize or not the CPU powerdown mode
 *
 * \param b_enable   true to authorize idle mode
 */
static void udd_sleep_mode(bool b_idle)
{
	if (!b_idle && udd_b_idle) {
		sleepmgr_unlock_mode(UDP_SLEEP_MODE_USB_IDLE);
	}
	if (b_idle && !udd_b_idle) {
		sleepmgr_lock_mode(UDP_SLEEP_MODE_USB_IDLE);
	}
	udd_b_idle = b_idle;
}
#else

static void udd_sleep_mode(bool b_idle)
{
	UNUSED(b_idle);
}

#endif // UDD_NO_SLEEP_MGR

//@}

/**
 * \name VBus monitor routine
 */
//@{

#if UDD_VBUS_IO

# if !defined(UDD_NO_SLEEP_MGR) && !defined(USB_VBUS_WKUP)
/* Lock to SLEEPMGR_SLEEP_WFI if VBus not connected */
static bool b_vbus_sleep_lock = false;
/**
 * Lock sleep mode for VBus PIO pin change detection
 */
static void udd_vbus_monitor_sleep_mode(bool b_lock)
{
	if (b_lock && !b_vbus_sleep_lock) {
		b_vbus_sleep_lock = true;
		sleepmgr_lock_mode(SLEEPMGR_SLEEP_WFI);
	}
	if (!b_lock && b_vbus_sleep_lock) {
		b_vbus_sleep_lock = false;
		sleepmgr_unlock_mode(SLEEPMGR_SLEEP_WFI);
	}
}
# else
#  define udd_vbus_monitor_sleep_mode(lock)
# endif

/**
 * USB VBus pin change handler
 */
static void udd_vbus_handler(uint32_t id, uint32_t mask)
{
	if (USB_VBUS_PIO_ID != id || USB_VBUS_PIO_MASK != mask) {
		return;
	}
	/* PIO interrupt status has been cleared, just detect level */
	bool b_vbus_high = Is_udd_vbus_high();
	if (b_vbus_high) {
		udd_ack_vbus_interrupt(true);
		udd_vbus_monitor_sleep_mode(false);
		udd_attach();
	} else {
		udd_ack_vbus_interrupt(false);
		udd_vbus_monitor_sleep_mode(true);
		udd_detach();
	}
	UDC_VBUS_EVENT(b_vbus_high);
}

#endif

//@}


/**
 * \name Control endpoint low level management routine.
 *
 * This function performs control endpoint management.
 * It handle the SETUP/DATA/HANDSHAKE phases of a control transaction.
 */
//@{

//! Global variable to give and record information about setup request management
COMPILER_WORD_ALIGNED udd_ctrl_request_t udd_g_ctrlreq;

//! Bit definitions about endpoint control state machine for udd_ep_control_state
typedef enum {
	UDD_EPCTRL_SETUP = 0,                  //!< Wait a SETUP packet
	UDD_EPCTRL_DATA_OUT = 1,               //!< Wait a OUT data packet
	UDD_EPCTRL_DATA_IN = 2,                //!< Wait a IN data packet
	UDD_EPCTRL_HANDSHAKE_WAIT_IN_ZLP = 3,  //!< Wait a IN ZLP packet
	UDD_EPCTRL_HANDSHAKE_WAIT_OUT_ZLP = 4, //!< Wait a OUT ZLP packet
	UDD_EPCTRL_STALL_REQ = 5               //!< STALL enabled on IN & OUT
} udd_ctrl_ep_state_t;

//! State of the endpoint control management
static udd_ctrl_ep_state_t udd_ep_control_state;

//! Total number of data received/sent during data packet phase with previous payload buffers
static uint16_t udd_ctrl_prev_payload_nb_trans;

//! Number of data received/sent to/from udd_g_ctrlreq.payload buffer
static uint16_t udd_ctrl_payload_nb_trans;

/**
 * \brief Reset control endpoint
 *
 * Called after a USB line reset or when UDD is enabled
 */
static void udd_reset_ep_ctrl(void);

/**
 * \brief Reset control endpoint management
 *
 * Called after a USB line reset or at the end of SETUP request (after ZLP)
 */
static void udd_ctrl_init(void);

//! \brief Managed reception of SETUP packet on control endpoint
static void udd_ctrl_setup_received(void);

//! \brief Managed reception of IN packet on control endpoint
static void udd_ctrl_in_sent(void);

//! \brief Managed reception of OUT packet on control endpoint
static void udd_ctrl_out_received(void);

//! \brief Managed stall event of IN/OUT packet on control endpoint
static void udd_ctrl_stall_data(void);

//! \brief Send a ZLP IN on control endpoint
static void udd_ctrl_send_zlp_in(void);

//! \brief Send a ZLP OUT on control endpoint
static void udd_ctrl_send_zlp_out(void);

//! \brief Call callback associated to setup request
static void udd_ctrl_endofrequest(void);


/**
 * \brief Main interrupt routine for control endpoint
 *
 * This switches control endpoint events to correct sub function.
 *
 * \return \c 1 if an event about control endpoint is occurred, otherwise \c 0.
 */
static bool udd_ctrl_interrupt(void);

//@}


/**
 * \name Management of bulk/interrupt/isochronous endpoints
 *
 * The UDD manages the data transfer on endpoints:
 * - Start data transfer on endpoint with USB Device DMA
 * - Send a ZLP packet if requested
 * - Call callback registered to signal end of transfer
 * The transfer abort and stall feature are supported.
 */
//@{
#if (0!=USB_DEVICE_MAX_EP)

//! Structure definition about job registered on an endpoint
typedef struct {
	union {
		//! Callback to call at the end of transfer
		udd_callback_trans_t call_trans;

		//! Callback to call when the endpoint halt is cleared
		udd_callback_halt_cleared_t call_nohalt;
	};
	//! Buffer located in internal RAM to send or fill during job
	uint8_t *buf;
	//! Size of buffer to send or fill
	iram_size_t buf_size;
	//! Total number of data transfered on endpoint
	iram_size_t buf_cnt;
	//! Maximum packet size for the endpoint
	uint32_t size:10;
	//! Current reception bank or current number of filled transmit banks
	uint32_t bank:2;
	//! A job is registered on this endpoint
	uint32_t busy:1;
	//! A stall is requested for this job on endpoint IN
	uint32_t b_stall_requested:1;
	//! A short packet is requested for this job on endpoint IN
	uint32_t b_shortpacket:1;
	//! Registered buffer on this endpoint reach its end
	uint32_t b_buf_end:1;
} udd_ep_job_t;

//! Job status to finish the job
#define UDD_EP_TRANSFER_BUFFER_END  0x2

//! Array to register a job on bulk/interrupt/isochronous endpoint
static udd_ep_job_t udd_ep_job[USB_DEVICE_MAX_EP];

//! \brief Reset all job table
static void udd_ep_job_table_reset(void);

//! \brief Abort all endpoint jobs on going
static void udd_ep_job_table_kill(void);

/**
 * \brief Fill banks and send them
 *
 * \param ep         endpoint number without direction flag
 * \param b_tx       true if data is ready to send
 *
 * \return true if data buffer is in transmitting
 */
static bool udd_ep_in_sent(udd_ep_id_t ep, bool b_tx);

/**
 * \brief Store received banks
 *
 * \param ep         endpoint number without direction flag
 */
static void udd_ep_out_received(udd_ep_id_t ep);

/**
 * \brief Abort endpoint job on going
 *
 * \param ep         endpoint number of job to abort
 */
static void udd_ep_abort_job(udd_ep_id_t ep);

/**
 * \brief Call the callback associated to the job which is finished
 *
 * \param ptr_job  job to complete
 * \param status   job status
 */
static void udd_ep_finish_job(udd_ep_job_t * ptr_job, int status,
		uint8_t ep_num);

/**
 * \brief Ack OUT received event.
 *
 * This acks the right bank when multiple banks are used.
 *
 * \param ep         endpoint number without direction flag
 */
static void udd_ep_ack_out_received(udd_ep_id_t ep);

/**
 * \brief Fill transmit data into FIFO
 *
 * \param ep         endpoint number without direction flag
 *
 * \return true if a short packet has been written
 */
static bool udd_ep_write_fifo(udd_ep_id_t ep);

/**
 * \brief Main interrupt routine for bulk/interrupt/isochronous endpoints
 *
 * This switches endpoint events to correct sub function.
 *
 * \return \c 1 if an event about bulk/interrupt/isochronous endpoints has occurred, otherwise \c 0.
 */
static bool udd_ep_interrupt(void);

#endif // (0!=USB_DEVICE_MAX_EP)
//@}


//--------------------------------------------------------
//--- INTERNAL ROUTINES TO MANAGED GLOBAL EVENTS

/**
 * \internal
 * \brief Function called by UDP interrupt to manage USB Device interrupts
 *
 * USB Device interrupt events are split in three parts:
 * - USB line events (SOF, reset, suspend, resume, wakeup)
 * - control endpoint events (setup reception, end of data transfer, underflow, overflow, stall)
 * - bulk/interrupt/isochronous endpoints events (end of data transfer)
 *
 */
ISR(UDD_USB_INT_FUN)
{
#ifndef UDD_NO_SLEEP_MGR
	/* For fast wakeup clocks restore
	 * In WAIT mode, clocks are switched to FASTRC.
	 * After wakeup clocks should be restored, before that ISR should not
	 * be served.
	 */
	if (!pmc_is_wakeup_clocks_restored() && !Is_udd_suspend()) {
		cpu_irq_disable();
		return;
	}
#endif
	/* The UDP peripheral clock in the Power Management Controller (PMC)
	   must be enabled before any read/write operations to the UDP registers
	   including the UDP_TXVC register. */
	udd_enable_periph_ck();

	if (Is_udd_sof_interrupt_enabled() && Is_udd_sof()) {
		udd_ack_sof();
		udc_sof_notify();
#ifdef UDC_SOF_EVENT
		UDC_SOF_EVENT();
#endif
		goto udd_interrupt_sof_end;
	}

	if (udd_ctrl_interrupt()) {
		goto udd_interrupt_end; // Interrupt acked by control endpoint managed
	}

#if (0 != USB_DEVICE_MAX_EP)
	if (udd_ep_interrupt()) {
		goto udd_interrupt_end; // Interrupt acked by bulk/interrupt/isochronous endpoint managed
	}
#endif

	if ((Is_udd_wake_up_interrupt_enabled() && Is_udd_wake_up()) ||
		(Is_udd_resume_interrupt_enabled() && Is_udd_resume()) ||
		(Is_udd_ext_resume_interrupt_enabled() && Is_udd_ext_resume())) {
		// Ack wakeup interrupt and enable suspend interrupt
		udd_ack_wakeups();
		// Do resume operations
		udd_disable_wakeups();

		udd_sleep_mode(true); // Enter in IDLE mode
#ifdef UDC_RESUME_EVENT
		UDC_RESUME_EVENT();
#endif
		udd_ack_suspend();
		udd_enable_suspend_interrupt();
		udd_enable_sof_interrupt();
		goto udd_interrupt_end;
	}

	if (Is_udd_suspend_interrupt_enabled() && Is_udd_suspend()) {
		// Ack suspend interrupt and enable resume interrupt
		udd_ack_suspend();
		udd_disable_suspend_interrupt();
		udd_enable_wake_up_interrupt();
		udd_enable_resume_interrupt();
		udd_enable_ext_resume_interrupt();
		udd_disable_periph_ck();

		udd_sleep_mode(false); // Enter in SUSPEND mode
#ifdef UDC_SUSPEND_EVENT
		UDC_SUSPEND_EVENT();
#endif
		goto udd_interrupt_end;
	}
	if (Is_udd_reset()) {
		// USB bus reset detection
		udd_ack_reset();

		// Abort all jobs on-going
#if (0 != USB_DEVICE_MAX_EP)
		udd_ep_job_table_kill();
#endif
		// Reset USB Device Stack Core
		udc_reset();
		// Reset device state
		udd_disable_address_state();
		udd_disable_configured_state();
		// Reset endpoint control
		udd_reset_ep_ctrl();
		// Reset endpoint control management
		udd_ctrl_init();
		// After a USB reset, the suspend and SOF interrupt masks has been reseted
		// Thus, re-enable these
		udd_enable_suspend_interrupt();
		udd_enable_sof_interrupt();
		goto udd_interrupt_end;
	}

udd_interrupt_end:
udd_interrupt_sof_end:
	return;
}


bool udd_include_vbus_monitoring(void)
{
#if UDD_VBUS_IO
	return true;
#else
	return false;
#endif
}


void udd_enable(void)
{
	irqflags_t flags;

	flags = cpu_irq_save();

#if SAMG55
	matrix_set_usb_device();
#endif

	// Enable USB hardware
	udd_enable_periph_ck();
	sysclk_enable_usb();
	// Cortex, uses NVIC, no need to register IRQ handler
	NVIC_SetPriority((IRQn_Type) ID_UDP, UDD_USB_INT_LEVEL);
	NVIC_EnableIRQ((IRQn_Type) ID_UDP);

	// Reset internal variables
#if (0!=USB_DEVICE_MAX_EP)
	udd_ep_job_table_reset();
#endif

	// Always authorize asynchronous USB interrupts to exit of sleep mode
	pmc_set_fast_startup_input(PMC_FSMR_USBAL);

#ifndef UDD_NO_SLEEP_MGR
	// Initialize the sleep mode authorized for the USB suspend mode
	udd_b_idle = false;
	sleepmgr_lock_mode(UDP_SLEEP_MODE_USB_SUSPEND);
#endif

#if UDD_VBUS_IO
	/* Initialize VBus monitor */
	udd_vbus_init(udd_vbus_handler);
	udd_vbus_monitor_sleep_mode(true);
	/* Force Vbus interrupt when Vbus is always high
	 * This is possible due to a short timing between a Host mode stop/start.
	 */
	if (Is_udd_vbus_high()) {
		udd_vbus_handler(USB_VBUS_PIO_ID, USB_VBUS_PIO_MASK);
	}
#else
#  ifndef USB_DEVICE_ATTACH_AUTO_DISABLE
	udd_attach();
#  endif
#endif

	cpu_irq_restore(flags);
}


void udd_disable(void)
{
	irqflags_t flags;
	flags = cpu_irq_save();

	udd_detach();

#ifndef UDD_NO_SLEEP_MGR
	sleepmgr_unlock_mode(UDP_SLEEP_MODE_USB_SUSPEND);
#endif

# if UDD_VBUS_IO
	udd_vbus_monitor_sleep_mode(false);
# endif

	cpu_irq_restore(flags);
}


void udd_attach(void)
{
	irqflags_t flags;
	flags = cpu_irq_save();

	// At startup the USB bus state is unknown,
	// therefore the state is considered IDLE to not miss any USB event
	udd_sleep_mode(true);

	// Enable peripheral clock and USB clock
	udd_enable_periph_ck();

	// Authorize attach if VBus is present
	udd_enable_transceiver();
	udd_attach_device();

	// Enable USB line events
	udd_enable_suspend_interrupt();
	udd_enable_wake_up_interrupt();
	udd_enable_resume_interrupt();
	udd_enable_ext_resume_interrupt();
	udd_enable_sof_interrupt();

	cpu_irq_restore(flags);
}


void udd_detach(void)
{
	// Disable transceiver
	udd_disable_transceiver();
	// Detach device from the bus
	udd_detach_device();
	udd_sleep_mode(false);
}


bool udd_is_high_speed(void)
{
	return false;
}


void udd_set_address(uint8_t address)
{
	udd_disable_address_state();
	udd_disable_address();
	if (address) {
		udd_configure_address(address);
		udd_enable_address();
		udd_enable_address_state();
	}
}


uint8_t udd_getaddress(void)
{
	if (Is_udd_address_state_enabled())
		return udd_get_configured_address();
	return 0;
}


uint16_t udd_get_frame_number(void)
{
	return udd_frame_number();
}


uint16_t udd_get_micro_frame_number(void)
{
	return 0;
}


void udd_send_remotewakeup(void)
{
#ifndef UDD_NO_SLEEP_MGR
	if (!udd_b_idle)
#endif
	{
		udd_sleep_mode(true); // Enter in IDLE mode
		udd_enable_periph_ck();
		udd_initiate_remote_wake_up();
	}
}


void udd_set_setup_payload( uint8_t *payload, uint16_t payload_size )
{
	udd_g_ctrlreq.payload = payload;
	udd_g_ctrlreq.payload_size = payload_size;
}


#if (0!=USB_DEVICE_MAX_EP)
bool udd_ep_alloc(udd_ep_id_t ep, uint8_t bmAttributes,
		uint16_t MaxEndpointSize)
{
	udd_ep_job_t *ptr_job;
	bool b_dir_in;
	bool b_iso;
	b_dir_in = ep & USB_EP_DIR_IN;
	b_iso = (bmAttributes&USB_EP_TYPE_MASK) == USB_EP_TYPE_ISOCHRONOUS;
	ep = ep & USB_EP_ADDR_MASK;

	if (ep > USB_DEVICE_MAX_EP) {
		return false;
	}
	if (Is_udd_endpoint_enabled(ep)) {
		return false;
	}

	// Check parameters
	if (b_iso && (!udd_is_endpoint_support_iso(ep))) {
		return false;
	}
	if (MaxEndpointSize > udd_get_endpoint_size_max(ep)) {
		return false;
	}
	ptr_job = &udd_ep_job[ep - 1];

	// Set endpoint size
	ptr_job->size = MaxEndpointSize;
	ptr_job->b_buf_end = false;
	ptr_job->b_stall_requested = false;
	if (b_dir_in) {
		// No data buffered in FIFO
		ptr_job->bank = 0;
	}

	// Reset FIFOs
	udd_reset_endpoint(ep);
	// Set configuration of new endpoint
	udd_configure_endpoint(ep,
		(b_dir_in ? ((bmAttributes&USB_EP_TYPE_MASK) | 0x4) : (bmAttributes&USB_EP_TYPE_MASK)),
		0);
	return true;
}


void udd_ep_free(udd_ep_id_t ep)
{
	uint8_t ep_index = ep & USB_EP_ADDR_MASK;
	if (USB_DEVICE_MAX_EP < ep_index) {
		return;
	}
	udd_disable_endpoint(ep_index);
	udd_ep_abort_job(ep);
}


bool udd_ep_is_halted(udd_ep_id_t ep)
{
	uint8_t ep_index = ep & USB_EP_ADDR_MASK;
	udd_ep_job_t *ptr_job = &udd_ep_job[ep_index - 1];
	if (USB_DEVICE_MAX_EP < ep_index) {
		return false;
	}
	return ptr_job->b_stall_requested ||
			Is_udd_endpoint_stall_pending(ep & USB_EP_ADDR_MASK);
}


bool udd_ep_set_halt(udd_ep_id_t ep)
{
	bool b_dir_in = ep & USB_EP_DIR_IN;
	uint8_t ep_index = ep & USB_EP_ADDR_MASK;
	udd_ep_job_t *ptr_job = &udd_ep_job[ep_index - 1];
	irqflags_t flags;
	if (USB_DEVICE_MAX_EP < ep_index) {
		return false;
	}
	flags = cpu_irq_save();
	if (b_dir_in && (Is_udd_transmit_ready(ep_index)
				|| ptr_job->bank > 1)) {
		// Halt until banks sent
		ptr_job->b_stall_requested = true;
		udd_enable_endpoint_interrupt(ep_index);
		cpu_irq_restore(flags);
		return true;
	} else {
		// Stall endpoint
		udd_enable_stall_handshake(ep_index);
		udd_enable_endpoint_interrupt(ep_index);
		cpu_irq_restore(flags);
	}
	return true;
}


bool udd_ep_clear_halt(udd_ep_id_t ep)
{
	udd_ep_job_t *ptr_job;

	ep &= USB_EP_ADDR_MASK;
	if (USB_DEVICE_MAX_EP < ep)
		return false;
	ptr_job = &udd_ep_job[ep - 1];

	ptr_job->b_stall_requested = false;
	if (Is_udd_endpoint_stall_requested(ep)) {
		// Remove stall
		udd_disable_stall_handshake(ep);
		// Reset FIFO and data toggle (after stall cleared)
		udd_reset_endpoint(ep);
		// Clear stall status
		udd_ack_stall(ep);
		// If a job is register on clear halt action
		// then execute callback
		if (ptr_job->busy == true) {
			ptr_job->busy = false;
			ptr_job->call_nohalt();
		}
	}
	return true;
}


bool udd_ep_run(udd_ep_id_t ep, bool b_shortpacket,
		uint8_t * buf, iram_size_t buf_size,
		udd_callback_trans_t callback)
{
	udd_ep_job_t *ptr_job;
	irqflags_t flags;
	bool b_dir_in = ep & USB_EP_DIR_IN;

	ep &= USB_EP_ADDR_MASK;
	if (USB_DEVICE_MAX_EP < ep) {
		return false;
	}
	// Get job about endpoint
	ptr_job = &udd_ep_job[ep - 1];

	if ((!Is_udd_endpoint_enabled(ep))
			|| ptr_job->b_stall_requested
			|| Is_udd_endpoint_stall_requested(ep)) {
		return false; // Endpoint is halted
	}

	flags = cpu_irq_save();
	if (ptr_job->busy == true) {
		cpu_irq_restore(flags);
		return false; // Job already on going
	}
	ptr_job->busy = true;
	cpu_irq_restore(flags);

	// No job running. Let's setup a new one.
	ptr_job->buf = buf;
	ptr_job->buf_size = buf_size;
	ptr_job->buf_cnt = 0;
	ptr_job->call_trans = callback;
	ptr_job->b_shortpacket = b_shortpacket || (buf_size == 0);
	ptr_job->b_buf_end = false;

	flags = cpu_irq_save();
	udd_enable_endpoint_interrupt(ep);
	// Request first transfer
	if (b_dir_in) {
		if (Is_udd_in_pending(ep)) {
			// Append more data (handled in interrupt service)
		} else {
			// Start new, try to fill 1~2 banks before handling status
			if (udd_ep_in_sent(ep, true)) {
				// Over one bank
				udd_ep_in_sent(ep, false);
			} else {
				// Less than one bank
			}
		}
	} else {
		// Waiting for OUT received interrupt
	}
	cpu_irq_restore(flags);

	return true;
}


void udd_ep_abort(udd_ep_id_t ep)
{
	bool b_dir_in = ep & USB_EP_DIR_IN;
	irqflags_t flags;

	ep &= USB_EP_ADDR_MASK;
	if (USB_DEVICE_MAX_EP < ep)
		return;

	// Disable interrupts
	flags = cpu_irq_save();
	udd_disable_endpoint_interrupt(ep);
	cpu_irq_restore(flags);
	// Clear pending statuses
	if (b_dir_in) {
		// Kill banks
		if (Is_udd_transmit_ready(ep)) {
			udd_kill_data_in_fifo(ep,
					udd_get_endpoint_bank_max_nbr(ep)>1);
		}
		udd_ack_in_sent(ep);
		// Reset number of buffered banks
		udd_ep_job[ep - 1].bank = 0;
	} else {
		// Clear all pending banks statuses
		while(Is_udd_any_bank_received(ep)) {
			udd_ep_ack_out_received(ep);
		}
	}
	// Reset FIFO and data toggle
	udd_reset_endpoint(ep);
	// Abort job
	udd_ep_abort_job(ep);
}


bool udd_ep_wait_stall_clear(udd_ep_id_t ep,
		udd_callback_halt_cleared_t callback)
{
	udd_ep_job_t *ptr_job;

	ep &= USB_EP_ADDR_MASK;
	if (USB_DEVICE_MAX_EP < ep) {
		return false;
	}

	ptr_job = &udd_ep_job[ep - 1];

	if (!Is_udd_endpoint_enabled(ep)) {
		return false; // Endpoint not enabled
	}

	// Wait clear halt endpoint
	if (ptr_job->busy == true) {
		return false; // Job already on going
	}

	if (Is_udd_endpoint_stall_requested(ep)
			|| ptr_job->b_stall_requested) {
		// Endpoint halted then registers the callback
		ptr_job->busy = true;
		ptr_job->call_nohalt = callback;
	} else {
		// endpoint not halted then call directly callback
		callback();
	}
	return true;
}
#endif // (0!=USB_DEVICE_MAX_EP)


//--------------------------------------------------------
//--- INTERNAL ROUTINES TO MANAGED THE CONTROL ENDPOINT

static void udd_reset_ep_ctrl(void)
{
	irqflags_t flags;

	// Reset USB address to 0
	udd_enable_address();
	udd_configure_address(0);
	// Alloc and configure control endpoint in OUT direction
	udd_configure_endpoint(0, USB_EP_TYPE_CONTROL, 0);
	udd_enable_endpoint(0);

	flags = cpu_irq_save();
	udd_enable_endpoint_interrupt(0);
	cpu_irq_restore(flags);
}

static void udd_ctrl_init(void)
{
	udd_g_ctrlreq.callback = NULL;
	udd_g_ctrlreq.over_under_run = NULL;
	udd_g_ctrlreq.payload_size = 0;
	udd_ep_control_state = UDD_EPCTRL_SETUP;
}


static void udd_ctrl_setup_received(void)
{
	uint8_t i;

	if (UDD_EPCTRL_SETUP != udd_ep_control_state) {
		// May be a hidden DATA or ZLP phase
		// or protocol abort
		udd_ctrl_endofrequest();

		// Reinitializes control endpoint management
		udd_ctrl_init();
	}
	// Fill setup request structure
	if (8 != udd_byte_count(0)) {
		udd_ack_setup_received(0);
		udd_ctrl_stall_data();
		return; // Error data number doesn't correspond to SETUP packet
	}
	for (i = 0; i < 8; i++) {
		((uint8_t *) & udd_g_ctrlreq.req)[i] =
			udd_endpoint_fifo_read(0);
	}
	// Manage LSB/MSB to fit with CPU usage
	udd_g_ctrlreq.req.wValue = le16_to_cpu(udd_g_ctrlreq.req.wValue);
	udd_g_ctrlreq.req.wIndex = le16_to_cpu(udd_g_ctrlreq.req.wIndex);
	udd_g_ctrlreq.req.wLength = le16_to_cpu(udd_g_ctrlreq.req.wLength);

	// Decode setup request
	if (udc_process_setup() == false) {
		// Setup request unknown then stall it
		udd_ack_setup_received(0);
		udd_ctrl_stall_data();
		return;
	}

	if (Udd_setup_is_in()) {
		// Set DIR
		udd_set_endpoint_direction_in(0);
		udd_ack_setup_received(0);
		// IN data phase requested
		udd_ctrl_prev_payload_nb_trans = 0;
		udd_ctrl_payload_nb_trans = 0;
		udd_ep_control_state = UDD_EPCTRL_DATA_IN;
		udd_ctrl_in_sent(); // Send first data transfer
	} else {
		udd_ack_setup_received(0);
		if (0 == udd_g_ctrlreq.req.wLength) {
			// No data phase requested
			// Send IN ZLP to ACK setup request
			udd_ctrl_send_zlp_in();
			return;
		}
		// OUT data phase requested
		udd_ctrl_prev_payload_nb_trans = 0;
		udd_ctrl_payload_nb_trans = 0;
		udd_ep_control_state = UDD_EPCTRL_DATA_OUT;
	}
}


static void udd_ctrl_in_sent(void)
{
	static bool b_shortpacket = false;
	uint16_t nb_remain;
	uint8_t i;
	uint8_t *ptr_src;
	irqflags_t flags;

	if (UDD_EPCTRL_HANDSHAKE_WAIT_IN_ZLP == udd_ep_control_state) {
		// Ack
		udd_ack_in_sent(0);
		// ZLP on IN is sent, then valid end of setup request
		udd_ctrl_endofrequest();
		// Reinitializes control endpoint management
		udd_ctrl_init();
		return;
	}
	Assert(udd_ep_control_state == UDD_EPCTRL_DATA_IN);

	nb_remain = udd_g_ctrlreq.payload_size - udd_ctrl_payload_nb_trans;
	if (0 == nb_remain) {
		// All content of current buffer payload are sent
		// Update number of total data sending by previous payload buffer
		udd_ctrl_prev_payload_nb_trans += udd_ctrl_payload_nb_trans;
		if ((udd_g_ctrlreq.req.wLength == udd_ctrl_prev_payload_nb_trans)
				|| b_shortpacket) {
			// All data requested are transfered or a short packet has been sent
			// then it is the end of data phase.
			// Generate an OUT ZLP for handshake phase.
			udd_ctrl_send_zlp_out();
			udd_ack_in_sent(0);
			return;
		}
		// Need of new buffer because the data phase is not complete
		if ((!udd_g_ctrlreq.over_under_run)
				|| (!udd_g_ctrlreq.over_under_run())) {
			// Underrun then send zlp on IN
			// Here nb_remain=0 and allows to send a IN ZLP
		} else {
			// A new payload buffer is given
			udd_ctrl_payload_nb_trans = 0;
			nb_remain = udd_g_ctrlreq.payload_size;
		}
	}
	// Continue transfer and send next data
	if (nb_remain >= USB_DEVICE_EP_CTRL_SIZE) {
		nb_remain = USB_DEVICE_EP_CTRL_SIZE;
		b_shortpacket = false;
	} else {
		b_shortpacket = true;
	}
	// Fill buffer of endpoint control
	ptr_src = udd_g_ctrlreq.payload + udd_ctrl_payload_nb_trans;
	//** Critical section
	// Only in case of DATA IN phase abort without USB Reset signal after.
	// The IN data don't must be written in endpoint 0 DPRAM during
	// a next setup reception in same endpoint 0 DPRAM.
	// Thereby, an OUT ZLP reception must check before IN data write
	// and if no OUT ZLP is received the data must be written quickly (800us)
	// before an eventually ZLP OUT and SETUP reception
	flags = cpu_irq_save();
	if (Is_udd_bank0_received(0)) {
		// IN DATA phase aborted by OUT ZLP
		cpu_irq_restore(flags);
		udd_ep_control_state = UDD_EPCTRL_HANDSHAKE_WAIT_OUT_ZLP;
		udd_ack_in_sent(0);
		return; // Exit of IN DATA phase
	}
	// Write quickly the IN data
	for (i = 0; i < nb_remain; i++) {
		udd_endpoint_fifo_write(0, *ptr_src++);
	}
	udd_ctrl_payload_nb_trans += nb_remain;
	// Validate and send the data available in the control endpoint buffer
	udd_set_transmit_ready(0);
	udd_ack_in_sent(0);

	// In case of abort of DATA IN phase, no need to enable nak OUT interrupt
	// because OUT endpoint is already free and ZLP OUT accepted.
	cpu_irq_restore(flags);
}


static void udd_ctrl_out_received(void)
{
	uint8_t i;
	uint16_t nb_data;

	if (UDD_EPCTRL_DATA_OUT != udd_ep_control_state) {
		if ((UDD_EPCTRL_DATA_IN == udd_ep_control_state)
				|| (UDD_EPCTRL_HANDSHAKE_WAIT_OUT_ZLP ==
				udd_ep_control_state)) {
			// End of SETUP request:
			// - Data IN Phase aborted,
			// - or last Data IN Phase hidden by ZLP OUT sending quickly,
			// - or ZLP OUT received normally.
			udd_ctrl_endofrequest();
		} else {
			// Protocol error during SETUP request
			udd_ctrl_stall_data();
		}
		udd_ack_bank0_received(0);
		// Reinitializes control endpoint management
		udd_ctrl_init();
		return;
	}
	// Read data received during OUT phase
	nb_data = udd_byte_count(0);
	if (udd_g_ctrlreq.payload_size < (udd_ctrl_payload_nb_trans + nb_data)) {
		// Payload buffer too small
		nb_data = udd_g_ctrlreq.payload_size -
				udd_ctrl_payload_nb_trans;
	}
	uint8_t *ptr_dest = udd_g_ctrlreq.payload + udd_ctrl_payload_nb_trans;
	for (i = 0; i < nb_data; i++) {
		*ptr_dest++ = udd_endpoint_fifo_read(0);
	}
	udd_ctrl_payload_nb_trans += nb_data;

	if ((USB_DEVICE_EP_CTRL_SIZE != nb_data)
			|| (udd_g_ctrlreq.req.wLength <=
			(udd_ctrl_prev_payload_nb_trans +
			udd_ctrl_payload_nb_trans))) {
		// End of reception because it is a short packet
		// Before send ZLP, call intermediate callback
		// in case of data receive generate a stall
		udd_g_ctrlreq.payload_size = udd_ctrl_payload_nb_trans;
		if (NULL != udd_g_ctrlreq.over_under_run) {
			if (!udd_g_ctrlreq.over_under_run()) {
				// Stall ZLP
				udd_ctrl_stall_data();
				// Ack reception of OUT to replace NAK by a STALL
				udd_ack_bank0_received(0);
				return;
			}
		}
		// Send IN ZLP to ACK setup request
		udd_ack_bank0_received(0);
		udd_ctrl_send_zlp_in();
		return;
	}

	if (udd_g_ctrlreq.payload_size == udd_ctrl_payload_nb_trans) {
		// Overrun then request a new payload buffer
		if (!udd_g_ctrlreq.over_under_run) {
			// No callback available to request a new payload buffer
			udd_ctrl_stall_data();
			// Ack reception of OUT to replace NAK by a STALL
			udd_ack_bank0_received(0);
			return;
		}
		if (!udd_g_ctrlreq.over_under_run()) {
			// No new payload buffer delivered
			udd_ctrl_stall_data();
			// Ack reception of OUT to replace NAK by a STALL
			udd_ack_bank0_received(0);
			return;
		}
		// New payload buffer available
		// Update number of total data received
		udd_ctrl_prev_payload_nb_trans += udd_ctrl_payload_nb_trans;
		// Reinit reception on payload buffer
		udd_ctrl_payload_nb_trans = 0;
	}
	// Free buffer of control endpoint to authorize next reception
	udd_ack_bank0_received(0);
}


static void udd_ctrl_stall_data(void)
{
	// Stall all packets on IN & OUT control endpoint
	udd_ep_control_state = UDD_EPCTRL_STALL_REQ;
	udd_enable_stall_handshake(0);
}


static void udd_ctrl_send_zlp_in(void)
{
	udd_ep_control_state = UDD_EPCTRL_HANDSHAKE_WAIT_IN_ZLP;
	// Validate and send empty IN packet on control endpoint
	// Send ZLP on IN endpoint
	udd_set_transmit_ready(0);
}


static void udd_ctrl_send_zlp_out(void)
{
	udd_ep_control_state = UDD_EPCTRL_HANDSHAKE_WAIT_OUT_ZLP;
	// No action is necessary to accept OUT ZLP
	// because the buffer of control endpoint is already free
}


static void udd_ctrl_endofrequest(void)
{
	// If a callback is registered then call it
	if (udd_g_ctrlreq.callback) {
		udd_g_ctrlreq.callback();
	}
}


static bool udd_ctrl_interrupt(void)
{
	if (!Is_udd_endpoint_interrupt(0))
		return false; // No interrupt events on control endpoint

	// Search event on control endpoint
	if (Is_udd_setup_received(0)) {
		// SETUP packet received
		udd_ctrl_setup_received();
		return true;
	}
	if (Is_udd_in_sent(0)) {
		// IN packet sent
		udd_ctrl_in_sent();
		return true;
	}
	if (Is_udd_bank0_received(0)) {
		// OUT packet received
		udd_ctrl_out_received();
		return true;
	}
	if (Is_udd_stall(0)) {
		// STALLed
		udd_ack_stall(0);
		return true;
	}
	return false;
}


//--------------------------------------------------------
//--- INTERNAL ROUTINES TO MANAGED THE BULK/INTERRUPT/ISOCHRONOUS ENDPOINTS

#if (0!=USB_DEVICE_MAX_EP)

static void udd_ep_job_table_reset(void)
{
	uint8_t i;
	for (i = 0; i < USB_DEVICE_MAX_EP; i++) {
		udd_ep_job[i].bank = 0;
		udd_ep_job[i].busy = false;
		udd_ep_job[i].b_stall_requested = false;
		udd_ep_job[i].b_shortpacket = false;
		udd_ep_job[i].b_buf_end = false;
	}
}


static void udd_ep_job_table_kill(void)
{
	uint8_t i;

	// For each endpoint, kill job
	for (i = 0; i < USB_DEVICE_MAX_EP; i++) {
		udd_ep_finish_job(&udd_ep_job[i], UDD_EP_TRANSFER_ABORT, i + 1);
	}
}


static void udd_ep_abort_job(udd_ep_id_t ep)
{
	ep &= USB_EP_ADDR_MASK;

	// Abort job on endpoint
	udd_ep_finish_job(&udd_ep_job[ep - 1], UDD_EP_TRANSFER_ABORT, ep);
}


static void udd_ep_finish_job(udd_ep_job_t * ptr_job, int status,
		uint8_t ep_num)
{
	if (ptr_job->busy == false) {
		return; // No on-going job
	}
	ptr_job->busy = false;
	if (NULL == ptr_job->call_trans) {
		return; // No callback linked to job
	}
	if (Is_udd_endpoint_type_in(ep_num)) {
		ep_num |= USB_EP_DIR_IN;
	}
	ptr_job->call_trans((status == UDD_EP_TRANSFER_ABORT) ?
		UDD_EP_TRANSFER_ABORT : UDD_EP_TRANSFER_OK, ptr_job->buf_size, ep_num);
}


static void udd_ep_ack_out_received(udd_ep_id_t ep)
{
	bool bank0_received, bank1_received;
	udd_ep_job_t *ptr_job = &udd_ep_job[ep - 1];

	bank0_received = Is_udd_bank0_received(ep);
	bank1_received = Is_udd_bank1_received(ep);

	if (bank0_received && bank1_received) {
		// The only way is to use ptr_job->bank
	} else if (bank0_received) {
		// Must be bank0
		ptr_job->bank = 0;
	} else {
		// Must be bank1
		ptr_job->bank = 1;
	}
	if (ptr_job->bank == 0) {
		udd_ack_bank0_received(ep);
		if (udd_get_endpoint_bank_max_nbr(ep) > 1) {
			ptr_job->bank = 1;
		}
	} else {
		udd_ack_bank1_received(ep);
		ptr_job->bank = 0;
	}
}


static bool udd_ep_write_fifo(udd_ep_id_t ep)
{
	udd_ep_job_t *ptr_job = &udd_ep_job[ep - 1];
	uint8_t *ptr_src = &ptr_job->buf[ptr_job->buf_cnt];
	uint32_t nb_remain = ptr_job->buf_size - ptr_job->buf_cnt;
	uint32_t pkt_size = ptr_job->size;
	bool is_short_pkt = false;

	// Packet size
	if (nb_remain < pkt_size) {
		pkt_size = nb_remain;
		is_short_pkt = true;
	}

	// Modify job information
	ptr_job->buf_cnt += pkt_size;

	// Speed block data transfer to FIFO (DPRAM)
	for (; pkt_size >= 8; pkt_size -= 8) {
		udd_endpoint_fifo_write(ep, *ptr_src++);
		udd_endpoint_fifo_write(ep, *ptr_src++);
		udd_endpoint_fifo_write(ep, *ptr_src++);
		udd_endpoint_fifo_write(ep, *ptr_src++);
		udd_endpoint_fifo_write(ep, *ptr_src++);
		udd_endpoint_fifo_write(ep, *ptr_src++);
		udd_endpoint_fifo_write(ep, *ptr_src++);
		udd_endpoint_fifo_write(ep, *ptr_src++);
	}
	// Normal speed data transfer to FIFO (DPRAM)
	for (; pkt_size; pkt_size--) {
		udd_endpoint_fifo_write(ep, *ptr_src++);
	}

	// Add to buffered banks
	ptr_job->bank++;
	return is_short_pkt;
}


static bool udd_ep_in_sent(udd_ep_id_t ep, bool b_tx)
{
	bool b_shortpacket;
	udd_ep_job_t *ptr_job = &udd_ep_job[ep - 1];

	// All banks are full
	if (ptr_job->bank >= udd_get_endpoint_bank_max_nbr(ep)) {
		return true; // Data pending
	}

	// No more data in buffer
	if (ptr_job->buf_cnt >= ptr_job->buf_size && !ptr_job->b_shortpacket) {
		return false;
	}

	// Fill FIFO
	b_shortpacket = udd_ep_write_fifo(ep);

	// Data is ready to send
	if (b_tx) {
		udd_set_transmit_ready(ep);
	}
	// Short PKT? no need to send it again.
	if (b_shortpacket) {
		ptr_job->b_shortpacket = false;
	}
	// All transfer done, including ZLP, Finish Job
	if ((ptr_job->buf_cnt >= ptr_job->buf_size)
			&& (!ptr_job->b_shortpacket)) {
		ptr_job->b_buf_end = true;
		return false;
	}
	return true; // Pending
}


static void udd_ep_out_received(udd_ep_id_t ep)
{
	udd_ep_job_t *ptr_job = &udd_ep_job[ep - 1];
	uint32_t nb_data = 0, i;
	uint32_t nb_remain = ptr_job->buf_size - ptr_job->buf_cnt;
	uint32_t pkt_size = ptr_job->size;
	uint8_t *ptr_dst = &ptr_job->buf[ptr_job->buf_cnt];
	bool b_full = false, b_short;

	// Read byte count
	nb_data = udd_byte_count(ep);
	b_short = (nb_data < pkt_size);

	// Copy data if there is
	if (nb_data > 0) {
		if (nb_data >= nb_remain) {
			nb_data = nb_remain;
			b_full = true;
		}
		// Modify job information
		ptr_job->buf_cnt += nb_data;

		// Copy FIFO (DPRAM) to buffer
		for (i = 0; i < nb_data; i++) {
			*ptr_dst++ = udd_endpoint_fifo_read(ep);
		}
	}
	// Clear FIFO Status
	udd_ep_ack_out_received(ep);
	// Finish job on error or short packet
	if ((b_full || b_short) &&
			!Is_udd_endpoint_stall_requested(ep)) {
		udd_disable_endpoint_interrupt(ep);
		ptr_job->buf_size = ptr_job->buf_cnt; // buf_size is passed to callback as XFR count
		udd_ep_finish_job(ptr_job, UDD_EP_TRANSFER_OK, ep);
	}
}


static bool udd_ep_interrupt(void)
{
	udd_ep_id_t ep;
	udd_ep_job_t *ptr_job;

	// For each endpoint different of control endpoint (0)
	for (ep = 1; ep <= USB_DEVICE_MAX_EP; ep++) {
		// Check RXRDY and TXEMPTY event for none DMA endpoints
		if (!Is_udd_endpoint_interrupt_enabled(ep)) {
			continue;
		}

		// Get job corresponding at endpoint
		ptr_job = &udd_ep_job[ep - 1];

		// RXOUT: Full packet received
		if (Is_udd_any_bank_received(ep)) {
			udd_ep_out_received(ep);
			return true;
		}
		// TXIN: packet sent
		if (Is_udd_in_sent(ep)) {

			ptr_job->bank--;
			// Stall when all banks free
			if (ptr_job->b_stall_requested) {
				if (ptr_job->bank) {
					// Send remaining
					udd_set_transmit_ready(ep);
					udd_ack_in_sent(ep);
				} else {
					// Ack last packet
					udd_ack_in_sent(ep);
					// Enable stall
					udd_enable_stall_handshake(ep);
					// Halt executed
					ptr_job->b_stall_requested = false;
				}
				return true;
			}
			// Finish Job when buffer end
			if (ptr_job->b_buf_end) {
				ptr_job->b_buf_end = false;
				ptr_job->buf_size = ptr_job->buf_cnt; // buf_size is passed to callback as XFR count
                udd_disable_endpoint_interrupt(ep);
                udd_ep_finish_job(ptr_job, UDD_EP_TRANSFER_OK, ep);
			}
			if (ptr_job->buf_cnt >= ptr_job->buf_size &&
					!ptr_job->b_shortpacket &&
					ptr_job->bank == 0) {
				// All transfer done, including ZLP
				irqflags_t flags = cpu_irq_save();
				udd_disable_endpoint_interrupt(ep);
				cpu_irq_restore(flags);
				// Ack last packet
				udd_ack_in_sent(ep);
				return true;
			} else if (udd_get_endpoint_bank_max_nbr(ep) > 1
					&& ptr_job->bank > 0) {
				// Already banks buffered, transmit while loading
				udd_set_transmit_ready(ep);
				udd_ack_in_sent(ep);
				udd_ep_in_sent(ep, false);
			} else if (udd_get_endpoint_bank_max_nbr(ep) > 1) {
				// Still bank free, load and transmit
				if (!udd_ep_in_sent(ep, true)) {
					ptr_job->b_buf_end = false;
					ptr_job->buf_size = ptr_job->buf_cnt; // buf_size is passed to callback as XFR count
                    udd_disable_endpoint_interrupt(ep);
                    udd_ep_finish_job(ptr_job, UDD_EP_TRANSFER_OK, ep);
				}
				udd_ack_in_sent(ep);
				udd_ep_in_sent(ep, false);
			} else {
				// Single bank transfer, ack when ready
				udd_ep_in_sent(ep, true);
				udd_ack_in_sent(ep);
			}
			return true;
		}
		// Stall sent/CRC error
		if (Is_udd_stall(ep)) {
			udd_ack_stall(ep);
			if (udd_get_endpoint_type(ep) == UDP_CSR_EPTYPE_ISO_OUT ||
				udd_get_endpoint_type(ep) == UDP_CSR_EPTYPE_ISO_IN) {
			}
			return true;
		}
	}
	return false;
}
#endif // (0!=USB_DEVICE_MAX_EP)

//@}
