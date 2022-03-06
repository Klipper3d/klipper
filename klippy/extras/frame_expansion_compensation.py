# Frame Expansion Compensation
#
# Copyright (C) 2022  Robert Pazdzior <robertp@norbital.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

# Compensates for thermal expansion induced toolhead Z movement in real-time
# using a frame-coupled temperature probe.
import threading

class FrameExpansionCompensator:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.lock = threading.Lock()
        self.config = config

        # Get config parameters, convert to SI units where necessary
        self.temp_coeff = config.getfloat('temp_coeff', minval=-1, maxval=1,
            default=0)
        self.temp_sensor_name = config.get('temp_sensor')
        self.smooth_time = config.getfloat('smooth_time', 2., above=0.)
        self.inv_smooth_time = 1. / self.smooth_time

        self.max_comp_z = config.getfloat('max_comp_z', 0.)
        self.max_offset = config.getfloat('max_z_offset', 99999999.)

        # Register printer events
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.printer.register_event_handler("homing:home_rails_end",
                                            self.handle_homing_move_end)

        # Setup temperature sensor
        self.last_temp = self.smoothed_temp = 0.
        self.last_temp_time = 0.
        self.last_home_temp = 0.

        # Z offset transformation
        self.z_drift_offset = 0.
        self.last_z_drift_offset = 0.
        self.comp_enable = True
        self.last_position = [0., 0., 0., 0.]
        self.next_transform = None
        self.comp_state = 'not homed'

        # Register gcode commands
        self.gcode.register_command('SET_FRAME_COMP',
                                    self.cmd_SET_FRAME_COMP,
                                    desc=self.cmd_SET_FRAME_COMP_help)
        self.gcode.register_command('QUERY_FRAME_COMP',
                                    self.cmd_QUERY_FRAME_COMP,
                                    desc=self.cmd_QUERY_FRAME_COMP_help)
        self.gcode.register_command('SET_FRAME_REF_TEMP',
                                    self.cmd_SET_FRAME_REF_TEMP,
                                    desc=self.cmd_SET_FRAME_REF_TEMP_help)

    def handle_connect(self):
        'Called after all printer objects are instantiated'
        self.toolhead = self.printer.lookup_object('toolhead')
        gcode_move = self.printer.lookup_object('gcode_move')

        # Temperature sensor config check and callback registration
        try:
            self.sensor = self.printer.lookup_object(self.temp_sensor_name)
        except Exception as e:
            msg = '''
            %s\nUse the full config section name in the "temp_sensor" parameter,
            e.g. "temp_sensor: temperature_sensor frame".
            ''' % e
            raise self.printer.config_error(msg)
        else:
            self.sensor.sensor.setup_callback(self.comp_temperature_callback)

        # Register move transformation
        self.next_transform = gcode_move.set_move_transform(self, force=True)

        # Pull Z step distance for minimum adjustment increment
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        steppers = [s.get_name() for s in kin.get_steppers()]
        z_stepper = kin.get_steppers()[steppers.index("stepper_z")]
        self.z_step_dist = z_stepper.get_step_dist()

    def get_status(self, eventtime):
        return {
            'temperature': self.smoothed_temp,
            'current_z_comp': self.z_drift_offset,
            'frame_ref_temp': self.last_home_temp,
            'state': self.comp_state
        }

    def handle_homing_move_end(self, homing_state, rails):
        'Triggered when Z axis is homed.'
        if 2 in homing_state.get_axes():
            self.last_home_temp = self.smoothed_temp
            self.z_drift_offset = 0.

    def calc_offset(self, pos):
        'Calculate total linear thermal expansion relative to last homing.'
        if not self.max_comp_z or pos[2] < self.max_comp_z:
            delta_t = self.smoothed_temp - self.last_home_temp

            # Calculate Z offset, compensates for thermal expansion since homing
            offset = -1 * self.temp_coeff * delta_t

            # compute sign (+1 or -1) for maximum offset setting
            sign = 1 - (offset <= 0)*2

            # Don't apply offsets smaller than step distance
            if abs(offset - self.z_drift_offset) > self.z_step_dist:
                self.z_drift_offset = min([self.max_offset*sign,
                    offset], key=abs)

        # Apply offset
        new_z = pos[2] + self.z_drift_offset
        self.last_z_drift_offset = self.z_drift_offset
        return [pos[0], pos[1], new_z, pos[3]]

    def calc_unoffset(self, pos):
        'Remove Z-drift offset'
        unoffset_z = pos[2] - self.z_drift_offset
        return [pos[0], pos[1], unoffset_z, pos[3]]

    def get_position(self):
        return self.calc_unoffset(self.next_transform.get_position())

    def move(self, newpos, speed):
        # don't apply to extrude only moves or when disabled
        if (newpos[0:2] == self.last_position[0:2]) or not self.comp_enable:
            z = newpos[2] + self.last_z_drift_offset
            corrected_pos = [newpos[0], newpos[1], z, newpos[3]]
            self.next_transform.move(corrected_pos, speed)
        else:
            corrected_pos = self.calc_offset(newpos)
            self.next_transform.move(corrected_pos, speed)
        self.last_position[:] = newpos

    def comp_temperature_callback(self, read_time, temp):
        'Called everytime the thermistor is read'
        self.sensor.temperature_callback(read_time, temp)
        with self.lock:
            time_diff = read_time - self.last_temp_time
            self.last_temp = temp
            self.last_temp_time = read_time
            temp_diff = temp - self.smoothed_temp
            adj_time = min(time_diff * self.inv_smooth_time, 1.)
            self.smoothed_temp += temp_diff * adj_time

    def cmd_SET_FRAME_COMP(self, gcmd):
        new_state = gcmd.get_int('ENABLE', 1, minval=0, maxval=1)
        self.temp_coeff = gcmd.get_float('COEFF', self.temp_coeff,
                                         minval=-1, maxval=1)
        state = 'ENABLED' if new_state else 'DISABLED'
        msg = ("state: %s\n"
               "temp_coeff: %f mm/K"
               % (state, self.temp_coeff)
        )
        gcmd.respond_info(msg)

        if new_state != self.comp_enable:
            gcode_move = self.printer.lookup_object('gcode_move')
            gcode_move.reset_last_position()
            self.comp_enable = new_state

    def cmd_QUERY_FRAME_COMP(self, gcmd):
        state = 'ENABLED' if self.comp_state else 'DISABLED'
        msg = ("state: %s\n"
               "current temperature: %.2f degC\n"
               "reference temperature: %.2f degC\n"
               "applied Z adjustment: %.4f mm" % (state,
                                            self.smoothed_temp,
                                            self.last_home_temp,
                                            self.z_drift_offset)
        )
        gcmd.respond_info(msg)

    def cmd_SET_FRAME_REF_TEMP(self, gcmd):
        ref_temp = gcmd.get_float('TEMPERATURE', -273.16, minval=-273.15)
        if ref_temp > -273.16:
            self.last_home_temp = ref_temp
        else:
            self.last_home_temp = self.smoothed_temp
        msg = ("Frame expansion compensation reference temperature set"
               " manually to %.3fc" % self.last_home_temp)
        gcmd.respond_info(msg)

    cmd_SET_FRAME_COMP_help = 'Enable/disable Z thermal expansion compensation.'
    cmd_QUERY_FRAME_COMP_help = 'Report current compensation parameters.'
    cmd_SET_FRAME_REF_TEMP_help = 'Use current frame temp. as comp. reference.'

def load_config(config):
    return FrameExpansionCompensator(config)
