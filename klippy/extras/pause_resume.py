# Pause/Resume functionality with position capture/restore
#
# Copyright (C) 2019  Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class PauseResume:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.recover_velocity = config.getfloat('recover_velocity', 50.)
        self.v_sd = None
        self.is_paused = False
        self.sd_paused = False
        self.pause_command_sent = False
        self.sd_interrupt_pending = False
        self._resume_in_progress = False
        self._pause_in_progress = False
        self._cancel_in_progress = False
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.gcode.register_command("PAUSE", self.cmd_PAUSE,
                                    desc=self.cmd_PAUSE_help)
        self.gcode.register_command("RESUME", self.cmd_RESUME,
                                    desc=self.cmd_RESUME_help)
        self.gcode.register_command("CLEAR_PAUSE", self.cmd_CLEAR_PAUSE,
                                    desc=self.cmd_CLEAR_PAUSE_help)
        self.gcode.register_command("CANCEL_PRINT", self.cmd_CANCEL_PRINT,
                                    desc=self.cmd_CANCEL_PRINT_help)
        webhooks = self.printer.lookup_object('webhooks')
        webhooks.register_endpoint("pause_resume/cancel",
                                   self._handle_cancel_request)
        webhooks.register_endpoint("pause_resume/pause",
                                   self._handle_pause_request)
        webhooks.register_endpoint("pause_resume/resume",
                                   self._handle_resume_request)
    def handle_connect(self):
        self.v_sd = self.printer.lookup_object('virtual_sdcard', None)
    def get_status(self, eventtime):
        return {
            'is_paused': self.is_paused,
        }
    def _request_sd_interrupt(self):
        # Set the interrupt flag whenever a virtual_sdcard exists.
        # Previously we gated this behind v_sd.is_active(), but the work
        # handler can exit between the host pause request arriving and us
        # grabbing the gcode lock - leaving must_pause_work cleared and a
        # stale PAUSE state.  Always set the flag; do_pause/do_resume will
        # decide what to do based on the actual work_timer state.
        if self.v_sd is not None:
            self.sd_interrupt_pending = True
            self.v_sd.must_pause_work = True
    def _handle_cancel_request(self, web_request):
        self._request_sd_interrupt()
        self.gcode.run_script("CANCEL_PRINT")
    def _handle_pause_request(self, web_request):
        self._request_sd_interrupt()
        self.gcode.run_script("PAUSE")
    def _handle_resume_request(self, web_request):
        self.gcode.run_script("RESUME")
    def is_sd_active(self):
        return self.v_sd is not None and self.v_sd.is_active()
    def send_pause_command(self):
        if not self.pause_command_sent:
            if self.is_sd_active() or self.sd_interrupt_pending:
                self.sd_interrupt_pending = False
                self.sd_paused = True
                self.v_sd.do_pause()
            else:
                self.sd_paused = False
                self.gcode.respond_info("action:paused")
            self.pause_command_sent = True
    cmd_PAUSE_help = ("Pauses the current print")
    def cmd_PAUSE(self, gcmd):
        if self._pause_in_progress:
            raise self.printer.command_error(
                "Pause already in progress, skipping duplicate request")
        if self.is_paused or self.sd_paused:
            raise self.printer.command_error(
                "Print already paused, skipping duplicate request")
        self._pause_in_progress = True
        try:
            self.send_pause_command()
            self.gcode.run_script_from_command("SAVE_GCODE_STATE NAME=PAUSE_STATE")
            # Overwrite RESUME.last_extruder_temp using live heater target.
            # The PAUSE macro template stores it via SET_GCODE_VARIABLE but
            # Moonraker may race-clear the target before the pause request.
            # We read the authoritative value here and patch the variable
            # dict directly on the gcode_macro object.
            extruder = self.printer.lookup_object('extruder', None)
            if extruder is not None:
                reactor = self.printer.get_reactor()
                targ = extruder.get_status(reactor.monotonic()).get('target', 0)
                if targ and targ > 0:
                    resume_macro = self.printer.lookup_object(
                        'gcode_macro RESUME', None)
                    if resume_macro is not None:
                        resume_macro.variables['last_extruder_temp'] = {
                            'restore': True, 'temp': targ}
            self.is_paused = True
        finally:
            self._pause_in_progress = False
    def send_resume_command(self):
        if self.sd_paused:
            self.v_sd.do_resume()
            self.sd_paused = False
        else:
            self.gcode.respond_info("action:resumed")
        self.pause_command_sent = False
    cmd_RESUME_help = ("Resumes the print from a pause")
    def cmd_RESUME(self, gcmd):
        if self._resume_in_progress:
            raise self.printer.command_error(
                "Resume already in progress, skipping duplicate request")
        # Any of these flags means we are in a paused state.  In theory
        # is_paused is the single source of truth, but races between
        # do_pause and work_handler teardown can leave sd_paused or
        # pause_command_sent set while is_paused is still False (or vice
        # versa).  Treat any of them as "paused" and repair consistency.
        paused = self.is_paused or self.sd_paused or self.pause_command_sent
        if not paused:
            raise self.printer.command_error(
                "Print is not paused, resume aborted")
        # Repair: if is_paused got out of sync, align it with the others
        if not self.is_paused:
            self.is_paused = True
        self._resume_in_progress = True
        try:
            velocity = gcmd.get_float('VELOCITY', self.recover_velocity)
            self.gcode.run_script_from_command(
                "RESTORE_GCODE_STATE NAME=PAUSE_STATE MOVE=1 MOVE_SPEED=%.4f"
                % (velocity))
            self.send_resume_command()
            self.is_paused = False
        finally:
            self._resume_in_progress = False
    cmd_CLEAR_PAUSE_help = (
        "Clears the current paused state without resuming the print")
    def cmd_CLEAR_PAUSE(self, gcmd):
        self.is_paused = self.pause_command_sent = False
    cmd_CANCEL_PRINT_help = ("Cancel the current print")
    def cmd_CANCEL_PRINT(self, gcmd):
        if self._cancel_in_progress:
            raise self.printer.command_error(
                "Cancel already in progress, skipping duplicate request")
        self._cancel_in_progress = True
        try:
            if self.is_sd_active() or self.sd_paused or self.sd_interrupt_pending:
                self.sd_interrupt_pending = False
                self.v_sd.do_cancel()
            else:
                gcmd.respond_info("action:cancel")
            self.cmd_CLEAR_PAUSE(gcmd)
        finally:
            self._cancel_in_progress = False

def load_config(config):
    return PauseResume(config)
