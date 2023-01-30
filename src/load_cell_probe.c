// Load cell probing and homing
//
// Copyright (C) 2023 Konstantin Vogel <konstantin.vogel@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // DECL_COMMAND
#include "basecmd.h" // oid_alloc
#include "board/irq.h" // irq_disable
#include "stepper.h"

struct load_cell_probe {
    uint32_t oid;
    int32_t limit;
    int32_t stepper_oid[3];
    int32_t triggering_active;
};

void command_config_load_cell_probe(uint32_t *args)
{
    struct load_cell_probe *lcp = oid_alloc(args[0], command_config_load_cell_probe, sizeof(*lcp));
    lcp->oid = args[0];
    lcp->limit = 0;
    lcp->stepper_oid[0] = args[1];
    lcp->stepper_oid[1] = args[2];
    lcp->stepper_oid[2] = args[3];
    lcp->triggering_active = 0;
}
DECL_COMMAND(command_config_load_cell_probe, "config_load_cell_probe oid=%c stepper1=%i stepper2=%i stepper3=%i");

void check_load_cell_probe(uint32_t oid, int32_t value, uint32_t time)
{
    struct load_cell_probe *lcp = oid_lookup(oid, command_config_load_cell_probe);
    if (value > lcp->limit && lcp->triggering_active)
    {   
        irq_disable();
        lcp->triggering_active = 0;
        for (int i=0; i<3; i++){
            if (lcp->stepper_oid[i] >= 0)
                stepper_manual_stop(lcp->stepper_oid[i]);
        }
        irq_enable();
        sendf("load_cell_probe_triggered oid=%u clock=%u", oid, time);
    } 
}

void command_enable_load_cell_trigger(uint32_t *args)
{
    struct load_cell_probe *lcp = oid_lookup(args[0], command_config_load_cell_probe);
    lcp->triggering_active = args[1];
    lcp->limit = args[2];
}
DECL_COMMAND(command_enable_load_cell_trigger, "enable_load_cell_trigger oid=%c enable=%i limit=%i");
