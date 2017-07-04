#!/usr/bin/env python2
# Main code for host side printer firmware
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, optparse, ConfigParser, logging, time, threading
import gcode, toolhead, util, mcu, fan, heater, extruder, reactor, queuelogger
import msgproto

message_ready = "Printer is ready"

message_startup = """
The klippy host software is attempting to connect.  Please
retry in a few moments.
Printer is not ready
"""

message_restart = """
Once the underlying issue is corrected, use the "RESTART"
command to reload the config and restart the host software.
Printer is halted
"""

message_protocol_error = """
This type of error is frequently caused by running an older
version of the firmware on the micro-controller (fix by
recompiling and flashing the firmware).
Once the underlying issue is corrected, use the "RESTART"
command to reload the config and restart the host software.
Protocol error connecting to printer
"""

message_mcu_connect_error = """
Once the underlying issue is corrected, use the
"FIRMWARE_RESTART" command to reset the firmware, reload the
config, and restart the host software.
Error configuring printer
"""

message_shutdown = """
Once the underlying issue is corrected, use the
"FIRMWARE_RESTART" command to reset the firmware, reload the
config, and restart the host software.
Printer is shutdown
"""

class ConfigWrapper:
    error = ConfigParser.Error
    class sentinel:
        pass
    def __init__(self, printer, section):
        self.printer = printer
        self.section = section
    def get_wrapper(self, parser, option, default
                    , minval=None, maxval=None, above=None, below=None):
        if (default is not self.sentinel
            and not self.printer.fileconfig.has_option(self.section, option)):
            return default
        self.printer.all_config_options[
            (self.section.lower(), option.lower())] = 1
        try:
            v = parser(self.section, option)
        except self.error as e:
            raise
        except:
            raise self.error("Unable to parse option '%s' in section '%s'" % (
                option, self.section))
        if minval is not None and v < minval:
            raise self.error(
                "Option '%s' in section '%s' must have minimum of %s" % (
                    option, self.section, minval))
        if maxval is not None and v > maxval:
            raise self.error(
                "Option '%s' in section '%s' must have maximum of %s" % (
                    option, self.section, maxval))
        if above is not None and v <= above:
            raise self.error(
                "Option '%s' in section '%s' must be above %s" % (
                    option, self.section, above))
        if below is not None and v >= below:
            raise self.error(
                "Option '%s' in section '%s' must be below %s" % (
                    option, self.section, below))
        return v
    def get(self, option, default=sentinel):
        return self.get_wrapper(self.printer.fileconfig.get, option, default)
    def getint(self, option, default=sentinel, minval=None, maxval=None):
        return self.get_wrapper(
            self.printer.fileconfig.getint, option, default, minval, maxval)
    def getfloat(self, option, default=sentinel
                 , minval=None, maxval=None, above=None, below=None):
        return self.get_wrapper(
            self.printer.fileconfig.getfloat, option, default
            , minval, maxval, above, below)
    def getboolean(self, option, default=sentinel):
        return self.get_wrapper(
            self.printer.fileconfig.getboolean, option, default)
    def getchoice(self, option, choices, default=sentinel):
        c = self.get(option, default)
        if c not in choices:
            raise self.error(
                "Option '%s' in section '%s' is not a valid choice" % (
                    option, self.section))
        return choices[c]
    def getsection(self, section):
        return ConfigWrapper(self.printer, section)
    def has_section(self, section):
        return self.printer.fileconfig.has_section(section)
    def get_prefix_sections(self, prefix):
        return [self.getsection(s) for s in self.printer.fileconfig.sections()
                if s.startswith(prefix)]

class ConfigLogger():
    def __init__(self, cfg, bglogger):
        self.lines = ["===== Config file ====="]
        cfg.write(self)
        self.lines.append("=======================")
        data = "\n".join(self.lines)
        logging.info(data)
        bglogger.set_rollover_info("config", data)
    def write(self, data):
        self.lines.append(data.strip())

class Printer:
    def __init__(self, conffile, input_fd, startup_state
                 , is_fileinput=False, version="?", bglogger=None):
        self.conffile = conffile
        self.startup_state = startup_state
        self.software_version = version
        self.bglogger = bglogger
        if bglogger is not None:
            bglogger.set_rollover_info("config", None)
        self.reactor = reactor.Reactor()
        self.objects = {}
        self.gcode = gcode.GCodeParser(self, input_fd, is_fileinput)
        self.stats_timer = self.reactor.register_timer(self.stats)
        self.connect_timer = self.reactor.register_timer(
            self.connect, self.reactor.NOW)
        self.all_config_options = {}
        self.need_dump_debug = False
        self.state_message = message_startup
        self.debugoutput = self.dictionary = None
        self.run_result = None
        self.fileconfig = None
        self.mcu = None
    def set_fileoutput(self, debugoutput, dictionary):
        self.debugoutput = debugoutput
        self.dictionary = dictionary
    def stats(self, eventtime, force_output=False):
        if self.need_dump_debug:
            # Call dump_debug here so it is executed in the main thread
            self.gcode.dump_debug()
            self.need_dump_debug = False
        toolhead = self.objects.get('toolhead')
        if toolhead is None or self.mcu is None:
            return eventtime + 1.
        is_active, thstats = toolhead.stats(eventtime)
        if not is_active and not force_output:
            return eventtime + 1.
        out = []
        out.append(self.gcode.stats(eventtime))
        out.append(thstats)
        out.append(self.mcu.stats(eventtime))
        logging.info("Stats %.1f: %s" % (eventtime, ' '.join(out)))
        return eventtime + 1.
    def add_object(self, name, obj):
        self.objects[name] = obj
    def load_config(self):
        self.fileconfig = ConfigParser.RawConfigParser()
        res = self.fileconfig.read(self.conffile)
        if not res:
            raise ConfigParser.Error("Unable to open config file %s" % (
                self.conffile,))
        if self.bglogger is not None:
            ConfigLogger(self.fileconfig, self.bglogger)
        self.mcu = mcu.MCU(self, ConfigWrapper(self, 'mcu'))
        if self.debugoutput is not None:
            self.mcu.connect_file(self.debugoutput, self.dictionary)
        # Create printer components
        config = ConfigWrapper(self, 'printer')
        for m in [extruder, fan, heater, toolhead]:
            m.add_printer_objects(self, config)
        # Validate that there are no undefined parameters in the config file
        valid_sections = { s: 1 for s, o in self.all_config_options }
        for section in self.fileconfig.sections():
            section = section.lower()
            if section not in valid_sections:
                raise ConfigParser.Error("Unknown config file section '%s'" % (
                    section,))
            for option in self.fileconfig.options(section):
                option = option.lower()
                if (section, option) not in self.all_config_options:
                    raise ConfigParser.Error(
                        "Unknown option '%s' in section '%s'" % (
                            option, section))
    def connect(self, eventtime):
        try:
            self.load_config()
            if self.debugoutput is None:
                self.reactor.update_timer(self.stats_timer, self.reactor.NOW)
            self.mcu.connect()
            self.gcode.set_printer_ready(True)
            self.state_message = message_ready
        except ConfigParser.Error as e:
            logging.exception("Config error")
            self.state_message = "%s%s" % (str(e), message_restart)
            self.reactor.update_timer(self.stats_timer, self.reactor.NEVER)
        except msgproto.error as e:
            logging.exception("Protocol error")
            self.state_message = "%s%s" % (str(e), message_protocol_error)
            self.reactor.update_timer(self.stats_timer, self.reactor.NEVER)
        except mcu.error as e:
            logging.exception("MCU error during connect")
            self.state_message = "%s%s" % (str(e), message_mcu_connect_error)
            self.reactor.update_timer(self.stats_timer, self.reactor.NEVER)
        except:
            logging.exception("Unhandled exception during connect")
            self.state_message = "Internal error during connect.%s" % (
                message_restart)
            self.reactor.update_timer(self.stats_timer, self.reactor.NEVER)
        self.reactor.unregister_timer(self.connect_timer)
        return self.reactor.NEVER
    def run(self):
        systime = time.time()
        monotime = self.reactor.monotonic()
        logging.info("Start printer at %s (%.1f %.1f)" % (
            time.asctime(time.localtime(systime)), systime, monotime))
        try:
            self.reactor.run()
        except:
            logging.exception("Unhandled exception during run")
            return
        return self.run_result
    def get_state_message(self):
        return self.state_message
    def note_shutdown(self, msg):
        if self.state_message == message_ready:
            self.need_dump_debug = True
        self.state_message = "Firmware shutdown: %s%s" % (
            msg, message_shutdown)
        self.gcode.set_printer_ready(False)
    def note_mcu_error(self, msg):
        self.state_message = "%s%s" % (msg, message_restart)
        self.gcode.set_printer_ready(False)
        self.gcode.motor_heater_off()
    def disconnect(self):
        try:
            if self.mcu is not None:
                self.stats(self.reactor.monotonic(), force_output=True)
                self.mcu.disconnect()
        except:
            logging.exception("Unhandled exception during disconnect")
    def firmware_restart(self):
        try:
            if self.mcu is not None:
                self.stats(self.reactor.monotonic(), force_output=True)
                self.mcu.microcontroller_restart()
                self.mcu.disconnect()
        except:
            logging.exception("Unhandled exception during firmware_restart")
    def get_startup_state(self):
        return self.startup_state
    def request_exit(self, result="exit"):
        self.run_result = result
        self.reactor.end()


######################################################################
# Startup
######################################################################

def read_dictionary(filename):
    dfile = open(filename, 'rb')
    dictionary = dfile.read()
    dfile.close()
    return dictionary

def main():
    usage = "%prog [options] <config file>"
    opts = optparse.OptionParser(usage)
    opts.add_option("-o", "--debugoutput", dest="outputfile",
                    help="write output to file instead of to serial port")
    opts.add_option("-i", "--debuginput", dest="inputfile",
                    help="read commands from file instead of from tty port")
    opts.add_option("-I", "--input-tty", dest="inputtty", default='/tmp/printer',
                    help="input tty name (default is /tmp/printer)")
    opts.add_option("-l", "--logfile", dest="logfile",
                    help="write log to file instead of stderr")
    opts.add_option("-v", action="store_true", dest="verbose",
                    help="enable debug messages")
    opts.add_option("-d", dest="read_dictionary",
                    help="file to read for mcu protocol dictionary")
    options, args = opts.parse_args()
    if len(args) != 1:
        opts.error("Incorrect number of arguments")
    conffile = args[0]

    input_fd = debuginput = debugoutput = bglogger = None

    debuglevel = logging.INFO
    if options.verbose:
        debuglevel = logging.DEBUG
    if options.inputfile:
        debuginput = open(options.inputfile, 'rb')
        input_fd = debuginput.fileno()
    else:
        input_fd = util.create_pty(options.inputtty)
    if options.outputfile:
        debugoutput = open(options.outputfile, 'wb')
    if options.logfile:
        bglogger = queuelogger.setup_bg_logging(options.logfile, debuglevel)
    else:
        logging.basicConfig(level=debuglevel)
    logging.info("Starting Klippy...")
    software_version = util.get_git_version()
    if bglogger is not None:
        lines = ["Args: %s" % (sys.argv,),
                 "Git version: %s" % (repr(software_version),),
                 "CPU: %s" % (util.get_cpu_info(),),
                 "Python: %s" % (repr(sys.version),)]
        lines = "\n".join(lines)
        logging.info(lines)
        bglogger.set_rollover_info('versions', lines)

    # Start firmware
    res = 'startup'
    while 1:
        is_fileinput = debuginput is not None
        printer = Printer(
            conffile, input_fd, res, is_fileinput, software_version, bglogger)
        if debugoutput:
            proto_dict = read_dictionary(options.read_dictionary)
            printer.set_fileoutput(debugoutput, proto_dict)
        res = printer.run()
        if res == 'restart':
            printer.disconnect()
            time.sleep(1.)
            logging.info("Restarting printer")
            continue
        elif res == 'firmware_restart':
            printer.firmware_restart()
            time.sleep(1.)
            logging.info("Restarting printer")
            continue
        elif res == 'exit_eof':
            printer.disconnect()
        break

    if bglogger is not None:
        bglogger.stop()

if __name__ == '__main__':
    main()
