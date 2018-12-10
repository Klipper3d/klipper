# Helper code for SPI and I2C bus communication
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import mcu


######################################################################
# SPI
######################################################################

# Helper code for working with devices connected to an MCU via an SPI bus
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


######################################################################
# I2C
######################################################################

# Helper code for working with devices connected to an MCU via an I2C bus
class MCU_I2C:
    def __init__(self, mcu, bus, addr, speed):
        self.mcu = mcu
        self.oid = self.mcu.create_oid()
        self.mcu.add_config_cmd("config_i2c oid=%d bus=%d rate=%d addr=%d" % (
            self.oid, bus, speed, addr))
        self.cmd_queue = self.mcu.alloc_command_queue()
        self.mcu.register_config_callback(self.build_config)
        self.i2c_write_cmd = self.i2c_read_cmd = self.i2c_modify_bits_cmd = None
    def get_oid(self):
        return self.oid
    def get_mcu(self):
        return self.mcu
    def get_command_queue(self):
        return self.cmd_queue
    def build_config(self):
        self.i2c_write_cmd = self.mcu.lookup_command(
            "i2c_write oid=%c data=%*s", cq=self.cmd_queue)
        self.i2c_read_cmd = self.mcu.lookup_command(
            "i2c_read oid=%c reg=%*s read_len=%u", cq=self.cmd_queue)
        self.i2c_modify_bits_cmd = self.mcu.lookup_command(
            "i2c_modify_bits oid=%c reg=%*s clear_set_bits=%*s",
            cq=self.cmd_queue)
    def i2c_write(self, data, minclock=0, reqclock=0):
        if self.i2c_write_cmd is None:
            # Send setup message via mcu initialization
            data_msg = "".join(["%02x" % (x,) for x in data])
            self.mcu.add_config_cmd("i2c_write oid=%d data=%s" % (
                self.oid, data_msg), is_init=True)
            return
        self.i2c_write_cmd.send([self.oid, data],
                                minclock=minclock, reqclock=reqclock)
    def i2c_read(self, write, read_len):
        return self.i2c_read_cmd.send_with_response(
            [self.oid, write, read_len], 'i2c_read_response', self.oid)
    def i2c_modify_bits(self, reg, clear_bits, set_bits,
                        minclock=0, reqclock=0):
        clearset = clear_bits + set_bits
        if self.i2c_modify_bits_cmd is None:
            # Send setup message via mcu initialization
            reg_msg = "".join(["%02x" % (x,) for x in reg])
            clearset_msg = "".join(["%02x" % (x,) for x in clearset])
            self.mcu.add_config_cmd(
                "i2c_modify_bits oid=%d reg=%s clear_set_bits=%s" % (
                    self.oid, reg_msg, clearset_msg), is_init=True)
            return
        self.i2c_modify_bits_cmd.send([self.oid, reg, clearset],
                                      minclock=minclock, reqclock=reqclock)

def MCU_I2C_from_config(config, default_addr=None, default_speed=100000):
    # Load bus parameters
    printer = config.get_printer()
    i2c_mcu = mcu.get_printer_mcu(printer, config.get('i2c_mcu', 'mcu'))
    speed = config.getint('i2c_speed', default_speed, minval=100000)
    bus = config.getint('i2c_bus', 0, minval=0)
    if default_addr is None:
        addr = config.getint('i2c_address')
    else:
        addr = config.getint('i2c_address', default_addr)
    # Create MCU_I2C object
    return MCU_I2C(i2c_mcu, bus, addr, speed)
