# Code for handling printer nozzle extruders
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import stepper, homing, chelper

EXTRUDE_DIFF_IGNORE = 1.02

class PrinterExtruder:
    def __init__(self, config, extruder_num):
        self.printer = config.get_printer()
        self.name = config.get_name()
        shared_heater = config.get('shared_heater', None)
        pheater = self.printer.lookup_object('heater')
        gcode_id = 'T%d' % (extruder_num,)
        if shared_heater is None:
            self.heater = pheater.setup_heater(config, gcode_id)
        else:
            self.heater = pheater.lookup_heater(shared_heater)
        self.stepper = stepper.PrinterStepper(config)
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
        self.stepper.set_max_jerk(9999999.9, 9999999.9)
        self.max_e_dist = config.getfloat(
            'max_extrude_only_distance', 50., minval=0.)
        gcode_macro = self.printer.try_load_module(config, 'gcode_macro')
        self.activate_gcode = gcode_macro.load_template(
            config, 'activate_gcode', '')
        self.deactivate_gcode = gcode_macro.load_template(
            config, 'deactivate_gcode', '')
        self.pressure_advance = config.getfloat(
            'pressure_advance', 0., minval=0.)
        self.pressure_advance_lookahead_time = config.getfloat(
            'pressure_advance_lookahead_time', 0.010, minval=0.)
        self.need_motor_enable = True
        self.extrude_pos = 0.
        # Setup iterative solver
        ffi_main, ffi_lib = chelper.get_ffi()
        self.cmove = ffi_main.gc(ffi_lib.move_alloc(), ffi_lib.free)
        self.extruder_move_fill = ffi_lib.extruder_move_fill
        self.stepper.setup_itersolve('extruder_stepper_alloc')
        # Setup SET_PRESSURE_ADVANCE command
        gcode = self.printer.lookup_object('gcode')
        if self.name in ('extruder', 'extruder0'):
            gcode.register_mux_command("SET_PRESSURE_ADVANCE", "EXTRUDER", None,
                                       self.cmd_default_SET_PRESSURE_ADVANCE,
                                       desc=self.cmd_SET_PRESSURE_ADVANCE_help)
        gcode.register_mux_command("SET_PRESSURE_ADVANCE", "EXTRUDER",
                                   self.name, self.cmd_SET_PRESSURE_ADVANCE,
                                   desc=self.cmd_SET_PRESSURE_ADVANCE_help)
    def get_status(self, eventtime):
        return dict(
            self.get_heater().get_status(eventtime),
            pressure_advance=self.pressure_advance,
            lookahead_time=self.pressure_advance_lookahead_time
        )
    def get_heater(self):
        return self.heater
    def set_active(self, print_time, is_active):
        return self.extrude_pos
    def get_activate_gcode(self, is_active):
        if is_active:
            return self.activate_gcode.render()
        return self.deactivate_gcode.render()
    def stats(self, eventtime):
        return self.heater.stats(eventtime)
    def motor_off(self, print_time):
        self.stepper.motor_enable(print_time, 0)
        self.need_motor_enable = True
    def check_move(self, move):
        move.extrude_r = move.axes_d[3] / move.move_d
        move.extrude_max_corner_v = 0.
        if not self.heater.can_extrude:
            raise homing.EndstopError(
                "Extrude below minimum temp\n"
                "See the 'min_extrude_temp' config option for details")
        if not move.is_kinematic_move or move.extrude_r < 0.:
            # Extrude only move (or retraction move) - limit accel and velocity
            if abs(move.axes_d[3]) > self.max_e_dist:
                raise homing.EndstopError(
                    "Extrude only move too long (%.3fmm vs %.3fmm)\n"
                    "See the 'max_extrude_only_distance' config"
                    " option for details" % (move.axes_d[3], self.max_e_dist))
            inv_extrude_r = 1. / abs(move.extrude_r)
            move.limit_speed(self.max_e_velocity * inv_extrude_r
                             , self.max_e_accel * inv_extrude_r)
        elif move.extrude_r > self.max_extrude_ratio:
            if move.axes_d[3] <= self.nozzle_diameter * self.max_extrude_ratio:
                # Permit extrusion if amount extruded is tiny
                move.extrude_r = self.max_extrude_ratio
                return
            area = move.axes_d[3] * self.filament_area / move.move_d
            logging.debug("Overextrude: %s vs %s (area=%.3f dist=%.3f)",
                          move.extrude_r, self.max_extrude_ratio,
                          area, move.move_d)
            raise homing.EndstopError(
                "Move exceeds maximum extrusion (%.3fmm^2 vs %.3fmm^2)\n"
                "See the 'max_extrude_cross_section' config option for details"
                % (area, self.max_extrude_ratio * self.filament_area))
    def calc_junction(self, prev_move, move):
        extrude = move.axes_d[3]
        prev_extrude = prev_move.axes_d[3]
        if extrude or prev_extrude:
            if not extrude or not prev_extrude:
                # Extrude move to non-extrude move - disable lookahead
                return 0.
            if ((move.extrude_r > prev_move.extrude_r * EXTRUDE_DIFF_IGNORE
                 or prev_move.extrude_r > move.extrude_r * EXTRUDE_DIFF_IGNORE)
                and abs(move.move_d * prev_move.extrude_r - extrude) >= .001):
                # Extrude ratio between moves is too different
                return 0.
            move.extrude_r = prev_move.extrude_r
        return move.max_cruise_v2
    def lookahead(self, moves, flush_count, lazy):
        lookahead_t = self.pressure_advance_lookahead_time
        if not self.pressure_advance or not lookahead_t:
            return flush_count
        # Calculate max_corner_v - the speed the head will accelerate
        # to after cornering.
        for i in range(flush_count):
            move = moves[i]
            if not move.decel_t:
                continue
            cruise_v = move.cruise_v
            max_corner_v = 0.
            sum_t = lookahead_t
            for j in range(i+1, flush_count):
                fmove = moves[j]
                if not fmove.max_start_v2:
                    break
                if fmove.cruise_v > max_corner_v:
                    if (not max_corner_v
                        and not fmove.accel_t and not fmove.cruise_t):
                        # Start timing after any full decel moves
                        continue
                    if sum_t >= fmove.accel_t:
                        max_corner_v = fmove.cruise_v
                    else:
                        max_corner_v = max(
                            max_corner_v, fmove.start_v + fmove.accel * sum_t)
                    if max_corner_v >= cruise_v:
                        break
                sum_t -= fmove.accel_t + fmove.cruise_t + fmove.decel_t
                if sum_t <= 0.:
                    break
            else:
                if lazy:
                    return i
            move.extrude_max_corner_v = max_corner_v
        return flush_count
    def move(self, print_time, move):
        if self.need_motor_enable:
            self.stepper.motor_enable(print_time, 1)
            self.need_motor_enable = False
        axis_d = move.axes_d[3]
        axis_r = axis_d / move.move_d
        accel = move.accel * axis_r
        start_v = move.start_v * axis_r
        cruise_v = move.cruise_v * axis_r
        accel_t, cruise_t, decel_t = move.accel_t, move.cruise_t, move.decel_t

        # Update for pressure advance
        extra_accel_v = extra_decel_v = 0.
        start_pos = self.extrude_pos
        if (axis_d >= 0. and (move.axes_d[0] or move.axes_d[1])
            and self.pressure_advance):
            # Calculate extra_accel_v
            pressure_advance = self.pressure_advance * move.extrude_r
            prev_pressure_d = start_pos - move.start_pos[3]
            if accel_t:
                npd = move.cruise_v * pressure_advance
                extra_accel_d = npd - prev_pressure_d
                if extra_accel_d > 0.:
                    extra_accel_v = extra_accel_d / accel_t
                    axis_d += extra_accel_d
                    prev_pressure_d += extra_accel_d
            # Calculate extra_decel_v
            emcv = move.extrude_max_corner_v
            if decel_t and emcv < move.cruise_v:
                npd = max(emcv, move.end_v) * pressure_advance
                extra_decel_d = npd - prev_pressure_d
                if extra_decel_d < 0.:
                    axis_d += extra_decel_d
                    extra_decel_v = extra_decel_d / decel_t

        # Generate steps
        self.extruder_move_fill(
            self.cmove, print_time, accel_t, cruise_t, decel_t, start_pos,
            start_v, cruise_v, accel, extra_accel_v, extra_decel_v)
        self.stepper.step_itersolve(self.cmove)
        self.extrude_pos = start_pos + axis_d
    cmd_SET_PRESSURE_ADVANCE_help = "Set pressure advance parameters"
    def cmd_default_SET_PRESSURE_ADVANCE(self, params):
        extruder = self.printer.lookup_object('toolhead').get_extruder()
        extruder.cmd_SET_PRESSURE_ADVANCE(params)
    def cmd_SET_PRESSURE_ADVANCE(self, params):
        self.printer.lookup_object('toolhead').get_last_move_time()
        gcode = self.printer.lookup_object('gcode')
        pressure_advance = gcode.get_float(
            'ADVANCE', params, self.pressure_advance, minval=0.)
        pressure_advance_lookahead_time = gcode.get_float(
            'ADVANCE_LOOKAHEAD_TIME', params,
            self.pressure_advance_lookahead_time, minval=0.)
        self.pressure_advance = pressure_advance
        self.pressure_advance_lookahead_time = pressure_advance_lookahead_time
        msg = ("pressure_advance: %.6f\n"
               "pressure_advance_lookahead_time: %.6f" % (
                   pressure_advance, pressure_advance_lookahead_time))
        self.printer.set_rollover_info(self.name, "%s: %s" % (self.name, msg))
        gcode.respond_info(msg, log=False)

# Dummy extruder class used when a printer has no extruder at all
class DummyExtruder:
    def set_active(self, print_time, is_active):
        return 0.
    def motor_off(self, move_time):
        pass
    def check_move(self, move):
        raise homing.EndstopMoveError(
            move.end_pos, "Extrude when no extruder present")
    def calc_junction(self, prev_move, move):
        return move.max_cruise_v2
    def lookahead(self, moves, flush_count, lazy):
        return flush_count

def add_printer_objects(config):
    printer = config.get_printer()
    for i in range(99):
        section = 'extruder%d' % (i,)
        if not config.has_section(section):
            if not i and config.has_section('extruder'):
                pe = PrinterExtruder(config.getsection('extruder'), 0)
                printer.add_object('extruder0', pe)
                continue
            break
        pe = PrinterExtruder(config.getsection(section), i)
        printer.add_object(section, pe)

def get_printer_extruders(printer):
    out = []
    for i in range(99):
        extruder = printer.lookup_object('extruder%d' % (i,), None)
        if extruder is None:
            break
        out.append(extruder)
    return out
