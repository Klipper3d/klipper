#!/usr/bin/env python2
# Main code for host side printer firmware
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, optparse, ConfigParser, logging, time, threading
import util, reactor, queuelogger, msgproto, gcode
import pins, mcu, chipmisc, toolhead, extruder, heater, fan

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
    def get_printer(self):
        return self.printer
    def get_name(self):
        return self.section
    def _get_wrapper(self, parser, option, default,
                     minval=None, maxval=None, above=None, below=None):
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
        return self._get_wrapper(self.printer.fileconfig.get, option, default)
    def getint(self, option, default=sentinel, minval=None, maxval=None):
        return self._get_wrapper(
            self.printer.fileconfig.getint, option, default, minval, maxval)
    def getfloat(self, option, default=sentinel
                 , minval=None, maxval=None, above=None, below=None):
        return self._get_wrapper(
            self.printer.fileconfig.getfloat, option, default
            , minval, maxval, above, below)
    def getboolean(self, option, default=sentinel):
        return self._get_wrapper(
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
    config_error = ConfigParser.Error
    def __init__(self, input_fd, bglogger, start_args):
        self.bglogger = bglogger
        self.start_args = start_args
        if bglogger is not None:
            bglogger.set_rollover_info("config", None)
        self.reactor = reactor.Reactor()
        self.gcode = gcode.GCodeParser(self, input_fd)
        self.objects = {'gcode': self.gcode}
        self.stats_timer = self.reactor.register_timer(self._stats)
        self.connect_timer = self.reactor.register_timer(
            self._connect, self.reactor.NOW)
        self.all_config_options = {}
        self.state_message = message_startup
        self.is_shutdown = False
        self.async_shutdown_msg = ""
        self.run_result = None
        self.fileconfig = None
        self.mcus = []
    def get_start_args(self):
        return self.start_args
    def get_reactor(self):
        return self.reactor
    def get_state_message(self):
        return self.state_message
    def add_object(self, name, obj):
        if obj in self.objects:
            raise self.config_error(
                "Printer object '%s' already created" % (name,))
        self.objects[name] = obj
    def lookup_object(self, name, default=ConfigWrapper.sentinel):
        if name in self.objects:
            return self.objects[name]
        if default is ConfigWrapper.sentinel:
            raise self.config_error("Unknown config object '%s'" % (name,))
        return default
    def lookup_module_objects(self, module_name):
        prefix = module_name + ' '
        objs = [self.objects[n]
                for n in sorted(self.objects) if n.startswith(prefix)]
        if module_name in self.objects:
            return [self.objects[module_name]] + objs
        return objs
    def set_rollover_info(self, name, info):
        if self.bglogger is not None:
            self.bglogger.set_rollover_info(name, info)
    def _stats(self, eventtime, force_output=False):
        toolhead = self.objects.get('toolhead')
        if toolhead is None:
            return eventtime + 1.
        is_active = toolhead.check_active(eventtime)
        if not is_active and not force_output:
            return eventtime + 1.
        out = []
        out.append(self.gcode.stats(eventtime))
        out.append(toolhead.stats(eventtime))
        for m in self.mcus:
            out.append(m.stats(eventtime))
        logging.info("Stats %.1f: %s", eventtime, ' '.join(out))
        return eventtime + 1.
    def _load_config(self):
        self.fileconfig = ConfigParser.RawConfigParser()
        config_file = self.start_args['config_file']
        res = self.fileconfig.read(config_file)
        if not res:
            raise self.config_error("Unable to open config file %s" % (
                config_file,))
        if self.bglogger is not None:
            ConfigLogger(self.fileconfig, self.bglogger)
        # Create printer components
        config = ConfigWrapper(self, 'printer')
        for m in [pins, mcu, chipmisc, toolhead, extruder, heater, fan]:
            m.add_printer_objects(self, config)
        self.mcus = self.lookup_module_objects('mcu')
        # Validate that there are no undefined parameters in the config file
        valid_sections = { s: 1 for s, o in self.all_config_options }
        for section in self.fileconfig.sections():
            section = section.lower()
            if section not in valid_sections:
                raise self.config_error("Unknown config file section '%s'" % (
                    section,))
            for option in self.fileconfig.options(section):
                option = option.lower()
                if (section, option) not in self.all_config_options:
                    raise self.config_error(
                        "Unknown option '%s' in section '%s'" % (
                            option, section))
    def _connect(self, eventtime):
        self.reactor.unregister_timer(self.connect_timer)
        try:
            self._load_config()
            for m in self.mcus:
                m.connect()
            self.gcode.connect()
            self.state_message = message_ready
            if self.start_args.get('debugoutput') is None:
                self.reactor.update_timer(self.stats_timer, self.reactor.NOW)
        except (self.config_error, pins.error) as e:
            logging.exception("Config error")
            self.state_message = "%s%s" % (str(e), message_restart)
        except msgproto.error as e:
            logging.exception("Protocol error")
            self.state_message = "%s%s" % (str(e), message_protocol_error)
        except mcu.error as e:
            logging.exception("MCU error during connect")
            self.state_message = "%s%s" % (str(e), message_mcu_connect_error)
        except:
            logging.exception("Unhandled exception during connect")
            self.state_message = "Internal error during connect.%s" % (
                message_restart,)
        return self.reactor.NEVER
    def run(self):
        systime = time.time()
        monotime = self.reactor.monotonic()
        logging.info("Start printer at %s (%.1f %.1f)",
                     time.asctime(time.localtime(systime)), systime, monotime)
        while 1:
            # Enter main reactor loop
            try:
                self.reactor.run()
            except:
                logging.exception("Unhandled exception during run")
                return "exit"
            # Check restart flags
            run_result = self.run_result
            try:
                if run_result == 'shutdown':
                    self.invoke_shutdown(self.async_shutdown_msg)
                    continue
                self._stats(self.reactor.monotonic(), force_output=True)
                for m in self.mcus:
                    if run_result == 'firmware_restart':
                        m.microcontroller_restart()
                    m.disconnect()
            except:
                logging.exception("Unhandled exception during post run")
            return run_result
    def invoke_shutdown(self, msg):
        if self.is_shutdown:
            return
        self.is_shutdown = True
        self.state_message = "%s%s" % (msg, message_shutdown)
        for m in self.mcus:
            m.do_shutdown()
        self.gcode.do_shutdown()
        toolhead = self.objects.get('toolhead')
        if toolhead is not None:
            toolhead.do_shutdown()
    def invoke_async_shutdown(self, msg):
        self.async_shutdown_msg = msg
        self.request_exit("shutdown")
    def request_exit(self, result="exit"):
        self.run_result = result
        self.reactor.end()


######################################################################
# Startup
######################################################################

def arg_dictionary(option, opt_str, value, parser):
    key, fname = "dictionary", value
    if '=' in value:
        mcu_name, fname = value.split('=', 1)
        key = "dictionary_" + mcu_name
    if parser.values.dictionary is None:
        parser.values.dictionary = {}
    parser.values.dictionary[key] = fname

def main():
    usage = "%prog [options] <config file>"
    opts = optparse.OptionParser(usage)
    opts.add_option("-i", "--debuginput", dest="debuginput",
                    help="read commands from file instead of from tty port")
    opts.add_option("-I", "--input-tty", dest="inputtty", default='/tmp/printer',
                    help="input tty name (default is /tmp/printer)")
    opts.add_option("-l", "--logfile", dest="logfile",
                    help="write log to file instead of stderr")
    opts.add_option("-v", action="store_true", dest="verbose",
                    help="enable debug messages")
    opts.add_option("-o", "--debugoutput", dest="debugoutput",
                    help="write output to file instead of to serial port")
    opts.add_option("-d", "--dictionary", dest="dictionary", type="string",
                    action="callback", callback=arg_dictionary,
                    help="file to read for mcu protocol dictionary")
    options, args = opts.parse_args()
    if len(args) != 1:
        opts.error("Incorrect number of arguments")
    start_args = {'config_file': args[0], 'start_reason': 'startup'}

    input_fd = bglogger = None

    debuglevel = logging.INFO
    if options.verbose:
        debuglevel = logging.DEBUG
    if options.debuginput:
        start_args['debuginput'] = options.debuginput
        debuginput = open(options.debuginput, 'rb')
        input_fd = debuginput.fileno()
    else:
        input_fd = util.create_pty(options.inputtty)
    if options.debugoutput:
        start_args['debugoutput'] = options.debugoutput
        start_args.update(options.dictionary)
    if options.logfile:
        bglogger = queuelogger.setup_bg_logging(options.logfile, debuglevel)
    else:
        logging.basicConfig(level=debuglevel)
    logging.info("Starting Klippy...")
    start_args['software_version'] = util.get_git_version()
    if bglogger is not None:
        lines = ["Args: %s" % (sys.argv,),
                 "Git version: %s" % (repr(start_args['software_version']),),
                 "CPU: %s" % (util.get_cpu_info(),),
                 "Python: %s" % (repr(sys.version),)]
        lines = "\n".join(lines)
        logging.info(lines)
        bglogger.set_rollover_info('versions', lines)

    # Start Printer() class
    while 1:
        printer = Printer(input_fd, bglogger, start_args)
        res = printer.run()
        if res == 'exit':
            break
        time.sleep(1.)
        logging.info("Restarting printer")
        start_args['start_reason'] = res

    if bglogger is not None:
        bglogger.stop()

if __name__ == '__main__':
    main()
