# Config for Recore,
#
# Copyright (C) 2017-2019  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2019-2023  Elias Bakken <elias@iagent.no>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, os
import pins, mcu

pins = {
    "A7": {
        'enable_pin': 'ar100:PF5',
        'oc_reset_pin': 'ar100:PF4',
        'gain_enable_t0': 'ar100:PD4',
        'gain_enable_t1': 'ar100:PH11',
        'gain_enable_t2': 'ar100:PE17',
        'gain_enable_t3': 'ar100:PB2',
        'pullup_t0': 'ar100:PG10',
        'pullup_t1': 'ar100:PG11',
        'pullup_t2': 'ar100:PG12',
        'pullup_t3': 'ar100:PG13',
        'offset_t0': 'ar100:PG0',
        'offset_t1': 'ar100:PG1',
        'offset_t2': 'ar100:PG2',
        'offset_t3': 'ar100:PG3'
    }
}


class recore:
    def __init__(self, config):
        printer = config.get_printer()
        ppins = printer.lookup_object('pins')
        ppins.register_chip('recore', self)
        revisions = {'A6':'A6', 'A7':'A7'}
        self.revision = config.getchoice('revision', revisions)

        pins["A6"] = pins["A7"]
        # Setup enable pin
        enable_pin = config.get('enable_pin',
                                pins[self.revision]['enable_pin'])
        mcu_power_enable = ppins.setup_pin('digital_out', enable_pin)
        mcu_power_enable.setup_start_value(start_value=0.,
                                           shutdown_value=1.,
                                           is_static=False)
        mcu_power_enable.setup_max_duration(0.)

        # Reset over current alarm
        oc_reset_pin = config.get('oc_reset_pin',
                                  pins[self.revision]['oc_reset_pin'])
        oc_reset = ppins.setup_pin('digital_out', oc_reset_pin)
        mcu = oc_reset.get_mcu()
        pin = oc_reset._pin
        mcu.add_config_cmd("set_digital_out pin=%s value=%d" % (pin, 0), True)
        mcu.add_config_cmd("set_digital_out pin=%s value=%d" % (pin, 1), True)

        for idx in range(4):
            gain = config.get('gain_t' + str(idx), '1')
            if gain not in ['1', '100']:
                raise Exception("Gain not 1 or 100")
            pin_name = pins[self.revision]['gain_enable_t' + str(idx)]
            if gain == '1':
                # Set pin to input
                pin = ppins.setup_pin('endstop', pin_name)
            else:
                pin = ppins.setup_pin('digital_out', pin_name)
                value = 1.0
                pin.setup_start_value(start_value=value,
                                      shutdown_value=value,
                                      is_static=True)

            pullup = config.get('pullup_t' + str(idx), '1')
            if pullup not in ['0', '1']:
                raise Exception("Pullup not 0 or 1")
            pin_name = pins[self.revision]['pullup_t' + str(idx)]
            if pullup == '0':
                pin = ppins.setup_pin('endstop', pin_name)
            else:
                pin = ppins.setup_pin('digital_out', pin_name)
                pin.setup_start_value(start_value=1.,
                                      shutdown_value=1.,
                                      is_static=True)
            offset = config.get('offset_t' + str(idx), '1')
            if offset not in ['0', '1']:
                raise Exception("Offset not 0 or 1")
            pin_name = pins[self.revision]['offset_t' + str(idx)]
            if offset == '0':
                pin = ppins.setup_pin('endstop', pin_name)
            else:
                pin = ppins.setup_pin('digital_out', pin_name)
                pin.setup_start_value(start_value=1.,
                                      shutdown_value=1.,
                                      is_static=True)


def load_config(config):
    return recore(config)
