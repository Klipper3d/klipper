# Module loading path customization support
#
# Copyright (C) 2020  Trevor Jones <trevorjones141@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os, logging, ConfigParser

class sentinel:
    pass

class ModulePaths:
    error = ConfigParser.Error
    def __init__(self, config):
        self._config = config
        printer = config.get_printer()
        printer.register_event_handler("klippy:ready", self.handle_ready)
    def handle_ready(self):
        self._config = sentinel
    def find_path(self, modulename):
        if self._config is sentinel:
            raise self.error(
                "Module path lookup after configuration phase is not supported")
        raw = self._config.get(modulename, None)
        if raw is None:
            if '/' in modulename:
                parts = modulename.split('/')
                package = parts[0]
                parent = self._config.get(package, None)
                if parent is not None:
                    raw = os.path.join(parent, "%s.py" % parts[1])
                    logging.info("no override found, checking for parent directory path: %s" % raw)
        if raw is None:
            return None
        if raw.endswith(".py"):
            path = os.path.normpath(os.path.expanduser(raw))
        else:
            path = os.path.join(os.path.normpath(os.path.expanduser(raw)),
                                '__init__.py')
        if not os.path.exists(path):
            raise self.error("Invalid module path %s" % path)
        logging.info("using custom path for %s: %s" % (modulename, path))
        return path

def load_config(config):
    return ModulePaths(config)