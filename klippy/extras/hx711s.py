# Support for 1-wire based temperature sensors
#
# Copyright (C) 2020 Alan Lord <alanslists@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from os import remove
import time
import mcu
import math

class HX711S:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object("gcode")
        self.s_count = config.getint('count', 1, 1, 4)
        self.base_avgs = [0, 0, 0, 0]
        self.del_dirty = False
        self.index_dirty = 0
        self.start_tick = 0
        self.need_wait = False
        self.s_clk_pin = []
        self.s_sdo_pin = []
        self.all_params = []
        self.all_vals = [[], [], [], []]
        for i in range(self.s_count):
            self.s_clk_pin.append(config.get('sensor%d_clk_pin' % i, None if i == 0 else self.s_clk_pin[i - 1]))
            self.s_sdo_pin.append(config.get('sensor%d_sdo_pin' % i, None if i == 0 else self.s_sdo_pin[i - 1]))
        self.mcu = mcu.get_printer_mcu(self.printer, config.get('use_mcu'))
        self.oid = self.mcu.create_oid()
        self.mcu.register_config_callback(self._build_config)
        self.mcu.register_response(self._handle_debug_hx711s, "debug_hx711s", self.oid)
        self.mcu.register_response(self._handle_result_hx711s, "result_hx711s", self.oid)
        self.printer.register_event_handler('klippy:mcu_identify', self._handle_mcu_identify)
        self.printer.register_event_handler("klippy:shutdown", self._handle_shutdown)
        self.printer.register_event_handler("klippy:disconnect", self._handle_disconnect)
        self.gcode.register_command('READ_HX711', self.cmd_READ_HX711, desc=self.cmd_READ_HX711_help)
        self.pi_count = int(0)
        self.show_msg = False
        self.filter = None 
        self.query_cmd = None
        self.mcu_freq = 72000000
        self.last_send_heart = 0.
        self.is_shutdown = True
        self.is_timeout = True
        pass

    def _build_config(self):
        self.mcu.add_config_cmd("config_hx711s oid=%d hx711_count=%d" % (self.oid, self.s_count))
        pins = self.printer.lookup_object("pins") 
        for i in range(self.s_count):
            clk_pin_params = pins.lookup_pin(self.s_clk_pin[i])
            sdo_pin_params = pins.lookup_pin(self.s_sdo_pin[i])
            self.mcu.add_config_cmd("add_hx711s oid=%d index=%d clk_pin=%s sdo_pin=%s" % (self.oid, i, clk_pin_params['pin'], sdo_pin_params['pin']))
        # self.query_cmd = self.mcu.lookup_command("query_hx711s oid=%c times_read=%hu is_ck_con=%c", cq=None)
        self.query_cmd = self.mcu.lookup_command("query_hx711s oid=%c times_read=%hu", cq=None)
        self.filter = self.printer.lookup_object('filter')
        self.mcu_freq = self.mcu.get_constant_float('CLOCK_FREQ')
        pass

    def _handle_mcu_identify(self):
        # self.send_heart_beat_cmd = self.mcu.lookup_query_command(
        #     "heart_beat_hx711s oid=%c",
        #     "heart_beat_hx711s_result oid=%c",
        #     oid=self.oid, cq=None)
        pass

        self.is_shutdown = False
        self.is_timeout = False
        pass

    def _handle_debug_hx711s(self, params):
        self.printer.lookup_object('prtouch').pnt_msg(str(params))
        pass

    def _handle_shutdown(self):
        self.is_shutdown = True
        pass

    def _handle_disconnect(self):
        self.is_timeout = True
        pass

    def _handle_result_hx711s(self, params):
        while self.need_wait:
            self.delay_s(0.001)
        self.start_tick = self.start_tick if len(self.all_params) != 0 else params['nt']
        if self.del_dirty and (params['vd'] != 0 or params['it'] > 20) and self.index_dirty == 0:
            self.index_dirty = 1
            return
        self.index_dirty -= 1 if self.index_dirty == 1 else 0
        self.all_params.append(params)
        for i in range(self.s_count):
            self.all_vals[i].append(params['v%d' % i] - self.base_avgs[i])
        if self.show_msg:
            self.gcode.respond_info('Hx711 Val=' + str(params))
        if len(self.all_params) > self.pi_count:
            del self.all_params[0]
            for i in range(self.s_count):
                del self.all_vals[i][0]
        pass

    def query_start(self, pi_count, cycle_count, del_dirty=False, show_msg=False, is_ck_con=False):
        if self.is_shutdown or self.is_timeout:
            pass
        if cycle_count != 0:
            self.pi_count = pi_count
            self.all_params = []
            self.all_vals = [[], [], [], []]
            self.show_msg = show_msg
            self.del_dirty = del_dirty
            self.index_dirty = 0
        # self.query_cmd.send([self.oid, cycle_count, 1 if is_ck_con else 0])
        self.query_cmd.send([self.oid, cycle_count])
        pass

    def get_params(self):
        self.need_wait = True
        tmps = [x for x in self.all_params]
        self.need_wait = False
        return tmps, self.start_tick

    def get_vals(self):
        self.need_wait = True
        tmps = [[], [], [], []]
        for i in range(self.s_count):
            tmps[i] = [x for x in self.all_vals[i]]
        self.need_wait = False
        return tmps

    def delay_s(self, delay_s):
        toolhead = self.printer.lookup_object("toolhead")
        reactor = self.printer.get_reactor()
        eventtime = reactor.monotonic()
        if not self.printer.is_shutdown():
            toolhead.get_last_move_time()
            eventtime = reactor.pause(eventtime + delay_s)
            pass

    def send_heart_beat(self):
        # if time.time() - self.last_send_heart > 0.1:
        #     self.send_heart_beat_cmd.send([self.oid])
        #     self.last_send_heart = time.time()
        pass

    def read_base(self, cnt, max_hold, reset_zero=True):
        avgs = [0, 0, 0, 0]
        rvs = [[], [], [], []]
        for i in range(3):
            self.base_avgs = [0, 0, 0, 0]
            avgs = [0, 0, 0, 0]
            self.query_start(cnt, cnt + 5, del_dirty=True, show_msg=False)
            t_last = time.time()
            while not (self.is_shutdown or self.is_timeout) and len(self.get_vals()[0]) < cnt and (time.time() - t_last) < cnt * 0.010 * 15:
                self.delay_s(0.010)
                pass
            vals = self.get_vals()
            if len(vals[0]) < cnt:
                raise self.printer.command_error("""{"code":"key503", "msg":"z-Touch::read_base: Can not read z-Touch data."}""")
                
            for j in range(self.s_count):
                del vals[j][0:int(len(vals[j]) / 2)]
            for j in range(self.s_count):
                del vals[j][vals[j].index(min(vals[j]))]
                del vals[j][vals[j].index(min(vals[j]))]
                del vals[j][vals[j].index(max(vals[j]))]
                del vals[j][vals[j].index(max(vals[j]))]
            rvs = [[], [], [], []]
            tf = self.filter.get_tft()
            lf = self.filter.get_lft(0.5)
            for j in range(self.s_count):
                vals[j] = tf.ftr_val(vals[j])
                vals[j] = lf.ftr_val(vals[j])
                rvs[j].append(min(vals[j]))
                rvs[j].append(sum(vals[j]) / len(vals[j]))
                rvs[j].append(max(vals[j]))
                avgs[j] = sum(vals[j]) / len(vals[j])
                self.printer.lookup_object('prtouch').pnt_msg('READ_BASE ch=%d min=%.2f avg=%.2f max=%.2f' % (j, rvs[j][-3], avgs[j], rvs[j][-1]))
            if reset_zero:
                self.base_avgs = avgs

            sum_max = 0
            for j in range(self.s_count):
                sum_max += math.fabs(rvs[j][2] - rvs[j][0])
            if sum_max < max_hold * 2:
                break
        return avgs, rvs

    cmd_READ_HX711_help = "Read hx711s vals"

    def cmd_READ_HX711(self, gcmd):
        cnt = gcmd.get_int('C', 1, minval=1, maxval=9999) 
        self.query_start(cnt, cnt, False, False, False)
        self.delay_s(1.)
        self.base_avgs = [0, 0, 0, 0]
        vals = self.get_vals()
        for i in range(self.s_count):
            self.gcode.respond_info('CH%d=' % i)
            sv = '['
            for j in range(len(vals[i])):
                sv += '%.2f, ' % vals[i][j]
            self.gcode.respond_info(sv + ']')
        self.read_base(40, 500000)
        pass


def load_config(config):
    return HX711S(config)
