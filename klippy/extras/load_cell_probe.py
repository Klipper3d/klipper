# Load Cell Probe
#
# Copyright (C) 2023 Gareth Farrington <gareth@waves.ky>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math
import mcu
from . import probe, sos_filter, load_cell, hx71x, ads1220


#class to keep context across probing/homing events
class ProbeSessionContext:
    def __init__(self, config, load_cell_inst):
        self.printer = printer = config.get_printer()
        self.load_cell = load_cell_inst
        self.collector = None
        # webhooks support
        self.clients = load_cell.ApiClientHelper(printer)
        name = config.get_name()
        header = {"header": ["probe_tap_event"]}
        self.clients.add_mux_endpoint("load_cell_probe/dump_taps",
                                        "load_cell_probe", name, header)

    def load_module(self, config, name, default):
        module = config.get(name, default=None)
        return default if module is None else self.printer.lookup_object(module)

    def notify_probe_start(self, print_time):
        if self.collector is not None:
            self.collector.start_collecting(print_time)

    def tapping_move(self, mcu_probe, pos, speed):
        self.collector = self.load_cell.get_collector()
        toolhead = self.printer.lookup_object('toolhead')
        curtime = self.printer.get_reactor().monotonic()
        if 'z' not in toolhead.get_status(curtime)['homed_axes']:
            raise self.printer.command_error("Must home before probe")
        phoming = self.printer.lookup_object('homing')
        epos = phoming.probing_move(mcu_probe, pos, speed)

        toolhead = self.printer.lookup_object('toolhead')
        probe_end = toolhead.get_last_move_time()
        samples, errors = self.collector.collect_until(probe_end)
        if errors:
            raise self.printer.command_error(
                "Sensor reported errors while homing: %i errors, %i overflows"
                % (errors[0], errors[1]))
        self.collector = None
        # broadcast tap event data:
        self.clients.send({'tap': {
                'time': load_cell.select_column(samples, 0),
                'force': load_cell.select_column(samples, 1),
            }})
        return epos


WATCHDOG_MAX = 3
# LoadCellEndstop implements both MCU_endstop and ProbeEndstopWrapper
class LoadCellEndstop:
    REASON_SENSOR_ERROR = mcu.MCU_trsync.REASON_COMMS_TIMEOUT + 1
    def __init__(self, config, load_cell_inst, helper):
        self._config = config
        self._config_name = config.get_name()
        self.printer = printer = config.get_printer()
        self.gcode = printer.lookup_object('gcode')
        printer.register_event_handler('klippy:mcu_identify',
                                       self._handle_mcu_identify)
        gcode_macro = self.printer.load_object(config, 'gcode_macro')
        self.activate_gcode = gcode_macro.load_template(
            config, 'activate_gcode', '')
        self.deactivate_gcode = gcode_macro.load_template(
            config, 'deactivate_gcode', '')
        self._load_cell = load_cell_inst
        self._helper = helper
        self._sensor = sensor = load_cell_inst.get_sensor()
        self._mcu = sensor.get_mcu()
        self._oid = self._mcu.create_oid()
        self._sos_filter = sos_filter.SosFilter(self._mcu)
        self._dispatch = mcu.TriggerDispatch(self._mcu)
        self._rest_time = 1. / float(sensor.get_samples_per_second())
        self._home_cmd = None
        self._query_cmd = None
        self._set_range_cmd = None
        # internal tare tracking
        self.tare_counts = 0
        self.last_trigger_time = 0
        self._position_endstop = config.getfloat('z_offset')
        # triggering options
        self.trigger_force_grams = config.getint('trigger_force',
                                    minval=10, maxval=250, default=75)
        self.force_safety_limit_grams = config.getint('force_safety_limit',
                                          minval=100, maxval=5000, default=2000)
        # optional continuous tearing
        self._continuous_tare_filter = self._build_sos_filter(config)
        # configure MCU objects
        self._config_commands()
        self._mcu.register_config_callback(self._build_config)
        printer.register_event_handler("klippy:ready", self._handle_ready)
        printer.register_event_handler("load_cell:tare",
                                       self._handle_load_cell_tare)
        # multi probes state
        self.multi = 'OFF'

    def _config_commands(self):
        self._sos_filter.create_filter()
        sos_filters = self._continuous_tare_filter.get_filter_sections()
        self._sos_filter.configure_filter(sos_filters)
        self._mcu.add_config_cmd("config_load_cell_endstop oid=%d"
                                 " sos_filter_oid=%d"
                                 % (self._oid, self._sos_filter.oid))

    def _build_config(self):
        # Lookup commands
        cmd_queue = self._dispatch.get_command_queue()
        self._query_cmd = self._mcu.lookup_query_command(
            "load_cell_endstop_query_state oid=%c",
            "load_cell_endstop_state oid=%c homing=%c homing_triggered=%c"
            " is_triggered=%c trigger_ticks=%u sample=%i sample_ticks=%u"
            , oid=self._oid, cq=cmd_queue)
        self._set_range_cmd = self._mcu.lookup_command(
            "set_range_load_cell_endstop"
            " oid=%c safety_counts_min=%i safety_counts_max=%i tare_counts=%i"
            " trigger_grams=%u grams_per_count=%i"
            , cq=cmd_queue)
        self._home_cmd = self._mcu.lookup_command(
            "load_cell_endstop_home oid=%c trsync_oid=%c trigger_reason=%c"
            " error_reason=%c clock=%u rest_ticks=%u timeout=%u"
            , cq=cmd_queue)

    def _build_sos_filter(self, config):
        # optional continuous tearing
        sps = self._load_cell.get_sensor().get_samples_per_second()
        # Collect 4x60hz power cycles of data to average across power noise
        default_tare_samples = max(2, round(sps * ((1 / 60) * 4)))
        self.tare_samples = config.getfloat('tare_samples',
                                            default=default_tare_samples,
                                            minval=2, maxval=sps)
        max_filter_frequency = math.floor(sps / 2.)
        hp_option = "drift_filter_cutoff_frequency"
        highpass = config.getfloat(hp_option, minval=0.1, maxval=20.0,
                                    below=max_filter_frequency, default=None)
        lowpass = config.getfloat("buzz_filter_cutoff_frequency",
                                    above=min(80.0, max_filter_frequency - 1.0),
                                    below=max_filter_frequency, default=None)
        notches = sos_filter.getfloatlist(config, "notch_filter_frequencies",
                                    max_len=2,
                                    above=(highpass or 0.),
                                    below=(lowpass or max_filter_frequency))
        notch_quality = config.getfloat("notch_filter_quality",
                                        minval=0.5, maxval=6.0, default=2.0)
        return sos_filter.DigitalFilter(sps, config.error, highpass,
                             lowpass, notches, notch_quality)

    def _handle_ready(self):
        self._sensor.attach_endstop(self._oid)

    def _handle_load_cell_tare(self, lc):
        if lc is self._load_cell:
            # logging.info("load cell tare event: %s" % (lc.get_tare_counts(),))
            self._set_endstop_range(int(lc.get_tare_counts()))

    def _handle_mcu_identify(self):
        kinematics = self.printer.lookup_object('toolhead').get_kinematics()
        for stepper in kinematics.get_steppers():
            if stepper.is_active_axis('z'):
                self.add_stepper(stepper)

    def get_status(self, eventtime):
        return {
            'endstop_tare_counts': self.tare_counts,
            'last_trigger_time': self.last_trigger_time
        }

    def _get_safety_range(self):
        counts_per_gram = self._load_cell.get_counts_per_gram()
        # calculate the safety band
        zero = self._load_cell.get_reference_tare_counts()
        safety_counts = int(counts_per_gram * self.force_safety_limit_grams)
        return int(zero - safety_counts), int(zero + safety_counts)

    def _get_filter_converter(self):
        counts_per_gram = self._load_cell.get_counts_per_gram()
        # The counts_per_gram could be so large that it becomes 0.0 when
        # converted to Q1 format. This would mean the ADC range only measures a
        # few grams which seems very unlikely. Treat this as an error:
        if counts_per_gram >= 2**sos_filter.Q1_FRAC_BITS:
            raise OverflowError("Counts per gram value too large to filter")
        grams_per_count = sos_filter.as_fixed_q1(1. / counts_per_gram)
        return grams_per_count

    def _clear_home(self):
        params = self._query_cmd.send([self._oid])
        # clear trsync from load_cell_endstop
        self._home_cmd.send([self._oid, 0, 0, 0, 0, 0, 0, 0])
        # The time of the first sample that triggered is in "trigger_ticks"
        trigger_ticks = self._mcu.clock32_to_clock64(params['trigger_ticks'])
        return self._mcu.clock_to_print_time(trigger_ticks)

    def _set_endstop_range(self, tare_counts):
        cmd_err = self.printer.command_error
        if not self._load_cell.is_calibrated():
            raise cmd_err("Probe aborted, Load cell not calibrated")
        # update internal tare value
        tare_counts = int(tare_counts)
        self.tare_counts = tare_counts
        safety_min, safety_max = self._get_safety_range()
        # counts to grams conversion factors
        grams_per_count  = self._get_filter_converter()
        args = [self._oid, safety_min, safety_max, tare_counts,
                self.trigger_force_grams, grams_per_count]
        self._set_range_cmd.send(args)

    # pauses for the last move to complete and then tares the load_cell
    # returns the last sample record used in taring
    def _pause_and_tare(self):
        import numpy as np
        toolhead = self.printer.lookup_object('toolhead')
        collector = self._load_cell.get_collector()
        # collect tare_samples AFTER current move ends
        collector.start_collecting(min_time=toolhead.get_last_move_time())
        tare_samples, errors = collector.collect_min(self.tare_samples)
        if errors:
            raise self.printer.command_error(
            "Sensor reported errors while homing: %i errors, %i overflows"
                              % (errors[0], errors[1]))
        tare_counts = np.average(np.array(tare_samples)[:, 2].astype(float))
        self._set_endstop_range(int(tare_counts))

    def get_oid(self):
        return self._oid

    ####
    # Interface for MCU_endstop
    def get_mcu(self):
        return self._mcu

    def add_stepper(self, stepper):
        self._dispatch.add_stepper(stepper)

    def get_steppers(self):
        return self._dispatch.get_steppers()

    def home_start(self, print_time, sample_time, sample_count, rest_time,
                   triggered=True):
        # do not permit homing if the load cell is not calibrated
        if not self._load_cell.is_calibrated():
            raise self.printer.command_error("Load Cell not calibrated")
        # tare the sensor just before probing
        # this uses pause(), requiring a print_time update
        self._pause_and_tare()
        # homing uses the toolhead last move time which gets special handling
        # to significantly buffer print_time if the move queue has drained
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        # start collector, only used when probing
        self._helper.notify_probe_start(print_time)
        # start trsync
        trigger_completion = self._dispatch.start(print_time)
        clock = self._mcu.print_time_to_clock(print_time)
        rest_ticks = self._mcu.seconds_to_clock(self._rest_time)
        self._home_cmd.send([self._oid, self._dispatch.get_oid(),
            mcu.MCU_trsync.REASON_ENDSTOP_HIT, self.REASON_SENSOR_ERROR, clock,
            rest_ticks, WATCHDOG_MAX], reqclock=clock)
        return trigger_completion

    def home_wait(self, home_end_time):
        self._dispatch.wait_end(home_end_time)
        # trigger has happened, now to find out why...
        res = self._dispatch.stop()
        if res >= mcu.MCU_trsync.REASON_COMMS_TIMEOUT:
            if res == mcu.MCU_trsync.REASON_COMMS_TIMEOUT:
                raise self.printer.command_error(
                    "Communication timeout during homing")
            raise self.printer.command_error("Load cell sensor error")
        if res != mcu.MCU_trsync.REASON_ENDSTOP_HIT:
            return 0.
        if self._mcu.is_fileoutput():
            return home_end_time
        self.last_trigger_time = self._clear_home()
        return self.last_trigger_time

    def query_endstop(self, print_time):
        clock = self._mcu.print_time_to_clock(print_time)
        if self._mcu.is_fileoutput():
            return 0
        params = self._query_cmd.send([self._oid], minclock=clock)
        #logging.info("Query Load Cell Endstop params returned: %s" % (params,))
        if params['homing'] == 1:
            return params['homing_triggered'] == 1
        else:
            return params['is_triggered'] == 1

    ####
    # Interface for ProbeEndstopWrapper
    def _raise_probe(self):
        toolhead = self.printer.lookup_object('toolhead')
        start_pos = toolhead.get_position()
        self.deactivate_gcode.run_gcode_from_command()
        if toolhead.get_position()[:3] != start_pos[:3]:
            raise self.printer.command_error(
                "Toolhead moved during probe deactivate_gcode script")

    def _lower_probe(self):
        toolhead = self.printer.lookup_object('toolhead')
        start_pos = toolhead.get_position()
        self.activate_gcode.run_gcode_from_command()
        if toolhead.get_position()[:3] != start_pos[:3]:
            raise self.printer.command_error(
                "Toolhead moved during probe activate_gcode script")

    def probing_move(self, pos, speed):
        return self._helper.tapping_move(self, pos, speed)

    def multi_probe_begin(self):
        self.multi = 'FIRST'

    def multi_probe_end(self):
        self._raise_probe()
        self.multi = 'OFF'

    def probe_prepare(self, hmove):
        if self.multi == 'OFF' or self.multi == 'FIRST':
            self._lower_probe()
            if self.multi == 'FIRST':
                self.multi = 'ON'

    def probe_finish(self, hmove):
        if self.multi == 'OFF':
            self._raise_probe()

    def get_position_endstop(self):
        return self._position_endstop


class LoadCellPrinterProbe:
    def __init__(self, config, load_cell_inst, load_cell_endstop):
        self.printer = config.get_printer()
        self.mcu_probe = load_cell_endstop
        self._load_cell = load_cell_inst
        self.cmd_helper = probe.ProbeCommandHelper(config, self,
                                             self.mcu_probe.query_endstop)
        self.probe_offsets = probe.ProbeOffsetsHelper(config)
        self.probe_session = probe.ProbeSessionHelper(config, self.mcu_probe)

    # Copy of PrinterProbe methods
    def get_probe_params(self, gcmd=None):
        return self.probe_session.get_probe_params(gcmd)

    def get_offsets(self):
        return self.probe_offsets.get_offsets()

    def start_probe_session(self, gcmd):
        return self.probe_session.start_probe_session(gcmd)

    def get_status(self, eventtime):
        status = self.cmd_helper.get_status(eventtime)
        status.update(self._load_cell.get_status(eventtime))
        status.update(self.mcu_probe.get_status(eventtime))
        return status


def load_config(config):
    # Sensor types supported by load_cell_probe
    sensors = {}
    sensors.update(hx71x.HX71X_SENSOR_TYPES)
    sensors.update(ads1220.ADS1220_SENSOR_TYPE)
    sensor_class = config.getchoice('sensor_type', sensors)
    sensor = sensor_class(config)
    lc = load_cell.LoadCell(config, sensor)
    printer = config.get_printer()
    name = config.get_name().split()[-1]
    lc_name = 'load_cell' if name == "load_cell_probe" else 'load_cell ' + name
    printer.add_object(lc_name, lc)
    lce = LoadCellEndstop(config, lc, ProbeSessionContext(config, lc))
    lc_probe = LoadCellPrinterProbe(config, lc, lce)
    printer.add_object('probe', lc_probe)
    return lc_probe
