# Change Z Homing behavior to use probing instead of the typical G28 homing.
#
# Copyright (C) 2025 Nick Weedon <nick@weedon.org.au>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import copy

class ProbeAsZHome:
    def __init__(self, config):
        logging.debug("Probe As Home: Initializing")
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        if self.gcode.register_command("G28", None) is not None:
            raise config.error("probe_as_z_home must be defined before \
                               homing_override or safe_z_homing")
        self.printer.load_object(config, 'homing')
        self.prev_G28 = self.gcode.register_command("G28", None)
        self.gcode.register_command("G28", self.cmd_G28)

    def cmd_G28(self, gcmd):
        logging.debug("Probe As Home: Performing G28")

        # We need to pass on the parameters to the original G28 command
        # we are 'decorating' but omit the Z axis as this tool will handle it
        new_params = copy.deepcopy(gcmd.get_command_parameters())
        # If neither X,Y or Z are specified, then this implies
        # that all axes need to be homed.
        if all(new_params.get(axis, None) is None for axis in "XYZ"):
            new_params['X'] = '0'
            new_params['Y'] = '0'

        original_z_param = new_params.pop('Z', None)

        # Perform XY homing if necessary using the original G28 command
        if any(new_params.get(axis, None) is not None for axis in "XY"):
            g28_gcmd = self.gcode.create_gcode_command("G28", "G28", new_params)
            self.prev_G28(g28_gcmd)

        # Perform Z homing using the probe
        if original_z_param is not None:
            logging.debug("Beggining probe based Z homing")
            new_params['Z'] = original_z_param
            new_params.pop('X', None)
            new_params.pop('Y', None)
            g28_gcmd = self.gcode.create_gcode_command("G28", "G28", new_params)
            self.prev_G28(g28_gcmd)

            # Probe Z
            probe = self.printer.lookup_object('probe', None)

            # Retract before probing since the homing procedure can position
            # the toolhead too close to the bed such that BL-Touch based
            # probes cannot deploy.
            self.toolhead = self.printer.lookup_object('toolhead')
            current_pos = self.toolhead.get_position()

            lift_speed = probe.get_probe_params()['lift_speed']
            retract_dist = probe.get_probe_params()['sample_retract_dist']

            current_pos[2] += retract_dist
            self.toolhead.move(current_pos, lift_speed)

            # Use the probe options from the configuration here
            g28_gcmd = self.gcode.create_gcode_command("PROBE", "PROBE", {})
            probe_session = probe.start_probe_session(gcmd)
            probe_session.run_probe(gcmd)
            probed_pos = probe_session.pull_probed_results()[0]
            current_pos = self.toolhead.get_position()
            z_error_delta = probed_pos[2] - current_pos[2]
            logging.debug("Result of probe: " + repr(probed_pos))
            logging.debug("Toolhead position: " + repr(current_pos))
            gcmd.respond_info("Correcting homed Z by: " + str(z_error_delta))

            # Set the new position based on the probe result
            new_position = current_pos.copy()
            new_position[2] = probed_pos[2]
            self.toolhead.set_position(new_position)

            # End the probe session only after setting the new position
            # in case 'end_probe_session()' moves the toolhead.
            probe_session.end_probe_session()

    def get_status(self, eventtime):
        return {'probe_as_home': True}


def load_config(config):
    return ProbeAsZHome(config)
