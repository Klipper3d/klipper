#!/usr/bin/env python2
# Main code for host side printer firmware
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, os, optparse, logging, time, threading
import collections, ConfigParser, importlib
import util, reactor, queuelogger, msgproto
import gcode, pins, heater, mcu, toolhead, extruder

message_ready = "Printer is ready"

message_startup = """
Printer is not ready
The klippy host software is attempting to connect.  Please
retry in a few moments.
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
    def __init__(self, printer, fileconfig, section):
        self.printer = printer
        self.fileconfig = fileconfig
        self.section = section
    def get_printer(self):
        return self.printer
    def get_name(self):
        return self.section
    def _get_wrapper(self, parser, option, default,
                     minval=None, maxval=None, above=None, below=None):
        if (default is not self.sentinel
            and not self.fileconfig.has_option(self.section, option)):
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
        return self._get_wrapper(self.fileconfig.get, option, default)
    def getint(self, option, default=sentinel, minval=None, maxval=None):
        return self._get_wrapper(
            self.fileconfig.getint, option, default, minval, maxval)
    def getfloat(self, option, default=sentinel,
                 minval=None, maxval=None, above=None, below=None):
        return self._get_wrapper(self.fileconfig.getfloat, option, default,
                                 minval, maxval, above, below)
    def getboolean(self, option, default=sentinel):
        return self._get_wrapper(self.fileconfig.getboolean, option, default)
    def getchoice(self, option, choices, default=sentinel):
        c = self.get(option, default)
        if c not in choices:
            raise self.error(
                "Choice '%s' for option '%s' in section '%s'"
                " is not a valid choice" % (c, option, self.section))
        return choices[c]
    def getsection(self, section):
        return ConfigWrapper(self.printer, self.fileconfig, section)
    def has_section(self, section):
        return self.fileconfig.has_section(section)
    def get_prefix_sections(self, prefix):
        return [self.getsection(s) for s in self.fileconfig.sections()
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
        self.reactor = reactor.Reactor()
        gc = gcode.GCodeParser(self, input_fd)
        self.objects = collections.OrderedDict({'gcode': gc})
        self.stats_timer = self.reactor.register_timer(self._stats)
        self.connect_timer = self.reactor.register_timer(
            self._connect, self.reactor.NOW)
        self.all_config_options = {}
        self.state_message = message_startup
        self.is_shutdown = False
        self.async_shutdown_msg = ""
        self.run_result = None
        self.stats_cb = []
        self.state_cb = []
    def get_start_args(self):
        return self.start_args
    def get_reactor(self):
        return self.reactor
    def get_state_message(self):
        return self.state_message
    def _set_state(self, msg):
        self.state_message = msg
        if (msg != message_ready
            and self.start_args.get('debuginput') is not None):
            self.request_exit('error_exit')
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
        objs = [self.objects[n] for n in self.objects if n.startswith(prefix)]
        if module_name in self.objects:
            return [self.objects[module_name]] + objs
        return objs
    def set_rollover_info(self, name, info):
        logging.info(info)
        if self.bglogger is not None:
            self.bglogger.set_rollover_info(name, info)
    def _stats(self, eventtime, force_output=False):
        stats = [cb(eventtime) for cb in self.stats_cb]
        if max([s[0] for s in stats] + [force_output]):
            logging.info("Stats %.1f: %s", eventtime,
                         ' '.join([s[1] for s in stats]))
        return eventtime + 1.
    def try_load_module(self, config, section):
        if section in self.objects:
            return self.objects[section]
        module_parts = section.split()
        module_name = module_parts[0]
        py_name = os.path.join(os.path.dirname(__file__),
                               'extras', module_name + '.py')
        if not os.path.exists(py_name):
            return None
        mod = importlib.import_module('extras.' + module_name)
        init_func = 'load_config'
        if len(module_parts) > 1:
            init_func = 'load_config_prefix'
        init_func = getattr(mod, init_func, None)
        if init_func is not None:
            self.objects[section] = init_func(config.getsection(section))
            return self.objects[section]
    def _read_config(self):
        fileconfig = ConfigParser.RawConfigParser()
        config_file = self.start_args['config_file']
        res = fileconfig.read(config_file)
        if not res:
            raise self.config_error("Unable to open config file %s" % (
                config_file,))
        if self.bglogger is not None:
            ConfigLogger(fileconfig, self.bglogger)
        # Create printer components
        config = ConfigWrapper(self, fileconfig, 'printer')
        for m in [pins, heater, mcu]:
            m.add_printer_objects(self, config)
        for section in fileconfig.sections():
            self.try_load_module(config, section)
        for m in [toolhead, extruder]:
            m.add_printer_objects(self, config)
        # Validate that there are no undefined parameters in the config file
        valid_sections = { s: 1 for s, o in self.all_config_options }
        for section_name in fileconfig.sections():
            section = section_name.lower()
            if section not in valid_sections and section not in self.objects:
                raise self.config_error(
                    "Section '%s' is not a valid config section" % (section,))
            for option in fileconfig.options(section_name):
                option = option.lower()
                if (section, option) not in self.all_config_options:
                    raise self.config_error(
                        "Option '%s' is not valid in section '%s'" % (
                            option, section))
        # Determine which printer objects have stats/state callbacks
        self.stats_cb = [o.stats for o in self.objects.values()
                         if hasattr(o, 'stats')]
        self.state_cb = [o.printer_state for o in self.objects.values()
                         if hasattr(o, 'printer_state')]
    def _connect(self, eventtime):
        self.reactor.unregister_timer(self.connect_timer)
        try:
            self._read_config()
            for cb in self.state_cb:
                if self.state_message is not message_startup:
                    return self.reactor.NEVER
                cb('connect')
            self._set_state(message_ready)
            for cb in self.state_cb:
                if self.state_message is not message_ready:
                    return self.reactor.NEVER
                cb('ready')
            if self.start_args.get('debugoutput') is None:
                self.reactor.update_timer(self.stats_timer, self.reactor.NOW)
        except (self.config_error, pins.error) as e:
            logging.exception("Config error")
            self._set_state("%s%s" % (str(e), message_restart))
        except msgproto.error as e:
            logging.exception("Protocol error")
            self._set_state("%s%s" % (str(e), message_protocol_error))
        except mcu.error as e:
            logging.exception("MCU error during connect")
            self._set_state("%s%s" % (str(e), message_mcu_connect_error))
        except:
            logging.exception("Unhandled exception during connect")
            self._set_state("Internal error during connect.%s" % (
                message_restart,))
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
                return "error_exit"
            # Check restart flags
            run_result = self.run_result
            try:
                if run_result == 'shutdown':
                    self.invoke_shutdown(self.async_shutdown_msg)
                    continue
                self._stats(self.reactor.monotonic(), force_output=True)
                if run_result == 'firmware_restart':
                    for m in self.lookup_module_objects('mcu'):
                        m.microcontroller_restart()
                for cb in self.state_cb:
                    cb('disconnect')
            except:
                logging.exception("Unhandled exception during post run")
            return run_result
    def invoke_shutdown(self, msg):
        if self.is_shutdown:
            return
        self.is_shutdown = True
        self._set_state("%s%s" % (msg, message_shutdown))
        for cb in self.state_cb:
            cb('shutdown')
    def invoke_async_shutdown(self, msg):
        self.async_shutdown_msg = msg
        self.request_exit("shutdown")
    def request_exit(self, result):
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
        versions = "\n".join([
            "Args: %s" % (sys.argv,),
            "Git version: %s" % (repr(start_args['software_version']),),
            "CPU: %s" % (util.get_cpu_info(),),
            "Python: %s" % (repr(sys.version),)])
        logging.info(versions)

    # Start Printer() class
    while 1:
        if bglogger is not None:
            bglogger.clear_rollover_info()
            bglogger.set_rollover_info('versions', versions)
        printer = Printer(input_fd, bglogger, start_args)
        res = printer.run()
        if res in ['exit', 'error_exit']:
            break
        time.sleep(1.)
        logging.info("Restarting printer")
        start_args['start_reason'] = res

    if bglogger is not None:
        bglogger.stop()

    if res == 'error_exit':
        sys.exit(-1)

if __name__ == '__main__':
    main()
