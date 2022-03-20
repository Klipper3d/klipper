# Z Thermal Adjust
#
# Copyright (C) 2022  Robert Pazdzior <robertp@norbital.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

# Adjusts Z position in real-time using a thermal probe to e.g. compensate
# for thermal expansion of the printer frame.

import threading

class ZThermalAdjuster:
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

        self.off_above_z = config.getfloat('z_adjust_off_above', 99999999.)
        self.max_z_adjust_mm = config.getfloat('max_z_adjustment', 99999999.)

        # Register printer events
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.printer.register_event_handler("homing:home_rails_end",
                                            self.handle_homing_move_end)

        # Setup temperature sensor
        self.last_temp = self.smoothed_temp = 0.
        self.last_temp_time = 0.
        self.ref_temperature = 0.

        # Z transformation
        self.z_adjust_mm = 0.
        self.last_z_adjust_mm = 0.
        self.adjust_enable = True
        self.last_position = [0., 0., 0., 0.]
        self.next_transform = None

        # Register gcode commands
        self.gcode.register_command('SET_Z_THERMAL_ADJUST',
                                    self.cmd_SET_Z_THERMAL_ADJUST,
                                    desc=self.cmd_SET_Z_THERMAL_ADJUST_help)
        self.gcode.register_command('QUERY_Z_THERMAL_ADJUST',
                                    self.cmd_QUERY_Z_THERMAL_ADJUST,
                                    desc=self.cmd_QUERY_Z_THERMAL_ADJUST_help)
        self.gcode.register_command('SET_Z_THERMAL_ADJUST_REF',
                                    self.cmd_SET_Z_THERMAL_ADJUST_REF,
                                    desc=self.cmd_SET_Z_THERMAL_ADJUST_REF_help)

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
            self.sensor.sensor.setup_callback(self.z_adj_temperature_callback)

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
            'current_z_adjust': self.z_adjust_mm,
            'z_adjust_ref_temperature': self.ref_temperature,
            'enabled': self.adjust_enable
        }

    def handle_homing_move_end(self, homing_state, rails):
        'Set reference temperature after Z homing.'
        if 2 in homing_state.get_axes():
            self.ref_temperature = self.smoothed_temp
            self.z_adjust_mm = 0.

    def calc_adjust(self, pos):
        'Z adjustment calculation'
        if pos[2] < self.off_above_z:
            delta_t = self.smoothed_temp - self.ref_temperature

            # Calculate Z adjustment
            adjust = -1 * self.temp_coeff * delta_t

            # compute sign (+1 or -1) for maximum offset setting
            sign = 1 - (adjust <= 0)*2

            # Don't apply adjustments smaller than step distance
            if abs(adjust - self.z_adjust_mm) > self.z_step_dist:
                self.z_adjust_mm = min([self.max_z_adjust_mm*sign,
                    adjust], key=abs)

        # Apply Z adjustment
        new_z = pos[2] + self.z_adjust_mm
        self.last_z_adjust_mm = self.z_adjust_mm
        return [pos[0], pos[1], new_z, pos[3]]

    def calc_unadjust(self, pos):
        'Remove Z adjustment'
        unadjusted_z = pos[2] - self.z_adjust_mm
        return [pos[0], pos[1], unadjusted_z, pos[3]]

    def get_position(self):
        return self.calc_unadjust(self.next_transform.get_position())

    def move(self, newpos, speed):
        # don't apply to extrude only moves or when disabled
        if (newpos[0:2] == self.last_position[0:2]) or not self.adjust_enable:
            z = newpos[2] + self.last_z_adjust_mm
            adjusted_pos = [newpos[0], newpos[1], z, newpos[3]]
            self.next_transform.move(adjusted_pos, speed)
        else:
            adjusted_pos = self.calc_adjust(newpos)
            self.next_transform.move(adjusted_pos, speed)
        self.last_position[:] = newpos

    def z_adj_temperature_callback(self, read_time, temp):
        'Called everytime the thermistor is read, used for smoothing'
        self.sensor.temperature_callback(read_time, temp)
        with self.lock:
            time_diff = read_time - self.last_temp_time
            self.last_temp = temp
            self.last_temp_time = read_time
            temp_diff = temp - self.smoothed_temp
            adj_time = min(time_diff * self.inv_smooth_time, 1.)
            self.smoothed_temp += temp_diff * adj_time

    def cmd_SET_Z_THERMAL_ADJUST(self, gcmd):
        new_state = gcmd.get_int('ENABLE', 1, minval=0, maxval=1)
        self.temp_coeff = gcmd.get_float('COEFF', self.temp_coeff,
                                         minval=-1, maxval=1)
        state = 'ENABLED' if new_state else 'DISABLED'
        msg = ("state: %s\n"
               "temp_coeff: %f mm/K"
               % (state, self.temp_coeff)
        )
        gcmd.respond_info(msg)

        if new_state != self.adjust_enable:
            gcode_move = self.printer.lookup_object('gcode_move')
            gcode_move.reset_last_position()
            self.adjust_enable = new_state

    def cmd_QUERY_Z_THERMAL_ADJUST(self, gcmd):
        state = 'ENABLED' if self.adjust_enable else 'DISABLED'
        msg = ("state: %s\n"
               "current temperature: %.2f degC\n"
               "reference temperature: %.2f degC\n"
               "applied Z adjustment: %.4f mm" % (state,
                                            self.smoothed_temp,
                                            self.ref_temperature,
                                            self.z_adjust_mm)
        )
        gcmd.respond_info(msg)

    def cmd_SET_Z_THERMAL_ADJUST_REF(self, gcmd):
        ref_temp = gcmd.get_float('TEMPERATURE', -273.16, minval=-273.15)
        if ref_temp > -273.16:
            self.ref_temperature = ref_temp
        else:
            self.ref_temperature = self.smoothed_temp
        msg = ("Z thermal adjust reference temperature set"
               " manually to %.3fc" % self.ref_temperature)
        gcmd.respond_info(msg)

    cmd_SET_Z_THERMAL_ADJUST_help = 'Set Z Thermal Adjust parameters.'
    cmd_QUERY_Z_THERMAL_ADJUST_help = ('Report current Z Thermal Adjust'
                                       'parameters.')
    cmd_SET_Z_THERMAL_ADJUST_REF_help = ('Use current probe temperature as Z '
                                         'Thermal Adjust reference '
                                         'temperature')

def load_config(config):
    return ZThermalAdjuster(config)
