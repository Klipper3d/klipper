/* ---------------------------------------------------------------------------- */
/*                  Atmel Microcontroller Software Support                      */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) %copyright_year%, Atmel Corporation                                        */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Atmel's name may not be used to endorse or promote products derived from     */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR   */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE   */
/* DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,      */
/* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,  */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    */
/* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING         */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, */
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                           */
/* ---------------------------------------------------------------------------- */

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
