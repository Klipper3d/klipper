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

#ifndef _SAM3N_
#define _SAM3N_

#if defined __SAM3N00A__
  #include "sam3n00a.h"
#elif defined __SAM3N00B__
  #include "sam3n00b.h"
#elif defined __SAM3N0A__
  #include "sam3n0a.h"
#elif defined __SAM3N0B__
  #include "sam3n0b.h"
#elif defined __SAM3N0C__
  #include "sam3n0c.h"
#elif defined __SAM3N1A__
  #include "sam3n1a.h"
#elif defined __SAM3N1B__
  #include "sam3n1b.h"
#elif defined __SAM3N1C__
  #include "sam3n1c.h"
#elif defined __SAM3N2A__
  #include "sam3n2a.h"
#elif defined __SAM3N2B__
  #include "sam3n2b.h"
#elif defined __SAM3N2C__
  #include "sam3n2c.h"
#elif defined __SAM3N4A__
  #include "sam3n4a.h"
#elif defined __SAM3N4B__
  #include "sam3n4b.h"
#elif defined __SAM3N4C__
  #include "sam3n4c.h"
#else
  #error Library does not support the specified device.
#endif

#endif /* _SAM3N_ */
