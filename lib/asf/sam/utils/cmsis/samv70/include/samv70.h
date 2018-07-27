/**
 * \file
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
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
#ifndef _SAMV70_
#define _SAMV70_

#if defined (__SAMV70J19B__)
  #include "samv70j19b.h"
#elif defined (__SAMV70J20B__)
  #include "samv70j20b.h"
#elif defined (__SAMV70N19B__)
  #include "samv70n19b.h"
#elif defined (__SAMV70N20B__)
  #include "samv70n20b.h"
#elif defined (__SAMV70Q19B__)
  #include "samv70q19b.h"
#elif defined (__SAMV70Q20B__)
  #include "samv70q20b.h"
#elif defined (__SAMV70J19__)
#include "samv70j19.h"
#elif defined (__SAMV70J20__)
#include "samv70j20.h"
#elif defined (__SAMV70N19__)
#include "samv70n19.h"
#elif defined (__SAMV70N20__)
#include "samv70n20.h"
#elif defined (__SAMV70Q19__)
#include "samv70q19.h"
#elif defined (__SAMV70Q20__)
#include "samv70q20.h"
#else
  #error Library does not support the specified device.
#endif

#endif /* _SAMV70_ */
