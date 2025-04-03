# Run user defined actions in place of a normal G28 homing command
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class HomingOverride:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.start_pos = [config.getfloat('set_position_' + a, None)
                          for a in 'xyz']
        self.axes = config.get('axes', 'XYZ').upper()
        gcode_macro = self.printer.load_object(config, 'gcode_macro')
        self.template = gcode_macro.load_template(config, 'gcode')
        self.in_script = False
        self.printer.load_object(config, 'homing')
        self.gcode = self.printer.lookup_object('gcode')
        self.prev_G28 = self.gcode.register_command("G28", None)
        self.gcode.register_command("G28", self.cmd_G28)
    def cmd_G28(self, gcmd):
        if self.in_script:
            # Was called recursively - invoke the real G28 command
            self.prev_G28(gcmd)
            return

        # if no axis is given as parameter we assume the override
        axes_to_home = ''
        for axis in 'XYZ':
            if gcmd.get(axis, None) is not None:
                axes_to_home += axis
        if not axes_to_home:
            axes_to_home = 'XYZ'

        if not (set(axes_to_home) & set(self.axes)):
            self.prev_G28(gcmd)
            return

        # Calculate forced position (if configured)
        toolhead = self.printer.lookup_object('toolhead')
        pos = toolhead.get_position()
        homing_axes = ""
        for axis, loc in enumerate(self.start_pos):
            if loc is not None:
                pos[axis] = loc
                homing_axes += "xyz"[axis]
        toolhead.set_position(pos, homing_axes=homing_axes)
        # Perform homing
        context = self.template.create_template_context()
        context['params'] = gcmd.get_command_parameters()
        context['rawparams'] = gcmd.get_raw_command_parameters()
        try:
            self.in_script = True
            self.template.run_gcode_from_command(context)
        finally:
            self.in_script = False
        curtime = self.printer.get_reactor().monotonic()
        homed_axes = toolhead.get_status(curtime)['homed_axes']
        for axis in axes_to_home:
            if axis.lower() not in homed_axes:
                raise gcmd.error(
                        'homing_override did not home requested axis %s', axis)

def load_config(config):
    return HomingOverride(config)
