# Virtual sdcard support (print files directly from a host g-code file)
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os, logging, io

VALID_GCODE_EXTS = ['gcode', 'g', 'gco']

class VirtualSDGCodeProvider:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        # sdcard state
        sd = config.get('path')
        self.sdcard_dirname = os.path.normpath(os.path.expanduser(sd))
        self.filename = ''
        self.current_file = None
        self.file_position = self.file_size = 0
        self.next_file_position = 0
        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        for cmd in ['M20', 'M21', 'M26', 'M27']:
            self.gcode.register_command(cmd, getattr(self, 'cmd_' + cmd))
        for cmd in ['M28', 'M29', 'M30']:
            self.gcode.register_command(cmd, self.cmd_error)
    # Generic methods of GCode provider
    def handle_shutdown(self):
        if self.current_file is not None:
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
    def get_stats(self, eventtime):
        return True, "sd_pos=%d" % (self.file_position,)
    def get_status(self, eventtime):
        return {
            'file_path': self.file_path(),
            'progress': self.progress(),
            'file_position': self.file_position,
            'file_size': self.file_size,
        }
    def is_active(self):
        return self.current_file is not None
    def get_name(self):
        return self.filename
    def reset(self):
        if self.current_file is not None:
            self.current_file.close()
            self.current_file = None
            self.filename = ''
        self.file_position = self.file_size = 0
    def get_gcode(self):
        logging.info("Starting SD card print (position %d)", self.file_position)
        try:
            self.current_file.seek(self.file_position)
        except:
            logging.exception("virtual_sdcard seek")
            return
        partial_input = ""
        lines = []
        while True:
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
            line = lines.pop()
            next_file_position = self.file_position + len(line) + 1
            self.next_file_position = next_file_position
            yield line
            self.file_position = self.next_file_position
            # Do we need to skip around?
            if self.next_file_position != next_file_position:
                try:
                    self.current_file.seek(self.file_position)
                except:
                    logging.exception("virtual_sdcard seek")
                    return
                lines = []
                partial_input = ""
        logging.info("Exiting SD card print (position %d)", self.file_position)
    # Virtual SD Card file management
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
    def file_path(self):
        if self.current_file:
            return self.current_file.name
        return None
    def progress(self):
        if self.file_size:
            return float(self.file_position) / self.file_size
        else:
            return 0.
    def load_file(self, gcmd, filename, check_subdirs=False):
        files = self.get_file_list(check_subdirs)
        flist = [f[0] for f in files]
        files_by_lower = { fname.lower(): fname for fname, fsize in files }
        fname = filename
        try:
            if fname not in flist:
                fname = files_by_lower[fname.lower()]
            fname = os.path.join(self.sdcard_dirname, fname)
            f = io.open(fname, 'r', newline='')
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
        self.filename = filename
    def get_file_position(self):
        return self.next_file_position
    def set_file_position(self, pos):
        self.next_file_position = pos
    # G-Code commands
    def cmd_error(self, gcmd):
        raise gcmd.error("SD write not supported")
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
    def cmd_M26(self, gcmd):
        # Set SD position
        if not self.is_active():
            gcmd.respond_raw("Not printing from SD card.")
        pos = gcmd.get_int('S', minval=0)
        self.set_file_position(pos)
    def cmd_M27(self, gcmd):
        # Report SD print status
        if not self.is_active():
            gcmd.respond_raw("Not printing from SD card.")
            return
        gcmd.respond_raw("SD printing byte %d/%d"
                         % (self.file_position, self.file_size))

class VirtualSD:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:shutdown",
                                            self.handle_shutdown)
        # Print Stat Tracking
        self.print_stats = self.printer.load_object(config, 'print_stats')
        # sdcard state
        self.virtualsd_gcode_provider = VirtualSDGCodeProvider(config)
        self.gcode_provider = None
        # Work timer
        self.reactor = self.printer.get_reactor()
        self.must_pause_work = self.cmd_from_sd = False
        self.work_timer = None
        # Error handling
        gcode_macro = self.printer.load_object(config, 'gcode_macro')
        self.on_error_gcode = gcode_macro.load_template(
            config, 'on_error_gcode', '')
        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        for cmd in ['M23', 'M24', 'M25']:
            self.gcode.register_command(cmd, getattr(self, 'cmd_' + cmd))
        self.gcode.register_command(
            "SDCARD_RESET_FILE", self.cmd_SDCARD_RESET_FILE,
            desc=self.cmd_SDCARD_RESET_FILE_help)
        self.gcode.register_command(
            "SDCARD_PRINT_FILE", self.cmd_SDCARD_PRINT_FILE,
            desc=self.cmd_SDCARD_PRINT_FILE_help)
    def handle_shutdown(self):
        if self.work_timer is not None:
            self.must_pause_work = True
            self.gcode_provider.handle_shutdown()
    def get_file_list(self, check_subdirs=False):
        return self.virtualsd_gcode_provider.get_file_list(check_subdirs)
    def stats(self, eventtime):
        if self.work_timer is None:
            return False, ""
        return self.gcode_provider.get_stats(eventtime)
    def get_status(self, eventtime):
        sts = {'is_active': self.is_active()}
        if self.gcode_provider:
            sts.update(self.gcode_provider.get_status(eventtime))
        else:
            sts.update(self.virtualsd_gcode_provider.get_status(eventtime))
        return sts
    def is_active(self):
        return self.work_timer is not None
    def do_pause(self):
        if self.work_timer is not None:
            self.must_pause_work = True
            while self.work_timer is not None and not self.cmd_from_sd:
                self.reactor.pause(self.reactor.monotonic() + .001)
    def do_resume(self):
        if self.work_timer is not None:
            raise self.gcode.error("SD busy")
        self.must_pause_work = False
        self.work_timer = self.reactor.register_timer(
            self.work_handler, self.reactor.NOW)
    def do_cancel(self):
        if self.gcode_provider is not None:
            self.do_pause()
            self.gcode_provider.reset()
            self.print_stats.note_cancel()
            self.gcode_provider = None
    def _set_gcode_provider(self, gcode_provider):
        if self.gcode_provider is not None:
            raise self.gcode.error(
                    'Print is active when resetting GCode provider')
        self.gcode_provider = gcode_provider
        self.print_stats.set_current_file(gcode_provider.get_name())
        self.gcode_lines = gcode_provider.get_gcode()
        self.current_line = ''
    def print_with_gcode_provider(self, gcode_provider):
        self._reset_print()
        self._set_gcode_provider(gcode_provider)
        self.do_resume()
    # G-Code commands
    def _reset_print(self):
        if self.gcode_provider is not None:
            self.do_pause()
            self.gcode_provider.reset()
            self.gcode_provider = None
        self.print_stats.reset()
        self.printer.send_event("virtual_sdcard:reset_file")
    cmd_SDCARD_RESET_FILE_help = "Clears a loaded SD File. Stops the print "\
        "if necessary"
    def cmd_SDCARD_RESET_FILE(self, gcmd):
        if self.cmd_from_sd:
            raise gcmd.error(
                "SDCARD_RESET_FILE cannot be run from the sdcard")
        self._reset_print()
    cmd_SDCARD_PRINT_FILE_help = "Loads a SD file and starts the print.  May "\
        "include files in subdirectories."
    def cmd_SDCARD_PRINT_FILE(self, gcmd):
        if self.work_timer is not None:
            raise gcmd.error("SD busy")
        self._reset_print()
        filename = gcmd.get("FILENAME")
        if filename[0] == '/':
            filename = filename[1:]
        self.virtualsd_gcode_provider.load_file(gcmd, filename,
                                                check_subdirs=True)
        self._set_gcode_provider(self.virtualsd_gcode_provider)
        self.do_resume()
    def cmd_M23(self, gcmd):
        # Select SD file
        if self.work_timer is not None:
            raise gcmd.error("SD busy")
        self._reset_print()
        filename = gcmd.get_raw_command_parameters().strip()
        if filename.startswith('/'):
            filename = filename[1:]
        self.virtualsd_gcode_provider.load_file(gcmd, filename,
                                                check_subdirs=True)
        self._set_gcode_provider(self.virtualsd_gcode_provider)
    def cmd_M24(self, gcmd):
        # Start/resume SD print
        self.do_resume()
    def cmd_M25(self, gcmd):
        # Pause SD print
        self.do_pause()
    def get_virtual_sdcard_gcode_provider(self):
        return self.virtualsd_gcode_provider
    def get_gcode_provider(self):
        return self.gcode_provider
    def is_cmd_from_sd(self):
        return self.cmd_from_sd
    # Background work timer
    def work_handler(self, eventtime):
        self.reactor.unregister_timer(self.work_timer)
        self.print_stats.note_start()
        gcode_mutex = self.gcode.get_mutex()
        error_message = None
        while not self.must_pause_work:
            if not self.current_line:
                try:
                    self.current_line = next(self.gcode_lines)
                except self.gcode.error as e:
                    error_message = str(e)
                    try:
                        self.gcode.run_script(self.on_error_gcode.render())
                    except:
                        logging.exception("virtual_sdcard on_error")
                    break
                except StopIteration:
                    self.gcode_provider = None
                    break
            # Pause if any other request is pending in the gcode class
            if gcode_mutex.test():
                self.reactor.pause(self.reactor.monotonic() + 0.100)
                continue
            # Dispatch command
            self.cmd_from_sd = True
            line = self.current_line
            try:
                self.gcode.run_script(line)
            except self.gcode.error as e:
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
            self.current_line = ''
        self.work_timer = None
        self.cmd_from_sd = False
        if error_message is not None:
            self.print_stats.note_error(error_message)
            self.gcode.respond_raw(error_message)
            self.gcode_provider = None
        elif self.gcode_provider is not None:
            self.print_stats.note_pause()
        else:
            self.print_stats.note_complete()
        return self.reactor.NEVER

def load_config(config):
    return VirtualSD(config)
