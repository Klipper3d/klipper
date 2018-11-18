# Helper code for SPI bus communication
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

# Helper code for working with devices conntect to an MCU via an SPI bus
class MCU_SPI:
    def __init__(self, mcu, bus, pin, mode, speed, shutdown_seq):
        self.mcu = mcu
        shutdown_msg = "".join(["%02x" % (x,) for x in shutdown_seq])
        self.oid = self.mcu.create_oid()
        if pin is None:
            self.mcu.add_config_cmd(
                "config_spi_without_cs oid=%d bus=%d mode=%d rate=%d"
                " shutdown_msg=%s" % (
                    self.oid, bus, mode, speed, shutdown_msg))
        else:
            self.mcu.add_config_cmd(
                "config_spi oid=%d bus=%d pin=%s mode=%d rate=%d"
                " shutdown_msg=%s" % (
                    self.oid, bus, pin, mode, speed, shutdown_msg))
        self.cmd_queue = self.mcu.alloc_command_queue()
        self.mcu.register_config_callback(self.build_config)
        self.spi_send_cmd = self.spi_transfer_cmd = None
    def get_oid(self):
        return self.oid
    def get_mcu(self):
        return self.mcu
    def get_command_queue(self):
        return self.cmd_queue
    def build_config(self):
        self.spi_send_cmd = self.mcu.lookup_command(
            "spi_send oid=%c data=%*s", cq=self.cmd_queue)
        self.spi_transfer_cmd = self.mcu.lookup_command(
            "spi_transfer oid=%c data=%*s", cq=self.cmd_queue)
    def spi_send(self, data, minclock=0, reqclock=0):
        if self.spi_send_cmd is None:
            # Send setup message via mcu initialization
            data_msg = "".join(["%02x" % (x,) for x in data])
            self.mcu.add_config_cmd("spi_send oid=%d data=%s" % (
                self.oid, data_msg), is_init=True)
            return
        self.spi_send_cmd.send([self.oid, data],
                               minclock=minclock, reqclock=reqclock)
    def spi_transfer(self, data):
        return self.spi_transfer_cmd.send_with_response(
            [self.oid, data], 'spi_transfer_response', self.oid)

# Helper to setup an spi bus from settings in a config section
def MCU_SPI_from_config(config, mode, pin_option="cs_pin",
                        default_speed=100000, shutdown_seq=()):
    # Determine pin from config
    ppins = config.get_printer().lookup_object("pins")
    cs_pin = config.get(pin_option)
    cs_pin_params = ppins.lookup_pin(cs_pin)
    pin = cs_pin_params['pin']
    if pin == 'None':
        pin = None
    # Load bus parameters
    speed = config.getint('spi_speed', default_speed, minval=100000)
    bus = config.getint('spi_bus', 0, minval=0)
    # Create MCU_SPI object
    mcu = cs_pin_params['chip']
    return MCU_SPI(mcu, bus, pin, mode, speed, shutdown_seq)
