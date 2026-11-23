# Custom Component Module
#
# Copyright (C) 2024  Kamil Trzciński <ayufan@ayufan.eu>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, util, os.path, importlib.util

class CustomComponent:
    def __init__(self, config):
        self.name = config.get_name().split()[-1]
        self.path = config.get('path')
        self.used = 0
        self.module = None
        self.git_info = None

    def load_module(self):
        self.used += 1
        if not self.module:
          full_path = os.path.expanduser(self.path)
          spec = importlib.util.spec_from_file_location(self.name, full_path)
          module = importlib.util.module_from_spec(spec)
          spec.loader.exec_module(module)
          self.git_info = util.get_git_version(full_path)
          self.module = module
        return self.module

    def get_status(self, eventtime):
        return {
          'path': self.path,
          'git_info': self.git_info,
          'used': self.used
        }

def load_config_prefix(config):
    return CustomComponent(config)
