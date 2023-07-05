# Support for toolchnagers
#
# Copyright (C) 2023 Viesturs Zarins <viesturz@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

# Main printer fan.
# Handles M106 M107 and routes to the active generic fan.
class PrinterFan:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.active_fan = None
        self.requested_speed = None
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("M106", self.cmd_M106)
        gcode.register_command("M107", self.cmd_M107)

    def status(self, eventtime):
        return {'fan': self.active_fan.name if self.active_fan else None,
                'speed': self.requested_speed}

    def get_fan(self):
        return self.activate_fan

    def activate_fan(self, fan):
        # Set new active fan and move the set speed to that fan.
        if self.active_fan == fan:
            return
        if self.active_fan and self.requested_speed is not None:
            self.active_fan.set_speed_from_command(0.)
        self.active_fan = fan
        if self.active_fan and self.requested_speed is not None:
            self.active_fan.set_speed_from_command(self.requested_speed)

    def cmd_M106(self, gcmd):
        # Set fan speed
        self.requested_speed = gcmd.get_float('S', 255., minval=0.) / 255.
        if self.active_fan:
            self.active_fan.set_speed_from_command(self.requested_speed)
    def cmd_M107(self, gcmd):
        # Turn fan off
        self.requested_speed = 0.
        if self.active_fan:
            self.active_fan.set_speed_from_command(self.requested_speed)

def load_config(config):
    return PrinterFan(config)
