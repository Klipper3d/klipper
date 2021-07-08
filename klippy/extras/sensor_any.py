import logging, json
import urllib

HOST_REPORT_TIME = 1.0
VALUE_FILE = "/dev/null"
FACTOR_CONVERT = 1.0
JSON_PATH = ""

class Sensor_ANY:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.name = config.get_name().split()[-1]
        self.path = config.get("sensor_path", VALUE_FILE)
        self.json_path = config.get("json_path", JSON_PATH)
        self.factor = float(config.get("conversion_factor", FACTOR_CONVERT))

        self.temp = self.min_temp = self.max_temp = 0.0

        self.printer.add_object("sensor_any " + self.name, self)

        if self.printer.get_start_args().get('debugoutput') is not None:
            return

        self.sample_timer = self.reactor.register_timer(
            self._sample_value)

        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)

    def handle_connect(self):
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)

    def setup_minmax(self, min_temp, max_temp):
        self.min_temp = min_temp
        self.max_temp = max_temp

    def setup_callback(self, cb):
        self._callback = cb

    def get_report_time_delta(self):
        return HOST_REPORT_TIME

    def _sample_value(self, eventtime):
        try:
            if self.path.startswith("http"):

                data = json.loads(urllib.urlopen(self.path).read())
                data = eval('data'+self.json_path)
                if data:
                    self.temp = float(data)
            else:
                tmp = ""
                with open(self.path,"r") as f: tmp = f.read()
                if tmp:
                    self.temp = float(tmp)
            self.temp*=self.factor

        except Exception:
            logging.exception("sensor_any: Error reading data")
            self.temp = 0.0
            return self.reactor.NEVER

        if self.temp < self.min_temp:
            self.printer.invoke_shutdown(
                "sensor_any value %0.1f below minimum value of %0.1f."
                % (self.temp, self.min_temp,))
        if self.temp > self.max_temp:
            self.printer.invoke_shutdown(
                "sensor_any value %0.1f above maximum value of %0.1f."
                % (self.temp, self.max_temp,))

        mcu = self.printer.lookup_object('mcu')
        measured_time = self.reactor.monotonic()
        self._callback(mcu.estimated_print_time(measured_time), self.temp)
        return measured_time + HOST_REPORT_TIME

    def get_status(self, eventtime):
        return {
            'temperature': self.temp,
        }



def load_config(config):
    # Register sensor
    pheaters = config.get_printer().load_object(config, "heaters")
    pheaters.add_sensor_factory("sensor_any", Sensor_ANY)
