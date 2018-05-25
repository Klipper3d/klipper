/* Copyright 2011 Adam Green (http://mbed.org/users/AdamGreen/)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifndef _MPU_H_
#define _MPU_H_

//#include <cmsis.h>
#include "LPC17xx.h"
#include "cmsis_nvic.h"

/* Memory Protection Unit Type Register Bits. */
/* Number of instruction regions supported by MPU.  0 for Cortex-M3 */
#define MPU_TYPE_IREGION_SHIFT      16
#define MPU_TYPE_IREGION_MASK       (0xFF << MPU_TYPE_IREGION_SHIFT)
/* Number of data regions supported by MPU. */
#define MPU_TYPE_DREGION_SHIFT      8
#define MPU_TYPE_DREGION_MASK       (0xFF << MPU_TYPE_DREGION_SHIFT)
/* Are instruction and data regions configured separately?  1 for yes and 0 otherwise. */
#define MPU_TYPE_SEPARATE           0x1

/* Memory Protection Unit Control Register Bits. */
/* Default memory map as background region for privileged access. 1 enables. */
#define MPU_CTRL_PRIVDEFENA         (1 << 2)
/* Hard fault and NMI exceptions to use MPU. 0 disables MPU for these handlers. */
#define MPU_CTRL_HFNMIENA           (1 << 1)
/* MPU Enable.  1 enables and disabled otherwise. */
#define MPU_CTRL_ENABLE             1

/* Memory Protection Unit Region Region Number Register Bits. */
#define MPU_RNR_REGION_MASK         0xFF

/* Memory Protection Unit Region Base Address Register Bits. */
/* Base address of this region. */
#define MPU_RBAR_ADDR_SHIFT         5
#define MPU_RBAR_ADDR_MASK          (0x7FFFFFF << MPU_RBAR_ADDR_SHIFT)
/* Are the region bits in this register valid or should RNR be used instead. */
#define MPU_RBAR_VALID              (1 << 4)
/* The region number.  Only used when MPU_RBAR_VALID is one. */
#define MPU_RBAR_REGION_MASK        0xF

/* Memory Protection Unit Region Attribute and Size Register Bits. */
/* eXecute Never bit.  1 means code can't execute from this region. */
#define MPU_RASR_XN                 (1 << 28)
/* Access permission bits. */
#define MPU_RASR_AP_SHIFT           24
#define MPU_RASR_AP_MASK            (0x7 << MPU_RASR_AP_SHIFT)
/* TEX, C, and B bits together determine memory type. */
#define MPU_RASR_TEX_SHIFT          19
#define MPU_RASR_TEX_MASK           (0x7 << MPU_RASR_TEX_SHIFT)
#define MPU_RASR_S                  (1 << 18)
#define MPU_RASR_C                  (1 << 17)
#define MPU_RASR_B                  (1 << 16)
/* Sub-region disable bits. */
#define MPU_RASR_SRD_SHIFT          8
#define MPU_RASR_SRD_MASK           (0xff << MPU_RASR_SRD_SHIFT)
/* Region size in 2^(value + 1) */
#define MPU_RASR_SIZE_SHIFT         1
#define MPU_RASR_SIZE_MASK          (0x1F << MPU_RASR_SIZE_SHIFT)
/* Region enable.  1 enables. */
#define MPU_RASR_ENABLE             1


/* MPU - Memory Protection Unit Routines. */
static __INLINE uint32_t getMPUDataRegionCount(void)
{
    return (MPU->TYPE & MPU_TYPE_DREGION_MASK) >> MPU_TYPE_DREGION_SHIFT;
}

static __INLINE uint32_t getHighestMPUDataRegionIndex(void)
{
    return getMPUDataRegionCount() - 1;
}

static __INLINE int isMPURegionNumberValid(uint32_t regionNumber)
{
    return regionNumber < getMPUDataRegionCount();
}

static __INLINE int isMPUNotPresent(void)
{
    return getMPUDataRegionCount() == 0;
}

static __INLINE uint32_t getMPUControlValue(void)
{
    if (isMPUNotPresent())
        return ~0U;

    return (MPU->CTRL);

}

static __INLINE void setMPUControlValue(uint32_t newControlValue)
{
    if (isMPUNotPresent())
        return;

    MPU->CTRL = newControlValue;
    __DSB();
    __ISB();
}

static __INLINE void disableMPU(void)
{
    if (isMPUNotPresent())
        return;

    MPU->CTRL &= ~MPU_CTRL_ENABLE;
    __DSB();
    __ISB();
}

static __INLINE void enableMPU(void)
{
    if (isMPUNotPresent())
        return;

    MPU->CTRL = MPU_CTRL_ENABLE;
    __DSB();
    __ISB();
}

static __INLINE void enableMPUWithHardAndNMIFaults(void)
{
    if (isMPUNotPresent())
        return;

    MPU->CTRL = MPU_CTRL_ENABLE | MPU_CTRL_HFNMIENA;
    __DSB();
    __ISB();
}

static __INLINE void enableMPUWithDefaultMemoryMap(void)
{
    if (isMPUNotPresent())
        return;

    MPU->CTRL = MPU_CTRL_ENABLE | MPU_CTRL_PRIVDEFENA;
    __DSB();
    __ISB();
}

static __INLINE int prepareToAccessMPURegion(uint32_t regionNumber)
{
    if (!isMPURegionNumberValid(regionNumber))
        return 0;

    MPU->RNR = regionNumber;
    return 1;
}

static __INLINE uint32_t getCurrentMPURegionNumber(void)
{
    return MPU->RNR;
}

static __INLINE void setMPURegionAddress(uint32_t address)
{
    if (isMPUNotPresent())
        return;

    MPU->RBAR = address & MPU_RBAR_ADDR_MASK;
}

static __INLINE uint32_t getMPURegionAddress(void)
{
    if (isMPUNotPresent())
        return 0;

    return MPU->RBAR & MPU_RBAR_ADDR_MASK;
}

static __INLINE void setMPURegionAttributeAndSize(uint32_t attributeAndSize)
{
    if (isMPUNotPresent())
        return;

    MPU->RASR = attributeAndSize;
}

static __INLINE uint32_t getMPURegionAttributeAndSize(void)
{
    if (isMPUNotPresent())
        return 0;

    return MPU->RASR;
}

#endif /* _MPU_H_ */
