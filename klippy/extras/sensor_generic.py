# Support for a generic sensor

KELVIN_TO_CELCIUS = -273.15

class PrinterSensorGeneric:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.sensor = self.printer.lookup_object('heater').setup_sensor(config)
        self.min_temp = config.getfloat('min_temp', minval=KELVIN_TO_CELCIUS)
        self.max_temp = config.getfloat('max_temp', above=self.min_temp)
        self.target_temp = config.getfloat('target_temp', 0., minval=self.min_temp, maxval=self.max_temp)
        self.sensor.setup_minmax(self.min_temp, self.max_temp)
        self.sensor.setup_callback(self.temperature_callback)
        gcode_id = config.get("gcode_id", None)
        if gcode_id is not None:
            self.printer.lookup_object('heater').register_gcode_sensor(self, gcode_id)
        self.last_temp = 0.
    def temperature_callback(self, read_time, temp):
        self.last_temp = temp
    def get_temp(self, eventtime):
         return self.last_temp, self.target_temp

def load_config_prefix(config):
    return PrinterSensorGeneric(config)
