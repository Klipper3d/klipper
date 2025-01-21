# MCP4018 digipot support (via bit-banging)
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class SoftwareI2C:
    def __init__(self, config, addr):
        self.addr = addr << 1
        self.update_pin_cmd = None
        # Lookup pins
        ppins = config.get_printer().lookup_object('pins')
        scl_pin = config.get('scl_pin')
        scl_params = ppins.lookup_pin(scl_pin, share_type='sw_scl')
        self.mcu = scl_params['chip']
        self.scl_pin = scl_params['pin']
        self.scl_main = scl_params.get('class')
        if self.scl_main is None:
            self.scl_main = scl_params['class'] = self
            self.scl_oid = self.mcu.create_oid()
            self.cmd_queue = self.mcu.alloc_command_queue()
            self.mcu.register_config_callback(self.build_config)
        else:
            self.scl_oid = self.scl_main.scl_oid
            self.cmd_queue = self.scl_main.cmd_queue
        sda_params = ppins.lookup_pin(config.get('sda_pin'))
        self.sda_oid = self.mcu.create_oid()
        if sda_params['chip'] != self.mcu:
            raise ppins.error("%s: scl_pin and sda_pin must be on same mcu" % (
                config.get_name(),))
        self.mcu.add_config_cmd("config_digital_out oid=%d pin=%s"
                                " value=%d default_value=%d max_duration=%d" % (
                                    self.sda_oid, sda_params['pin'], 1, 1, 0))
    def get_mcu(self):
        return self.mcu
    def build_config(self):
        self.mcu.add_config_cmd("config_digital_out oid=%d pin=%s value=%d"
                                " default_value=%d max_duration=%d" % (
                                    self.scl_oid, self.scl_pin, 1, 1, 0))
        self.update_pin_cmd = self.mcu.lookup_command(
            "update_digital_out oid=%c value=%c", cq=self.cmd_queue)
    def i2c_write(self, msg, minclock=0, reqclock=0):
        msg = [self.addr] + msg
        send = self.scl_main.update_pin_cmd.send
        # Send ack
        send([self.sda_oid, 0], minclock=minclock, reqclock=reqclock)
        send([self.scl_oid, 0], minclock=minclock, reqclock=reqclock)
        # Send bytes
        sda_last = 0
        for data in msg:
            # Transmit 8 data bits
            for i in range(8):
                sda_next = not not (data & (0x80 >> i))
                if sda_last != sda_next:
                    sda_last = sda_next
                    send([self.sda_oid, sda_last],
                         minclock=minclock, reqclock=reqclock)
                send([self.scl_oid, 1], minclock=minclock, reqclock=reqclock)
                send([self.scl_oid, 0], minclock=minclock, reqclock=reqclock)
            # Transmit clock for ack
            send([self.scl_oid, 1], minclock=minclock, reqclock=reqclock)
            send([self.scl_oid, 0], minclock=minclock, reqclock=reqclock)
        # Send stop
        if sda_last:
            send([self.sda_oid, 0], minclock=minclock, reqclock=reqclock)
        send([self.scl_oid, 1], minclock=minclock, reqclock=reqclock)
        send([self.sda_oid, 1], minclock=minclock, reqclock=reqclock)

class mcp4018:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.i2c = SoftwareI2C(config, 0x2f)
        self.scale = config.getfloat('scale', 1., above=0.)
        self.start_value = config.getfloat('wiper',
                                           minval=0., maxval=self.scale)
        config.get_printer().register_event_handler("klippy:connect",
                                                    self.handle_connect)
        # Register commands
        self.name = config.get_name().split()[1]
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("SET_DIGIPOT", "DIGIPOT", self.name,
                                   self.cmd_SET_DIGIPOT,
                                   desc=self.cmd_SET_DIGIPOT_help)
    def handle_connect(self):
        self.set_dac(self.start_value)
    def set_dac(self, value):
        val = int(value * 127. / self.scale + .5)
        self.i2c.i2c_write([val])
    cmd_SET_DIGIPOT_help = "Set digipot value"
    def cmd_SET_DIGIPOT(self, gcmd):
        wiper = gcmd.get_float('WIPER', minval=0., maxval=self.scale)
        if wiper is not None:
            self.set_dac(wiper)
            gcmd.respond_info("New value for DIGIPOT = %s, wiper = %.2f"
                               % (self.name, wiper))
def load_config_prefix(config):
    return mcp4018(config)
