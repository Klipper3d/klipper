/**
 * \file
 *
 * \brief Chip-specific system clock management functions
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

#include <compiler.h>
#include <stdbool.h>
#include <sysclk.h>
#include <flashcalw.h>
#include <bpm.h>
#include <osc.h>

/**
 * \weakgroup sysclk_group
 * @{
 */

#if ((CONFIG_SYSCLK_CPU_DIV > CONFIG_SYSCLK_PBA_DIV) || \
     (CONFIG_SYSCLK_CPU_DIV > CONFIG_SYSCLK_PBB_DIV) || \
     (CONFIG_SYSCLK_CPU_DIV > CONFIG_SYSCLK_PBC_DIV) || \
     (CONFIG_SYSCLK_CPU_DIV > CONFIG_SYSCLK_PBD_DIV))
# error CONFIG_SYSCLK_PBx_DIV must be equal to or more than CONFIG_SYSCLK_CPU_DIV.
#endif

/**
 * \internal
 * \defgroup sysclk_internals_group System Clock internals
 *
 * System clock management is fairly straightforward apart from one
 * thing: Enabling and disabling bus bridges. When all peripherals on a
 * given bus are disabled, the bridge to the bus may be disabled. Only
 * the PBA and PBB busses support this, and it is not practical to
 * disable the PBA bridge as it includes the PM and SCIF modules, so turning
 * it off would make it impossible to turn anything back on again.
 *
 * The system clock implementation keeps track of a reference count for
 * PBB. When the reference count is zero, the bus bridge is disabled, otherwise
 * it is enabled.
 *
 * @{
 */

/**
 * \internal
 * \name Initial module clock masks
 *
 * These are the mask values written to the xxxMASK registers during
 * initialization if the user has overridden the default behavior of all clocks
 * left enabled. These values assume that:
 *   - Debugging should be possible
 *   - The program may be running from flash
 *   - The PM should be available to unmask other clocks
 *   - All on-chip RAM should be available
 *   - SCIF, BPM, BSCIF and GPIO are made permanently available for now; this
 *     may change in the future.
 */
//@{
//! \internal
//! \brief Initial value of CPUMASK
#define SYSCLK_INIT_MINIMAL_CPUMASK       0

//! \internal
//! \brief Initial value of HSBMASK
#define SYSCLK_INIT_MINIMAL_HSBMASK                                    \
	((1 << SYSCLK_HFLASHC_DATA)                                        \
		| (1 << SYSCLK_PBB_BRIDGE)                                     \
		| (1 << SYSCLK_PBC_BRIDGE)                                     \
		| (1 << SYSCLK_PBD_BRIDGE))

//! \internal
//! \brief Initial value of PBAMASK
#define SYSCLK_INIT_MINIMAL_PBAMASK       0

//! \internal
//! \brief Initial value of PBBMASK
#define SYSCLK_INIT_MINIMAL_PBBMASK       (1 << SYSCLK_HFLASHC_REGS)

//! \internal
//! \brief Initial value of PBCMASK
#define SYSCLK_INIT_MINIMAL_PBCMASK                                    \
	((1 << SYSCLK_PM)                                                  \
		| (1 << SYSCLK_GPIO)                                           \
		| (1 << SYSCLK_SCIF))

//! \internal
//! \brief Initial value of PBDMASK
#define SYSCLK_INIT_MINIMAL_PBDMASK                                    \
	((1 << SYSCLK_BPM)                                                 \
		| (1 << SYSCLK_BSCIF))
//@}

#if defined(CONFIG_SYSCLK_DEFAULT_RETURNS_SLOW_OSC)
/**
 * \brief boolean signalling that the sysclk_init is done.
 */
bool sysclk_initialized = false;
#endif

/**
 * \internal
 * \brief Enable a maskable module clock.
 * \param bus_id Bus index, given by the \c PM_CLK_GRP_xxx definitions.
 * \param module_index Index of the module to be enabled. This is the
 * bit number in the corresponding xxxMASK register.
 */
void sysclk_priv_enable_module(uint32_t bus_id, uint32_t module_index)
{
	irqflags_t flags;
	uint32_t   mask;

	flags = cpu_irq_save();

	/* Enable the clock */
	mask = *(&PM->PM_CPUMASK + bus_id);
	mask |= 1U << module_index;
	PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu) |
		BPM_UNLOCK_ADDR(((uint32_t)&PM->PM_CPUMASK - (uint32_t)PM) + (4 * bus_id));
	*(&PM->PM_CPUMASK + bus_id) = mask;

	cpu_irq_restore(flags);
}

/**
 * \internal
 * \brief Disable a maskable module clock.
 * \param bus_id Bus index, given by the \c PM_CLK_GRP_xxx definitions.
 * \param module_index Index of the module to be disabled. This is the
 * bit number in the corresponding xxxMASK register.
 */
void sysclk_priv_disable_module(uint32_t bus_id, uint32_t module_index)
{
	irqflags_t flags;
	uint32_t   mask;

	flags = cpu_irq_save();

	/* Disable the clock */
	mask = *(&PM->PM_CPUMASK + bus_id);
	mask &= ~(1U << module_index);
	PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu) |
		BPM_UNLOCK_ADDR(((uint32_t)&PM->PM_CPUMASK - (uint32_t)PM) + (4 * bus_id));
	*(&PM->PM_CPUMASK + bus_id) = mask;

	cpu_irq_restore(flags);
}

//! @}

/**
 * \brief Enable a module clock derived from the PBA clock
 * \param module_index Index of the module clock in the PBAMASK register
 */
void sysclk_enable_pba_module(uint32_t module_index)
{
	irqflags_t flags;

	/* Enable the bridge if necessary */
	flags = cpu_irq_save();

	if (PM->PM_PBAMASK == 0) {
		sysclk_enable_hsb_module(SYSCLK_PBA_BRIDGE);
	}

	cpu_irq_restore(flags);

	/* Enable the module */
	sysclk_priv_enable_module(PM_CLK_GRP_PBA, module_index);
}

/**
 * \brief Disable a module clock derived from the PBA clock
 * \param module_index Index of the module clock in the PBAMASK register
 */
void sysclk_disable_pba_module(uint32_t module_index)
{
	irqflags_t flags;

	/* Disable the module */
	sysclk_priv_disable_module(PM_CLK_GRP_PBA, module_index);

	/* Disable the bridge if possible */
	flags = cpu_irq_save();

	if (PM->PM_PBAMASK == 0) {
		sysclk_disable_hsb_module(SYSCLK_PBA_BRIDGE);
	}

	cpu_irq_restore(flags);
}

/**
 * \brief Enable a module clock derived from the PBB clock
 * \param module_index Index of the module clock in the PBBMASK register
 */
void sysclk_enable_pbb_module(uint32_t module_index)
{
	irqflags_t flags;

	/* Enable the bridge if necessary */
	flags = cpu_irq_save();

	if (PM->PM_PBBMASK == 0) {
		sysclk_enable_hsb_module(SYSCLK_PBB_BRIDGE);
	}

	cpu_irq_restore(flags);

	/* Enable the module */
	sysclk_priv_enable_module(PM_CLK_GRP_PBB, module_index);
}

/**
 * \brief Disable a module clock derived from the PBB clock
 * \param module_index Index of the module clock in the PBBMASK register
 */
void sysclk_disable_pbb_module(uint32_t module_index)
{
	irqflags_t flags;

	/* Disable the module */
	sysclk_priv_disable_module(PM_CLK_GRP_PBB, module_index);

	/* Disable the bridge if possible */
	flags = cpu_irq_save();

	if (PM->PM_PBBMASK == 0) {
		sysclk_disable_hsb_module(SYSCLK_PBB_BRIDGE);
	}

	cpu_irq_restore(flags);
}

/**
 * \brief Retrieves the current rate in Hz of the Peripheral Bus clock attached
 *        to the specified peripheral.
 *
 * \param module Pointer to the module's base address.
 *
 * \return Frequency of the bus attached to the specified peripheral, in Hz.
 */
uint32_t sysclk_get_peripheral_bus_hz(const volatile void *module)
{
	/* Fallthroughs intended for modules sharing the same peripheral bus. */
	switch ((uintptr_t)module) {
	case IISC_ADDR:
	case SPI_ADDR:
	case TC0_ADDR:
	case TC1_ADDR:
	case TWIM0_ADDR:
	case TWIS0_ADDR:
	case TWIM1_ADDR:
	case TWIS1_ADDR:
	case USART0_ADDR:
	case USART1_ADDR:
	case USART2_ADDR:
	case USART3_ADDR:
	case ADCIFE_ADDR:
	case DACC_ADDR:
	case ACIFC_ADDR:
	case GLOC_ADDR:
	case ABDACB_ADDR:
	case TRNG_ADDR:
	case PARC_ADDR:
	case CATB_ADDR:
	case TWIM2_ADDR:
	case TWIM3_ADDR:
	#if !SAM4LS
	case LCDCA_ADDR:
	#endif
		return sysclk_get_pba_hz();

	case HFLASHC_ADDR:
	case HCACHE_ADDR:
	case HMATRIX_ADDR:
	case PDCA_ADDR:
	case CRCCU_ADDR:
	case USBC_ADDR:
	case PEVC_ADDR:
		return sysclk_get_pbb_hz();

	case PM_ADDR:
	case CHIPID_ADDR:
	case SCIF_ADDR:
	case FREQM_ADDR:
	case GPIO_ADDR:
		return sysclk_get_pbc_hz();

	case BPM_ADDR:
	case BSCIF_ADDR:
	case AST_ADDR:
	case WDT_ADDR:
	case EIC_ADDR:
	case PICOUART_ADDR:
		return sysclk_get_pbd_hz();

	default:
		Assert(false);
		return 0;
	}
}

/**
 * \brief Enable a peripheral's clock from its base address.
 *
 *  Enables the clock to a peripheral, given its base address. If the peripheral
 *  has an associated clock on the HSB bus, this will be enabled also.
 *
 * \param module Pointer to the module's base address.
 */
void sysclk_enable_peripheral_clock(const volatile void *module)
{
	switch ((uintptr_t)module) {

	#if !SAM4LS
	case AESA_ADDR:
		sysclk_enable_hsb_module(SYSCLK_AESA_HSB);
		break;
	#endif

	case IISC_ADDR:
		sysclk_enable_pba_module(SYSCLK_IISC);
		break;

	case SPI_ADDR:
		sysclk_enable_pba_module(SYSCLK_SPI);
		break;

	case TC0_ADDR:
		sysclk_enable_pba_module(SYSCLK_TC0);
		sysclk_enable_pba_divmask(PBA_DIVMASK_TIMER_CLOCK2
			| PBA_DIVMASK_TIMER_CLOCK3
			| PBA_DIVMASK_TIMER_CLOCK4
			| PBA_DIVMASK_TIMER_CLOCK5
			);
		break;

	case TC1_ADDR:
		sysclk_enable_pba_module(SYSCLK_TC1);
		sysclk_enable_pba_divmask(PBA_DIVMASK_TIMER_CLOCK2
			| PBA_DIVMASK_TIMER_CLOCK3
			| PBA_DIVMASK_TIMER_CLOCK4
			| PBA_DIVMASK_TIMER_CLOCK5
			);
		break;

	case TWIM0_ADDR:
		sysclk_enable_pba_module(SYSCLK_TWIM0);
		break;

	case TWIS0_ADDR:
		sysclk_enable_pba_module(SYSCLK_TWIS0);
		break;

	case TWIM1_ADDR:
		sysclk_enable_pba_module(SYSCLK_TWIM1);
		break;

	case TWIS1_ADDR:
		sysclk_enable_pba_module(SYSCLK_TWIS1);
		break;

	case USART0_ADDR:
		sysclk_enable_pba_module(SYSCLK_USART0);
		sysclk_enable_pba_divmask(PBA_DIVMASK_CLK_USART);
		break;

	case USART1_ADDR:
		sysclk_enable_pba_module(SYSCLK_USART1);
		sysclk_enable_pba_divmask(PBA_DIVMASK_CLK_USART);
		break;

	case USART2_ADDR:
		sysclk_enable_pba_module(SYSCLK_USART2);
		sysclk_enable_pba_divmask(PBA_DIVMASK_CLK_USART);
		break;

	case USART3_ADDR:
		sysclk_enable_pba_module(SYSCLK_USART3);
		sysclk_enable_pba_divmask(PBA_DIVMASK_CLK_USART);
		break;

	case ADCIFE_ADDR:
		sysclk_enable_pba_module(SYSCLK_ADCIFE);
		break;

	case DACC_ADDR:
		sysclk_enable_pba_module(SYSCLK_DACC);
		break;

	case ACIFC_ADDR:
		sysclk_enable_pba_module(SYSCLK_ACIFC);
		break;

	case GLOC_ADDR:
		sysclk_enable_pba_module(SYSCLK_GLOC);
		break;

	case ABDACB_ADDR:
		sysclk_enable_pba_module(SYSCLK_ABDACB);
		break;

	case TRNG_ADDR:
		sysclk_enable_pba_module(SYSCLK_TRNG);
		break;

	case PARC_ADDR:
		sysclk_enable_pba_module(SYSCLK_PARC);
		break;

	case CATB_ADDR:
		sysclk_enable_pba_module(SYSCLK_CATB);
		break;

	case TWIM2_ADDR:
		sysclk_enable_pba_module(SYSCLK_TWIM2);
		break;

	case TWIM3_ADDR:
		sysclk_enable_pba_module(SYSCLK_TWIM3);
		break;

	#if !SAM4LS
	case LCDCA_ADDR:
		sysclk_enable_pba_module(SYSCLK_LCDCA);
		break;
	#endif

	case HFLASHC_ADDR:
		sysclk_enable_hsb_module(SYSCLK_HFLASHC_DATA);
		sysclk_enable_pbb_module(SYSCLK_HFLASHC_REGS);
		break;

	case HCACHE_ADDR:
		sysclk_enable_hsb_module(SYSCLK_HRAMC1_DATA);
		sysclk_enable_pbb_module(SYSCLK_HRAMC1_REGS);
		break;

	case HMATRIX_ADDR:
		sysclk_enable_pbb_module(SYSCLK_HMATRIX);
		break;

	case PDCA_ADDR:
		sysclk_enable_hsb_module(SYSCLK_PDCA_HSB);
		sysclk_enable_pbb_module(SYSCLK_PDCA_PB);
		break;

	case CRCCU_ADDR:
		sysclk_enable_hsb_module(SYSCLK_CRCCU_DATA);
		sysclk_enable_pbb_module(SYSCLK_CRCCU_REGS);
		break;

	case USBC_ADDR:
		sysclk_enable_hsb_module(SYSCLK_USBC_DATA);
		sysclk_enable_pbb_module(SYSCLK_USBC_REGS);
		break;

	case PEVC_ADDR:
		sysclk_enable_pbb_module(SYSCLK_PEVC);
		break;

	case PM_ADDR:
		sysclk_enable_pbc_module(SYSCLK_PM);
		break;

	case CHIPID_ADDR:
		sysclk_enable_pbc_module(SYSCLK_CHIPID);
		break;

	case SCIF_ADDR:
		sysclk_enable_pbc_module(SYSCLK_SCIF);
		break;

	case FREQM_ADDR:
		sysclk_enable_pbc_module(SYSCLK_FREQM);
		break;

	case GPIO_ADDR:
		sysclk_enable_pbc_module(SYSCLK_GPIO);
		break;

	case BPM_ADDR:
		sysclk_enable_pbd_module(SYSCLK_BPM);
		break;

	case BSCIF_ADDR:
		sysclk_enable_pbd_module(SYSCLK_BSCIF);
		break;

	case AST_ADDR:
		sysclk_enable_pbd_module(SYSCLK_AST);
		break;

	case WDT_ADDR:
		sysclk_enable_pbd_module(SYSCLK_WDT);
		break;

	case EIC_ADDR:
		sysclk_enable_pbd_module(SYSCLK_EIC);
		break;

	case PICOUART_ADDR:
		sysclk_enable_pbd_module(SYSCLK_PICOUART);
		break;

	default:
		Assert(false);
		return;
	}
}

/**
 * \brief Disable a peripheral's clock from its base address.
 *
 *  Disables the clock to a peripheral, given its base address. If the peripheral
 *  has an associated clock on the HSB bus, this will be disabled also.
 *
 * \param module Pointer to the module's base address.
 */
void sysclk_disable_peripheral_clock(const volatile void *module)
{
	switch ((uintptr_t)module) {

	#if !SAM4LS
	case AESA_ADDR:
		sysclk_disable_hsb_module(SYSCLK_AESA_HSB);
		break;
	#endif

	case IISC_ADDR:
		sysclk_disable_pba_module(SYSCLK_IISC);
		break;

	case SPI_ADDR:
		sysclk_disable_pba_module(SYSCLK_SPI);
		break;

	case TC0_ADDR:
		sysclk_disable_pba_module(SYSCLK_TC0);
		break;

	case TC1_ADDR:
		sysclk_disable_pba_module(SYSCLK_TC1);
		break;

	case TWIM0_ADDR:
		sysclk_disable_pba_module(SYSCLK_TWIM0);
		break;

	case TWIS0_ADDR:
		sysclk_disable_pba_module(SYSCLK_TWIS0);
		break;

	case TWIM1_ADDR:
		sysclk_disable_pba_module(SYSCLK_TWIM1);
		break;

	case TWIS1_ADDR:
		sysclk_disable_pba_module(SYSCLK_TWIS1);
		break;

	case USART0_ADDR:
		sysclk_disable_pba_module(SYSCLK_USART0);
		break;

	case USART1_ADDR:
		sysclk_disable_pba_module(SYSCLK_USART1);
		break;

	case USART2_ADDR:
		sysclk_disable_pba_module(SYSCLK_USART2);
		break;

	case USART3_ADDR:
		sysclk_disable_pba_module(SYSCLK_USART3);
		break;

	case ADCIFE_ADDR:
		sysclk_disable_pba_module(SYSCLK_ADCIFE);
		break;

	case DACC_ADDR:
		sysclk_disable_pba_module(SYSCLK_DACC);
		break;

	case ACIFC_ADDR:
		sysclk_disable_pba_module(SYSCLK_ACIFC);
		break;

	case GLOC_ADDR:
		sysclk_disable_pba_module(SYSCLK_GLOC);
		break;

	case ABDACB_ADDR:
		sysclk_disable_pba_module(SYSCLK_ABDACB);
		break;

	case TRNG_ADDR:
		sysclk_disable_pba_module(SYSCLK_TRNG);
		break;

	case PARC_ADDR:
		sysclk_disable_pba_module(SYSCLK_PARC);
		break;

	case CATB_ADDR:
		sysclk_disable_pba_module(SYSCLK_CATB);
		break;

	case TWIM2_ADDR:
		sysclk_disable_pba_module(SYSCLK_TWIM2);
		break;

	case TWIM3_ADDR:
		sysclk_disable_pba_module(SYSCLK_TWIM3);
		break;

	#if !SAM4LS
	case LCDCA_ADDR:
		sysclk_disable_pba_module(SYSCLK_LCDCA);
		break;
	#endif

	case HFLASHC_ADDR:
		sysclk_disable_pbb_module(SYSCLK_HFLASHC_REGS);
		break;

	case HCACHE_ADDR:
		sysclk_disable_hsb_module(SYSCLK_HRAMC1_DATA);
		sysclk_disable_pbb_module(SYSCLK_HRAMC1_REGS);
		break;

	case HMATRIX_ADDR:
		sysclk_disable_pbb_module(SYSCLK_HMATRIX);
		break;

	case PDCA_ADDR:
		sysclk_disable_hsb_module(SYSCLK_PDCA_HSB);
		sysclk_disable_pbb_module(SYSCLK_PDCA_PB);
		break;

	case CRCCU_ADDR:
		sysclk_disable_hsb_module(SYSCLK_CRCCU_DATA);
		sysclk_disable_pbb_module(SYSCLK_CRCCU_REGS);
		break;

	case USBC_ADDR:
		sysclk_disable_hsb_module(SYSCLK_USBC_DATA);
		sysclk_disable_pbb_module(SYSCLK_USBC_REGS);
		break;

	case PEVC_ADDR:
		sysclk_disable_pbb_module(SYSCLK_PEVC);
		break;

	case PM_ADDR:
		sysclk_disable_pbc_module(SYSCLK_PM);
		break;

	case CHIPID_ADDR:
		sysclk_disable_pbc_module(SYSCLK_CHIPID);
		break;

	case SCIF_ADDR:
		sysclk_disable_pbc_module(SYSCLK_SCIF);
		break;

	case FREQM_ADDR:
		sysclk_disable_pbc_module(SYSCLK_FREQM);
		break;

	case GPIO_ADDR:
		sysclk_disable_pbc_module(SYSCLK_GPIO);
		break;

	case BPM_ADDR:
		sysclk_disable_pbd_module(SYSCLK_BPM);
		break;

	case BSCIF_ADDR:
		sysclk_disable_pbd_module(SYSCLK_BSCIF);
		break;

	case AST_ADDR:
		sysclk_disable_pbd_module(SYSCLK_AST);
		break;

	case WDT_ADDR:
		sysclk_disable_pbd_module(SYSCLK_WDT);
		break;

	case EIC_ADDR:
		sysclk_disable_pbd_module(SYSCLK_EIC);
		break;

	case PICOUART_ADDR:
		sysclk_disable_pbd_module(SYSCLK_PICOUART);
		break;

	default:
		Assert(false);
		return;
	}

	// Disable PBA divided clock if possible.
#define PBADIV_CLKSRC_MASK ((1 << SYSCLK_TC0) | \
							(1 << SYSCLK_TC1) | \
							(1 << SYSCLK_USART0) | \
							(1 << SYSCLK_USART1) | \
							(1 << SYSCLK_USART2) | \
							(1 << SYSCLK_USART3))
	if ((PM->PM_PBAMASK & PBADIV_CLKSRC_MASK) == 0) {
		sysclk_disable_pba_divmask(PBA_DIVMASK_Msk);
	}
}


/**
 * \brief Set system clock prescaler configuration
 *
 * This function will change the system clock prescaler configuration to
 * match the parameters.
 *
 * \note The parameters to this function are device-specific.
 *
 * \param cpu_shift The CPU clock will be divided by \f$2^{cpu\_shift}\f$
 * \param pba_shift The PBA clock will be divided by \f$2^{pba\_shift}\f$
 * \param pbb_shift The PBB clock will be divided by \f$2^{pbb\_shift}\f$
 * \param pbc_shift The PBC clock will be divided by \f$2^{pbc\_shift}\f$
 * \param pbd_shift The PBD clock will be divided by \f$2^{pbd\_shift}\f$
 */
void sysclk_set_prescalers(uint32_t cpu_shift,
		uint32_t pba_shift, uint32_t pbb_shift,
		uint32_t pbc_shift, uint32_t pbd_shift)
{
	irqflags_t flags;
	uint32_t   cpu_cksel = 0;
	uint32_t   pba_cksel = 0;
	uint32_t   pbb_cksel = 0;
	uint32_t   pbc_cksel = 0;
	uint32_t   pbd_cksel = 0;

	Assert(cpu_shift <= pba_shift);
	Assert(cpu_shift <= pbb_shift);
	Assert(cpu_shift <= pbc_shift);
	Assert(cpu_shift <= pbd_shift);

	if (cpu_shift > 0) {
		cpu_cksel = (PM_CPUSEL_CPUSEL(cpu_shift - 1))
				| PM_CPUSEL_CPUDIV;
	}

	if (pba_shift > 0) {
		pba_cksel = (PM_PBASEL_PBSEL(pba_shift - 1))
				| PM_PBASEL_PBDIV;
	}

	if (pbb_shift > 0) {
		pbb_cksel = (PM_PBBSEL_PBSEL(pbb_shift - 1))
				| PM_PBBSEL_PBDIV;
	}

	if (pbc_shift > 0) {
		pbc_cksel = (PM_PBCSEL_PBSEL(pbc_shift - 1))
				| PM_PBCSEL_PBDIV;
	}

	if (pbd_shift > 0) {
		pbd_cksel = (PM_PBDSEL_PBSEL(pbd_shift - 1))
				| PM_PBDSEL_PBDIV;
	}

	flags = cpu_irq_save();

	PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu)
		| PM_UNLOCK_ADDR((uint32_t)&PM->PM_CPUSEL - (uint32_t)PM);
	PM->PM_CPUSEL = cpu_cksel;

	PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu)
		| PM_UNLOCK_ADDR((uint32_t)&PM->PM_PBASEL - (uint32_t)PM);
	PM->PM_PBASEL = pba_cksel;

	PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu)
		| PM_UNLOCK_ADDR((uint32_t)&PM->PM_PBBSEL - (uint32_t)PM);
	PM->PM_PBBSEL = pbb_cksel;

	PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu)
		| PM_UNLOCK_ADDR((uint32_t)&PM->PM_PBCSEL - (uint32_t)PM);
	PM->PM_PBCSEL = pbc_cksel;

	PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu)
		| PM_UNLOCK_ADDR((uint32_t)&PM->PM_PBDSEL - (uint32_t)PM);
	PM->PM_PBDSEL = pbd_cksel;

	cpu_irq_restore(flags);
}

/**
 * \brief Change the source of the main system clock.
 *
 * \param src The new system clock source. Must be one of the constants
 * from the <em>System Clock Sources</em> section.
 */
void sysclk_set_source(uint32_t src)
{
	irqflags_t flags;
	Assert(src <= SYSCLK_SRC_RC1M);

	flags = cpu_irq_save();
	PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu)
		| PM_UNLOCK_ADDR((uint32_t)&PM->PM_MCCTRL - (uint32_t)PM);
	PM->PM_MCCTRL = src;
	cpu_irq_restore(flags);
}

#if defined(CONFIG_USBCLK_SOURCE) || defined(__DOXYGEN__)
/**
 * \brief Enable the USB generic clock
 *
 * \pre The USB generic clock must be configured to 48MHz.
 * CONFIG_USBCLK_SOURCE and CONFIG_USBCLK_DIV must be defined with proper
 * configuration. The selected clock source must also be configured.
 */
void sysclk_enable_usb(void)
{
	// Note: the SYSCLK_PBB_BRIDGE clock is enabled by
	// sysclk_enable_pbb_module().
	sysclk_enable_pbb_module(SYSCLK_USBC_REGS);
	sysclk_enable_hsb_module(SYSCLK_USBC_DATA);

	genclk_enable_config(7, CONFIG_USBCLK_SOURCE, CONFIG_USBCLK_DIV);
}

/**
 * \brief Disable the USB generic clock
 */
void sysclk_disable_usb(void)
{
   genclk_disable(7);
}
#endif // CONFIG_USBCLK_SOURCE

void sysclk_init(void)
{
	uint32_t ps_value = 0;
	bool is_fwu_enabled = false;

#if CONFIG_HCACHE_ENABLE == 1
	/* Enable HCACHE */
	sysclk_enable_peripheral_clock(HCACHE);
	HCACHE->HCACHE_CTRL = HCACHE_CTRL_CEN_YES;
	while (!(HCACHE->HCACHE_SR & HCACHE_SR_CSTS_EN));
#endif

	/* Set up system clock dividers if different from defaults */
	if ((CONFIG_SYSCLK_CPU_DIV > 0) || (CONFIG_SYSCLK_PBA_DIV > 0) ||
			(CONFIG_SYSCLK_PBB_DIV > 0) || (CONFIG_SYSCLK_PBC_DIV > 0) ||
			(CONFIG_SYSCLK_PBD_DIV > 0)) {
		sysclk_set_prescalers(CONFIG_SYSCLK_CPU_DIV,
				CONFIG_SYSCLK_PBA_DIV,
				CONFIG_SYSCLK_PBB_DIV,
				CONFIG_SYSCLK_PBC_DIV,
				CONFIG_SYSCLK_PBD_DIV
				);
	}

	/* Automatically select best power scaling mode */
#ifdef CONFIG_FLASH_READ_MODE_HIGH_SPEED_ENABLE
	ps_value = BPM_PS_2;
	is_fwu_enabled = false;
#elif (defined(CONFIG_PLL0_MUL) || defined(CONFIG_DFLL0_MUL) ||	defined(CONFIG_USBCLK_DIV))
	/* USB/DFLL/PLL are not available in PS1 (BPM.PMCON.PS=1) mode */
	ps_value = BPM_PS_0;
	is_fwu_enabled = false;
#else
	if (sysclk_get_cpu_hz() <= FLASH_FREQ_PS1_FWS_1_MAX_FREQ) {
		ps_value = BPM_PS_1;
		if (sysclk_get_cpu_hz() > FLASH_FREQ_PS1_FWS_0_MAX_FREQ) {
			bpm_enable_fast_wakeup(BPM);
			is_fwu_enabled = true;
		}
	} else {
		ps_value = BPM_PS_0;
	}
#endif

	/* Switch to system clock selected by user */
	if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_RCSYS) {
		/* Already running from RCSYS */
	}
#ifdef BOARD_OSC0_HZ
	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_OSC0) {
		osc_enable(OSC_ID_OSC0);
		osc_wait_ready(OSC_ID_OSC0);
		// Set a flash wait state depending on the new cpu frequency.
		flash_set_bus_freq(sysclk_get_cpu_hz(), ps_value, is_fwu_enabled);
		sysclk_set_source(SYSCLK_SRC_OSC0);
	}
#endif
#ifdef CONFIG_DFLL0_SOURCE
	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_DFLL) {
		dfll_enable_config_defaults(0);
		// Set a flash wait state depending on the new cpu frequency.
		flash_set_bus_freq(sysclk_get_cpu_hz(), ps_value, is_fwu_enabled);
		sysclk_set_source(SYSCLK_SRC_DFLL);
	}
#endif
#ifdef CONFIG_PLL0_SOURCE
	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_PLL0) {
		pll_enable_config_defaults(0);
		// Set a flash wait state depending on the new cpu frequency.
		flash_set_bus_freq(sysclk_get_cpu_hz(), ps_value, is_fwu_enabled);
		sysclk_set_source(SYSCLK_SRC_PLL0);
	}
#endif
	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_RC80M) {
		osc_enable(OSC_ID_RC80M);
		osc_wait_ready(OSC_ID_RC80M);
		// Set a flash wait state depending on the new cpu frequency.
		flash_set_bus_freq(sysclk_get_cpu_hz(), ps_value, is_fwu_enabled);
		sysclk_set_source(SYSCLK_SRC_RC80M);
	}
	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_RCFAST) {
		osc_enable(OSC_ID_RCFAST);
		osc_wait_ready(OSC_ID_RCFAST);
		// Set a flash wait state depending on the new cpu frequency.
		flash_set_bus_freq(sysclk_get_cpu_hz(), ps_value, is_fwu_enabled);
		sysclk_set_source(SYSCLK_SRC_RCFAST);
	}
	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_RC1M) {
		osc_enable(OSC_ID_RC1M);
		osc_wait_ready(OSC_ID_RC1M);
		// Set a flash wait state depending on the new cpu frequency.
		flash_set_bus_freq(sysclk_get_cpu_hz(), ps_value, is_fwu_enabled);
		sysclk_set_source(SYSCLK_SRC_RC1M);
	}
	else {
		Assert(false);
	}

	/* Automatically switch to low power mode */
	bpm_configure_power_scaling(BPM, ps_value, BPM_PSCM_CPU_NOT_HALT);
	while ((bpm_get_status(BPM) & BPM_SR_PSOK) == 0);

	/* If the user has specified clock masks, enable only requested clocks */
	irqflags_t const flags = cpu_irq_save();
#if defined(CONFIG_SYSCLK_INIT_CPUMASK)
	PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu)
		| PM_UNLOCK_ADDR((uint32_t)&PM->PM_CPUMASK - (uint32_t)PM);
	PM->PM_CPUMASK = SYSCLK_INIT_MINIMAL_CPUMASK | CONFIG_SYSCLK_INIT_CPUMASK;
#endif

#if defined(CONFIG_SYSCLK_INIT_HSBMASK)
	PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu)
		| PM_UNLOCK_ADDR((uint32_t)&PM->PM_HSBMASK - (uint32_t)PM);
	PM->PM_HSBMASK = SYSCLK_INIT_MINIMAL_HSBMASK | CONFIG_SYSCLK_INIT_HSBMASK;
#endif

#if defined(CONFIG_SYSCLK_INIT_PBAMASK)
	PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu)
		| PM_UNLOCK_ADDR((uint32_t)&PM->PM_PBAMASK - (uint32_t)PM);
	PM->PM_PBAMASK = SYSCLK_INIT_MINIMAL_PBAMASK | CONFIG_SYSCLK_INIT_PBAMASK;
#endif

#if defined(CONFIG_SYSCLK_INIT_PBBMASK)
	PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu)
		| PM_UNLOCK_ADDR((uint32_t)&PM->PM_PBBMASK - (uint32_t)PM);
	PM->PM_PBBMASK = SYSCLK_INIT_MINIMAL_PBBMASK | CONFIG_SYSCLK_INIT_PBBMASK;
#endif

#if defined(CONFIG_SYSCLK_INIT_PBCMASK)
	PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu)
		| PM_UNLOCK_ADDR((uint32_t)&PM->PM_PBCMASK - (uint32_t)PM);
	PM->PM_PBCMASK = SYSCLK_INIT_MINIMAL_PBCMASK | CONFIG_SYSCLK_INIT_PBCMASK;
#endif

#if defined(CONFIG_SYSCLK_INIT_PBDMASK)
	PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu)
		| PM_UNLOCK_ADDR((uint32_t)&PM->PM_PBDMASK - (uint32_t)PM);
	PM->PM_PBDMASK = SYSCLK_INIT_MINIMAL_PBDMASK | CONFIG_SYSCLK_INIT_PBDMASK;
#endif

	cpu_irq_restore(flags);

#if (defined CONFIG_SYSCLK_DEFAULT_RETURNS_SLOW_OSC)
	/* Signal that the internal frequencies are setup */
	sysclk_initialized = true;
#endif
}

//! @}
