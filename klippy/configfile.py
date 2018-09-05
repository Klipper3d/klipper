# Code for reading the Klipper config file
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import ConfigParser

error = ConfigParser.Error

class sentinel:
    pass

class ConfigWrapper:
    error = ConfigParser.Error
    def __init__(self, printer, fileconfig, access_tracking, section):
        self.printer = printer
        self.fileconfig = fileconfig
        self.access_tracking = access_tracking
        self.section = section
    def get_printer(self):
        return self.printer
    def get_name(self):
        return self.section
    def _get_wrapper(self, parser, option, default,
                     minval=None, maxval=None, above=None, below=None):
        if (default is not sentinel
            and not self.fileconfig.has_option(self.section, option)):
            return default
        self.access_tracking[(self.section.lower(), option.lower())] = 1
        try:
            v = parser(self.section, option)
        except self.error as e:
            raise
        except:
            raise error("Unable to parse option '%s' in section '%s'" % (
                option, self.section))
        if minval is not None and v < minval:
            raise error(
                "Option '%s' in section '%s' must have minimum of %s" % (
                    option, self.section, minval))
        if maxval is not None and v > maxval:
            raise error(
                "Option '%s' in section '%s' must have maximum of %s" % (
                    option, self.section, maxval))
        if above is not None and v <= above:
            raise error("Option '%s' in section '%s' must be above %s" % (
                option, self.section, above))
        if below is not None and v >= below:
            raise self.error("Option '%s' in section '%s' must be below %s" % (
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
            raise error("Choice '%s' for option '%s' in section '%s'"
                        " is not a valid choice" % (c, option, self.section))
        return choices[c]
    def getsection(self, section):
        return ConfigWrapper(self.printer, self.fileconfig,
                             self.access_tracking, section)
    def has_section(self, section):
        return self.fileconfig.has_section(section)
    def get_prefix_sections(self, prefix):
        return [self.getsection(s) for s in self.fileconfig.sections()
                if s.startswith(prefix)]

class ConfigLogger:
    def __init__(self, fileconfig, printer):
        self.lines = ["===== Config file ====="]
        fileconfig.write(self)
        self.lines.append("=======================")
        printer.set_rollover_info("config", "\n".join(self.lines))
    def write(self, data):
        self.lines.append(data.strip())

class PrinterConfig:
    def __init__(self, printer):
        self.printer = printer
    def read_config(self, filename):
        fileconfig = ConfigParser.RawConfigParser()
        res = fileconfig.read(filename)
        if not res:
            raise error("Unable to open config file %s" % (filename,))
        return ConfigWrapper(self.printer, fileconfig, {}, 'printer')
    def read_main_config(self):
        filename = self.printer.get_start_args()['config_file']
        return self.read_config(filename)
    def check_unused_options(self, config):
        access_tracking = config.access_tracking
        fileconfig = config.fileconfig
        objects = dict(self.printer.lookup_objects())
        # Validate that there are no undefined parameters in the config file
        valid_sections = { s: 1 for s, o in access_tracking }
        for section_name in fileconfig.sections():
            section = section_name.lower()
            if section not in valid_sections and section not in objects:
                raise error("Section '%s' is not a valid config section" % (
                    section,))
            for option in fileconfig.options(section_name):
                option = option.lower()
                if (section, option) not in access_tracking:
                    raise error("Option '%s' is not valid in section '%s'" % (
                        option, section))
    def log_config(self, config):
        ConfigLogger(config.fileconfig, self.printer)
