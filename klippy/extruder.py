# Code for handling printer nozzle extruders
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import stepper, heater, homing

class PrinterExtruder:
    def __init__(self, printer, config):
        self.heater = heater.PrinterHeater(printer, config)
        self.stepper = stepper.PrinterStepper(printer, config)
        self.pressure_advance = config.getfloat('pressure_advance', 0.)
        self.stepper_pos = 0
        self.extrude_pos = 0.
    def build_config(self):
        self.heater.build_config()
        self.stepper.set_max_jerk(9999999.9)
        self.stepper.build_config()
    def motor_off(self, move_time):
        self.stepper.motor_enable(move_time, 0)
    def check_move(self, move):
        if not self.heater.can_extrude:
            raise homing.EndstopError(move.end_pos, "Extrude below minimum temp")
        if (not move.do_calc_junction
            and not move.axes_d[0] and not move.axes_d[1]
            and not move.axes_d[2]):
            # Extrude only move - limit accel and velocity
            move.limit_speed(self.stepper.max_velocity, self.stepper.max_accel)
    def move(self, move_time, move):
        axis_d = move.axes_d[3]
        extrude_r = axis_d / move.move_d
        inv_accel = 1. / (move.accel * extrude_r)

        start_v = move.start_v * extrude_r
        cruise_v = move.cruise_v * extrude_r
        end_v = move.end_v * extrude_r
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
            move_extrude_r = move.extrude_r
            prev_pressure_d = start_pos - move.start_pos[3]
            if accel_t:
                npd = move.cruise_v * move_extrude_r * self.pressure_advance
                extra_accel_d = npd - prev_pressure_d
                if extra_accel_d > 0.:
                    accel_d += extra_accel_d
                    start_v += extra_accel_d / accel_t
                    prev_pressure_d += extra_accel_d
            # Update decel and retract parameters when decelerating
            if decel_t:
                npd = move.end_v * move_extrude_r * self.pressure_advance
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
                        retract_t = -end_v * inv_accel
                        retract_d = -end_v * 0.5 * retract_t
                        decel_t -= retract_t
                        decel_d = decel_v * 0.5 * decel_t
                    else:
                        # There is still only a decel phase (no retraction)
                        decel_d -= extra_decel_d

        # Determine regular steps
        forward_d = accel_d + cruise_d + decel_d
        end_pos = start_pos + forward_d
        inv_step_dist = self.stepper.inv_step_dist
        new_step_pos = int(end_pos*inv_step_dist + 0.5)
        if new_step_pos != self.stepper_pos:
            steps = forward_d * inv_step_dist
            step_offset = self.stepper_pos - start_pos * inv_step_dist + 0.5
            self.stepper_pos = new_step_pos
            sdir = 0
            if steps < 0:
                sdir = 1
                steps = -steps
                step_offset = 1. - step_offset
            mcu_time, so = self.stepper.prep_move(move_time, sdir)

            move_step_d = forward_d / steps
            inv_move_step_d = 1. / move_step_d

            # Acceleration steps
            #t = sqrt(2*pos/accel + (start_v/accel)**2) - start_v/accel
            accel_time_offset = start_v * inv_accel
            accel_sqrt_offset = accel_time_offset**2
            accel_multiplier = 2.0 * move_step_d * inv_accel
            accel_steps = accel_d * inv_move_step_d
            step_offset = so.step_sqrt(
                mcu_time - accel_time_offset, accel_steps, step_offset
                , accel_sqrt_offset, accel_multiplier)
            mcu_time += accel_t
            # Cruising steps
            #t = pos/cruise_v
            cruise_multiplier = move_step_d / cruise_v
            cruise_steps = cruise_d * inv_move_step_d
            step_offset = so.step_factor(
                mcu_time, cruise_steps, step_offset, cruise_multiplier)
            mcu_time += cruise_t
            # Deceleration steps
            #t = cruise_v/accel - sqrt((cruise_v/accel)**2 - 2*pos/accel)
            decel_time_offset = decel_v * inv_accel
            decel_sqrt_offset = decel_time_offset**2
            decel_steps = decel_d * inv_move_step_d
            so.step_sqrt(
                mcu_time + decel_time_offset, decel_steps, step_offset
                , decel_sqrt_offset, -accel_multiplier)

        # Determine retract steps
        start_pos = end_pos
        end_pos -= retract_d
        new_step_pos = int(end_pos*inv_step_dist + 0.5)
        if new_step_pos != self.stepper_pos:
            steps = retract_d * inv_step_dist
            step_offset = start_pos * inv_step_dist - self.stepper_pos + 0.5
            self.stepper_pos = new_step_pos
            mcu_time, so = self.stepper.prep_move(
                move_time+accel_t+cruise_t+decel_t, 1)

            move_step_d = retract_d / steps

            # Acceleration steps
            #t = sqrt(2*pos/accel + (start_v/accel)**2) - start_v/accel
            accel_time_offset = retract_v * inv_accel
            accel_sqrt_offset = accel_time_offset**2
            accel_multiplier = 2.0 * move_step_d * inv_accel
            so.step_sqrt(mcu_time - accel_time_offset, steps, step_offset
                         , accel_sqrt_offset, accel_multiplier)
        self.extrude_pos = end_pos
