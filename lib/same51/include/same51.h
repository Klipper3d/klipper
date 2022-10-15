/**
 * \file
 *
 * \brief Top header file for SAME51
 *
 * Copyright (c) 2019 Microchip Technology Inc.
 *
 * \asf_license_start
 *
 * \page License
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the Licence at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * \asf_license_stop
 *
 */

#ifndef _SAME51_
#define _SAME51_

/**
 * \defgroup SAME51_definitions SAME51 Device Definitions
 * \brief SAME51 CMSIS Definitions.
 */

#if   defined(__SAME51G18A__) || defined(__ATSAME51G18A__)
  #include "same51g18a.h"
#elif defined(__SAME51G19A__) || defined(__ATSAME51G19A__)
  #include "same51g19a.h"
#elif defined(__SAME51J18A__) || defined(__ATSAME51J18A__)
  #include "same51j18a.h"
#elif defined(__SAME51J19A__) || defined(__ATSAME51J19A__)
  #include "same51j19a.h"
#elif defined(__SAME51J20A__) || defined(__ATSAME51J20A__)
  #include "same51j20a.h"
#elif defined(__SAME51N19A__) || defined(__ATSAME51N19A__)
  #include "same51n19a.h"
#elif defined(__SAME51N20A__) || defined(__ATSAME51N20A__)
  #include "same51n20a.h"
#else
  #error Library does not support the specified device.
#endif

#endif /* _SAME51_ */
