# Support for button detection and callbacks
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import mcu
import time

class DirZCtl:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.toolhead = None
        self.mcu = mcu.get_printer_mcu(self.printer, config.get('use_mcu'))
        self.oid = self.mcu.create_oid()
        self.steppers = []
        self.mcu.register_config_callback(self._build_config)
        self.mcu.register_response(self._handle_debug_dirzctl, "debug_dirzctl", self.oid)
        self.mcu.register_response(self._handle_result_dirzctl, "result_dirzctl", self.oid)
        self.printer.register_event_handler('klippy:mcu_identify', self._handle_mcu_identify)
        self.printer.register_event_handler("klippy:shutdown", self._handle_shutdown)
        self.printer.register_event_handler("klippy:disconnect", self._handle_disconnect)
        self.gcode = self.printer.lookup_object("gcode")
        self.gcode.register_command('DIRZCTL', self.cmd_DIRZCTL, desc=self.cmd_DIRZCTL_help)
        self.all_params = []
        self.hx711s = None
        self.mcu_freq = 72000000
        self.step_base = config.getfloat('step_base', default=2, minval=1, maxval=6)
        self.last_send_heart = 0.
        self.is_shutdown = True
        self.is_timeout = True
        pass

    def _handle_mcu_identify(self):
        self.hx711s = self.printer.lookup_object('hx711s')
        self.steppers = []
        self.toolhead = self.printer.lookup_object('toolhead')
        for stepper in self.toolhead.get_kinematics().get_steppers():
            if stepper.is_active_axis('z'):
                self.steppers.append(stepper)
        self.mcu_freq = self.mcu.get_constant_float('CLOCK_FREQ')   

        # self.send_heart_beat_cmd = self.mcu.lookup_query_command(
        # "heart_beat_dirzctl oid=%c",
        # "heart_beat_dirzctl_result oid=%c",
        # oid=self.oid, cq=None)

        self.is_shutdown = False
        self.is_timeout = False
        pass

    def _build_config(self):
        self.mcu.add_config_cmd("config_dirzctl oid=%d z_count=%d" % (self.oid, len(self.steppers)))
        for i in range(len(self.steppers)):
            dir_pin, step_pin, ivt_dir, ivt_step = self.steppers[i].get_pin_info()
            self.mcu.add_config_cmd("add_dirzctl oid=%d index=%d dir_pin=%s step_pin=%s dir_invert=%d step_invert=%d" % (self.oid, i, dir_pin, step_pin, ivt_dir, ivt_step))

        # self.run_cmd = self.mcu.lookup_command("run_dirzctl oid=%c direct=%c step_us=%u step_cnt=%u is_ck_con=%c", cq=None)
        self.run_cmd = self.mcu.lookup_command("run_dirzctl oid=%c direct=%c step_us=%u step_cnt=%u", cq=None)
        pass

    def _handle_shutdown(self):
        self.is_shutdown = True
        pass
    
    def _handle_disconnect(self):
        self.is_timeout = True
        pass

    def _handle_debug_dirzctl(self, params):
        self.printer.lookup_object('prtouch').pnt_msg(str(params))
        pass

    def _handle_result_dirzctl(self, params):
        self.all_params.append(params)
        # self.printer.lookup_object('prtouch').pnt_msg(str(params))
        pass

    def get_params(self):
        return self.all_params, (self.all_params[0]['tick'] if len(self.all_params) > 0 else 0)

    def check_and_run(self, direct, step_us, step_cnt, wait_finish=True, is_ck_con=False):
        if self.is_shutdown or self.is_timeout:
            pass
        if step_cnt != 0:
            self.all_params = []
        # self.run_cmd.send([self.oid, direct, step_us, step_cnt, 1 if is_ck_con else 0])
        self.run_cmd.send([self.oid, direct, step_us, step_cnt])
        t_start = time.time()
        while not (self.is_shutdown or self.is_timeout) and wait_finish and ((time.time() - t_start) < (1.5 * 1000 * 1000 * step_us * step_cnt)) and len(self.all_params) != 2:
            self.hx711s.delay_s(0.05)
        pass

    def send_heart_beat(self):
        #if time.time() - self.last_send_heart > 0.1:
        #    self.send_heart_beat_cmd.send([self.oid])
        #    self.last_send_heart = time.time()
        pass

    cmd_DIRZCTL_help = "Test DIRZCTL."
    # DIRZCTL DIRECT=1 STEP_US=1500 STEP_CNT=100
    def cmd_DIRZCTL(self, gcmd):
        index = gcmd.get_int('INDEX', len(self.steppers), minval=0, maxval=len(self.steppers))
        direct = gcmd.get_int('DIRECT', 1, minval=0, maxval=1)
        step_us = gcmd.get_int('STEP_US', 1500, minval=4, maxval=100000)
        step_cnt = gcmd.get_int('STEP_CNT', 256, minval=0, maxval=10000)
        self.check_and_run(direct, step_us, step_cnt, False, False)
        pass


def load_config(config):
    return DirZCtl(config)
