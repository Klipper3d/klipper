// SAM4 cache enable
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "sam4e.h" // CMCC
#include "sched.h" // DECL_INIT

void
sam4_cache_init(void)
{
    if (!(CMCC->CMCC_SR & CMCC_SR_CSTS))
        CMCC->CMCC_CTRL = CMCC_CTRL_CEN;
}
DECL_INIT(sam4_cache_init);
