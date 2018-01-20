# Code to configure miscellaneous chips
#
# Copyright (C) 2017,2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import pins, mcu


######################################################################
# Output pins
######################################################################

class PrinterStaticDigitalOut:
    def __init__(self, printer, config):
        pin_list = [pin.strip() for pin in config.get('pins').split(',')]
        for pin_desc in pin_list:
            mcu_pin = pins.setup_pin(printer, 'digital_out', pin_desc)
            mcu_pin.setup_start_value(1, 1, True)

PIN_MIN_TIME = 0.100

class PrinterPin:
    def __init__(self, printer, config):
        self.printer = printer
        self.is_pwm = 'pwm' in config.get_name().split()[0]
        if self.is_pwm:
            self.mcu_pin = pins.setup_pin(printer, 'pwm', config.get('pin'))
            hard_pwm = config.getint('hard_pwm', None, minval=1)
            if hard_pwm is None:
                self.mcu_pin.setup_cycle_time(config.getfloat(
                    'cycle_time', 0.100, above=0.))
            else:
                self.mcu_pin.setup_hard_pwm(hard_pwm)
            self.scale = config.getfloat('scale', 1., above=0.)
        else:
            self.mcu_pin = pins.setup_pin(
                printer, 'digital_out', config.get('pin'))
            self.scale = 1.
        self.mcu_pin.setup_max_duration(0.)
        self.last_value_time = 0.
        self.last_value = config.getfloat(
            'value', 0., minval=0., maxval=self.scale) / self.scale
        self.is_static = config.get_name().startswith('static_')
        if self.is_static:
            self.mcu_pin.setup_start_value(
                self.last_value, self.last_value, True)
        else:
            shutdown_value = config.getfloat(
                'shutdown_value', 0., minval=0., maxval=self.scale) / self.scale
            self.mcu_pin.setup_start_value(self.last_value, shutdown_value)
        self.gcode = printer.lookup_object('gcode')
        self.gcode.register_command("SET_PIN", self.cmd_SET_PIN,
                                    desc=self.cmd_SET_PIN_help)
    cmd_SET_PIN_help = "Set the value of an output pin"
    def cmd_SET_PIN(self, params):
        pin_name = self.gcode.get_str('PIN', params)
        pin = self.printer.lookup_object('pin ' + pin_name, None)
        if pin is not self:
            if pin is None:
                raise self.gcode.error("Pin not configured")
            return pin.cmd_SET_PIN(params)
        if self.is_static:
            raise self.gcode.error("Static pin can not be changed at run-time")
        value = self.gcode.get_float('VALUE', params) / self.scale
        if value == self.last_value:
            return
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        print_time = max(print_time, self.last_value_time + PIN_MIN_TIME)
        if self.is_pwm:
            if value < 0. or value > 1.:
                raise self.gcode.error("Invalid pin value")
            self.mcu_pin.set_pwm(print_time, value)
        else:
            if value not in [0., 1.]:
                raise self.gcode.error("Invalid pin value")
            self.mcu_pin.set_digital(print_time, value)
        self.last_value = value
        self.last_value_time = print_time

class PrinterMultiPin:
    def __init__(self, printer, config):
        self.printer = printer
        try:
            pins.get_printer_pins(printer).register_chip('multi_pin', self)
        except pins.error:
            pass
        self.pin_type = None
        self.pin_list = [pin.strip() for pin in config.get('pins').split(',')]
        self.mcu_pins = []
    def setup_pin(self, pin_params):
        pin_name = pin_params['pin']
        pin = self.printer.lookup_object('multi_pin ' + pin_name, None)
        if pin is not self:
            if pin is None:
                raise pins.error("multi_pin %s not configured" % (pin_name,))
            return pin.setup_pin(pin_params)
        if self.pin_type is not None:
            raise pins.error("Can't setup multi_pin %s twice" % (pin_name,))
        self.pin_type = pin_params['type']
        invert = ""
        if pin_params['invert']:
            invert = "!"
        self.mcu_pins = [
            pins.setup_pin(self.printer, self.pin_type, invert + pin_desc)
            for pin_desc in self.pin_list]
        return self
    def get_mcu(self):
        return self.mcu_pins[0].get_mcu()
    def setup_max_duration(self, max_duration):
        for mcu_pin in self.mcu_pins:
            mcu_pin.setup_max_duration(max_duration)
    def setup_start_value(self, start_value, shutdown_value):
        for mcu_pin in self.mcu_pins:
            mcu_pin.setup_start_value(start_value, shutdown_value)
    def setup_cycle_time(self, cycle_time):
        for mcu_pin in self.mcu_pins:
            mcu_pin.setup_cycle_time(cycle_time)
    def setup_hard_pwm(self, hard_cycle_ticks):
        for mcu_pin in self.mcu_pins:
            mcu_pin.setup_hard_pwm(hard_cycle_ticks)
    def set_digital(self, print_time, value):
        for mcu_pin in self.mcu_pins:
            mcu_pin.set_digital(print_time, value)
    def set_pwm(self, print_time, value):
        for mcu_pin in self.mcu_pins:
            mcu_pin.set_pwm(print_time, value)


######################################################################
# Servos
######################################################################

SERVO_SIGNAL_PERIOD = 0.020

class PrinterServo:
    def __init__(self, printer, config):
        self.printer = printer
        self.mcu_servo = pins.setup_pin(printer, 'pwm', config.get('pin'))
        self.mcu_servo.setup_max_duration(0.)
        self.mcu_servo.setup_cycle_time(SERVO_SIGNAL_PERIOD)
        self.min_width = config.getfloat(
            'minimum_pulse_width', .001, above=0., below=SERVO_SIGNAL_PERIOD)
        self.max_width = config.getfloat(
            'maximum_pulse_width', .002
            , above=self.min_width, below=SERVO_SIGNAL_PERIOD)
        self.max_angle = config.getfloat('maximum_servo_angle', 180.)
        self.angle_to_width = (self.max_width - self.min_width) / self.max_angle
        self.width_to_value = 1. / SERVO_SIGNAL_PERIOD
        self.last_value = self.last_value_time = 0.
        self.gcode = printer.lookup_object('gcode')
        self.gcode.register_command("SET_SERVO", self.cmd_SET_SERVO,
                                    desc=self.cmd_SET_SERVO_help)
    def set_pwm(self, print_time, value):
        if value == self.last_value:
            return
        print_time = max(print_time, self.last_value_time + PIN_MIN_TIME)
        self.mcu_servo.set_pwm(print_time, value)
        self.last_value = value
        self.last_value_time = print_time
    def set_angle(self, print_time, angle):
        angle = max(0., min(self.max_angle, angle))
        width = self.min_width + angle * self.angle_to_width
        self.set_pwm(print_time, width * self.width_to_value)
    def set_pulse_width(self, print_time, width):
        width = max(self.min_width, min(self.max_width, width))
        self.set_pwm(print_time, width * self.width_to_value)
    cmd_SET_SERVO_help = "Set servo angle"
    def cmd_SET_SERVO(self, params):
        servo_name = self.gcode.get_str('SERVO', params)
        servo = self.printer.lookup_object('servo ' + servo_name, None)
        if servo is not self:
            if servo is None:
                raise self.gcode.error("Servo not configured")
            return servo.cmd_SET_SERVO(params)
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        if 'WIDTH' in params:
            self.set_pulse_width(print_time,
                                 self.gcode.get_float('WIDTH', params))
        else:
            self.set_angle(print_time, self.gcode.get_float('ANGLE', params))


######################################################################
# AD5206 digipot
######################################################################

class ad5206:
    def __init__(self, printer, config):
        enable_pin_params = pins.get_printer_pins(printer).lookup_pin(
            'digital_out', config.get('enable_pin'))
        self.mcu = enable_pin_params['chip']
        self.pin = enable_pin_params['pin']
        self.mcu.add_config_object(self)
        scale = config.getfloat('scale', 1., above=0.)
        self.channels = [None] * 6
        for i in range(len(self.channels)):
            val = config.getfloat('channel_%d' % (i+1,), None,
                                  minval=0., maxval=scale)
            if val is not None:
                self.channels[i] = int(val * 256. / scale + .5)
    def build_config(self):
        for i, val in enumerate(self.channels):
            if val is not None:
                self.mcu.add_config_cmd(
                    "send_spi_message pin=%s msg=%02x%02x" % (self.pin, i, val))


######################################################################
# Replicape board
######################################################################

REPLICAPE_MAX_CURRENT = 3.84
REPLICAPE_SHIFT_REGISTER_BUS = 1
REPLICAPE_SHIFT_REGISTER_DEVICE = 1
REPLICAPE_PCA9685_BUS = 2
REPLICAPE_PCA9685_ADDRESS = 0x70
REPLICAPE_PCA9685_CYCLE_TIME = .001

class pca9685_pwm:
    def __init__(self, replicape, channel, pin_params):
        self._replicape = replicape
        self._channel = channel
        if pin_params['type'] not in ['digital_out', 'pwm']:
            raise pins.error("Pin type not supported on replicape")
        self._mcu = replicape.host_mcu
        self._mcu.add_config_object(self)
        self._bus = REPLICAPE_PCA9685_BUS
        self._address = REPLICAPE_PCA9685_ADDRESS
        self._cycle_time = REPLICAPE_PCA9685_CYCLE_TIME
        self._max_duration = 2.
        self._oid = None
        self._invert = pin_params['invert']
        self._start_value = self._shutdown_value = float(self._invert)
        self._is_static = False
        self._last_clock = 0
        self._pwm_max = 0.
        self._cmd_queue = self._mcu.alloc_command_queue()
        self._set_cmd = None
    def get_mcu(self):
        return self._mcu
    def setup_max_duration(self, max_duration):
        self._max_duration = max_duration
    def setup_cycle_time(self, cycle_time):
        pass
    def setup_hard_pwm(self, hard_cycle_ticks):
        if hard_cycle_ticks:
            raise pins.error("pca9685 does not support hard_pwm parameter")
    def setup_start_value(self, start_value, shutdown_value, is_static=False):
        if is_static and start_value != shutdown_value:
            raise pins.error("Static pin can not have shutdown value")
        if self._invert:
            start_value = 1. - start_value
            shutdown_value = 1. - shutdown_value
        self._start_value = max(0., min(1., start_value))
        self._shutdown_value = max(0., min(1., shutdown_value))
        self._is_static = is_static
        self._replicape.note_pwm_start_value(
            self._channel, self._start_value, self._shutdown_value)
    def build_config(self):
        self._pwm_max = self._mcu.get_constant_float("PCA9685_MAX")
        cycle_ticks = self._mcu.seconds_to_clock(self._cycle_time)
        if self._is_static:
            self._mcu.add_config_cmd(
                "set_pca9685_out bus=%d addr=%d channel=%d"
                " cycle_ticks=%d value=%d" % (
                    self._bus, self._address, self._channel,
                    cycle_ticks, self._start_value * self._pwm_max))
            return
        self._oid = self._mcu.create_oid()
        self._mcu.add_config_cmd(
            "config_pca9685 oid=%d bus=%d addr=%d channel=%d cycle_ticks=%d"
            " value=%d default_value=%d max_duration=%d" % (
                self._oid, self._bus, self._address, self._channel, cycle_ticks,
                self._start_value * self._pwm_max,
                self._shutdown_value * self._pwm_max,
                self._mcu.seconds_to_clock(self._max_duration)))
        self._set_cmd = self._mcu.lookup_command(
            "schedule_pca9685_out oid=%c clock=%u value=%hu")
    def set_pwm(self, print_time, value):
        clock = self._mcu.print_time_to_clock(print_time)
        if self._invert:
            value = 1. - value
        value = int(max(0., min(1., value)) * self._pwm_max + 0.5)
        self._replicape.note_pwm_enable(print_time, self._channel, value)
        msg = self._set_cmd.encode(self._oid, clock, value)
        self._mcu.send(msg, minclock=self._last_clock, reqclock=clock
                      , cq=self._cmd_queue)
        self._last_clock = clock
    def set_digital(self, print_time, value):
        if value:
            self.set_pwm(print_time, 1.)
        else:
            self.set_pwm(print_time, 0.)

class ReplicapeDACEnable:
    def __init__(self, replicape, channel, pin_params):
        if pin_params['type'] != 'digital_out':
            raise pins.error("Replicape virtual enable pin must be digital_out")
        if pin_params['invert']:
            raise pins.error("Replicape virtual enable pin can not be inverted")
        self.mcu = replicape.host_mcu
        self.value = replicape.stepper_dacs[channel]
        self.pwm = pca9685_pwm(replicape, channel, pin_params)
    def get_mcu(self):
        return self.mcu
    def setup_max_duration(self, max_duration):
        self.pwm.setup_max_duration(max_duration)
    def set_digital(self, print_time, value):
        if value:
            self.pwm.set_pwm(print_time, self.value)
        else:
            self.pwm.set_pwm(print_time, 0.)

ReplicapeStepConfig = {
    'disable': None,
    '1': (1<<7)|(1<<5), '2': (1<<7)|(1<<5)|(1<<6), 'spread2': (1<<5),
    '4': (1<<7)|(1<<5)|(1<<4), '16': (1<<7)|(1<<5)|(1<<6)|(1<<4),
    'spread4': (1<<5)|(1<<4), 'spread16': (1<<7), 'stealth4': (1<<7)|(1<<6),
    'stealth16': 0
}

class Replicape:
    def __init__(self, printer, config):
        pins.get_printer_pins(printer).register_chip('replicape', self)
        revisions = {'B3': 'B3'}
        config.getchoice('revision', revisions)
        self.host_mcu = mcu.get_printer_mcu(printer, config.get('host_mcu'))
        # Setup enable pin
        self.mcu_pwm_enable = pins.setup_pin(
            printer, 'digital_out', config.get('enable_pin', '!P9_41'))
        self.mcu_pwm_enable.setup_max_duration(0.)
        self.mcu_pwm_start_value = self.mcu_pwm_shutdown_value = False
        # Setup power pins
        self.pins = {
            "power_e": (pca9685_pwm, 5), "power_h": (pca9685_pwm, 3),
            "power_hotbed": (pca9685_pwm, 4),
            "power_fan0": (pca9685_pwm, 7), "power_fan1": (pca9685_pwm, 8),
            "power_fan2": (pca9685_pwm, 9), "power_fan3": (pca9685_pwm, 10) }
        # Setup stepper config
        self.stepper_dacs = {}
        shift_registers = [1, 0, 0, 1, 1]
        for port, name in enumerate('xyzeh'):
            prefix = 'stepper_%s_' % (name,)
            sc = config.getchoice(
                prefix + 'microstep_mode', ReplicapeStepConfig, 'disable')
            if sc is None:
                continue
            sc |= shift_registers[port]
            if config.getboolean(prefix + 'chopper_off_time_high', False):
                sc |= 1<<3
            if config.getboolean(prefix + 'chopper_hysteresis_high', False):
                sc |= 1<<2
            if config.getboolean(prefix + 'chopper_blank_time_high', True):
                sc |= 1<<1
            shift_registers[port] = sc
            channel = port + 11
            cur = config.getfloat(
                prefix + 'current', above=0., maxval=REPLICAPE_MAX_CURRENT)
            self.stepper_dacs[channel] = cur / REPLICAPE_MAX_CURRENT
            self.pins[prefix + 'enable'] = (ReplicapeDACEnable, channel)
        self.enabled_channels = {ch: False for cl, ch in self.pins.values()}
        self.disable_stepper_cmd = "send_spi bus=%d dev=%d msg=%s" % (
            REPLICAPE_SHIFT_REGISTER_BUS, REPLICAPE_SHIFT_REGISTER_DEVICE,
            "".join(["%02x" % (x,) for x in reversed(shift_registers)]))
        if [i for i in [0, 1, 2] if 11+i in self.stepper_dacs]:
            # Enable xyz steppers
            shift_registers[0] &= ~1
        if [i for i in [3, 4] if 11+i in self.stepper_dacs]:
            # Enable eh steppers
            shift_registers[3] &= ~1
        if (config.getboolean('standstill_power_down', False)
            and self.stepper_dacs):
            shift_registers[4] &= ~1
        self.enable_stepper_cmd = "send_spi bus=%d dev=%d msg=%s" % (
            REPLICAPE_SHIFT_REGISTER_BUS, REPLICAPE_SHIFT_REGISTER_DEVICE,
            "".join(["%02x" % (x,) for x in reversed(shift_registers)]))
        self.host_mcu.add_config_cmd(self.disable_stepper_cmd)
        self.last_stepper_time = 0.
    def note_pwm_start_value(self, channel, start_value, shutdown_value):
        self.mcu_pwm_start_value |= not not start_value
        self.mcu_pwm_shutdown_value |= not not shutdown_value
        self.mcu_pwm_enable.setup_start_value(
            self.mcu_pwm_start_value, self.mcu_pwm_shutdown_value)
        self.enabled_channels[channel] = not not start_value
    def note_pwm_enable(self, print_time, channel, value):
        is_enable = not not value
        if self.enabled_channels[channel] == is_enable:
            # Nothing to do
            return
        self.enabled_channels[channel] = is_enable
        # Check if need to set the pca9685 enable pin
        on_channels = [1 for c, e in self.enabled_channels.items() if e]
        if not on_channels:
            self.mcu_pwm_enable.set_digital(print_time, 0)
        elif is_enable and len(on_channels) == 1:
            self.mcu_pwm_enable.set_digital(print_time, 1)
        # Check if need to set the stepper enable lines
        if channel not in self.stepper_dacs:
            return
        on_dacs = [1 for c in self.stepper_dacs.keys()
                   if self.enabled_channels[c]]
        if not on_dacs:
            cmd = self.disable_stepper_cmd
        elif is_enable and len(on_dacs) == 1:
            cmd = self.enable_stepper_cmd
        else:
            return
        print_time = max(print_time, self.last_stepper_time + PIN_MIN_TIME)
        clock = self.host_mcu.print_time_to_clock(print_time)
        # XXX - the send_spi message should be scheduled
        self.host_mcu.send(self.host_mcu.create_command(cmd),
                           minclock=clock, reqclock=clock)
    def setup_pin(self, pin_params):
        pin = pin_params['pin']
        if pin not in self.pins:
            raise pins.error("Unknown replicape pin %s" % (pin,))
        pclass, channel = self.pins[pin]
        return pclass(self, channel, pin_params)


######################################################################
# Setup
######################################################################

def add_printer_objects(printer, config):
    if config.has_section('replicape'):
        printer.add_object('replicape', Replicape(
            printer, config.getsection('replicape')))
    for s in config.get_prefix_sections('static_digital_output '):
        printer.add_object(s.section, PrinterStaticDigitalOut(printer, s))
    for s in config.get_prefix_sections('digital_output '):
        printer.add_object('pin' + s.section[14:], PrinterPin(printer, s))
    for s in config.get_prefix_sections('static_pwm_output '):
        printer.add_object('pin' + s.section[17:], PrinterPin(printer, s))
    for s in config.get_prefix_sections('pwm_output '):
        printer.add_object('pin' + s.section[10:], PrinterPin(printer, s))
    for s in config.get_prefix_sections('multi_pin '):
        printer.add_object(s.section, PrinterMultiPin(printer, s))
    for s in config.get_prefix_sections('servo '):
        printer.add_object(s.section, PrinterServo(printer, s))
    for s in config.get_prefix_sections('ad5206 '):
        printer.add_object(s.section, ad5206(printer, s))
