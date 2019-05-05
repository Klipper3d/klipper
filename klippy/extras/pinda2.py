# P.I.N.D.A. 2 support
# configuration example:
#[pinda2]
#pin: PB4
#x_offset: 24
#y_offset: 5
#z_offset: 0.25
#speed: 10.0
#sensor_pin: PF3
#sensor_type: EPCOS 100K B57560G104F
#min_temp:0
#max_temp:100
#z_offset_calibration:[(0, 0.00),(40, 0.00),(50, -0.25), (100, -0.50)]

#additionl command:
#PROBE_TEMP
# gives temperature of P.I.N.D.A. sensor
import pins, homing, manual_probe, adc_temperature, probe

class Pinda2Probe (probe.PrinterProbe):
    def __init__(self, config, mcu_probe):
        probe.PrinterProbe.__init__(self, config, mcu_probe)
        self.gcode.register_command('PROBE_TEMP', self.cmd_PROBE_TEMP,
                                    desc=self.cmd_PROBE_TEMP_help)

    cmd_PROBE_TEMP_help = "Return the temperature of P.I.N.D.A 2 sensor"
    def cmd_PROBE_TEMP(self, params):
        temp = self.mcu_probe.last_temp
        self.gcode.respond_info( "pinda2_temp: %s" % temp)

class Pinda2EndstopWrapper(probe.ProbeEndstopWrapper):
    def __init__(self, config):
        probe.ProbeEndstopWrapper.__init__(self, config)
        self.init_sensor(config)

    def init_sensor(self, config):
        self.speed = config.getfloat('speed', 5.0)
        self.min_temp = config.getfloat('min_temp', minval=-273)
        self.max_temp = config.getfloat('max_temp', above=self.min_temp)
        self.sensor = self.printer.lookup_object('heater').setup_sensor(config)
        self.sensor.setup_minmax(self.min_temp, self.max_temp)
        self.sensor.setup_callback(self.temperature_callback)
        self.printer.lookup_object('heater').register_sensor(config, self)

        s = config.get('z_offset_calibration')

        try:
            self.z_offset_calibration_samples= eval(s)
        except SyntaxError:
            raise config.error(
                "Syntax error in z_offset_calibration. Should be [(t1, e1), (t"
                "2, e2),...(tn, en)] where t is temperature, e is offset corre"
                "ction. There is z_offset_calibration: " + s)
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

    def correct_pos(self, pos_z):
        temp = self.last_temp
        e = self.z_offset_calibration_li.interpolate(temp)
        return pos_z + e

    def apply_corection(self):
        toolhead = self.printer.lookup_object('toolhead')
        pos = toolhead.get_position()
        pos_z = pos[2]
        pos[2] = self.correct_pos(pos_z)
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
            self.apply_corection()
        except self.gcode.error as e:
            raise homing.EndstopError(str(e))
        self.mcu_endstop.home_finalize()

def load_config(config):
    p2 = Pinda2Probe(config, Pinda2EndstopWrapper(config))
    config.get_printer().add_object('probe', p2)
    return p2
