# Palette 2 MMU support, Firmware 9.0.9 and newer supported only!
#
# Copyright (C) 2021 Clifford Roche <clifford.roche@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging
import os
import serial

from serial import SerialException

try:
    from queue import Queue, Empty
except ImportError:
    from Queue import Queue, Empty

COMMAND_HEARTBEAT = "O99"
COMMAND_CUT = "O10 D5"
COMMAND_CLEAR = [
    "O10 D5",
    "O10 D0 D0 D0 DFFE1",
    "O10 D1 D0 D0 DFFE1",
    "O10 D2 D0 D0 DFFE1",
    "O10 D3 D0 D0 DFFE1",
    "O10 D4 D0 D0 D0069"]
COMMAND_FILENAME = "O51"
COMMAND_FILENAMES_DONE = "O52"
COMMAND_FIRMWARE = "O50"
COMMAND_PING = "O31"
COMMAND_SMART_LOAD_STOP = "O102 D1"

HEARTBEAT_SEND = 5.
HEARTBEAT_TIMEOUT = (HEARTBEAT_SEND * 2.) + 1.
SETUP_TIMEOUT = 300

SERIAL_TIMER = 0.1
AUTOLOAD_TIMER = 5.

INFO_NOT_CONNECTED = "Palette 2 is not connected, connect first"


class Palette2:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        try:
            self.virtual_sdcard = self.printer.load_object(
                config, "virtual_sdcard")
        except config.error:
            raise self.printer.config_error(
                "Palette 2 requires [virtual_sdcard] to work,"
                " please add it to your config!")
        try:
            self.pause_resume = self.printer.load_object(
                config, "pause_resume")
        except config.error:
            raise self.printer.config_error(
                "Palette 2 requires [pause_resume] to work,"
                " please add it to your config!")
        self.gcode_move = self.printer.load_object(config, 'gcode_move')
        self.gcode = self.printer.lookup_object("gcode")
        self.gcode.register_command(
            "PALETTE_CONNECT", self.cmd_Connect, desc=self.cmd_Connect_Help)
        self.gcode.register_command(
            "PALETTE_DISCONNECT",
            self.cmd_Disconnect,
            desc=self.cmd_Disconnect_Help)
        self.gcode.register_command(
            "PALETTE_CLEAR", self.cmd_Clear, desc=self.cmd_Clear_Help)
        self.gcode.register_command(
            "PALETTE_CUT", self.cmd_Cut, desc=self.cmd_Cut_Help)
        self.gcode.register_command(
            "PALETTE_SMART_LOAD",
            self.cmd_Smart_Load,
            desc=self.cmd_Smart_Load_Help)
        self.serial = None
        self.serial_port = config.get("serial")
        if not self.serial_port:
            raise config.error("Invalid serial port specific for Palette 2")
        self.baud = config.getint("baud", default=250000)
        self.feedrate_splice = config.getfloat(
            "feedrate_splice", 0.8, minval=0., maxval=1.)
        self.feedrate_normal = config.getfloat(
            "feedrate_normal", 1.0, minval=0., maxval=1.)
        self.auto_load_speed = config.getint("auto_load_speed", 2)

        # Omega code matchers
        self.omega_header = [None] * 9
        omega_handlers = ["O" + str(i) for i in range(33)]
        for cmd in omega_handlers:
            func = getattr(self, 'cmd_' + cmd, None)
            desc = getattr(self, 'cmd_' + cmd + '_help', None)
            if func:
                self.gcode.register_command(cmd, func, desc=desc)
            else:
                self.gcode.register_command(cmd, self.cmd_OmegaDefault)

        self._reset()

        self.read_timer = None
        self.read_buffer = ""
        self.read_queue = Queue()
        self.write_timer = None
        self.write_queue = Queue()
        self.heartbeat_timer = None
        self.heartbeat = None
        self.signal_disconnect = False

        self.is_printing = False
        self.smart_load_timer = None

    def _reset(self):
        self.files = []
        self.is_setup_complete = False
        self.is_splicing = False
        self.is_loading = False
        self.remaining_load_length = None
        self.omega_algorithms = []
        self.omega_algorithms_counter = 0
        self.omega_splices = []
        self.omega_splices_counter = 0
        self.omega_pings = []
        self.omega_pongs = []
        self.omega_current_ping = None
        self.omega_header = [None] * 9
        self.omega_header_counter = 0
        self.omega_last_command = ""
        self.omega_drivers = []

    def _check_P2(self, gcmd=None):
        if self.serial:
            return True
        if gcmd:
            gcmd.respond_info(INFO_NOT_CONNECTED)
        return False

    cmd_Connect_Help = ("Connect to the Palette 2")

    def cmd_Connect(self, gcmd):
        if self.serial:
            gcmd.respond_info(
                "Palette 2 serial port is already active, disconnect first")
            return

        self.signal_disconnect = False
        logging.info("Connecting to Palette 2 on port (%s) at (%s)" %
                     (self.serial_port, self.baud))
        try:
            self.serial = serial.Serial(
                self.serial_port, self.baud, timeout=0, write_timeout=0)
        except SerialException:
            gcmd.respond_info("Unable to connect to the Palette 2")
            return

        with self.write_queue.mutex:
            self.write_queue.queue.clear()
        with self.read_queue.mutex:
            self.read_queue.queue.clear()

        self.read_timer = self.reactor.register_timer(
            self._run_Read, self.reactor.NOW)
        self.write_timer = self.reactor.register_timer(
            self._run_Write, self.reactor.NOW)
        self.heartbeat_timer = self.reactor.register_timer(
            self._run_Heartbeat, self.reactor.NOW)

        # Tell the device we're alive
        self.write_queue.put("\n")
        self.write_queue.put(COMMAND_FIRMWARE)

    cmd_Disconnect_Help = ("Disconnect from the Palette 2")

    def cmd_Disconnect(self, gmcd=None):
        logging.info("Disconnecting from Palette 2")
        if self.serial:
            self.serial.close()
            self.serial = None

        self.reactor.unregister_timer(self.read_timer)
        self.reactor.unregister_timer(self.write_timer)
        self.reactor.unregister_timer(self.heartbeat_timer)
        self.read_timer = None
        self.write_timer = None
        self.heartbeat = None
        self.is_printing = False

    cmd_Clear_Help = ("Clear the input and output of the Palette 2")

    def cmd_Clear(self, gcmd):
        logging.info("Clearing Palette 2 input and output")
        if self._check_P2(gcmd):
            for l in COMMAND_CLEAR:
                self.write_queue.put(l)

    cmd_Cut_Help = ("Cut the outgoing filament")

    def cmd_Cut(self, gcmd):
        logging.info("Cutting outgoing filament in Palette 2")
        if self._check_P2(gcmd):
            self.write_queue.put(COMMAND_CUT)

    cmd_Smart_Load_Help = ("Automatically load filament through the extruder")

    def cmd_Smart_Load(self, gcmd):
        if self._check_P2(gcmd):
            if not self.is_loading:
                gcmd.respond_info(
                    "Cannot auto load when the Palette 2 is not ready")
                return
            self.p2cmd_O102(params=None)

    def cmd_OmegaDefault(self, gcmd):
        logging.debug("Omega Code: %s" % (gcmd.get_command()))
        if self._check_P2(gcmd):
            self.write_queue.put(gcmd.get_commandline())

    cmd_O1_help = (
        "Initialize the print, and check connection with the Palette 2")

    def cmd_O1(self, gcmd):
        logging.info("Initializing print with Pallete 2")
        if not self._check_P2(gcmd):
            raise self.printer.command_error(
                "Cannot initialize print, palette 2 is not connected")

        startTs = self.reactor.monotonic()
        while self.heartbeat is None and self.heartbeat < (
                self.reactor.monotonic() -
                HEARTBEAT_TIMEOUT) and startTs > (
                self.reactor.monotonic() -
                HEARTBEAT_TIMEOUT):
            self.reactor.pause(1)

        if self.heartbeat < (self.reactor.monotonic() - HEARTBEAT_TIMEOUT):
            raise self.printer.command_error(
                "No response from Palette 2 when initializing")

        self.write_queue.put(gcmd.get_commandline())
        self.gcode.respond_info(
            "Palette 2 waiting on user to complete setup")
        self.pause_resume.send_pause_command()

    cmd_O9_help = ("Reset print information")

    def cmd_O9(self, gcmd):
        logging.info("Print finished, resetting Palette 2 state")
        if self._check_P2(gcmd):
            self.write_queue.put(gcmd.get_commandline())
        self.is_printing = False

    def cmd_O21(self, gcmd):
        logging.debug("Omega version: %s" % (gcmd.get_commandline()))
        self._reset()
        self.omega_header[0] = gcmd.get_commandline()
        self.is_printing = True

    def cmd_O22(self, gcmd):
        logging.debug("Omega printer profile: %s" % (gcmd.get_commandline()))
        self.omega_header[1] = gcmd.get_commandline()

    def cmd_O23(self, gcmd):
        logging.debug("Omega slicer profile: %s" % (gcmd.get_commandline()))
        self.omega_header[2] = gcmd.get_commandline()

    def cmd_O24(self, gcmd):
        logging.debug("Omega PPM: %s" % (gcmd.get_commandline()))
        self.omega_header[3] = gcmd.get_commandline()

    def cmd_O25(self, gcmd):
        logging.debug("Omega inputs: %s" % (gcmd.get_commandline()))
        self.omega_header[4] = gcmd.get_commandline()
        drives = self.omega_header[4][4:].split()
        for idx in range(len(drives)):
            state = drives[idx][:2]
            if state == "D1":
                drives[idx] = "U" + str(60 + idx)
        self.omega_drives = [d for d in drives if d != "D0"]
        logging.info("Omega drives: %s" % self.omega_drives)

    def cmd_O26(self, gcmd):
        logging.debug("Omega splices %s" % (gcmd.get_commandline()))
        self.omega_header[5] = gcmd.get_commandline()

    def cmd_O27(self, gcmd):
        logging.debug("Omega pings: %s" % (gcmd.get_commandline()))
        self.omega_header[6] = gcmd.get_commandline()

    def cmd_O28(self, gcmd):
        logging.debug("Omega MSF NA: %s" % (gcmd.get_commandline()))
        self.omega_header[7] = gcmd.get_commandline()

    def cmd_O29(self, gcmd):
        logging.debug("Omega MSF NH: %s" % (gcmd.get_commandline()))
        self.omega_header[8] = gcmd.get_commandline()

    def cmd_O30(self, gcmd):
        try:
            param_drive = gcmd.get_commandline()[5:6]
            param_distance = gcmd.get_commandline()[8:]
        except IndexError:
            gmcd.respond_info(
                "Incorrect number of arguments for splice command")
        try:
            self.omega_splices.append((int(param_drive), param_distance))
        except ValueError:
            gcmd.respond_info("Incorrectly formatted splice command")
        logging.debug("Omega splice command drive %s distance %s" %
                      (param_drive, param_distance))

    def cmd_O31(self, gcmd):
        if self._check_P2(gcmd):
            self.omega_current_ping = gcmd.get_commandline()
            logging.debug("Omega ping command: %s" %
                          (gcmd.get_commandline()))

            self.write_queue.put(COMMAND_PING)
            self.gcode.create_gcode_command("G4", "G4", {"P": "10"})

    def cmd_O32(self, gcmd):
        logging.debug("Omega algorithm: %s" % (gcmd.get_commandline()))
        self.omega_algorithms.append(gcmd.get_commandline())

    def p2cmd_O20(self, params):
        if not self.is_printing:
            return

        # First print, we can ignore
        if params[0] == "D5":
            logging.info("First print on Palette")
            return

        try:
            n = int(params[0][1:])
        except (TypeError, IndexError):
            logging.error("O20 command has invalid parameters")
            return

        if n == 0:
            logging.info("Sending omega header %s" % self.omega_header_counter)
            self.write_queue.put(self.omega_header[self.omega_header_counter])
            self.omega_header_counter = self.omega_header_counter + 1
        elif n == 1:
            logging.info("Sending splice info %s" % self.omega_splices_counter)
            splice = self.omega_splices[self.omega_splices_counter]
            self.write_queue.put("O30 D%d D%s" % (splice[0], splice[1]))
            self.omega_splices_counter = self.omega_splices_counter + 1
        elif n == 2:
            logging.info("Sending current ping info %s" %
                         self.omega_current_ping)
            self.write_queue.put(self.omega_current_ping)
        elif n == 4:
            logging.info("Sending algorithm info %s" %
                         self.omega_algorithms_counter)
            self.write_queue.put(
                self.omega_algorithms[self.omega_algorithms_counter])
            self.omega_algorithms_counter = self.omega_algorithms_counter + 1
        elif n == 8:
            logging.info("Resending the last command to Palette 2")
            self.write_queue.put(self.omega_last_command)

    def p2cmd_O34(self, params):
        if not self.is_printing:
            return

        if len(params) > 2:
            percent = float(params[1][1:])
            if params[0] == "D1":
                number = len(self.omega_pings) + 1
                d = {"number": number, "percent": percent}
                logging.info("Ping %d, %d percent" % (number, percent))
                self.omega_pings.append(d)
            elif params[0] == "D2":
                number = len(self.omega_pongs) + 1
                d = {"number": number, "percent": percent}
                logging.info("Pong %d, %d percent" % (number, percent))
                self.omega_pongs.append(d)

    def p2cmd_O40(self, params):
        logging.info("Resume request from Palette 2")
        self.pause_resume.send_resume_command()

    def p2cmd_O50(self, params):
        if len(params) > 1:
            try:
                fw = params[0][1:]
                logging.info(
                    "Palette 2 firmware version %s detected" % os.fwalk)
            except (TypeError, IndexError):
                logging.error("Unable to parse firmware version")

            if fw < "9.0.9":
                raise self.printer.command_error(
                    "Palette 2 firmware version is too old, "
                    "update to at least 9.0.9")
        else:
            self.files = [
                file for (
                    file,
                    size) in self.virtual_sdcard.get_file_list(
                    check_subdirs=True) if ".mcf.gcode" in file]
            for file in self.files:
                self.write_queue.put("%s D%s" % (COMMAND_FILENAME, file))
            self.write_queue.put(COMMAND_FILENAMES_DONE)

    def p2cmd_O53(self, params):
        if len(params) > 1 and params[0] == "D1":
            try:
                idx = int(params[1][1:], 16)
                file = self.files[::-1][idx]
                self.gcode.run_script("SDCARD_PRINT_FILE FILENAME=%s" % file)
            except (TypeError, IndexError):
                logging.error("O53 has invalid command parameters")

    def p2cmd_O88(self, params):
        logging.error("Palette 2 error detected")
        try:
            error = int(params[0][1:], 16)
            logging.error("Palette 2 error code %d" % error)
        except (TypeError, IndexError):
            logging.error("Unable to parse Palette 2 error")

    def p2cmd_O97(self, params):
        def printCancelling(params):
            logging.info("Print Cancelling")
            self.gcode.run_script("CLEAR_PAUSE")
            self.gcode.run_script("CANCEL_PRINT")

        def printCancelled(params):
            logging.info("Print Cancelled")
            self._reset()

        def loadingOffsetStart(params):
            logging.info("Waiting for user to load filament into printer")
            self.is_loading = True

        def loadingOffset(params):
            self.remaining_load_length = int(params[1][1:])
            logging.debug("Loading filamant remaining %d" %
                          self.remaining_load_length)
            if self.remaining_load_length >= 0 and self.smart_load_timer:
                logging.info("Smart load filament is complete")
                self.reactor.unregister_timer(self.smart_load_timer)
                self.smart_load_timer = None
                self.is_loading = False

        def feedrateStart(params):
            logging.info("Setting feedrate to %f for splice" %
                         self.feedrate_splice)
            self.is_splicing = True
            self.gcode.run_script("M220 S%d" % (self.feedrate_splice * 100))

        def feedrateEnd(params):
            logging.info("Setting feedrate to %f splice done" %
                         self.feedrate_normal)
            self.is_splicing = False
            self.gcode.run_script("M220 S%d" % (self.feedrate_normal * 100))

        matchers = []
        if self.is_printing:
            matchers = matchers + [
                [printCancelling, 2, "U0", "D2"],
                [printCancelled, 2, "U0", "D3"],
                [loadingOffset, 2, "U39"],
                [loadingOffsetStart, 1, "U39"],
            ]

        matchers.append([feedrateStart, 3, "U25", "D0"])
        matchers.append([feedrateEnd, 3, "U25", "D1"])
        self._param_Matcher(matchers, params)

    def p2cmd_O100(self, params):
        logging.info("Pause request from Palette 2")
        self.is_setup_complete = True
        self.pause_resume.send_pause_command()

    def p2cmd_O102(self, params):
        toolhead = self.printer.lookup_object("toolhead")
        if not toolhead.get_extruder().get_heater().can_extrude:
            self.write_queue.put(COMMAND_SMART_LOAD_STOP)
            self.gcode.respond_info(
                "Unable to auto load filament, extruder is below minimum temp")
            return

        if self.smart_load_timer is None:
            logging.info("Smart load starting")
            self.smart_load_timer = self.reactor.register_timer(
                self._run_Smart_Load, self.reactor.NOW)

    def p2cmd(self, line):
        t = line.split()
        ocode = t[0]
        params = t[1:]
        params_count = len(params)
        if params_count:
            res = [i for i in params if i[0] == "D" or i[0] == "U"]
            if not all(res):
                logging.error("Omega parameters are invalid")
                return

        func = getattr(self, 'p2cmd_' + ocode, None)
        if func is not None:
            func(params)

    def _param_Matcher(self, matchers, params):
        # Match the command with the handling table
        for matcher in matchers:
            if len(params) >= matcher[1]:
                match_params = matcher[2:]
                res = all([match_params[i] == params[i]
                           for i in range(len(match_params))])
                if res:
                    matcher[0](params)
                    return True
        return False

    def _run_Read(self, eventtime):
        if self.signal_disconnect:
            self.cmd_Disconnect()
            return self.reactor.NEVER

        # Do non-blocking reads from serial and try to find lines
        while True:
            try:
                raw_bytes = self.serial.read()
            except SerialException:
                logging.error("Unable to communicate with the Palette 2")
                self.cmd_Disconnect()
                return self.reactor.NEVER
            if len(raw_bytes):
                text_buffer = self.read_buffer + raw_bytes.decode()
                while True:
                    i = text_buffer.find("\n")
                    if i >= 0:
                        line = text_buffer[0:i+1]
                        self.read_queue.put(line.strip())
                        text_buffer = text_buffer[i+1:]
                    else:
                        break
                self.read_buffer = text_buffer
            else:
                break

        # Process any decoded lines from the device
        while not self.read_queue.empty():
            try:
                text_line = self.read_queue.get_nowait()
            except Empty:
                pass

            heartbeat_strings = [COMMAND_HEARTBEAT, "Connection Okay"]
            if not any(x in text_line for x in heartbeat_strings):
                logging.debug("%0.3f P2 -> : %s" %(eventtime, text_line))

            # Received a heartbeat from the device
            if text_line == COMMAND_HEARTBEAT:
                self.heartbeat = eventtime

            elif text_line[0] == "O":
                self.p2cmd(text_line)

        return eventtime + SERIAL_TIMER

    def _run_Heartbeat(self, eventtime):
        self.write_queue.put(COMMAND_HEARTBEAT)
        if self.heartbeat and self.heartbeat < (
                eventtime - HEARTBEAT_TIMEOUT):
            logging.error(
                "P2 has not responded to heartbeat, Palette will disconnect")
            self.cmd_Disconnect()
            return self.reactor.NEVER
        return eventtime + HEARTBEAT_SEND

    def _run_Write(self, eventtime):
        while not self.write_queue.empty():
            try:
                text_line = self.write_queue.get_nowait()
            except Empty:
                continue

            if text_line:
                self.omega_last_command = text_line
                l = text_line.strip()
                if COMMAND_HEARTBEAT not in l:
                    logging.debug(
                        "%s -> P2 : %s" %
                        (self.reactor.monotonic(), l))
                terminated_line = "%s\n" % (l)
                try:
                    self.serial.write(terminated_line.encode())
                except SerialException:
                    logging.error("Unable to communicate with the Palette 2")
                    self.signal_disconnect = True
                    return self.reactor.NEVER
        return eventtime + SERIAL_TIMER

    def _run_Smart_Load(self, eventtime):
        if not self.is_splicing and self.remaining_load_length < 0:
            # Make sure toolhead class isn't busy
            toolhead = self.printer.lookup_object("toolhead")
            print_time, est_print_time, lookahead_empty = toolhead.check_busy(
                eventtime)
            idle_time = est_print_time - print_time
            if not lookahead_empty or idle_time < 0.5:
                return eventtime + \
                    max(0., min(1., print_time - est_print_time))

            extrude = abs(self.remaining_load_length)
            extrude = min(50, extrude / 2)
            if extrude <= 10:
                extrude = 1
            logging.info("Smart loading %dmm filament with %dmm remaining" % (
                extrude, abs(self.remaining_load_length)))

            self.gcode.run_script("G92 E0")
            self.gcode.run_script("G1 E%d F%d" % (
                extrude, self.auto_load_speed * 60))
            return self.reactor.NOW
        return eventtime + AUTOLOAD_TIMER


def load_config(config):
    return Palette2(config)
