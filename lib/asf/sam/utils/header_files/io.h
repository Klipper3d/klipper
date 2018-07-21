/**
 * \file
 *
 * \brief Arch file for SAM.
 *
 * This file defines common SAM series.
 *
 * Copyright (c) 2011-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _SAM_IO_
#define _SAM_IO_

/* SAM3 family */

/* SAM3S series */
#if (SAM3S)
# if (SAM3S8 || SAM3SD8)
#  include "sam3s8.h"
# else
#  include "sam3s.h"
# endif
#endif

/* SAM3U series */
#if (SAM3U)
#  include "sam3u.h"
#endif

/* SAM3N series */
#if (SAM3N)
#  include "sam3n.h"
#endif

/* SAM3XA series */
#if (SAM3XA)
#  include "sam3xa.h"
#endif

/* SAM4S series */
#if (SAM4S)
#  include "sam4s.h"
#endif

/* SAM4L series */
#if (SAM4L)
#  include "sam4l.h"
#endif

/* SAM4E series */
#if (SAM4E)
#  include "sam4e.h"
#endif

/* SAM4N series */
#if (SAM4N)
#  include "sam4n.h"
#endif

/* SAM4C series */
#if (SAM4C)
#  include "sam4c.h"
#endif

/* SAM4CM series */
#if (SAM4CM)
#  if (SAM4CMP32 || SAM4CMS32)
#    include "sam4cm32.h"
#  else
#    include "sam4cm.h"
#  endif
#endif

/* SAM4CP series */
#if (SAM4CP)
#  include "sam4cp.h"
#endif

/* SAMG51 series */
#if (SAMG51)
#  include "samg51.h"
#endif

/* SAMG53 series */
#if (SAMG53)
#  include "samg53.h"
#endif

/* SAMG54 series */
#if (SAMG54)
#  include "samg54.h"
#endif

/* SAMG55 series */
#if (SAMG55)
#  include "samg55.h"
#endif

/* SAMV71 series */
#if (SAMV71)
#  include "samv71.h"
#endif

/* SAMV70 series */
#if (SAMV70)
#  include "samv70.h"
#endif

/* SAME70 series */
#if (SAME70)
#  include "same70.h"
#endif

/* SAMS70 series */
#if (SAMS70)
#  include "sams70.h"
#endif

#endif /* _SAM_IO_ */
