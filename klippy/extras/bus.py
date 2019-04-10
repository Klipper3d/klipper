# Helper code for SPI and I2C bus communication
#
# Copyright (C) 2018,2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import mcu

def resolve_bus_name(mcu, param, bus):
    # Find enumerations for the given bus
    enumerations = mcu.get_enumerations()
    enums = enumerations.get(param, enumerations.get('bus'))
    if enums is None:
        if bus is None:
            return 0
        return bus
    # Verify bus is a valid enumeration
    ppins = mcu.get_printer().lookup_object("pins")
    mcu_name = mcu.get_name()
    if bus is None:
        rev_enums = {v: k for k, v in enums.items()}
        if 0 not in rev_enums:
            raise ppins.error("Must specify %s on mcu '%s'" % (param, mcu_name))
        bus = rev_enums[0]
    if bus not in enums:
        raise ppins.error("Unknown %s '%s'" % (param, bus))
    # Check for reserved bus pins
    constants = mcu.get_constants()
    reserve_pins = constants.get('BUS_PINS_%s' % (bus,), None)
    if reserve_pins is not None:
        for pin in reserve_pins.split(','):
            ppins.reserve_pin(mcu_name, pin, bus)
    return bus


######################################################################
# SPI
######################################################################

# Helper code for working with devices connected to an MCU via an SPI bus
class MCU_SPI:
    def __init__(self, mcu, bus, pin, mode, speed, shutdown_seq, sw_pins=None):
        self.mcu = mcu
        self.bus = bus
        shutdown_msg = "".join(["%02x" % (x,) for x in shutdown_seq])
        self.oid = self.mcu.create_oid()
        if pin is not None:
            # Set all CS pins high before first config_spi
            self.mcu.add_config_cmd("set_digital_out pin=%s value=1" % (pin,))
        self.config_sw_msg = None
        if sw_pins is not None:
            software_spi_oid = self.mcu.create_oid()
            self.config_sw_msg = (
                "config_software_spi oid=%d sclk_pin=%s mosi_pin=%s miso_pin=%s"
                " mode=%d rate=%d" % (
                    software_spi_oid, sw_pins[0], sw_pins[1], sw_pins[2],
                    mode, speed))
            self.config_fmt = (
                "config_spi_from_software oid=%d sw_oid=%d pin=%s"
                " shutdown_msg=%s" % (
                    self.oid, software_spi_oid, pin, shutdown_msg))
        elif pin is None:
            self.config_fmt = (
                "config_spi_without_cs oid=%d spi_bus=%%s mode=%d rate=%d"
                " shutdown_msg=%s" % (self.oid, mode, speed, shutdown_msg))
        else:
            self.config_fmt = (
                "config_spi oid=%d spi_bus=%%s pin=%s mode=%d rate=%d"
                " shutdown_msg=%s" % (self.oid, pin, mode, speed, shutdown_msg))
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
        if self.config_sw_msg is not None:
            self.mcu.add_config_cmd(self.config_sw_msg)
            self.mcu.add_config_cmd(self.config_fmt)
        else:
            bus = resolve_bus_name(self.mcu, "spi_bus", self.bus)
            self.mcu.add_config_cmd(self.config_fmt % (bus,))
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
        ppins.reset_pin_sharing(cs_pin_params)
        pin = None
    # Load bus parameters
    mcu = cs_pin_params['chip']
    speed = config.getint('spi_speed', default_speed, minval=100000)
    if config.get('spi_software_sclk_pin', None) is not None:
        sw_pin_names = ['spi_software_%s_pin' % (name,)
                        for name in ['sclk', 'mosi', 'miso']]
        sw_pin_params = [ppins.lookup_pin(config.get(name), share_type=name)
                         for name in sw_pin_names]
        for pin_params in sw_pin_params:
            if pin_params['chip'] != mcu:
                raise ppins.error("%s: spi pins must be on same mcu" % (
                    config.get_name(),))
        sw_pins = tuple([pin_params['pin'] for pin_params in sw_pin_params])
        bus = None
    else:
        bus = config.get('spi_bus', None)
        sw_pins = None
    # Create MCU_SPI object
    return MCU_SPI(mcu, bus, pin, mode, speed, shutdown_seq, sw_pins)


######################################################################
# I2C
######################################################################

# Helper code for working with devices connected to an MCU via an I2C bus
class MCU_I2C:
    def __init__(self, mcu, bus, addr, speed):
        self.mcu = mcu
        self.bus = bus
        self.i2c_address = addr
        self.oid = self.mcu.create_oid()
        self.config_fmt = "config_i2c oid=%d i2c_bus=%%s rate=%d address=%d" % (
            self.oid, speed, addr)
        self.cmd_queue = self.mcu.alloc_command_queue()
        self.mcu.register_config_callback(self.build_config)
        self.i2c_write_cmd = self.i2c_read_cmd = self.i2c_modify_bits_cmd = None
    def get_oid(self):
        return self.oid
    def get_mcu(self):
        return self.mcu
    def get_i2c_address(self):
        return self.i2c_address
    def get_command_queue(self):
        return self.cmd_queue
    def build_config(self):
        bus = resolve_bus_name(self.mcu, "i2c_bus", self.bus)
        self.mcu.add_config_cmd(self.config_fmt % (bus,))
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
    bus = config.get('i2c_bus', None)
    if default_addr is None:
        addr = config.getint('i2c_address', minval=0, maxval=127)
    else:
        addr = config.getint('i2c_address', default_addr, minval=0, maxval=127)
    # Create MCU_I2C object
    return MCU_I2C(i2c_mcu, bus, addr, speed)
