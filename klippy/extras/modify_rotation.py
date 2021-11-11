
import logging

class RotationDistanceModifier:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.steppers = {}	
        gcode.register_mux_command("SET_STEPPER_STEP_DISTANCE", "STEPPER",
                                   self.name, self.cmd_SET_E_ROT_DISTANCE,
                                   desc=self.cmd_SET_E_ROT_DISTANCE_help)
	#gcode.register_command('FORCE_MOVE', self.cmd_FORCE_MOVE,
        #                          desc=self.cmd_FORCE_MOVE_help)
        #issue command

    #see if stepper exists
    def _lookup_stepper(self, gcmd):
        name = gcmd.get('STEPPER')
        if name not in self.steppers:
            raise gcmd.error("Unknown stepper %s" % (name,))
        return self.steppers[name]
    
    
    cmd_SET_E_ROT_DISTANCE_help = "Set stepper step distance"
    def cmd_SET_E_ROT_DISTANCE(self, gcmd):
        #toolhead = self.printer.lookup_object('toolhead')
        stepper = self._lookup_stepper(gcmd)
        dist = gcmd.get_float('DISTANCE', None, above=0.) #accept if non zero the terminal command
        if dist is None:
            step_dist = self.stepper.get_step_dist()
            gcmd.respond_info("stepper '%s' step distance is %0.6f"
                              % (self.name, step_dist))
            return
        toolhead.flush_step_generation() 
        self.stepper.set_step_dist(dist)
        gcmd.respond_info("stepper '%s' step distance set to %0.6f"
                          % (self.name, dist))


def load_config(config):
    return RotationDistanceModifier(config)