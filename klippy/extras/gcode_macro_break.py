class GCodeMacroBreaker:
    def __init__(self, config):
        # Gcode macro interupt
        self.printer = config.get_printer()
        webhooks = self.printer.lookup_object('webhooks')
        webhooks.register_endpoint("breakmacro", self._handle_breakmacro)
        webhooks.register_endpoint("resumemacro", self._handle_resumemacro)
        self.gcode = self.printer.lookup_object('gcode')
    def _handle_breakmacro(self, web_request):
        self.gcode.break_flag = True
    def _handle_resumemacro(self, web_request):
        self.gcode.break_flag = False

def load_config(config):
    return GCodeMacroBreaker(config)
