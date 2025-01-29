# Code for reading and writing the Klipper config file
#
# Copyright (C) 2016-2024  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, os, glob, re, time, logging, configparser, io

error = configparser.Error


######################################################################
# Config section parsing helper
######################################################################

class sentinel:
    pass

class ConfigWrapper:
    error = configparser.Error
    def __init__(self, printer, fileconfig, access_tracking, section):
        self.printer = printer
        self.fileconfig = fileconfig
        self.access_tracking = access_tracking
        self.section = section
    def get_printer(self):
        return self.printer
    def get_name(self):
        return self.section
    def _get_wrapper(self, parser, option, default, minval=None, maxval=None,
                     above=None, below=None, note_valid=True):
        if not self.fileconfig.has_option(self.section, option):
            if default is not sentinel:
                if note_valid and default is not None:
                    acc_id = (self.section.lower(), option.lower())
                    self.access_tracking[acc_id] = default
                return default
            raise error("Option '%s' in section '%s' must be specified"
                        % (option, self.section))
        try:
            v = parser(self.section, option)
        except self.error as e:
            raise
        except:
            raise error("Unable to parse option '%s' in section '%s'"
                        % (option, self.section))
        if note_valid:
            self.access_tracking[(self.section.lower(), option.lower())] = v
        if minval is not None and v < minval:
            raise error("Option '%s' in section '%s' must have minimum of %s"
                        % (option, self.section, minval))
        if maxval is not None and v > maxval:
            raise error("Option '%s' in section '%s' must have maximum of %s"
                        % (option, self.section, maxval))
        if above is not None and v <= above:
            raise error("Option '%s' in section '%s' must be above %s"
                        % (option, self.section, above))
        if below is not None and v >= below:
            raise self.error("Option '%s' in section '%s' must be below %s"
                             % (option, self.section, below))
        return v
    def get(self, option, default=sentinel, note_valid=True):
        return self._get_wrapper(self.fileconfig.get, option, default,
                                 note_valid=note_valid)
    def getint(self, option, default=sentinel, minval=None, maxval=None,
               note_valid=True):
        return self._get_wrapper(self.fileconfig.getint, option, default,
                                 minval, maxval, note_valid=note_valid)
    def getfloat(self, option, default=sentinel, minval=None, maxval=None,
                 above=None, below=None, note_valid=True):
        return self._get_wrapper(self.fileconfig.getfloat, option, default,
                                 minval, maxval, above, below,
                                 note_valid=note_valid)
    def getboolean(self, option, default=sentinel, note_valid=True):
        return self._get_wrapper(self.fileconfig.getboolean, option, default,
                                 note_valid=note_valid)
    def getchoice(self, option, choices, default=sentinel, note_valid=True):
        if type(choices) == type([]):
            choices = {i: i for i in choices}
        if choices and type(list(choices.keys())[0]) == int:
            c = self.getint(option, default, note_valid=note_valid)
        else:
            c = self.get(option, default, note_valid=note_valid)
        if c not in choices:
            raise error("Choice '%s' for option '%s' in section '%s'"
                        " is not a valid choice" % (c, option, self.section))
        return choices[c]
    def getlists(self, option, default=sentinel, seps=(',',), count=None,
                 parser=str, note_valid=True):
        def lparser(value, pos):
            if len(value.strip()) == 0:
                # Return an empty list instead of [''] for empty string
                parts = []
            else:
                parts = [p.strip() for p in value.split(seps[pos])]
            if pos:
                # Nested list
                return tuple([lparser(p, pos - 1) for p in parts if p])
            res = [parser(p) for p in parts]
            if count is not None and len(res) != count:
                raise error("Option '%s' in section '%s' must have %d elements"
                            % (option, self.section, count))
            return tuple(res)
        def fcparser(section, option):
            return lparser(self.fileconfig.get(section, option), len(seps) - 1)
        return self._get_wrapper(fcparser, option, default,
                                 note_valid=note_valid)
    def getlist(self, option, default=sentinel, sep=',', count=None,
                note_valid=True):
        return self.getlists(option, default, seps=(sep,), count=count,
                             parser=str, note_valid=note_valid)
    def getintlist(self, option, default=sentinel, sep=',', count=None,
                   note_valid=True):
        return self.getlists(option, default, seps=(sep,), count=count,
                             parser=int, note_valid=note_valid)
    def getfloatlist(self, option, default=sentinel, sep=',', count=None,
                     note_valid=True):
        return self.getlists(option, default, seps=(sep,), count=count,
                             parser=float, note_valid=note_valid)
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
    def deprecate(self, option, value=None):
        if not self.fileconfig.has_option(self.section, option):
            return
        if value is None:
            msg = ("Option '%s' in section '%s' is deprecated."
                   % (option, self.section))
        else:
            msg = ("Value '%s' in option '%s' in section '%s' is deprecated."
                   % (value, option, self.section))
        pconfig = self.printer.lookup_object("configfile")
        pconfig.deprecate(self.section, option, value, msg)


######################################################################
# Config file parsing (with include file support)
######################################################################

class ConfigFileReader:
    def read_config_file(self, filename):
        try:
            f = open(filename, 'r')
            data = f.read()
            f.close()
        except:
            msg = "Unable to open config file %s" % (filename,)
            logging.exception(msg)
            raise error(msg)
        return data.replace('\r\n', '\n')
    def build_config_string(self, fileconfig):
        sfile = io.StringIO()
        fileconfig.write(sfile)
        return sfile.getvalue().strip()
    def append_fileconfig(self, fileconfig, data, filename):
        if not data:
            return
        # Strip trailing comments
        lines = data.split('\n')
        for i, line in enumerate(lines):
            pos = line.find('#')
            if pos >= 0:
                lines[i] = line[:pos]
        sbuffer = io.StringIO('\n'.join(lines))
        if sys.version_info.major >= 3:
            fileconfig.read_file(sbuffer, filename)
        else:
            fileconfig.readfp(sbuffer, filename)
    def _create_fileconfig(self):
        if sys.version_info.major >= 3:
            fileconfig = configparser.RawConfigParser(
                strict=False, inline_comment_prefixes=(';', '#'))
        else:
            fileconfig = configparser.RawConfigParser()
        return fileconfig
    def build_fileconfig(self, data, filename):
        fileconfig = self._create_fileconfig()
        self.append_fileconfig(fileconfig, data, filename)
        return fileconfig
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
            include_data = self.read_config_file(include_filename)
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
        buf = []
        for line in lines:
            # Strip trailing comment
            pos = line.find('#')
            if pos >= 0:
                line = line[:pos]
            # Process include or buffer line
            mo = configparser.RawConfigParser.SECTCRE.match(line)
            header = mo and mo.group('header')
            if header and header.startswith('include '):
                self.append_fileconfig(fileconfig, '\n'.join(buf), filename)
                del buf[:]
                include_spec = header[8:].strip()
                self._resolve_include(filename, include_spec, fileconfig,
                                      visited)
            else:
                buf.append(line)
        self.append_fileconfig(fileconfig, '\n'.join(buf), filename)
        visited.remove(path)
    def build_fileconfig_with_includes(self, data, filename):
        fileconfig = self._create_fileconfig()
        self._parse_config(data, filename, fileconfig, set())
        return fileconfig


######################################################################
# Config auto save helper
######################################################################

AUTOSAVE_HEADER = """
#*# <---------------------- SAVE_CONFIG ---------------------->
#*# DO NOT EDIT THIS BLOCK OR BELOW. The contents are auto-generated.
#*#
"""

class ConfigAutoSave:
    def __init__(self, printer):
        self.printer = printer
        self.fileconfig = None
        self.status_save_pending = {}
        self.save_config_pending = False
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("SAVE_CONFIG", self.cmd_SAVE_CONFIG,
                               desc=self.cmd_SAVE_CONFIG_help)
    def _find_autosave_data(self, data):
        regular_data = data
        autosave_data = ""
        pos = data.find(AUTOSAVE_HEADER)
        if pos >= 0:
            regular_data = data[:pos]
            autosave_data = data[pos + len(AUTOSAVE_HEADER):].strip()
        # Check for errors and strip line prefixes
        if "\n#*# " in regular_data or autosave_data.find(AUTOSAVE_HEADER) >= 0:
            logging.warning("Can't read autosave from config file"
                            " - autosave state corrupted")
            return data, ""
        out = [""]
        for line in autosave_data.split('\n'):
            if ((not line.startswith("#*#")
                 or (len(line) >= 4 and not line.startswith("#*# ")))
                and autosave_data):
                logging.warning("Can't read autosave from config file"
                                " - modifications after header")
                return data, ""
            out.append(line[4:])
        out.append("")
        return regular_data, "\n".join(out)
    comment_r = re.compile('[#;].*$')
    value_r = re.compile('[^A-Za-z0-9_].*$')
    def _strip_duplicates(self, data, fileconfig):
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
            if fileconfig.has_option(section, field):
                is_dup_field = True
                lines[lineno] = '#' + lines[lineno]
        return "\n".join(lines)
    def load_main_config(self):
        filename = self.printer.get_start_args()['config_file']
        cfgrdr = ConfigFileReader()
        data = cfgrdr.read_config_file(filename)
        regular_data, autosave_data = self._find_autosave_data(data)
        regular_fileconfig = cfgrdr.build_fileconfig_with_includes(
            regular_data, filename)
        autosave_data = self._strip_duplicates(autosave_data,
                                               regular_fileconfig)
        self.fileconfig = cfgrdr.build_fileconfig(autosave_data, filename)
        cfgrdr.append_fileconfig(regular_fileconfig,
                                 autosave_data, '*AUTOSAVE*')
        return regular_fileconfig, self.fileconfig
    def get_status(self, eventtime):
        return {'save_config_pending': self.save_config_pending,
                'save_config_pending_items': self.status_save_pending}
    def set(self, section, option, value):
        if not self.fileconfig.has_section(section):
            self.fileconfig.add_section(section)
        svalue = str(value)
        self.fileconfig.set(section, option, svalue)
        pending = dict(self.status_save_pending)
        if not section in pending or pending[section] is None:
            pending[section] = {}
        else:
            pending[section] = dict(pending[section])
        pending[section][option] = svalue
        self.status_save_pending = pending
        self.save_config_pending = True
        logging.info("save_config: set [%s] %s = %s", section, option, svalue)
    def remove_section(self, section):
        if self.fileconfig.has_section(section):
            self.fileconfig.remove_section(section)
            pending = dict(self.status_save_pending)
            pending[section] = None
            self.status_save_pending = pending
            self.save_config_pending = True
        elif (section in self.status_save_pending and
              self.status_save_pending[section] is not None):
            pending = dict(self.status_save_pending)
            del pending[section]
            self.status_save_pending = pending
            self.save_config_pending = True
    def _disallow_include_conflicts(self, regular_fileconfig):
        for section in self.fileconfig.sections():
            for option in self.fileconfig.options(section):
                if regular_fileconfig.has_option(section, option):
                    msg = ("SAVE_CONFIG section '%s' option '%s' conflicts "
                           "with included value" % (section, option))
                    raise self.printer.command_error(msg)
    cmd_SAVE_CONFIG_help = "Overwrite config file and restart"
    def cmd_SAVE_CONFIG(self, gcmd):
        if not self.fileconfig.sections():
            return
        # Create string containing autosave data
        cfgrdr = ConfigFileReader()
        autosave_data = cfgrdr.build_config_string(self.fileconfig)
        lines = [('#*# ' + l).strip()
                 for l in autosave_data.split('\n')]
        lines.insert(0, "\n" + AUTOSAVE_HEADER.rstrip())
        lines.append("")
        autosave_data = '\n'.join(lines)
        # Read in and validate current config file
        cfgname = self.printer.get_start_args()['config_file']
        try:
            data = cfgrdr.read_config_file(cfgname)
        except error as e:
            msg = "Unable to read existing config on SAVE_CONFIG"
            logging.exception(msg)
            raise gcmd.error(msg)
        regular_data, old_autosave_data = self._find_autosave_data(data)
        regular_data = self._strip_duplicates(regular_data, self.fileconfig)
        data = regular_data.rstrip() + autosave_data
        new_regular_data, new_autosave_data = self._find_autosave_data(data)
        if not new_autosave_data:
            raise gcmd.error(
                "Existing config autosave is corrupted."
                " Can't complete SAVE_CONFIG")
        try:
            regular_fileconfig = cfgrdr.build_fileconfig_with_includes(
                new_regular_data, cfgname)
        except error as e:
            msg = "Unable to parse existing config on SAVE_CONFIG"
            logging.exception(msg)
            raise gcmd.error(msg)
        self._disallow_include_conflicts(regular_fileconfig)
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
            f = open(temp_name, 'w')
            f.write(data)
            f.close()
            os.rename(cfgname, backup_name)
            os.rename(temp_name, cfgname)
        except:
            msg = "Unable to write config file during SAVE_CONFIG"
            logging.exception(msg)
            raise gcmd.error(msg)
        # Request a restart
        gcode = self.printer.lookup_object('gcode')
        gcode.request_restart('restart')


######################################################################
# Config validation (check for undefined options)
######################################################################

class ConfigValidate:
    def __init__(self, printer):
        self.printer = printer
        self.status_settings = {}
        self.access_tracking = {}
        self.autosave_options = {}
    def start_access_tracking(self, autosave_fileconfig):
        # Note autosave options for use during undefined options check
        self.autosave_options = {}
        for section in autosave_fileconfig.sections():
            for option in autosave_fileconfig.options(section):
                self.autosave_options[(section.lower(), option.lower())] = 1
        self.access_tracking = {}
        return self.access_tracking
    def check_unused(self, fileconfig):
        # Don't warn on fields set in autosave segment
        access_tracking = dict(self.access_tracking)
        access_tracking.update(self.autosave_options)
        # Note locally used sections
        valid_sections = { s: 1 for s, o in self.printer.lookup_objects() }
        valid_sections.update({ s: 1 for s, o in access_tracking })
        # Validate that there are no undefined parameters in the config file
        for section_name in fileconfig.sections():
            section = section_name.lower()
            if section not in valid_sections:
                raise error("Section '%s' is not a valid config section"
                            % (section,))
            for option in fileconfig.options(section_name):
                option = option.lower()
                if (section, option) not in access_tracking:
                    raise error("Option '%s' is not valid in section '%s'"
                                % (option, section))
        # Setup get_status()
        self._build_status_settings()
        # Clear tracking state
        self.access_tracking.clear()
        self.autosave_options.clear()
    def _build_status_settings(self):
        self.status_settings = {}
        for (section, option), value in self.access_tracking.items():
            self.status_settings.setdefault(section, {})[option] = value
    def get_status(self, eventtime):
        return {'settings': self.status_settings}


######################################################################
# Main printer config tracking
######################################################################

class PrinterConfig:
    def __init__(self, printer):
        self.printer = printer
        self.autosave = ConfigAutoSave(printer)
        self.validate = ConfigValidate(printer)
        self.deprecated = {}
        self.runtime_warnings = []
        self.deprecate_warnings = []
        self.status_raw_config = {}
        self.status_warnings = []
    def get_printer(self):
        return self.printer
    def read_config(self, filename):
        cfgrdr = ConfigFileReader()
        data = cfgrdr.read_config_file(filename)
        fileconfig = cfgrdr.build_fileconfig(data, filename)
        return ConfigWrapper(self.printer, fileconfig, {}, 'printer')
    def read_main_config(self):
        fileconfig, autosave_fileconfig = self.autosave.load_main_config()
        access_tracking = self.validate.start_access_tracking(
            autosave_fileconfig)
        config = ConfigWrapper(self.printer, fileconfig,
                               access_tracking, 'printer')
        self._build_status_config(config)
        return config
    def log_config(self, config):
        cfgrdr = ConfigFileReader()
        lines = ["===== Config file =====",
                 cfgrdr.build_config_string(config.fileconfig),
                 "======================="]
        self.printer.set_rollover_info("config", "\n".join(lines))
    def check_unused_options(self, config):
        self.validate.check_unused(config.fileconfig)
    # Deprecation warnings
    def runtime_warning(self, msg):
        logging.warning(msg)
        res = {'type': 'runtime_warning', 'message': msg}
        self.runtime_warnings.append(res)
        self.status_warnings = self.runtime_warnings + self.deprecate_warnings
    def deprecate(self, section, option, value=None, msg=None):
        key = (section, option, value)
        if key in self.deprecated and self.deprecated[key] == msg:
            return
        self.deprecated[key] = msg
        self.deprecate_warnings = []
        for (section, option, value), msg in self.deprecated.items():
            if value is None:
                res = {'type': 'deprecated_option'}
            else:
                res = {'type': 'deprecated_value', 'value': value}
            res['message'] = msg
            res['section'] = section
            res['option'] = option
            self.deprecate_warnings.append(res)
        self.status_warnings = self.runtime_warnings + self.deprecate_warnings
    # Status reporting
    def _build_status_config(self, config):
        self.status_raw_config = {}
        for section in config.get_prefix_sections(''):
            self.status_raw_config[section.get_name()] = section_status = {}
            for option in section.get_prefix_options(''):
                section_status[option] = section.get(option, note_valid=False)
    def get_status(self, eventtime):
        status = {'config': self.status_raw_config,
                  'warnings': self.status_warnings}
        status.update(self.autosave.get_status(eventtime))
        status.update(self.validate.get_status(eventtime))
        return status
    # Autosave functions
    def set(self, section, option, value):
        self.autosave.set(section, option, value)
    def remove_section(self, section):
        self.autosave.remove_section(section)
