#!/usr/bin/env python
# Main code for host side printer firmware
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, optparse, ConfigParser, logging, time, threading
import gcode, toolhead, util, mcu, fan, heater, extruder, reactor

class ConfigWrapper:
    def __init__(self, printer, section):
        self.printer = printer
        self.section = section
    def get(self, option, default=None):
        if not self.printer.fileconfig.has_option(self.section, option):
            return default
        return self.printer.fileconfig.get(self.section, option)
    def getint(self, option, default=None):
        if not self.printer.fileconfig.has_option(self.section, option):
            return default
        return self.printer.fileconfig.getint(self.section, option)
    def getfloat(self, option, default=None):
        if not self.printer.fileconfig.has_option(self.section, option):
            return default
        return self.printer.fileconfig.getfloat(self.section, option)
    def getboolean(self, option, default=None):
        if not self.printer.fileconfig.has_option(self.section, option):
            return default
        return self.printer.fileconfig.getboolean(self.section, option)
    def getsection(self, section):
        return ConfigWrapper(self.printer, section)

class Printer:
    def __init__(self, conffile, debuginput=None):
        self.fileconfig = ConfigParser.RawConfigParser()
        self.fileconfig.read(conffile)
        self.reactor = reactor.Reactor()

        self._pconfig = ConfigWrapper(self, 'printer')
        ptty = self._pconfig.get('pseudo_tty', '/tmp/printer')
        if debuginput is None:
            pseudo_tty = util.create_pty(ptty)
        else:
            pseudo_tty = debuginput.fileno()

        self.gcode = gcode.GCodeParser(
            self, pseudo_tty, inputfile=debuginput is not None)
        self.mcu = None
        self.stat_timer = None

        self.objects = {}
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
            self, self._pconfig)

    def stats(self, eventtime):
        out = []
        out.append(self.gcode.stats(eventtime))
        out.append(self.objects['toolhead'].stats(eventtime))
        out.append(self.mcu.stats(eventtime))
        logging.info("Stats %.0f: %s" % (eventtime, ' '.join(out)))
        return eventtime + 1.
    def build_config(self):
        for oname in sorted(self.objects.keys()):
            self.objects[oname].build_config()
        self.gcode.build_config()
        self.mcu.build_config()
    def connect(self):
        self.mcu = mcu.MCU(self, ConfigWrapper(self, 'mcu'))
        self.mcu.connect()
        self.build_config()
        self.stats_timer = self.reactor.register_timer(
            self.stats, self.reactor.NOW)
    def connect_debug(self, debugoutput):
        self.mcu = mcu.DummyMCU(debugoutput)
        self.mcu.connect()
        self.build_config()
    def connect_file(self, output, dictionary):
        self.mcu = mcu.MCU(self, ConfigWrapper(self, 'mcu'))
        self.mcu.connect_file(output, dictionary)
        self.build_config()
    def run(self):
        self.gcode.run()
        # If gcode exits, then exit the MCU
        self.stats(time.time())
        self.mcu.disconnect()
        self.stats(time.time())
    def shutdown(self):
        self.gcode.shutdown()


######################################################################
# Startup
######################################################################

def read_dictionary(filename):
    dfile = open(filename, 'rb')
    dictionary = dfile.read()
    dfile.close()
    return dictionary

def store_dictionary(filename, printer):
    f = open(filename, 'wb')
    f.write(printer.mcu.serial.msgparser.raw_identify_data)
    f.close()

def main():
    usage = "%prog [options] <config file>"
    opts = optparse.OptionParser(usage)
    opts.add_option("-o", "--debugoutput", dest="outputfile",
                    help="write output to file instead of to serial port")
    opts.add_option("-i", "--debuginput", dest="inputfile",
                    help="read commands from file instead of from tty port")
    opts.add_option("-l", "--logfile", dest="logfile",
                    help="write log to file instead of stderr")
    opts.add_option("-v", action="store_true", dest="verbose",
                    help="enable debug messages")
    opts.add_option("-d", dest="read_dictionary",
                    help="file to read for mcu protocol dictionary")
    opts.add_option("-D", dest="write_dictionary",
                    help="file to write mcu protocol dictionary")
    options, args = opts.parse_args()
    if len(args) != 1:
        opts.error("Incorrect number of arguments")
    conffile = args[0]

    debuginput = debugoutput = None

    debuglevel = logging.INFO
    if options.verbose:
        debuglevel = logging.DEBUG
    if options.inputfile:
        debuginput = open(options.inputfile, 'rb')
    if options.outputfile:
        debugoutput = open(options.outputfile, 'wb')
    if options.logfile:
        logoutput = open(options.logfile, 'wb')
        logging.basicConfig(stream=logoutput, level=debuglevel)
    else:
        logging.basicConfig(level=debuglevel)
    logging.info("Starting Klippy...")

    # Start firmware
    printer = Printer(conffile, debuginput=debuginput)
    if debugoutput:
        proto_dict = read_dictionary(options.read_dictionary)
        printer.connect_file(debugoutput, proto_dict)
    else:
        printer.connect()
    if options.write_dictionary:
        store_dictionary(options.write_dictionary, printer)
    printer.run()

if __name__ == '__main__':
    main()
