# Support for bed level probes featuring a load cell at which the hotend is
# suspended.
#
# Copyright (C) 2021 Martin Hierholzer <martin@hierholzer.info>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

from mathutil import linear_regression

DIRECTION_CHOICE_LIST={
  'X+':[0,+1],
  'x+':[0,+1],
  'X-':[0,-1],
  'x-':[0,-1],
  'Y+':[1,+1],
  'y+':[1,+1],
  'Y-':[1,-1],
  'y-':[1,-1],
  'Z+':[2,+1],
  'z+':[2,+1],
  'Z-':[2,-1],
  'z-':[2,-1],
}


class LoadCellProbe:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name()
        self.reactor = self.printer.get_reactor()

        pin_name = config.get('adc')
        ppins = self.printer.lookup_object('pins')
        self.mcu_adc = ppins.setup_pin('adc', pin_name)

        self.last_z_result = 0

        # general parameters
        self.speed = config.getfloat('speed', 50.0, above=0.)
        self.max_retry = config.getint('max_retry', 5, minval=0)
        self.max_abs_force = \
            config.getint('max_abs_force', 5000, minval=0)

        # parameters for force measurement
        self.adc_n_average = config.getint('adc_n_average', 2, minval=1)
        self.adc_n_average_precise = config.getint('adc_n_average_precise',
          3, minval=self.adc_n_average)
        self.max_variance = config.getint('max_variance', 15, minval=0)
        self.delay_exceeding_max_variance =                                    \
            config.getfloat('delay_exceeding_max_variance', 0.1, above=0.)
        self.max_retry_stable_force =                                          \
            config.getint('max_retry_stable_force', 100, minval=0)

        # parameters for fast approach
        self.threshold = [0,0,0]
        self.step_size = [0,0,0]
        self.threshold[2] = config.getint('threshold', 50, minval=1)
        self.step_size[2] = config.getfloat('step_size', 0.1, above=0.)
        self.threshold[0] = config.getint('threshold_xy', self.threshold[2],   \
            minval=1)
        self.step_size[0] = config.getfloat('step_size_xy', self.step_size[2], \
            above=0.)
        self.threshold[1] = self.threshold[0]
        self.step_size[1] = self.step_size[0]

        # parameters for compensated force measurement
        self.compensation_z_lift = [0,0,0]
        self.compensation_z_lift[2] =                                          \
            config.getfloat('compensation_z_lift', 0.2, minval=self.step_size)
        self.compensation_z_lift[0] = config.getfloat(                         \
            'compensation_z_lift_xy', 2.0, minval=self.step_size)
        self.compensation_z_lift[1] = self.compensation_z_lift[0]

        # parameters for fit
        self.fit_points = config.getint('fit_points', 5, minval=3)
        self.fit_step_size = config.getfloat('fit_step_size', 0.005, above=0.)
        self.fit_min_quality = config.getfloat('fit_min_quality', 0.85,
            above=0.)
        self.fit_threshold = config.getint('fit_threshold', 6, minval=1)

        # parameters for probe accuracy
        self.sample_retract_dist = config.getfloat('sample_retract_dist', 2.,
                                                   above=0.)
        self.lift_speed = config.getfloat('lift_speed', self.speed, above=0.)

        # initialise data members
        self.force_offset = None
        self.last_force = 0
        self.force_subscribers = []

        # subscribe to ADC callbackin ready event, since ADC module might not
        # yet be available.
        self.printer.register_event_handler("klippy:ready", self._handle_ready)

        # register gcode commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('PROBE', self.cmd_PROBE,
                                    desc=self.cmd_PROBE_help)

        self.gcode.register_command('PROBE_ACCURACY', self.cmd_PROBE_ACCURACY,
                                    desc=self.cmd_PROBE_ACCURACY_help)

        self.gcode.register_command('READ_LOAD_CELL', self.cmd_READ_LOAD_CELL,
                                    desc=self.cmd_READ_LOAD_CELL_help)

        self.gcode.register_command('COMPENSATE_LOAD_CELL',
                                    self.cmd_COMPENSATE_LOAD_CELL,
                                    desc=self.cmd_COMPENSATE_LOAD_CELL_help)


    cmd_PROBE_ACCURACY_help = "Determine probe accuracy at current position "  \
            + "in the specified direction (defaults to negative Z)."
    cmd_PROBE_help = "Run probe in the specified direction (defaults to "      \
            + "negative Z) and stop at the contact position."
    cmd_READ_LOAD_CELL_help = "Print current load cell measurement to the "    \
            + "console using the same averaging setting as for the probe."
    cmd_COMPENSATE_LOAD_CELL_help = "Set load cell compensation offset to "    \
            + "current measured value. Only affects output of READ_LOAD_CELL." \
            + " Any PROBE command will also perform the compensation."


    def _handle_ready(self):
        self.mcu_adc.setup_adc_callback(None, self._adc_callback)


    # Allow another module to subscribe to the load-cell measurement. This is
    # used e.g. for the z_sense_offset module.
    def subscribe_force(self, callback):
        self.force_subscribers.append(callback)


    def get_lift_speed(self, gcmd=None):
        if gcmd is not None:
            return gcmd.get_float("LIFT_SPEED", self.lift_speed, above=0.)
        return self.lift_speed


    def get_offsets(self):
        return 0, 0, 0


    def multi_probe_begin(self):
        pass


    def multi_probe_end(self):
        pass


    def _adc_callback(self, time, value):
        # First value after start: use as zero offset
        if self.force_offset == None :
          self.force_offset = value
        # Store zero offset compensated value for display
        self.last_force = value - self.force_offset
        # Forward uncompensated value to subscribers
        for sub in self.force_subscribers :
            sub(value)


    def _move_z_relative(self, length, wait=True):
        pos = self.tool.get_position()
        self.tool.manual_move([pos[0],pos[1],pos[2]+length], self.speed)
        if wait:
          self.tool.wait_moves()


    def _move_axis_relative(self, length, wait=True):
        # move along selected probing axis
        pos = self.tool.get_position()
        pos[self.probing_axis] += length
        self.tool.manual_move([pos[0],pos[1],pos[2]], self.speed)
        if wait:
          self.tool.wait_moves()


    def _move_axis_absolute(self, position, wait=True):
        pos = self.tool.get_position()
        pos[self.probing_axis] = position
        self.tool.manual_move([pos[0],pos[1],pos[2]], self.speed)
        if wait:
          self.tool.wait_moves()


    def _average_force(self, gcmd, precise):
        # discard one values, because the ADC sampling is asynchronous to the
        # movement. The readout is asynchronous to the ADC sampling, but it is
        # synchronised to the movement, hence we do not need to discard another
        # value.
        self.mcu_adc.read_single_value()
        # number of averaging values depends on precise mode
        n_average = ( self.adc_n_average_precise if precise else
            self.adc_n_average )
        # start averaging
        attempt = 0
        while True:
          # if printer has shut down, terminate a running probe
          if self.printer.in_shutdown_state :
            raise gcmd.error("Printer has shut down.")

          force = 0.
          min_force = +1e6
          max_force = -1e6
          for i in range(0, n_average) :
            val = self.mcu_adc.read_single_value()
            force = force + val
            min_force = min(val,min_force)
            max_force = max(val,max_force)

          if max_force - min_force < self.max_variance:
            force = force / n_average
            if abs(force) > self.max_abs_force :
              # lift tool head to prevent permament strong force applied to head
              # and bed
              self._move_z_relative(self.compensation_z_lift[self.probing_axis])
              raise gcmd.error("Maximum absolute force exceeded.")
            return force

          self.reactor.pause(self.reactor.monotonic() +
            self.delay_exceeding_max_variance)

          attempt = attempt+1
          if attempt > self.max_retry_stable_force :
            raise gcmd.error("Unstable force reading, maximum retries "
                "exceeded.")


    def _lower_to_threshold(self, gcmd):
        # Lower the tool head until the force threshold is exceeded
        while True:
          # Check threshold before first movement, to prevent doing an unchecked
          # step after a retry
          force = self._average_force(gcmd,False) - self.force_offset
          gcmd.respond_info("pos = %f, force = %.1f"
              % (self.tool.get_position()[self.probing_axis], force))
          if(abs(force) > self.threshold[self.probing_axis]):
            break
          self._move_axis_relative(                                            \
              self.probing_direction*self.step_size[self.probing_axis])


    def _fast_approach(self, gcmd):
        # Strategy for fast approach: lower tool head until exceeding threshold,
        # then lift head a bit and compare force with original force_offset. If
        # it matches, the contact is assumed. If not, the force_offset has
        # drifed and the search is continued with the new offset.
        gcmd.respond_info("Commencing fast approach.")
        self.force_offset = self._average_force(gcmd,False)
        attempt = 0
        attempt_start_pos = self.tool.get_position()[self.probing_axis]
        while True:

          # lower tool head until force threshold is exceeded
          self._lower_to_threshold(gcmd)

          # confirm contact with compensated measuerment (also updating the
          # force_offset)
          force = self._compensated_measurement(gcmd)

          # if contact is confirmed with new measurement, terminate fast
          # approach
          if(abs(force) > self.threshold[self.probing_axis]):
            # stay at slightly z-lifted position without contact when returning
            gcmd.respond_info("Fast approach found contact.")
            return force

          # check for failure condition
          attempt_dist = attempt_start_pos + \
            self.probing_direction*self.tool.get_position()[self.probing_axis]
          if attempt_dist < 2*self.step_size[self.probing_axis] :
            attempt = attempt + 1
            if attempt > self.max_retry :
              raise gcmd.error("Force reading drifting too much, maximum "
                  "retries exceeded.")
          else :
            attempt = 0
            attempt_start_pos = self.tool.get_position()[self.probing_axis]


    def _compensated_measurement(self, gcmd):
        # take compensated measurement, update force_offset
        self._move_z_relative(self.compensation_z_lift[self.probing_axis])
        self.force_offset = self._average_force(gcmd,True)
        self._move_z_relative(-self.compensation_z_lift[self.probing_axis])
        force_in = self._average_force(gcmd,True)
        force = force_in - self.force_offset

        gcmd.respond_info("pos = %f, force(cmp) = %.1f" %
            (self.tool.get_position()[self.probing_axis], force))

        return force


    def _find_fit_start(self, gcmd, force0):
        force1 = force0
        self._move_axis_relative(                                              \
            -self.probing_direction*self.step_size[self.probing_axis]/2,False)
        while abs(force1) > self.fit_threshold*2:
          force2 = self._compensated_measurement(gcmd)
          if abs(force2) < self.fit_threshold*2:
            break
          slope = (self.step_size[self.probing_axis]/2)/(force2-force1)
          dist = min(abs((force2-self.fit_threshold)*slope),                   \
              self.step_size[self.probing_axis]/2)
          self._move_axis_relative(-self.probing_direction*dist,False)
          force1 = force2


    def _perform_fit(self, gcmd):
        gcmd.respond_info("PERFORM FIT")

        # initialise array with measurement data
        data = []

        # take raster scan measurements to collect data for fit
        while True:
          force = self._compensated_measurement(gcmd)

          # check abort condition
          if len(data) >= self.fit_points:
            break

          # store measurement data for linear fit
          if abs(force) > self.fit_threshold :
            height = self.tool.get_position()[self.probing_axis]
            data.append([height, force])

          # move to next position
          self._move_axis_relative(self.probing_direction*self.fit_step_size,
              False)

        # perform fit to find zero force contact position
        heights = [ d[0] for d in data ]
        forces = [ d[1] for d in data ]
        m,b,r,sm,sb = linear_regression(forces, heights)

        gcmd.respond_info(
          "Fit result: m = %f, b = %f, r = %f, sm = %f, sb = %f"
          % (m,b,r,sm,sb))

        # safety check: r must be big enough
        if abs(r) < self.fit_min_quality :
          gcmd.respond_info(
            "Fit failed, fit quality factor r too small: %f < %f" %
            (abs(r), self.fit_min_quality))
          return None

        # return 0-force offset
        return b


    def run_probe(self, gcmd, probing_axis = 2, probing_direction = -1):
        # probe by default in Z direction
        self.probing_axis = probing_axis
        self.probing_direction = probing_direction

        # obtain toolhead object
        self.tool = self.printer.lookup_object('toolhead')

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
          if repeat_count > self.max_retry:
            raise gcmd.error("Maximum retries reached, giving up.")
          gcmd.respond_info("Retrying...")
          self._move_axis_relative(-self.probing_direction*5*                  \
              self.step_size[self.probing_axis], False)

        pos = self.tool.get_position()
        gcmd.respond_info("FINISHED toolhead Z = %f" % result)
        pos[self.probing_axis] = result
        self.last_z_result = result
        return pos[0], pos[1], pos[2]


    def cmd_PROBE_ACCURACY(self, gcmd):
        lift_speed = self.get_lift_speed(gcmd)
        sample_count = gcmd.get_int("SAMPLES", 10, minval=1)
        sample_retract_dist = gcmd.get_float("SAMPLE_RETRACT_DIST",
                                             self.sample_retract_dist, above=0.)
        dirpara = gcmd.get("DIRECTION", "Z-")
        if dirpara not in DIRECTION_CHOICE_LIST :
          raise gcmd.error("DIRECTION parameter has illegal value.")
        (axis,direction) = DIRECTION_CHOICE_LIST[dirpara]

        toolhead = self.printer.lookup_object('toolhead')
        pos = toolhead.get_position()
        gcmd.respond_info("PROBE_ACCURACY at X:%.3f Y:%.3f Z:%.3f"
                          " (samples=%d retract=%.3f"
                          " lift_speed=%.1f axis=%d)\n"
                          % (pos[0], pos[1], pos[2],
                             sample_count, sample_retract_dist,
                             lift_speed, axis))
        # Probe bed sample_count times
        self.multi_probe_begin()
        positions = []
        while len(positions) < sample_count:
            # Probe position
            pos = self.run_probe(gcmd, axis, direction)
            positions.append(pos)
            # Retract
            self._move_axis_relative(-direction*sample_retract_dist)
        self.multi_probe_end()
        # Calculate maximum, minimum and average values
        max_value = max([p[axis] for p in positions])
        min_value = min([p[axis] for p in positions])
        range_value = max_value - min_value
        avg_value = self._calc_mean(positions)[axis]
        median = self._calc_median(positions)[axis]
        # calculate the standard deviation
        deviation_sum = 0
        for i in range(len(positions)):
            deviation_sum += pow(positions[i][axis] - avg_value, 2.)
        sigma = (deviation_sum / len(positions)) ** 0.5
        # Show information
        gcmd.respond_info(
            "probe accuracy results: maximum %.6f, minimum %.6f, range %.6f, "
            "average %.6f, median %.6f, standard deviation %.6f" % (
            max_value, min_value, range_value, avg_value, median, sigma))


    def cmd_PROBE(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead')
        pos = toolhead.get_position()

        dirpara = gcmd.get("DIRECTION", "Z-")
        if dirpara not in DIRECTION_CHOICE_LIST :
          raise gcmd.error("DIRECTION parameter has illegal value.")
        (axis,direction) = DIRECTION_CHOICE_LIST[dirpara]

        gcmd.respond_info("PROBE at X:%.3f Y:%.3f Z:%.3f"
                          " (axis=%d direction=%d)\n"
                          % (pos[0], pos[1], pos[2], axis, direction))

        pos = self.run_probe(gcmd, axis, direction)
        self.tool.manual_move([pos[0],pos[1],pos[2]], self.speed)
        self.tool.wait_moves()


    def _calc_mean(self, positions):
        count = float(len(positions))
        return [sum([pos[i] for pos in positions]) / count
                for i in range(3)]


    def _calc_median(self, positions):
        z_sorted = sorted(positions, key=(lambda p: p[2]))
        middle = len(positions) // 2
        if (len(positions) & 1) == 1:
            # odd number of samples
            return z_sorted[middle]
        # even number of samples
        return self._calc_mean(z_sorted[middle-1:middle+1])


    def cmd_READ_LOAD_CELL(self, gcmd):
        force = self._average_force(gcmd,True)
        gcmd.respond_info("Uncompensated: %.1f  compensated: %.1f" %           \
                          (force, force - self.force_offset))


    def cmd_COMPENSATE_LOAD_CELL(self, gcmd):
        self.force_offset = self._average_force(gcmd,True)


    def get_status(self, eventtime):
        return {
          'last_force': self.last_force,
          'last_z_result': self.last_z_result,
        }


def load_config(config):
    probe = LoadCellProbe(config)
    config.printer.add_object('probe', probe)
    # Add second "load_cell" object to make load cell measurement available to
    # other modules (e.g. z_sense_offset)
    config.printer.add_object('load_cell', probe)
    return probe
