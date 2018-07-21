/**
 * \file
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

#ifndef _SAM4S_
#define _SAM4S_

#if defined __SAM4SA16B__
  #include "sam4sa16b.h"
#elif defined __SAM4SA16C__
  #include "sam4sa16c.h"
#elif defined __SAM4SD16B__
  #include "sam4sd16b.h"
#elif defined __SAM4SD16C__
  #include "sam4sd16c.h"
#elif defined __SAM4SD32B__
  #include "sam4sd32b.h"
#elif defined __SAM4SD32C__
  #include "sam4sd32c.h"
#elif defined __SAM4S2A__
  #include "sam4s2a.h"
#elif defined __SAM4S2B__
  #include "sam4s2b.h"
#elif defined __SAM4S2C__
  #include "sam4s2c.h"
#elif defined __SAM4S4A__
  #include "sam4s4a.h"
#elif defined __SAM4S4B__
  #include "sam4s4b.h"
#elif defined __SAM4S4C__
  #include "sam4s4c.h"
#elif defined __SAM4S8B__
  #include "sam4s8b.h"
#elif defined __SAM4S8C__
  #include "sam4s8c.h"
#elif defined __SAM4S16B__
  #include "sam4s16b.h"
#elif defined __SAM4S16C__
  #include "sam4s16c.h"
#else
  #error Library does not support the specified device.
#endif

#endif /* _SAM4S_ */
