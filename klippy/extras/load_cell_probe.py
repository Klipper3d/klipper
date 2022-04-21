# Support for bed level probes featuring a load cell at which the hotend is
# suspended.
#
# Copyright (C) 2022 Martin Hierholzer <martin@hierholzer.info>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import mathutil
import sys
import logging
import random
import math

syspath = sys.path
sys.path = syspath[1:]
import statistics
sys.path = syspath

###########################
# Configuration constants #
###########################

# Speed used for all movements; "as fast as possible"
SPEED=500.0

# Lift distance for compensation measurement (relative to fit step size)
COMPENSATION_Z_LIFT_FACTOR=40

# Aim for forces lower than max_abs_force by this factor ("target force").
# Note that the target force can be exceeded regularly by a factor of 1.5 by
# design (since threshold is half the target force).
FORCE_SAFETY_MARGIN=5.

# Divider to compute threshold from target force
THRESHOLD_DIVIDER=4.

# Number of points for the fit (theoretical minimum is 3)
FIT_POINTS=5

# Minimum fit quality factor to accept result
FIT_MIN_QUALITY=0.85

# Factor above noise level to use data points for fit
FIT_THRESHOLD_FACTOR=10.

# Maximum number of fit steps per full motor step. Typically, micro steps are
# not perfectly linear. Using a smaller step size for the fit raster scan than
# 1/4 or maybe 1/8 full step might reduce precision.
FIT_SUB_STEPPING=4

# Minimum step size to use for fit
FIT_MIN_STEP_SIZE=0.01

# Number of retries if fit fails (due to FIT_MIN_QUALITY)
MAX_RETRY=5

###########################


class LoadCellProbe:
    def __init__(self, config):

        self._printer = config.get_printer()
        self._name = config.get_name()
        self._reactor = self._printer.get_reactor()
        self._configfile = self._printer.lookup_object('configfile')

        pin_name = config.get('adc')
        ppins = self._printer.lookup_object('pins')
        self._mcu_adc = ppins.setup_pin('adc', pin_name)

        # Parameters from configuration, e.g. determined by calibration commands
        # ----------------------------------------------------------------------

        # ADC conversion rate to request (in SPS)
        self._report_time = 1./config.getfloat('adc_rate', above=0.)

        # Conversion factor to convert ADC readings into physical units.
        self._force_calibration = config.getfloat('force_calibration',         \
            default=1.0)

        # Maximum acceptable force
        self._max_abs_force = config.getfloat('max_abs_force', above=0.)

        # Stiffness/"spring constant", i.e. force per distance
        # Default value is a safe value resulting in very small step sizes.
        self._stiffness = config.getfloat('stiffness', above=0., default=1e99)

        # Noise level of force measurements (standard deviation, in physical
        # units)
        self._noise_level = config.getfloat('noise_level', above=0.,           \
            default=1e-3)

        # From stepper_z section: Distance for one full step
        cfg_stepper_z = config.getsection("stepper_z")
        rotation_distance = cfg_stepper_z.getfloat("rotation_distance")
        full_steps = config.getint('full_steps_per_rotation', 200)
        full_step_distance = rotation_distance/full_steps


        # Derived parameters
        # ------------------
        target_force = self._max_abs_force / FORCE_SAFETY_MARGIN

        # Threshold is 1/4 "target" force
        self._threshold = target_force / THRESHOLD_DIVIDER

        # Step size is the distance to  of the target_force
        self._step_size = target_force / self._stiffness

        # Threshold for data points used in fit
        self._fit_threshold = self._noise_level * FIT_THRESHOLD_FACTOR

        # Step size of the fit data points: chosen as small as possibe, such
        # that each data point is significantly different from previous data
        # point and the step size is not below a quater step on the z axis
        self._fit_step_size = max( self._fit_threshold / self._stiffness,
                                  full_step_distance/FIT_SUB_STEPPING)
        if self._fit_step_size < FIT_MIN_STEP_SIZE :
          self._fit_step_size = FIT_MIN_STEP_SIZE

        # Distance of Z compensation lift
        self._compensation_z_lift = self._fit_step_size *                      \
                                   COMPENSATION_Z_LIFT_FACTOR

        # initialise data members
        self._last_z_result = 0.
        self._force_offset = None
        self._last_uncompensated_force = None
        self._last_force = 0.
        self._last_time = None
        self._stiffness_points = []

        # subscribe to ADC callback
        max_val = self._max_abs_force / self._force_calibration
        self._mcu_adc.setup_minmax(self._report_time, 1, -max_val, max_val)
        self._mcu_adc.setup_adc_callback(self._report_time, self._adc_callback)

        # do some late initialisation when ready
        self._printer.register_event_handler("klippy:ready", self._handle_ready)

        # register gcode commands
        self._gcode = self._printer.lookup_object('gcode')
        self._gcode.register_command('PROBE', self.cmd_PROBE,
                                    desc=self.cmd_PROBE_help)

        self._gcode.register_command('PROBE_ACCURACY', self.cmd_PROBE_ACCURACY,
                                    desc=self.cmd_PROBE_ACCURACY_help)

        self._gcode.register_command('LCP_READ', self.cmd_LCP_READ,
                                    desc=self.cmd_LCP_READ_help)

        self._gcode.register_command('LCP_COMPENSATE',
                                    self.cmd_LCP_COMPENSATE,
                                    desc=self.cmd_LCP_COMPENSATE_help)

        self._gcode.register_command('LCP_CALIB_NOISE',
                                    self.cmd_LCP_CALIB_NOISE,
                                    desc=self.cmd_LCP_CALIB_NOISE_help)

        self._gcode.register_command('LCP_CALIB_WEIGHT',
                                    self.cmd_LCP_CALIB_WEIGHT,
                                    desc=self.cmd_LCP_CALIB_WEIGHT_help)

        self._gcode.register_command('LCP_CALIB_STIFFNESS',
                                    self.cmd_LCP_CALIB_STIFFNESS,
                                    desc=self.cmd_LCP_CALIB_STIFFNESS_help)

        self._gcode.register_command('LCP_INFO', self.LCP_INFO,
                                    desc=self.cmd_LCP_INFO_help)


    cmd_PROBE_ACCURACY_help = "Determine probe accuracy at current position."
    cmd_PROBE_help = "Run probe and stop at the contact position."
    cmd_LCP_READ_help = "Print current load cell measurement to the "          \
            + "console using the same averaging setting as for the probe."
    cmd_LCP_COMPENSATE_help = "Set load cell compensation offset to "          \
            + "current measured value. Only affects output of READ_LOAD_CELL." \
            + " Any PROBE command will also perform the compensation."
    cmd_LCP_CALIB_NOISE_help = "Determine typical noise level of load "        \
            + "cell measurements."
    cmd_LCP_CALIB_WEIGHT_help = "Calibrate to physical weight unit."
    cmd_LCP_CALIB_STIFFNESS_help = "Calibrate system stiffness."
    cmd_LCP_INFO_help = "Print load cell probe parameters to console."


    def get_lift_speed(self, gcmd=None):
        if gcmd is not None:
            return gcmd.get_float("LIFT_SPEED", SPEED, above=0.)
        return SPEED


    def get_offsets(self):
        return 0, 0, 0


    def multi_probe_begin(self):
        pass


    def multi_probe_end(self):
        pass


    def get_status(self, eventtime):
        return {
          'last_force': self._last_force,
          'last_z_result': self._last_z_result,
        }


    def run_probe(self, gcmd):

        # wait until toolhead is in position
        self.tool.wait_moves()

        repeat_count=0
        while True:
          # fast, coarse approach
          force = self._fast_approach(gcmd)

          # find start position for fit (no contact but as close to surface)
          self._find_fit_start(gcmd, force)

          # perform raster scan and fit
          result = self._perform_fit(gcmd)
          if result is not None:
            break

          # check abort condition
          repeat_count += 1
          if repeat_count > MAX_RETRY:
            raise gcmd.error("Maximum retries reached, giving up.")
          gcmd.respond_info("Retrying...")
          self._move_z_relative(5*self._step_size, False)

        pos = self.tool.get_position()
        gcmd.respond_info("FINISHED toolhead Z = %f" % result)
        pos[2] = result
        self._last_z_result = result
        return pos[0], pos[1], pos[2]


    def _handle_ready(self):
        # obtain toolhead object
        self.tool = self._printer.lookup_object('toolhead')


    def _adc_callback(self, time, value):
        # convert to physical unit
        self._last_uncompensated_force = value * self._force_calibration
        # First value after start: use as zero offset
        if self._force_offset == None :
          self._force_offset = self._last_uncompensated_force
        self._last_time = time
        # Store zero offset compensated value for display
        self._last_force = self._last_uncompensated_force - self._force_offset


    def _adc_wait_conversion_ready(self, gcmd):
        last_time = self._last_time
        clocksync = self._mcu_adc.get_mcu()._clocksync
        clock = clocksync.print_time_to_clock(last_time)
        last_sys_time = clocksync.estimate_clock_systime(clock)
        for n in range(1,10) :
          # wait shortly after the timer has called _sample_timer
          self._reactor.pause(last_sys_time + n*self._report_time + 0.0001)
          if self._last_time != last_time:
            return
        # callback not called after 10 report time intervals -> error
        raise gcmd.error("Timeout waiting for ADC value.")


    def _move_z_relative(self, length, wait=True):
        pos = self.tool.get_position()
        self.tool.manual_move([pos[0],pos[1],pos[2]+length], SPEED)
        if wait:
          self.tool.wait_moves()


    def _read_uncompensated_force(self, gcmd):
        # discard one values, because the ADC sampling is asynchronous to the
        # movement.
        self._adc_wait_conversion_ready(gcmd)

        # read ADC sample
        self._adc_wait_conversion_ready(gcmd)

        return self._last_uncompensated_force


    def _lower_to_threshold(self, gcmd):
        # Lower the tool head until the force threshold is exceeded
        while True:
          # Check threshold before first movement, to prevent doing an unchecked
          # step after a retry
          force = self._read_uncompensated_force(gcmd) - self._force_offset
          gcmd.respond_info("pos = %f, force = %.1f"
              % (self.tool.get_position()[2], force))
          if(abs(force) > self._threshold):
            break
          self._move_z_relative(-1*self._step_size)


    def _fast_approach(self, gcmd):
        # Strategy for fast approach: lower tool head until exceeding threshold,
        # then lift head a bit and compare force with original force_offset. If
        # it matches, the contact is assumed. If not, the force_offset has
        # drifed and the search is continued with the new offset.
        gcmd.respond_info("Commencing fast approach.")
        self._force_offset = self._read_uncompensated_force(gcmd)
        attempt = 0
        attempt_start_pos = self.tool.get_position()[2]
        while True:

          # lower tool head until force threshold is exceeded
          self._lower_to_threshold(gcmd)

          # confirm contact with compensated measuerment (also updating the
          # force_offset)
          force = self._compensated_measurement(gcmd)

          # if contact is confirmed with new measurement, terminate fast
          # approach
          if(abs(force) > self._threshold):
            # stay at slightly z-lifted position without contact when returning
            gcmd.respond_info("Fast approach found contact.")
            return force

          # check for failure condition
          attempt_dist = attempt_start_pos + \
            -1*self.tool.get_position()[2]
          if attempt_dist < 2*self._step_size :
            attempt = attempt + 1
            if attempt > MAX_RETRY :
              raise gcmd.error("Force reading drifting too much, maximum "
                  "retries exceeded.")
          else :
            attempt = 0
            attempt_start_pos = self.tool.get_position()[2]


    def _compensated_measurement(self, gcmd):
        # take compensated measurement, update force_offset
        self._move_z_relative(self._compensation_z_lift)
        self._force_offset = self._read_uncompensated_force(gcmd)
        self._move_z_relative(-self._compensation_z_lift)
        force_in = self._read_uncompensated_force(gcmd)
        force = force_in - self._force_offset

        gcmd.respond_info("pos = %f, force(cmp) = %.1f" %
            (self.tool.get_position()[2], force))

        return force


    def _find_fit_start(self, gcmd, force0):
        force1 = force0
        dist = self._step_size/2
        while abs(force1) > self._fit_threshold*2:
          self._move_z_relative(dist, False)
          force2 = self._compensated_measurement(gcmd)
          if abs(force2) < self._fit_threshold*2:
            break
          slope = dist/max(force2-force1,1)
          dist = min(abs((force2-self._fit_threshold)*slope), self._step_size/2)
          force1 = force2


    def _perform_fit(self, gcmd):
        gcmd.respond_info("PERFORM FIT")

        z_start = self.tool.get_position()[2]

        # initialise array with measurement data
        data = []

        # take raster scan measurements to collect data for fit
        while True:
          force = self._compensated_measurement(gcmd)

          # check abort condition
          if len(data) >= FIT_POINTS:
            break

          # store measurement data for linear fit
          if abs(force) > self._fit_threshold :
            height = self.tool.get_position()[2]
            data.append([height, force])

          # move to next position
          self._move_z_relative(-1*self._fit_step_size, False)

          # abort if moved too far from start position
          if z_start-self.tool.get_position()[2] > self._compensation_z_lift:
            gcmd.respond_info("Fit failed, no contact found")
            return None

        # perform fit to find zero force contact position
        heights = [ d[0] for d in data ]
        forces = [ d[1] for d in data ]
        m,b,r,sm,sb = mathutil.linear_regression(forces, heights)

        gcmd.respond_info(
          "Fit result: m = %f, b = %f, r = %f, sm = %f, sb = %f"
          % (m,b,r,sm,sb))

        # safety check: r must be big enough
        if abs(r) < FIT_MIN_QUALITY :
          gcmd.respond_info(
            "Fit failed, fit quality factor r too small: %f < %f" %
            (abs(r), FIT_MIN_QUALITY))
          return None

        # return 0-force offset
        return b


    def _calc_mean(self, positions):
        count = float(len(positions))
        return [sum([pos[i] for pos in positions]) / count
                for i in range(3)]


    def _calc_median(self, positions):
        z_sorted = sorted(positions, key=(lambda p: p[2]))
        middle = len(positions) / 2
        if (len(positions) & 1) == 1:
            # odd number of samples
            return z_sorted[middle]
        # even number of samples
        return self._calc_mean(z_sorted[middle-1:middle+1])


    def cmd_PROBE_ACCURACY(self, gcmd):
        lift_speed = self.get_lift_speed(gcmd)
        sample_count = gcmd.get_int("SAMPLES", 10, minval=1)
        sample_retract_dist = gcmd.get_float("SAMPLE_RETRACT_DIST",
                                             2., above=0.)

        pos = self.tool.get_position()
        gcmd.respond_info("PROBE_ACCURACY at X:%.3f Y:%.3f Z:%.3f"
                          " (samples=%d retract=%.3f"
                          " lift_speed=%.1f)\n"
                          % (pos[0], pos[1], pos[2],
                             sample_count, sample_retract_dist,
                             lift_speed))
        # Probe bed sample_count times
        self.multi_probe_begin()
        positions = []
        while len(positions) < sample_count:
            # Probe position
            pos = self.run_probe(gcmd)
            positions.append(pos)
            # Retract
            self._move_z_relative(sample_retract_dist)
        self.multi_probe_end()
        # Calculate maximum, minimum and average values
        max_value = max([p[2] for p in positions])
        min_value = min([p[2] for p in positions])
        range_value = max_value - min_value
        avg_value = self._calc_mean(positions)[2]
        median = self._calc_median(positions)[2]
        # calculate the standard deviation
        deviation_sum = 0
        for i in range(len(positions)):
            deviation_sum += pow(positions[i][2] - avg_value, 2.)
        sigma = (deviation_sum / len(positions)) ** 0.5
        # Show information
        gcmd.respond_info(
            "probe accuracy results: maximum %.6f, minimum %.6f, range %.6f, "
            "average %.6f, median %.6f, standard deviation %.6f" % (
            max_value, min_value, range_value, avg_value, median, sigma))


    def cmd_PROBE(self, gcmd):
        pos = self.tool.get_position()

        gcmd.respond_info("PROBE at X:%.3f Y:%.3f Z:%.3f\n"
                          % (pos[0], pos[1], pos[2]))

        pos = self.run_probe(gcmd)
        self.tool.manual_move([pos[0],pos[1],pos[2]], SPEED)
        self.tool.wait_moves()


    def cmd_LCP_READ(self, gcmd):
        force = self._read_uncompensated_force(gcmd)
        gcmd.respond_info("Uncompensated: %.6f  compensated: %.6f" %           \
                          (force, force - self._force_offset))


    def cmd_LCP_COMPENSATE(self, gcmd):
        sample_count = gcmd.get_int("SAMPLES", 10, minval=2)
        gcmd.respond_info("Determine zero level from %d samples:" %            \
                          sample_count)

        self._force_offset = 0
        for s in range(0, sample_count):
          self._force_offset += self._read_uncompensated_force(gcmd)
        self._force_offset /= sample_count
        gcmd.respond_info("force_offset = %.6f" % self._force_offset)


    def cmd_LCP_CALIB_NOISE(self, gcmd):
        sample_count = gcmd.get_int("SAMPLES", 50, minval=2)
        gcmd.respond_info("Determine noise level from %d samples:" %           \
                          sample_count)

        samples = []
        for s in range(0, sample_count):
          samples.append(self._read_uncompensated_force(gcmd))

        noise_level = mathutil.std(samples)

        if noise_level == 0:
          raise gcmd.error("Noise level is 0. Please set noise level manually" \
              " to the equivalent of one LSB of the ADC.")

        self._noise_level = noise_level
        gcmd.respond_info("noise_level = %.6f" % self._noise_level)

        self._configfile.set(self._name, 'noise_level', self._noise_level)
        gcmd.respond_info("The SAVE_CONFIG command will update the printer\n"
                  "config file and restart the printer.")


    def cmd_LCP_CALIB_WEIGHT(self, gcmd):
        sample_count = gcmd.get_int("SAMPLES", 10, minval=2)
        weight = gcmd.get_float("WEIGHT", above=0.0)
        gcmd.respond_info(
          "Determine weight calibration from %d samples using weight %.6f:" %  \
          (sample_count, weight))

        force = 0
        for s in range(0, sample_count):
          force += self._read_uncompensated_force(gcmd)
        force /= sample_count
        force -= self._force_offset

        if abs(force) < FIT_THRESHOLD_FACTOR*self._noise_level :
          raise gcmd.error("Measured force too close to noise level: %.6f" %   \
              force);

        correction_factor = weight/force
        self._force_calibration *= correction_factor
        gcmd.respond_info(
          "New force_calibration = %.6f:" % self._force_calibration)
        self._noise_level *= correction_factor
        self._force_offset *= correction_factor
        self._configfile.set(self._name, 'force_calibration',                  \
            self._force_calibration)
        self._configfile.set(self._name, 'noise_level', self._noise_level)
        gcmd.respond_info("The SAVE_CONFIG command will update the printer\n"
                  "config file and restart the printer.")


    def cmd_LCP_CALIB_STIFFNESS(self, gcmd):
        sample_count = gcmd.get_int("SAMPLES", 10, minval=2)
        npt = len(self._stiffness_points)+1
        pos = self.tool.get_position()

        gcmd.respond_info(("Record stiffness measurement point number %d "+    \
          "with %d samples at z position %f:") % (npt, sample_count, pos[2]))

        force = 0
        for s in range(0, sample_count):
          force += self._read_uncompensated_force(gcmd)
        force /= sample_count
        force -= self._force_offset

        self._stiffness_points.append([pos[2], force])
        gcmd.respond_info("Data point: z=%f, force=%f" % (pos[2], force))

        if npt >= 2:
          heights = [ d[0] for d in self._stiffness_points ]
          forces = [ d[1] for d in self._stiffness_points ]
          m,b,r,sm,sb = mathutil.linear_regression(forces, heights)

          self._stiffness = 1./m
          self._configfile.set(self._name, 'stiffness', self._stiffness)

          gcmd.respond_info("Stiffness updated: %f" % self._stiffness)

          gcmd.respond_info("The SAVE_CONFIG command will update the printer\n"
                    "config file and restart the printer.")

          gcmd.respond_info("Re-running LCP_CALIB_STIFFNESS will add more\n"
              "data points to increase precision.")



    def LCP_INFO(self, gcmd):
        gcmd.respond_info("Primary parameters:")
        gcmd.respond_info("max_abs_force = %f" % self._max_abs_force)
        gcmd.respond_info("force_calibration = %f" % self._force_calibration)
        gcmd.respond_info("stiffness = %f" % self._stiffness)
        gcmd.respond_info("noise_level = %f" % self.noise_level)
        gcmd.respond_info("Derived parameters:")
        gcmd.respond_info("threshold = %f" % self._threshold)
        gcmd.respond_info("step_size = %f" % self._step_size)
        gcmd.respond_info("fit_threshold = %f" % self._fit_threshold)
        gcmd.respond_info("fit_step_size = %f" % self._fit_step_size)
        gcmd.respond_info("compensation_z_lift = %f" %                         \
            self._compensation_z_lift)


def load_config(config):
    probe = LoadCellProbe(config)
    config.printer.add_object('probe', probe)
    return probe
