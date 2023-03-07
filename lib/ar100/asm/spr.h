/*
 * Copyright © 2014 OpenRISC Project Maintainers. All rights reserved.
 * SPDX-License-Identifier: BSD-1-Clause
 */

#ifndef COMMON_ARCH_SPR_H
#define COMMON_ARCH_SPR_H

#ifdef __ASSEMBLER__
#define U(n)          (n)
#else
#define U(n)          (n ## U)
#endif

#define SPR_GROUP_BITS 5
#define SPR_GROUP_LSB  11
#define SPR_GROUP_MSB  15
#define SPR_INDEX_BITS 11
#define SPR_INDEX_LSB  0
#define SPR_INDEX_MSB  10

/****************/
/* System Group */
/****************/
#define SPR_SYS_GROUP                    0x00

/* Version Register */
#define SPR_SYS_VR_INDEX                 U(0x000)
#define SPR_SYS_VR_ADDR                  U(0x0000)

/* Revision */
#define SPR_SYS_VR_REV_LSB               0
#define SPR_SYS_VR_REV_MSB               5
#define SPR_SYS_VR_REV_BITS              6
#define SPR_SYS_VR_REV_MASK              U(0x0000003f)
#define SPR_SYS_VR_REV_GET(x)            (((x) >> 0) & U(0x0000003f))
#define SPR_SYS_VR_REV_SET(x, y)         (((x) & U(0xffffffc0)) | \
    ((y) << 0))

/* Updated Version Registers Present */
#define SPR_SYS_VR_UVRP_OFFSET           6
#define SPR_SYS_VR_UVRP_MASK             0x00000040
#define SPR_SYS_VR_UVRP_GET(x)           (((x) >> 6) & 0x1)
#define SPR_SYS_VR_UVRP_SET(x, y)        (((x) & U(0xffffffbf)) | \
    ((!!(y)) << 6))

/* Configuration Template */
#define SPR_SYS_VR_CFG_LSB               16
#define SPR_SYS_VR_CFG_MSB               23
#define SPR_SYS_VR_CFG_BITS              8
#define SPR_SYS_VR_CFG_MASK              U(0x00ff0000)
#define SPR_SYS_VR_CFG_GET(x)            (((x) >> 16) & U(0x000000ff))
#define SPR_SYS_VR_CFG_SET(x, y)         (((x) & U(0xff00ffff)) | \
    ((y) << 16))

/* Version */
#define SPR_SYS_VR_VER_LSB               24
#define SPR_SYS_VR_VER_MSB               31
#define SPR_SYS_VR_VER_BITS              8
#define SPR_SYS_VR_VER_MASK              U(0xff000000)
#define SPR_SYS_VR_VER_GET(x)            (((x) >> 24) & U(0x000000ff))
#define SPR_SYS_VR_VER_SET(x, y)         (((x) & U(0x00ffffff)) | \
    ((y) << 24))

/* Unit Present Register */
#define SPR_SYS_UPR_INDEX                U(0x001)
#define SPR_SYS_UPR_ADDR                 U(0x0001)

/* UPR Present */
#define SPR_SYS_UPR_UP_OFFSET            0
#define SPR_SYS_UPR_UP_MASK              0x00000001
#define SPR_SYS_UPR_UP_GET(x)            (((x) >> 0) & 0x1)
#define SPR_SYS_UPR_UP_SET(x, y)         (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Data Cache Present */
#define SPR_SYS_UPR_DCP_OFFSET           1
#define SPR_SYS_UPR_DCP_MASK             0x00000002
#define SPR_SYS_UPR_DCP_GET(x)           (((x) >> 1) & 0x1)
#define SPR_SYS_UPR_DCP_SET(x, y)        (((x) & U(0xfffffffd)) | \
    ((!!(y)) << 1))

/* Instruction Cache Present */
#define SPR_SYS_UPR_ICP_OFFSET           2
#define SPR_SYS_UPR_ICP_MASK             0x00000004
#define SPR_SYS_UPR_ICP_GET(x)           (((x) >> 2) & 0x1)
#define SPR_SYS_UPR_ICP_SET(x, y)        (((x) & U(0xfffffffb)) | \
    ((!!(y)) << 2))

/* Data MMU Present */
#define SPR_SYS_UPR_DMP_OFFSET           3
#define SPR_SYS_UPR_DMP_MASK             0x00000008
#define SPR_SYS_UPR_DMP_GET(x)           (((x) >> 3) & 0x1)
#define SPR_SYS_UPR_DMP_SET(x, y)        (((x) & U(0xfffffff7)) | \
    ((!!(y)) << 3))

/* Instruction MMU Present */
#define SPR_SYS_UPR_IMP_OFFSET           4
#define SPR_SYS_UPR_IMP_MASK             0x00000010
#define SPR_SYS_UPR_IMP_GET(x)           (((x) >> 4) & 0x1)
#define SPR_SYS_UPR_IMP_SET(x, y)        (((x) & U(0xffffffef)) | \
    ((!!(y)) << 4))

/* MAC Present */
#define SPR_SYS_UPR_MP_OFFSET            5
#define SPR_SYS_UPR_MP_MASK              0x00000020
#define SPR_SYS_UPR_MP_GET(x)            (((x) >> 5) & 0x1)
#define SPR_SYS_UPR_MP_SET(x, y)         (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* Debug Unit Present */
#define SPR_SYS_UPR_DUP_OFFSET           6
#define SPR_SYS_UPR_DUP_MASK             0x00000040
#define SPR_SYS_UPR_DUP_GET(x)           (((x) >> 6) & 0x1)
#define SPR_SYS_UPR_DUP_SET(x, y)        (((x) & U(0xffffffbf)) | \
    ((!!(y)) << 6))

/* Performance Counters Unit Present */
#define SPR_SYS_UPR_PCUP_OFFSET          7
#define SPR_SYS_UPR_PCUP_MASK            0x00000080
#define SPR_SYS_UPR_PCUP_GET(x)          (((x) >> 7) & 0x1)
#define SPR_SYS_UPR_PCUP_SET(x, y)       (((x) & U(0xffffff7f)) | \
    ((!!(y)) << 7))

/* Power Management Present */
#define SPR_SYS_UPR_PICP_OFFSET          8
#define SPR_SYS_UPR_PICP_MASK            0x00000100
#define SPR_SYS_UPR_PICP_GET(x)          (((x) >> 8) & 0x1)
#define SPR_SYS_UPR_PICP_SET(x, y)       (((x) & U(0xfffffeff)) | \
    ((!!(y)) << 8))

/* Programmable Interrupt Controller Present */
#define SPR_SYS_UPR_PMP_OFFSET           9
#define SPR_SYS_UPR_PMP_MASK             0x00000200
#define SPR_SYS_UPR_PMP_GET(x)           (((x) >> 9) & 0x1)
#define SPR_SYS_UPR_PMP_SET(x, y)        (((x) & U(0xfffffdff)) | \
    ((!!(y)) << 9))

/* Tick Timer Present */
#define SPR_SYS_UPR_TTP_OFFSET           10
#define SPR_SYS_UPR_TTP_MASK             0x00000400
#define SPR_SYS_UPR_TTP_GET(x)           (((x) >> 10) & 0x1)
#define SPR_SYS_UPR_TTP_SET(x, y)        (((x) & U(0xfffffbff)) | \
    ((!!(y)) << 10))

/* Custom Units Present */
#define SPR_SYS_UPR_CUP_LSB              24
#define SPR_SYS_UPR_CUP_MSB              31
#define SPR_SYS_UPR_CUP_BITS             8
#define SPR_SYS_UPR_CUP_MASK             U(0xff000000)
#define SPR_SYS_UPR_CUP_GET(x)           (((x) >> 24) & U(0x000000ff))
#define SPR_SYS_UPR_CUP_SET(x, y)        (((x) & U(0x00ffffff)) | \
    ((y) << 24))

/* CPU Configuration Register */
#define SPR_SYS_CPUCFGR_INDEX            U(0x002)
#define SPR_SYS_CPUCFGR_ADDR             U(0x0002)

/* Number of Shadow GPR Files */
#define SPR_SYS_CPUCFGR_NSGF_LSB         0
#define SPR_SYS_CPUCFGR_NSGF_MSB         3
#define SPR_SYS_CPUCFGR_NSGF_BITS        4
#define SPR_SYS_CPUCFGR_NSGF_MASK        U(0x0000000f)
#define SPR_SYS_CPUCFGR_NSGF_GET(x)      (((x) >> 0) & U(0x0000000f))
#define SPR_SYS_CPUCFGR_NSGF_SET(x, y)   (((x) & U(0xfffffff0)) | \
    ((y) << 0))

/* Custom GPR File */
#define SPR_SYS_CPUCFGR_CGF_OFFSET       4
#define SPR_SYS_CPUCFGR_CGF_MASK         0x00000010
#define SPR_SYS_CPUCFGR_CGF_GET(x)       (((x) >> 4) & 0x1)
#define SPR_SYS_CPUCFGR_CGF_SET(x, y)    (((x) & U(0xffffffef)) | \
    ((!!(y)) << 4))

/* ORBIS32 Supported */
#define SPR_SYS_CPUCFGR_OB32S_OFFSET     5
#define SPR_SYS_CPUCFGR_OB32S_MASK       0x00000020
#define SPR_SYS_CPUCFGR_OB32S_GET(x)     (((x) >> 5) & 0x1)
#define SPR_SYS_CPUCFGR_OB32S_SET(x, y)  (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* ORBIS64 Supported */
#define SPR_SYS_CPUCFGR_OB64S_OFFSET     6
#define SPR_SYS_CPUCFGR_OB64S_MASK       0x00000040
#define SPR_SYS_CPUCFGR_OB64S_GET(x)     (((x) >> 6) & 0x1)
#define SPR_SYS_CPUCFGR_OB64S_SET(x, y)  (((x) & U(0xffffffbf)) | \
    ((!!(y)) << 6))

/* ORFPX32 Supported */
#define SPR_SYS_CPUCFGR_OF32S_OFFSET     7
#define SPR_SYS_CPUCFGR_OF32S_MASK       0x00000080
#define SPR_SYS_CPUCFGR_OF32S_GET(x)     (((x) >> 7) & 0x1)
#define SPR_SYS_CPUCFGR_OF32S_SET(x, y)  (((x) & U(0xffffff7f)) | \
    ((!!(y)) << 7))

/* ORFPX64 Supported */
#define SPR_SYS_CPUCFGR_OF64S_OFFSET     8
#define SPR_SYS_CPUCFGR_OF64S_MASK       0x00000100
#define SPR_SYS_CPUCFGR_OF64S_GET(x)     (((x) >> 8) & 0x1)
#define SPR_SYS_CPUCFGR_OF64S_SET(x, y)  (((x) & U(0xfffffeff)) | \
    ((!!(y)) << 8))

/* ORVDX64 Supported */
#define SPR_SYS_CPUCFGR_OV64S_OFFSET     9
#define SPR_SYS_CPUCFGR_OV64S_MASK       0x00000200
#define SPR_SYS_CPUCFGR_OV64S_GET(x)     (((x) >> 9) & 0x1)
#define SPR_SYS_CPUCFGR_OV64S_SET(x, y)  (((x) & U(0xfffffdff)) | \
    ((!!(y)) << 9))

/* No Delay-Slot */
#define SPR_SYS_CPUCFGR_ND_OFFSET        10
#define SPR_SYS_CPUCFGR_ND_MASK          0x00000400
#define SPR_SYS_CPUCFGR_ND_GET(x)        (((x) >> 10) & 0x1)
#define SPR_SYS_CPUCFGR_ND_SET(x, y)     (((x) & U(0xfffffbff)) | \
    ((!!(y)) << 10))

/* Architecture Version Register Present */
#define SPR_SYS_CPUCFGR_AVRP_OFFSET      11
#define SPR_SYS_CPUCFGR_AVRP_MASK        0x00000800
#define SPR_SYS_CPUCFGR_AVRP_GET(x)      (((x) >> 11) & 0x1)
#define SPR_SYS_CPUCFGR_AVRP_SET(x, y)   (((x) & U(0xfffff7ff)) | \
    ((!!(y)) << 11))

/* Exception Vector Base Address Register Present */
#define SPR_SYS_CPUCFGR_EVBARP_OFFSET    12
#define SPR_SYS_CPUCFGR_EVBARP_MASK      0x00001000
#define SPR_SYS_CPUCFGR_EVBARP_GET(x)    (((x) >> 12) & 0x1)
#define SPR_SYS_CPUCFGR_EVBARP_SET(x, y) (((x) & U(0xffffefff)) | \
    ((!!(y)) << 12))

/* Implementation-Specific Registers (ISR0-7) Present */
#define SPR_SYS_CPUCFGR_ISRP_OFFSET      13
#define SPR_SYS_CPUCFGR_ISRP_MASK        0x00002000
#define SPR_SYS_CPUCFGR_ISRP_GET(x)      (((x) >> 13) & 0x1)
#define SPR_SYS_CPUCFGR_ISRP_SET(x, y)   (((x) & U(0xffffdfff)) | \
    ((!!(y)) << 13))

/* Arithmetic Exception Control/Status Registers Present */
#define SPR_SYS_CPUCFGR_AECSRP_OFFSET    14
#define SPR_SYS_CPUCFGR_AECSRP_MASK      0x00004000
#define SPR_SYS_CPUCFGR_AECSRP_GET(x)    (((x) >> 14) & 0x1)
#define SPR_SYS_CPUCFGR_AECSRP_SET(x, y) (((x) & U(0xffffbfff)) | \
    ((!!(y)) << 14))

/* Data MMU Configuration Register */
#define SPR_SYS_DMMUCFGR_INDEX           U(0x003)
#define SPR_SYS_DMMUCFGR_ADDR            U(0x0003)

/* Number of TLB Ways */
#define SPR_SYS_DMMUCFGR_NTW_LSB         0
#define SPR_SYS_DMMUCFGR_NTW_MSB         1
#define SPR_SYS_DMMUCFGR_NTW_BITS        2
#define SPR_SYS_DMMUCFGR_NTW_MASK        U(0x00000003)
#define SPR_SYS_DMMUCFGR_NTW_GET(x)      (((x) >> 0) & U(0x00000003))
#define SPR_SYS_DMMUCFGR_NTW_SET(x, y)   (((x) & U(0xfffffffc)) | \
    ((y) << 0))

/* Number of TLB Sets */
#define SPR_SYS_DMMUCFGR_NTS_LSB         2
#define SPR_SYS_DMMUCFGR_NTS_MSB         4
#define SPR_SYS_DMMUCFGR_NTS_BITS        3
#define SPR_SYS_DMMUCFGR_NTS_MASK        U(0x0000001c)
#define SPR_SYS_DMMUCFGR_NTS_GET(x)      (((x) >> 2) & U(0x00000007))
#define SPR_SYS_DMMUCFGR_NTS_SET(x, y)   (((x) & U(0xffffffe3)) | \
    ((y) << 2))

/* Number of ATB Entries */
#define SPR_SYS_DMMUCFGR_NAE_LSB         5
#define SPR_SYS_DMMUCFGR_NAE_MSB         7
#define SPR_SYS_DMMUCFGR_NAE_BITS        3
#define SPR_SYS_DMMUCFGR_NAE_MASK        U(0x000000e0)
#define SPR_SYS_DMMUCFGR_NAE_GET(x)      (((x) >> 5) & U(0x00000007))
#define SPR_SYS_DMMUCFGR_NAE_SET(x, y)   (((x) & U(0xffffff1f)) | \
    ((y) << 5))

/* Control Register Implemented */
#define SPR_SYS_DMMUCFGR_CRI_OFFSET      8
#define SPR_SYS_DMMUCFGR_CRI_MASK        0x00000100
#define SPR_SYS_DMMUCFGR_CRI_GET(x)      (((x) >> 8) & 0x1)
#define SPR_SYS_DMMUCFGR_CRI_SET(x, y)   (((x) & U(0xfffffeff)) | \
    ((!!(y)) << 8))

/* Protection Register Implemented */
#define SPR_SYS_DMMUCFGR_PRI_OFFSET      9
#define SPR_SYS_DMMUCFGR_PRI_MASK        0x00000200
#define SPR_SYS_DMMUCFGR_PRI_GET(x)      (((x) >> 9) & 0x1)
#define SPR_SYS_DMMUCFGR_PRI_SET(x, y)   (((x) & U(0xfffffdff)) | \
    ((!!(y)) << 9))

/* TLB Entry Invalidate Register Implemented */
#define SPR_SYS_DMMUCFGR_TEIRI_OFFSET    10
#define SPR_SYS_DMMUCFGR_TEIRI_MASK      0x00000400
#define SPR_SYS_DMMUCFGR_TEIRI_GET(x)    (((x) >> 10) & 0x1)
#define SPR_SYS_DMMUCFGR_TEIRI_SET(x, y) (((x) & U(0xfffffbff)) | \
    ((!!(y)) << 10))

/* Hardware TLB Reload */
#define SPR_SYS_DMMUCFGR_HTR_OFFSET      11
#define SPR_SYS_DMMUCFGR_HTR_MASK        0x00000800
#define SPR_SYS_DMMUCFGR_HTR_GET(x)      (((x) >> 11) & 0x1)
#define SPR_SYS_DMMUCFGR_HTR_SET(x, y)   (((x) & U(0xfffff7ff)) | \
    ((!!(y)) << 11))

/* DTLB reloaded in software */
#define SPR_SYS_DMMUCFGR_HTR_SW          0
/* DTLB reloaded in hardware */
#define SPR_SYS_DMMUCFGR_HTR_HW          1

/* Instruction MMU Configuration Register */
#define SPR_SYS_IMMUCFGR_INDEX           U(0x004)
#define SPR_SYS_IMMUCFGR_ADDR            U(0x0004)

/* Number of TLB Ways */
#define SPR_SYS_IMMUCFGR_NTW_LSB         0
#define SPR_SYS_IMMUCFGR_NTW_MSB         1
#define SPR_SYS_IMMUCFGR_NTW_BITS        2
#define SPR_SYS_IMMUCFGR_NTW_MASK        U(0x00000003)
#define SPR_SYS_IMMUCFGR_NTW_GET(x)      (((x) >> 0) & U(0x00000003))
#define SPR_SYS_IMMUCFGR_NTW_SET(x, y)   (((x) & U(0xfffffffc)) | \
    ((y) << 0))

/* Number of TLB Sets */
#define SPR_SYS_IMMUCFGR_NTS_LSB         2
#define SPR_SYS_IMMUCFGR_NTS_MSB         4
#define SPR_SYS_IMMUCFGR_NTS_BITS        3
#define SPR_SYS_IMMUCFGR_NTS_MASK        U(0x0000001c)
#define SPR_SYS_IMMUCFGR_NTS_GET(x)      (((x) >> 2) & U(0x00000007))
#define SPR_SYS_IMMUCFGR_NTS_SET(x, y)   (((x) & U(0xffffffe3)) | \
    ((y) << 2))

/* Number of ATB Entries */
#define SPR_SYS_IMMUCFGR_NAE_LSB         5
#define SPR_SYS_IMMUCFGR_NAE_MSB         7
#define SPR_SYS_IMMUCFGR_NAE_BITS        3
#define SPR_SYS_IMMUCFGR_NAE_MASK        U(0x000000e0)
#define SPR_SYS_IMMUCFGR_NAE_GET(x)      (((x) >> 5) & U(0x00000007))
#define SPR_SYS_IMMUCFGR_NAE_SET(x, y)   (((x) & U(0xffffff1f)) | \
    ((y) << 5))

/* Control Register Implemented */
#define SPR_SYS_IMMUCFGR_CRI_OFFSET      8
#define SPR_SYS_IMMUCFGR_CRI_MASK        0x00000100
#define SPR_SYS_IMMUCFGR_CRI_GET(x)      (((x) >> 8) & 0x1)
#define SPR_SYS_IMMUCFGR_CRI_SET(x, y)   (((x) & U(0xfffffeff)) | \
    ((!!(y)) << 8))

/* Protection Register Implemented */
#define SPR_SYS_IMMUCFGR_PRI_OFFSET      9
#define SPR_SYS_IMMUCFGR_PRI_MASK        0x00000200
#define SPR_SYS_IMMUCFGR_PRI_GET(x)      (((x) >> 9) & 0x1)
#define SPR_SYS_IMMUCFGR_PRI_SET(x, y)   (((x) & U(0xfffffdff)) | \
    ((!!(y)) << 9))

/* TLB Entry Invalidate Register Implemented */
#define SPR_SYS_IMMUCFGR_TEIRI_OFFSET    10
#define SPR_SYS_IMMUCFGR_TEIRI_MASK      0x00000400
#define SPR_SYS_IMMUCFGR_TEIRI_GET(x)    (((x) >> 10) & 0x1)
#define SPR_SYS_IMMUCFGR_TEIRI_SET(x, y) (((x) & U(0xfffffbff)) | \
    ((!!(y)) << 10))

/* Hardware TLB Reload */
#define SPR_SYS_IMMUCFGR_HTR_OFFSET      11
#define SPR_SYS_IMMUCFGR_HTR_MASK        0x00000800
#define SPR_SYS_IMMUCFGR_HTR_GET(x)      (((x) >> 11) & 0x1)
#define SPR_SYS_IMMUCFGR_HTR_SET(x, y)   (((x) & U(0xfffff7ff)) | \
    ((!!(y)) << 11))

/* DTLB reloaded in software */
#define SPR_SYS_IMMUCFGR_HTR_SW          0
/* DTLB reloaded in hardware */
#define SPR_SYS_IMMUCFGR_HTR_HW          1

/* Data Cache Configuration Register */
#define SPR_SYS_DCCFGR_INDEX             U(0x005)
#define SPR_SYS_DCCFGR_ADDR              U(0x0005)

/* Number of Cache Ways */
#define SPR_SYS_DCCFGR_NCW_LSB           0
#define SPR_SYS_DCCFGR_NCW_MSB           2
#define SPR_SYS_DCCFGR_NCW_BITS          3
#define SPR_SYS_DCCFGR_NCW_MASK          U(0x00000007)
#define SPR_SYS_DCCFGR_NCW_GET(x)        (((x) >> 0) & U(0x00000007))
#define SPR_SYS_DCCFGR_NCW_SET(x, y)     (((x) & U(0xfffffff8)) | \
    ((y) << 0))

/* Number of Cache Sets */
#define SPR_SYS_DCCFGR_NCS_LSB           3
#define SPR_SYS_DCCFGR_NCS_MSB           6
#define SPR_SYS_DCCFGR_NCS_BITS          4
#define SPR_SYS_DCCFGR_NCS_MASK          U(0x00000078)
#define SPR_SYS_DCCFGR_NCS_GET(x)        (((x) >> 3) & U(0x0000000f))
#define SPR_SYS_DCCFGR_NCS_SET(x, y)     (((x) & U(0xffffff87)) | \
    ((y) << 3))

/* Cache Block Size */
#define SPR_SYS_DCCFGR_CBS_OFFSET        7
#define SPR_SYS_DCCFGR_CBS_MASK          0x00000080
#define SPR_SYS_DCCFGR_CBS_GET(x)        (((x) >> 7) & 0x1)
#define SPR_SYS_DCCFGR_CBS_SET(x, y)     (((x) & U(0xffffff7f)) | \
    ((!!(y)) << 7))

/* 16 Bytes */
#define SPR_SYS_DCCFGR_CBS_16            0
/* 32 Bytes */
#define SPR_SYS_DCCFGR_CBS_32            1

/* Cache Write Strategy */
#define SPR_SYS_DCCFGR_CWS_OFFSET        8
#define SPR_SYS_DCCFGR_CWS_MASK          0x00000100
#define SPR_SYS_DCCFGR_CWS_GET(x)        (((x) >> 8) & 0x1)
#define SPR_SYS_DCCFGR_CWS_SET(x, y)     (((x) & U(0xfffffeff)) | \
    ((!!(y)) << 8))

/* Write Through */
#define SPR_SYS_DCCFGR_CWS_WT            0
/* Write Back */
#define SPR_SYS_DCCFGR_CWS_WB            1

/* Cache Control Register Implemented */
#define SPR_SYS_DCCFGR_CCRI_OFFSET       9
#define SPR_SYS_DCCFGR_CCRI_MASK         0x00000200
#define SPR_SYS_DCCFGR_CCRI_GET(x)       (((x) >> 9) & 0x1)
#define SPR_SYS_DCCFGR_CCRI_SET(x, y)    (((x) & U(0xfffffdff)) | \
    ((!!(y)) << 9))

/* Cache Block Invalidate Register Implemented */
#define SPR_SYS_DCCFGR_CBIRI_OFFSET      10
#define SPR_SYS_DCCFGR_CBIRI_MASK        0x00000400
#define SPR_SYS_DCCFGR_CBIRI_GET(x)      (((x) >> 10) & 0x1)
#define SPR_SYS_DCCFGR_CBIRI_SET(x, y)   (((x) & U(0xfffffbff)) | \
    ((!!(y)) << 10))

/* Cache Block Prefetch Register Implemented */
#define SPR_SYS_DCCFGR_CBPRI_OFFSET      11
#define SPR_SYS_DCCFGR_CBPRI_MASK        0x00000800
#define SPR_SYS_DCCFGR_CBPRI_GET(x)      (((x) >> 11) & 0x1)
#define SPR_SYS_DCCFGR_CBPRI_SET(x, y)   (((x) & U(0xfffff7ff)) | \
    ((!!(y)) << 11))

/* Cache Block Lock Register Implemented */
#define SPR_SYS_DCCFGR_CBLRI_OFFSET      12
#define SPR_SYS_DCCFGR_CBLRI_MASK        0x00001000
#define SPR_SYS_DCCFGR_CBLRI_GET(x)      (((x) >> 12) & 0x1)
#define SPR_SYS_DCCFGR_CBLRI_SET(x, y)   (((x) & U(0xffffefff)) | \
    ((!!(y)) << 12))

/* Cache Block Flush Register Implemented */
#define SPR_SYS_DCCFGR_CBFRI_OFFSET      13
#define SPR_SYS_DCCFGR_CBFRI_MASK        0x00002000
#define SPR_SYS_DCCFGR_CBFRI_GET(x)      (((x) >> 13) & 0x1)
#define SPR_SYS_DCCFGR_CBFRI_SET(x, y)   (((x) & U(0xffffdfff)) | \
    ((!!(y)) << 13))

/* Cache Block Write-back Register Implemented */
#define SPR_SYS_DCCFGR_CBWBRI_OFFSET     14
#define SPR_SYS_DCCFGR_CBWBRI_MASK       0x00004000
#define SPR_SYS_DCCFGR_CBWBRI_GET(x)     (((x) >> 14) & 0x1)
#define SPR_SYS_DCCFGR_CBWBRI_SET(x, y)  (((x) & U(0xffffbfff)) | \
    ((!!(y)) << 14))

/* Instruction Cache Configuration Register */
#define SPR_SYS_ICCFGR_INDEX             U(0x006)
#define SPR_SYS_ICCFGR_ADDR              U(0x0006)

/* Number of Cache Ways */
#define SPR_SYS_ICCFGR_NCW_LSB           0
#define SPR_SYS_ICCFGR_NCW_MSB           2
#define SPR_SYS_ICCFGR_NCW_BITS          3
#define SPR_SYS_ICCFGR_NCW_MASK          U(0x00000007)
#define SPR_SYS_ICCFGR_NCW_GET(x)        (((x) >> 0) & U(0x00000007))
#define SPR_SYS_ICCFGR_NCW_SET(x, y)     (((x) & U(0xfffffff8)) | \
    ((y) << 0))

/* Number of Cache Sets */
#define SPR_SYS_ICCFGR_NCS_LSB           3
#define SPR_SYS_ICCFGR_NCS_MSB           6
#define SPR_SYS_ICCFGR_NCS_BITS          4
#define SPR_SYS_ICCFGR_NCS_MASK          U(0x00000078)
#define SPR_SYS_ICCFGR_NCS_GET(x)        (((x) >> 3) & U(0x0000000f))
#define SPR_SYS_ICCFGR_NCS_SET(x, y)     (((x) & U(0xffffff87)) | \
    ((y) << 3))

/* Cache Block Size */
#define SPR_SYS_ICCFGR_CBS_OFFSET        7
#define SPR_SYS_ICCFGR_CBS_MASK          0x00000080
#define SPR_SYS_ICCFGR_CBS_GET(x)        (((x) >> 7) & 0x1)
#define SPR_SYS_ICCFGR_CBS_SET(x, y)     (((x) & U(0xffffff7f)) | \
    ((!!(y)) << 7))

/* 16 Bytes */
#define SPR_SYS_ICCFGR_CBS_16            0
/* 32 Bytes */
#define SPR_SYS_ICCFGR_CBS_32            1

/* Cache Control Register Implemented */
#define SPR_SYS_ICCFGR_CCRI_OFFSET       9
#define SPR_SYS_ICCFGR_CCRI_MASK         0x00000200
#define SPR_SYS_ICCFGR_CCRI_GET(x)       (((x) >> 9) & 0x1)
#define SPR_SYS_ICCFGR_CCRI_SET(x, y)    (((x) & U(0xfffffdff)) | \
    ((!!(y)) << 9))

/* Cache Block Invalidate Register Implemented */
#define SPR_SYS_ICCFGR_CBIRI_OFFSET      10
#define SPR_SYS_ICCFGR_CBIRI_MASK        0x00000400
#define SPR_SYS_ICCFGR_CBIRI_GET(x)      (((x) >> 10) & 0x1)
#define SPR_SYS_ICCFGR_CBIRI_SET(x, y)   (((x) & U(0xfffffbff)) | \
    ((!!(y)) << 10))

/* Cache Block Prefetch Register Implemented */
#define SPR_SYS_ICCFGR_CBPRI_OFFSET      11
#define SPR_SYS_ICCFGR_CBPRI_MASK        0x00000800
#define SPR_SYS_ICCFGR_CBPRI_GET(x)      (((x) >> 11) & 0x1)
#define SPR_SYS_ICCFGR_CBPRI_SET(x, y)   (((x) & U(0xfffff7ff)) | \
    ((!!(y)) << 11))

/* Cache Block Lock Register Implemented */
#define SPR_SYS_ICCFGR_CBLRI_OFFSET      12
#define SPR_SYS_ICCFGR_CBLRI_MASK        0x00001000
#define SPR_SYS_ICCFGR_CBLRI_GET(x)      (((x) >> 12) & 0x1)
#define SPR_SYS_ICCFGR_CBLRI_SET(x, y)   (((x) & U(0xffffefff)) | \
    ((!!(y)) << 12))

/* Debug Configuration Register */
#define SPR_SYS_DCFGR_INDEX              U(0x007)
#define SPR_SYS_DCFGR_ADDR               U(0x0007)

/* Number of Debug Pairs */
#define SPR_SYS_DCFGR_NDP_LSB            0
#define SPR_SYS_DCFGR_NDP_MSB            2
#define SPR_SYS_DCFGR_NDP_BITS           3
#define SPR_SYS_DCFGR_NDP_MASK           U(0x00000007)
#define SPR_SYS_DCFGR_NDP_GET(x)         (((x) >> 0) & U(0x00000007))
#define SPR_SYS_DCFGR_NDP_SET(x, y)      (((x) & U(0xfffffff8)) | \
    ((y) << 0))

/* Watchpoint Counters Implemented */
#define SPR_SYS_DCFGR_WPCI_OFFSET        3
#define SPR_SYS_DCFGR_WPCI_MASK          0x00000008
#define SPR_SYS_DCFGR_WPCI_GET(x)        (((x) >> 3) & 0x1)
#define SPR_SYS_DCFGR_WPCI_SET(x, y)     (((x) & U(0xfffffff7)) | \
    ((!!(y)) << 3))

/* Performance Counters Configuration */
#define SPR_SYS_PCCFGR_INDEX             U(0x008)
#define SPR_SYS_PCCFGR_ADDR              U(0x0008)

/* Number of Performance Counters */
#define SPR_SYS_PCCFGR_NPC_LSB           0
#define SPR_SYS_PCCFGR_NPC_MSB           2
#define SPR_SYS_PCCFGR_NPC_BITS          3
#define SPR_SYS_PCCFGR_NPC_MASK          U(0x00000007)
#define SPR_SYS_PCCFGR_NPC_GET(x)        (((x) >> 0) & U(0x00000007))
#define SPR_SYS_PCCFGR_NPC_SET(x, y)     (((x) & U(0xfffffff8)) | \
    ((y) << 0))

/* Version Register 2 */
#define SPR_SYS_VR2_INDEX                U(0x009)
#define SPR_SYS_VR2_ADDR                 U(0x0009)

/* Version */
#define SPR_SYS_VR2_VER_LSB              0
#define SPR_SYS_VR2_VER_MSB              23
#define SPR_SYS_VR2_VER_BITS             24
#define SPR_SYS_VR2_VER_MASK             U(0x00ffffff)
#define SPR_SYS_VR2_VER_GET(x)           (((x) >> 0) & U(0x00ffffff))
#define SPR_SYS_VR2_VER_SET(x, y)        (((x) & U(0xff000000)) | \
    ((y) << 0))

/* CPU Identification Number */
#define SPR_SYS_VR2_CPUID_LSB            24
#define SPR_SYS_VR2_CPUID_MSB            31
#define SPR_SYS_VR2_CPUID_BITS           8
#define SPR_SYS_VR2_CPUID_MASK           U(0xff000000)
#define SPR_SYS_VR2_CPUID_GET(x)         (((x) >> 24) & U(0x000000ff))
#define SPR_SYS_VR2_CPUID_SET(x, y)      (((x) & U(0x00ffffff)) | \
    ((y) << 24))

/* Architecture Version Register */
#define SPR_SYS_AVR_INDEX                U(0x00a)
#define SPR_SYS_AVR_ADDR                 U(0x000a)

/* Major Architecture Version Number */
#define SPR_SYS_AVR_REV_LSB              8
#define SPR_SYS_AVR_REV_MSB              15
#define SPR_SYS_AVR_REV_BITS             8
#define SPR_SYS_AVR_REV_MASK             U(0x0000ff00)
#define SPR_SYS_AVR_REV_GET(x)           (((x) >> 8) & U(0x000000ff))
#define SPR_SYS_AVR_REV_SET(x, y)        (((x) & U(0xffff00ff)) | \
    ((y) << 8))

/* Minor Architecture Version Number */
#define SPR_SYS_AVR_MIN_LSB              16
#define SPR_SYS_AVR_MIN_MSB              23
#define SPR_SYS_AVR_MIN_BITS             8
#define SPR_SYS_AVR_MIN_MASK             U(0x00ff0000)
#define SPR_SYS_AVR_MIN_GET(x)           (((x) >> 16) & U(0x000000ff))
#define SPR_SYS_AVR_MIN_SET(x, y)        (((x) & U(0xff00ffff)) | \
    ((y) << 16))

/* Architecture Revision Number */
#define SPR_SYS_AVR_MAJ_LSB              24
#define SPR_SYS_AVR_MAJ_MSB              31
#define SPR_SYS_AVR_MAJ_BITS             8
#define SPR_SYS_AVR_MAJ_MASK             U(0xff000000)
#define SPR_SYS_AVR_MAJ_GET(x)           (((x) >> 24) & U(0x000000ff))
#define SPR_SYS_AVR_MAJ_SET(x, y)        (((x) & U(0x00ffffff)) | \
    ((y) << 24))

/* Exception Vector Base Address Register */
#define SPR_SYS_EVBAR_INDEX              U(0x00b)
#define SPR_SYS_EVBAR_ADDR               U(0x000b)

/* Exception Vector Base Address */
#define SPR_SYS_EVBAR_EVBA_LSB           13
#define SPR_SYS_EVBAR_EVBA_MSB           31
#define SPR_SYS_EVBAR_EVBA_BITS          19
#define SPR_SYS_EVBAR_EVBA_MASK          U(0xffffe000)
#define SPR_SYS_EVBAR_EVBA_GET(x)        (((x) >> 13) & U(0x0007ffff))
#define SPR_SYS_EVBAR_EVBA_SET(x, y)     (((x) & U(0x00001fff)) | \
    ((y) << 13))

/* Arithmetic Exception Control Register */
#define SPR_SYS_AECR_INDEX               U(0x00c)
#define SPR_SYS_AECR_ADDR                U(0x000c)

/* Carry on Add Exception Enabled */
#define SPR_SYS_AECR_CYADDE_OFFSET       0
#define SPR_SYS_AECR_CYADDE_MASK         0x00000001
#define SPR_SYS_AECR_CYADDE_GET(x)       (((x) >> 0) & 0x1)
#define SPR_SYS_AECR_CYADDE_SET(x, y)    (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Overflow on Add Exception Enabled */
#define SPR_SYS_AECR_OVADDE_OFFSET       1
#define SPR_SYS_AECR_OVADDE_MASK         0x00000002
#define SPR_SYS_AECR_OVADDE_GET(x)       (((x) >> 1) & 0x1)
#define SPR_SYS_AECR_OVADDE_SET(x, y)    (((x) & U(0xfffffffd)) | \
    ((!!(y)) << 1))

/* Carry on Multiply Exception Enabled */
#define SPR_SYS_AECR_CYMULE_OFFSET       2
#define SPR_SYS_AECR_CYMULE_MASK         0x00000004
#define SPR_SYS_AECR_CYMULE_GET(x)       (((x) >> 2) & 0x1)
#define SPR_SYS_AECR_CYMULE_SET(x, y)    (((x) & U(0xfffffffb)) | \
    ((!!(y)) << 2))

/* Overflow on Multiply Exception Enabled */
#define SPR_SYS_AECR_OVMULE_OFFSET       3
#define SPR_SYS_AECR_OVMULE_MASK         0x00000008
#define SPR_SYS_AECR_OVMULE_GET(x)       (((x) >> 3) & 0x1)
#define SPR_SYS_AECR_OVMULE_SET(x, y)    (((x) & U(0xfffffff7)) | \
    ((!!(y)) << 3))

/* Divide by Zero Exception Enabled */
#define SPR_SYS_AECR_DBZE_OFFSET         4
#define SPR_SYS_AECR_DBZE_MASK           0x00000010
#define SPR_SYS_AECR_DBZE_GET(x)         (((x) >> 4) & 0x1)
#define SPR_SYS_AECR_DBZE_SET(x, y)      (((x) & U(0xffffffef)) | \
    ((!!(y)) << 4))

/* Carry on MAC Addition Exception Enabled */
#define SPR_SYS_AECR_CYMACADDE_OFFSET    5
#define SPR_SYS_AECR_CYMACADDE_MASK      0x00000020
#define SPR_SYS_AECR_CYMACADDE_GET(x)    (((x) >> 5) & 0x1)
#define SPR_SYS_AECR_CYMACADDE_SET(x, y) (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* Overflow on MAC Addition Exception Enabled */
#define SPR_SYS_AECR_OVMACADDE_OFFSET    6
#define SPR_SYS_AECR_OVMACADDE_MASK      0x00000040
#define SPR_SYS_AECR_OVMACADDE_GET(x)    (((x) >> 6) & 0x1)
#define SPR_SYS_AECR_OVMACADDE_SET(x, y) (((x) & U(0xffffffbf)) | \
    ((!!(y)) << 6))

/* Arithmetic Exception Status Register */
#define SPR_SYS_AESR_INDEX               U(0x00d)
#define SPR_SYS_AESR_ADDR                U(0x000d)

/* Carry on Add Exception */
#define SPR_SYS_AESR_CYADDE_OFFSET       0
#define SPR_SYS_AESR_CYADDE_MASK         0x00000001
#define SPR_SYS_AESR_CYADDE_GET(x)       (((x) >> 0) & 0x1)
#define SPR_SYS_AESR_CYADDE_SET(x, y)    (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Overflow on Add Exception */
#define SPR_SYS_AESR_OVADDE_OFFSET       1
#define SPR_SYS_AESR_OVADDE_MASK         0x00000002
#define SPR_SYS_AESR_OVADDE_GET(x)       (((x) >> 1) & 0x1)
#define SPR_SYS_AESR_OVADDE_SET(x, y)    (((x) & U(0xfffffffd)) | \
    ((!!(y)) << 1))

/* Carry on Multiply Exception */
#define SPR_SYS_AESR_CYMULE_OFFSET       2
#define SPR_SYS_AESR_CYMULE_MASK         0x00000004
#define SPR_SYS_AESR_CYMULE_GET(x)       (((x) >> 2) & 0x1)
#define SPR_SYS_AESR_CYMULE_SET(x, y)    (((x) & U(0xfffffffb)) | \
    ((!!(y)) << 2))

/* Overflow on Multiply Exception */
#define SPR_SYS_AESR_OVMULE_OFFSET       3
#define SPR_SYS_AESR_OVMULE_MASK         0x00000008
#define SPR_SYS_AESR_OVMULE_GET(x)       (((x) >> 3) & 0x1)
#define SPR_SYS_AESR_OVMULE_SET(x, y)    (((x) & U(0xfffffff7)) | \
    ((!!(y)) << 3))

/* Divide by Zero Exception */
#define SPR_SYS_AESR_DBZE_OFFSET         4
#define SPR_SYS_AESR_DBZE_MASK           0x00000010
#define SPR_SYS_AESR_DBZE_GET(x)         (((x) >> 4) & 0x1)
#define SPR_SYS_AESR_DBZE_SET(x, y)      (((x) & U(0xffffffef)) | \
    ((!!(y)) << 4))

/* Carry on MAC Addition Exception */
#define SPR_SYS_AESR_CYMACADDE_OFFSET    5
#define SPR_SYS_AESR_CYMACADDE_MASK      0x00000020
#define SPR_SYS_AESR_CYMACADDE_GET(x)    (((x) >> 5) & 0x1)
#define SPR_SYS_AESR_CYMACADDE_SET(x, y) (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* Overflow on MAC Addition Exception */
#define SPR_SYS_AESR_OVMACADDE_OFFSET    6
#define SPR_SYS_AESR_OVMACADDE_MASK      0x00000040
#define SPR_SYS_AESR_OVMACADDE_GET(x)    (((x) >> 6) & 0x1)
#define SPR_SYS_AESR_OVMACADDE_SET(x, y) (((x) & U(0xffffffbf)) | \
    ((!!(y)) << 6))

/* Next Program Counter */
#define SPR_SYS_NPC_INDEX                U(0x010)
#define SPR_SYS_NPC_ADDR                 U(0x0010)

/* Supervision Register */
#define SPR_SYS_SR_INDEX                 U(0x011)
#define SPR_SYS_SR_ADDR                  U(0x0011)

/* Supervisor Mode */
#define SPR_SYS_SR_SM_OFFSET             0
#define SPR_SYS_SR_SM_MASK               0x00000001
#define SPR_SYS_SR_SM_GET(x)             (((x) >> 0) & 0x1)
#define SPR_SYS_SR_SM_SET(x, y)          (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Tick Timer Exception Enabled */
#define SPR_SYS_SR_TEE_OFFSET            1
#define SPR_SYS_SR_TEE_MASK              0x00000002
#define SPR_SYS_SR_TEE_GET(x)            (((x) >> 1) & 0x1)
#define SPR_SYS_SR_TEE_SET(x, y)         (((x) & U(0xfffffffd)) | \
    ((!!(y)) << 1))

/* Interrupt Exception Enabled */
#define SPR_SYS_SR_IEE_OFFSET            2
#define SPR_SYS_SR_IEE_MASK              0x00000004
#define SPR_SYS_SR_IEE_GET(x)            (((x) >> 2) & 0x1)
#define SPR_SYS_SR_IEE_SET(x, y)         (((x) & U(0xfffffffb)) | \
    ((!!(y)) << 2))

/* Data Cache Enabled */
#define SPR_SYS_SR_DCE_OFFSET            3
#define SPR_SYS_SR_DCE_MASK              0x00000008
#define SPR_SYS_SR_DCE_GET(x)            (((x) >> 3) & 0x1)
#define SPR_SYS_SR_DCE_SET(x, y)         (((x) & U(0xfffffff7)) | \
    ((!!(y)) << 3))

/* Instruction Cache Enabled */
#define SPR_SYS_SR_ICE_OFFSET            4
#define SPR_SYS_SR_ICE_MASK              0x00000010
#define SPR_SYS_SR_ICE_GET(x)            (((x) >> 4) & 0x1)
#define SPR_SYS_SR_ICE_SET(x, y)         (((x) & U(0xffffffef)) | \
    ((!!(y)) << 4))

/* Data MMU Enabled */
#define SPR_SYS_SR_DME_OFFSET            5
#define SPR_SYS_SR_DME_MASK              0x00000020
#define SPR_SYS_SR_DME_GET(x)            (((x) >> 5) & 0x1)
#define SPR_SYS_SR_DME_SET(x, y)         (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* Instruction MMU Enabled */
#define SPR_SYS_SR_IME_OFFSET            6
#define SPR_SYS_SR_IME_MASK              0x00000040
#define SPR_SYS_SR_IME_GET(x)            (((x) >> 6) & 0x1)
#define SPR_SYS_SR_IME_SET(x, y)         (((x) & U(0xffffffbf)) | \
    ((!!(y)) << 6))

/* Little Endian Enabled */
#define SPR_SYS_SR_LEE_OFFSET            7
#define SPR_SYS_SR_LEE_MASK              0x00000080
#define SPR_SYS_SR_LEE_GET(x)            (((x) >> 7) & 0x1)
#define SPR_SYS_SR_LEE_SET(x, y)         (((x) & U(0xffffff7f)) | \
    ((!!(y)) << 7))

/* CID Enable */
#define SPR_SYS_SR_CE_OFFSET             8
#define SPR_SYS_SR_CE_MASK               0x00000100
#define SPR_SYS_SR_CE_GET(x)             (((x) >> 8) & 0x1)
#define SPR_SYS_SR_CE_SET(x, y)          (((x) & U(0xfffffeff)) | \
    ((!!(y)) << 8))

/* Flag */
#define SPR_SYS_SR_F_OFFSET              9
#define SPR_SYS_SR_F_MASK                0x00000200
#define SPR_SYS_SR_F_GET(x)              (((x) >> 9) & 0x1)
#define SPR_SYS_SR_F_SET(x, y)           (((x) & U(0xfffffdff)) | \
    ((!!(y)) << 9))

/* Carry */
#define SPR_SYS_SR_CY_OFFSET             10
#define SPR_SYS_SR_CY_MASK               0x00000400
#define SPR_SYS_SR_CY_GET(x)             (((x) >> 10) & 0x1)
#define SPR_SYS_SR_CY_SET(x, y)          (((x) & U(0xfffffbff)) | \
    ((!!(y)) << 10))

/* Overflow */
#define SPR_SYS_SR_OV_OFFSET             11
#define SPR_SYS_SR_OV_MASK               0x00000800
#define SPR_SYS_SR_OV_GET(x)             (((x) >> 11) & 0x1)
#define SPR_SYS_SR_OV_SET(x, y)          (((x) & U(0xfffff7ff)) | \
    ((!!(y)) << 11))

/* Overflow Exception Enabled */
#define SPR_SYS_SR_OVE_OFFSET            12
#define SPR_SYS_SR_OVE_MASK              0x00001000
#define SPR_SYS_SR_OVE_GET(x)            (((x) >> 12) & 0x1)
#define SPR_SYS_SR_OVE_SET(x, y)         (((x) & U(0xffffefff)) | \
    ((!!(y)) << 12))

/* Delay-slot Exception */
#define SPR_SYS_SR_DSX_OFFSET            13
#define SPR_SYS_SR_DSX_MASK              0x00002000
#define SPR_SYS_SR_DSX_GET(x)            (((x) >> 13) & 0x1)
#define SPR_SYS_SR_DSX_SET(x, y)         (((x) & U(0xffffdfff)) | \
    ((!!(y)) << 13))

/* Exception Prefix High */
#define SPR_SYS_SR_EPH_OFFSET            14
#define SPR_SYS_SR_EPH_MASK              0x00004000
#define SPR_SYS_SR_EPH_GET(x)            (((x) >> 14) & 0x1)
#define SPR_SYS_SR_EPH_SET(x, y)         (((x) & U(0xffffbfff)) | \
    ((!!(y)) << 14))

/* Fixed One */
#define SPR_SYS_SR_FO_OFFSET             15
#define SPR_SYS_SR_FO_MASK               0x00008000
#define SPR_SYS_SR_FO_GET(x)             (((x) >> 15) & 0x1)
#define SPR_SYS_SR_FO_SET(x, y)          (((x) & U(0xffff7fff)) | \
    ((!!(y)) << 15))

/* SPR User Mode Read Access */
#define SPR_SYS_SR_SUMRA_OFFSET          16
#define SPR_SYS_SR_SUMRA_MASK            0x00010000
#define SPR_SYS_SR_SUMRA_GET(x)          (((x) >> 16) & 0x1)
#define SPR_SYS_SR_SUMRA_SET(x, y)       (((x) & U(0xfffeffff)) | \
    ((!!(y)) << 16))

/* Context ID */
#define SPR_SYS_SR_CID_LSB               28
#define SPR_SYS_SR_CID_MSB               31
#define SPR_SYS_SR_CID_BITS              4
#define SPR_SYS_SR_CID_MASK              U(0xf0000000)
#define SPR_SYS_SR_CID_GET(x)            (((x) >> 28) & U(0x0000000f))
#define SPR_SYS_SR_CID_SET(x, y)         (((x) & U(0x0fffffff)) | \
    ((y) << 28))

/* Previous Program Counter */
#define SPR_SYS_PPC_INDEX                U(0x012)
#define SPR_SYS_PPC_ADDR                 U(0x0012)

/* Floating Point Control Status Register */
#define SPR_SYS_FPCSR_INDEX              U(0x014)
#define SPR_SYS_FPCSR_ADDR               U(0x0014)

/* Floating Point Exception Enabled */
#define SPR_SYS_FPCSR_FPEE_OFFSET        0
#define SPR_SYS_FPCSR_FPEE_MASK          0x00000001
#define SPR_SYS_FPCSR_FPEE_GET(x)        (((x) >> 0) & 0x1)
#define SPR_SYS_FPCSR_FPEE_SET(x, y)     (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Rounding Mode */
#define SPR_SYS_FPCSR_RM_LSB             1
#define SPR_SYS_FPCSR_RM_MSB             2
#define SPR_SYS_FPCSR_RM_BITS            2
#define SPR_SYS_FPCSR_RM_MASK            U(0x00000006)
#define SPR_SYS_FPCSR_RM_GET(x)          (((x) >> 1) & U(0x00000003))
#define SPR_SYS_FPCSR_RM_SET(x, y)       (((x) & U(0xfffffff9)) | \
    ((y) << 1))

/* Round to nearest */
#define SPR_SYS_FPCSR_RM_NEAREST         0
/* Round to zero */
#define SPR_SYS_FPCSR_RM_ZERO            1
/* Round to infinity+ */
#define SPR_SYS_FPCSR_RM_INFPLUS         2
/* Round to infinity- */
#define SPR_SYS_FPCSR_RM_INFMINUS        3

/* Overflow Flag */
#define SPR_SYS_FPCSR_OVF_OFFSET         3
#define SPR_SYS_FPCSR_OVF_MASK           0x00000008
#define SPR_SYS_FPCSR_OVF_GET(x)         (((x) >> 3) & 0x1)
#define SPR_SYS_FPCSR_OVF_SET(x, y)      (((x) & U(0xfffffff7)) | \
    ((!!(y)) << 3))

/* Underflow Flag */
#define SPR_SYS_FPCSR_UNF_OFFSET         4
#define SPR_SYS_FPCSR_UNF_MASK           0x00000010
#define SPR_SYS_FPCSR_UNF_GET(x)         (((x) >> 4) & 0x1)
#define SPR_SYS_FPCSR_UNF_SET(x, y)      (((x) & U(0xffffffef)) | \
    ((!!(y)) << 4))

/* SNAN Flag */
#define SPR_SYS_FPCSR_SNF_OFFSET         5
#define SPR_SYS_FPCSR_SNF_MASK           0x00000020
#define SPR_SYS_FPCSR_SNF_GET(x)         (((x) >> 5) & 0x1)
#define SPR_SYS_FPCSR_SNF_SET(x, y)      (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* QNAN Flag */
#define SPR_SYS_FPCSR_QNF_OFFSET         6
#define SPR_SYS_FPCSR_QNF_MASK           0x00000040
#define SPR_SYS_FPCSR_QNF_GET(x)         (((x) >> 6) & 0x1)
#define SPR_SYS_FPCSR_QNF_SET(x, y)      (((x) & U(0xffffffbf)) | \
    ((!!(y)) << 6))

/* Zero Flag */
#define SPR_SYS_FPCSR_ZF_OFFSET          7
#define SPR_SYS_FPCSR_ZF_MASK            0x00000080
#define SPR_SYS_FPCSR_ZF_GET(x)          (((x) >> 7) & 0x1)
#define SPR_SYS_FPCSR_ZF_SET(x, y)       (((x) & U(0xffffff7f)) | \
    ((!!(y)) << 7))

/* Inexact Flag */
#define SPR_SYS_FPCSR_IXF_OFFSET         8
#define SPR_SYS_FPCSR_IXF_MASK           0x00000100
#define SPR_SYS_FPCSR_IXF_GET(x)         (((x) >> 8) & 0x1)
#define SPR_SYS_FPCSR_IXF_SET(x, y)      (((x) & U(0xfffffeff)) | \
    ((!!(y)) << 8))

/* Invalid Flag */
#define SPR_SYS_FPCSR_IVF_OFFSET         9
#define SPR_SYS_FPCSR_IVF_MASK           0x00000200
#define SPR_SYS_FPCSR_IVF_GET(x)         (((x) >> 9) & 0x1)
#define SPR_SYS_FPCSR_IVF_SET(x, y)      (((x) & U(0xfffffdff)) | \
    ((!!(y)) << 9))

/* Infinity Flag */
#define SPR_SYS_FPCSR_INF_OFFSET         10
#define SPR_SYS_FPCSR_INF_MASK           0x00000400
#define SPR_SYS_FPCSR_INF_GET(x)         (((x) >> 10) & 0x1)
#define SPR_SYS_FPCSR_INF_SET(x, y)      (((x) & U(0xfffffbff)) | \
    ((!!(y)) << 10))

/* Divide by Zero Flag */
#define SPR_SYS_FPCSR_DZF_OFFSET         11
#define SPR_SYS_FPCSR_DZF_MASK           0x00000800
#define SPR_SYS_FPCSR_DZF_GET(x)         (((x) >> 11) & 0x1)
#define SPR_SYS_FPCSR_DZF_SET(x, y)      (((x) & U(0xfffff7ff)) | \
    ((!!(y)) << 11))

/* Implementation-specific Registers */
#define SPR_SYS_ISR_BASE                 U(0x015)
#define SPR_SYS_ISR_COUNT                U(0x008)
#define SPR_SYS_ISR_STEP                 U(0x001)
#define SPR_SYS_ISR_INDEX(N)             (SPR_SYS_ISR_BASE + \
    (SPR_SYS_ISR_STEP * (N)))
#define SPR_SYS_ISR_ADDR(N)              ((SPR_SYS_GROUP << SPR_GROUP_LSB) | \
    SPR_SYS_ISR_INDEX(N))

/* Exception PC Registers */
#define SPR_SYS_EPCR_BASE                U(0x020)
#define SPR_SYS_EPCR_COUNT               U(0x010)
#define SPR_SYS_EPCR_STEP                U(0x001)
#define SPR_SYS_EPCR_INDEX(N)            (SPR_SYS_EPCR_BASE + \
    (SPR_SYS_EPCR_STEP * (N)))
#define SPR_SYS_EPCR_ADDR(N)             ((SPR_SYS_GROUP << SPR_GROUP_LSB) | \
    SPR_SYS_EPCR_INDEX(N))

/* Exception Effective Address Registers */
#define SPR_SYS_EEAR_BASE                U(0x030)
#define SPR_SYS_EEAR_COUNT               U(0x010)
#define SPR_SYS_EEAR_STEP                U(0x001)
#define SPR_SYS_EEAR_INDEX(N)            (SPR_SYS_EEAR_BASE + \
    (SPR_SYS_EEAR_STEP * (N)))
#define SPR_SYS_EEAR_ADDR(N)             ((SPR_SYS_GROUP << SPR_GROUP_LSB) | \
    SPR_SYS_EEAR_INDEX(N))

/* Exception Supervision Registers */
#define SPR_SYS_ESR_BASE                 U(0x040)
#define SPR_SYS_ESR_COUNT                U(0x010)
#define SPR_SYS_ESR_STEP                 U(0x001)
#define SPR_SYS_ESR_INDEX(N)             (SPR_SYS_ESR_BASE + \
    (SPR_SYS_ESR_STEP * (N)))
#define SPR_SYS_ESR_ADDR(N)              ((SPR_SYS_GROUP << SPR_GROUP_LSB) | \
    SPR_SYS_ESR_INDEX(N))

/* Supervisor Mode */
#define SPR_SYS_ESR_SM_OFFSET            0
#define SPR_SYS_ESR_SM_MASK              0x00000001
#define SPR_SYS_ESR_SM_GET(x)            (((x) >> 0) & 0x1)
#define SPR_SYS_ESR_SM_SET(x, y)         (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Tick Timer Exception Enabled */
#define SPR_SYS_ESR_TEE_OFFSET           1
#define SPR_SYS_ESR_TEE_MASK             0x00000002
#define SPR_SYS_ESR_TEE_GET(x)           (((x) >> 1) & 0x1)
#define SPR_SYS_ESR_TEE_SET(x, y)        (((x) & U(0xfffffffd)) | \
    ((!!(y)) << 1))

/* Interrupt Exception Enabled */
#define SPR_SYS_ESR_IEE_OFFSET           2
#define SPR_SYS_ESR_IEE_MASK             0x00000004
#define SPR_SYS_ESR_IEE_GET(x)           (((x) >> 2) & 0x1)
#define SPR_SYS_ESR_IEE_SET(x, y)        (((x) & U(0xfffffffb)) | \
    ((!!(y)) << 2))

/* Data Cache Enabled */
#define SPR_SYS_ESR_DCE_OFFSET           3
#define SPR_SYS_ESR_DCE_MASK             0x00000008
#define SPR_SYS_ESR_DCE_GET(x)           (((x) >> 3) & 0x1)
#define SPR_SYS_ESR_DCE_SET(x, y)        (((x) & U(0xfffffff7)) | \
    ((!!(y)) << 3))

/* Instruction Cache Enabled */
#define SPR_SYS_ESR_ICE_OFFSET           4
#define SPR_SYS_ESR_ICE_MASK             0x00000010
#define SPR_SYS_ESR_ICE_GET(x)           (((x) >> 4) & 0x1)
#define SPR_SYS_ESR_ICE_SET(x, y)        (((x) & U(0xffffffef)) | \
    ((!!(y)) << 4))

/* Data MMU Enabled */
#define SPR_SYS_ESR_DME_OFFSET           5
#define SPR_SYS_ESR_DME_MASK             0x00000020
#define SPR_SYS_ESR_DME_GET(x)           (((x) >> 5) & 0x1)
#define SPR_SYS_ESR_DME_SET(x, y)        (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* Instruction MMU Enabled */
#define SPR_SYS_ESR_IME_OFFSET           6
#define SPR_SYS_ESR_IME_MASK             0x00000040
#define SPR_SYS_ESR_IME_GET(x)           (((x) >> 6) & 0x1)
#define SPR_SYS_ESR_IME_SET(x, y)        (((x) & U(0xffffffbf)) | \
    ((!!(y)) << 6))

/* Little Endian Enabled */
#define SPR_SYS_ESR_LEE_OFFSET           7
#define SPR_SYS_ESR_LEE_MASK             0x00000080
#define SPR_SYS_ESR_LEE_GET(x)           (((x) >> 7) & 0x1)
#define SPR_SYS_ESR_LEE_SET(x, y)        (((x) & U(0xffffff7f)) | \
    ((!!(y)) << 7))

/* CID Enable */
#define SPR_SYS_ESR_CE_OFFSET            8
#define SPR_SYS_ESR_CE_MASK              0x00000100
#define SPR_SYS_ESR_CE_GET(x)            (((x) >> 8) & 0x1)
#define SPR_SYS_ESR_CE_SET(x, y)         (((x) & U(0xfffffeff)) | \
    ((!!(y)) << 8))

/* Flag */
#define SPR_SYS_ESR_F_OFFSET             9
#define SPR_SYS_ESR_F_MASK               0x00000200
#define SPR_SYS_ESR_F_GET(x)             (((x) >> 9) & 0x1)
#define SPR_SYS_ESR_F_SET(x, y)          (((x) & U(0xfffffdff)) | \
    ((!!(y)) << 9))

/* Carry */
#define SPR_SYS_ESR_CY_OFFSET            10
#define SPR_SYS_ESR_CY_MASK              0x00000400
#define SPR_SYS_ESR_CY_GET(x)            (((x) >> 10) & 0x1)
#define SPR_SYS_ESR_CY_SET(x, y)         (((x) & U(0xfffffbff)) | \
    ((!!(y)) << 10))

/* Overflow */
#define SPR_SYS_ESR_OV_OFFSET            11
#define SPR_SYS_ESR_OV_MASK              0x00000800
#define SPR_SYS_ESR_OV_GET(x)            (((x) >> 11) & 0x1)
#define SPR_SYS_ESR_OV_SET(x, y)         (((x) & U(0xfffff7ff)) | \
    ((!!(y)) << 11))

/* Overflow Exception Enabled */
#define SPR_SYS_ESR_OVE_OFFSET           12
#define SPR_SYS_ESR_OVE_MASK             0x00001000
#define SPR_SYS_ESR_OVE_GET(x)           (((x) >> 12) & 0x1)
#define SPR_SYS_ESR_OVE_SET(x, y)        (((x) & U(0xffffefff)) | \
    ((!!(y)) << 12))

/* Delay-slot Exception */
#define SPR_SYS_ESR_DSX_OFFSET           13
#define SPR_SYS_ESR_DSX_MASK             0x00002000
#define SPR_SYS_ESR_DSX_GET(x)           (((x) >> 13) & 0x1)
#define SPR_SYS_ESR_DSX_SET(x, y)        (((x) & U(0xffffdfff)) | \
    ((!!(y)) << 13))

/* Exception Prefix High */
#define SPR_SYS_ESR_EPH_OFFSET           14
#define SPR_SYS_ESR_EPH_MASK             0x00004000
#define SPR_SYS_ESR_EPH_GET(x)           (((x) >> 14) & 0x1)
#define SPR_SYS_ESR_EPH_SET(x, y)        (((x) & U(0xffffbfff)) | \
    ((!!(y)) << 14))

/* Fixed One */
#define SPR_SYS_ESR_FO_OFFSET            15
#define SPR_SYS_ESR_FO_MASK              0x00008000
#define SPR_SYS_ESR_FO_GET(x)            (((x) >> 15) & 0x1)
#define SPR_SYS_ESR_FO_SET(x, y)         (((x) & U(0xffff7fff)) | \
    ((!!(y)) << 15))

/* SPR User Mode Read Access */
#define SPR_SYS_ESR_SUMRA_OFFSET         16
#define SPR_SYS_ESR_SUMRA_MASK           0x00010000
#define SPR_SYS_ESR_SUMRA_GET(x)         (((x) >> 16) & 0x1)
#define SPR_SYS_ESR_SUMRA_SET(x, y)      (((x) & U(0xfffeffff)) | \
    ((!!(y)) << 16))

/* Context ID */
#define SPR_SYS_ESR_CID_LSB              28
#define SPR_SYS_ESR_CID_MSB              31
#define SPR_SYS_ESR_CID_BITS             4
#define SPR_SYS_ESR_CID_MASK             U(0xf0000000)
#define SPR_SYS_ESR_CID_GET(x)           (((x) >> 28) & U(0x0000000f))
#define SPR_SYS_ESR_CID_SET(x, y)        (((x) & U(0x0fffffff)) | \
    ((y) << 28))

/* Core identifier (multicore) */
#define SPR_SYS_COREID_INDEX             U(0x080)
#define SPR_SYS_COREID_ADDR              U(0x0080)

/* Number of cores (multicore) */
#define SPR_SYS_NUMCORES_INDEX           U(0x081)
#define SPR_SYS_NUMCORES_ADDR            U(0x0081)

/* General Purpose Registers */
#define SPR_SYS_GPR_BASE                 U(0x400)
#define SPR_SYS_GPR_COUNT                U(0x100)
#define SPR_SYS_GPR_STEP                 U(0x001)
#define SPR_SYS_GPR_INDEX(N)             (SPR_SYS_GPR_BASE + \
    (SPR_SYS_GPR_STEP * (N)))
#define SPR_SYS_GPR_ADDR(N)              ((SPR_SYS_GROUP << SPR_GROUP_LSB) | \
    SPR_SYS_GPR_INDEX(N))

/******************/
/* Data MMU Group */
/******************/
#define SPR_DMMU_GROUP                  0x01

/* Instruction MMU Control Register */
#define SPR_DMMU_DMMUCR_INDEX           U(0x000)
#define SPR_DMMU_DMMUCR_ADDR            U(0x0800)

/* DTLB Flush */
#define SPR_DMMU_DMMUCR_DTF_OFFSET      0
#define SPR_DMMU_DMMUCR_DTF_MASK        0x00000001
#define SPR_DMMU_DMMUCR_DTF_GET(x)      (((x) >> 0) & 0x1)
#define SPR_DMMU_DMMUCR_DTF_SET(x, y)   (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Page Table Base Pointer */
#define SPR_DMMU_DMMUCR_PTBP_LSB        10
#define SPR_DMMU_DMMUCR_PTBP_MSB        31
#define SPR_DMMU_DMMUCR_PTBP_BITS       22
#define SPR_DMMU_DMMUCR_PTBP_MASK       U(0xfffffc00)
#define SPR_DMMU_DMMUCR_PTBP_GET(x)     (((x) >> 10) & U(0x003fffff))
#define SPR_DMMU_DMMUCR_PTBP_SET(x, y)  (((x) & U(0x000003ff)) | \
    ((y) << 10))

/* Data MMU Protection Register */
#define SPR_DMMU_DMMUPR_INDEX           U(0x001)
#define SPR_DMMU_DMMUPR_ADDR            U(0x0801)

/* Supervisor Read Enable 1 */
#define SPR_DMMU_DMMUPR_SRE1_OFFSET     0
#define SPR_DMMU_DMMUPR_SRE1_MASK       0x00000001
#define SPR_DMMU_DMMUPR_SRE1_GET(x)     (((x) >> 0) & 0x1)
#define SPR_DMMU_DMMUPR_SRE1_SET(x, y)  (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Supervisor Write Enable 1 */
#define SPR_DMMU_DMMUPR_SWE1_OFFSET     1
#define SPR_DMMU_DMMUPR_SWE1_MASK       0x00000002
#define SPR_DMMU_DMMUPR_SWE1_GET(x)     (((x) >> 1) & 0x1)
#define SPR_DMMU_DMMUPR_SWE1_SET(x, y)  (((x) & U(0xfffffffd)) | \
    ((!!(y)) << 1))

/* User Read Enable 1 */
#define SPR_DMMU_DMMUPR_URE1_OFFSET     2
#define SPR_DMMU_DMMUPR_URE1_MASK       0x00000004
#define SPR_DMMU_DMMUPR_URE1_GET(x)     (((x) >> 2) & 0x1)
#define SPR_DMMU_DMMUPR_URE1_SET(x, y)  (((x) & U(0xfffffffb)) | \
    ((!!(y)) << 2))

/* User Write Enable 1 */
#define SPR_DMMU_DMMUPR_UWE1_OFFSET     3
#define SPR_DMMU_DMMUPR_UWE1_MASK       0x00000008
#define SPR_DMMU_DMMUPR_UWE1_GET(x)     (((x) >> 3) & 0x1)
#define SPR_DMMU_DMMUPR_UWE1_SET(x, y)  (((x) & U(0xfffffff7)) | \
    ((!!(y)) << 3))

/* Supervisor Read Enable 2 */
#define SPR_DMMU_DMMUPR_SRE2_OFFSET     4
#define SPR_DMMU_DMMUPR_SRE2_MASK       0x00000010
#define SPR_DMMU_DMMUPR_SRE2_GET(x)     (((x) >> 4) & 0x1)
#define SPR_DMMU_DMMUPR_SRE2_SET(x, y)  (((x) & U(0xffffffef)) | \
    ((!!(y)) << 4))

/* Supervisor Write Enable 2 */
#define SPR_DMMU_DMMUPR_SWE2_OFFSET     5
#define SPR_DMMU_DMMUPR_SWE2_MASK       0x00000020
#define SPR_DMMU_DMMUPR_SWE2_GET(x)     (((x) >> 5) & 0x1)
#define SPR_DMMU_DMMUPR_SWE2_SET(x, y)  (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* User Read Enable 2 */
#define SPR_DMMU_DMMUPR_URE2_OFFSET     6
#define SPR_DMMU_DMMUPR_URE2_MASK       0x00000040
#define SPR_DMMU_DMMUPR_URE2_GET(x)     (((x) >> 6) & 0x1)
#define SPR_DMMU_DMMUPR_URE2_SET(x, y)  (((x) & U(0xffffffbf)) | \
    ((!!(y)) << 6))

/* User Write Enable 2 */
#define SPR_DMMU_DMMUPR_UWE2_OFFSET     7
#define SPR_DMMU_DMMUPR_UWE2_MASK       0x00000080
#define SPR_DMMU_DMMUPR_UWE2_GET(x)     (((x) >> 7) & 0x1)
#define SPR_DMMU_DMMUPR_UWE2_SET(x, y)  (((x) & U(0xffffff7f)) | \
    ((!!(y)) << 7))

/* Supervisor Read Enable 3 */
#define SPR_DMMU_DMMUPR_SRE3_OFFSET     8
#define SPR_DMMU_DMMUPR_SRE3_MASK       0x00000100
#define SPR_DMMU_DMMUPR_SRE3_GET(x)     (((x) >> 8) & 0x1)
#define SPR_DMMU_DMMUPR_SRE3_SET(x, y)  (((x) & U(0xfffffeff)) | \
    ((!!(y)) << 8))

/* Supervisor Write Enable 3 */
#define SPR_DMMU_DMMUPR_SWE3_OFFSET     9
#define SPR_DMMU_DMMUPR_SWE3_MASK       0x00000200
#define SPR_DMMU_DMMUPR_SWE3_GET(x)     (((x) >> 9) & 0x1)
#define SPR_DMMU_DMMUPR_SWE3_SET(x, y)  (((x) & U(0xfffffdff)) | \
    ((!!(y)) << 9))

/* User Read Enable 3 */
#define SPR_DMMU_DMMUPR_URE3_OFFSET     10
#define SPR_DMMU_DMMUPR_URE3_MASK       0x00000400
#define SPR_DMMU_DMMUPR_URE3_GET(x)     (((x) >> 10) & 0x1)
#define SPR_DMMU_DMMUPR_URE3_SET(x, y)  (((x) & U(0xfffffbff)) | \
    ((!!(y)) << 10))

/* User Write Enable 3 */
#define SPR_DMMU_DMMUPR_UWE3_OFFSET     11
#define SPR_DMMU_DMMUPR_UWE3_MASK       0x00000800
#define SPR_DMMU_DMMUPR_UWE3_GET(x)     (((x) >> 11) & 0x1)
#define SPR_DMMU_DMMUPR_UWE3_SET(x, y)  (((x) & U(0xfffff7ff)) | \
    ((!!(y)) << 11))

/* Supervisor Read Enable 4 */
#define SPR_DMMU_DMMUPR_SRE4_OFFSET     12
#define SPR_DMMU_DMMUPR_SRE4_MASK       0x00001000
#define SPR_DMMU_DMMUPR_SRE4_GET(x)     (((x) >> 12) & 0x1)
#define SPR_DMMU_DMMUPR_SRE4_SET(x, y)  (((x) & U(0xffffefff)) | \
    ((!!(y)) << 12))

/* Supervisor Write Enable 4 */
#define SPR_DMMU_DMMUPR_SWE4_OFFSET     13
#define SPR_DMMU_DMMUPR_SWE4_MASK       0x00002000
#define SPR_DMMU_DMMUPR_SWE4_GET(x)     (((x) >> 13) & 0x1)
#define SPR_DMMU_DMMUPR_SWE4_SET(x, y)  (((x) & U(0xffffdfff)) | \
    ((!!(y)) << 13))

/* User Read Enable 4 */
#define SPR_DMMU_DMMUPR_URE4_OFFSET     14
#define SPR_DMMU_DMMUPR_URE4_MASK       0x00004000
#define SPR_DMMU_DMMUPR_URE4_GET(x)     (((x) >> 14) & 0x1)
#define SPR_DMMU_DMMUPR_URE4_SET(x, y)  (((x) & U(0xffffbfff)) | \
    ((!!(y)) << 14))

/* User Write Enable 4 */
#define SPR_DMMU_DMMUPR_UWE4_OFFSET     15
#define SPR_DMMU_DMMUPR_UWE4_MASK       0x00008000
#define SPR_DMMU_DMMUPR_UWE4_GET(x)     (((x) >> 15) & 0x1)
#define SPR_DMMU_DMMUPR_UWE4_SET(x, y)  (((x) & U(0xffff7fff)) | \
    ((!!(y)) << 15))

/* Supervisor Read Enable 5 */
#define SPR_DMMU_DMMUPR_SRE5_OFFSET     16
#define SPR_DMMU_DMMUPR_SRE5_MASK       0x00010000
#define SPR_DMMU_DMMUPR_SRE5_GET(x)     (((x) >> 16) & 0x1)
#define SPR_DMMU_DMMUPR_SRE5_SET(x, y)  (((x) & U(0xfffeffff)) | \
    ((!!(y)) << 16))

/* Supervisor Write Enable 5 */
#define SPR_DMMU_DMMUPR_SWE5_OFFSET     17
#define SPR_DMMU_DMMUPR_SWE5_MASK       0x00020000
#define SPR_DMMU_DMMUPR_SWE5_GET(x)     (((x) >> 17) & 0x1)
#define SPR_DMMU_DMMUPR_SWE5_SET(x, y)  (((x) & U(0xfffdffff)) | \
    ((!!(y)) << 17))

/* User Read Enable 5 */
#define SPR_DMMU_DMMUPR_URE5_OFFSET     18
#define SPR_DMMU_DMMUPR_URE5_MASK       0x00040000
#define SPR_DMMU_DMMUPR_URE5_GET(x)     (((x) >> 18) & 0x1)
#define SPR_DMMU_DMMUPR_URE5_SET(x, y)  (((x) & U(0xfffbffff)) | \
    ((!!(y)) << 18))

/* User Write Enable 5 */
#define SPR_DMMU_DMMUPR_UWE5_OFFSET     19
#define SPR_DMMU_DMMUPR_UWE5_MASK       0x00080000
#define SPR_DMMU_DMMUPR_UWE5_GET(x)     (((x) >> 19) & 0x1)
#define SPR_DMMU_DMMUPR_UWE5_SET(x, y)  (((x) & U(0xfff7ffff)) | \
    ((!!(y)) << 19))

/* Supervisor Read Enable 6 */
#define SPR_DMMU_DMMUPR_SRE6_OFFSET     20
#define SPR_DMMU_DMMUPR_SRE6_MASK       0x00100000
#define SPR_DMMU_DMMUPR_SRE6_GET(x)     (((x) >> 20) & 0x1)
#define SPR_DMMU_DMMUPR_SRE6_SET(x, y)  (((x) & U(0xffefffff)) | \
    ((!!(y)) << 20))

/* Supervisor Write Enable 6 */
#define SPR_DMMU_DMMUPR_SWE6_OFFSET     21
#define SPR_DMMU_DMMUPR_SWE6_MASK       0x00200000
#define SPR_DMMU_DMMUPR_SWE6_GET(x)     (((x) >> 21) & 0x1)
#define SPR_DMMU_DMMUPR_SWE6_SET(x, y)  (((x) & U(0xffdfffff)) | \
    ((!!(y)) << 21))

/* User Read Enable 6 */
#define SPR_DMMU_DMMUPR_URE6_OFFSET     22
#define SPR_DMMU_DMMUPR_URE6_MASK       0x00400000
#define SPR_DMMU_DMMUPR_URE6_GET(x)     (((x) >> 22) & 0x1)
#define SPR_DMMU_DMMUPR_URE6_SET(x, y)  (((x) & U(0xffbfffff)) | \
    ((!!(y)) << 22))

/* User Write Enable 6 */
#define SPR_DMMU_DMMUPR_UWE6_OFFSET     23
#define SPR_DMMU_DMMUPR_UWE6_MASK       0x00800000
#define SPR_DMMU_DMMUPR_UWE6_GET(x)     (((x) >> 23) & 0x1)
#define SPR_DMMU_DMMUPR_UWE6_SET(x, y)  (((x) & U(0xff7fffff)) | \
    ((!!(y)) << 23))

/* Supervisor Read Enable 7 */
#define SPR_DMMU_DMMUPR_SRE7_OFFSET     24
#define SPR_DMMU_DMMUPR_SRE7_MASK       0x01000000
#define SPR_DMMU_DMMUPR_SRE7_GET(x)     (((x) >> 24) & 0x1)
#define SPR_DMMU_DMMUPR_SRE7_SET(x, y)  (((x) & U(0xfeffffff)) | \
    ((!!(y)) << 24))

/* Supervisor Write Enable 7 */
#define SPR_DMMU_DMMUPR_SWE7_OFFSET     25
#define SPR_DMMU_DMMUPR_SWE7_MASK       0x02000000
#define SPR_DMMU_DMMUPR_SWE7_GET(x)     (((x) >> 25) & 0x1)
#define SPR_DMMU_DMMUPR_SWE7_SET(x, y)  (((x) & U(0xfdffffff)) | \
    ((!!(y)) << 25))

/* User Read Enable 7 */
#define SPR_DMMU_DMMUPR_URE7_OFFSET     26
#define SPR_DMMU_DMMUPR_URE7_MASK       0x04000000
#define SPR_DMMU_DMMUPR_URE7_GET(x)     (((x) >> 26) & 0x1)
#define SPR_DMMU_DMMUPR_URE7_SET(x, y)  (((x) & U(0xfbffffff)) | \
    ((!!(y)) << 26))

/* User Write Enable 7 */
#define SPR_DMMU_DMMUPR_UWE7_OFFSET     27
#define SPR_DMMU_DMMUPR_UWE7_MASK       0x08000000
#define SPR_DMMU_DMMUPR_UWE7_GET(x)     (((x) >> 27) & 0x1)
#define SPR_DMMU_DMMUPR_UWE7_SET(x, y)  (((x) & U(0xf7ffffff)) | \
    ((!!(y)) << 27))

/* Data TLB Entry Invalidate Register */
#define SPR_DMMU_DTLBEIR_INDEX          U(0x002)
#define SPR_DMMU_DTLBEIR_ADDR           U(0x0802)

/* Data ATB Match Registers */
#define SPR_DMMU_DATBMR_BASE            U(0x004)
#define SPR_DMMU_DATBMR_COUNT           U(0x004)
#define SPR_DMMU_DATBMR_STEP            U(0x001)
#define SPR_DMMU_DATBMR_INDEX(N)        (SPR_DMMU_DATBMR_BASE + \
    (SPR_DMMU_DATBMR_STEP * (N)))
#define SPR_DMMU_DATBMR_ADDR(N)         ((SPR_DMMU_GROUP << SPR_GROUP_LSB) | \
    SPR_DMMU_DATBMR_INDEX(N))

/* Valid */
#define SPR_DMMU_DATBMR_V_OFFSET        0
#define SPR_DMMU_DATBMR_V_MASK          0x00000001
#define SPR_DMMU_DATBMR_V_GET(x)        (((x) >> 0) & 0x1)
#define SPR_DMMU_DATBMR_V_SET(x, y)     (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Context ID */
#define SPR_DMMU_DATBMR_CID_LSB         1
#define SPR_DMMU_DATBMR_CID_MSB         4
#define SPR_DMMU_DATBMR_CID_BITS        4
#define SPR_DMMU_DATBMR_CID_MASK        U(0x0000001e)
#define SPR_DMMU_DATBMR_CID_GET(x)      (((x) >> 1) & U(0x0000000f))
#define SPR_DMMU_DATBMR_CID_SET(x, y)   (((x) & U(0xffffffe1)) | \
    ((y) << 1))

/* Page Size */
#define SPR_DMMU_DATBMR_PS_OFFSET       5
#define SPR_DMMU_DATBMR_PS_MASK         0x00000020
#define SPR_DMMU_DATBMR_PS_GET(x)       (((x) >> 5) & 0x1)
#define SPR_DMMU_DATBMR_PS_SET(x, y)    (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* Virtual Page Number */
#define SPR_DMMU_DATBMR_VPN_LSB         10
#define SPR_DMMU_DATBMR_VPN_MSB         31
#define SPR_DMMU_DATBMR_VPN_BITS        22
#define SPR_DMMU_DATBMR_VPN_MASK        U(0xfffffc00)
#define SPR_DMMU_DATBMR_VPN_GET(x)      (((x) >> 10) & U(0x003fffff))
#define SPR_DMMU_DATBMR_VPN_SET(x, y)   (((x) & U(0x000003ff)) | \
    ((y) << 10))

/* Data ATB Translate Registers */
#define SPR_DMMU_DATBTR_BASE            U(0x008)
#define SPR_DMMU_DATBTR_COUNT           U(0x004)
#define SPR_DMMU_DATBTR_STEP            U(0x001)
#define SPR_DMMU_DATBTR_INDEX(N)        (SPR_DMMU_DATBTR_BASE + \
    (SPR_DMMU_DATBTR_STEP * (N)))
#define SPR_DMMU_DATBTR_ADDR(N)         ((SPR_DMMU_GROUP << SPR_GROUP_LSB) | \
    SPR_DMMU_DATBTR_INDEX(N))

/* Cache Coherency */
#define SPR_DMMU_DATBTR_CC_OFFSET       0
#define SPR_DMMU_DATBTR_CC_MASK         0x00000001
#define SPR_DMMU_DATBTR_CC_GET(x)       (((x) >> 0) & 0x1)
#define SPR_DMMU_DATBTR_CC_SET(x, y)    (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Cache Inhibit */
#define SPR_DMMU_DATBTR_CI_OFFSET       1
#define SPR_DMMU_DATBTR_CI_MASK         0x00000002
#define SPR_DMMU_DATBTR_CI_GET(x)       (((x) >> 1) & 0x1)
#define SPR_DMMU_DATBTR_CI_SET(x, y)    (((x) & U(0xfffffffd)) | \
    ((!!(y)) << 1))

/* Write-back Cache */
#define SPR_DMMU_DATBTR_WBC_OFFSET      2
#define SPR_DMMU_DATBTR_WBC_MASK        0x00000004
#define SPR_DMMU_DATBTR_WBC_GET(x)      (((x) >> 2) & 0x1)
#define SPR_DMMU_DATBTR_WBC_SET(x, y)   (((x) & U(0xfffffffb)) | \
    ((!!(y)) << 2))

/* Weakly-ordered Memory */
#define SPR_DMMU_DATBTR_WOM_OFFSET      3
#define SPR_DMMU_DATBTR_WOM_MASK        0x00000008
#define SPR_DMMU_DATBTR_WOM_GET(x)      (((x) >> 3) & 0x1)
#define SPR_DMMU_DATBTR_WOM_SET(x, y)   (((x) & U(0xfffffff7)) | \
    ((!!(y)) << 3))

/* Accessed */
#define SPR_DMMU_DATBTR_A_OFFSET        4
#define SPR_DMMU_DATBTR_A_MASK          0x00000010
#define SPR_DMMU_DATBTR_A_GET(x)        (((x) >> 4) & 0x1)
#define SPR_DMMU_DATBTR_A_SET(x, y)     (((x) & U(0xffffffef)) | \
    ((!!(y)) << 4))

/* Dirty */
#define SPR_DMMU_DATBTR_D_OFFSET        5
#define SPR_DMMU_DATBTR_D_MASK          0x00000020
#define SPR_DMMU_DATBTR_D_GET(x)        (((x) >> 5) & 0x1)
#define SPR_DMMU_DATBTR_D_SET(x, y)     (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* Supervisor Read Enable */
#define SPR_DMMU_DATBTR_SRE_OFFSET      6
#define SPR_DMMU_DATBTR_SRE_MASK        0x00000040
#define SPR_DMMU_DATBTR_SRE_GET(x)      (((x) >> 6) & 0x1)
#define SPR_DMMU_DATBTR_SRE_SET(x, y)   (((x) & U(0xffffffbf)) | \
    ((!!(y)) << 6))

/* Supervisor Write Enable */
#define SPR_DMMU_DATBTR_SWE_OFFSET      7
#define SPR_DMMU_DATBTR_SWE_MASK        0x00000080
#define SPR_DMMU_DATBTR_SWE_GET(x)      (((x) >> 7) & 0x1)
#define SPR_DMMU_DATBTR_SWE_SET(x, y)   (((x) & U(0xffffff7f)) | \
    ((!!(y)) << 7))

/* User Read Enable */
#define SPR_DMMU_DATBTR_URE_OFFSET      8
#define SPR_DMMU_DATBTR_URE_MASK        0x00000100
#define SPR_DMMU_DATBTR_URE_GET(x)      (((x) >> 8) & 0x1)
#define SPR_DMMU_DATBTR_URE_SET(x, y)   (((x) & U(0xfffffeff)) | \
    ((!!(y)) << 8))

/* User Write Enable */
#define SPR_DMMU_DATBTR_UWE_OFFSET      9
#define SPR_DMMU_DATBTR_UWE_MASK        0x00000200
#define SPR_DMMU_DATBTR_UWE_GET(x)      (((x) >> 9) & 0x1)
#define SPR_DMMU_DATBTR_UWE_SET(x, y)   (((x) & U(0xfffffdff)) | \
    ((!!(y)) << 9))

/* Physical Page Number */
#define SPR_DMMU_DATBTR_PPN_LSB         10
#define SPR_DMMU_DATBTR_PPN_MSB         31
#define SPR_DMMU_DATBTR_PPN_BITS        22
#define SPR_DMMU_DATBTR_PPN_MASK        U(0xfffffc00)
#define SPR_DMMU_DATBTR_PPN_GET(x)      (((x) >> 10) & U(0x003fffff))
#define SPR_DMMU_DATBTR_PPN_SET(x, y)   (((x) & U(0x000003ff)) | \
    ((y) << 10))

/* Data TLB */
#define SPR_DMMU_DTLBW_BASE             U(0x200)
#define SPR_DMMU_DTLBW_COUNT            U(0x004)
#define SPR_DMMU_DTLBW_STEP             U(0x100)
#define SPR_DMMU_DTLBW_SUBBASE(N0)      (SPR_DMMU_DTLBW_BASE + \
    (SPR_DMMU_DTLBW_STEP * (N0)))

/* Data TLB Match Registers */
#define SPR_DMMU_DTLBW_MR_BASE          U(0x000)
#define SPR_DMMU_DTLBW_MR_COUNT         U(0x080)
#define SPR_DMMU_DTLBW_MR_STEP          U(0x001)

#define SPR_DMMU_DTLBW_MR_INDEX(N0, N1) (SPR_DMMU_DTLBW_SUBBASE(N0) + \
    SPR_DMMU_DTLBW_MR_BASE + \
    (SPR_DMMU_DTLBW_MR_STEP * (N1)))
#define SPR_DMMU_DTLBW_MR_ADDR(N0, N1)  ((SPR_DMMU_GROUP << SPR_GROUP_LSB) | \
    SPR_DMMU_DTLBW_MR_INDEX(N0, N1))

/* Valid */
#define SPR_DMMU_DTLBW_MR_V_OFFSET      0
#define SPR_DMMU_DTLBW_MR_V_MASK        0x00000001
#define SPR_DMMU_DTLBW_MR_V_GET(x)      (((x) >> 0) & 0x1)
#define SPR_DMMU_DTLBW_MR_V_SET(x, y)   (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Page Level 1 */
#define SPR_DMMU_DTLBW_MR_PL1_OFFSET    1
#define SPR_DMMU_DTLBW_MR_PL1_MASK      0x00000002
#define SPR_DMMU_DTLBW_MR_PL1_GET(x)    (((x) >> 1) & 0x1)
#define SPR_DMMU_DTLBW_MR_PL1_SET(x, y) (((x) & U(0xfffffffd)) | \
    ((!!(y)) << 1))

/* Context ID */
#define SPR_DMMU_DTLBW_MR_CID_LSB       2
#define SPR_DMMU_DTLBW_MR_CID_MSB       5
#define SPR_DMMU_DTLBW_MR_CID_BITS      4
#define SPR_DMMU_DTLBW_MR_CID_MASK      U(0x0000003c)
#define SPR_DMMU_DTLBW_MR_CID_GET(x)    (((x) >> 2) & U(0x0000000f))
#define SPR_DMMU_DTLBW_MR_CID_SET(x, y) (((x) & U(0xffffffc3)) | \
    ((y) << 2))

/* Least Recently Used */
#define SPR_DMMU_DTLBW_MR_LRU_LSB       6
#define SPR_DMMU_DTLBW_MR_LRU_MSB       7
#define SPR_DMMU_DTLBW_MR_LRU_BITS      2
#define SPR_DMMU_DTLBW_MR_LRU_MASK      U(0x000000c0)
#define SPR_DMMU_DTLBW_MR_LRU_GET(x)    (((x) >> 6) & U(0x00000003))
#define SPR_DMMU_DTLBW_MR_LRU_SET(x, y) (((x) & U(0xffffff3f)) | \
    ((y) << 6))

/* Virtual Page Number */
#define SPR_DMMU_DTLBW_MR_VPN_LSB       13
#define SPR_DMMU_DTLBW_MR_VPN_MSB       31
#define SPR_DMMU_DTLBW_MR_VPN_BITS      19
#define SPR_DMMU_DTLBW_MR_VPN_MASK      U(0xffffe000)
#define SPR_DMMU_DTLBW_MR_VPN_GET(x)    (((x) >> 13) & U(0x0007ffff))
#define SPR_DMMU_DTLBW_MR_VPN_SET(x, y) (((x) & U(0x00001fff)) | \
    ((y) << 13))

/* Data TLB Translate Registers */
#define SPR_DMMU_DTLBW_TR_BASE          U(0x080)
#define SPR_DMMU_DTLBW_TR_COUNT         U(0x080)
#define SPR_DMMU_DTLBW_TR_STEP          U(0x001)

#define SPR_DMMU_DTLBW_TR_INDEX(N0, N1) (SPR_DMMU_DTLBW_SUBBASE(N0) + \
    SPR_DMMU_DTLBW_TR_BASE + \
    (SPR_DMMU_DTLBW_TR_STEP * (N1)))
#define SPR_DMMU_DTLBW_TR_ADDR(N0, N1)  ((SPR_DMMU_GROUP << SPR_GROUP_LSB) | \
    SPR_DMMU_DTLBW_TR_INDEX(N0, N1))

/* Cache Coherency */
#define SPR_DMMU_DTLBW_TR_CC_OFFSET     0
#define SPR_DMMU_DTLBW_TR_CC_MASK       0x00000001
#define SPR_DMMU_DTLBW_TR_CC_GET(x)     (((x) >> 0) & 0x1)
#define SPR_DMMU_DTLBW_TR_CC_SET(x, y)  (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Cache Inhibit */
#define SPR_DMMU_DTLBW_TR_CI_OFFSET     1
#define SPR_DMMU_DTLBW_TR_CI_MASK       0x00000002
#define SPR_DMMU_DTLBW_TR_CI_GET(x)     (((x) >> 1) & 0x1)
#define SPR_DMMU_DTLBW_TR_CI_SET(x, y)  (((x) & U(0xfffffffd)) | \
    ((!!(y)) << 1))

/* Write-back Cache */
#define SPR_DMMU_DTLBW_TR_WBC_OFFSET    2
#define SPR_DMMU_DTLBW_TR_WBC_MASK      0x00000004
#define SPR_DMMU_DTLBW_TR_WBC_GET(x)    (((x) >> 2) & 0x1)
#define SPR_DMMU_DTLBW_TR_WBC_SET(x, y) (((x) & U(0xfffffffb)) | \
    ((!!(y)) << 2))

/* Weakly-ordered Memory */
#define SPR_DMMU_DTLBW_TR_WOM_OFFSET    3
#define SPR_DMMU_DTLBW_TR_WOM_MASK      0x00000008
#define SPR_DMMU_DTLBW_TR_WOM_GET(x)    (((x) >> 3) & 0x1)
#define SPR_DMMU_DTLBW_TR_WOM_SET(x, y) (((x) & U(0xfffffff7)) | \
    ((!!(y)) << 3))

/* Accessed */
#define SPR_DMMU_DTLBW_TR_A_OFFSET      4
#define SPR_DMMU_DTLBW_TR_A_MASK        0x00000010
#define SPR_DMMU_DTLBW_TR_A_GET(x)      (((x) >> 4) & 0x1)
#define SPR_DMMU_DTLBW_TR_A_SET(x, y)   (((x) & U(0xffffffef)) | \
    ((!!(y)) << 4))

/* Dirty */
#define SPR_DMMU_DTLBW_TR_D_OFFSET      5
#define SPR_DMMU_DTLBW_TR_D_MASK        0x00000020
#define SPR_DMMU_DTLBW_TR_D_GET(x)      (((x) >> 5) & 0x1)
#define SPR_DMMU_DTLBW_TR_D_SET(x, y)   (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* User Read Enable */
#define SPR_DMMU_DTLBW_TR_URE_OFFSET    6
#define SPR_DMMU_DTLBW_TR_URE_MASK      0x00000040
#define SPR_DMMU_DTLBW_TR_URE_GET(x)    (((x) >> 6) & 0x1)
#define SPR_DMMU_DTLBW_TR_URE_SET(x, y) (((x) & U(0xffffffbf)) | \
    ((!!(y)) << 6))

/* User Write Enable */
#define SPR_DMMU_DTLBW_TR_UWE_OFFSET    7
#define SPR_DMMU_DTLBW_TR_UWE_MASK      0x00000080
#define SPR_DMMU_DTLBW_TR_UWE_GET(x)    (((x) >> 7) & 0x1)
#define SPR_DMMU_DTLBW_TR_UWE_SET(x, y) (((x) & U(0xffffff7f)) | \
    ((!!(y)) << 7))

/* Supervisor Read Enable */
#define SPR_DMMU_DTLBW_TR_SRE_OFFSET    8
#define SPR_DMMU_DTLBW_TR_SRE_MASK      0x00000100
#define SPR_DMMU_DTLBW_TR_SRE_GET(x)    (((x) >> 8) & 0x1)
#define SPR_DMMU_DTLBW_TR_SRE_SET(x, y) (((x) & U(0xfffffeff)) | \
    ((!!(y)) << 8))

/* Supervisor Write Enable */
#define SPR_DMMU_DTLBW_TR_SWE_OFFSET    9
#define SPR_DMMU_DTLBW_TR_SWE_MASK      0x00000200
#define SPR_DMMU_DTLBW_TR_SWE_GET(x)    (((x) >> 9) & 0x1)
#define SPR_DMMU_DTLBW_TR_SWE_SET(x, y) (((x) & U(0xfffffdff)) | \
    ((!!(y)) << 9))

/* Physical Page Number */
#define SPR_DMMU_DTLBW_TR_PPN_LSB       13
#define SPR_DMMU_DTLBW_TR_PPN_MSB       31
#define SPR_DMMU_DTLBW_TR_PPN_BITS      19
#define SPR_DMMU_DTLBW_TR_PPN_MASK      U(0xffffe000)
#define SPR_DMMU_DTLBW_TR_PPN_GET(x)    (((x) >> 13) & U(0x0007ffff))
#define SPR_DMMU_DTLBW_TR_PPN_SET(x, y) (((x) & U(0x00001fff)) | \
    ((y) << 13))

/*************************/
/* Instruction MMU Group */
/*************************/
#define SPR_IMMU_GROUP                  0x02

/* Instruction MMU Control Register */
#define SPR_IMMU_IMMUCR_INDEX           U(0x000)
#define SPR_IMMU_IMMUCR_ADDR            U(0x1000)

/* ITLB Flush */
#define SPR_IMMU_IMMUCR_ITF_OFFSET      0
#define SPR_IMMU_IMMUCR_ITF_MASK        0x00000001
#define SPR_IMMU_IMMUCR_ITF_GET(x)      (((x) >> 0) & 0x1)
#define SPR_IMMU_IMMUCR_ITF_SET(x, y)   (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Page Table Base Pointer */
#define SPR_IMMU_IMMUCR_PTBP_LSB        10
#define SPR_IMMU_IMMUCR_PTBP_MSB        31
#define SPR_IMMU_IMMUCR_PTBP_BITS       22
#define SPR_IMMU_IMMUCR_PTBP_MASK       U(0xfffffc00)
#define SPR_IMMU_IMMUCR_PTBP_GET(x)     (((x) >> 10) & U(0x003fffff))
#define SPR_IMMU_IMMUCR_PTBP_SET(x, y)  (((x) & U(0x000003ff)) | \
    ((y) << 10))

/* Instruction MMU Protection Register */
#define SPR_IMMU_IMMUPR_INDEX           U(0x001)
#define SPR_IMMU_IMMUPR_ADDR            U(0x1001)

/* Supervisor Execute Enable 1 */
#define SPR_IMMU_IMMUPR_SXE1_OFFSET     0
#define SPR_IMMU_IMMUPR_SXE1_MASK       0x00000001
#define SPR_IMMU_IMMUPR_SXE1_GET(x)     (((x) >> 0) & 0x1)
#define SPR_IMMU_IMMUPR_SXE1_SET(x, y)  (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* User Execute Enable 1 */
#define SPR_IMMU_IMMUPR_UXE1_OFFSET     1
#define SPR_IMMU_IMMUPR_UXE1_MASK       0x00000002
#define SPR_IMMU_IMMUPR_UXE1_GET(x)     (((x) >> 1) & 0x1)
#define SPR_IMMU_IMMUPR_UXE1_SET(x, y)  (((x) & U(0xfffffffd)) | \
    ((!!(y)) << 1))

/* Supervisor Execute Enable 2 */
#define SPR_IMMU_IMMUPR_SXE2_OFFSET     2
#define SPR_IMMU_IMMUPR_SXE2_MASK       0x00000004
#define SPR_IMMU_IMMUPR_SXE2_GET(x)     (((x) >> 2) & 0x1)
#define SPR_IMMU_IMMUPR_SXE2_SET(x, y)  (((x) & U(0xfffffffb)) | \
    ((!!(y)) << 2))

/* User Execute Enable 2 */
#define SPR_IMMU_IMMUPR_UXE2_OFFSET     3
#define SPR_IMMU_IMMUPR_UXE2_MASK       0x00000008
#define SPR_IMMU_IMMUPR_UXE2_GET(x)     (((x) >> 3) & 0x1)
#define SPR_IMMU_IMMUPR_UXE2_SET(x, y)  (((x) & U(0xfffffff7)) | \
    ((!!(y)) << 3))

/* Supervisor Execute Enable 3 */
#define SPR_IMMU_IMMUPR_SXE3_OFFSET     4
#define SPR_IMMU_IMMUPR_SXE3_MASK       0x00000010
#define SPR_IMMU_IMMUPR_SXE3_GET(x)     (((x) >> 4) & 0x1)
#define SPR_IMMU_IMMUPR_SXE3_SET(x, y)  (((x) & U(0xffffffef)) | \
    ((!!(y)) << 4))

/* User Execute Enable 3 */
#define SPR_IMMU_IMMUPR_UXE3_OFFSET     5
#define SPR_IMMU_IMMUPR_UXE3_MASK       0x00000020
#define SPR_IMMU_IMMUPR_UXE3_GET(x)     (((x) >> 5) & 0x1)
#define SPR_IMMU_IMMUPR_UXE3_SET(x, y)  (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* Supervisor Execute Enable 4 */
#define SPR_IMMU_IMMUPR_SXE4_OFFSET     6
#define SPR_IMMU_IMMUPR_SXE4_MASK       0x00000040
#define SPR_IMMU_IMMUPR_SXE4_GET(x)     (((x) >> 6) & 0x1)
#define SPR_IMMU_IMMUPR_SXE4_SET(x, y)  (((x) & U(0xffffffbf)) | \
    ((!!(y)) << 6))

/* User Execute Enable 4 */
#define SPR_IMMU_IMMUPR_UXE4_OFFSET     7
#define SPR_IMMU_IMMUPR_UXE4_MASK       0x00000080
#define SPR_IMMU_IMMUPR_UXE4_GET(x)     (((x) >> 7) & 0x1)
#define SPR_IMMU_IMMUPR_UXE4_SET(x, y)  (((x) & U(0xffffff7f)) | \
    ((!!(y)) << 7))

/* Supervisor Execute Enable 5 */
#define SPR_IMMU_IMMUPR_SXE5_OFFSET     8
#define SPR_IMMU_IMMUPR_SXE5_MASK       0x00000100
#define SPR_IMMU_IMMUPR_SXE5_GET(x)     (((x) >> 8) & 0x1)
#define SPR_IMMU_IMMUPR_SXE5_SET(x, y)  (((x) & U(0xfffffeff)) | \
    ((!!(y)) << 8))

/* User Execute Enable 5 */
#define SPR_IMMU_IMMUPR_UXE5_OFFSET     9
#define SPR_IMMU_IMMUPR_UXE5_MASK       0x00000200
#define SPR_IMMU_IMMUPR_UXE5_GET(x)     (((x) >> 9) & 0x1)
#define SPR_IMMU_IMMUPR_UXE5_SET(x, y)  (((x) & U(0xfffffdff)) | \
    ((!!(y)) << 9))

/* Supervisor Execute Enable 6 */
#define SPR_IMMU_IMMUPR_SXE6_OFFSET     10
#define SPR_IMMU_IMMUPR_SXE6_MASK       0x00000400
#define SPR_IMMU_IMMUPR_SXE6_GET(x)     (((x) >> 10) & 0x1)
#define SPR_IMMU_IMMUPR_SXE6_SET(x, y)  (((x) & U(0xfffffbff)) | \
    ((!!(y)) << 10))

/* User Execute Enable 6 */
#define SPR_IMMU_IMMUPR_UXE6_OFFSET     11
#define SPR_IMMU_IMMUPR_UXE6_MASK       0x00000800
#define SPR_IMMU_IMMUPR_UXE6_GET(x)     (((x) >> 11) & 0x1)
#define SPR_IMMU_IMMUPR_UXE6_SET(x, y)  (((x) & U(0xfffff7ff)) | \
    ((!!(y)) << 11))

/* Supervisor Execute Enable 7 */
#define SPR_IMMU_IMMUPR_SXE7_OFFSET     12
#define SPR_IMMU_IMMUPR_SXE7_MASK       0x00001000
#define SPR_IMMU_IMMUPR_SXE7_GET(x)     (((x) >> 12) & 0x1)
#define SPR_IMMU_IMMUPR_SXE7_SET(x, y)  (((x) & U(0xffffefff)) | \
    ((!!(y)) << 12))

/* User Execute Enable 7 */
#define SPR_IMMU_IMMUPR_UXE7_OFFSET     13
#define SPR_IMMU_IMMUPR_UXE7_MASK       0x00002000
#define SPR_IMMU_IMMUPR_UXE7_GET(x)     (((x) >> 13) & 0x1)
#define SPR_IMMU_IMMUPR_UXE7_SET(x, y)  (((x) & U(0xffffdfff)) | \
    ((!!(y)) << 13))

/* Instruction TLB Entry Invalidate Register */
#define SPR_IMMU_ITLBEIR_INDEX          U(0x002)
#define SPR_IMMU_ITLBEIR_ADDR           U(0x1002)

/* Instruction ATB Match Registers */
#define SPR_IMMU_IATBMR_BASE            U(0x004)
#define SPR_IMMU_IATBMR_COUNT           U(0x004)
#define SPR_IMMU_IATBMR_STEP            U(0x001)
#define SPR_IMMU_IATBMR_INDEX(N)        (SPR_IMMU_IATBMR_BASE + \
    (SPR_IMMU_IATBMR_STEP * (N)))
#define SPR_IMMU_IATBMR_ADDR(N)         ((SPR_IMMU_GROUP << SPR_GROUP_LSB) | \
    SPR_IMMU_IATBMR_INDEX(N))

/* Valid */
#define SPR_IMMU_IATBMR_V_OFFSET        0
#define SPR_IMMU_IATBMR_V_MASK          0x00000001
#define SPR_IMMU_IATBMR_V_GET(x)        (((x) >> 0) & 0x1)
#define SPR_IMMU_IATBMR_V_SET(x, y)     (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Context ID */
#define SPR_IMMU_IATBMR_CID_LSB         1
#define SPR_IMMU_IATBMR_CID_MSB         4
#define SPR_IMMU_IATBMR_CID_BITS        4
#define SPR_IMMU_IATBMR_CID_MASK        U(0x0000001e)
#define SPR_IMMU_IATBMR_CID_GET(x)      (((x) >> 1) & U(0x0000000f))
#define SPR_IMMU_IATBMR_CID_SET(x, y)   (((x) & U(0xffffffe1)) | \
    ((y) << 1))

/* Page Size */
#define SPR_IMMU_IATBMR_PS_OFFSET       5
#define SPR_IMMU_IATBMR_PS_MASK         0x00000020
#define SPR_IMMU_IATBMR_PS_GET(x)       (((x) >> 5) & 0x1)
#define SPR_IMMU_IATBMR_PS_SET(x, y)    (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* Virtual Page Number */
#define SPR_IMMU_IATBMR_VPN_LSB         10
#define SPR_IMMU_IATBMR_VPN_MSB         31
#define SPR_IMMU_IATBMR_VPN_BITS        22
#define SPR_IMMU_IATBMR_VPN_MASK        U(0xfffffc00)
#define SPR_IMMU_IATBMR_VPN_GET(x)      (((x) >> 10) & U(0x003fffff))
#define SPR_IMMU_IATBMR_VPN_SET(x, y)   (((x) & U(0x000003ff)) | \
    ((y) << 10))

/* Instruction ATB Translate Registers */
#define SPR_IMMU_IATBTR_BASE            U(0x008)
#define SPR_IMMU_IATBTR_COUNT           U(0x004)
#define SPR_IMMU_IATBTR_STEP            U(0x001)
#define SPR_IMMU_IATBTR_INDEX(N)        (SPR_IMMU_IATBTR_BASE + \
    (SPR_IMMU_IATBTR_STEP * (N)))
#define SPR_IMMU_IATBTR_ADDR(N)         ((SPR_IMMU_GROUP << SPR_GROUP_LSB) | \
    SPR_IMMU_IATBTR_INDEX(N))

/* Cache Coherency */
#define SPR_IMMU_IATBTR_CC_OFFSET       0
#define SPR_IMMU_IATBTR_CC_MASK         0x00000001
#define SPR_IMMU_IATBTR_CC_GET(x)       (((x) >> 0) & 0x1)
#define SPR_IMMU_IATBTR_CC_SET(x, y)    (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Cache Inhibit */
#define SPR_IMMU_IATBTR_CI_OFFSET       1
#define SPR_IMMU_IATBTR_CI_MASK         0x00000002
#define SPR_IMMU_IATBTR_CI_GET(x)       (((x) >> 1) & 0x1)
#define SPR_IMMU_IATBTR_CI_SET(x, y)    (((x) & U(0xfffffffd)) | \
    ((!!(y)) << 1))

/* Write-back Cache */
#define SPR_IMMU_IATBTR_WBC_OFFSET      2
#define SPR_IMMU_IATBTR_WBC_MASK        0x00000004
#define SPR_IMMU_IATBTR_WBC_GET(x)      (((x) >> 2) & 0x1)
#define SPR_IMMU_IATBTR_WBC_SET(x, y)   (((x) & U(0xfffffffb)) | \
    ((!!(y)) << 2))

/* Weakly-ordered Memory */
#define SPR_IMMU_IATBTR_WOM_OFFSET      3
#define SPR_IMMU_IATBTR_WOM_MASK        0x00000008
#define SPR_IMMU_IATBTR_WOM_GET(x)      (((x) >> 3) & 0x1)
#define SPR_IMMU_IATBTR_WOM_SET(x, y)   (((x) & U(0xfffffff7)) | \
    ((!!(y)) << 3))

/* Accessed */
#define SPR_IMMU_IATBTR_A_OFFSET        4
#define SPR_IMMU_IATBTR_A_MASK          0x00000010
#define SPR_IMMU_IATBTR_A_GET(x)        (((x) >> 4) & 0x1)
#define SPR_IMMU_IATBTR_A_SET(x, y)     (((x) & U(0xffffffef)) | \
    ((!!(y)) << 4))

/* Dirty */
#define SPR_IMMU_IATBTR_D_OFFSET        5
#define SPR_IMMU_IATBTR_D_MASK          0x00000020
#define SPR_IMMU_IATBTR_D_GET(x)        (((x) >> 5) & 0x1)
#define SPR_IMMU_IATBTR_D_SET(x, y)     (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* Supervisor Execute Enable */
#define SPR_IMMU_IATBTR_SRE_OFFSET      6
#define SPR_IMMU_IATBTR_SRE_MASK        0x00000040
#define SPR_IMMU_IATBTR_SRE_GET(x)      (((x) >> 6) & 0x1)
#define SPR_IMMU_IATBTR_SRE_SET(x, y)   (((x) & U(0xffffffbf)) | \
    ((!!(y)) << 6))

/* User Execute Enable */
#define SPR_IMMU_IATBTR_URE_OFFSET      7
#define SPR_IMMU_IATBTR_URE_MASK        0x00000080
#define SPR_IMMU_IATBTR_URE_GET(x)      (((x) >> 7) & 0x1)
#define SPR_IMMU_IATBTR_URE_SET(x, y)   (((x) & U(0xffffff7f)) | \
    ((!!(y)) << 7))

/* Physical Page Number */
#define SPR_IMMU_IATBTR_PPN_LSB         10
#define SPR_IMMU_IATBTR_PPN_MSB         31
#define SPR_IMMU_IATBTR_PPN_BITS        22
#define SPR_IMMU_IATBTR_PPN_MASK        U(0xfffffc00)
#define SPR_IMMU_IATBTR_PPN_GET(x)      (((x) >> 10) & U(0x003fffff))
#define SPR_IMMU_IATBTR_PPN_SET(x, y)   (((x) & U(0x000003ff)) | \
    ((y) << 10))

/* Instruction TLB */
#define SPR_IMMU_ITLBW_BASE             U(0x200)
#define SPR_IMMU_ITLBW_COUNT            U(0x004)
#define SPR_IMMU_ITLBW_STEP             U(0x100)
#define SPR_IMMU_ITLBW_SUBBASE(N0)      (SPR_IMMU_ITLBW_BASE + \
    (SPR_IMMU_ITLBW_STEP * (N0)))

/* Instruction TLB Match Registers */
#define SPR_IMMU_ITLBW_MR_BASE          U(0x000)
#define SPR_IMMU_ITLBW_MR_COUNT         U(0x080)
#define SPR_IMMU_ITLBW_MR_STEP          U(0x001)

#define SPR_IMMU_ITLBW_MR_INDEX(N0, N1) (SPR_IMMU_ITLBW_SUBBASE(N0) + \
    SPR_IMMU_ITLBW_MR_BASE + \
    (SPR_IMMU_ITLBW_MR_STEP * (N1)))
#define SPR_IMMU_ITLBW_MR_ADDR(N0, N1)  ((SPR_IMMU_GROUP << SPR_GROUP_LSB) | \
    SPR_IMMU_ITLBW_MR_INDEX(N0, N1))

/* Valid */
#define SPR_IMMU_ITLBW_MR_V_OFFSET      0
#define SPR_IMMU_ITLBW_MR_V_MASK        0x00000001
#define SPR_IMMU_ITLBW_MR_V_GET(x)      (((x) >> 0) & 0x1)
#define SPR_IMMU_ITLBW_MR_V_SET(x, y)   (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Page Level 1 */
#define SPR_IMMU_ITLBW_MR_PL1_OFFSET    1
#define SPR_IMMU_ITLBW_MR_PL1_MASK      0x00000002
#define SPR_IMMU_ITLBW_MR_PL1_GET(x)    (((x) >> 1) & 0x1)
#define SPR_IMMU_ITLBW_MR_PL1_SET(x, y) (((x) & U(0xfffffffd)) | \
    ((!!(y)) << 1))

/* Context ID */
#define SPR_IMMU_ITLBW_MR_CID_LSB       2
#define SPR_IMMU_ITLBW_MR_CID_MSB       5
#define SPR_IMMU_ITLBW_MR_CID_BITS      4
#define SPR_IMMU_ITLBW_MR_CID_MASK      U(0x0000003c)
#define SPR_IMMU_ITLBW_MR_CID_GET(x)    (((x) >> 2) & U(0x0000000f))
#define SPR_IMMU_ITLBW_MR_CID_SET(x, y) (((x) & U(0xffffffc3)) | \
    ((y) << 2))

/* Least Recently Used */
#define SPR_IMMU_ITLBW_MR_LRU_LSB       6
#define SPR_IMMU_ITLBW_MR_LRU_MSB       7
#define SPR_IMMU_ITLBW_MR_LRU_BITS      2
#define SPR_IMMU_ITLBW_MR_LRU_MASK      U(0x000000c0)
#define SPR_IMMU_ITLBW_MR_LRU_GET(x)    (((x) >> 6) & U(0x00000003))
#define SPR_IMMU_ITLBW_MR_LRU_SET(x, y) (((x) & U(0xffffff3f)) | \
    ((y) << 6))

/* Virtual Page Number */
#define SPR_IMMU_ITLBW_MR_VPN_LSB       13
#define SPR_IMMU_ITLBW_MR_VPN_MSB       31
#define SPR_IMMU_ITLBW_MR_VPN_BITS      19
#define SPR_IMMU_ITLBW_MR_VPN_MASK      U(0xffffe000)
#define SPR_IMMU_ITLBW_MR_VPN_GET(x)    (((x) >> 13) & U(0x0007ffff))
#define SPR_IMMU_ITLBW_MR_VPN_SET(x, y) (((x) & U(0x00001fff)) | \
    ((y) << 13))

/* Instruction TLB Translate Registers */
#define SPR_IMMU_ITLBW_TR_BASE          U(0x080)
#define SPR_IMMU_ITLBW_TR_COUNT         U(0x080)
#define SPR_IMMU_ITLBW_TR_STEP          U(0x001)

#define SPR_IMMU_ITLBW_TR_INDEX(N0, N1) (SPR_IMMU_ITLBW_SUBBASE(N0) + \
    SPR_IMMU_ITLBW_TR_BASE + \
    (SPR_IMMU_ITLBW_TR_STEP * (N1)))
#define SPR_IMMU_ITLBW_TR_ADDR(N0, N1)  ((SPR_IMMU_GROUP << SPR_GROUP_LSB) | \
    SPR_IMMU_ITLBW_TR_INDEX(N0, N1))

/* Cache Coherency */
#define SPR_IMMU_ITLBW_TR_CC_OFFSET     0
#define SPR_IMMU_ITLBW_TR_CC_MASK       0x00000001
#define SPR_IMMU_ITLBW_TR_CC_GET(x)     (((x) >> 0) & 0x1)
#define SPR_IMMU_ITLBW_TR_CC_SET(x, y)  (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Cache Inhibit */
#define SPR_IMMU_ITLBW_TR_CI_OFFSET     1
#define SPR_IMMU_ITLBW_TR_CI_MASK       0x00000002
#define SPR_IMMU_ITLBW_TR_CI_GET(x)     (((x) >> 1) & 0x1)
#define SPR_IMMU_ITLBW_TR_CI_SET(x, y)  (((x) & U(0xfffffffd)) | \
    ((!!(y)) << 1))

/* Write-back Cache */
#define SPR_IMMU_ITLBW_TR_WBC_OFFSET    2
#define SPR_IMMU_ITLBW_TR_WBC_MASK      0x00000004
#define SPR_IMMU_ITLBW_TR_WBC_GET(x)    (((x) >> 2) & 0x1)
#define SPR_IMMU_ITLBW_TR_WBC_SET(x, y) (((x) & U(0xfffffffb)) | \
    ((!!(y)) << 2))

/* Weakly-ordered Memory */
#define SPR_IMMU_ITLBW_TR_WOM_OFFSET    3
#define SPR_IMMU_ITLBW_TR_WOM_MASK      0x00000008
#define SPR_IMMU_ITLBW_TR_WOM_GET(x)    (((x) >> 3) & 0x1)
#define SPR_IMMU_ITLBW_TR_WOM_SET(x, y) (((x) & U(0xfffffff7)) | \
    ((!!(y)) << 3))

/* Accessed */
#define SPR_IMMU_ITLBW_TR_A_OFFSET      4
#define SPR_IMMU_ITLBW_TR_A_MASK        0x00000010
#define SPR_IMMU_ITLBW_TR_A_GET(x)      (((x) >> 4) & 0x1)
#define SPR_IMMU_ITLBW_TR_A_SET(x, y)   (((x) & U(0xffffffef)) | \
    ((!!(y)) << 4))

/* Dirty */
#define SPR_IMMU_ITLBW_TR_D_OFFSET      5
#define SPR_IMMU_ITLBW_TR_D_MASK        0x00000020
#define SPR_IMMU_ITLBW_TR_D_GET(x)      (((x) >> 5) & 0x1)
#define SPR_IMMU_ITLBW_TR_D_SET(x, y)   (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* User Execute Enable */
#define SPR_IMMU_ITLBW_TR_UXE_OFFSET    6
#define SPR_IMMU_ITLBW_TR_UXE_MASK      0x00000040
#define SPR_IMMU_ITLBW_TR_UXE_GET(x)    (((x) >> 6) & 0x1)
#define SPR_IMMU_ITLBW_TR_UXE_SET(x, y) (((x) & U(0xffffffbf)) | \
    ((!!(y)) << 6))

/* Supervisor Execute Enable */
#define SPR_IMMU_ITLBW_TR_SXE_OFFSET    7
#define SPR_IMMU_ITLBW_TR_SXE_MASK      0x00000080
#define SPR_IMMU_ITLBW_TR_SXE_GET(x)    (((x) >> 7) & 0x1)
#define SPR_IMMU_ITLBW_TR_SXE_SET(x, y) (((x) & U(0xffffff7f)) | \
    ((!!(y)) << 7))

/* Physical Page Number */
#define SPR_IMMU_ITLBW_TR_PPN_LSB       13
#define SPR_IMMU_ITLBW_TR_PPN_MSB       31
#define SPR_IMMU_ITLBW_TR_PPN_BITS      19
#define SPR_IMMU_ITLBW_TR_PPN_MASK      U(0xffffe000)
#define SPR_IMMU_ITLBW_TR_PPN_GET(x)    (((x) >> 13) & U(0x0007ffff))
#define SPR_IMMU_ITLBW_TR_PPN_SET(x, y) (((x) & U(0x00001fff)) | \
    ((y) << 13))

/********************/
/* Data Cache Group */
/********************/
#define SPR_DCACHE_GROUP             0x03

/* Data Cache Control Register */
#define SPR_DCACHE_DCCR_INDEX        U(0x000)
#define SPR_DCACHE_DCCR_ADDR         U(0x1800)

/* Enable Ways */
#define SPR_DCACHE_DCCR_EW_LSB       0
#define SPR_DCACHE_DCCR_EW_MSB       7
#define SPR_DCACHE_DCCR_EW_BITS      8
#define SPR_DCACHE_DCCR_EW_MASK      U(0x000000ff)
#define SPR_DCACHE_DCCR_EW_GET(x)    (((x) >> 0) & U(0x000000ff))
#define SPR_DCACHE_DCCR_EW_SET(x, y) (((x) & U(0xffffff00)) | \
    ((y) << 0))

/* Data Cache Block Prefetch Register */
#define SPR_DCACHE_DCBPR_INDEX       U(0x001)
#define SPR_DCACHE_DCBPR_ADDR        U(0x1801)

/* Data Cache Block Flush Register */
#define SPR_DCACHE_DCBFR_INDEX       U(0x002)
#define SPR_DCACHE_DCBFR_ADDR        U(0x1802)

/* Data Cache Block Invalidate Register */
#define SPR_DCACHE_DCBIR_INDEX       U(0x003)
#define SPR_DCACHE_DCBIR_ADDR        U(0x1803)

/* Data Cache Block Write-back Register */
#define SPR_DCACHE_DCBWR_INDEX       U(0x004)
#define SPR_DCACHE_DCBWR_ADDR        U(0x1804)

/* Data Cache Block Lock Register */
#define SPR_DCACHE_DCBLR_INDEX       U(0x005)
#define SPR_DCACHE_DCBLR_ADDR        U(0x1805)

/***************************/
/* Instruction Cache Group */
/***************************/
#define SPR_ICACHE_GROUP             0x04

/* Instruction Cache Control Register */
#define SPR_ICACHE_ICCR_INDEX        U(0x000)
#define SPR_ICACHE_ICCR_ADDR         U(0x2000)

/* Enable Ways */
#define SPR_ICACHE_ICCR_EW_LSB       0
#define SPR_ICACHE_ICCR_EW_MSB       7
#define SPR_ICACHE_ICCR_EW_BITS      8
#define SPR_ICACHE_ICCR_EW_MASK      U(0x000000ff)
#define SPR_ICACHE_ICCR_EW_GET(x)    (((x) >> 0) & U(0x000000ff))
#define SPR_ICACHE_ICCR_EW_SET(x, y) (((x) & U(0xffffff00)) | \
    ((y) << 0))

/* Instruction Cache Block Prefetch Register */
#define SPR_ICACHE_ICBPR_INDEX       U(0x001)
#define SPR_ICACHE_ICBPR_ADDR        U(0x2001)

/* Instruction Cache Block Invalidate Register */
#define SPR_ICACHE_ICBIR_INDEX       U(0x002)
#define SPR_ICACHE_ICBIR_ADDR        U(0x2002)

/* Instruction Cache Block Lock Register */
#define SPR_ICACHE_ICBLR_INDEX       U(0x003)
#define SPR_ICACHE_ICBLR_ADDR        U(0x2003)

/*********************************/
/* Multiply and Accumulate Group */
/*********************************/
#define SPR_MAC_GROUP       0x05

/* MAC Result Low Word */
#define SPR_MAC_MACLO_INDEX U(0x001)
#define SPR_MAC_MACLO_ADDR  U(0x2801)

/* MAC Result High Word */
#define SPR_MAC_MACHI_INDEX U(0x002)
#define SPR_MAC_MACHI_ADDR  U(0x2802)

/***************/
/* Debug Group */
/***************/
#define SPR_DEBUG_GROUP               0x06

/* Debug Value Registers */
#define SPR_DEBUG_DVR_BASE            U(0x000)
#define SPR_DEBUG_DVR_COUNT           U(0x008)
#define SPR_DEBUG_DVR_STEP            U(0x001)
#define SPR_DEBUG_DVR_INDEX(N)        (SPR_DEBUG_DVR_BASE + \
    (SPR_DEBUG_DVR_STEP * (N)))
#define SPR_DEBUG_DVR_ADDR(N)         ((SPR_DEBUG_GROUP << SPR_GROUP_LSB) | \
    SPR_DEBUG_DVR_INDEX(N))

/* Debug Control Registers */
#define SPR_DEBUG_DCR_BASE            U(0x008)
#define SPR_DEBUG_DCR_COUNT           U(0x008)
#define SPR_DEBUG_DCR_STEP            U(0x001)
#define SPR_DEBUG_DCR_INDEX(N)        (SPR_DEBUG_DCR_BASE + \
    (SPR_DEBUG_DCR_STEP * (N)))
#define SPR_DEBUG_DCR_ADDR(N)         ((SPR_DEBUG_GROUP << SPR_GROUP_LSB) | \
    SPR_DEBUG_DCR_INDEX(N))

/* DVR/DCR Present */
#define SPR_DEBUG_DCR_DP_OFFSET       0
#define SPR_DEBUG_DCR_DP_MASK         0x00000001
#define SPR_DEBUG_DCR_DP_GET(x)       (((x) >> 0) & 0x1)
#define SPR_DEBUG_DCR_DP_SET(x, y)    (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Compare Condition */
#define SPR_DEBUG_DCR_CC_LSB          1
#define SPR_DEBUG_DCR_CC_MSB          3
#define SPR_DEBUG_DCR_CC_BITS         3
#define SPR_DEBUG_DCR_CC_MASK         U(0x0000000e)
#define SPR_DEBUG_DCR_CC_GET(x)       (((x) >> 1) & U(0x00000007))
#define SPR_DEBUG_DCR_CC_SET(x, y)    (((x) & U(0xfffffff1)) | \
    ((y) << 1))

/* Masked */
#define SPR_DEBUG_DCR_CC_MASKED       0
/* Equal */
#define SPR_DEBUG_DCR_CC_EQ           1
/* Less than */
#define SPR_DEBUG_DCR_CC_LT           2
/* Less than or equal */
#define SPR_DEBUG_DCR_CC_LTE          3
/* Greater than */
#define SPR_DEBUG_DCR_CC_GT           4
/* Greater than or equal */
#define SPR_DEBUG_DCR_CC_GTE          5
/* Not equal */
#define SPR_DEBUG_DCR_CC_NEQ          6

/* Signed Comparison */
#define SPR_DEBUG_DCR_SC_OFFSET       4
#define SPR_DEBUG_DCR_SC_MASK         0x00000010
#define SPR_DEBUG_DCR_SC_GET(x)       (((x) >> 4) & 0x1)
#define SPR_DEBUG_DCR_SC_SET(x, y)    (((x) & U(0xffffffef)) | \
    ((!!(y)) << 4))

/* Compare To */
#define SPR_DEBUG_DCR_CT_LSB          5
#define SPR_DEBUG_DCR_CT_MSB          7
#define SPR_DEBUG_DCR_CT_BITS         3
#define SPR_DEBUG_DCR_CT_MASK         U(0x000000e0)
#define SPR_DEBUG_DCR_CT_GET(x)       (((x) >> 5) & U(0x00000007))
#define SPR_DEBUG_DCR_CT_SET(x, y)    (((x) & U(0xffffff1f)) | \
    ((y) << 5))

/* Comparison disabled */
#define SPR_DEBUG_DCR_CT_DISABLED     0
/* Instruction fetch EA */
#define SPR_DEBUG_DCR_CT_FEA          1
/* Load EA */
#define SPR_DEBUG_DCR_CT_LEA          2
/* Store EA */
#define SPR_DEBUG_DCR_CT_SEA          3
/* Load data */
#define SPR_DEBUG_DCR_CT_LD           4
/* Store data */
#define SPR_DEBUG_DCR_CT_SD           5
/* Load/store EA */
#define SPR_DEBUG_DCR_CT_LSEA         6
/* Load/store data */
#define SPR_DEBUG_DCR_CT_LSD          7

/* Debug Mode Register 1 */
#define SPR_DEBUG_DMR1_INDEX          U(0x010)
#define SPR_DEBUG_DMR1_ADDR           U(0x3010)

/* Chain Watchpoint 0 */
#define SPR_DEBUG_DMR1_CW0_LSB        0
#define SPR_DEBUG_DMR1_CW0_MSB        1
#define SPR_DEBUG_DMR1_CW0_BITS       2
#define SPR_DEBUG_DMR1_CW0_MASK       U(0x00000003)
#define SPR_DEBUG_DMR1_CW0_GET(x)     (((x) >> 0) & U(0x00000003))
#define SPR_DEBUG_DMR1_CW0_SET(x, y)  (((x) & U(0xfffffffc)) | \
    ((y) << 0))

/* Chain Watchpoint 1 */
#define SPR_DEBUG_DMR1_CW1_LSB        2
#define SPR_DEBUG_DMR1_CW1_MSB        3
#define SPR_DEBUG_DMR1_CW1_BITS       2
#define SPR_DEBUG_DMR1_CW1_MASK       U(0x0000000c)
#define SPR_DEBUG_DMR1_CW1_GET(x)     (((x) >> 2) & U(0x00000003))
#define SPR_DEBUG_DMR1_CW1_SET(x, y)  (((x) & U(0xfffffff3)) | \
    ((y) << 2))

/* Chain Watchpoint 2 */
#define SPR_DEBUG_DMR1_CW2_LSB        4
#define SPR_DEBUG_DMR1_CW2_MSB        5
#define SPR_DEBUG_DMR1_CW2_BITS       2
#define SPR_DEBUG_DMR1_CW2_MASK       U(0x00000030)
#define SPR_DEBUG_DMR1_CW2_GET(x)     (((x) >> 4) & U(0x00000003))
#define SPR_DEBUG_DMR1_CW2_SET(x, y)  (((x) & U(0xffffffcf)) | \
    ((y) << 4))

/* Chain Watchpoint 3 */
#define SPR_DEBUG_DMR1_CW3_LSB        6
#define SPR_DEBUG_DMR1_CW3_MSB        7
#define SPR_DEBUG_DMR1_CW3_BITS       2
#define SPR_DEBUG_DMR1_CW3_MASK       U(0x000000c0)
#define SPR_DEBUG_DMR1_CW3_GET(x)     (((x) >> 6) & U(0x00000003))
#define SPR_DEBUG_DMR1_CW3_SET(x, y)  (((x) & U(0xffffff3f)) | \
    ((y) << 6))

/* Chain Watchpoint 4 */
#define SPR_DEBUG_DMR1_CW4_LSB        9
#define SPR_DEBUG_DMR1_CW4_MSB        9
#define SPR_DEBUG_DMR1_CW4_BITS       1
#define SPR_DEBUG_DMR1_CW4_MASK       U(0x00000200)
#define SPR_DEBUG_DMR1_CW4_GET(x)     (((x) >> 9) & U(0x00000001))
#define SPR_DEBUG_DMR1_CW4_SET(x, y)  (((x) & U(0xfffffdff)) | \
    ((y) << 9))

/* Chain Watchpoint 5 */
#define SPR_DEBUG_DMR1_CW5_LSB        10
#define SPR_DEBUG_DMR1_CW5_MSB        11
#define SPR_DEBUG_DMR1_CW5_BITS       2
#define SPR_DEBUG_DMR1_CW5_MASK       U(0x00000c00)
#define SPR_DEBUG_DMR1_CW5_GET(x)     (((x) >> 10) & U(0x00000003))
#define SPR_DEBUG_DMR1_CW5_SET(x, y)  (((x) & U(0xfffff3ff)) | \
    ((y) << 10))

/* Chain Watchpoint 6 */
#define SPR_DEBUG_DMR1_CW6_LSB        12
#define SPR_DEBUG_DMR1_CW6_MSB        13
#define SPR_DEBUG_DMR1_CW6_BITS       2
#define SPR_DEBUG_DMR1_CW6_MASK       U(0x00003000)
#define SPR_DEBUG_DMR1_CW6_GET(x)     (((x) >> 12) & U(0x00000003))
#define SPR_DEBUG_DMR1_CW6_SET(x, y)  (((x) & U(0xffffcfff)) | \
    ((y) << 12))

/* Chain Watchpoint 7 */
#define SPR_DEBUG_DMR1_CW7_LSB        14
#define SPR_DEBUG_DMR1_CW7_MSB        15
#define SPR_DEBUG_DMR1_CW7_BITS       2
#define SPR_DEBUG_DMR1_CW7_MASK       U(0x0000c000)
#define SPR_DEBUG_DMR1_CW7_GET(x)     (((x) >> 14) & U(0x00000003))
#define SPR_DEBUG_DMR1_CW7_SET(x, y)  (((x) & U(0xffff3fff)) | \
    ((y) << 14))

/* Chain Watchpoint 8 */
#define SPR_DEBUG_DMR1_CW8_LSB        16
#define SPR_DEBUG_DMR1_CW8_MSB        17
#define SPR_DEBUG_DMR1_CW8_BITS       2
#define SPR_DEBUG_DMR1_CW8_MASK       U(0x00030000)
#define SPR_DEBUG_DMR1_CW8_GET(x)     (((x) >> 16) & U(0x00000003))
#define SPR_DEBUG_DMR1_CW8_SET(x, y)  (((x) & U(0xfffcffff)) | \
    ((y) << 16))

/* Chain Watchpoint 9 */
#define SPR_DEBUG_DMR1_CW9_LSB        18
#define SPR_DEBUG_DMR1_CW9_MSB        19
#define SPR_DEBUG_DMR1_CW9_BITS       2
#define SPR_DEBUG_DMR1_CW9_MASK       U(0x000c0000)
#define SPR_DEBUG_DMR1_CW9_GET(x)     (((x) >> 18) & U(0x00000003))
#define SPR_DEBUG_DMR1_CW9_SET(x, y)  (((x) & U(0xfff3ffff)) | \
    ((y) << 18))

/* Single-step Trace */
#define SPR_DEBUG_DMR1_ST_OFFSET      22
#define SPR_DEBUG_DMR1_ST_MASK        0x00400000
#define SPR_DEBUG_DMR1_ST_GET(x)      (((x) >> 22) & 0x1)
#define SPR_DEBUG_DMR1_ST_SET(x, y)   (((x) & U(0xffbfffff)) | \
    ((!!(y)) << 22))

/* Branch Trace */
#define SPR_DEBUG_DMR1_BT_OFFSET      23
#define SPR_DEBUG_DMR1_BT_MASK        0x00800000
#define SPR_DEBUG_DMR1_BT_GET(x)      (((x) >> 23) & 0x1)
#define SPR_DEBUG_DMR1_BT_SET(x, y)   (((x) & U(0xff7fffff)) | \
    ((!!(y)) << 23))

/* Debug Mode Register 2 */
#define SPR_DEBUG_DMR2_INDEX          U(0x011)
#define SPR_DEBUG_DMR2_ADDR           U(0x3011)

/* Watchpoint Counter Enable 0 */
#define SPR_DEBUG_DMR2_WCE0_OFFSET    0
#define SPR_DEBUG_DMR2_WCE0_MASK      0x00000001
#define SPR_DEBUG_DMR2_WCE0_GET(x)    (((x) >> 0) & 0x1)
#define SPR_DEBUG_DMR2_WCE0_SET(x, y) (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Watchpoint Counter Enable 1 */
#define SPR_DEBUG_DMR2_WCE1_OFFSET    1
#define SPR_DEBUG_DMR2_WCE1_MASK      0x00000002
#define SPR_DEBUG_DMR2_WCE1_GET(x)    (((x) >> 1) & 0x1)
#define SPR_DEBUG_DMR2_WCE1_SET(x, y) (((x) & U(0xfffffffd)) | \
    ((!!(y)) << 1))

/* Assign Watchpoints to Counter */
#define SPR_DEBUG_DMR2_AWTC_LSB       2
#define SPR_DEBUG_DMR2_AWTC_MSB       11
#define SPR_DEBUG_DMR2_AWTC_BITS      10
#define SPR_DEBUG_DMR2_AWTC_MASK      U(0x00000ffc)
#define SPR_DEBUG_DMR2_AWTC_GET(x)    (((x) >> 2) & U(0x000003ff))
#define SPR_DEBUG_DMR2_AWTC_SET(x, y) (((x) & U(0xfffff003)) | \
    ((y) << 2))

/* Watchpoints Generating Breakpoint */
#define SPR_DEBUG_DMR2_WGB_LSB        12
#define SPR_DEBUG_DMR2_WGB_MSB        21
#define SPR_DEBUG_DMR2_WGB_BITS       10
#define SPR_DEBUG_DMR2_WGB_MASK       U(0x003ff000)
#define SPR_DEBUG_DMR2_WGB_GET(x)     (((x) >> 12) & U(0x000003ff))
#define SPR_DEBUG_DMR2_WGB_SET(x, y)  (((x) & U(0xffc00fff)) | \
    ((y) << 12))

/* Watchpoints Breakpoint Status */
#define SPR_DEBUG_DMR2_WBS_LSB        22
#define SPR_DEBUG_DMR2_WBS_MSB        31
#define SPR_DEBUG_DMR2_WBS_BITS       10
#define SPR_DEBUG_DMR2_WBS_MASK       U(0xffc00000)
#define SPR_DEBUG_DMR2_WBS_GET(x)     (((x) >> 22) & U(0x000003ff))
#define SPR_DEBUG_DMR2_WBS_SET(x, y)  (((x) & U(0x003fffff)) | \
    ((y) << 22))

/* Debug Watchpoint Counter Registers */
#define SPR_DEBUG_DCWR_BASE           U(0x012)
#define SPR_DEBUG_DCWR_COUNT          U(0x002)
#define SPR_DEBUG_DCWR_STEP           U(0x001)
#define SPR_DEBUG_DCWR_INDEX(N)       (SPR_DEBUG_DCWR_BASE + \
    (SPR_DEBUG_DCWR_STEP * (N)))
#define SPR_DEBUG_DCWR_ADDR(N)        ((SPR_DEBUG_GROUP << SPR_GROUP_LSB) | \
    SPR_DEBUG_DCWR_INDEX(N))

/* Debug Stop Register */
#define SPR_DEBUG_DSR_INDEX           U(0x014)
#define SPR_DEBUG_DSR_ADDR            U(0x3014)

/* Reset Exception */
#define SPR_DEBUG_DSR_RSTE_OFFSET     0
#define SPR_DEBUG_DSR_RSTE_MASK       0x00000001
#define SPR_DEBUG_DSR_RSTE_GET(x)     (((x) >> 0) & 0x1)
#define SPR_DEBUG_DSR_RSTE_SET(x, y)  (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Bus Error Exception */
#define SPR_DEBUG_DSR_BUSEE_OFFSET    1
#define SPR_DEBUG_DSR_BUSEE_MASK      0x00000002
#define SPR_DEBUG_DSR_BUSEE_GET(x)    (((x) >> 1) & 0x1)
#define SPR_DEBUG_DSR_BUSEE_SET(x, y) (((x) & U(0xfffffffd)) | \
    ((!!(y)) << 1))

/* Data Page Fault Exception */
#define SPR_DEBUG_DSR_DPFE_OFFSET     2
#define SPR_DEBUG_DSR_DPFE_MASK       0x00000004
#define SPR_DEBUG_DSR_DPFE_GET(x)     (((x) >> 2) & 0x1)
#define SPR_DEBUG_DSR_DPFE_SET(x, y)  (((x) & U(0xfffffffb)) | \
    ((!!(y)) << 2))

/* Instruction Page Fault Exception */
#define SPR_DEBUG_DSR_IPFE_OFFSET     3
#define SPR_DEBUG_DSR_IPFE_MASK       0x00000008
#define SPR_DEBUG_DSR_IPFE_GET(x)     (((x) >> 3) & 0x1)
#define SPR_DEBUG_DSR_IPFE_SET(x, y)  (((x) & U(0xfffffff7)) | \
    ((!!(y)) << 3))

/* Tick Timer Exception */
#define SPR_DEBUG_DSR_TTE_OFFSET      4
#define SPR_DEBUG_DSR_TTE_MASK        0x00000010
#define SPR_DEBUG_DSR_TTE_GET(x)      (((x) >> 4) & 0x1)
#define SPR_DEBUG_DSR_TTE_SET(x, y)   (((x) & U(0xffffffef)) | \
    ((!!(y)) << 4))

/* Alignment Exception */
#define SPR_DEBUG_DSR_AE_OFFSET       5
#define SPR_DEBUG_DSR_AE_MASK         0x00000020
#define SPR_DEBUG_DSR_AE_GET(x)       (((x) >> 5) & 0x1)
#define SPR_DEBUG_DSR_AE_SET(x, y)    (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* Illegal Instruction Exception */
#define SPR_DEBUG_DSR_IIE_OFFSET      6
#define SPR_DEBUG_DSR_IIE_MASK        0x00000040
#define SPR_DEBUG_DSR_IIE_GET(x)      (((x) >> 6) & 0x1)
#define SPR_DEBUG_DSR_IIE_SET(x, y)   (((x) & U(0xffffffbf)) | \
    ((!!(y)) << 6))

/* Interrupt Exception */
#define SPR_DEBUG_DSR_INTE_OFFSET     7
#define SPR_DEBUG_DSR_INTE_MASK       0x00000080
#define SPR_DEBUG_DSR_INTE_GET(x)     (((x) >> 7) & 0x1)
#define SPR_DEBUG_DSR_INTE_SET(x, y)  (((x) & U(0xffffff7f)) | \
    ((!!(y)) << 7))

/* DTLB Miss Exception */
#define SPR_DEBUG_DSR_DME_OFFSET      8
#define SPR_DEBUG_DSR_DME_MASK        0x00000100
#define SPR_DEBUG_DSR_DME_GET(x)      (((x) >> 8) & 0x1)
#define SPR_DEBUG_DSR_DME_SET(x, y)   (((x) & U(0xfffffeff)) | \
    ((!!(y)) << 8))

/* ITLB Miss Exception */
#define SPR_DEBUG_DSR_IME_OFFSET      9
#define SPR_DEBUG_DSR_IME_MASK        0x00000200
#define SPR_DEBUG_DSR_IME_GET(x)      (((x) >> 9) & 0x1)
#define SPR_DEBUG_DSR_IME_SET(x, y)   (((x) & U(0xfffffdff)) | \
    ((!!(y)) << 9))

/* Range Exception */
#define SPR_DEBUG_DSR_RE_OFFSET       10
#define SPR_DEBUG_DSR_RE_MASK         0x00000400
#define SPR_DEBUG_DSR_RE_GET(x)       (((x) >> 10) & 0x1)
#define SPR_DEBUG_DSR_RE_SET(x, y)    (((x) & U(0xfffffbff)) | \
    ((!!(y)) << 10))

/* System Call Exception */
#define SPR_DEBUG_DSR_SCE_OFFSET      11
#define SPR_DEBUG_DSR_SCE_MASK        0x00000800
#define SPR_DEBUG_DSR_SCE_GET(x)      (((x) >> 11) & 0x1)
#define SPR_DEBUG_DSR_SCE_SET(x, y)   (((x) & U(0xfffff7ff)) | \
    ((!!(y)) << 11))

/* Floating Point Exception */
#define SPR_DEBUG_DSR_FPE_OFFSET      12
#define SPR_DEBUG_DSR_FPE_MASK        0x00001000
#define SPR_DEBUG_DSR_FPE_GET(x)      (((x) >> 12) & 0x1)
#define SPR_DEBUG_DSR_FPE_SET(x, y)   (((x) & U(0xffffefff)) | \
    ((!!(y)) << 12))

/* Trap Exception */
#define SPR_DEBUG_DSR_TE_OFFSET       13
#define SPR_DEBUG_DSR_TE_MASK         0x00002000
#define SPR_DEBUG_DSR_TE_GET(x)       (((x) >> 13) & 0x1)
#define SPR_DEBUG_DSR_TE_SET(x, y)    (((x) & U(0xffffdfff)) | \
    ((!!(y)) << 13))

/* Debug Reason Register */
#define SPR_DEBUG_DRR_INDEX           U(0x015)
#define SPR_DEBUG_DRR_ADDR            U(0x3015)

/* Reset Exception */
#define SPR_DEBUG_DRR_RSTE_OFFSET     0
#define SPR_DEBUG_DRR_RSTE_MASK       0x00000001
#define SPR_DEBUG_DRR_RSTE_GET(x)     (((x) >> 0) & 0x1)
#define SPR_DEBUG_DRR_RSTE_SET(x, y)  (((x) & U(0xfffffffe)) | \
    ((!!(y)) << 0))

/* Bus Error Exception */
#define SPR_DEBUG_DRR_BUSEE_OFFSET    1
#define SPR_DEBUG_DRR_BUSEE_MASK      0x00000002
#define SPR_DEBUG_DRR_BUSEE_GET(x)    (((x) >> 1) & 0x1)
#define SPR_DEBUG_DRR_BUSEE_SET(x, y) (((x) & U(0xfffffffd)) | \
    ((!!(y)) << 1))

/* Data Page Fault Exception */
#define SPR_DEBUG_DRR_DPFE_OFFSET     2
#define SPR_DEBUG_DRR_DPFE_MASK       0x00000004
#define SPR_DEBUG_DRR_DPFE_GET(x)     (((x) >> 2) & 0x1)
#define SPR_DEBUG_DRR_DPFE_SET(x, y)  (((x) & U(0xfffffffb)) | \
    ((!!(y)) << 2))

/* Instruction Page Fault Exception */
#define SPR_DEBUG_DRR_IPFE_OFFSET     3
#define SPR_DEBUG_DRR_IPFE_MASK       0x00000008
#define SPR_DEBUG_DRR_IPFE_GET(x)     (((x) >> 3) & 0x1)
#define SPR_DEBUG_DRR_IPFE_SET(x, y)  (((x) & U(0xfffffff7)) | \
    ((!!(y)) << 3))

/* Tick Timer Exception */
#define SPR_DEBUG_DRR_TTE_OFFSET      4
#define SPR_DEBUG_DRR_TTE_MASK        0x00000010
#define SPR_DEBUG_DRR_TTE_GET(x)      (((x) >> 4) & 0x1)
#define SPR_DEBUG_DRR_TTE_SET(x, y)   (((x) & U(0xffffffef)) | \
    ((!!(y)) << 4))

/* Alignment Exception */
#define SPR_DEBUG_DRR_AE_OFFSET       5
#define SPR_DEBUG_DRR_AE_MASK         0x00000020
#define SPR_DEBUG_DRR_AE_GET(x)       (((x) >> 5) & 0x1)
#define SPR_DEBUG_DRR_AE_SET(x, y)    (((x) & U(0xffffffdf)) | \
    ((!!(y)) << 5))

/* Illegal Instruction Exception */
#define SPR_DEBUG_DRR_IIE_OFFSET      6
#define SPR_DEBUG_DRR_IIE_MASK        0x00000040
#define SPR_DEBUG_DRR_IIE_GET(x)      (((x) >> 6) & 0x1)
#define SPR_DEBUG_DRR_IIE_SET(x, y)   (((x) & U(0xffffffbf)) | \
    ((!!(y)) << 6))

/* Interrupt Exception */
#define SPR_DEBUG_DRR_INTE_OFFSET     7
#define SPR_DEBUG_DRR_INTE_MASK       0x00000080
#define SPR_DEBUG_DRR_INTE_GET(x)     (((x) >> 7) & 0x1)
#define SPR_DEBUG_DRR_INTE_SET(x, y)  (((x) & U(0xffffff7f)) | \
    ((!!(y)) << 7))

/* DTLB Miss Exception */
#define SPR_DEBUG_DRR_DME_OFFSET      8
#define SPR_DEBUG_DRR_DME_MASK        0x00000100
#define SPR_DEBUG_DRR_DME_GET(x)      (((x) >> 8) & 0x1)
#define SPR_DEBUG_DRR_DME_SET(x, y)   (((x) & U(0xfffffeff)) | \
    ((!!(y)) << 8))

/* ITLB Miss Exception */
#define SPR_DEBUG_DRR_IME_OFFSET      9
#define SPR_DEBUG_DRR_IME_MASK        0x00000200
#define SPR_DEBUG_DRR_IME_GET(x)      (((x) >> 9) & 0x1)
#define SPR_DEBUG_DRR_IME_SET(x, y)   (((x) & U(0xfffffdff)) | \
    ((!!(y)) << 9))

/* Range Exception */
#define SPR_DEBUG_DRR_RE_OFFSET       10
#define SPR_DEBUG_DRR_RE_MASK         0x00000400
#define SPR_DEBUG_DRR_RE_GET(x)       (((x) >> 10) & 0x1)
#define SPR_DEBUG_DRR_RE_SET(x, y)    (((x) & U(0xfffffbff)) | \
    ((!!(y)) << 10))

/* System Call Exception */
#define SPR_DEBUG_DRR_SCE_OFFSET      11
#define SPR_DEBUG_DRR_SCE_MASK        0x00000800
#define SPR_DEBUG_DRR_SCE_GET(x)      (((x) >> 11) & 0x1)
#define SPR_DEBUG_DRR_SCE_SET(x, y)   (((x) & U(0xfffff7ff)) | \
    ((!!(y)) << 11))

/* Floating Point Exception */
#define SPR_DEBUG_DRR_FPE_OFFSET      12
#define SPR_DEBUG_DRR_FPE_MASK        0x00001000
#define SPR_DEBUG_DRR_FPE_GET(x)      (((x) >> 12) & 0x1)
#define SPR_DEBUG_DRR_FPE_SET(x, y)   (((x) & U(0xffffefff)) | \
    ((!!(y)) << 12))

/* Trap Exception */
#define SPR_DEBUG_DRR_TE_OFFSET       13
#define SPR_DEBUG_DRR_TE_MASK         0x00002000
#define SPR_DEBUG_DRR_TE_GET(x)       (((x) >> 13) & 0x1)
#define SPR_DEBUG_DRR_TE_SET(x, y)    (((x) & U(0xffffdfff)) | \
    ((!!(y)) << 13))

/******************************/
/* Performance Counters Group */
/******************************/
#define SPR_PERF_GROUP         0x07

/* Performance Counters Count Registers */
#define SPR_PERF_PCCR_BASE     U(0x000)
#define SPR_PERF_PCCR_COUNT    U(0x008)
#define SPR_PERF_PCCR_STEP     U(0x001)
#define SPR_PERF_PCCR_INDEX(N) (SPR_PERF_PCCR_BASE + \
    (SPR_PERF_PCCR_STEP * (N)))
#define SPR_PERF_PCCR_ADDR(N)  ((SPR_PERF_GROUP << SPR_GROUP_LSB) | \
    SPR_PERF_PCCR_INDEX(N))

/* Performance Counters Mode Registers */
#define SPR_PERF_PCMR_BASE     U(0x008)
#define SPR_PERF_PCMR_COUNT    U(0x008)
#define SPR_PERF_PCMR_STEP     U(0x001)
#define SPR_PERF_PCMR_INDEX(N) (SPR_PERF_PCMR_BASE + \
    (SPR_PERF_PCMR_STEP * (N)))
#define SPR_PERF_PCMR_ADDR(N)  ((SPR_PERF_GROUP << SPR_GROUP_LSB) | \
    SPR_PERF_PCMR_INDEX(N))

/**************************/
/* Power Management Group */
/**************************/
#define SPR_POWER_GROUP     0x08

/* Power Management Register */
#define SPR_POWER_PMR_INDEX U(0x000)
#define SPR_POWER_PMR_ADDR  U(0x4000)

/*******************************************/
/* Programmable Interrupt Controller Group */
/*******************************************/
#define SPR_PIC_GROUP       0x09

/* PIC Mask Register */
#define SPR_PIC_PICMR_INDEX U(0x000)
#define SPR_PIC_PICMR_ADDR  U(0x4800)

/* PIC Status Register */
#define SPR_PIC_PICSR_INDEX U(0x002)
#define SPR_PIC_PICSR_ADDR  U(0x4802)

/********************/
/* Tick Timer Group */
/********************/
#define SPR_TICK_GROUP               0x0a

/* Tick Timer Mode Register */
#define SPR_TICK_TTMR_INDEX          U(0x000)
#define SPR_TICK_TTMR_ADDR           U(0x5000)

/* Time Period */
#define SPR_TICK_TTMR_TP_LSB         0
#define SPR_TICK_TTMR_TP_MSB         27
#define SPR_TICK_TTMR_TP_BITS        28
#define SPR_TICK_TTMR_TP_MASK        U(0x0fffffff)
#define SPR_TICK_TTMR_TP_GET(x)      (((x) >> 0) & U(0x0fffffff))
#define SPR_TICK_TTMR_TP_SET(x, y)   (((x) & U(0xf0000000)) | \
    ((y) << 0))

/* Interrupt Pending */
#define SPR_TICK_TTMR_IP_OFFSET      28
#define SPR_TICK_TTMR_IP_MASK        0x10000000
#define SPR_TICK_TTMR_IP_GET(x)      (((x) >> 28) & 0x1)
#define SPR_TICK_TTMR_IP_SET(x, y)   (((x) & U(0xefffffff)) | \
    ((!!(y)) << 28))

/* Interrupt Enable */
#define SPR_TICK_TTMR_IE_OFFSET      29
#define SPR_TICK_TTMR_IE_MASK        0x20000000
#define SPR_TICK_TTMR_IE_GET(x)      (((x) >> 29) & 0x1)
#define SPR_TICK_TTMR_IE_SET(x, y)   (((x) & U(0xdfffffff)) | \
    ((!!(y)) << 29))

/* Mode */
#define SPR_TICK_TTMR_MODE_LSB       30
#define SPR_TICK_TTMR_MODE_MSB       31
#define SPR_TICK_TTMR_MODE_BITS      2
#define SPR_TICK_TTMR_MODE_MASK      U(0xc0000000)
#define SPR_TICK_TTMR_MODE_GET(x)    (((x) >> 30) & U(0x00000003))
#define SPR_TICK_TTMR_MODE_SET(x, y) (((x) & U(0x3fffffff)) | \
    ((y) << 30))

/* Disabled */
#define SPR_TICK_TTMR_MODE_DISABLE   0
/* Restart counting when TTMR[TP]==TTCR */
#define SPR_TICK_TTMR_MODE_RESTART   1
/* Stop counting when TTMR[TP]==TTCR */
#define SPR_TICK_TTMR_MODE_STOP      2
/* Continue counting when TTMR[TP]==TTCR */
#define SPR_TICK_TTMR_MODE_CONTINUE  3

/* Tick Timer Count Register */
#define SPR_TICK_TTCR_INDEX          U(0x001)
#define SPR_TICK_TTCR_ADDR           U(0x5001)

/*****************************/
/* Floating Point Unit Group */
/*****************************/
#define SPR_FPU_GROUP 0x0b

#endif /* COMMON_ARCH_SPR_H */
