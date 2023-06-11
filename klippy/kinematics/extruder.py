# Code for handling printer nozzle extruders
#
# Copyright (C) 2016-2022  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import stepper, chelper

class ExtruderSmoother:
    def __init__(self, config, pa_model):
        self.smooth_time = config.getfloat(
                'pressure_advance_smooth_time', 0.040, above=0., maxval=.200)
        # A 4-th order smoothing function that goes to 0 together with
        # its derivative at the ends of the smoothing interval
        self.a = [15./8., 0., -15., 0., 30.]
        self.pa_model = pa_model
        self.axes = ['x', 'y', 'z']
    def update(self, gcmd):
        self.smooth_time = gcmd.get_float('SMOOTH_TIME', self.smooth_time)
    def update_pa_model(self, pa_model):
        self.pa_model = pa_model
    def enable_axis(self, axis):
        if axis not in self.axes:
            self.axes.append(axis)
    def disable_axis(self, axis):
        if axis in self.axes:
            self.axes.remove(axis)
    def update_extruder_kinematics(self, extruder_sk):
        ffi_main, ffi_lib = chelper.get_ffi()
        n = len(self.a)
        success = True
        smooth_time = self.smooth_time if self.pa_model.enabled() else 0.
        for axis in self.axes:
            if not ffi_lib.extruder_set_smoothing_params(
                    extruder_sk, axis.encode(), n, self.a,
                    smooth_time, 0.) == 0:
                success = False
        return success
    def get_status(self, eventtime):
        return {'smooth_time': self.smooth_time}
    def get_msg(self):
        return "pressure_advance_smooth_time: %.6f" % (self.smooth_time,)

class PALinearModel:
    name = 'linear'
    def __init__(self, config=None):
        if config:
            self.pressure_advance = config.getfloat(
                    'pressure_advance', 0., minval=0.)
        else:
            self.pressure_advance = 0.
    def update(self, gcmd):
        self.pressure_advance = gcmd.get_float(
                'ADVANCE', self.pressure_advance, minval=0.)
    def enabled(self):
        return self.pressure_advance > 0.
    def get_pa_params(self):
        return (self.pressure_advance,)
    def get_status(self, eventtime):
        return {'pressure_advance': self.pressure_advance}
    def get_msg(self):
        return 'pressure_advance: %.6f' % (self.pressure_advance,)
    def get_func(self):
        ffi_main, ffi_lib = chelper.get_ffi()
        return ffi_lib.pressure_advance_linear_model_func

class PANonLinearModel:
    def __init__(self, config=None):
        if config:
            self.linear_advance = config.getfloat('linear_advance',
                                                  0., minval=0.)
            self.linear_offset = config.getfloat('linear_offset',
                                                  0., minval=0.)
            if self.linear_offset:
                self.linearization_velocity = config.getfloat(
                        'linearization_velocity', above=0.)
            else:
                self.linearization_velocity = config.getfloat(
                        'linearization_velocity', 0., minval=0.)
        else:
            self.linear_advance = 0.
            self.linear_offset = 0.
            self.linearization_velocity = 0.
    def update(self, gcmd):
        self.linear_advance = gcmd.get_float(
                'ADVANCE', self.linear_advance, minval=0.)
        self.linear_offset = gcmd.get_float(
                'OFFSET', self.linear_offset, minval=0.)
        self.linearization_velocity = gcmd.get_float(
                'VELOCITY', self.linearization_velocity)
        if self.linear_offset and self.linearization_velocity <= 0.:
            raise gcmd.error('VELOCITY must be set to a positive value '
                             'when OFFSET is non-zero')
    def enabled(self):
        return self.linear_advance > 0. or self.linear_offset > 0.
    def get_pa_params(self):
        # The order must match the order of parameters in the
        # pressure_advance_params struct in kin_extruder.c
        return (self.linear_advance, self.linear_offset,
                self.linearization_velocity)
    def get_status(self, eventtime):
        return {'linear_advance': self.linear_advance,
                'linear_offset': self.linear_offset,
                'linearization_velocity': self.linearization_velocity}
    def get_msg(self):
        return ('linear_advance: %.6f\n'
                'linear_offset: %.6f\n'
                'linearization_velocity: %.6f' % (
                    self.linear_advance,
                    self.linear_offset,
                    self.linearization_velocity))
    def get_func(self):
        return None

class PATanhModel(PANonLinearModel):
    name = 'tanh'
    def __init__(self, config=None):
        PANonLinearModel.__init__(self, config)
    def get_func(self):
        ffi_main, ffi_lib = chelper.get_ffi()
        return ffi_lib.pressure_advance_tanh_model_func

class PAReciprModel(PANonLinearModel):
    name = 'recipr'
    def __init__(self, config=None):
        PANonLinearModel.__init__(self, config)
    def get_func(self):
        ffi_main, ffi_lib = chelper.get_ffi()
        return ffi_lib.pressure_advance_recipr_model_func

class ExtruderStepper:
    pa_models = {PALinearModel.name: PALinearModel,
                 PATanhModel.name: PATanhModel,
                 PAReciprModel.name: PAReciprModel}
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.pa_model = config.getchoice('pressure_advance_model',
                                         self.pa_models,
                                         PALinearModel.name)(config)
        self.smoother = ExtruderSmoother(config, self.pa_model)
        self.pressure_advance_time_offset = config.getfloat(
                'pressure_advance_time_offset', 0.0, minval=-0.2, maxval=0.2)
        # Setup stepper
        self.stepper = stepper.PrinterStepper(config)
        ffi_main, ffi_lib = chelper.get_ffi()
        self.sk_extruder = ffi_main.gc(ffi_lib.extruder_stepper_alloc(),
                                       ffi_lib.free)
        self.stepper.set_stepper_kinematics(self.sk_extruder)
        ffi_lib.extruder_set_pressure_advance_model_func(
                self.sk_extruder, self.pa_model.get_func())
        self.motion_queue = None
        self.extruder = None
        # Register commands
        self.printer.register_event_handler("klippy:connect",
                                            self._handle_connect)
        gcode = self.printer.lookup_object('gcode')
        if self.name == 'extruder':
            gcode.register_mux_command("SET_PRESSURE_ADVANCE", "EXTRUDER", None,
                                       self.cmd_default_SET_PRESSURE_ADVANCE,
                                       desc=self.cmd_SET_PRESSURE_ADVANCE_help)
        gcode.register_mux_command("SET_PRESSURE_ADVANCE", "EXTRUDER",
                                   self.name, self.cmd_SET_PRESSURE_ADVANCE,
                                   desc=self.cmd_SET_PRESSURE_ADVANCE_help)
        gcode.register_mux_command("SET_EXTRUDER_ROTATION_DISTANCE", "EXTRUDER",
                                   self.name, self.cmd_SET_E_ROTATION_DISTANCE,
                                   desc=self.cmd_SET_E_ROTATION_DISTANCE_help)
        gcode.register_mux_command("SYNC_EXTRUDER_MOTION", "EXTRUDER",
                                   self.name, self.cmd_SYNC_EXTRUDER_MOTION,
                                   desc=self.cmd_SYNC_EXTRUDER_MOTION_help)
        gcode.register_mux_command("SET_EXTRUDER_STEP_DISTANCE", "EXTRUDER",
                                   self.name, self.cmd_SET_E_STEP_DISTANCE,
                                   desc=self.cmd_SET_E_STEP_DISTANCE_help)
        gcode.register_mux_command("SYNC_STEPPER_TO_EXTRUDER", "STEPPER",
                                   self.name, self.cmd_SYNC_STEPPER_TO_EXTRUDER,
                                   desc=self.cmd_SYNC_STEPPER_TO_EXTRUDER_help)
    def _handle_connect(self):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_step_generator(self.stepper.generate_steps)
        self._update_pressure_advance(self.pa_model,
                                      self.pressure_advance_time_offset)
        self.smoother.update_extruder_kinematics(self.sk_extruder)
    def get_status(self, eventtime):
        sts = {'pressure_advance_model': self.pa_model.name,
               'time_offset': self.pressure_advance_time_offset,
               'motion_queue': self.motion_queue}
        sts.update(self.pa_model.get_status(eventtime))
        sts.update(self.smoother.get_status(eventtime))
        return sts
    def find_past_position(self, print_time):
        mcu_pos = self.stepper.get_past_mcu_position(print_time)
        return self.stepper.mcu_to_commanded_position(mcu_pos)
    def sync_to_extruder(self, extruder_name):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        if not extruder_name:
            if self.extruder is not None:
                self.extruder.unlink_extruder_stepper(self)
            self.motion_queue = None
            self.extruder = None
            return
        extruder = self.printer.lookup_object(extruder_name, None)
        if extruder is None or not isinstance(extruder, PrinterExtruder):
            raise self.printer.command_error("'%s' is not a valid extruder."
                                             % (extruder_name,))
        extruder.link_extruder_stepper(self)
        self.motion_queue = extruder_name
        self.extruder = extruder
    def _update_pressure_advance(self, pa_model, time_offset):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        ffi_main, ffi_lib = chelper.get_ffi()
        old_delay = ffi_lib.extruder_get_step_gen_window(self.sk_extruder)
        if self.pa_model.name != pa_model.name:
            pa_func = pa_model.get_func()
            ffi_lib.extruder_set_pressure_advance_model_func(self.sk_extruder,
                                                             pa_func)
        pa_params = pa_model.get_pa_params()
        ffi_lib.extruder_set_pressure_advance(
                self.sk_extruder, len(pa_params), pa_params, time_offset)
        self.smoother.update_pa_model(pa_model)
        self.smoother.update_extruder_kinematics(self.sk_extruder)
        new_delay = ffi_lib.extruder_get_step_gen_window(self.sk_extruder)
        if old_delay != new_delay:
            toolhead.note_step_generation_scan_time(new_delay, old_delay)
        self.pa_model = pa_model
        self.pressure_advance_time_offset = time_offset
    def update_input_shaping(self, shapers, exact_mode):
        ffi_main, ffi_lib = chelper.get_ffi()
        old_delay = ffi_lib.extruder_get_step_gen_window(self.sk_extruder)
        failed_shapers = []
        for shaper in shapers:
            if not shaper.update_extruder_kinematics(self.sk_extruder,
                                                     exact_mode):
                failed_shapers.append(shaper)
            # Pressure advance requires extruder smoothing, make sure that
            # some smoothing is enabled
            if shaper.is_extruder_smoothing(exact_mode) and shaper.is_enabled():
                self.smoother.disable_axis(shaper.get_axis())
            else:
                self.smoother.enable_axis(shaper.get_axis())
        self.smoother.update_extruder_kinematics(self.sk_extruder)
        new_delay = ffi_lib.extruder_get_step_gen_window(self.sk_extruder)
        toolhead = self.printer.lookup_object('toolhead')
        if old_delay != new_delay:
            toolhead.note_step_generation_scan_time(new_delay, old_delay)
        return failed_shapers
    cmd_SET_PRESSURE_ADVANCE_help = "Set pressure advance parameters"
    def cmd_default_SET_PRESSURE_ADVANCE(self, gcmd):
        extruder = self.printer.lookup_object('toolhead').get_extruder()
        extruder_steppers = extruder.get_extruder_steppers()
        if not extruder_steppers:
            raise gcmd.error("Active extruder does not have a stepper")
        for extruder_stepper in extruder_steppers:
            strapq = extruder_stepper.stepper.get_trapq()
            if strapq is not extruder.get_trapq():
                raise gcmd.error("Unable to infer active extruder stepper")
            extruder_stepper.cmd_SET_PRESSURE_ADVANCE(gcmd)
    def cmd_SET_PRESSURE_ADVANCE(self, gcmd):
        pa_model_name = gcmd.get('MODEL', self.pa_model.name)
        if pa_model_name not in self.pa_models:
            raise gcmd.error("Invalid MODEL='%s' choice" % (pa_model_name,))
        pa_model = self.pa_model
        if pa_model_name != self.pa_model.name:
            pa_model = self.pa_models[pa_model_name]()
        pa_model.update(gcmd)
        self.smoother.update(gcmd)
        time_offset = gcmd.get_float('TIME_OFFSET',
                                     self.pressure_advance_time_offset,
                                     minval=-0.2, maxval=0.2)
        self._update_pressure_advance(pa_model, time_offset)
        msg = ("pressure_advance_model: %s\n" % (pa_model.name,) +
               pa_model.get_msg() + "\n" +
               self.smoother.get_msg() +
               "\npressure_advance_time_offset: %.6f"
               % (time_offset,))
        self.printer.set_rollover_info(self.name, "%s: %s" % (self.name, msg))
        gcmd.respond_info(msg, log=False)
    cmd_SET_E_ROTATION_DISTANCE_help = "Set extruder rotation distance"
    def cmd_SET_E_ROTATION_DISTANCE(self, gcmd):
        rotation_dist = gcmd.get_float('DISTANCE', None)
        if rotation_dist is not None:
            if not rotation_dist:
                raise gcmd.error("Rotation distance can not be zero")
            invert_dir, orig_invert_dir = self.stepper.get_dir_inverted()
            next_invert_dir = orig_invert_dir
            if rotation_dist < 0.:
                next_invert_dir = not orig_invert_dir
                rotation_dist = -rotation_dist
            toolhead = self.printer.lookup_object('toolhead')
            toolhead.flush_step_generation()
            self.stepper.set_rotation_distance(rotation_dist)
            self.stepper.set_dir_inverted(next_invert_dir)
        else:
            rotation_dist, spr = self.stepper.get_rotation_distance()
        invert_dir, orig_invert_dir = self.stepper.get_dir_inverted()
        if invert_dir != orig_invert_dir:
            rotation_dist = -rotation_dist
        gcmd.respond_info("Extruder '%s' rotation distance set to %0.6f"
                          % (self.name, rotation_dist))
    cmd_SYNC_EXTRUDER_MOTION_help = "Set extruder stepper motion queue"
    def cmd_SYNC_EXTRUDER_MOTION(self, gcmd):
        ename = gcmd.get('MOTION_QUEUE')
        self.sync_to_extruder(ename)
        gcmd.respond_info("Extruder '%s' now syncing with '%s'"
                          % (self.name, ename))
    cmd_SET_E_STEP_DISTANCE_help = "Set extruder step distance"
    def cmd_SET_E_STEP_DISTANCE(self, gcmd):
        step_dist = gcmd.get_float('DISTANCE', None, above=0.)
        if step_dist is not None:
            toolhead = self.printer.lookup_object('toolhead')
            toolhead.flush_step_generation()
            rd, steps_per_rotation = self.stepper.get_rotation_distance()
            self.stepper.set_rotation_distance(step_dist * steps_per_rotation)
        else:
            step_dist = self.stepper.get_step_dist()
        gcmd.respond_info("Extruder '%s' step distance set to %0.6f"
                          % (self.name, step_dist))
    cmd_SYNC_STEPPER_TO_EXTRUDER_help = "Set extruder stepper"
    def cmd_SYNC_STEPPER_TO_EXTRUDER(self, gcmd):
        ename = gcmd.get('EXTRUDER')
        self.sync_to_extruder(ename)
        gcmd.respond_info("Extruder '%s' now syncing with '%s'"
                          % (self.name, ename))

# Tracking for hotend heater, extrusion motion queue, and extruder stepper
class PrinterExtruder:
    def __init__(self, config, extruder_num):
        self.printer = config.get_printer()
        self.name = config.get_name()
        self.last_position = [0., 0., 0.]
        # Setup hotend heater
        shared_heater = config.get('shared_heater', None)
        pheaters = self.printer.load_object(config, 'heaters')
        gcode_id = 'T%d' % (extruder_num,)
        if shared_heater is None:
            self.heater = pheaters.setup_heater(config, gcode_id)
        else:
            config.deprecate('shared_heater')
            self.heater = pheaters.lookup_heater(shared_heater)
        # Setup kinematic checks
        self.nozzle_diameter = config.getfloat('nozzle_diameter', above=0.)
        filament_diameter = config.getfloat(
            'filament_diameter', minval=self.nozzle_diameter)
        self.filament_area = math.pi * (filament_diameter * .5)**2
        def_max_cross_section = 4. * self.nozzle_diameter**2
        def_max_extrude_ratio = def_max_cross_section / self.filament_area
        max_cross_section = config.getfloat(
            'max_extrude_cross_section', def_max_cross_section, above=0.)
        self.max_extrude_ratio = max_cross_section / self.filament_area
        logging.info("Extruder max_extrude_ratio=%.6f", self.max_extrude_ratio)
        toolhead = self.printer.lookup_object('toolhead')
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_e_velocity = config.getfloat(
            'max_extrude_only_velocity', max_velocity * def_max_extrude_ratio
            , above=0.)
        self.max_e_accel = config.getfloat(
            'max_extrude_only_accel', max_accel * def_max_extrude_ratio
            , above=0.)
        self.max_e_dist = config.getfloat(
            'max_extrude_only_distance', 50., minval=0.)
        self.instant_corner_v = config.getfloat(
            'instantaneous_corner_velocity', 1., minval=0.)
        # Setup extruder trapq (trapezoidal motion queue)
        ffi_main, ffi_lib = chelper.get_ffi()
        self.trapq = ffi_main.gc(ffi_lib.trapq_alloc(), ffi_lib.trapq_free)
        self.trapq_append = ffi_lib.trapq_append
        self.trapq_finalize_moves = ffi_lib.trapq_finalize_moves
        # Setup extruder stepper
        self.extruder_steppers = []
        if (config.get('step_pin', None) is not None
            or config.get('dir_pin', None) is not None
            or config.get('rotation_distance', None) is not None):
            self.link_extruder_stepper(ExtruderStepper(config))
        # Register commands
        gcode = self.printer.lookup_object('gcode')
        if self.name == 'extruder':
            toolhead.set_extruder(self, 0.)
            gcode.register_command("M104", self.cmd_M104)
            gcode.register_command("M109", self.cmd_M109)
        gcode.register_mux_command("ACTIVATE_EXTRUDER", "EXTRUDER",
                                   self.name, self.cmd_ACTIVATE_EXTRUDER,
                                   desc=self.cmd_ACTIVATE_EXTRUDER_help)
    def link_extruder_stepper(self, extruder_stepper):
        if extruder_stepper not in self.extruder_steppers:
            self.extruder_steppers.append(extruder_stepper)
            extruder_stepper.stepper.set_position(self.last_position)
            extruder_stepper.stepper.set_trapq(self.trapq)
    def unlink_extruder_stepper(self, extruder_stepper):
        if extruder_stepper in self.extruder_steppers:
            self.extruder_steppers.remove(extruder_stepper)
            extruder_stepper.stepper.set_trapq(None)
    def get_extruder_steppers(self):
        return self.extruder_steppers
    def update_move_time(self, flush_time):
        self.trapq_finalize_moves(self.trapq, flush_time)
    def get_status(self, eventtime):
        sts = self.heater.get_status(eventtime)
        sts['can_extrude'] = self.heater.can_extrude
        if self.extruder_steppers:
            sts.update(self.extruder_steppers[0].get_status(eventtime))
        return sts
    def get_name(self):
        return self.name
    def get_heater(self):
        return self.heater
    def get_trapq(self):
        return self.trapq
    def stats(self, eventtime):
        return self.heater.stats(eventtime)
    def check_move(self, move):
        axis_r = move.axes_r[3]
        if not self.heater.can_extrude:
            raise self.printer.command_error(
                "Extrude below minimum temp\n"
                "See the 'min_extrude_temp' config option for details")
        if (not move.axes_d[0] and not move.axes_d[1]) or axis_r < 0.:
            # Extrude only move (or retraction move) - limit accel and velocity
            if abs(move.axes_d[3]) > self.max_e_dist:
                raise self.printer.command_error(
                    "Extrude only move too long (%.3fmm vs %.3fmm)\n"
                    "See the 'max_extrude_only_distance' config"
                    " option for details" % (move.axes_d[3], self.max_e_dist))
            inv_extrude_r = 1. / abs(axis_r)
            move.limit_speed(self.max_e_velocity * inv_extrude_r,
                             self.max_e_accel * inv_extrude_r)
        elif axis_r > self.max_extrude_ratio:
            if move.axes_d[3] <= self.nozzle_diameter * self.max_extrude_ratio:
                # Permit extrusion if amount extruded is tiny
                return
            area = axis_r * self.filament_area
            logging.debug("Overextrude: %s vs %s (area=%.3f dist=%.3f)",
                          axis_r, self.max_extrude_ratio, area, move.move_d)
            raise self.printer.command_error(
                "Move exceeds maximum extrusion (%.3fmm^2 vs %.3fmm^2)\n"
                "See the 'max_extrude_cross_section' config option for details"
                % (area, self.max_extrude_ratio * self.filament_area))
    def calc_junction(self, prev_move, move):
        diff_r = move.axes_r[3] - prev_move.axes_r[3]
        if diff_r:
            return (self.instant_corner_v / abs(diff_r))**2
        return move.max_cruise_v2
    def move(self, print_time, move):
        axis_r = move.axes_r[3]
        abs_axis_r = abs(axis_r)
        accel = move.accel * abs_axis_r
        start_v = move.start_v * abs_axis_r
        cruise_v = move.cruise_v * abs_axis_r
        extr_pos = self.last_position
        if move.is_kinematic_move:
            # Regular kinematic move with extrusion
            extr_r = [math.copysign(r * r, axis_r) for r in move.axes_r[:3]]
        else:
            # Extrude-only move, do not apply pressure advance
            extr_r = [0., 0., axis_r]
        self.trapq_append(self.trapq, print_time,
                          move.accel_t, move.cruise_t, move.decel_t,
                          extr_pos[0], extr_pos[1], extr_pos[2],
                          extr_r[0], extr_r[1], extr_r[2],
                          start_v, cruise_v, accel)
        extr_d = abs(move.axes_d[3])
        for i in range(3):
            self.last_position[i] += extr_d * extr_r[i]
    def find_past_position(self, print_time):
        if not self.extruder_steppers:
            return 0.
        return self.extruder_steppers[0].find_past_position(print_time)
    def cmd_M104(self, gcmd, wait=False):
        # Set Extruder Temperature
        temp = gcmd.get_float('S', 0.)
        index = gcmd.get_int('T', None, minval=0)
        if index is not None:
            section = 'extruder'
            if index:
                section = 'extruder%d' % (index,)
            extruder = self.printer.lookup_object(section, None)
            if extruder is None:
                if temp <= 0.:
                    return
                raise gcmd.error("Extruder not configured")
        else:
            extruder = self.printer.lookup_object('toolhead').get_extruder()
        pheaters = self.printer.lookup_object('heaters')
        pheaters.set_temperature(extruder.get_heater(), temp, wait)
    def cmd_M109(self, gcmd):
        # Set Extruder Temperature and Wait
        self.cmd_M104(gcmd, wait=True)
    cmd_ACTIVATE_EXTRUDER_help = "Change the active extruder"
    def cmd_ACTIVATE_EXTRUDER(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead')
        if toolhead.get_extruder() is self:
            gcmd.respond_info("Extruder %s already active" % (self.name,))
            return
        gcmd.respond_info("Activating extruder %s" % (self.name,))
        toolhead.flush_step_generation()
        toolhead.set_extruder(self, sum(self.last_position))
        self.printer.send_event("extruder:activate_extruder")

# Dummy extruder class used when a printer has no extruder at all
class DummyExtruder:
    def __init__(self, printer):
        self.printer = printer
    def update_move_time(self, flush_time):
        pass
    def check_move(self, move):
        raise move.move_error("Extrude when no extruder present")
    def find_past_position(self, print_time):
        return 0.
    def calc_junction(self, prev_move, move):
        return move.max_cruise_v2
    def get_name(self):
        return ""
    def get_extruder_steppers(self):
        return []
    def get_heater(self):
        raise self.printer.command_error("Extruder not configured")
    def get_trapq(self):
        raise self.printer.command_error("Extruder not configured")

def add_printer_objects(config):
    printer = config.get_printer()
    for i in range(99):
        section = 'extruder'
        if i:
            section = 'extruder%d' % (i,)
        if not config.has_section(section):
            break
        pe = PrinterExtruder(config.getsection(section), i)
        printer.add_object(section, pe)
