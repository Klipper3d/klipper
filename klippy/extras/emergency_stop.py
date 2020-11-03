# Host independent emergency stop button
#
# Copyright (C) 2020  Lucas Felix <lucas.felix0738@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class EmergencyStop:
    def __init__(self, config):
        printer = config.get_printer()
        ppins = printer.lookup_object('pins')
        pin_params = ppins.lookup_pin(config.get('pin'), True, True)
        mcu = pin_params['chip']
        oid = mcu.create_oid()
        mcu.add_config_cmd(
            "config_emergency oid=%d pin=%s pull_up=%d invert=%d" % (
            oid, pin_params['pin'], pin_params['pullup'], pin_params['invert']))
        mcu.register_response(self._handle_reset, 'emergency_reset')
        self.gcode = printer.lookup_object('gcode')

    def _handle_reset(self, params):
        self.gcode.request_restart('firmware_restart')

def load_config(config):
    return EmergencyStop(config)
