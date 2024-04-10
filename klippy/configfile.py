# Code for reading and writing the Klipper config file
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os, glob, re, time, logging, ConfigParser, StringIO

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
    def get_prefix_options(self, prefix):
        return [o for o in self.fileconfig.options(self.section)
                if o.startswith(prefix)]

AUTOSAVE_HEADER = """
#*# <---------------------- SAVE_CONFIG ---------------------->
#*# DO NOT EDIT THIS BLOCK OR BELOW. The contents are auto-generated.
#*#
"""

class PrinterConfig:
    def __init__(self, printer):
        self.printer = printer
        self.autosave = None
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("SAVE_CONFIG", self.cmd_SAVE_CONFIG,
                               desc=self.cmd_SAVE_CONFIG_help)
    def _read_config_file(self, filename):
        try:
            f = open(filename, 'rb')
            data = f.read()
            f.close()
        except:
            msg = "Unable to open config file %s" % (filename,)
            logging.exception(msg)
            raise error(msg)
        return data.replace('\r\n', '\n')
    def _find_autosave_data(self, data):
        regular_data = data
        autosave_data = ""
        pos = data.find(AUTOSAVE_HEADER)
        if pos >= 0:
            regular_data = data[:pos]
            autosave_data = data[pos + len(AUTOSAVE_HEADER):].strip()
        # Check for errors and strip line prefixes
        if "\n#*# " in regular_data:
            logging.warn("Can't read autosave from config file"
                         " - autosave state corrupted")
            return data, ""
        out = [""]
        for line in autosave_data.split('\n'):
            if ((not line.startswith("#*#")
                 or (len(line) >= 4 and not line.startswith("#*# ")))
                and autosave_data):
                logging.warn("Can't read autosave from config file"
                             " - modifications after header")
                return data, ""
            out.append(line[4:])
        out.append("")
        return regular_data, "\n".join(out)
    comment_r = re.compile('[#;].*$')
    value_r = re.compile('[^A-Za-z0-9_].*$')
    def _strip_duplicates(self, data, config):
        fileconfig = config.fileconfig
        # Comment out fields in 'data' that are defined in 'config'
        lines = data.split('\n')
        section = None
        is_dup_field = False
        for lineno, line in enumerate(lines):
            pruned_line = self.comment_r.sub('', line).rstrip()
            if not pruned_line:
                continue
            if pruned_line[0].isspace():
                if is_dup_field:
                    lines[lineno] = '#' + lines[lineno]
                continue
            is_dup_field = False
            if pruned_line[0] == '[':
                section = pruned_line[1:-1].strip()
                continue
            field = self.value_r.sub('', pruned_line)
            if config.fileconfig.has_option(section, field):
                is_dup_field = True
                lines[lineno] = '#' + lines[lineno]
        return "\n".join(lines)
    def _parse_config_buffer(self, buffer, filename, fileconfig):
        if not buffer:
            return
        data = '\n'.join(buffer)
        del buffer[:]
        sbuffer = StringIO.StringIO(data)
        fileconfig.readfp(sbuffer, filename)
    def _resolve_include(self, source_filename, include_spec, fileconfig,
                         visited):
        dirname = os.path.dirname(source_filename)
        include_spec = include_spec.strip()
        include_glob = os.path.join(dirname, include_spec)
        include_filenames = glob.glob(include_glob)
        if not include_filenames and not glob.has_magic(include_glob):
            # Empty set is OK if wildcard but not for direct file reference
            raise error("Include file '%s' does not exist" % (include_glob,))
        include_filenames.sort()
        for include_filename in include_filenames:
            include_data = self._read_config_file(include_filename)
            self._parse_config(include_data, include_filename, fileconfig,
                               visited)
        return include_filenames
    def _parse_config(self, data, filename, fileconfig, visited):
        path = os.path.abspath(filename)
        if path in visited:
            raise error("Recursive include of config file '%s'" % (filename))
        visited.add(path)
        lines = data.split('\n')
        # Buffer lines between includes and parse as a unit so that overrides
        # in includes apply linearly as they do within a single file
        buffer = []
        for line in lines:
            # Strip trailing comment
            pos = line.find('#')
            if pos >= 0:
                line = line[:pos]
            # Process include or buffer line
            mo = ConfigParser.RawConfigParser.SECTCRE.match(line)
            header = mo and mo.group('header')
            if header and header.startswith('include '):
                self._parse_config_buffer(buffer, filename, fileconfig)
                include_spec = header[8:].strip()
                self._resolve_include(filename, include_spec, fileconfig,
                                      visited)
            else:
                buffer.append(line)
        self._parse_config_buffer(buffer, filename, fileconfig)
        visited.remove(path)
    def _build_config_wrapper(self, data, filename):
        fileconfig = ConfigParser.RawConfigParser()
        self._parse_config(data, filename, fileconfig, set())
        return ConfigWrapper(self.printer, fileconfig, {}, 'printer')
    def _build_config_string(self, config):
        sfile = StringIO.StringIO()
        config.fileconfig.write(sfile)
        return sfile.getvalue().strip()
    def read_config(self, filename):
        return self._build_config_wrapper(self._read_config_file(filename),
                                          filename)
    def read_main_config(self):
        filename = self.printer.get_start_args()['config_file']
        data = self._read_config_file(filename)
        regular_data, autosave_data = self._find_autosave_data(data)
        regular_config = self._build_config_wrapper(regular_data, filename)
        autosave_data = self._strip_duplicates(autosave_data, regular_config)
        self.autosave = self._build_config_wrapper(autosave_data, filename)
        self._config = self._build_config_wrapper(regular_data + autosave_data,
                                                  filename)
        return self._config
    def check_unused_options(self, config):
        fileconfig = config.fileconfig
        objects = dict(self.printer.lookup_objects())
        # Determine all the fields that have been accessed
        access_tracking = dict(config.access_tracking)
        for section in self.autosave.fileconfig.sections():
            for option in self.autosave.fileconfig.options(section):
                access_tracking[(section.lower(), option.lower())] = 1
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
        lines = ["===== Config file =====",
                 self._build_config_string(config),
                 "======================="]
        self.printer.set_rollover_info("config", "\n".join(lines))
    # Autosave functions
    def set(self, section, option, value):
        if not self.autosave.fileconfig.has_section(section):
            self.autosave.fileconfig.add_section(section)
        svalue = str(value)
        self.autosave.fileconfig.set(section, option, svalue)
        logging.info("save_config: set [%s] %s = %s", section, option, svalue)
    def remove_section(self, section):
        self.autosave.fileconfig.remove_section(section)
    def _disallow_include_conflicts(self, regular_data, cfgname, gcode):
        config = self._build_config_wrapper(regular_data, cfgname)
        for section in self.autosave.fileconfig.sections():
            for option in self.autosave.fileconfig.options(section):
                if config.fileconfig.has_option(section, option):
                    msg = "SAVE_CONFIG section '%s' option '%s' conflicts " \
                          "with included value" % (section, option)
                    raise gcode.error(msg)
    cmd_SAVE_CONFIG_help = "Overwrite config file and restart"
    def cmd_SAVE_CONFIG(self, params):
        if not self.autosave.fileconfig.sections():
            return
        gcode = self.printer.lookup_object('gcode')
        # Create string containing autosave data
        autosave_data = self._build_config_string(self.autosave)
        lines = [('#*# ' + l).strip()
                 for l in autosave_data.split('\n')]
        lines.insert(0, "\n" + AUTOSAVE_HEADER.rstrip())
        lines.append("")
        autosave_data = '\n'.join(lines)
        # Read in and validate current config file
        cfgname = self.printer.get_start_args()['config_file']
        try:
            data = self._read_config_file(cfgname)
            regular_data, old_autosave_data = self._find_autosave_data(data)
            config = self._build_config_wrapper(regular_data, cfgname)
        except error as e:
            msg = "Unable to parse existing config on SAVE_CONFIG"
            logging.exception(msg)
            raise gcode.error(msg)
        regular_data = self._strip_duplicates(regular_data, self.autosave)
        self._disallow_include_conflicts(regular_data, cfgname, gcode)
        data = regular_data.rstrip() + autosave_data
        # Determine filenames
        datestr = time.strftime("-%Y%m%d_%H%M%S")
        backup_name = cfgname + datestr
        temp_name = cfgname + "_autosave"
        if cfgname.endswith(".cfg"):
            backup_name = cfgname[:-4] + datestr + ".cfg"
            temp_name = cfgname[:-4] + "_autosave.cfg"
        # Create new config file with temporary name and swap with main config
        logging.info("SAVE_CONFIG to '%s' (backup in '%s')",
                     cfgname, backup_name)
        try:
            f = open(temp_name, 'wb')
            f.write(data)
            f.close()
            os.rename(cfgname, backup_name)
            os.rename(temp_name, cfgname)
        except:
            msg = "Unable to write config file during SAVE_CONFIG"
            logging.exception(msg)
            raise gcode.error(msg)
        # Request a restart
        gcode.request_restart('restart')
