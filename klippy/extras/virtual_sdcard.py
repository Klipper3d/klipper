# Virtual sdcard support (print files directly from a host g-code file)
#
# Copyright (C) 2018-2024  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os, sys, logging, io, re

VALID_GCODE_EXTS = ['gcode', 'g', 'gco']

DEFAULT_ERROR_GCODE = """
{% if 'heaters' in printer %}
   TURN_OFF_HEATERS
{% endif %}
"""

class VirtualSD:
    def __init__(self, config):
        self.printer = config.get_printer()
        # sdcard state
        sd = config.get('path')
        self.sdcard_dirname = os.path.normpath(os.path.expanduser(sd))
        self.current_file = None
        self.file_position = self.file_size = 0
        # Print Stat Tracking
        self.print_stats = self.printer.load_object(config, 'print_stats')
        # Work timer
        self.reactor = self.printer.get_reactor()
        self.must_pause_work = self.cmd_from_sd = False
        self.next_file_position = 0
        self.work_timer = None
        # Error handling
        gcode_macro = self.printer.load_object(config, 'gcode_macro')
        aio = self.printer.load_object(config, 'aio_executor')
        self.executor = aio.allocate_executor("virtual_sdcard")
        self.on_error_gcode = gcode_macro.load_template(
            config, 'on_error_gcode', DEFAULT_ERROR_GCODE)
        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        for cmd in ['M20', 'M21', 'M23', 'M24', 'M25', 'M26', 'M27']:
            self.gcode.register_command(cmd, getattr(self, 'cmd_' + cmd))
        for cmd in ['M28', 'M29', 'M30']:
            self.gcode.register_command(cmd, self.cmd_error)
        self.gcode.register_command(
            "SDCARD_RESET_FILE", self.cmd_SDCARD_RESET_FILE,
            desc=self.cmd_SDCARD_RESET_FILE_help)
        self.gcode.register_command(
            "SDCARD_PRINT_FILE", self.cmd_SDCARD_PRINT_FILE,
            desc=self.cmd_SDCARD_PRINT_FILE_help)
        self.printer.register_event_handler("klippy:analyze_shutdown",
                                            self._handle_analyze_shutdown)
    def _handle_analyze_shutdown(self, msg, details):
        if self.work_timer is not None:
            self.must_pause_work = True
            try:
                readpos = max(self.file_position - 1024, 0)
                readcount = self.file_position - readpos
                self.current_file.seek(readpos)
                data = self.current_file.read(readcount + 128)
            except:
                logging.exception("virtual_sdcard shutdown read")
                return
            logging.info("Virtual sdcard (%d): %s\nUpcoming (%d): %s",
                         readpos, repr(data[:readcount]),
                         self.file_position, repr(data[readcount:]))
    def stats(self, eventtime):
        if self.work_timer is None:
            return False, ""
        return True, "sd_pos=%d" % (self.file_position,)
    def get_file_list(self, check_subdirs=False):
        if check_subdirs:
            flist = []
            for root, dirs, files in os.walk(
                    self.sdcard_dirname, followlinks=True):
                for name in files:
                    ext = name[name.rfind('.')+1:]
                    if ext not in VALID_GCODE_EXTS:
                        continue
                    full_path = os.path.join(root, name)
                    r_path = full_path[len(self.sdcard_dirname) + 1:]
                    size = os.path.getsize(full_path)
                    flist.append((r_path, size))
            return sorted(flist, key=lambda f: f[0].lower())
        else:
            dname = self.sdcard_dirname
            try:
                filenames = os.listdir(self.sdcard_dirname)
                return [(fname, os.path.getsize(os.path.join(dname, fname)))
                        for fname in sorted(filenames, key=str.lower)
                        if not fname.startswith('.')
                        and os.path.isfile((os.path.join(dname, fname)))]
            except:
                logging.exception("virtual_sdcard get_file_list")
                raise self.gcode.error("Unable to get file list")
    def get_status(self, eventtime):
        return {
            'file_path': self.file_path(),
            'progress': self.progress(),
            'is_active': self.is_active(),
            'file_position': self.file_position,
            'file_size': self.file_size,
        }
    def file_path(self):
        if self.current_file:
            return self.current_file.name
        return None
    def progress(self):
        if self.file_size:
            return float(self.file_position) / self.file_size
        else:
            return 0.
    def is_active(self):
        return self.work_timer is not None
    def do_pause(self):
        if self.work_timer is not None:
            self.must_pause_work = True
            # Wait until the work handler fully exits (work_timer cleared).
            # Previously we returned immediately when cmd_from_sd was True,
            # assuming the work handler would still observe must_pause_work
            # and exit shortly.  That created a window where PAUSE set
            # is_paused=True but the work handler was still running inside
            # _wait_extruder_ready, blocking temperature waits, or the
            # "retry unconsumed line" path - and a subsequent RESUME could
            # race the work handler's teardown and hit "SD busy" or an
            # inconsistent state.  Unconditionally wait for work_timer to
            # become None so PAUSE always quiesces the SD print before
            # reporting success.
            while self.work_timer is not None:
                self.reactor.pause(self.reactor.monotonic() + .001)
        else:
            # Defensive: if the work handler has already exited (e.g. due to
            # an interrupt from a blocking temperature wait), make sure the
            # pause flag is cleared so a subsequent resume sees a clean state.
            self.must_pause_work = False
    def do_resume(self):
        if self.work_timer is not None:
            raise self.gcode.error("SD busy")
        if self.current_file is None:
            raise self.gcode.error("No SD file loaded")
        self.must_pause_work = False
        self.work_timer = self.reactor.register_timer(
            self.work_handler, self.reactor.NOW)
    def do_cancel(self):
        if self.current_file is not None:
            self.do_pause()
            self.current_file.close()
            self.current_file = None
            self.print_stats.note_cancel()
        self.file_position = self.file_size = 0
    # G-Code commands
    def cmd_error(self, gcmd):
        raise gcmd.error("SD write not supported")
    def _reset_file(self):
        if self.current_file is not None:
            self.do_pause()
            self.current_file.close()
            self.current_file = None
        self.file_position = self.file_size = 0
        self.print_stats.reset()
        self.printer.send_event("virtual_sdcard:reset_file")
    cmd_SDCARD_RESET_FILE_help = "Clears a loaded SD File. Stops the print "\
        "if necessary"
    def cmd_SDCARD_RESET_FILE(self, gcmd):
        if self.cmd_from_sd:
            raise gcmd.error(
                "SDCARD_RESET_FILE cannot be run from the sdcard")
        self._reset_file()
    cmd_SDCARD_PRINT_FILE_help = "Loads a SD file and starts the print.  May "\
        "include files in subdirectories."
    def cmd_SDCARD_PRINT_FILE(self, gcmd):
        if self.work_timer is not None:
            raise gcmd.error("SD busy")
        self._reset_file()
        filename = gcmd.get("FILENAME")
        if filename[0] == '/':
            filename = filename[1:]
        self._load_file(gcmd, filename, check_subdirs=True)
        self.do_resume()
    def cmd_M20(self, gcmd):
        # List SD card
        files = self.get_file_list()
        gcmd.respond_raw("Begin file list")
        for fname, fsize in files:
            gcmd.respond_raw("%s %d" % (fname, fsize))
        gcmd.respond_raw("End file list")
    def cmd_M21(self, gcmd):
        # Initialize SD card
        gcmd.respond_raw("SD card ok")
    def cmd_M23(self, gcmd):
        # Select SD file
        if self.work_timer is not None:
            raise gcmd.error("SD busy")
        self._reset_file()
        filename = gcmd.get_raw_command_parameters().strip()
        if filename.startswith('/'):
            filename = filename[1:]
        self._load_file(gcmd, filename)
    def _load_file(self, gcmd, filename, check_subdirs=False):
        files = self.get_file_list(check_subdirs)
        flist = [f[0] for f in files]
        files_by_lower = { fname.lower(): fname for fname, fsize in files }
        fname = filename
        try:
            if fname not in flist:
                fname = files_by_lower[fname.lower()]
            fname = os.path.join(self.sdcard_dirname, fname)
            f = self.executor.submit(io.open, fname, 'rb', buffering=0)
            f = self.executor.wrap_obj(f)
            f = io.BufferedReader(f)
            f = io.TextIOWrapper(f, newline='')
            f.seek(0, os.SEEK_END)
            fsize = f.tell()
            f.seek(0)
        except:
            logging.exception("virtual_sdcard file open")
            raise gcmd.error("Unable to open file")
        gcmd.respond_raw("File opened:%s Size:%d" % (filename, fsize))
        gcmd.respond_raw("File selected")
        self.current_file = f
        self.file_position = 0
        self.file_size = fsize
        self.print_stats.set_current_file(filename)

        # Preheat: scan gcode header for M104/M109 and preset extruder
        # temperature. This gives the heater advance time so that when
        # work_handler starts reading gcode lines, the extruder is close
        # to target temp already. Without this, M104 in the gcode file is
        # only processed inside work_handler — too late for the first E
        # move which triggers "Extrude below minimum temp".
        try:
            toolhead = self.printer.lookup_object('toolhead')
            extruder = toolhead.get_extruder()
            if extruder is not None:
                heater = extruder.get_heater()
                if heater is not None:
                    st = heater.get_status(self.reactor.monotonic())
                    if st.get('target', 0) <= 0:
                        for _ in range(100):
                            line = f.readline()
                            if not line:
                                break
                            m = re.match(
                                r'^\s*[Mm]10[49]\s+.*?[Ss](\d+\.?\d*)', line)
                            if m:
                                t = float(m.group(1))
                                if 0 < t < 300:
                                    heater.set_temp(t)
                                    logging.info(
                                        "SD print preheat: preset "
                                        "extruder %.1fC from gcode", t)
                                    break
            f.seek(0)
        except Exception:
            logging.exception("virtual_sdcard preheat")
            try:
                f.seek(0)
            except Exception:
                pass

        self.printer.send_event("virtual_sdcard:load_file")
    def cmd_M24(self, gcmd):
        # Start/resume SD print
        self.do_resume()
    def cmd_M25(self, gcmd):
        # Pause SD print
        self.do_pause()
    def cmd_M26(self, gcmd):
        # Set SD position
        if self.work_timer is not None:
            raise gcmd.error("SD busy")
        pos = gcmd.get_int('S', minval=0)
        self.file_position = pos
    def cmd_M27(self, gcmd):
        # Report SD print status
        if self.current_file is None:
            gcmd.respond_raw("Not SD printing.")
            return
        gcmd.respond_raw("SD printing byte %d/%d"
                         % (self.file_position, self.file_size))
    def get_file_position(self):
        return self.next_file_position
    def set_file_position(self, pos):
        self.next_file_position = pos
    def is_cmd_from_sd(self):
        return self.cmd_from_sd
    def _wait_extruder_ready(self):
        # Wait (without holding the gcode mutex) until the extruder becomes
        # extrude-capable.  Returns False when the extruder is not heating
        # (genuine cold extrude - abort), when a host pause/cancel request
        # arrives (must_pause_work), when the heater is turned off, or when
        # the wait exceeds 10 minutes.  The failing line stays unconsumed in
        # every False path, so a pause/resume cycle naturally retries it.
        reactor = self.reactor
        toolhead = self.printer.lookup_object('toolhead')
        extruder = toolhead.get_extruder()
        if extruder is None:
            return False
        heater = extruder.get_heater()
        if heater is None:
            return False
        st = heater.get_status(reactor.monotonic())
        if st.get('target', 0) <= 0:
            return False
        logging.info(
            "SD print: extruder below min_extrude_temp (%.1fC), waiting to "
            "reach target %.1fC before retrying",
            heater.min_extrude_temp, st['target'])
        deadline = reactor.monotonic() + 600.
        while not self.must_pause_work:
            st = heater.get_status(reactor.monotonic())
            if heater.can_extrude:
                return True
            if st.get('target', 0) <= 0:
                logging.error(
                    "SD print: extruder heater turned off during "
                    "cold-extrude wait, aborting")
                return False
            if reactor.monotonic() >= deadline:
                logging.error(
                    "SD print: extruder still below min_extrude_temp after "
                    "600s, aborting cold-extrude wait")
                return False
            reactor.pause(reactor.monotonic() + 0.5)
        return False
    # Background work timer
    def work_handler(self, eventtime):
        logging.info("Starting SD card print (position %d)", self.file_position)
        self.reactor.unregister_timer(self.work_timer)
        # Keep self.work_timer as-is (stale, unregistered) for the whole print.
        # is_active() is defined as "work_timer is not None", so clearing it
        # here would make is_active() return False during the print - which
        # breaks pause_resume: _request_sd_interrupt() would silently do
        # nothing (must_pause_work never set, so a blocking temperature wait
        # is never interrupted) and send_pause_command() would emit a fake
        # "action:paused" without stopping the SD print.
        # Guard: if current_file was cleared while we were waiting for the
        # gcode mutex (e.g. by a _reset_file or do_cancel in another macro),
        # bail out cleanly instead of crashing on seek.
        if self.current_file is None:
            logging.info("SD card print aborted: current_file is None")
            self.work_timer = None
            self.must_pause_work = False
            return self.reactor.NEVER
        try:
            self.current_file.seek(self.file_position)
        except:
            logging.exception("virtual_sdcard seek")
            self.work_timer = None
            return self.reactor.NEVER
        self.print_stats.note_start()

        # Preheating is handled in _load_file (see that function).
        # Do NOT add reactor-blocking wait here — it freezes the reactor
        # and breaks HTTP/Moonraker communication.

        gcode_mutex = self.gcode.get_mutex()
        partial_input = ""
        lines = []
        error_message = None
        while not self.must_pause_work:
            if not lines:
                # Read more data
                try:
                    data = self.current_file.read(8192)
                except:
                    logging.exception("virtual_sdcard read")
                    break
                if not data:
                    # End of file
                    self.current_file.close()
                    self.current_file = None
                    logging.info("Finished SD card print")
                    self.gcode.respond_raw("Done printing file")
                    break
                lines = data.split('\n')
                lines[0] = partial_input + lines[0]
                partial_input = lines.pop()
                lines.reverse()
                self.reactor.pause(self.reactor.NOW)
                continue
            # Pause if any other request is pending in the gcode class
            if gcode_mutex.test():
                self.reactor.pause(self.reactor.monotonic() + 0.050)
                continue
            # Yield to reactor so host pause/cancel webhook requests get a
            # chance to run and set must_pause_work BEFORE we take the mutex
            # and potentially block on a long-running command (G28,
            # START_PRINT macro, TEMPERATURE_WAIT, etc.).  Without this, a
            # PAUSE/CANCEL fired during any blocking command that is NOT a
            # heater wait would be stuck behind the gcode mutex until that
            # command finishes (see heaters._check_temp_wait_interrupt for
            # the heater-specific path that also needs this generic fallback).
            self.reactor.pause(self.reactor.NOW)
            if self.must_pause_work:
                continue
            # Dispatch command
            self.cmd_from_sd = True
            line = lines.pop()
            if sys.version_info.major >= 3:
                next_file_position = self.file_position + len(line.encode()) + 1
            else:
                next_file_position = self.file_position + len(line) + 1
            self.next_file_position = next_file_position
            try:
                self.gcode.run_script(line)
            except self.gcode.error as e:
                if self.must_pause_work:
                    # A host pause/cancel request interrupted this command
                    # (e.g. a blocking temperature wait).  Leave the line
                    # unconsumed so it is retried on resume; the queued
                    # PAUSE/CANCEL_PRINT request now runs.
                    logging.info("SD card print interrupted by host request")
                    break
                # Cold-extrude guard: cloud-sliced gcode (FlashForge) issues
                # E moves right after an async M104 with no M109 wait, so the
                # first extrusion can fire while the nozzle is still heating
                # (bed-only M190 passed instantly, or heaters had been turned
                # off by a previous cancel).  If the extruder is actively
                # heating toward a target, wait for it to become extrude-
                # capable and retry the line instead of aborting the print
                # through on_error_gcode (device config: CANCEL_PRINT).
                if (str(e).startswith("Extrude below minimum temp")
                        and self._wait_extruder_ready()):
                    lines.append(line)  # retry the unconsumed line
                    self.cmd_from_sd = False
                    continue
                if self.must_pause_work:
                    # The cold-extrude wait above was interrupted by a host
                    # pause/cancel request - same handling as the interrupt
                    # path: the queued PAUSE/CANCEL_PRINT request now runs.
                    logging.info("SD card print interrupted by host request")
                    break
                error_message = str(e)
                try:
                    self.gcode.run_script(self.on_error_gcode.render())
                except:
                    logging.exception("virtual_sdcard on_error")
                break
            except:
                logging.exception("virtual_sdcard dispatch")
                break
            self.cmd_from_sd = False
            self.file_position = self.next_file_position
            # Do we need to skip around?
            if self.next_file_position != next_file_position:
                try:
                    self.current_file.seek(self.file_position)
                except:
                    logging.exception("virtual_sdcard seek")
                    self.work_timer = None
                    return self.reactor.NEVER
                lines = []
                partial_input = ""
        logging.info("Exiting SD card print (position %d)", self.file_position)
        self.work_timer = None
        self.cmd_from_sd = False
        if error_message is not None:
            self.print_stats.note_error(error_message)
        elif self.current_file is not None:
            self.print_stats.note_pause()
        else:
            self.print_stats.note_complete()
        return self.reactor.NEVER

def load_config(config):
    return VirtualSD(config)
