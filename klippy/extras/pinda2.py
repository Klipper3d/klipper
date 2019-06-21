# P.I.N.D.A. 2 support
#
# Copyright (C) 2019  Rafal Jastrzebski <r2@jastrzebscy.org>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

# configuration example:
#[pinda2]
#pin: PB4
#x_offset: 24
#y_offset: 5
#z_offset: 0.25
#speed: 10.0
#sensor_pin: PF3
#sensor_type: EPCOS 100K B57560G104F
## min and max temp define safe operating range, like other thermistors
#min_temp:5
#max_temp:60
## Autoadjust correction after home 
## if you want to do that manually add PROBE_ADJUST in start code
#auto_adjust: True
#z_offset_calibration:
#    0, 0.00
#    30, -0.00
#    35, -0.05
#    40, -0.15
#    45, -0.4
#    50, -0.8
#    55, -1.7
#    60, -2.6
#    100, -2.00

#additionl command:
#PROBE_TEMP
# gives temperature of P.I.N.D.A. sensor

#PROBE_ADJUST
# moves Z down a little if probe is hot
# use when auto_adjust=False

# You have to use
# homing_retract_dist: 5
# for stepper_z

import pins, homing, manual_probe, adc_temperature, probe

class Pinda2Probe (probe.PrinterProbe):
    def __init__(self, config, mcu_probe):
        probe.PrinterProbe.__init__(self, config, mcu_probe)
        self.gcode.register_command('PROBE_TEMP', self.cmd_PROBE_TEMP,
                                    desc=self.cmd_PROBE_TEMP_help)

        self.gcode.register_command('PROBE_ADJUST', self.cmd_PROBE_ADJUST,
                                desc=self.cmd_PROBE_ADJUST_help)

    cmd_PROBE_TEMP_help = "Return the temperature of P.I.N.D.A 2 sensor"
    def cmd_PROBE_TEMP(self, params):
        temp = self.mcu_probe.last_temp
        self.gcode.respond_info( "pinda2_temp: %s" % temp)

    cmd_PROBE_ADJUST_help = "Adjust P.I.N.D.A 2 offset"
    def cmd_PROBE_ADJUST(self, params):
        self.mcu_probe.apply_corection()
        self.gcode.respond_info( "pinda2_temp: %s , temp correction: %s" 
                                % (self.mcu_probe.last_temp, self.mcu_probe.last_e) )

    def _probe(self, speed):
        self.mcu_probe.in_probe = True
        probe.PrinterProbe._probe(self, speed)
        self.mcu_probe.in_probe = False

    def getProbePointsHelper(self, config, finalize_callback, default_points=None) :
        return ProbePointsHelper(config, finalize_callback, default_points)

class Pinda2EndstopWrapper(probe.ProbeEndstopWrapper):
    def __init__(self, config):
        probe.ProbeEndstopWrapper.__init__(self, config)
        self.init_sensor(config)
        self.in_probe = False
        self.home_e = 0

    def init_sensor(self, config):
        self.speed = config.getfloat('speed', 5.0)
        self.auto_adjust = config.getboolean('auto_adjust', True)
        self.sensor = self.printer.lookup_object('heater').setup_sensor(config)
        self.sensor.setup_callback(self.temperature_callback)
        self.printer.lookup_object('heater').register_sensor(config, self)

        s = config.get('z_offset_calibration')
        z_offset_calibration_samples = s.split('\n')
        try:
            offset_map = [line.split(',', 1)
                for line in z_offset_calibration_samples if line.strip()]
            self.z_offset_calibration_samples = []
            for zp in offset_map :
                self.z_offset_calibration_samples += [(float(zp[0].strip()), 
                                                       float(zp[1].strip()))]
        except:
            raise config.error(
                "Unable to parse date %s, %s. A newline separated list of "
                "temperature, correction" % (
                    config.get_name(), s))
        if len(self.z_offset_calibration_samples) < 2:
            raise config.error(
                "Calibration map requires at least two lines")
        try:
            li = adc_temperature.LinearInterpolate(
                self.z_offset_calibration_samples)
        except ValueError as e:
            raise config.error("Error in z_offset_calibration %s, %s" % (
                str(e), config.get_name()))
        self.z_offset_calibration_li = li

    def temperature_callback(self, read_time, temp):
        self.last_temp = temp

    def get_temp(self, eventtime):
        return self.last_temp, self.target_temp

    def get_e(self):
        temp = self.last_temp
        self.last_e = self.z_offset_calibration_li.interpolate(temp)
        return self.last_e

    def apply_corection(self):
        toolhead = self.printer.lookup_object('toolhead')
        pos = toolhead.get_position()
        pos_z = pos[2]
        self.home_e = self.get_e()
        new_z = pos_z + self.home_e
        if new_z != pos_z :
            pos[2] = new_z
            try:
                toolhead.move(pos, self.speed)
                pos[2] = pos_z
                toolhead.set_position(pos)
            except homing.EndstopError as e:
                self._finalize(False)
                raise self.gcode.error(str(e))


    def home_finalize(self):
        try:
            self.deactivate_gcode.run_gcode_from_command()
            if self.auto_adjust and not self.in_probe:
                self.apply_corection()

        except self.gcode.error as e:
            raise homing.EndstopError(str(e))
        self.mcu_endstop.home_finalize()
    def get_position_endstop(self):
        return self.position_endstop

class ProbePointsHelper(probe.ProbePointsHelper):
    def __init__(self, config, finalize_callback, default_points=None):
        probe.ProbePointsHelper.__init__(self, config, 
                                         finalize_callback, default_points)
        self.probe = self.printer.lookup_object('probe')

    def _automatic_probe_point(self):
        probe.ProbePointsHelper._automatic_probe_point(self)
        r = self.results[-1]
        e = self.probe.mcu_probe.get_e() - self.probe.mcu_probe.home_e
        r[2] = r[2] + e
        self.results[-1] = r

def load_config(config):
    p2 = Pinda2Probe(config, Pinda2EndstopWrapper(config))
    config.get_printer().add_object('probe', p2)
    return p2
