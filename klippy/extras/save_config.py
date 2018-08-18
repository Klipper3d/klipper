# Support for saving and restoring persistent printer state
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import ConfigParser, StringIO, time, os, logging
import klippy

HEADER = """
#*# <---------------------- SAVE_CONFIG ---------------------->
#*# DO NOT EDIT THIS BLOCK OR BELOW. The contents are auto-generated.
#*#
"""

class error(Exception):
    pass

class SaveConfig:
    error = error
    def __init__(self, config):
        self.printer = config.get_printer()
        self.fileconfig = None
        self.access_tracking = {}
        # Register SAVE_CONFIG command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("SAVE_CONFIG", self.cmd_SAVE_CONFIG,
                                    desc=self.cmd_SAVE_CONFIG_help)
    def _read_config(self):
        # Read config file
        filename = self.printer.get_start_args()['config_file']
        f = open(filename, 'rb')
        data = f.read()
        f.close()
        data = data.replace('\r\n', '\n')
        # Find auto-save state in config file
        regular_config = data
        persist_config = ""
        pos = data.find(HEADER)
        if pos >= 0:
            regular_config = data[:pos]
            persist_config = data[pos + len(HEADER):].strip()
        # Check for errors and strip line prefixes
        if "\n#*# " in regular_config:
            raise error("Can't read state from config file"
                        " - persistent state corrupted")
        out = []
        for line in persist_config.split('\n'):
            if ((not line.startswith("#*#")
                 or (len(line) >= 4 and not line.startswith("#*# ")))
                and persist_config):
                raise error("Can't read state from config file"
                            " - modifications after header")
            out.append(line[4:])
        return regular_config, "\n".join(out)
    def getsection(self, section):
        if self.fileconfig is None:
            # Load and parse the auto-save configuration
            regular_config, autosave_config = self._read_config()
            sfile = StringIO.StringIO(autosave_config)
            self.fileconfig = ConfigParser.RawConfigParser()
            self.fileconfig.readfp(sfile)
            if sfile.getvalue():
                klippy.ConfigLogger(self.fileconfig, self.printer,
                                    "Autosave", "save_config")
        return klippy.ConfigWrapper(self.printer, self.fileconfig,
                                    self.access_tracking, section)
    def set(self, section, option, value):
        if self.fileconfig is None:
            self.getsection(section)
        if not self.fileconfig.has_section(section):
            self.fileconfig.add_section(section)
        svalue = str(value)
        self.fileconfig.set(section, option, svalue)
        logging.info("save_config: set [%s] %s = %s", section, option, value)
    def remove_section(self, section):
        if self.fileconfig is None:
            self.getsection(section)
        self.fileconfig.remove_section(section)
    cmd_SAVE_CONFIG_help = "Save internal state back to config file"
    def cmd_SAVE_CONFIG(self, params):
        if self.fileconfig is None:
            return
        # Create string containing persistent config
        sfile = StringIO.StringIO()
        self.fileconfig.write(sfile) # XXX - should prune unused options
        lines = [('#*# ' + l).strip()
                 for l in sfile.getvalue().strip().split('\n')]
        lines.insert(0, "\n" + HEADER.rstrip())
        lines.append("")
        autosave_config = '\n'.join(lines)
        # Determine filenames
        cfgname = self.printer.get_start_args()['config_file']
        datestr = time.strftime("_%Y%m%d_%H%M%S")
        backup_name = cfgname + datestr
        temp_name = cfgname + "_autosave"
        if cfgname.endswith(".cfg"):
            backup_name = cfgname[:-4] + datestr + ".cfg"
            temp_name = cfgname[:-4] + "_autosave.cfg"
        # Create new config file with temporary name
        regular_config, old_autosave_config = self._read_config()
        f = open(temp_name, 'wb') # XXX - should catch errors
        f.write(regular_config.rstrip() + autosave_config)
        f.close()
        # Replace existing config with new config
        os.rename(cfgname, backup_name) # XXX - catch errors
        os.rename(temp_name, cfgname)
        # XXX - should warn user about any config conflicts

def load_config(config):
    return SaveConfig(config)
