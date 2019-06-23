# Endstop accuracy improvement via stepper phase tracking
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import homing

TRINAMIC_DRIVERS = ["tmc2130", "tmc2208", "tmc2209", "tmc2660", "tmc5160"]

class EndstopPhase:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[1]
        # Determine number of stepper phases
        for driver in TRINAMIC_DRIVERS:
            driver_name = "%s %s" % (driver, self.name)
            if config.has_section(driver_name):
                module = self.printer.try_load_module(config, driver_name)
                self.get_phase = module.get_phase
                self.phases = module.get_microsteps() * 4
                break
        else:
            self.get_phase = None
            self.phases = config.getint('phases', minval=1)
        # Determine endstop phase position
        self.endstop_phase = config.getint('endstop_phase', None,
                                           minval=0, maxval=self.phases-1)
        self.endstop_align_zero = config.getboolean('endstop_align_zero', False)
        # Determine endstop accuracy
        stepper_config = config.getsection(self.name)
        self.step_dist = step_dist = stepper_config.getfloat('step_distance')
        endstop_accuracy = config.getfloat('endstop_accuracy', None, above=0.)
        if endstop_accuracy is None:
            self.endstop_accuracy = self.phases//2 - 1
        elif self.endstop_phase is not None:
            self.endstop_accuracy = int(
                math.ceil(endstop_accuracy * .5 / step_dist))
        else:
            self.endstop_accuracy = int(math.ceil(endstop_accuracy / step_dist))
        if self.endstop_accuracy >= self.phases // 2:
            raise config.error("Endstop for %s is not accurate enough for"
                               " stepper phase adjustment" % (self.name,))
        if self.printer.get_start_args().get('debugoutput') is not None:
            self.endstop_accuracy = self.phases
        self.phase_history = [0] * self.phases
        # Register event handler
        self.printer.register_event_handler(
            "homing:homed_rails", self.handle_homed_rails)
        self.printer.try_load_module(config, "endstop_phase")
    def align_endstop(self, pos):
        if not self.endstop_align_zero or self.endstop_phase is None:
            return pos
        # Adjust the endstop position so 0.0 is always at a full step
        microsteps = self.phases // 4
        half_microsteps = microsteps // 2
        phase_offset = (((self.endstop_phase + half_microsteps) % microsteps)
                        - half_microsteps) * self.step_dist
        full_step = microsteps * self.step_dist
        return int(pos / full_step + .5) * full_step + phase_offset
    def get_homed_offset(self, stepper):
        if self.get_phase is not None:
            try:
                phase = self.get_phase()
            except Exception as e:
                msg = "Unable to get stepper %s phase: %s" % (self.name, str(e))
                logging.exception(msg)
                raise homing.EndstopError(msg)
            if stepper.is_dir_inverted():
                phase = (self.phases - 1) - phase
        else:
            phase = stepper.get_mcu_position() % self.phases
        self.phase_history[phase] += 1
        if self.endstop_phase is None:
            logging.info("Setting %s endstop phase to %d", self.name, phase)
            self.endstop_phase = phase
            return 0.
        delta = (phase - self.endstop_phase) % self.phases
        if delta >= self.phases - self.endstop_accuracy:
            delta -= self.phases
        elif delta > self.endstop_accuracy:
            raise homing.EndstopError(
                "Endstop %s incorrect phase (got %d vs %d)" % (
                    self.name, phase, self.endstop_phase))
        return delta * self.step_dist
    def handle_homed_rails(self, homing_state, rails):
        for rail in rails:
            stepper = rail.get_steppers()[0]
            if stepper.get_name() != self.name:
                continue
            orig_pos = rail.get_commanded_position()
            offset = self.get_homed_offset(stepper)
            pos = self.align_endstop(orig_pos) + offset
            if pos == orig_pos:
                return False
            rail.set_commanded_position(pos)
            return True

class EndstopPhases:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("ENDSTOP_PHASE_CALIBRATE",
                                    self.cmd_ENDSTOP_PHASE_CALIBRATE,
                                    desc=self.cmd_ENDSTOP_PHASE_CALIBRATE_help)
        self.tracking = {}
        # Register event handler
        self.printer.register_event_handler(
            "homing:homed_rails", self.handle_homed_rails)
    def lookup_rail(self, stepper, stepper_name):
        mod_name = "endstop_phase %s" % (stepper_name,)
        m = self.printer.lookup_object(mod_name, None)
        if m is not None:
            return (None, m.phase_history)
        for driver in TRINAMIC_DRIVERS:
            mod_name = "%s %s" % (driver, stepper_name)
            m = self.printer.lookup_object(mod_name, None)
            if m is not None:
                return (m.get_phase, [0] * (m.get_microsteps() * 4))
        return None
    def update_rail(self, info, stepper):
        if info is None:
            return
        get_phase, phase_history = info
        if get_phase is None:
            return
        try:
            phase = get_phase()
        except:
            logging.exception("Error in EndstopPhases get_phase")
            return
        phase_history[phase] += 1
    def handle_homed_rails(self, homing_state, rails):
        for rail in rails:
            stepper = rail.get_steppers()[0]
            stepper_name = stepper.get_name()
            if stepper_name not in self.tracking:
                info = self.lookup_rail(stepper, stepper_name)
                self.tracking[stepper_name] = info
            self.update_rail(self.tracking[stepper_name], stepper)
    cmd_ENDSTOP_PHASE_CALIBRATE_help = "Calibrate stepper phase"
    def cmd_ENDSTOP_PHASE_CALIBRATE(self, params):
        stepper_name = self.gcode.get_str('STEPPER', params, None)
        if stepper_name is None:
            self.report_stats()
            return
        info = self.tracking.get(stepper_name)
        if info is None:
            raise self.gcode.error("Stats not available for stepper %s" % (
                stepper_name,))
        endstop_phase = self.generate_stats(stepper_name, info)
        configfile = self.printer.lookup_object('configfile')
        section = 'endstop_phase %s' % stepper_name
        configfile.remove_section(section)
        configfile.set(section, "endstop_phase", endstop_phase)
        self.gcode.respond_info(
            "The SAVE_CONFIG command will update the printer config\n"
            "file with these parameters and restart the printer.")
    def generate_stats(self, stepper_name, info):
        get_phase, phase_history = info
        wph = phase_history + phase_history
        count = sum(phase_history)
        phases = len(phase_history)
        half_phases = phases // 2
        res = []
        for i in range(phases):
            phase = i + half_phases
            cost = sum([wph[j] * abs(j-phase) for j in range(i, i+phases)])
            res.append((cost, phase))
        res.sort()
        best = res[0][1]
        found = [j for j in range(best - half_phases, best + half_phases)
                 if wph[j]]
        best_phase = best % phases
        lo, hi = found[0] % phases, found[-1] % phases
        self.gcode.respond_info("%s: endstop_phase=%d (range %d to %d)" % (
            stepper_name, best_phase, lo, hi))
        return best_phase
    def report_stats(self):
        if not self.tracking:
            self.gcode.respond_info(
                "No steppers found. (Be sure to home at least once.)")
            return
        for stepper_name, info in sorted(self.tracking.items()):
            if info is None:
                continue
            self.generate_stats(stepper_name, info)

def load_config_prefix(config):
    return EndstopPhase(config)

def load_config(config):
    return EndstopPhases(config)
