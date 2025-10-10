/**
 * Copyright (c) 2024 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Support old header for compatibility (and if included, support old variable name)
#include "hardware/structs/xip.h"
#define XIP_STAT_FIFO_FULL XIP_STAT_FIFO_FULL_BITS
#define XIP_STAT_FIFO_EMPTY XIP_STAT_FIFO_EMPTY_BITS
#define XIP_STAT_FLUSH_RDY XIP_STAT_FLUSH_READY_BITS
