class GuppyConfigHelper:
    def __init__(self, config):
        self.printer = config.get_printer()
        
        # Register commands
        gcode = config.get_printer().lookup_object('gcode')
        gcode.register_command("_GUPPY_SAVE_CONFIG", self.cmd_guppy_save_config)
        gcode.register_command("_GUPPY_DELETE_CONFIG", self.cmd_guppy_delete_config)

    def cmd_guppy_save_config(self, gcmd):
        self.section = gcmd.get('SECTION', None)
        self.pairs = gcmd.get('KEY_VALUE', None)

        if self.section and self.pairs:
            configfile = self.printer.lookup_object('configfile')
            kv = self.pairs.split(',')
            d = dict(s.split(':') for s in kv)

            configfile.remove_section(self.section)

            for k, v in d.items():
                configfile.set(self.section, k, v)

    def cmd_guppy_delete_config(self, gcmd):
        self.section = gcmd.get('SECTION', None)

        if self.section:
            configfile = self.printer.lookup_object('configfile')
            configfile.remove_section(self.section)

def load_config(config):
    return GuppyConfigHelper(config)
