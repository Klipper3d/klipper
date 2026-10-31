# Calibration of fan settings (off_below / kick_start_time) using the
# tachometer
#
# Copyright (C) 2026 Klipper contributors
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

# Added to the lowest speed that still keeps the fan spinning.
OFF_BELOW_MARGIN = 0.02
# Points kept in the saved rpm_curve
MAX_CURVE_POINTS = 17
# Fan speed used to check the tachometer for undercounting.
UNDERCOUNT_CHECK_SPEED = 0.9
# Recommended ratio between the tachometer edge rate and its poll rate.
UNDERCOUNT_MARGIN = 5.
# Tachometer samples must be read back slower than they are produced,
# otherwise two consecutive polls can return the same sample and look
# stable while the fan is still spinning up.
POLL_FACTOR = 1.5
# Number of tachometer samples averaged to obtain max_rpm.
MAX_RPM_SAMPLES = 3
# Relative difference between consecutive samples considered stable.
STABLE_DELTA = 0.02
# Maximum time to wait for the fan speed to stabilize.
STABLE_TIMEOUT = 20.
# Time given to the fan to coast to a standstill before a start attempt.
COAST_TIME = 4.
# Step size and upper limit of the kick start time search.
KICK_START_STEP = 0.05
KICK_START_MAX = 1.
# Safety margin applied to the measured kick start time.
KICK_START_MARGIN = 1.5
# Config section prefixes that may hold the fan being calibrated.
FAN_PREFIXES = ['fan_generic', 'heater_fan', 'controller_fan',
                'temperature_fan']

class FanCalibrate:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.step = config.getfloat('calibrate_step', 0.05,
                                    above=0., maxval=0.5)
        self.rpm_threshold = config.getfloat('calibrate_rpm_threshold', 0.05,
                                             above=0., maxval=1.)
        self.poll_time = 0.
        self.curve = []
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command('FAN_CALIBRATE', self.cmd_FAN_CALIBRATE,
                               desc=self.cmd_FAN_CALIBRATE_help)
    cmd_FAN_CALIBRATE_help = "Calibrate fan off_below and kick_start_time"
    def _lookup_fan(self, gcmd, name):
        if not name or name == 'fan':
            sections = ['fan']
        else:
            sections = ['%s %s' % (prefix, name) for prefix in FAN_PREFIXES]
        for section in sections:
            obj = self.printer.lookup_object(section, default=None)
            if obj is not None:
                # Unwrap the Fan object from its wrapper class
                return section, obj.fan
        raise gcmd.error("FAN_CALIBRATE: could not find fan named '%s'"
                         % (name or 'fan',))
    def cmd_FAN_CALIBRATE(self, gcmd):
        fan_section, fan = self._lookup_fan(gcmd, gcmd.get('FAN', None))
        # A tachometer is required for feedback based calibration.
        sample_time = fan.tachometer.get_sample_time()
        if sample_time is None:
            raise gcmd.error(
                "FAN_CALIBRATE requires tachometer_pin on fan '%s'"
                % (fan_section,))
        self.poll_time = sample_time * POLL_FACTOR
        reactor = self.printer.get_reactor()
        initial_speed = fan.get_status(reactor.monotonic())['speed']
        # The calibration must not be influenced by the very settings it
        # measures - a previously saved off_below would clamp the low
        # speeds to zero and min_power would scale them away, so the
        # result could only ever confirm the previous one.
        saved_params = fan.set_speed_params(0., 0., 0., False)
        try:
            max_rpm, off_below, kick_start = self._run(gcmd, fan)
        finally:
            self._restore(fan, saved_params, initial_speed)
        logging.info("FAN_CALIBRATE [%s] max_rpm=%.0f off_below=%.3f"
                     " kick_start_time=%.3f", fan_section, max_rpm,
                     off_below, kick_start)
        gcmd.respond_info(
            "Fan '%s' calibration:\n"
            "  max_rpm         = %.0f\n"
            "  off_below       = %.3f\n"
            "  kick_start_time = %.3f\n"
            "The SAVE_CONFIG command will update the printer config file\n"
            "with these parameters and restart the printer." % (
                fan_section, max_rpm, off_below, kick_start))
        # Store results for SAVE_CONFIG
        configfile = self.printer.lookup_object('configfile')
        configfile.set(fan_section, 'off_below', "%.3f" % (off_below,))
        configfile.set(fan_section, 'kick_start_time', "%.3f" % (kick_start,))
        curve = self._format_curve(gcmd)
        if curve is not None:
            configfile.set(fan_section, 'rpm_curve', curve)
    def _format_curve(self, gcmd):
        # The curve is only usable if it rises in both duty cycle and rpm;
        # a dip means the tachometer reading cannot be trusted.
        pts = sorted(self.curve)
        if len(pts) < 2:
            return None
        for (d0, r0), (d1, r1) in zip(pts, pts[1:]):
            if r1 <= r0:
                gcmd.respond_info(
                    "FAN_CALIBRATE: not storing rpm_curve - the measured"
                    " speed does not rise from %.3f (%.0f rpm) to %.3f"
                    " (%.0f rpm)" % (d0, r0, d1, r1))
                return None
        return "".join("\n  %.4f, %.1f" % pt for pt in self._thin(pts))
    def _thin(self, pts):
        # The sweep collects a point per step, which is far more detail
        # than the interpolation needs - keep a subset of them
        if len(pts) <= MAX_CURVE_POINTS:
            return pts
        step = (len(pts) + MAX_CURVE_POINTS - 1) // MAX_CURVE_POINTS
        thinned = pts[::step]
        if thinned[-1] != pts[-1]:
            thinned.append(pts[-1])
        return thinned
    def _restore(self, fan, saved_params, initial_speed):
        # Never let a restore failure mask the original error
        try:
            fan.set_speed_params(*saved_params)
            fan.set_speed(initial_speed)
        except Exception:
            logging.exception("FAN_CALIBRATE: unable to restore fan state")
    def _run(self, gcmd, fan):
        self.curve = []
        max_rpm = self._measure_max_rpm(fan)
        if max_rpm <= 0.:
            raise self.printer.command_error(
                "FAN_CALIBRATE: no tachometer reading at full fan speed")
        gcmd.respond_info("FAN_CALIBRATE: max_rpm=%.0f" % (max_rpm,))
        self._check_undercounting(gcmd, fan, max_rpm)
        off_below = self._find_off_below(gcmd, fan, max_rpm)
        kick_start = self._find_kick_start(gcmd, fan, max_rpm, off_below)
        # Confirm the fan really does start with the pair just measured
        if not self._try_start(fan, self._lowest_duty(off_below), kick_start,
                               max_rpm):
            raise self.printer.command_error(
                "FAN_CALIBRATE: fan did not start with the calibrated values"
                " (off_below=%.3f kick_start_time=%.3f)"
                % (off_below, kick_start))
        self.curve.append((1., max_rpm))
        return max_rpm, off_below, kick_start
    def _pause(self, duration):
        if self.printer.is_shutdown():
            raise self.printer.command_error(
                "FAN_CALIBRATE interrupted (shutdown)")
        reactor = self.printer.get_reactor()
        reactor.pause(reactor.monotonic() + duration)
    def _read_rpm(self, fan):
        reactor = self.printer.get_reactor()
        rpm = fan.get_status(reactor.monotonic())['rpm']
        if rpm is None:
            raise self.printer.command_error(
                "FAN_CALIBRATE: tachometer returned no rpm reading")
        return rpm
    def _wait_stable_rpm(self, fan):
        # The tachometer reports a trailing average that is only refreshed
        # once per sample time, so poll at a slower cadence until two
        # consecutive samples agree.
        last = 0.
        for i in range(int(STABLE_TIMEOUT / self.poll_time) + 1):
            self._pause(self.poll_time)
            rpm = self._read_rpm(fan)
            if i and abs(rpm - last) <= STABLE_DELTA * max(rpm, last, 1.):
                return rpm
            last = rpm
        return last
    def _lowest_duty(self, off_below):
        # off_below is the lowest speed still passed through to the fan;
        # anything below it is turned into a full stop.
        return max(off_below, self.step)
    def _measure_max_rpm(self, fan):
        fan.set_speed(1.)
        self._wait_stable_rpm(fan)
        # max_rpm sets the thresholds for everything that follows, so
        # average a few samples rather than trusting a single one.
        total = 0.
        for _ in range(MAX_RPM_SAMPLES):
            self._pause(self.poll_time)
            total += self._read_rpm(fan)
        return total / MAX_RPM_SAMPLES
    def _check_undercounting(self, gcmd, fan, max_rpm):
        # The micro-controller counter registers at most one edge per poll
        # interval, so a tachometer_poll_interval that is too large
        # silently undercounts at high speed. The tell-tale sign is a
        # lower fan speed reporting a higher rpm than full speed.
        fan.set_speed(UNDERCOUNT_CHECK_SPEED)
        rpm = self._wait_stable_rpm(fan)
        if rpm <= max_rpm:
            return
        poll_interval = 30. / (fan.tachometer.ppr * rpm * UNDERCOUNT_MARGIN)
        gcmd.respond_info(
            "FAN_CALIBRATE: warning - %.0f%% speed reads %.0f rpm, above the"
            " %.0f rpm measured at full speed. The tachometer is"
            " undercounting; set tachometer_poll_interval below %.5f and"
            " calibrate again."
            % (UNDERCOUNT_CHECK_SPEED * 100., rpm, max_rpm, poll_interval))
    def _find_off_below(self, gcmd, fan, max_rpm):
        # Step the speed down until the fan stalls. Speed 1.0 is already
        # covered by the max_rpm measurement and speed 0 is never tried -
        # it would only prove that a stopped fan reports no rpm.
        threshold = self.rpm_threshold * max_rpm
        last_spinning = 0.
        for i in range(int(round(1. / self.step)) - 1, 0, -1):
            value = i * self.step
            fan.set_speed(value)
            rpm = self._wait_stable_rpm(fan)
            gcmd.respond_info("FAN_CALIBRATE: speed %.3f -> %.0f rpm"
                              % (value, rpm))
            if rpm < threshold:
                return min(1., last_spinning + OFF_BELOW_MARGIN)
            self.curve.append((value, rpm))
            last_spinning = value
        # The fan never stalled - no minimum speed needs to be enforced
        return 0.
    def _try_start(self, fan, duty, kick_start_time, max_rpm):
        # Restart the fan through its own kick start logic so the test
        # matches what kick_start_time will do in normal operation.
        fan.set_speed(0.)
        self._pause(COAST_TIME)
        fan.set_speed_params(0., kick_start_time, 0., False)
        fan.set_speed(duty)
        return self._wait_stable_rpm(fan) >= self.rpm_threshold * max_rpm
    def _find_kick_start(self, gcmd, fan, max_rpm, off_below):
        # kick_start_time is the time the fan is driven at full power
        # before the drive drops to the requested speed, so the quantity
        # to measure is the shortest kick after which the fan keeps
        # spinning at the lowest speed it will ever be given. Timing the
        # spin-up with the tachometer instead would be hopeless - its
        # readings are only refreshed once per sample time.
        duty = self._lowest_duty(off_below)
        for i in range(int(round(KICK_START_MAX / KICK_START_STEP)) + 1):
            kick_start_time = i * KICK_START_STEP
            if self._try_start(fan, duty, kick_start_time, max_rpm):
                gcmd.respond_info(
                    "FAN_CALIBRATE: fan starts at speed %.3f with a %.3fs"
                    " kick" % (duty, kick_start_time))
                return min(KICK_START_MAX,
                           kick_start_time * KICK_START_MARGIN)
        raise self.printer.command_error(
            "FAN_CALIBRATE: fan did not start at speed %.3f even with a"
            " %.3fs kick start" % (duty, KICK_START_MAX))

def load_config(config):
    return FanCalibrate(config)
