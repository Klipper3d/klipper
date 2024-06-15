# More verbose information on micro-controller errors
#
# Copyright (C) 2024  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

message_shutdown = """
Once the underlying issue is corrected, use the
"FIRMWARE_RESTART" command to reset the firmware, reload the
config, and restart the host software.
Printer is shutdown
"""

Common_MCU_errors = {
    ("Timer too close",): """
This often indicates the host computer is overloaded. Check
for other processes consuming excessive CPU time, high swap
usage, disk errors, overheating, unstable voltage, or
similar system problems on the host computer.""",
    ("Missed scheduling of next ",): """
This is generally indicative of an intermittent
communication failure between micro-controller and host.""",
    ("ADC out of range",): """
This generally occurs when a heater temperature exceeds
its configured min_temp or max_temp.""",
    ("Rescheduled timer in the past", "Stepper too far in past"): """
This generally occurs when the micro-controller has been
requested to step at a rate higher than it is capable of
obtaining.""",
    ("Command request",): """
This generally occurs in response to an M112 G-Code command
or in response to an internal error in the host software.""",
}

def error_hint(msg):
    for prefixes, help_msg in Common_MCU_errors.items():
        for prefix in prefixes:
            if msg.startswith(prefix):
                return help_msg
    return ""

class PrinterMCUError:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:notify_mcu_shutdown",
                                            self._handle_notify_mcu_shutdown)
    def _check_mcu_shutdown(self, msg, details):
        mcu_name = details['mcu']
        mcu_msg = details['reason']
        event_type = details['event_type']
        prefix = "MCU '%s' shutdown: " % (mcu_name,)
        if event_type == 'is_shutdown':
            prefix = "Previous MCU '%s' shutdown: " % (mcu_name,)
        # Lookup generic hint
        hint = error_hint(msg)
        # Update error message
        newmsg = "%s%s%s%s" % (prefix, mcu_msg, hint, message_shutdown)
        self.printer.update_error_msg(msg, newmsg)
    def _handle_notify_mcu_shutdown(self, msg, details):
        if msg == "MCU shutdown":
            self._check_mcu_shutdown(msg, details)
        else:
            self.printer.update_error_msg(msg, "%s%s" % (msg, message_shutdown))

def load_config(config):
    return PrinterMCUError(config)
