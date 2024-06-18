# Support for MKS Servo42D drivers over CANBUS.
#
# Copyright (C) 2024 Maya Matuszczyk <maccraft123mc@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import can;

class MksServo42D:
    def __init__(self, config):
        hold_ratios = ['10%', '20%', '30%', '40%', '50%', '60%', '70%', '80%',
                '90%', None]
        operating_modes = ['open', 'close', 'vfoc']
        self.printer = config.get_printer()
        self.id = config.getint("canbus_nodeid")
        self.current = config.getfloat("run_current", minval=0., maxval=1.6)
        self.hold_ratio = config.get('hold_ratio', None, hold_ratios)
        self.operating_mode = config.get('operating_mode', operating_modes)
        self.interpolate = config.getboolean("interpolate", False)
        self.stepper_name = ' '.join(config.get_name().split()[1:])
        stepper = config.getsection(self.stepper_name)
        self.microsteps = stepper.getint('microsteps')

        if self.hold_ratio is not None and self.operating_mode == "vfoc":
            raise config.error("Cannot set hold ratio when in vfoc mode")

        filters = [{"can_id": self.id, "can_mask": 0x7ff, "extended": False}]
        canbus_iface = config.get("canbus_interface", "can0")
        self.bus = can.interface.Bus(channel=canbus_iface,
                                     can_filters=filters,
                                     bustype='socketcan')

        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)

    def verify_status(self, cmd):
        try:
            recv = self.bus.recv(timeout=5)
            if recv is None:
                raise self.printer.command_error("No reply from Servo42D")
            assert recv.dlc == 3
            calculated_crc = (self.id + recv.data[0] + recv.data[1]) & 0xff
            assert calculated_crc== recv.data[2]
            assert recv.data[0] == cmd
            assert recv.data[1] == 1

        except AssertionError:
            raise self.printer.command_error("Invalid reply from Servo42D")

    def _set_and_verify_status(self, cmd, value):
        crc = (self.id + cmd + value) & 0xff
        msg = can.Message(
                arbitration_id=self.id,
                data=[cmd, value, crc],
                is_extended_id=False
                )
        self.bus.send(msg)
        self.verify_status(cmd)

    def send_working_current(self, current):
        hi = (current >> 8) & 0xff
        lo = current & 0xff
        crc = (self.id + 0x83 + hi + lo) & 0xff
        msg = can.Message(
                arbitration_id=self.id,
                data=[0x83, hi, lo, crc],
                is_extended_id=False
                )
        self.bus.send(msg)
        self.verify_status(0x83)

    def _send_config(self):
        if self.operating_mode == 'open':
            self._set_and_verify_status(0x82, 0x00)
        elif self.operating_mode == 'close':
            self._set_and_verify_status(0x82, 0x01)
        elif self.operating_mode == 'vfoc':
            self._set_and_verify_status(0x82, 0x02)
        else:
            raise self.printer.command_error("Entered unreachable code")

        self._set_and_verify_status(0x84, self.microsteps)
        self._set_and_verify_status(0x89, self.interpolate)
        self.send_working_current(int(self.current * 1000))
        if self.hold_ratio == '10%':
            self._set_and_verify_status(0x9b, 0x00)
        elif self.hold_ratio == '20%':
            self._set_and_verify_status(0x9b, 0x01)
        elif self.hold_ratio == '30%':
            self._set_and_verify_status(0x9b, 0x02)
        elif self.hold_ratio == '40%':
            self._set_and_verify_status(0x9b, 0x03)
        elif self.hold_ratio == '50%':
            self._set_and_verify_status(0x9b, 0x04)
        elif self.hold_ratio == '60%':
            self._set_and_verify_status(0x9b, 0x05)
        elif self.hold_ratio == '70%':
            self._set_and_verify_status(0x9b, 0x06)
        elif self.hold_ratio == '80%':
            self._set_and_verify_status(0x9b, 0x07)
        elif self.hold_ratio == '90%':
            self._set_and_verify_status(0x9b, 0x08)
        elif self.hold_ratio is None and self.operating_mode == 'vfoc':
            pass
        else:
            raise self.printer.command_error("Entered unreachable code")

    def handle_connect(self):
        self._send_config()

def load_config_prefix(config):
    return MksServo42D(config)
