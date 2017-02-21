# Parse gcode commands
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os, re, logging, collections
import homing

# Parse out incoming GCode and find and translate head movements
class GCodeParser:
    RETRY_TIME = 0.100
    def __init__(self, printer, fd, is_fileinput=False):
        self.printer = printer
        self.fd = fd
        self.is_fileinput = is_fileinput
        # Input handling
        self.reactor = printer.reactor
        self.is_processing_data = False
        self.fd_handle = None
        if not is_fileinput:
            self.fd_handle = self.reactor.register_fd(self.fd, self.process_data)
        self.input_commands = [""]
        self.bytes_read = 0
        self.input_log = collections.deque([], 50)
        # Command handling
        self.gcode_handlers = {}
        self.is_printer_ready = False
        self.need_ack = False
        self.toolhead = self.heater_nozzle = self.heater_bed = self.fan = None
        self.speed = 25.0
        self.absolutecoord = self.absoluteextrude = True
        self.base_position = [0.0, 0.0, 0.0, 0.0]
        self.last_position = [0.0, 0.0, 0.0, 0.0]
        self.homing_add = [0.0, 0.0, 0.0, 0.0]
        self.axis2pos = {'X': 0, 'Y': 1, 'Z': 2, 'E': 3}
        self.build_handlers()
    def build_config(self):
        self.toolhead = self.printer.objects['toolhead']
        self.heater_nozzle = None
        extruder = self.printer.objects.get('extruder')
        if extruder:
            self.heater_nozzle = extruder.heater
        self.heater_bed = self.printer.objects.get('heater_bed')
        self.fan = self.printer.objects.get('fan')
    def build_handlers(self):
        handlers = ['G1', 'G4', 'G20', 'G21', 'G28', 'G90', 'G91', 'G92',
                    'M18', 'M82', 'M83', 'M105', 'M110', 'M112', 'M114', 'M115',
                    'M206', 'M400',
                    'HELP', 'QUERY_ENDSTOPS', 'RESTART', 'CLEAR_SHUTDOWN',
                    'STATUS']
        if self.heater_nozzle is not None:
            handlers.extend(['M104', 'M109', 'PID_TUNE'])
        if self.heater_bed is not None:
            handlers.extend(['M140', 'M190'])
        if self.fan is not None:
            handlers.extend(['M106', 'M107'])
        if not self.is_printer_ready:
            handlers = [h for h in handlers
                        if getattr(self, 'cmd_'+h+'_when_not_ready', False)]
        self.gcode_handlers = dict((h, getattr(self, 'cmd_'+h))
                                   for h in handlers)
        for h, f in self.gcode_handlers.items():
            aliases = getattr(self, 'cmd_'+h+'_aliases', [])
            self.gcode_handlers.update(dict([(a, f) for a in aliases]))
    def stats(self, eventtime):
        return "gcodein=%d" % (self.bytes_read,)
    def set_printer_ready(self, is_ready):
        if self.is_printer_ready == is_ready:
            return
        self.is_printer_ready = is_ready
        self.build_handlers()
        if is_ready and self.is_fileinput and self.fd_handle is None:
            self.fd_handle = self.reactor.register_fd(self.fd, self.process_data)
    def motor_heater_off(self):
        if self.toolhead is None:
            return
        self.toolhead.motor_off()
        print_time = self.toolhead.get_last_move_time()
        if self.heater_nozzle is not None:
            self.heater_nozzle.set_temp(print_time, 0.)
        if self.heater_bed is not None:
            self.heater_bed.set_temp(print_time, 0.)
        if self.fan is not None:
            self.fan.set_speed(print_time, 0.)
    def dump_debug(self):
        logging.info("Dumping gcode input %d blocks" % (
            len(self.input_log),))
        for eventtime, data in self.input_log:
            logging.info("Read %f: %s" % (eventtime, repr(data)))
    # Parse input into commands
    args_r = re.compile('([a-zA-Z_]+|[a-zA-Z*])')
    def process_commands(self, eventtime):
        while len(self.input_commands) > 1:
            line = self.input_commands.pop(0)
            # Ignore comments and leading/trailing spaces
            line = origline = line.strip()
            cpos = line.find(';')
            if cpos >= 0:
                line = line[:cpos]
            # Break command into parts
            parts = self.args_r.split(line)[1:]
            params = dict((parts[i].upper(), parts[i+1].strip())
                          for i in range(0, len(parts), 2))
            params['#original'] = origline
            if parts and parts[0].upper() == 'N':
                # Skip line number at start of command
                del parts[:2]
            if not parts:
                self.cmd_default(params)
                continue
            params['#command'] = cmd = parts[0].upper() + parts[1].strip()
            # Invoke handler for command
            self.need_ack = True
            handler = self.gcode_handlers.get(cmd, self.cmd_default)
            try:
                handler(params)
            except:
                logging.exception("Exception in command handler")
                self.toolhead.force_shutdown()
                self.respond_error('Internal error on command:"%s"' % (cmd,))
                if self.is_fileinput:
                    self.printer.request_exit_eof()
                    break
            self.ack()
    def process_data(self, eventtime):
        data = os.read(self.fd, 4096)
        self.input_log.append((eventtime, data))
        self.bytes_read += len(data)
        lines = data.split('\n')
        lines[0] = self.input_commands.pop() + lines[0]
        self.input_commands.extend(lines)
        if self.is_processing_data:
            if len(lines) <= 1:
                return
            if not self.is_fileinput and lines[0].strip().upper() == 'M112':
                self.cmd_M112({})
            self.reactor.unregister_fd(self.fd_handle)
            self.fd_handle = None
            return
        self.is_processing_data = True
        self.process_commands(eventtime)
        self.is_processing_data = False
        if self.fd_handle is None:
            self.fd_handle = self.reactor.register_fd(self.fd, self.process_data)
        if not data and self.is_fileinput:
            self.motor_heater_off()
            self.printer.request_exit_eof()
    # Response handling
    def ack(self, msg=None):
        if not self.need_ack or self.is_fileinput:
            return
        if msg:
            os.write(self.fd, "ok %s\n" % (msg,))
        else:
            os.write(self.fd, "ok\n")
        self.need_ack = False
    def respond(self, msg):
        logging.debug(msg)
        if self.is_fileinput:
            return
        os.write(self.fd, msg+"\n")
    def respond_info(self, msg):
        lines = [l.strip() for l in msg.strip().split('\n')]
        self.respond("// " + "\n// ".join(lines))
    def respond_error(self, msg):
        lines = msg.strip().split('\n')
        if len(lines) > 1:
            self.respond_info("\n".join(lines[:-1]))
        self.respond('!! %s' % (lines[-1].strip(),))
    # Temperature wrappers
    def get_temp(self):
        if not self.is_printer_ready:
            return "T:0"
        # T:XXX /YYY B:XXX /YYY
        out = []
        if self.heater_nozzle:
            cur, target = self.heater_nozzle.get_temp()
            out.append("T:%.1f /%.1f" % (cur, target))
        if self.heater_bed:
            cur, target = self.heater_bed.get_temp()
            out.append("B:%.1f /%.1f" % (cur, target))
        return " ".join(out)
    def bg_temp(self, heater):
        if self.is_fileinput:
            return
        eventtime = self.reactor.monotonic()
        while self.is_printer_ready and heater.check_busy(eventtime):
            print_time = self.toolhead.get_last_move_time()
            self.respond(self.get_temp())
            eventtime = self.reactor.pause(eventtime + 1.)
    def set_temp(self, heater, params, wait=False):
        print_time = self.toolhead.get_last_move_time()
        temp = float(params.get('S', '0'))
        try:
            heater.set_temp(print_time, temp)
        except heater.error, e:
            self.respond_error(str(e))
            return
        if wait:
            self.bg_temp(heater)
    # Individual command handlers
    def cmd_default(self, params):
        if not self.is_printer_ready:
            self.respond_error(self.printer.get_state_message())
            return
        cmd = params.get('#command')
        if not cmd:
            logging.debug(params['#original'])
            return
        self.respond('echo:Unknown command:"%s"' % (cmd,))
    cmd_G1_aliases = ['G0']
    def cmd_G1(self, params):
        # Move
        for a, p in self.axis2pos.items():
            if a in params:
                v = float(params[a])
                if not self.absolutecoord or (p>2 and not self.absoluteextrude):
                    # value relative to position of last move
                    self.last_position[p] += v
                else:
                    # value relative to base coordinate position
                    self.last_position[p] = v + self.base_position[p]
        if 'F' in params:
            self.speed = float(params['F']) / 60.
        try:
            self.toolhead.move(self.last_position, self.speed)
        except homing.EndstopError, e:
            self.respond_error(str(e))
            self.last_position = self.toolhead.get_position()
    def cmd_G4(self, params):
        # Dwell
        if 'S' in params:
            delay = float(params['S'])
        else:
            delay = float(params.get('P', '0')) / 1000.
        self.toolhead.dwell(delay)
    def cmd_G20(self, params):
        # Set units to inches
        self.respond_error('Machine does not support G20 (inches) command')
    def cmd_G21(self, params):
        # Set units to millimeters
        pass
    def cmd_G28(self, params):
        # Move to origin
        axes = []
        for axis in 'XYZ':
            if axis in params:
                axes.append(self.axis2pos[axis])
        if not axes:
            axes = [0, 1, 2]
        homing_state = homing.Homing(self.toolhead, axes)
        if self.is_fileinput:
            homing_state.set_no_verify_retract()
        try:
            self.toolhead.home(homing_state)
        except homing.EndstopError, e:
            self.toolhead.motor_off()
            self.respond_error(str(e))
            return
        newpos = self.toolhead.get_position()
        for axis in homing_state.get_axes():
            self.last_position[axis] = newpos[axis]
            self.base_position[axis] = -self.homing_add[axis]
    def cmd_G90(self, params):
        # Use absolute coordinates
        self.absolutecoord = True
    def cmd_G91(self, params):
        # Use relative coordinates
        self.absolutecoord = False
    def cmd_G92(self, params):
        # Set position
        mcount = 0
        for a, p in self.axis2pos.items():
            if a in params:
                self.base_position[p] = self.last_position[p] - float(params[a])
                mcount += 1
        if not mcount:
            self.base_position = list(self.last_position)
    def cmd_M82(self, params):
        # Use absolute distances for extrusion
        self.absoluteextrude = True
    def cmd_M83(self, params):
        # Use relative distances for extrusion
        self.absoluteextrude = False
    cmd_M18_aliases = ["M84"]
    def cmd_M18(self, params):
        # Turn off motors
        self.toolhead.motor_off()
    cmd_M105_when_not_ready = True
    def cmd_M105(self, params):
        # Get Extruder Temperature
        self.ack(self.get_temp())
    def cmd_M104(self, params):
        # Set Extruder Temperature
        self.set_temp(self.heater_nozzle, params)
    def cmd_M109(self, params):
        # Set Extruder Temperature and Wait
        self.set_temp(self.heater_nozzle, params, wait=True)
    cmd_M110_when_not_ready = True
    def cmd_M110(self, params):
        # Set Current Line Number
        pass
    def cmd_M112(self, params):
        # Emergency Stop
        self.toolhead.force_shutdown()
    cmd_M114_when_not_ready = True
    def cmd_M114(self, params):
        # Get Current Position
        if self.toolhead is None:
            self.cmd_default(params)
            return
        kinpos = self.toolhead.get_position()
        self.respond("X:%.3f Y:%.3f Z:%.3f E:%.3f Count X:%.3f Y:%.3f Z:%.3f" % (
            self.last_position[0], self.last_position[1],
            self.last_position[2], self.last_position[3],
            kinpos[0], kinpos[1], kinpos[2]))
    cmd_M115_when_not_ready = True
    def cmd_M115(self, params):
        # Get Firmware Version and Capabilities
        kw = {"FIRMWARE_NAME": "Klipper"
              , "FIRMWARE_VERSION": self.printer.software_version}
        self.ack(" ".join(["%s:%s" % (k, v) for k, v in kw.items()]))
    def cmd_M140(self, params):
        # Set Bed Temperature
        self.set_temp(self.heater_bed, params)
    def cmd_M190(self, params):
        # Set Bed Temperature and Wait
        self.set_temp(self.heater_bed, params, wait=True)
    def cmd_M106(self, params):
        # Set fan speed
        print_time = self.toolhead.get_last_move_time()
        self.fan.set_speed(print_time, float(params.get('S', '255')) / 255.)
    def cmd_M107(self, params):
        # Turn fan off
        print_time = self.toolhead.get_last_move_time()
        self.fan.set_speed(print_time, 0.)
    def cmd_M206(self, params):
        # Set home offset
        for a, p in self.axis2pos.items():
            if a in params:
                v = float(params[a])
                self.base_position[p] += self.homing_add[p] - v
                self.homing_add[p] = v
    def cmd_M400(self, params):
        # Wait for current moves to finish
        self.toolhead.wait_moves()
    cmd_QUERY_ENDSTOPS_help = "Report on the status of each endstop"
    cmd_QUERY_ENDSTOPS_aliases = ["M119"]
    def cmd_QUERY_ENDSTOPS(self, params):
        # Get Endstop Status
        if self.is_fileinput:
            return
        try:
            res = self.toolhead.query_endstops()
        except self.printer.mcu.error, e:
            self.respond_error(str(e))
            return
        self.respond(" ".join(["%s:%s" % (name, ["open", "TRIGGERED"][not not t])
                               for name, t in res]))
    cmd_PID_TUNE_help = "Run PID Tuning"
    cmd_PID_TUNE_aliases = ["M303"]
    def cmd_PID_TUNE(self, params):
        # Run PID tuning
        heater = int(params.get('E', '0'))
        heater = {0: self.heater_nozzle, -1: self.heater_bed}[heater]
        temp = float(params.get('S', '60'))
        heater.start_auto_tune(temp)
        self.bg_temp(heater)
    cmd_CLEAR_SHUTDOWN_when_not_ready = True
    cmd_CLEAR_SHUTDOWN_help = "Clear a firmware shutdown and restart"
    def cmd_CLEAR_SHUTDOWN(self, params):
        if self.toolhead is None:
            self.cmd_default(params)
            return
        self.printer.mcu.clear_shutdown()
        self.printer.request_restart()
    cmd_RESTART_when_not_ready = True
    cmd_RESTART_help = "Reload config file and restart host software"
    def cmd_RESTART(self, params):
        if self.is_printer_ready:
            self.respond_info("Preparing to restart...")
            self.motor_heater_off()
            self.toolhead.dwell(0.500)
            self.toolhead.wait_moves()
        self.printer.request_restart()
    cmd_STATUS_when_not_ready = True
    cmd_STATUS_help = "Report the printer status"
    def cmd_STATUS(self, params):
        msg = self.printer.get_state_message()
        if self.is_printer_ready:
            self.respond_info(msg)
        else:
            self.respond_error(msg)
    cmd_HELP_when_not_ready = True
    def cmd_HELP(self, params):
        cmdhelp = []
        if not self.is_printer_ready:
            cmdhelp.append("Printer is not ready - not all commands available.")
        cmdhelp.append("Available extended commands:")
        for cmd in self.gcode_handlers:
            desc = getattr(self, 'cmd_'+cmd+'_help', None)
            if desc is not None:
                cmdhelp.append("%-10s: %s" % (cmd, desc))
        self.respond_info("\n".join(cmdhelp))
