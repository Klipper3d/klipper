# HX710/HX711 weighing sensor support
#
# Copyright (C) 2023 guoge <guoge@tiertime.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import mcu

######################################################################
# Compatible Sensors:
#       HX710A / HX711 / HX712
######################################################################


class HX71X:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()
        self.mcu = mcu.get_printer_mcu(self.printer,
                                       config.get('hx71x_mcu', 'mcu'))
        self.oid = self.mcu.create_oid()

        # Determine pin from config
        ppins = config.get_printer().lookup_object("pins")
        sck_params = ppins.lookup_pin(config.get('hx71x_sck_pin'))
        dout_params = ppins.lookup_pin(config.get('hx71x_dout_pin'))

        self.mcu.add_config_cmd(
            "config_hx71x oid=%d  sck_pin=%s dout_pin=%s"
            % (self.oid, sck_params['pin'], dout_params['pin']))

        #update period
        self.report_time = config.getfloat('hx71x_report_time', 2,
                                           minval=0.02)

        #unit scale
        self.scale = config.getfloat('hx71x_scale', 0.001)

        self.weight = 0.0
        self.sample_timer = self.reactor.register_timer(self._sample_hx71x)
        self.printer.add_object("hx71x " + self.name, self)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)

        self.cmd_queue = self.mcu.alloc_command_queue()
        self.mcu.register_config_callback(self.build_config)

    def handle_connect(self):
        self.reactor.update_timer(self.sample_timer, self.reactor.NOW)
        return

    def build_config(self):
        self.read_hx71x_cmd = self.mcu.lookup_query_command(
            "read_hx71x oid=%c read_len=%u",
            "read_hx71x_response oid=%c response=%*s", oid=self.oid,
            cq=self.cmd_queue)

    def read_hx71x(self, read_len):
        return self.read_hx71x_cmd.send([self.oid, read_len])

    def setup_callback(self, cb):
        self._callback = cb

    def get_report_time_delta(self):
        return self.report_time

    def _sample_hx71x(self, eventtime):
        params = self.read_hx71x(4)

        response = bytearray(params['response'])
        w = int.from_bytes(response, byteorder='little', signed=True)
        self.weight = w * self.scale # weight scale

        logging.info(" read hx711 @ %.3f , weight:%.2f", eventtime, self.weight)
        return eventtime + self.report_time

    def get_status(self, eventtime):
        return {
            'weight': round(self.weight, 2)
        }


def load_config(config):
    return HX71X(config)

def load_config_prefix(config):
    return HX71X(config)
