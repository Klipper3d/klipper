# Support for plasma cutter control
#
# Copyright (C) 2020  Lucas Felix <lucas.felix0738@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

MSG_STATUS_BIT = 0
MSG_ERROR_BIT  = 1
STATUS_OFF = 0
STATUS_ON  = 1
ERROR_NONE          = 0
ERROR_NO_TRANSFER   = 1
ERROR_TRANSFER_LOST = 2

class Plasma:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        ppins = self.printer.lookup_object('pins')
        start_pin_params = ppins.lookup_pin(config.get('start_pin'))
        transfer_pin_params = ppins.lookup_pin(config.get('transfer_pin'),
                                               can_invert=True, can_pullup=True)
        self.mcu = start_pin_params['chip']
        self.plasma_oid = self.mcu.create_oid()
        self.toolhead = None

        self.mcu.add_config_cmd("config_plasma oid=%d start_pin=%s"
            " transfer_pin=%s transfer_pullup=%d transfer_invert=%d"
            " transfer_timeout_ms=%u" % (
            self.plasma_oid, start_pin_params['pin'],
            transfer_pin_params['pin'], transfer_pin_params['pullup'],
            transfer_pin_params['invert'],
            config.getint('transfer_timeout_ms')))

        self.cmd_queue = self.mcu.alloc_command_queue()
        self.ack_queue = self.mcu.alloc_command_queue()
        self.mcu.register_config_callback(self.build_config)
        self.plasma_start_cmd = None
        self.plasma_stop_cmd = None
        self.plasma_status_ack_cmd = None

        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode_io = self.printer.lookup_object('gcode_io')

        self.gcode.register_command("M3", self.cmd_M3)
        self.gcode.register_command("M5", self.cmd_M5)
        self.last_M3 = 0

        self.mcu.register_response(self.handle_plasma_status, 'plasma_status')
        self.mcu.register_response(self.handle_clock_drift, 'clock_drift')

        self.error = ERROR_NONE
        self.status = STATUS_OFF
        self.error_displayed = False

    def build_config(self):
        self.toolhead = self.printer.lookup_object('toolhead')
        self.plasma_start_cmd = self.mcu.lookup_command(
            "plasma_start oid=%c clock=%u", cq=self.cmd_queue)
        self.plasma_stop_cmd = self.mcu.lookup_command(
            "plasma_stop oid=%c clock=%u", cq=self.cmd_queue)
        self.plasma_status_ack_cmd = self.mcu.lookup_command(
            "plasma_status_ack oid=%c seq=%c", cq=self.ack_queue)

    def handle_plasma_status(self, params):
        seq = params['seq']
        status = params['status']
        self.status = (status >> MSG_STATUS_BIT) & 0b00000001
        self.error =  (status >> MSG_ERROR_BIT)  & 0b00000011

        if not self.error_displayed:
            if self.error == ERROR_NO_TRANSFER:
                self.gcode._respond_error('Arc transfer timeout')
            elif self.error == ERROR_TRANSFER_LOST:
                self.gcode._respond_error('Arc transfer lost')
            self.error_displayed = True

        self.plasma_status_ack_cmd.send([self.plasma_oid, seq])

    def handle_clock_drift(self, params):
        self.mcu._clocksync.apply_clock_drift(params['clock'])
        self.mcu.force_retransmit()

    def handle_jit_timeout(self):
        self.cmd_M5(gcmd=None)

    def cmd_M3(self, gcmd):
        if self.status == STATUS_ON:
            self.gcode.respond_info('Warning: M3 needs M5 to be re-armed')
            return

        self.gcode_io.enable_jit(self.handle_jit_timeout)
        self.error = ERROR_NONE
        self.status = STATUS_ON
        self.error_displayed = False

        # send start command
        clock = self.mcu.print_time_to_clock(self.toolhead.get_last_move_time())
        self.last_M3 = clock
        self.plasma_start_cmd.send([self.plasma_oid, clock], reqclock=clock)

    def cmd_M5(self, gcmd):
        print_time = self.toolhead.get_last_move_time()
        clock = self.mcu.print_time_to_clock(print_time)
        if self.status == STATUS_ON:
            self.gcode_io.disable_jit()
            self.plasma_stop_cmd.send([self.plasma_oid, clock],
                                      minclock=self.last_M3, reqclock=clock)

        # Even if plasma was off, M5 has to wait in order to be coherent with
        # specs -> M5 is ALWAYS a blocking command
        now = self.reactor.monotonic()
        self.reactor.pause(now + print_time
                           - self.mcu.estimated_print_time(now) + 0.05)

        # Reactor pause may be insufficient due to clock shift, so we ensure
        # we recovered from it through status message from MCU
        while(self.status != STATUS_OFF):
            self.reactor.pause(self.reactor.monotonic() + 0.01)

def load_config(config):
    return Plasma(config)
