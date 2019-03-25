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
        self.captured_position = None
        self.captured_speed = 0.
        self.captured_epos = None
        self.toolhead = self.v_sd = None
        self.pause_command_sent = False
        self.sd_paused = False
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        self.gcode.register_command("PAUSE", self.cmd_PAUSE)
        self.gcode.register_command("RESUME", self.cmd_RESUME)
    def handle_ready(self):
        self.toolhead = self.printer.lookup_object('toolhead')
        self.v_sd = self.printer.lookup_object('virtual_sdcard', None)
    def get_status(self, eventtime):
        paused = self.captured_position is not None
        return {
            'is_paused': paused
        }
    def send_pause_command(self):
        # This sends the appropriate pause command from an event.
        if not self.pause_command_sent:
            if self.v_sd is not None and self.v_sd.is_active():
                # Printing from virtual sd, run pause command
                self.sd_paused = True
                self.v_sd.do_pause()
            else:
                self.sd_paused = False
                self.gcode.respond_info("action:pause")
            self.pause_command_sent = True
    def cmd_PAUSE(self, params):
        if self.captured_position is not None:
            self.gcode.respond_info("Print already paused")
            return
        self.send_pause_command()
        self.toolhead.wait_moves()
        self.captured_position = self.toolhead.get_position()
        reactor = self.printer.get_reactor()
        gcs = self.gcode.get_status(reactor.monotonic())
        self.captured_speed = gcs['speed']
        if gcs['abs_extrude']:
            self.captured_epos = gcs['last_epos'] - gcs['base_epos']
    def cmd_RESUME(self, params):
        if self.captured_position is None:
            self.gcode.respond_info("Print is not paused, resume aborted")
            return
        velocity = self.gcode.get_float(
            'VELOCITY', params, self.recover_velocity)

        cur_pos = self.toolhead.get_position()
        self.captured_position[3] = cur_pos[3]
        self.toolhead.move(self.captured_position, velocity)
        self.toolhead.get_last_move_time()
        self.gcode.reset_last_position()
        # restore previous speed
        self.gcode.run_script_from_command(
            "G1 F%.6f" % (self.captured_speed))
        if self.captured_epos is not None:
            self.gcode.run_script_from_command(
                "G92 E%.6f" % (self.captured_epos))
        self.captured_position = self.captured_epos = None
        self.pause_command_sent = False
        if self.sd_paused:
            # Printing from virtual sd, run pause command
            self.v_sd.cmd_M24({})
        else:
            self.gcode.respond_info("action:resume")

def load_config(config):
    return PauseResume(config)
