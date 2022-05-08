# Support for hx711 load cell sensor
#
# Copyright (C) 2022  Jochen Loeser <jochen_loeser@trimble.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class SensorHx711(object):
    def __init__(self, config):
        try:
            import numpy as np
        except:
            raise config.error("HX711 requires numpy module")
        self.printer = config.get_printer()
        self._name = config.get_name().split()[-1]
        self._last_value = None
        # Create polynom
        try:
            data_str = config.get('calib_data', '0,0;1,1')
            data = np.array(np.mat(data_str))
            fit = np.polyfit(data[:,0], data[:,1], 1)
            self._line = np.poly1d(fit)
        except:
            raise config.error("HX711: There is an error with your calib_data.")
        # Setup pins
        ppins = self.printer.lookup_object('pins')
        self._sclk_pinparam = ppins.lookup_pin(config.get('sclk_pin'))
        self._dout_pinparam = ppins.lookup_pin(config.get('dout_pin'))
        self._mcu = None
        for pin in [self._sclk_pinparam, self._dout_pinparam]:
            if self._mcu is not None and pin['chip'] != self._mcu:
                raise ppins.error("button pins must be on same mcu")
            self._mcu = pin['chip']
        # Create an OID
        self._oid = self._mcu.create_oid()
        # Register commands
        self._query_hx711_value_cmd = None
        self._mcu.register_config_callback(self._build_config)
        self._mcu.register_response(self._handle_hx711_value,
            "hx711_value", oid=self._oid)
        # Register event handler
        self.printer.register_event_handler("klippy:ready", self._handle_ready)
        # Register GCODE command
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("QUERY_HX711", "NAME", self._name,
                                   self.cmd_QUERY_HX711,
                                   desc=self.cmd_QUERY_HX711_help)
    def _handle_ready(self):
        self._start_measurement()
    def _build_config(self):
        self._mcu.add_config_cmd("config_hx711 oid=%d sclk_pin=%s dout_pin=%s"
            % (self._oid, self._sclk_pinparam['pin'],
                self._dout_pinparam['pin']))
        self._query_hx711_value_cmd = self._mcu.lookup_command(
            "query_hx711_value oid=%c clock=%u rest_ticks=%u")
    def _handle_hx711_value(self, params):
        self._last_value = params['value']
        self._start_measurement()
    def _start_measurement(self):
        clock = self._mcu.get_query_slot(self._oid)
        rest_ticks = self._mcu.seconds_to_clock(0.001)
        if self._query_hx711_value_cmd is not None:
            self._query_hx711_value_cmd.send([self._oid, clock, rest_ticks],
                reqclock=clock)
    def get_status(self, eventtime):
        if self._last_value is None:
            return {
                'raw_value': None,
                'value':     None,
            }
        raw_value = self._last_value
        value = self._line(raw_value)
        return {
            'raw_value': str(raw_value),
            'value':     str(value),
        }
    cmd_QUERY_HX711_help = "Query hx711 for the current values"
    def cmd_QUERY_HX711(self, gcmd):
        if self._last_value is None:
            gcmd.respond_info("hx711 value(raw_value): no value queried")
            return
        raw_value = self._last_value
        value = self._line(raw_value)
        gcmd.respond_info("hx711 value(raw_value): %.6f(%u)"
            % (value, raw_value))

def load_config_prefix(config):
    return SensorHx711(config)
