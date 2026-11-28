# Heater power budget controller for Klipper
#
# Copyright (C) 2026  Sezgin ACIKGOZ <sezginacikgoz@mail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#
# Weighted, borrowable heater power budget:
# - Priorities define guaranteed shares only while the budget is contended.
# - Unused share of one heater may be borrowed by the other.
# - PID autotune receives reserved priority while it is active.
# - No cross-heater PWM scheduling is performed.
# - Both PID control ceilings and final PWM outputs are constrained.
#
# Power is estimated from PWM duty * configured nominal heater power.
# It is not a real electrical power measurement.

import logging

from . import pid_calibrate


EPSILON = 1.0e-6


class HeaterPowerBudget:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.max_total_power = config.getfloat("max_total_power", above=0.0)
        self.extruder_nominal_power = config.getfloat(
            "extruder_nominal_power", above=0.0)
        self.bed_nominal_power = config.getfloat(
            "bed_nominal_power", above=0.0)
        self.extruder_priority = config.getfloat(
            "extruder_priority", 30.0, minval=0.0)
        self.bed_priority = config.getfloat(
            "bed_priority", 70.0, minval=0.0)
        self.priority_sum = self.extruder_priority + self.bed_priority
        if self.priority_sum <= 0.0:
            raise config.error(
                "heater_power_budget: at least one priority must be > 0")

        self.extruder_name = config.get("extruder", "extruder")
        self.bed_name = config.get("bed", "heater_bed")
        self.extruder = None
        self.bed = None
        self.extruder_config_max_pwm = 1.0
        self.bed_config_max_pwm = 1.0
        self.extruder_requested_pwm = 0.0
        self.bed_requested_pwm = 0.0
        self.extruder_allowed_pwm = 1.0
        self.bed_allowed_pwm = 1.0
        self._extruder_set_pwm_original = None
        self._bed_set_pwm_original = None
        self._extruder_temp_cb_original = None
        self._bed_temp_cb_original = None

        self.printer.register_event_handler("klippy:ready",
                                            self._handle_ready)
        gcode = self.printer.lookup_object("gcode")
        gcode.register_command(
            "QUERY_HEATER_POWER_BUDGET",
            self.cmd_QUERY_HEATER_POWER_BUDGET,
            desc="Report heater power budget state")

    @staticmethod
    def _clamp01(value):
        return max(0.0, min(1.0, value))

    def _handle_ready(self):
        pheaters = self.printer.lookup_object("heaters")
        self.extruder = pheaters.lookup_heater(self.extruder_name)
        self.bed = pheaters.lookup_heater(self.bed_name)
        if self.extruder is self.bed:
            raise self.printer.config_error(
                "heater_power_budget: extruder and bed must be "
                "different heaters")

        self.extruder_config_max_pwm = self.extruder.get_max_power()
        self.bed_config_max_pwm = self.bed.get_max_power()
        self._extruder_set_pwm_original = self.extruder.set_pwm
        self._bed_set_pwm_original = self.bed.set_pwm
        self.extruder.set_pwm = self._extruder_set_pwm
        self.bed.set_pwm = self._bed_set_pwm

        self._extruder_temp_cb_original = self.extruder.temperature_callback
        self._bed_temp_cb_original = self.bed.temperature_callback
        self.extruder.sensor.setup_callback(
            self._extruder_temperature_callback)
        self.bed.sensor.setup_callback(self._bed_temperature_callback)

        self.extruder_requested_pwm = self._clamp01(
            self.extruder.last_pwm_value)
        self.bed_requested_pwm = self._clamp01(self.bed.last_pwm_value)
        self._refresh_control_limits()

        extruder_percent = (
            100.0 * self.extruder_priority / self.priority_sum)
        bed_percent = 100.0 * self.bed_priority / self.priority_sum
        logging.info(
            "heater_power_budget: enabled, budget=%.1fW, "
            "priority bed=%.1f%% extruder=%.1f%%",
            self.max_total_power, bed_percent, extruder_percent)

    def _base_shares(self):
        extruder_share = (
            self.max_total_power * self.extruder_priority / self.priority_sum)
        bed_share = self.max_total_power - extruder_share
        return extruder_share, bed_share

    def _configured_max_power(self, heater_name):
        if heater_name == "extruder":
            return self.extruder_nominal_power * self.extruder_config_max_pwm
        return self.bed_nominal_power * self.bed_config_max_pwm

    def _requested_power(self, heater_name):
        if heater_name == "extruder":
            return self.extruder_requested_pwm * self.extruder_nominal_power
        return self.bed_requested_pwm * self.bed_nominal_power

    def _actual_power(self, heater_name):
        if heater_name == "extruder":
            return (self._clamp01(self.extruder.last_pwm_value)
                    * self.extruder_nominal_power)
        return (self._clamp01(self.bed.last_pwm_value)
                * self.bed_nominal_power)

    def _get_autotune_heater(self):
        extruder_tuning = isinstance(
            self.extruder.control, pid_calibrate.ControlAutoTune)
        bed_tuning = isinstance(
            self.bed.control, pid_calibrate.ControlAutoTune)

        if extruder_tuning == bed_tuning:
            return None
        if extruder_tuning:
            return "extruder"
        return "bed"

    def _calculate_autotune_allowed_powers(self, heater_name):
        extruder_max = self._configured_max_power("extruder")
        bed_max = self._configured_max_power("bed")

        if heater_name == "extruder":
            extruder_allowed = min(
                extruder_max, self.max_total_power)
            bed_allowed = min(
                bed_max,
                max(0.0, self.max_total_power - extruder_allowed))
        else:
            bed_allowed = min(
                bed_max, self.max_total_power)
            extruder_allowed = min(
                extruder_max,
                max(0.0, self.max_total_power - bed_allowed))

        return extruder_allowed, bed_allowed

    def _calculate_allowed_powers(self):
        autotune_heater = self._get_autotune_heater()
        if autotune_heater is not None:
            return self._calculate_autotune_allowed_powers(
                autotune_heater)

        extruder_share, bed_share = self._base_shares()
        extruder_request = min(
            self._requested_power("extruder"),
            self._configured_max_power("extruder"))
        bed_request = min(
            self._requested_power("bed"),
            self._configured_max_power("bed"))

        extruder_allowed = (
            extruder_share + max(0.0, bed_share - bed_request))
        bed_allowed = (
            bed_share + max(0.0, extruder_share - extruder_request))

        extruder_allowed = min(
            extruder_allowed,
            self._configured_max_power("extruder"),
            self.max_total_power)
        bed_allowed = min(
            bed_allowed,
            self._configured_max_power("bed"),
            self.max_total_power)
        return max(0.0, extruder_allowed), max(0.0, bed_allowed)

    def _power_to_pwm(self, power, nominal_power, config_max_pwm):
        return min(config_max_pwm,
                   self._clamp01(power / nominal_power))

    def _set_control_limit(self, heater, allowed_pwm):
        allowed_pwm = min(heater.get_max_power(),
                          self._clamp01(allowed_pwm))
        control = heater.control
        if hasattr(control, "heater_max_power"):
            control.heater_max_power = allowed_pwm

    def _refresh_control_limits(self):
        extruder_power, bed_power = self._calculate_allowed_powers()
        self.extruder_allowed_pwm = self._power_to_pwm(
            extruder_power, self.extruder_nominal_power,
            self.extruder_config_max_pwm)
        self.bed_allowed_pwm = self._power_to_pwm(
            bed_power, self.bed_nominal_power,
            self.bed_config_max_pwm)
        self._set_control_limit(self.extruder, self.extruder_allowed_pwm)
        self._set_control_limit(self.bed, self.bed_allowed_pwm)

    def _temperature_callback_common(self, heater_name, read_time, temp):
        self._refresh_control_limits()
        if heater_name == "extruder":
            self._extruder_temp_cb_original(read_time, temp)
        else:
            self._bed_temp_cb_original(read_time, temp)

    def _extruder_temperature_callback(self, read_time, temp):
        self._temperature_callback_common("extruder", read_time, temp)

    def _bed_temperature_callback(self, read_time, temp):
        self._temperature_callback_common("bed", read_time, temp)

    def _calculate_safe_pwm(self, heater_name, requested_pwm,
                            ideal_allowed_pwm):
        if heater_name == "extruder":
            nominal_power = self.extruder_nominal_power
            config_max_pwm = self.extruder_config_max_pwm
            other_actual_power = self._actual_power("bed")
        else:
            nominal_power = self.bed_nominal_power
            config_max_pwm = self.bed_config_max_pwm
            other_actual_power = self._actual_power("extruder")

        remaining_power = max(
            0.0, self.max_total_power - other_actual_power)
        safety_pwm = self._power_to_pwm(
            remaining_power, nominal_power, config_max_pwm)

        return min(
            self._clamp01(requested_pwm),
            ideal_allowed_pwm,
            safety_pwm,
            config_max_pwm)

    def _set_pwm_common(self, heater_name, read_time, value):
        requested_pwm = self._clamp01(value)
        if heater_name == "extruder":
            self.extruder_requested_pwm = requested_pwm
        else:
            self.bed_requested_pwm = requested_pwm

        extruder_power, bed_power = self._calculate_allowed_powers()
        self.extruder_allowed_pwm = self._power_to_pwm(
            extruder_power, self.extruder_nominal_power,
            self.extruder_config_max_pwm)
        self.bed_allowed_pwm = self._power_to_pwm(
            bed_power, self.bed_nominal_power,
            self.bed_config_max_pwm)

        if heater_name == "extruder":
            heater = self.extruder
            original = self._extruder_set_pwm_original
            ideal_allowed_pwm = self.extruder_allowed_pwm
        else:
            heater = self.bed
            original = self._bed_set_pwm_original
            ideal_allowed_pwm = self.bed_allowed_pwm

        final_pwm = self._calculate_safe_pwm(
            heater_name, requested_pwm, ideal_allowed_pwm)

        force_reduction = heater.last_pwm_value > final_pwm + EPSILON
        old_min_pwm_change = heater.min_pwm_change
        if force_reduction:
            heater.min_pwm_change = 0.0
        try:
            original(read_time, final_pwm)
        finally:
            if force_reduction:
                heater.min_pwm_change = old_min_pwm_change

    def _extruder_set_pwm(self, read_time, value):
        self._set_pwm_common("extruder", read_time, value)

    def _bed_set_pwm(self, read_time, value):
        self._set_pwm_common("bed", read_time, value)

    def get_status(self, eventtime):
        if self.extruder is None or self.bed is None:
            return {
                "max_total_power": round(self.max_total_power, 1),
                "total_power": 0.0,
                "budget_exceeded": False,
            }

        extruder_status = self.extruder.get_status(eventtime)
        bed_status = self.bed.get_status(eventtime)

        extruder_share, bed_share = self._base_shares()
        extruder_actual_pwm = self._clamp01(
            extruder_status["power"])
        bed_actual_pwm = self._clamp01(
            bed_status["power"])
        extruder_requested_power = (
            self.extruder_requested_pwm * self.extruder_nominal_power)
        bed_requested_power = (
            self.bed_requested_pwm * self.bed_nominal_power)
        extruder_allowed_power = (
            self.extruder_allowed_pwm * self.extruder_nominal_power)
        bed_allowed_power = self.bed_allowed_pwm * self.bed_nominal_power
        extruder_actual_power = (
            extruder_actual_pwm * self.extruder_nominal_power)
        bed_actual_power = bed_actual_pwm * self.bed_nominal_power
        total_power = extruder_actual_power + bed_actual_power

        extruder_limited = (
            extruder_status["target"] > 0.0
            and self.extruder_allowed_pwm
                < self.extruder_config_max_pwm - EPSILON
            and self.extruder_requested_pwm
                >= self.extruder_allowed_pwm - EPSILON)
        bed_limited = (
            bed_status["target"] > 0.0
            and self.bed_allowed_pwm < self.bed_config_max_pwm - EPSILON
            and self.bed_requested_pwm >= self.bed_allowed_pwm - EPSILON)

        return {
            "max_total_power": round(self.max_total_power, 1),
            "extruder_priority": round(
                self.extruder_priority / self.priority_sum, 4),
            "bed_priority": round(
                self.bed_priority / self.priority_sum, 4),
            "extruder_share_power": round(extruder_share, 1),
            "bed_share_power": round(bed_share, 1),
            "extruder_requested_pwm": round(
                self.extruder_requested_pwm, 4),
            "bed_requested_pwm": round(self.bed_requested_pwm, 4),
            "extruder_requested_power": round(
                extruder_requested_power, 1),
            "bed_requested_power": round(bed_requested_power, 1),
            "extruder_allowed_pwm": round(self.extruder_allowed_pwm, 4),
            "bed_allowed_pwm": round(self.bed_allowed_pwm, 4),
            "extruder_allowed_power": round(extruder_allowed_power, 1),
            "bed_allowed_power": round(bed_allowed_power, 1),
            "extruder_actual_pwm": round(extruder_actual_pwm, 4),
            "bed_actual_pwm": round(bed_actual_pwm, 4),
            "extruder_actual_power": round(extruder_actual_power, 1),
            "bed_actual_power": round(bed_actual_power, 1),
            "total_power": round(total_power, 1),
            "extruder_limited": extruder_limited,
            "bed_limited": bed_limited,
            "budget_exceeded":
                total_power > self.max_total_power + 0.1,
        }

    def cmd_QUERY_HEATER_POWER_BUDGET(self, gcmd):
        status = self.get_status(self.printer.get_reactor().monotonic())
        gcmd.respond_info(
            "Heater power budget:\n"
            "  Budget   : %.1f W\n"
            "  Priority : bed %.1f%% / hotend %.1f%%\n"
            "  Shares   : bed %.1f W / hotend %.1f W\n"
            "  Bed      : req %.1f W / allowed %.1f W / actual %.1f W\n"
            "  Hotend   : req %.1f W / allowed %.1f W / actual %.1f W\n"
            "  Total    : %.1f W / %.1f W\n"
            "  Limited  : bed=%s hotend=%s\n"
            "  Over     : %s"
            % (
                status["max_total_power"],
                status["bed_priority"] * 100.0,
                status["extruder_priority"] * 100.0,
                status["bed_share_power"],
                status["extruder_share_power"],
                status["bed_requested_power"],
                status["bed_allowed_power"],
                status["bed_actual_power"],
                status["extruder_requested_power"],
                status["extruder_allowed_power"],
                status["extruder_actual_power"],
                status["total_power"],
                status["max_total_power"],
                status["bed_limited"],
                status["extruder_limited"],
                status["budget_exceeded"],
            ))


def load_config(config):
    return HeaterPowerBudget(config)
