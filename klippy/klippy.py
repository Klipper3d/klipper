#!/usr/bin/env python
# Main code for host side printer firmware
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, optparse, ConfigParser, logging, time, threading
import gcode, toolhead, util, mcu, fan, heater, extruder, reactor, queuelogger

message_startup = """
The klippy host software is attempting to connect.  Please
retry in a few moments.
Printer is not ready
"""

message_restart = """
This is an unrecoverable error.  Please correct the
underlying issue and then manually restart the klippy host
software.
Printer is halted
"""

message_mcu_connect_error = """
This is an unrecoverable error.  Please manually restart
both the firmware and the host software.
Error configuring printer
"""

message_shutdown = """
This is an unrecoverable error.  Please correct the
underlying issue and then manually restart both the
firmware and the host software.
Printer is shutdown
"""

class ConfigWrapper:
    error = ConfigParser.Error
    class sentinel:
        pass
    def __init__(self, printer, section):
        self.printer = printer
        self.section = section
    def get_wrapper(self, parser, option, default):
        if (default is not self.sentinel
            and not self.printer.fileconfig.has_option(self.section, option)):
            return default
        self.printer.all_config_options[
            (self.section.lower(), option.lower())] = 1
        try:
            return parser(self.section, option)
        except self.error, e:
            raise
        except:
            raise self.error("Unable to parse option '%s' in section '%s'" % (
                option, self.section))
    def get(self, option, default=sentinel):
        return self.get_wrapper(self.printer.fileconfig.get, option, default)
    def getint(self, option, default=sentinel):
        return self.get_wrapper(self.printer.fileconfig.getint, option, default)
    def getfloat(self, option, default=sentinel):
        return self.get_wrapper(
            self.printer.fileconfig.getfloat, option, default)
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

class Printer:
    def __init__(self, conffile, input_fd, is_fileinput=False):
        self.conffile = conffile
        self.reactor = reactor.Reactor()
        self.gcode = gcode.GCodeParser(self, input_fd, is_fileinput)
        self.stats_timer = self.reactor.register_timer(self.stats)
        self.connect_timer = self.reactor.register_timer(
            self.connect, self.reactor.NOW)
        self.all_config_options = {}
        self.state_message = message_startup
        self.debugoutput = self.dictionary = None
        self.fileconfig = None
        self.mcu = None
        self.objects = {}
    def set_fileoutput(self, debugoutput, dictionary):
        self.debugoutput = debugoutput
        self.dictionary = dictionary
    def stats(self, eventtime):
        out = []
        out.append(self.gcode.stats(eventtime))
        toolhead = self.objects.get('toolhead')
        out.append(toolhead.stats(eventtime))
        out.append(self.mcu.stats(eventtime))
        logging.info("Stats %.0f: %s" % (eventtime, ' '.join(out)))
        return eventtime + 1.
    def load_config(self):
        self.fileconfig = ConfigParser.RawConfigParser()
        res = self.fileconfig.read(self.conffile)
        if not res:
            raise ConfigParser.Error("Unable to open config file %s" % (
                self.conffile,))
        self.mcu = mcu.MCU(self, ConfigWrapper(self, 'mcu'))
        if self.fileconfig.has_section('fan'):
            self.objects['fan'] = fan.PrinterFan(
                self, ConfigWrapper(self, 'fan'))
        if self.fileconfig.has_section('extruder'):
            self.objects['extruder'] = extruder.PrinterExtruder(
                self, ConfigWrapper(self, 'extruder'))
        if self.fileconfig.has_section('heater_bed'):
            self.objects['heater_bed'] = heater.PrinterHeater(
                self, ConfigWrapper(self, 'heater_bed'))
        self.objects['toolhead'] = toolhead.ToolHead(
            self, ConfigWrapper(self, 'printer'))
    def build_config(self):
        for oname in sorted(self.objects.keys()):
            self.objects[oname].build_config()
        self.gcode.build_config()
        self.mcu.build_config()
    def validate_config(self):
        valid_sections = dict([(s, 1) for s, o in self.all_config_options])
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
            else:
                self.mcu.connect_file(self.debugoutput, self.dictionary)
            self.mcu.connect()
            self.build_config()
            self.validate_config()
            self.gcode.set_printer_ready(True)
            self.state_message = "Running"
        except ConfigParser.Error, e:
            logging.exception("Config error")
            self.state_message = "%s%s" % (str(e), message_restart)
            self.reactor.update_timer(self.stats_timer, self.reactor.NEVER)
        except mcu.error, e:
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
        try:
            self.reactor.run()
        except:
            logging.exception("Unhandled exception during run")
            return
        # If gcode exits, then exit the MCU
        self.stats(time.time())
        self.mcu.disconnect()
        self.stats(time.time())
    def get_state_message(self):
        return self.state_message
    def note_shutdown(self, msg):
        self.state_message = "Firmware shutdown: %s%s" % (
            msg, message_shutdown)
        self.gcode.set_printer_ready(False)


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

    # Start firmware
    printer = Printer(conffile, input_fd, is_fileinput=debuginput is not None)
    if debugoutput:
        proto_dict = read_dictionary(options.read_dictionary)
        printer.set_fileoutput(debugoutput, proto_dict)
    printer.run()

    if bglogger is not None:
        bglogger.stop()

if __name__ == '__main__':
    main()
