# Code for handling printer nozzle extruders
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import stepper, heater, homing

EXTRUDE_DIFF_IGNORE = 1.02

class PrinterExtruder:
    def __init__(self, printer, config):
        self.config = config
        self.heater = heater.PrinterHeater(printer, config)
        self.stepper = stepper.PrinterStepper(printer, config)
        self.nozzle_diameter = config.getfloat('nozzle_diameter', above=0.)
        filament_diameter = config.getfloat(
            'filament_diameter', minval=self.nozzle_diameter)
        self.filament_area = math.pi * (filament_diameter * .5)**2
        max_cross_section = config.getfloat(
            'max_extrude_cross_section', 4. * self.nozzle_diameter**2
            , above=0.)
        self.max_extrude_ratio = max_cross_section / self.filament_area
        toolhead = printer.objects['toolhead']
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_e_velocity = self.config.getfloat(
            'max_extrude_only_velocity', max_velocity * self.max_extrude_ratio
            , above=0.)
        self.max_e_accel = self.config.getfloat(
            'max_extrude_only_accel', max_accel * self.max_extrude_ratio
            , above=0.)
        self.stepper.set_max_jerk(9999999.9, 9999999.9)
        self.max_e_dist = config.getfloat(
            'max_extrude_only_distance', 50., minval=0.)
        self.activate_gcode = config.get('activate_gcode', '')
        self.deactivate_gcode = config.get('deactivate_gcode', '')
        self.pressure_advance = config.getfloat(
            'pressure_advance', 0., minval=0.)
        self.pressure_advance_lookahead_time = 0.
        if self.pressure_advance:
            self.pressure_advance_lookahead_time = config.getfloat(
                'pressure_advance_lookahead_time', 0.010, minval=0.)
        self.need_motor_enable = True
        self.extrude_pos = 0.
    def get_heater(self):
        return self.heater
    def set_active(self, print_time, is_active):
        return self.extrude_pos
    def get_activate_gcode(self, is_active):
        if is_active:
            return self.activate_gcode
        return self.deactivate_gcode
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
        elif (move.extrude_r > self.max_extrude_ratio
              and move.axes_d[3] > self.nozzle_diameter*self.max_extrude_ratio):
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
        if not lookahead_t:
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
        axis_r = abs(axis_d) / move.move_d
        accel = move.accel * axis_r
        start_v = move.start_v * axis_r
        cruise_v = move.cruise_v * axis_r
        end_v = move.end_v * axis_r
        accel_t, cruise_t, decel_t = move.accel_t, move.cruise_t, move.decel_t
        accel_d = move.accel_r * axis_d
        cruise_d = move.cruise_r * axis_d
        decel_d = move.decel_r * axis_d

        retract_t = retract_d = retract_v = 0.
        decel_v = cruise_v

        # Update for pressure advance
        start_pos = self.extrude_pos
        if (axis_d >= 0. and (move.axes_d[0] or move.axes_d[1])
            and self.pressure_advance):
            # Increase accel_d and start_v when accelerating
            pressure_advance = self.pressure_advance * move.extrude_r
            prev_pressure_d = start_pos - move.start_pos[3]
            if accel_d:
                npd = move.cruise_v * pressure_advance
                extra_accel_d = npd - prev_pressure_d
                if extra_accel_d > 0.:
                    accel_d += extra_accel_d
                    start_v += extra_accel_d / accel_t
                    prev_pressure_d += extra_accel_d
            # Update decel and retract parameters when decelerating
            emcv = move.extrude_max_corner_v
            if decel_d and emcv < move.cruise_v:
                npd = max(emcv, move.end_v) * pressure_advance
                extra_decel_d = prev_pressure_d - npd
                if extra_decel_d > 0.:
                    extra_decel_v = extra_decel_d / decel_t
                    decel_v -= extra_decel_v
                    end_v -= extra_decel_v
                    if decel_v <= 0.:
                        # The entire decel phase is replaced with retraction
                        retract_t = decel_t
                        retract_d = -(end_v + decel_v) * 0.5 * decel_t
                        retract_v = -decel_v
                        decel_t = decel_d = 0.
                    elif end_v < 0.:
                        # Split decel phase into decel and retraction
                        retract_t = -end_v / accel
                        retract_d = -end_v * 0.5 * retract_t
                        decel_t -= retract_t
                        decel_d = decel_v * 0.5 * decel_t
                    else:
                        # There is still only a decel phase (no retraction)
                        decel_d -= extra_decel_d

        # Prepare for steps
        step_const = self.stepper.step_const
        move_time = print_time

        # Acceleration steps
        if accel_d:
            step_const(move_time, start_pos, accel_d, start_v, accel)
            start_pos += accel_d
            move_time += accel_t
        # Cruising steps
        if cruise_d:
            step_const(move_time, start_pos, cruise_d, cruise_v, 0.)
            start_pos += cruise_d
            move_time += cruise_t
        # Deceleration steps
        if decel_d:
            step_const(move_time, start_pos, decel_d, decel_v, -accel)
            start_pos += decel_d
            move_time += decel_t
        # Retraction steps
        if retract_d:
            step_const(move_time, start_pos, -retract_d, retract_v, accel)
            start_pos -= retract_d
        self.extrude_pos = start_pos

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

def add_printer_objects(printer, config):
    for i in range(99):
        section = 'extruder%d' % (i,)
        if not config.has_section(section):
            if not i and config.has_section('extruder'):
                printer.add_object('extruder0', PrinterExtruder(
                    printer, config.getsection('extruder')))
                continue
            break
        printer.add_object(section, PrinterExtruder(
            printer, config.getsection(section)))

def get_printer_extruders(printer):
    out = []
    for i in range(99):
        extruder = printer.objects.get('extruder%d' % (i,))
        if extruder is None:
            break
        out.append(extruder)
    return out

def get_printer_heater(printer, name):
    if name == 'heater_bed' and name in printer.objects:
        return printer.objects[name]
    if name == 'extruder':
        name = 'extruder0'
    if name.startswith('extruder') and name in printer.objects:
        return printer.objects[name].get_heater()
    raise printer.config_error("Unknown heater '%s'" % (name,))
