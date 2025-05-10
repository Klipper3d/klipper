# Load Cell Probe
#
# Copyright (C) 2025  Gareth Farrington <gareth@waves.ky>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math
import mcu
from . import probe, sos_filter, load_cell, hx71x, ads1220
from .sos_filter import FixedPointSosFilter

Q2_INT_BITS = 2
Q2_FRAC_BITS = (32 - (1 + Q2_INT_BITS))
Q16_INT_BITS = 16
Q16_FRAC_BITS = (32 - (1 + Q16_INT_BITS))

class TapAnalysis:
    def __init__(self, samples):
        import numpy as np
        nd_samples = np.asarray(samples, dtype=np.float64)
        self.time = nd_samples[:, 0]
        self.force = nd_samples[:, 1]

    # convert to dictionary for JSON encoder
    def to_dict(self):
        return {
            'time': self.time.tolist(),
            'force': self.force.tolist(),
            'is_valid': True,
        }

class ContinuousTareFilter:
    def __init__(self, config, sensor):
        self._config = config
        self._sensor = sensor
        filter_design = self._build_filter_design()
        self._sos_filter = self._build_filter(filter_design)
    def _build_filter_design(self):
        sps = self._sensor.get_samples_per_second()
        max_filter_frequency = math.floor(sps / 2.)
        hp_option = "drift_filter_cutoff_frequency"
        highpass = self._config.getfloat(hp_option, minval=0.1, maxval=20.0,
            below=max_filter_frequency, default=None)
        lowpass = self._config.getfloat("buzz_filter_cutoff_frequency",
            above=min(80.0, max_filter_frequency - 1.0),
            below=max_filter_frequency, default=None)
        notches = sos_filter.getfloatlist(self._config,
            "notch_filter_frequencies", max_len=2, above=(highpass or 0.),
            below=(lowpass or max_filter_frequency))
        notch_quality = self._config.getfloat("notch_filter_quality",
            minval=0.5, maxval=6.0, default=2.0)
        return sos_filter.DigitalFilter(sps, self._config.error, highpass,
                             lowpass, notches, notch_quality)

    def _build_filter(self, filter_design):
        fixed_filter = FixedPointSosFilter(filter_design.get_filter_sections(),
            filter_design.get_initial_state(), Q2_INT_BITS, Q16_FRAC_BITS)
        return sos_filter.SosFilter(self._sensor.get_mcu(), fixed_filter, 4)

    def get_sos_filter(self):
        return self._sos_filter


# check results from the collector for errors and raise an exception is found
def check_sensor_errors(results, printer):
    samples, errors = results
    if errors:
        raise printer.command_error("Load cell sensor reported errors while"
            " probing: %i errors, %i overflows"
            % (errors[0], errors[1]))
    return samples

class LoadCellProbeConfigHelper:
    TRIGGER_FORCE_MIN = 10
    TRIGGER_FORCE_MAX = 250
    def __init__(self, config, load_cell_inst):
        self._config = config
        self._printer = config.get_printer()
        self._load_cell = load_cell_inst
        self._sensor = load_cell_inst.get_sensor()
        self._rest_time = 1. / float(self._sensor.get_samples_per_second())
        # Collect 4x60hz power cycles of data to average across power noise
        sps = self._sensor.get_samples_per_second()
        default_tare_samples = max(2, round(sps * ((1 / 60) * 4)))
        self._tare_samples = config.getfloat(
            'tare_samples', default=default_tare_samples,minval=2, maxval=sps)
        # triggering options
        self._trigger_force_grams = config.getint('trigger_force',
            minval=self.TRIGGER_FORCE_MIN, maxval=self.TRIGGER_FORCE_MAX,
            default=75)
        self._force_safety_limit_grams = config.getint(
            'force_safety_limit', minval=100, maxval=5000, default=2000)

    def get_tare_samples(self):
        return self._tare_samples

    def get_trigger_force_grams(self, gcmd=None):
        if gcmd is None:
            return self._trigger_force_grams
        return gcmd.get_int("TRIGGER_FORCE_GRAMS", self._trigger_force_grams,
            minval=self.TRIGGER_FORCE_MIN, maxval=self.TRIGGER_FORCE_MAX)

    def get_rest_time(self):
        return self._rest_time

    def get_safety_limit_grams(self):
        return self._force_safety_limit_grams

    def get_safety_range(self):
        counts_per_gram = self._load_cell.get_counts_per_gram()
        # calculate the safety band
        zero = self._load_cell.get_reference_tare_counts()
        safety_counts = int(counts_per_gram * self._force_safety_limit_grams)
        safety_min = int(zero - safety_counts)
        safety_max = int(zero + safety_counts)
        # don't allow a safety range outside the sensor's real range
        sensor_min, sensor_max = self._load_cell.get_sensor().get_range()
        if safety_min <= sensor_min or  safety_max >= sensor_max:
            cmd_err = self._printer.command_error
            raise cmd_err("Load cell force_safety_limit exceeds sensor range!")
        return safety_min, safety_max

    def get_grams_per_count(self):
        counts_per_gram = self._load_cell.get_counts_per_gram()
        # The counts_per_gram could be so large that it becomes 0.0 when
        # converted to Q2 format. This would mean the ADC range only measures a
        # few grams which seems very unlikely. Treat this as an error:
        if counts_per_gram >= 2**Q2_FRAC_BITS:
            raise OverflowError("counts_per_gram value is too large to filter")
        return sos_filter.to_fixed_32((1. / counts_per_gram), Q2_INT_BITS)


# McuLoadCellProbe is the interface to `load_cell_probe` on the MCU
# This also manages the SosFilter so all commands use one command queue
class McuLoadCellProbe:
    WATCHDOG_MAX = 3
    ERROR_SAFETY_RANGE = mcu.MCU_trsync.REASON_COMMS_TIMEOUT + 1
    ERROR_OVERFLOW = mcu.MCU_trsync.REASON_COMMS_TIMEOUT + 2
    ERROR_WATCHDOG = mcu.MCU_trsync.REASON_COMMS_TIMEOUT + 3
    def __init__(self, config, load_cell_inst, continuous_tare_filter,
                 config_helper):
        self._config = config
        self._printer = config.get_printer()
        self._load_cell = load_cell_inst
        self._continuous_tare_filter = continuous_tare_filter
        self._sos_filter = continuous_tare_filter.get_sos_filter()
        self._config_helper = config_helper
        self._sensor = load_cell_inst.get_sensor()
        self._mcu = self._sensor.get_mcu()
        # configure MCU objects
        self._dispatch = mcu.TriggerDispatch(self._mcu)
        self._cmd_queue = self._dispatch.get_command_queue()
        self._oid = self._mcu.create_oid()
        self._config_commands()
        self._home_cmd = None
        self._query_cmd = None
        self._set_range_cmd = None
        self._mcu.register_config_callback(self._build_config)
        self._printer.register_event_handler(
            "klippy:connect", self._on_connect)
        self._printer.register_event_handler(
            "load_cell:tare", self._on_load_cell_tare)

    def _config_commands(self):
        self._sos_filter.create_filter()
        self._mcu.add_config_cmd(
            "config_load_cell_probe oid=%d sos_filter_oid=%d"
             % (self._oid, self._sos_filter.get_oid()))

    def _build_config(self):
        # Lookup commands
        self._query_cmd = self._mcu.lookup_query_command(
            "load_cell_probe_query_state oid=%c",
            "load_cell_probe_state oid=%c is_homing_trigger=%c trigger_ticks=%u"
            , oid=self._oid, cq=self._cmd_queue)
        self._set_range_cmd = self._mcu.lookup_command(
            "load_cell_probe_set_range"
            " oid=%c safety_counts_min=%i safety_counts_max=%i tare_counts=%i"
            " trigger_grams=%u grams_per_count=%i"
            , cq=self._cmd_queue)
        self._home_cmd = self._mcu.lookup_command(
            "load_cell_probe_home oid=%c trsync_oid=%c trigger_reason=%c"
            " error_reason=%c clock=%u rest_ticks=%u timeout=%u"
            , cq=self._cmd_queue)

    # the sensor data stream is connected on the MCU at the ready event
    def _on_connect(self):
        self._sensor.attach_load_cell_probe(self._oid)

    def _on_load_cell_tare(self, lc):
        if lc is self._load_cell:
            # logging.info("load cell tare event: %s" % (lc.get_tare_counts(),))
            self.set_endstop_range(int(lc.get_tare_counts()))

    def get_oid(self):
        return self._oid

    def get_mcu(self):
        return self._mcu

    def get_load_cell(self):
        return self._load_cell

    def get_dispatch(self):
        return self._dispatch

    def set_endstop_range(self, tare_counts, gcmd=None):
        # update internal tare value
        safety_min, safety_max = self._config_helper.get_safety_range()
        args = [self._oid, safety_min, safety_max, int(tare_counts),
                self._config_helper.get_trigger_force_grams(gcmd),
                self._config_helper.get_grams_per_count()]
        self._set_range_cmd.send(args)
        self._sos_filter.reset_filter(cq=self._cmd_queue)

    def home_start(self, print_time):
        clock = self._mcu.print_time_to_clock(print_time)
        rest_time = self._config_helper.get_rest_time()
        rest_ticks = self._mcu.seconds_to_clock(rest_time)
        self._home_cmd.send([self._oid, self._dispatch.get_oid(),
            mcu.MCU_trsync.REASON_ENDSTOP_HIT, self.ERROR_SAFETY_RANGE, clock,
            rest_ticks, self.WATCHDOG_MAX], reqclock=clock)

    def clear_home(self):
        params = self._query_cmd.send([self._oid])
        # The time of the first sample that triggered is in "trigger_ticks"
        trigger_ticks = self._mcu.clock32_to_clock64(params['trigger_ticks'])
        # clear trsync from load_cell_endstop
        self._home_cmd.send([self._oid, 0, 0, 0, 0, 0, 0, 0])
        return self._mcu.clock_to_print_time(trigger_ticks)


# Execute probing moves using the McuLoadCellProbe
class LoadCellProbingMove:
    ERROR_MAP = {
        mcu.MCU_trsync.REASON_COMMS_TIMEOUT:
            "Communication timeout during homing",
        McuLoadCellProbe.ERROR_SAFETY_RANGE:
            "Load Cell Probe Error: load exceeds safety limit",
        McuLoadCellProbe.ERROR_OVERFLOW:
            "Load Cell Probe Error: fixed point math overflow",
        McuLoadCellProbe.ERROR_WATCHDOG:
            "Load Cell Probe Error: timed out waiting for sensor data"
    }
    def __init__(self, config, mcu_load_cell_probe,
                 param_helper, config_helper):
        self._printer = config.get_printer()
        self._mcu_load_cell_probe = mcu_load_cell_probe
        self._param_helper = param_helper
        self._config_helper = config_helper
        self._mcu = mcu_load_cell_probe.get_mcu()
        self._load_cell = mcu_load_cell_probe.get_load_cell()
        self._z_min_position = probe.lookup_minimum_z(config)
        self._dispatch = mcu_load_cell_probe.get_dispatch()
        probe.LookupZSteppers(config, self._dispatch.add_stepper)
        # internal state tracking
        self._tare_counts = 0
        self._last_trigger_time = 0

    def _start_collector(self):
        # homing uses the toolhead last move time which gets special handling
        # to significantly buffer print_time if the move queue has drained
        toolhead = self._printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        collector = self._load_cell.get_collector()
        collector.start_collecting(min_time=print_time)
        return collector

    # pauses for the last move to complete and then tares the load_cell
    # returns the last sample record used in taring
    def _pause_and_tare(self, gcmd):
        import numpy as np
        collector = self._start_collector()
        num_samples = self._config_helper.get_tare_samples()
        results = collector.collect_min(num_samples)
        tare_samples = check_sensor_errors(results, self._printer)
        tare_counts = np.average(np.array(tare_samples)[:, 2].astype(float))
        self._mcu_load_cell_probe.set_endstop_range(int(tare_counts), gcmd)

    def _home_start(self, print_time):
        cmd_err = self._printer.command_error
        if not self._load_cell.is_calibrated():
            raise cmd_err("Probe aborted, Load cell not calibrated")
        # start trsync
        trigger_completion = self._dispatch.start(print_time)
        self._mcu_load_cell_probe.home_start(print_time)
        return trigger_completion

    def home_start(self, print_time, sample_time, sample_count, rest_time,
                   triggered=True):
        return self._home_start(print_time)

    def home_wait(self, home_end_time):
        self._dispatch.wait_end(home_end_time)
        # trigger has happened, now to find out why...
        res = self._dispatch.stop()
        # clear the homing state so it stops processing samples
        self._last_trigger_time = self._mcu_load_cell_probe.clear_home()
        if res >= mcu.MCU_trsync.REASON_COMMS_TIMEOUT:
            error = "Load Cell Probe Error: unknown reason code %i" % (res,)
            if res in self.ERROR_MAP:
                error = self.ERROR_MAP[res]
            raise self._printer.command_error(error)
        if res != mcu.MCU_trsync.REASON_ENDSTOP_HIT:
            return 0.
        return self._last_trigger_time

    def get_steppers(self):
        return self._dispatch.get_steppers()

    # Probe towards z_min until the load_cell_probe on the MCU triggers
    def probing_move(self, gcmd):
        # do not permit probing if the load cell is not calibrated
        if not self._load_cell.is_calibrated():
            raise self._printer.command_error("Load Cell not calibrated")
        # tare the sensor just before probing
        self._pause_and_tare(gcmd)
        # get params for the homing move
        toolhead = self._printer.lookup_object('toolhead')
        pos = toolhead.get_position()
        pos[2] = self._z_min_position
        speed = self._param_helper.get_probe_params(gcmd)['probe_speed']
        phoming = self._printer.lookup_object('homing')
        # start collector after tare samples are consumed
        collector = self._start_collector()
        # do homing move
        return phoming.probing_move(self, pos, speed), collector

    # Wait for the MCU to trigger with no movement
    def probing_test(self, gcmd, timeout):
        self._pause_and_tare(gcmd)
        toolhead = self._printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        self._home_start(print_time)
        #review: should this be toolhead.get_last_move_time()
        systime = self._printer.get_reactor().monotonic()
        print_time = self._mcu.estimated_print_time(systime) + timeout
        return self.home_wait(print_time)

    def get_status(self, eventtime):
        return {
            'tare_counts': self._tare_counts,
            'last_trigger_time': self._last_trigger_time,
        }


# Perform a single complete tap
class TappingMove:
    def __init__(self, config, mcu_load_cell_probe,
                 config_helper):
        self._printer = config.get_printer()
        self._mcu_load_cell_probe = mcu_load_cell_probe
        self._config_helper = config_helper
        # track results of the last tap
        self._last_result = None
        self._is_last_result_valid = False
        # webhooks support
        self._clients = load_cell.ApiClientHelper(config.get_printer())
        name = config.get_name()
        header = {"header": ["probe_tap_event"]}
        self._clients.add_mux_endpoint("load_cell_probe/dump_taps",
                                      "load_cell_probe", name, header)

    # ProbeSession interface
    def run_tap(self, gcmd):
        # do the descending move
        epos, collector = self._mcu_load_cell_probe.probing_move(gcmd)
        # collect samples from the tap
        toolhead = self._printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        move_end = toolhead.get_last_move_time()
        results = collector.collect_until(move_end)
        samples = check_sensor_errors(results, self._printer)
        # Analyze the tap data
        ppa = TapAnalysis(samples)
        # broadcast tap event data:
        self._clients.send({'tap': ppa.to_dict()})
        self._is_last_result_valid = True
        self._last_result = epos[2]
        return epos, self._is_last_result_valid

    def get_status(self, eventtime):
        return {
            'last_z_result': self._last_result,
            'is_last_tap_valid': self._is_last_result_valid
        }


# Probe `activate_gcode` and `deactivate_gcode` support
class ProbeActivationHelper:
    def __init__(self, config):
        self._printer = config.get_printer()
        gcode_macro = self._printer.load_object(config, 'gcode_macro')
        self._activate_gcode = gcode_macro.load_template(
            config, 'activate_gcode', '')
        self._deactivate_gcode = gcode_macro.load_template(
            config, 'deactivate_gcode', '')
    def activate_probe(self):
        toolhead = self._printer.lookup_object('toolhead')
        start_pos = toolhead.get_position()
        self._activate_gcode.run_gcode_from_command()
        if toolhead.get_position()[:3] != start_pos[:3]:
            raise self._printer.command_error(
                "Toolhead moved during probe activate_gcode script")
    def deactivate_probe(self):
        toolhead = self._printer.lookup_object('toolhead')
        start_pos = toolhead.get_position()
        self._deactivate_gcode.run_gcode_from_command()
        if toolhead.get_position()[:3] != start_pos[:3]:
            raise self._printer.command_error(
                "Toolhead moved during probe deactivate_gcode script")

# ProbeSession that implements Tap logic
class TapSession:
    def __init__(self, config, tapping_move, probe_params_helper):
        self.printer = config.get_printer()
        self.load_cell_probe_session = tapping_move
        self.probe_params_helper = probe_params_helper
        # Session state
        self.results = []
        self.activator = ProbeActivationHelper(config)

    def start_probe_session(self, gcmd):
        self.activator.activate_probe()
        return self

    def end_probe_session(self):
        self.activator.deactivate_probe()
        self.results = []

    # probe until a single good sample is returned or retries are exhausted
    def run_probe(self, gcmd):
        epos, is_good = self.load_cell_probe_session.run_tap(gcmd)
        self.results.append(epos)

    def pull_probed_results(self):
        res = self.results
        self.results = []
        return res

class SingleTapSession:
    def __init__(self, config, tapping_move):
        self.config = config
        self.tapping_move = tapping_move

class LoadCellProbeCommands:
    def __init__(self, config, load_cell_probing_move,
                 tapping_move, config_helper):
        self._printer = config.get_printer()
        self._load_cell_probing_move = load_cell_probing_move
        self._tapping_move = tapping_move
        self._config_helper = config_helper
        self._register_commands()

    def _register_commands(self):
        # Register commands
        gcode = self._printer.lookup_object('gcode')
        # for now there can only be 1 probe so no need for mux_command
        gcode.register_command("LOAD_CELL_TAP",
            self.cmd_LOAD_CELL_TAP, desc=self.cmd_LOAD_CELL_TAP_help)
        gcode.register_command("LOAD_CELL_TEST_TAP",
            self.cmd_LOAD_CELL_TEST_TAP, desc=self.cmd_LOAD_CELL_TEST_TAP_help)

    cmd_LOAD_CELL_TAP_help="Perform a single tap"
    def cmd_LOAD_CELL_TAP(self, gcmd):
        self._tapping_move.run_tap(gcmd)

    cmd_LOAD_CELL_TEST_TAP_help = "Tap the load cell probe to verify operation"
    def cmd_LOAD_CELL_TEST_TAP(self, gcmd):
        taps = gcmd.get_int("TAPS", 3, minval=1, maxval=10)
        timeout = gcmd.get_float("TIMEOUT_SECONDS", 30., minval=1., maxval=120.)
        gcmd.respond_info("Tap the load cell %s times:" % (taps,))
        for i in range(0, taps):
            result = self._load_cell_probing_move.probing_test(gcmd, timeout)
            if result == 0.:
                # notify of error, likely due to timeout
                raise gcmd.error("Test timeout out")
            gcmd.respond_info("Tap Detected!")
        gcmd.respond_info("Test complete, %s taps detected" % (taps,))


class LoadCellPrinterProbe:
    def __init__(self, config):
        cfg_error = config.error
        try:
            import numpy as np
        except:
            raise cfg_error("[load_cell_probe] requires the NumPy module")
        self._config = config
        self._printer = config.get_printer()
        # Sensor types supported by load_cell_probe
        sensors = {}
        sensors.update(hx71x.HX71X_SENSOR_TYPES)
        sensors.update(ads1220.ADS1220_SENSOR_TYPE)
        sensor_class = config.getchoice('sensor_type', sensors)
        sensor = sensor_class(config)
        self._load_cell = load_cell.LoadCell(config, sensor)
        # Read all user configuration and build modules
        config_helper = LoadCellProbeConfigHelper(config, self._load_cell)
        continuous_tare_filter = ContinuousTareFilter(config, sensor)
        # Probe Interface
        self._param_helper = probe.ProbeParameterHelper(config)
        self._cmd_helper = probe.ProbeCommandHelper(config, self)
        self._probe_offsets = probe.ProbeOffsetsHelper(config)
        # custom
        self._mcu_load_cell_probe = McuLoadCellProbe(
            config, self._load_cell, continuous_tare_filter, config_helper)
        load_cell_probing_move = LoadCellProbingMove(config,
            self._mcu_load_cell_probe, self._param_helper, config_helper)
        self._tapping_move = TappingMove(
            config, load_cell_probing_move, config_helper)
        tap_session = TapSession(
            config, self._tapping_move, self._param_helper)
        self._probe_session = probe.ProbeSessionHelper(
            config, self._param_helper, tap_session.start_probe_session)
        # printer integration
        LoadCellProbeCommands(
            config, load_cell_probing_move, self._tapping_move, config_helper)
        probe.ProbeVirtualEndstopDeprecation(config)
        self._printer.add_object('probe', self)

    # load a module from config, fallback to a default implementation
    def _load_module(self, name, default):
        module = self._config.get(name, default=None)
        if module is None:
            return default
        return self._printer.lookup_object(module)

    def get_probe_params(self, gcmd=None):
        return self._param_helper.get_probe_params(gcmd)

    def get_offsets(self):
        return self._probe_offsets.get_offsets()

    def start_probe_session(self, gcmd):
        return self._probe_session.start_probe_session(gcmd)

    def get_status(self, eventtime):
        status = self._cmd_helper.get_status(eventtime)
        status.update(self._load_cell.get_status(eventtime))
        status.update(self._tapping_move.get_status(eventtime))
        return status


def load_config(config):
    return LoadCellPrinterProbe(config)
