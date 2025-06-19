# Z Thermal Adjust
#
# Copyright (C) 2022  Robert Pazdzior <robertp@norbital.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

# Adjusts Z position in real-time using a thermal probe to e.g. compensate
# for thermal expansion of the printer frame.

import threading

KELVIN_TO_CELSIUS = -273.15

class ZThermalAdjuster:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.lock = threading.Lock()

        # Get config parameters, convert to SI units where necessary
        self.temp_coeff = config.getfloat('temp_coeff', minval=-1, maxval=1,
            default=0)
        self.off_above_z = config.getfloat('z_adjust_off_above', 99999999.)
        self.max_z_adjust_mm = config.getfloat('max_z_adjustment', 99999999.)

        # Register printer events
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.printer.register_event_handler("homing:home_rails_end",
                                            self.handle_homing_move_end)

        # Setup temperature sensor
        self.smooth_time = config.getfloat('smooth_time', 2., above=0.)
        self.inv_smooth_time = 1. / self.smooth_time
        self.min_temp = config.getfloat('min_temp', minval=KELVIN_TO_CELSIUS)
        self.max_temp = config.getfloat('max_temp', above=self.min_temp)
        pheaters = self.printer.load_object(config, 'heaters')
        self.sensor = pheaters.setup_sensor(config)
        self.sensor.setup_minmax(self.min_temp, self.max_temp)
        self.sensor.setup_callback(self.temperature_callback)
        pheaters.register_sensor(config, self)

        self.last_temp = 0.
        self.measured_min = self.measured_max = 0.
        self.smoothed_temp = 0.
        self.last_temp_time = 0.
        self.ref_temperature = 0.
        self.ref_temp_override = False

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

    def handle_connect(self):
        'Called after all printer objects are instantiated'
        self.toolhead = self.printer.lookup_object('toolhead')
        gcode_move = self.printer.lookup_object('gcode_move')

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
            'measured_min_temp': round(self.measured_min, 2),
            'measured_max_temp': round(self.measured_max, 2),
            'current_z_adjust': self.z_adjust_mm,
            'z_adjust_ref_temperature': self.ref_temperature,
            'enabled': self.adjust_enable
        }

    def handle_homing_move_end(self, homing_state, rails):
        'Set reference temperature after Z homing.'
        if 2 in homing_state.get_axes():
            self.ref_temperature = self.smoothed_temp
            self.ref_temp_override = False
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
        return [pos[0], pos[1], new_z] + pos[3:]

    def calc_unadjust(self, pos):
        'Remove Z adjustment'
        unadjusted_z = pos[2] - self.z_adjust_mm
        return [pos[0], pos[1], unadjusted_z] + pos[3:]

    def get_position(self):
        position = self.calc_unadjust(self.next_transform.get_position())
        self.last_position = self.calc_adjust(position)
        return position

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

    def temperature_callback(self, read_time, temp):
        'Called everytime the Z adjust thermistor is read'
        with self.lock:
            time_diff = read_time - self.last_temp_time
            self.last_temp = temp
            self.last_temp_time = read_time
            temp_diff = temp - self.smoothed_temp
            adj_time = min(time_diff * self.inv_smooth_time, 1.)
            self.smoothed_temp += temp_diff * adj_time
            self.measured_min = min(self.measured_min, self.smoothed_temp)
            self.measured_max = max(self.measured_max, self.smoothed_temp)

    def get_temp(self, eventtime):
        return self.smoothed_temp, 0.

    def stats(self, eventtime):
        return False, '%s: temp=%.1f' % ("z_thermal_adjust", self.smoothed_temp)

    def cmd_SET_Z_THERMAL_ADJUST(self, gcmd):
        enable = gcmd.get_int('ENABLE', None, minval=0, maxval=1)
        coeff = gcmd.get_float('TEMP_COEFF', None, minval=-1, maxval=1)
        ref_temp = gcmd.get_float('REF_TEMP', None, minval=KELVIN_TO_CELSIUS)

        if ref_temp is not None:
            self.ref_temperature = ref_temp
            self.ref_temp_override = True
        if coeff is not None:
            self.temp_coeff = coeff
        if enable is not None:
            if enable != self.adjust_enable:
                self.adjust_enable = True if enable else False
                gcode_move = self.printer.lookup_object('gcode_move')
                gcode_move.reset_last_position()

        state = '1 (enabled)' if self.adjust_enable else '0 (disabled)'
        override = ' (manual)' if self.ref_temp_override else ''
        msg = ("enable: %s\n"
               "temp_coeff: %f mm/degC\n"
               "ref_temp: %.2f degC%s\n"
               "-------------------\n"
               "Current Z temp: %.2f degC\n"
               "Applied Z adjustment: %.4f mm"
               % (state,
                  self.temp_coeff,
                  self.ref_temperature, override,
                  self.smoothed_temp,
                  self.z_adjust_mm)
        )
        gcmd.respond_info(msg)

    cmd_SET_Z_THERMAL_ADJUST_help = 'Set/query Z Thermal Adjust parameters.'

def load_config(config):
    return ZThermalAdjuster(config)
