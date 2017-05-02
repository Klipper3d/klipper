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
        self.stepper = stepper.PrinterStepper(printer, config, 'extruder')
        self.nozzle_diameter = config.getfloat('nozzle_diameter', above=0.)
        filament_diameter = config.getfloat(
            'filament_diameter', minval=self.nozzle_diameter)
        filament_area = math.pi * (filament_diameter * .5)**2
        max_cross_section = config.getfloat(
            'max_extrude_cross_section', 4. * self.nozzle_diameter**2
            , above=0.)
        self.max_extrude_ratio = max_cross_section / filament_area
        self.max_e_dist = config.getfloat(
            'max_extrude_only_distance', 50., minval=0.)
        self.max_e_velocity = self.max_e_accel = None
        self.pressure_advance = config.getfloat(
            'pressure_advance', 0., minval=0.)
        self.pressure_advance_lookahead_time = 0.
        if self.pressure_advance:
            self.pressure_advance_lookahead_time = config.getfloat(
                'pressure_advance_lookahead_time', 0.010, minval=0.)
        self.need_motor_enable = True
        self.extrude_pos = 0.
    def set_max_jerk(self, max_xy_halt_velocity, max_velocity, max_accel):
        self.max_e_velocity = self.config.getfloat(
            'max_extrude_only_velocity', max_velocity * self.max_extrude_ratio
            , above=0.)
        self.max_e_accel = self.config.getfloat(
            'max_extrude_only_accel', max_accel * self.max_extrude_ratio
            , above=0.)
        self.stepper.set_max_jerk(9999999.9, 9999999.9)
    def motor_off(self, move_time):
        self.stepper.motor_enable(move_time, 0)
        self.need_motor_enable = True
    def check_move(self, move):
        move.extrude_r = move.axes_d[3] / move.move_d
        move.extrude_max_corner_v = 0.
        if not self.heater.can_extrude:
            raise homing.EndstopMoveError(
                move.end_pos, "Extrude below minimum temp")
        if not move.is_kinematic_move or move.extrude_r < 0.:
            # Extrude only move (or retraction move) - limit accel and velocity
            if abs(move.axes_d[3]) > self.max_e_dist:
                raise homing.EndstopMoveError(
                    move.end_pos, "Extrude move too long")
            inv_extrude_r = 1. / abs(move.extrude_r)
            move.limit_speed(self.max_e_velocity * inv_extrude_r
                             , self.max_e_accel * inv_extrude_r)
        elif (move.extrude_r > self.max_extrude_ratio
              and move.axes_d[3] > self.nozzle_diameter*self.max_extrude_ratio):
            logging.debug("Overextrude: %s vs %s" % (
                move.extrude_r, self.max_extrude_ratio))
            raise homing.EndstopMoveError(
                move.end_pos, "Move exceeds maximum extrusion cross section")
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
    def move(self, move_time, move):
        if self.need_motor_enable:
            self.stepper.motor_enable(move_time, 1)
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
        mcu_stepper = self.stepper.mcu_stepper
        mcu_time = mcu_stepper.print_to_mcu_time(move_time)

        # Acceleration steps
        if accel_d:
            mcu_stepper.step_const(mcu_time, start_pos, accel_d, start_v, accel)
            start_pos += accel_d
            mcu_time += accel_t
        # Cruising steps
        if cruise_d:
            mcu_stepper.step_const(mcu_time, start_pos, cruise_d, cruise_v, 0.)
            start_pos += cruise_d
            mcu_time += cruise_t
        # Deceleration steps
        if decel_d:
            mcu_stepper.step_const(mcu_time, start_pos, decel_d, decel_v, -accel)
            start_pos += decel_d
            mcu_time += decel_t
        # Retraction steps
        if retract_d:
            mcu_stepper.step_const(
                mcu_time, start_pos, -retract_d, retract_v, accel)
            start_pos -= retract_d
        self.extrude_pos = start_pos

# Dummy extruder class used when a printer has no extruder at all
class DummyExtruder:
    def set_max_jerk(self, max_xy_halt_velocity, max_velocity, max_accel):
        pass
    def motor_off(self, move_time):
        pass
    def check_move(self, move):
        raise homing.EndstopMoveError(
            move.end_pos, "Extrude when no extruder present")
    def calc_junction(self, prev_move, move):
        return move.max_cruise_v2
    def lookahead(self, moves, flush_count, lazy):
        return flush_count
