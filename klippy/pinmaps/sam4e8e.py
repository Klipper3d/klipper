# SAM4E8E pinmap
#
# Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

def update_pinmap(pins, mcu_type):
    if mcu_type != 'sam4e8e':
        raise error("Incompatible mcu_type for sam4e8e pinmap: %s" % (mcu_type))
    for port in range(5):
        portchr = chr(65 + port)
        if portchr == 'I':
            continue
        for portbit in range(32):
            pins['P%c%d' % (portchr, portbit)] = port * 32 + portbit
    return pins