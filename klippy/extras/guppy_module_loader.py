import os.path
import tempfile

class GuppyModuleLoader:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.config = config

        # Register commands
        gcode = config.get_printer().lookup_object('gcode')
        gcode.register_command("_GUPPY_LOAD_MODULE", self.cmd_guppy_load_module)
        gcode.register_command("_GUPPY_UNLOAD_MODULE", self.cmd_guppy_unload_module)

    def cmd_guppy_load_module(self, gcmd):
        self.section = gcmd.get('SECTION', None)

        if self.section and self.section not in self.printer.objects:
            self.printer.load_object(self.config, self.section)

    def cmd_guppy_unload_module(self, gcmd):
        self.section = gcmd.get('SECTION', None)

        if self.section and self.section in self.printer.objects:
            self.printer.objects.pop(self.section)


def load_config(config):
    return GuppyModuleLoader(config)
