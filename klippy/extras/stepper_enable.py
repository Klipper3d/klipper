# Support for enable pins on stepper motor drivers
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

DISABLE_STALL_TIME = 0.100

# Tracking of shared stepper enable pins
class StepperEnablePin:
    def __init__(self, mcu_enable, enable_count=0):
        self.mcu_enable = mcu_enable
        self.enable_count = enable_count
    def set_enable(self, print_time):
        if not self.enable_count:
            self.mcu_enable.set_digital(print_time, 1)
        self.enable_count += 1
    def set_disable(self, print_time):
        self.enable_count -= 1
        if not self.enable_count:
            self.mcu_enable.set_digital(print_time, 0)

# Automatic multiple pin enable lines
class StepperMultiEnablePin:
    def __init__(self, enable_list):
        self.enable_list = enable_list
    def set_enable(self, print_time):
        for en in self.enable_list:
            en.set_enable(print_time)
    def set_disable(self, print_time):
        for en in self.enable_list:
            en.set_disable(print_time)

# Resolve the 'enable_pin' stepper config setting
def lookup_enable_pin(ppins, pin_list):
    if pin_list is None:
        return StepperEnablePin(None, 9999)
    enable_list = []
    for pin in pin_list.split(','):
        pin_params = ppins.lookup_pin(pin, can_invert=True,
                                      share_type='stepper_enable')
        enable = pin_params.get('class')
        if enable is None:
            mcu_enable = pin_params['chip'].setup_pin('digital_out', pin_params)
            mcu_enable.setup_max_duration(0.)
            pin_params['class'] = enable = StepperEnablePin(mcu_enable)
        enable_list.append(enable)
    if len(enable_list) == 1:
        return enable_list[0]
    return StepperMultiEnablePin(enable_list)

# Enable line tracking for each stepper motor
class EnableTracking:
    def __init__(self, printer, stepper, pin):
        self.stepper = stepper
        self.is_enabled = False
        self.stepper.add_active_callback(self.motor_enable)
        self.enable = lookup_enable_pin(printer.lookup_object('pins'), pin)
    def motor_enable(self, print_time):
        if not self.is_enabled:
            self.enable.set_enable(print_time)
            self.is_enabled = True
    def motor_disable(self, print_time):
        if self.is_enabled:
            # Enable stepper on future stepper movement
            self.enable.set_disable(print_time)
            self.is_enabled = False
            self.stepper.add_active_callback(self.motor_enable)
    def is_motor_enabled(self):
        return self.is_enabled

class PrinterStepperEnable:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.enable_lines = {}
        self.printer.register_event_handler("gcode:request_restart",
                                            self._handle_request_restart)
        # Register M18/M84 commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("M18", self.cmd_M18)
        gcode.register_command("M84", self.cmd_M18)
    def register_stepper(self, stepper, pin):
        name = stepper.get_name()
        self.enable_lines[name] = EnableTracking(self.printer, stepper, pin)
    def motor_off(self):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.dwell(DISABLE_STALL_TIME)
        print_time = toolhead.get_last_move_time()
        for el in self.enable_lines.values():
            el.motor_disable(print_time)
        self.printer.send_event("stepper_enable:motor_off", print_time)
        toolhead.dwell(DISABLE_STALL_TIME)
        logging.debug('; Max time of %f', print_time)
    def _handle_request_restart(self, print_time):
        self.motor_off()
    def cmd_M18(self, params):
        # Turn off motors
        self.motor_off()
    def lookup_enable(self, name):
        return self.enable_lines[name]

def load_config(config):
    return PrinterStepperEnable(config)
