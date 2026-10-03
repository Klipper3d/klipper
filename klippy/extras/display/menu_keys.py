# -*- coding: utf-8 -*-
# Support for menu button press tracking
#
# Copyright (C) 2018  Janar Sööt <janar.soot@gmail.com>
# Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
# Architecture Spec: Direct zero-latency hardware pin manipulation for
# click feedback; avoided G-code queueing to prevent audio output lag
# during main thread execution.
from .. import pwm_cycle_time

LONG_PRESS_DURATION = 0.800
TIMER_DELAY = .200

class MenuKeys:
    def __init__(self, config, callback):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.callback = callback
        buttons = self.printer.load_object(config, "buttons")
        # Register rotary encoder
        encoder_pins = config.get('encoder_pins', None)
        encoder_steps_per_detent = config.getchoice('encoder_steps_per_detent',
                                                    [2, 4], 4)
        if encoder_pins is not None:
            try:
                pin1, pin2 = encoder_pins.split(',')
            except:
                raise config.error("Unable to parse encoder_pins")
            buttons.register_rotary_encoder(pin1.strip(), pin2.strip(),
                                            self.encoder_cw_callback,
                                            self.encoder_ccw_callback,
                                            encoder_steps_per_detent)
        self.encoder_fast_rate = config.getfloat('encoder_fast_rate',
                                                 .030, above=0.)
        self.last_encoder_cw_eventtime = 0
        self.last_encoder_ccw_eventtime = 0
        # Register click button
        self.is_short_click = False
        self.click_timer = self.reactor.register_timer(self.long_click_event)
        self.beep_timer = self.reactor.register_timer(self._do_beep_click)
        self.register_button(config, 'click_pin', self.click_callback, False)

        # Register integrated buzzer pin
        self.buzzer_pin = config.get('buzzer_pin', None)
        self.buzzer_pwm = config.getboolean('buzzer_pwm', True)
        self.beeper_mcu_pin = None
        self.beeper_last_print_time = 0.
        self.beeper_last_value = 0.
        self.beeper_last_cycle_time = 0.001

        if self.buzzer_pin is not None:
            ppins = self.printer.lookup_object('pins')
            if self.buzzer_pwm:
                pin_params = ppins.lookup_pin(self.buzzer_pin, can_invert=True)
                self.beeper_mcu_pin = pwm_cycle_time.MCU_pwm_cycle(
                    pin_params, 0.001, 0., 0.)
                self.beeper_mcu = self.beeper_mcu_pin.get_mcu()
            else:
                self.beeper_mcu_pin = ppins.setup_pin(
            'digital_out', self.buzzer_pin)
                self.beeper_mcu_pin.setup_max_duration(0.)
                self.beeper_mcu_pin.setup_start_value(0., 0.)
                self.beeper_mcu = self.beeper_mcu_pin.get_mcu()

            gcode = self.printer.lookup_object('gcode')
            gcode.register_command(
            "M300", self.cmd_M300, desc="Play tone on display buzzer")

        # Register other buttons
        self.register_button(config, 'back_pin', self.back_callback)
        self.register_button(config, 'up_pin', self.up_callback)
        self.register_button(config, 'down_pin', self.down_callback)
        self.register_button(config, 'kill_pin', self.kill_callback)

    def register_button(self, config, name, callback, push_only=True):
        pin = config.get(name, None)
        if pin is None:
            return
        buttons = self.printer.lookup_object("buttons")
        if config.get('analog_range_' + name, None) is None:
            if push_only:
                buttons.register_button_push(pin, callback)
            else:
                buttons.register_buttons([pin], callback)
            return
        amin, amax = config.getfloatlist('analog_range_' + name, count=2)
        pullup = config.getfloat('analog_pullup_resistor', 4700., above=0.)
        if push_only:
            buttons.register_adc_button_push(pin, amin, amax, pullup, callback)
        else:
            buttons.register_adc_button(pin, amin, amax, pullup, callback)

    # Rotary encoder callbacks
    def encoder_cw_callback(self, eventtime):
        fast_rate = (eventtime < self.last_encoder_cw_eventtime
                     + self.encoder_fast_rate)
        self.last_encoder_cw_eventtime = eventtime
        if fast_rate:
            self.callback('fast_up', eventtime)
        else:
            self.callback('up', eventtime)

    def encoder_ccw_callback(self, eventtime):
        fast_rate = (eventtime < self.last_encoder_ccw_eventtime
                     + self.encoder_fast_rate)
        self.last_encoder_ccw_eventtime = eventtime
        if fast_rate:
            self.callback('fast_down', eventtime)
        else:
            self.callback('down', eventtime)

    # Click handling
    def _do_beep_click(self, eventtime):
        if self.beeper_mcu_pin is not None:
            if hasattr(self.beeper_mcu, 'systime_to_print_time'):
                systime = self.reactor.monotonic()
                print_time = (self.beeper_mcu.systime_to_print_time(systime)
                              + self.beeper_mcu.min_schedule_time())
            else:
                toolhead = self.printer.lookup_object('toolhead')
                print_time = toolhead.get_last_move_time()
            self._queue_beep(print_time, 0.040, 1000.0)
        else:
            # Fallback to output_pin beeper for backward compatibility
            beeper = self.printer.lookup_object('output_pin beeper', None)
            if beeper is not None:
                # Safely resolve mcu and methods for CI Mock test compatibility
                mcu_pin = getattr(beeper, 'mcu_pin', None)
                mcu = mcu_pin.get_mcu() if mcu_pin is not None else None

                if mcu is not None and hasattr(mcu, 'systime_to_print_time'):
                    systime = self.reactor.monotonic()
                    print_time = (mcu.systime_to_print_time(systime)
                                  + mcu.min_schedule_time())
                else:
                    toolhead = self.printer.lookup_object('toolhead')
                    print_time = toolhead.get_last_move_time()

                beeper.gcrq.send_async_request(0.5, print_time)
                beeper.gcrq.send_async_request(0.0, print_time + 0.040)
            else:
                gcode = self.printer.lookup_object('gcode', None)
                if gcode is not None:
                    gcode.run_script_from_command("BEEP_CLICK")
        return self.reactor.NEVER

    def cmd_M300(self, gcmd):
        duration = gcmd.get_float('P', 100.0, minval=0.0) / 1000.0
        frequency = gcmd.get_float('S', 1000.0, minval=0.0)
        if self.beeper_mcu_pin is not None:
            toolhead = self.printer.lookup_object('toolhead')
            toolhead.register_lookahead_callback(
                lambda pt: self._queue_beep(pt, duration, frequency))

    def _set_beeper(self, print_time, value, cycle_time):
        if (value == self.beeper_last_value and
            cycle_time == self.beeper_last_cycle_time):
            return
        min_sched = self.beeper_mcu.min_schedule_time()
        print_time = max(print_time, self.beeper_last_print_time + min_sched)
        if self.buzzer_pwm:
            self.beeper_mcu_pin.set_pwm_cycle(print_time, value, cycle_time)
        else:
            self.beeper_mcu_pin.set_digital(print_time, 1 if value > 0. else 0)
        self.beeper_last_value = value
        self.beeper_last_cycle_time = cycle_time
        self.beeper_last_print_time = print_time

    def _queue_beep(self, print_time, duration, frequency):
        cycle_time = 1.0 / frequency if frequency > 0. else 0.001
        value = 0.5 if frequency > 0. else 0.0
        self._set_beeper(print_time, value, cycle_time)
        self._set_beeper(print_time + duration, 0.0, cycle_time)

    def _beep_click(self):
        # Defer beep execution to avoid blocking the main thread
        # and dropping button events during high-speed printing
        self.reactor.update_timer(self.beep_timer, self.reactor.NOW)

    def long_click_event(self, eventtime):
        self.is_short_click = False
        self.callback('long_click', eventtime)
        return self.reactor.NEVER

    def click_callback(self, eventtime, state):
        if state:
            self._beep_click()
            self.is_short_click = True
            self.reactor.update_timer(self.click_timer,
                                      eventtime + LONG_PRESS_DURATION)
        elif self.is_short_click:
            self.reactor.update_timer(self.click_timer, self.reactor.NEVER)
            self.callback('click', eventtime)

    # Other button callbacks
    def back_callback(self, eventtime):
        self.callback('back', eventtime)

    def up_callback(self, eventtime):
        self.callback('up', eventtime)

    def down_callback(self, eventtime):
        self.callback('down', eventtime)

    def kill_callback(self, eventtime):
        self.printer.invoke_shutdown("Shutdown due to kill button!")
