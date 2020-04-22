# Virtual sdcard support (print files directly from a host g-code file)
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os, logging, tempfile, shutil

class VirtualSD:
    def __init__(self, config):
        printer = config.get_printer()
        printer.register_event_handler("klippy:shutdown", self.handle_shutdown)
        # sdcard state
        sd = config.get('path')
        self.pre_spool = config.getboolean('pre_spool', False)
        self.max_spool_size_mb = config.getint('max_spool_size', 64)
        self.sdcard_dirname = os.path.normpath(os.path.expanduser(sd))
        self.current_file = None
        self.current_file_name = None
        self.file_position = self.file_size = 0
        # Work timer
        self.reactor = printer.get_reactor()
        self.must_pause_work = self.cmd_from_sd = False
        self.work_timer = None
        # Register commands
        self.gcode = printer.lookup_object('gcode')
        self.gcode.register_command('M21', None)
        for cmd in ['M20', 'M21', 'M23', 'M24', 'M25', 'M26', 'M27']:
            self.gcode.register_command(cmd, getattr(self, 'cmd_' + cmd))
        for cmd in ['M28', 'M29', 'M30']:
            self.gcode.register_command(cmd, self.cmd_error)
    def handle_shutdown(self):
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
    def get_file_list(self):
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
        progress = 0.
        if self.work_timer is not None and self.file_size:
            progress = float(self.file_position) / self.file_size
        return {'progress': progress}
    def is_active(self):
        return self.work_timer is not None
    def do_pause(self):
        if self.work_timer is not None:
            self.must_pause_work = True
            while self.work_timer is not None and not self.cmd_from_sd:
                self.reactor.pause(self.reactor.monotonic() + .001)
    def select_file(self, fname):
        if self.work_timer is not None:
            raise self.gcode.error("SD busy")
        if self.current_file is not None:
            self.clear_file()
        if self.pre_spool:
            try:
                self.current_file = tempfile.SpooledTemporaryFile(
                    max_size=self.max_spool_size_mb * 1024 * 1024)
                self.current_file_name = fname
                shutil.copyfileobj(open(fname, 'rb'), self.current_file)
                self.read_file_size()
            except:
                self.clear_file()
                logging.exception("virtual_sdcard pre_spool")
                raise self.gcode.error("Unable to pre_spool gcode file")
        else:
            try:
                self.current_file = open(fname, 'rb')
                self.current_file_name = fname
                self.read_file_size()
            except:
                self.clear_file()
                logging.exception("virtual_sdcard file open")
                raise self.gcode.error("Unable to open file")
    def clear_file(self):
        self.current_file.close()
        self.current_file = None
        self.current_file_name = None
        self.file_position = self.file_size = 0
    def read_file_size(self):
        self.current_file.seek(0, os.SEEK_END)
        self.file_size = self.current_file.tell()
        self.current_file.seek(0)
        self.file_position = 0
    # G-Code commands
    def cmd_error(self, params):
        raise self.gcode.error("SD write not supported")
    def cmd_M20(self, params):
        # List SD card
        files = self.get_file_list()
        self.gcode.respond_raw("Begin file list")
        for fname, fsize in files:
            self.gcode.respond_raw("%s %d" % (fname, fsize))
        self.gcode.respond_raw("End file list")
    def cmd_M21(self, params):
        # Initialize SD card
        self.gcode.respond_raw("SD card ok")
    def cmd_M23(self, params):
        # Select SD file
        if self.work_timer is not None:
            raise self.gcode.error("SD busy")
        try:
            orig = params['#original']
            filename = orig[orig.find("M23") + 4:].split()[0].strip()
            if '*' in filename:
                filename = filename[:filename.find('*')].strip()
        except:
            raise self.gcode.error("Unable to extract filename")
        if filename.startswith('/'):
            filename = filename[1:]
        files = self.get_file_list()
        files_by_lower = { fname.lower(): fname for fname, fsize in files }
        try:
            fname = files_by_lower[filename.lower()]
            fname = os.path.join(self.sdcard_dirname, fname)
            self.select_file(fname)
        except:
            logging.exception("virtual_sdcard file open")
            raise self.gcode.error("Unable to select file")
        self.gcode.respond_raw("File opened:%s Size:%d" %
                               (self.current_file_name, self.file_size))
        self.gcode.respond_raw("File selected")
    def cmd_M24(self, params):
        # Start/resume SD print
        if self.work_timer is not None:
            raise self.gcode.error("SD busy")
        self.must_pause_work = False
        self.work_timer = self.reactor.register_timer(
            self.work_handler, self.reactor.NOW)
    def cmd_M25(self, params):
        # Pause SD print
        self.do_pause()
    def cmd_M26(self, params):
        # Set SD position
        if self.work_timer is not None:
            raise self.gcode.error("SD busy")
        pos = self.gcode.get_int('S', params, minval=0)
        self.file_position = pos
    def cmd_M27(self, params):
        # Report SD print status
        if self.current_file is None:
            self.gcode.respond_raw("Not SD printing.")
            return
        self.gcode.respond_raw("SD printing byte %d/%d" % (
            self.file_position, self.file_size))
    # Background work timer
    def work_handler(self, eventtime):
        logging.info("Starting SD card print (position %d)", self.file_position)
        self.reactor.unregister_timer(self.work_timer)
        try:
            self.current_file.seek(self.file_position)
        except:
            logging.exception("virtual_sdcard seek")
            self.work_timer = None
            return self.reactor.NEVER
        gcode_mutex = self.gcode.get_mutex()
        partial_input = ""
        lines = []
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
                self.reactor.pause(self.reactor.monotonic() + 0.100)
                continue
            # Dispatch command
            self.cmd_from_sd = True
            try:
                self.gcode.run_script(lines[-1])
            except self.gcode.error as e:
                break
            except:
                logging.exception("virtual_sdcard dispatch")
                break
            self.cmd_from_sd = False
            self.file_position += len(lines.pop()) + 1
        logging.info("Exiting SD card print (position %d)", self.file_position)
        self.work_timer = None
        self.cmd_from_sd = False
        return self.reactor.NEVER

def load_config(config):
    return VirtualSD(config)
