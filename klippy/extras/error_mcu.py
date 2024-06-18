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

message_protocol_error1 = """
This is frequently caused by running an older version of the
firmware on the MCU(s). Fix by recompiling and flashing the
firmware.
"""

message_protocol_error2 = """
Once the underlying issue is corrected, use the "RESTART"
command to reload the config and restart the host software.
"""

message_mcu_connect_error = """
Once the underlying issue is corrected, use the
"FIRMWARE_RESTART" command to reset the firmware, reload the
config, and restart the host software.
Error configuring printer
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
        self.clarify_callbacks = {}
        self.printer.register_event_handler("klippy:notify_mcu_shutdown",
                                            self._handle_notify_mcu_shutdown)
        self.printer.register_event_handler("klippy:notify_mcu_error",
                                            self._handle_notify_mcu_error)
    def add_clarify(self, msg, callback):
        self.clarify_callbacks.setdefault(msg, []).append(callback)
    def _check_mcu_shutdown(self, msg, details):
        mcu_name = details['mcu']
        mcu_msg = details['reason']
        event_type = details['event_type']
        prefix = "MCU '%s' shutdown: " % (mcu_name,)
        if event_type == 'is_shutdown':
            prefix = "Previous MCU '%s' shutdown: " % (mcu_name,)
        # Lookup generic hint
        hint = error_hint(mcu_msg)
        # Add per instance help
        clarify = [cb(msg, details)
                   for cb in self.clarify_callbacks.get(mcu_msg, [])]
        clarify = [cm for cm in clarify if cm is not None]
        clarify_msg = ""
        if clarify:
            clarify_msg = "\n".join(["", ""] + clarify + [""])
        # Update error message
        newmsg = "%s%s%s%s%s" % (prefix, mcu_msg, clarify_msg,
                                 hint, message_shutdown)
        self.printer.update_error_msg(msg, newmsg)
    def _handle_notify_mcu_shutdown(self, msg, details):
        if msg == "MCU shutdown":
            self._check_mcu_shutdown(msg, details)
        else:
            self.printer.update_error_msg(msg, "%s%s" % (msg, message_shutdown))
    def _check_protocol_error(self, msg, details):
        host_version = self.printer.start_args['software_version']
        msg_update = []
        msg_updated = []
        for mcu_name, mcu in self.printer.lookup_objects('mcu'):
            try:
                mcu_version = mcu.get_status()['mcu_version']
            except:
                logging.exception("Unable to retrieve mcu_version from mcu")
                continue
            if mcu_version != host_version:
                msg_update.append("%s: Current version %s"
                                  % (mcu_name.split()[-1], mcu_version))
            else:
                msg_updated.append("%s: Current version %s"
                                   % (mcu_name.split()[-1], mcu_version))
        if not msg_update:
            msg_update.append("<none>")
        if not msg_updated:
            msg_updated.append("<none>")
        newmsg = ["MCU Protocol error",
                  message_protocol_error1,
                  "Your Klipper version is: %s" % (host_version,),
                  "MCU(s) which should be updated:"]
        newmsg += msg_update + ["Up-to-date MCU(s):"] + msg_updated
        newmsg += [message_protocol_error2, details['error']]
        self.printer.update_error_msg(msg, "\n".join(newmsg))
    def _check_mcu_connect_error(self, msg, details):
        newmsg = "%s%s" % (details['error'], message_mcu_connect_error)
        self.printer.update_error_msg(msg, newmsg)
    def _handle_notify_mcu_error(self, msg, details):
        if msg == "Protocol error":
            self._check_protocol_error(msg, details)
        elif msg == "MCU error during connect":
            self._check_mcu_connect_error(msg, details)

def load_config(config):
    return PrinterMCUError(config)
