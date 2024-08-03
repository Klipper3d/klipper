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
    pin_resolver = ppins.get_pin_resolver(mcu_name)
    if reserve_pins is not None:
        for pin in reserve_pins.split(','):
            pin_resolver.reserve_pin(pin, bus)
    return bus


######################################################################
# SPI
######################################################################

# Helper code for working with devices connected to an MCU via an SPI bus
class MCU_SPI:
    def __init__(self, mcu, bus, pin, mode, speed, sw_pins=None,
                 cs_active_high=False, sw_bus_width=8, is_little_endian=False,
                 is_lsb_first=False):
        self.mcu = mcu
        self.bus = bus
        # Config SPI object (set all CS pins high before spi_set_bus commands)
        self.oid = mcu.create_oid()
        self.width = sw_bus_width
        self.is_little_endian = is_little_endian
        self.is_lsb_first = is_lsb_first
        # Limit the width to be 5 or more bits to ensure that multiple
        # transactions can be differentiated from a single transaction.
        # (e.g. for w=4 and then 1 byte might be 1 transaction and 4 spare bits
        # or 2 transactions with 4 bits each)
        if self.width < 5 or self.width > 64:
            raise self.mcu.error("Unsupported spi bus width %d "
                                 "(should be 5..64)" % (self.width,))
        if pin is None:
            mcu.add_config_cmd("config_spi_without_cs oid=%d" % (self.oid,))
        else:
            mcu.add_config_cmd("config_spi oid=%d pin=%s cs_active_high=%d"
                               % (self.oid, pin, cs_active_high))
        # Generate SPI bus config message
        if sw_pins is not None:
            self.config_fmt = (
                "spi_set_software_bus oid=%d"
                " miso_pin=%s mosi_pin=%s sclk_pin=%s mode=%d rate=%d width=%d"
                % (self.oid, sw_pins[0], sw_pins[1], sw_pins[2],
                mode, speed, self.width))
        else:
            self.config_fmt = (
                "spi_set_bus oid=%d spi_bus=%%s mode=%d rate=%d"
                % (self.oid, mode, speed))
        self.cmd_queue = mcu.alloc_command_queue()
        mcu.register_config_callback(self.build_config)
        self.spi_send_cmd = self.spi_transfer_cmd = None
    def setup_shutdown_msg(self, shutdown_seq):
        shutdown_msg = "".join(["%02x" % (x,) for x in shutdown_seq])
        self.mcu.add_config_cmd(
            "config_spi_shutdown oid=%d spi_oid=%d shutdown_msg=%s"
            % (self.mcu.create_oid(), self.oid, shutdown_msg))
    def get_oid(self):
        return self.oid
    def get_mcu(self):
        return self.mcu
    def get_command_queue(self):
        return self.cmd_queue
    def build_config(self):
        if '%' in self.config_fmt:
            bus = resolve_bus_name(self.mcu, "spi_bus", self.bus)
            self.config_fmt = self.config_fmt % (bus,)
        self.mcu.add_config_cmd(self.config_fmt)
        self.spi_send_cmd = self.mcu.lookup_command(
            "spi_send oid=%c data=%*s", cq=self.cmd_queue)
        self.spi_transfer_cmd = self.mcu.lookup_query_command(
            "spi_transfer oid=%c data=%*s",
            "spi_transfer_response oid=%c response=%*s", oid=self.oid,
            cq=self.cmd_queue)
    def spi_send(self, data, minclock=0, reqclock=0):
        data_encoded = self.encode(data)
        if self.spi_send_cmd is None:
            # Send setup message via mcu initialization
            data_msg = "".join(["%02x" % (x,) for x in data_encoded])
            self.mcu.add_config_cmd("spi_send oid=%d data=%s" % (
                self.oid, data_msg), is_init=True)
            return
        self.spi_send_cmd.send([self.oid, data_encoded],
                               minclock=minclock, reqclock=reqclock)
    def spi_transfer(self, data, minclock=0, reqclock=0):
        data_encoded = self.encode(data)
        recv = self.spi_transfer_cmd.send([self.oid, data_encoded],
                                          minclock=minclock, reqclock=reqclock)
        return self.decode(recv)

    def spi_transfer_with_preface(self, preface_data, data,
                                  minclock=0, reqclock=0):
        recv = self.spi_transfer_cmd.send_with_preface(
            self.spi_send_cmd, [self.oid, preface_data], [self.oid, data],
            minclock=minclock, reqclock=reqclock)
        return self.decode(recv)

    def encode(self, data):
        if self.width == 8 and not self.is_lsb_first:
            return data
        encoded_bytes = []
        accumulator = 0  # To accumulate bits
        acc_bits = 0  # Count of bits in the accumulator
        format_str = '0{}b'.format(self.width)
        append_from_left = self.is_little_endian
        if self.is_lsb_first:
            append_from_left = not append_from_left
        for number in data:
            if self.is_lsb_first:
                # rotate all bits of the byte msb<->lsb
                number = int(''.join(reversed(format(number, format_str))), 2)
            if append_from_left:
                # Shift the current number into the accumulator from the left
                accumulator |= number << acc_bits
            else:
                # Shift the current number into the accumulator from the right
                accumulator = (accumulator << self.width) | number
            acc_bits += self.width
            # While we have at least 8 bits, form a byte and append it
            while acc_bits >= 8:
                acc_bits -= 8  # Decrease bit count by 8
                if append_from_left:
                    # Extract the 8 least significant bits to form a byte
                    byte = accumulator & 0xFF
                    # Remove lsb 8 bits from the accumulator
                    accumulator >>= 8
                else:
                    # Extract the 8 most significant bits to form a byte
                    byte = (accumulator >> acc_bits) & 0xFF
                    # Remove msb 8 bits from the accumulator
                    accumulator &= (1 << acc_bits) - 1
                encoded_bytes.append(byte)
        # Handle any remaining bits by padding them on the right to byte
        if acc_bits > 0:
            last_byte = (accumulator << (8 - acc_bits)) & 0xFF
            encoded_bytes.append(last_byte)
        return encoded_bytes

    def decode(self, encoded_data):
        if self.width == 8 and not self.is_lsb_first:
            return encoded_data
        decoded_data = []
        accumulator = 0
        acc_bits = 0
        format_str = '0{}b'.format(self.width)
        append_from_left = self.is_little_endian
        if self.is_lsb_first:
            append_from_left = not append_from_left
        if append_from_left:
            # shift the last byte right if it is not full to simplify decoding
            # as leftovers always stay in the msb side of this byte
            last_byte = encoded_data[-1]
            left_overs = len(encoded_data) * 8 % self.width
            encoded_data[-1] = last_byte >> (8 - left_overs)
        for byte in encoded_data:
            # Adjust byte order based on endianness
            if append_from_left:
                # Shift the bytes into the accumulator from the left
                accumulator |= byte << acc_bits
            else:
                # Shift the bytes into the accumulator from the right
                accumulator = (accumulator << 8) | byte
            acc_bits += 8
            # Decode numbers from the accumulator
            while acc_bits >= self.width:
                acc_bits -= self.width
                num = 0
                if append_from_left:
                    num = accumulator & ((1 << self.width) - 1)
                    accumulator >>= self.width
                else:
                    num = (accumulator >> acc_bits) & ((1 << self.width) - 1)
                    accumulator &= (1 << acc_bits) - 1
                # Apply lsb-first rotation if required
                if self.is_lsb_first:
                    num = int(''.join(reversed(format(num, format_str))), 2)
                # Append the decoded number
                decoded_data.append(num)
        # as byte array may have leftover bits (len*width % 8 != 0),
        # it is ok to ignore acc_bits and drop remaining accumulated bits
        return decoded_data

# Helper to setup an spi bus from settings in a config section
def MCU_SPI_from_config(config, mode, pin_option="cs_pin",
                        default_speed=100000, share_type=None,
                        cs_active_high=False, sw_bus_width=8,
                        is_little_endian=False, is_lsb_first=False):
    # Determine pin from config
    ppins = config.get_printer().lookup_object("pins")
    cs_pin = config.get(pin_option)
    cs_pin_params = ppins.lookup_pin(cs_pin, share_type=share_type)
    pin = cs_pin_params['pin']
    if pin == 'None':
        ppins.reset_pin_sharing(cs_pin_params)
        pin = None
    # Load bus parameters
    mcu = cs_pin_params['chip']
    speed = config.getint('spi_speed', default_speed, minval=100000)
    sw_bus_width = config.getint('spi_software_bus_width', sw_bus_width,
                                 minval=5, maxval=64)
    sw_bus_is_little_endian=config.getboolean('spi_software_is_little_endian',
                                              is_little_endian)
    sw_bus_is_lsb_first=config.getboolean('spi_software_is_lsb_first',
                                          is_lsb_first)
    if config.get('spi_software_sclk_pin', None) is not None:
        sw_pin_names = ['spi_software_%s_pin' % (name,)
                        for name in ['miso', 'mosi', 'sclk']]
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
    return MCU_SPI(mcu, bus, pin, mode, speed, sw_pins,
                   cs_active_high, sw_bus_width, sw_bus_is_little_endian,
                   sw_bus_is_lsb_first)

######################################################################
# I2C
######################################################################

# Helper code for working with devices connected to an MCU via an I2C bus
class MCU_I2C:
    def __init__(self, mcu, bus, addr, speed, sw_pins=None):
        self.mcu = mcu
        self.bus = bus
        self.i2c_address = addr
        self.oid = self.mcu.create_oid()
        mcu.add_config_cmd("config_i2c oid=%d" % (self.oid,))
        # Generate I2C bus config message
        if sw_pins is not None:
            self.config_fmt = (
                "i2c_set_software_bus oid=%d"
                " scl_pin=%s sda_pin=%s rate=%d address=%d"
                % (self.oid, sw_pins[0], sw_pins[1], speed, addr))
        else:
            self.config_fmt = (
                "i2c_set_bus oid=%d i2c_bus=%%s rate=%d address=%d"
                % (self.oid, speed, addr))
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
        if '%' in self.config_fmt:
            bus = resolve_bus_name(self.mcu, "i2c_bus", self.bus)
            self.config_fmt = self.config_fmt % (bus,)
        self.mcu.add_config_cmd(self.config_fmt)
        self.i2c_write_cmd = self.mcu.lookup_command(
            "i2c_write oid=%c data=%*s", cq=self.cmd_queue)
        self.i2c_read_cmd = self.mcu.lookup_query_command(
            "i2c_read oid=%c reg=%*s read_len=%u",
            "i2c_read_response oid=%c response=%*s", oid=self.oid,
            cq=self.cmd_queue)
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
    def i2c_write_wait_ack(self, data, minclock=0, reqclock=0):
        self.i2c_write_cmd.send_wait_ack([self.oid, data],
                                minclock=minclock, reqclock=reqclock)
    def i2c_read(self, write, read_len):
        return self.i2c_read_cmd.send([self.oid, write, read_len])
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
    if default_addr is None:
        addr = config.getint('i2c_address', minval=0, maxval=127)
    else:
        addr = config.getint('i2c_address', default_addr, minval=0, maxval=127)
    # Determine pin from config
    ppins = config.get_printer().lookup_object("pins")
    if config.get('i2c_software_scl_pin', None) is not None:
        sw_pin_names = ['i2c_software_%s_pin' % (name,)
                        for name in ['scl', 'sda']]
        sw_pin_params = [ppins.lookup_pin(config.get(name), share_type=name)
                         for name in sw_pin_names]
        sw_pins = tuple([pin_params['pin'] for pin_params in sw_pin_params])
        bus = None
    else:
        bus = config.get('i2c_bus', None)
        sw_pins = None
    # Create MCU_I2C object
    return MCU_I2C(i2c_mcu, bus, addr, speed, sw_pins)


######################################################################
# Bus synchronized digital outputs
######################################################################

# Helper code for a gpio that updates on a cmd_queue
class MCU_bus_digital_out:
    def __init__(self, mcu, pin_desc, cmd_queue=None, value=0):
        self.mcu = mcu
        self.oid = mcu.create_oid()
        ppins = mcu.get_printer().lookup_object('pins')
        pin_params = ppins.lookup_pin(pin_desc)
        if pin_params['chip'] is not mcu:
            raise ppins.error("Pin %s must be on mcu %s" % (
                pin_desc, mcu.get_name()))
        mcu.add_config_cmd("config_digital_out oid=%d pin=%s value=%d"
                           " default_value=%d max_duration=%d"
                           % (self.oid, pin_params['pin'], value, value, 0))
        mcu.register_config_callback(self.build_config)
        if cmd_queue is None:
            cmd_queue = mcu.alloc_command_queue()
        self.cmd_queue = cmd_queue
        self.update_pin_cmd = None
    def get_oid(self):
        return self.oid
    def get_mcu(self):
        return self.mcu
    def get_command_queue(self):
        return self.cmd_queue
    def build_config(self):
        self.update_pin_cmd = self.mcu.lookup_command(
            "update_digital_out oid=%c value=%c", cq=self.cmd_queue)
    def update_digital_out(self, value, minclock=0, reqclock=0):
        if self.update_pin_cmd is None:
            # Send setup message via mcu initialization
            self.mcu.add_config_cmd("update_digital_out oid=%c value=%c"
                                    % (self.oid, not not value))
            return
        self.update_pin_cmd.send([self.oid, not not value],
                                 minclock=minclock, reqclock=reqclock)
