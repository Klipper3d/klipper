# Generic Filament Jam Sensor Module
#
# Copyright (C) 2019 Windo.AC
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging


class JamSensor:

    def __init__(self, config):
        self.mname = config.get_name().split()[0]
        self.name = config.get_name().split()[1]
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.reactor = self.printer.get_reactor()
        self.buttons = self.printer.try_load_module(config, 'buttons')
        self.buttons.register_buttons([config.get('pin')],
                self._signal_handler)
        self.timer = config.getfloat('timer', 1., above=0.)
        self.extruder_index = int(config.getint('extruder_index', 0))
        self.base_usage = config.getfloat('base_usage', None, above=0.)
        self.slow_usage = config.getfloat('slow_usage', None,
                above=self.base_usage)
        self.slow_speed = config.getfloat('slow_speed', 60., above=0.)
        self.slow_gcode = config.get('slow_gcode', None)
        self.jam_usage = config.getfloat('jam_usage', None,
                above=self.slow_usage)
        self.jam_gcode = config.get('jam_gcode', None)
        self.jam_pause = config.getboolean('jam_pause', True)
        self.filament_usage_last = 0.
        self.timer_usage_last = 0.
        self.toolhead = None
        self.pause_resume = None
        self.enable = False
        self.debug = False
        self.action = True
        self.cmd_SET_FILAMENT_JAM_SENSOR_help = \
            'Set param of FILAMENT_JAM_SENSOR'
        self.gcode.register_mux_command('SET_FILAMENT_JAM_SENSOR',
                'SENSOR', self.name, self.cmd_SET_FILAMENT_JAM_SENSOR,
                desc=self.cmd_SET_FILAMENT_JAM_SENSOR_help)
        self.printer.register_event_handler('klippy:ready',
                self._handle_ready)

    def _handle_ready(self):
        self.toolhead = self.printer.lookup_object('toolhead')
        if self.base_usage is None or self.slow_usage is None \
            or self.jam_usage is None:
            logging.exception('base_usage / slow_usage / jam_usage must defined'
                              )
            self.enable = False
        elif self.base_usage >= self.slow_usage or self.slow_usage \
            >= self.jam_usage:
            logging.exception(
              'base_usage must < slow_usage / slow_usage must < jam_usage'
                              )
            self.enable = False
        else:
            self.enable = True
            self.filament_usage_last = self.get_filament_usage()
            self.pause_resume = \
                self.printer.lookup_object('pause_resume', None)
            if self.pause_resume is None:
                logging.exception('pause_resume must define')
            else:
                self.reactor.register_timer(self._timer_handler,
                        self.reactor.NOW)
                logging.info('%s(%s): ready to use', self.mname,
                             self.name)

    def _signal_handler(self, eventtime, state):
        if self.enable and state == 1:
            new_usage = self.get_filament_usage()
            delta_usage = abs(new_usage - self.filament_usage_last)
            self.filament_usage_last = new_usage
            if self.debug:
                self.gcode.respond_info('%s(%s): %.2f %% ( %.2f / %.2f )'
                         % (self.mname, self.name, delta_usage
                        / self.base_usage * 100., delta_usage,
                        self.base_usage))
                logging.debug(
                  '%s(%s): _signal_handler triggered | delta_usage = %s'
                              , self.mname, self.name, delta_usage)
            self.check_jam(eventtime, delta_usage)

    def _timer_handler(self, eventtime):
        if self.enable:
            new_usage = self.get_filament_usage()
            delta_usage = abs(new_usage - self.filament_usage_last)
            if self.debug:
                logging.debug(
                  '%s(%s): _timer_handler triggered | delta_usage = %s'
                              , self.mname, self.name, delta_usage)
            self.check_jam(eventtime, delta_usage)
            if self.timer_usage_last != new_usage:  # ignore when it's not move
                self.check_jam(eventtime, delta_usage)
            self.timer_usage_last = new_usage
        return eventtime + self.timer

    def check_jam(self, eventtime, delta_usage):
        if delta_usage > self.jam_usage:
            # detect jam
            # restart the last value avoid trigger when pause
            self.filament_usage_last = self.get_filament_usage()
            self.gcode.respond_info('%s(%s): detect jam' % (self.mname,
                                    self.name))
            if self.action and self.pause_resume.captured_position \
                is None:  # only trigger pause when not in pause status
                self.pause_resume.send_pause_command()
                if self.jam_gcode:
                    self._exec_gcode(self.jam_gcode)
        elif delta_usage > self.slow_usage:
            # detect slow
            self.gcode.respond_info('%s(%s): detect slow'
                                    % (self.mname, self.name))
            if self.action:
                self.gcode.speed_factor = self.slow_speed / (60. * 100.)
                if self.slow_gcode:
                    self._exec_gcode(self.slow_gcode)

    def get_filament_usage(self):
        theout = 0.
        if self.toolhead:
            theout = self.toolhead.get_position()[3
                    + self.extruder_index]
        return float(theout)

    def _exec_gcode(self, script):
        try:
            self.gcode.run_script(script)
        except Exception:
            logging.exception('Script running error')

    def cmd_SET_FILAMENT_JAM_SENSOR(self, params):
        self.timer = self.gcode.get_float('TIMER', params, self.timer,
                minval=0.)
        self.base_usage = self.gcode.get_float('BASE_USAGE', params,
                self.base_usage, minval=0.)
        temp_slow_usage = self.gcode.get_float('SLOW_USAGE', params,
                self.slow_usage, minval=0.)
        if temp_slow_usage is not None and temp_slow_usage \
            > self.base_usage:
            self.slow_usage = temp_slow_usage
        temp_jam_usage = self.gcode.get_float('JAM_USAGE', params,
                self.jam_usage, minval=0.)
        if temp_jam_usage is not None and temp_jam_usage \
            > self.slow_usage:
            self.jam_usage = temp_jam_usage
        self.enable = (True if self.gcode.get_int('ENABLE', params,
                       (1 if self.enable else 0)) == 1 else False)
        self.debug = (True if self.gcode.get_int('DEBUG', params,
                      (1 if self.debug else 0)) == 1 else False)
        self.action = (True if self.gcode.get_int('ACTION', params,
                       (1 if self.action else 0)) == 1 else False)
        self.gcode.respond_info(
          '%s(%s): timer = %.2f | base_usage = %.2f | slow_usage = %.2f '
          '| jam_usage = %.2f | enable = %s | debug = %s | action = %s'
            % (
            self.mname,
            self.name,
            self.timer,
            self.base_usage,
            self.slow_usage,
            self.jam_usage,
            self.enable,
            self.debug,
            self.action,
            ))
        # reset some counter
        self.timer_usage_last = self.filament_usage_last = \
            self.get_filament_usage()


def load_config_prefix(config):
    return JamSensor(config)
