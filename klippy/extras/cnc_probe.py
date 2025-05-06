import stepper

PROBE_REST_TIME = .001

class CncProbe:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.probes = []

        probe_pin = config.get('probe_pin')
        ppins = self.printer.lookup_object('pins')        

        self.mcu_multiaxis_probe = ppins.setup_pin('multiaxis_probe', probe_pin)

        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("G38.3", self.cmd_G38_3)

        self.speed = 25.
        self.speed_factor = 1. / 60.

        return
    
    def probe_move(self, movepos, speed):
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        x, y, z = movepos 

        # Enable probe
        self.mcu_multiaxis_probe.probe_start(print_time, PROBE_REST_TIME, True)
        # Move to desire position
        toolhead.move([x, y, z, 0.0], speed)
        # Clear probe
        self.mcu_multiaxis_probe.probe_clear()
        # Check if probe is triggered
        return self.mcu_multiaxis_probe.query_probe()
    
    def cmd_G38_3(self, gcmd):
        params = gcmd.get_command_parameters()
        target_pos = [0.0, 0.0, 0.0]
        try:
            for pos, axis in enumerate('XYZ'):
                if axis in params:
                    v = float(params[axis])
                    target_pos[pos] += v
            if 'F' in params:
                gcode_speed = float(params['F'])
                if gcode_speed <= 0.:
                    raise gcmd.error("Invalid speed in '%s'"
                                        % (gcmd.get_commandline(),))
                self.speed = gcode_speed * self.speed_factor
        except ValueError as e:
            raise gcmd.error("Unable to parse move '%s'"
                             % (gcmd.get_commandline(),))
        
        msg = self.probe_move(target_pos, self.speed)
        gcmd.respond_raw("probe_value : " + str(msg))
        return

def load_config(config):
    return CncProbe(config)