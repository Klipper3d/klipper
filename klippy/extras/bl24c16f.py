# Support for i2c based temperature sensors
#
# Copyright (C) 2020  Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import struct
from . import bus

BL24C16F_CHIP_ADDR_0 = 0x50
BL24C16F_CHIP_ADDR_1 = 0x51
BL24C16F_CHIP_ADDR_2 = 0x52
BL24C16F_CHIP_ADDR_3 = 0x53
BL24C16F_CHIP_ADDR_4 = 0x54
BL24C16F_CHIP_ADDR_5 = 0x55
BL24C16F_CHIP_ADDR_6 = 0x56
BL24C16F_CHIP_ADDR_7 = 0x57

class EEPROMCommandHelper:
    def __init__(self, config, chip):
        self.printer = config.get_printer()
        self.chip = chip
        name_parts = config.get_name().split()
        self.base_name = name_parts[0]
        self.name = name_parts[-1]
        self.register_commands(self.name)
        if len(name_parts) == 1:
            if self.name == "bl24c16f" or not config.has_section("bl24c16f"):
                self.register_commands(None)

    def register_commands(self, name):
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("EEPROM_DEBUG_READ", "CHIP", name,
                                   self.cmd_EEPROM_DEBUG_READ,
                                   desc=self.cmd_EEPROM_DEBUG_READ_help)
        gcode.register_mux_command("EEPROM_DEBUG_WRITE_BYTE", "CHIP", name,
                                   self.cmd_EEPROM_DEBUG_WRITE_BYTE,
                                   desc=self.cmd_EEPROM_DEBUG_WRITE_BYTE_help)
        gcode.register_mux_command("EEPROM_DEBUG_WRITE_INT", "CHIP", name,
                                   self.cmd_EEPROM_DEBUG_WRITE_INT,
                                   desc=self.cmd_EEPROM_DEBUG_WRITE_INT_help)
        gcode.register_mux_command("EEPROM_DEBUG_WRITE_FLOAT", "CHIP", name,
                                   self.cmd_EEPROM_DEBUG_WRITE_FLOAT,
                                   desc=self.cmd_EEPROM_DEBUG_WRITE_FLOAT_help)
        
        gcode.register_mux_command("EEPROM_READ", "CHIP", name,
                                   self.cmd_EEPROM_READ,
                                   desc=self.cmd_EEPROM_READ_help)
        gcode.register_mux_command("EEPROM_WRITE_BYTE", "CHIP", name,
                                   self.cmd_EEPROM_WRITE_BYTE,
                                   desc=self.cmd_EEPROM_WRITE_BYTE_help)
        gcode.register_mux_command("EEPROM_WRITE_INT", "CHIP", name,
                                   self.cmd_EEPROM_WRITE_INT,
                                   desc=self.cmd_EEPROM_WRITE_INT_help)
        gcode.register_mux_command("EEPROM_WRITE_FLOAT", "CHIP", name,
                                   self.cmd_EEPROM_WRITE_FLOAT,
                                   desc=self.cmd_EEPROM_WRITE_FLOAT_help)
        
        gcode.register_mux_command("EEPROM_IS_FIRST_USED", "CHIP", name,
                                   self.cmd_EEPROM_IS_FIRST_USED)
        gcode.register_mux_command("EEPROM_POS", "CHIP", name,
                                   self.cmd_EEPROM_POS)
        gcode.register_mux_command("EEPROM_PRINTER_INFO", "CHIP", name,
                                   self.cmd_EEPROM_PRINTER_INFO)

    def cmd_EEPROM_IS_FIRST_USED(self, gcmd):
        val = self.chip.read_reg(1, 1)
        state = False if int.from_bytes(val, 'little') != 255 else True
        gcmd.respond_info("EEPROM_IS_USED val:%s state:%s" % (int.from_bytes(val, 'little'), state))
        if int.from_bytes(val, 'little') != 255:
            return False
        else:
            return True
    
    def cmd_EEPROM_POS(self, gcmd):
        pos = self.chip.read_reg(0, 1)
        gcmd.respond_info("EEPROM_POS int_pos:%s, pos:%s" % (int.from_bytes(pos, 'little'), pos))
        
    def cmd_EEPROM_PRINTER_INFO(self, gcmd):
        pos = int.from_bytes(self.chip.read_reg(0, 1), 'little')
        file_position = self.chip.read_reg(pos*8, 4)
        base_position_e = self.chip.read_reg(pos*8+4, 4)
        ret =  {"file_position": int.from_bytes(file_position, 'little'), "base_position_e": struct.unpack('f', base_position_e)[0]}
        gcmd.respond_info("EEPROM_PRINTER_INFO ret:%s" % str(ret))

    def cmd_EEPROM_DEBUG_READ(self, gcmd):
        addr = gcmd.get("ADDR", minval=0, maxval=2047, parser=lambda x: int(x, 0))
        size = gcmd.get("SIZE", minval=0, maxval=56, parser=lambda x: int(x, 0))
        vals = self.chip.read_reg(addr, size)
        gcmd.respond_info("EEPROM_DEBUG_READ size: 0x%x" % size)
        reg_vals = 'read vals: '
        for i in range(size):
            if i % 16 == 0:
                reg_vals += '\n'
            reg_vals += '0x%x ' % vals[i]

        gcmd.respond_info(reg_vals)

    cmd_EEPROM_DEBUG_READ_help = "Read data bytes from eeprom"

    def cmd_EEPROM_DEBUG_WRITE_BYTE(self, gcmd):
        addr = gcmd.get("ADDR", minval=0, maxval=2047, parser=lambda x: int(x, 0))
        val = gcmd.get("VAL", minval=0, maxval=255, parser=lambda x: int(x, 0))
        gcmd.respond_info("EEPROM_DEBUG_WRITE_BYTE : ADDR[0x%x] = 0x%x" % (addr, val))
        self.chip.write_reg(addr, val)

    cmd_EEPROM_DEBUG_WRITE_BYTE_help = "Write byte data to eeprom"

    def cmd_EEPROM_DEBUG_WRITE_INT(self, gcmd):
        pos = self.chip.read_reg(0, 1)
        gcmd.respond_info("EEPROM_POS int_pos:%s" % int.from_bytes(pos, 'little'))
        addr = gcmd.get("ADDR", minval=0, maxval=2047, parser=lambda x: int(x, 0))
        val = gcmd.get("VAL", minval=0, maxval=4294967296, parser=lambda x: int(x, 0))
        gcmd.respond_info("EEPROM_DEBUG_WRITE_INT : val = %d" % val)
        vals = [val & 0xFF]
        vals += [   (val >> 8) & 0xFF,
                    (val >> 16) & 0xFF,
                    (val >> 24) & 0xFF,
                ]
        gcmd.respond_info("EEPROM_DEBUG_WRITE_INT : ADDR[0x%x] = 0x%02x 0x%02x 0x%02x 0x%02x"
                % (addr, vals[0], vals[1], vals[2], vals[3]))
        self.chip.write_reg(addr, vals)

    cmd_EEPROM_DEBUG_WRITE_INT_help = "Write int (4 byte) data to eeprom"

    def cmd_EEPROM_DEBUG_WRITE_FLOAT(self, gcmd):
        addr = gcmd.get("ADDR", minval=0, maxval=2047, parser=lambda x: int(x, 0))
        val = gcmd.get_float("VAL", 0.)
        gcmd.respond_info("EEPROM_DEBUG_WRITE_FLOAT : val = %f" % val)
        bs = struct.pack("f", val)
        data = int.from_bytes(bs, byteorder="little")

        vals = [data & 0xFF]
        vals += [   (data >> 8) & 0xFF, 
                    (data >> 16) & 0xFF,
                    (data >> 24) & 0xFF
                ]
        gcmd.respond_info("EEPROM_DEBUG_WRITE_FLOAT : ADDR[0x%x] = 0x%02x 0x%02x 0x%02x 0x%02x"
                        % (addr, vals[0], vals[1], vals[2], vals[3]))
        self.chip.write_reg(addr, vals)

    cmd_EEPROM_DEBUG_WRITE_FLOAT_help = "Write float (4 byte) data to eeprom"
    
    def cmd_EEPROM_READ(self, gcmd):
        addr = gcmd.get("ADDR", minval=0, maxval=2047, parser=lambda x: int(x, 0))
        size = gcmd.get("SIZE", minval=0, maxval=56, parser=lambda x: int(x, 0))
        vals = self.chip.read_reg(addr, size)
        # gcmd.respond_info("EEPROM_READ size: 0x%x" % size)
        reg_vals = 'read vals: '
        for i in range(size):
            if i % 16 == 0:
                reg_vals += '\n'
            reg_vals += '0x%x ' % vals[i]
        # gcmd.respond_info(reg_vals)
    cmd_EEPROM_READ_help = "Read data bytes from eeprom"

    def cmd_EEPROM_WRITE_BYTE(self, gcmd):
        addr = gcmd.get("ADDR", minval=0, maxval=2047, parser=lambda x: int(x, 0))
        val = gcmd.get("VAL", minval=0, maxval=255, parser=lambda x: int(x, 0))
        # gcmd.respond_info("EEPROM_WRITE_BYTE : ADDR[0x%x] = 0x%x" % (addr, val))
        self.chip.write_reg(addr, val)
    cmd_EEPROM_WRITE_BYTE_help = "Write byte data to eeprom"

    def cmd_EEPROM_WRITE_INT(self, gcmd):
        # pos = self.chip.read_reg(0, 1)
        # gcmd.respond_info("EEPROM_POS int_pos:%s" % int.from_bytes(pos, 'little'))
        addr = gcmd.get("ADDR", minval=0, maxval=2047, parser=lambda x: int(x, 0))
        val = gcmd.get("VAL", minval=0, maxval=4294967296, parser=lambda x: int(x, 0))
        # gcmd.respond_info("EEPROM_WRITE_INT : val = %d" % val)
        vals = [val & 0xFF]
        vals += [   (val >> 8) & 0xFF,
                    (val >> 16) & 0xFF,
                    (val >> 24) & 0xFF,
                ]
        # gcmd.respond_info("EEPROM_WRITE_INT : ADDR[0x%x] = 0x%02x 0x%02x 0x%02x 0x%02x"
                # % (addr, vals[0], vals[1], vals[2], vals[3]))
        self.chip.write_reg(addr, vals)
    cmd_EEPROM_WRITE_INT_help = "Write int (4 byte) data to eeprom"

    def cmd_EEPROM_WRITE_FLOAT(self, gcmd):
        addr = gcmd.get("ADDR", minval=0, maxval=2047, parser=lambda x: int(x, 0))
        val = gcmd.get_float("VAL", 0.)
        # gcmd.respond_info("EEPROM_WRITE_FLOAT : val = %f" % val)
        bs = struct.pack("f", val)
        data = int.from_bytes(bs, byteorder="little")

        vals = [data & 0xFF]
        vals += [   (data >> 8) & 0xFF, 
                    (data >> 16) & 0xFF,
                    (data >> 24) & 0xFF
                ]
        # gcmd.respond_info("EEPROM_WRITE_FLOAT : ADDR[0x%x] = 0x%02x 0x%02x 0x%02x 0x%02x"
                        # % (addr, vals[0], vals[1], vals[2], vals[3]))
        self.chip.write_reg(addr, vals)
    cmd_EEPROM_WRITE_FLOAT_help = "Write float (4 byte) data to eeprom"  

class BL24C16F:
    def __init__(self, config):
        self.printer = config.get_printer()
        EEPROMCommandHelper(config, self)
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()
        self.i2c0 = bus.MCU_I2C_from_config(
            config, default_addr=BL24C16F_CHIP_ADDR_0, default_speed=400000)
        self.i2c1 = bus.MCU_I2C_from_config(
            config, default_addr=BL24C16F_CHIP_ADDR_1, default_speed=400000)
        self.i2c2 = bus.MCU_I2C_from_config(
            config, default_addr=BL24C16F_CHIP_ADDR_2, default_speed=400000)
        self.i2c3 = bus.MCU_I2C_from_config(
            config, default_addr=BL24C16F_CHIP_ADDR_3, default_speed=400000)
        self.i2c4 = bus.MCU_I2C_from_config(
            config, default_addr=BL24C16F_CHIP_ADDR_4, default_speed=400000)
        self.i2c5 = bus.MCU_I2C_from_config(
            config, default_addr=BL24C16F_CHIP_ADDR_5, default_speed=400000)
        self.i2c6 = bus.MCU_I2C_from_config(
            config, default_addr=BL24C16F_CHIP_ADDR_6, default_speed=400000)
        self.i2c7 = bus.MCU_I2C_from_config(
            config, default_addr=BL24C16F_CHIP_ADDR_7, default_speed=400000)
        self.mcu = self.i2c0.get_mcu()
        self.printer.add_object("bl24c16f " + self.name, self)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)

    def handle_connect(self):
        self._init_bl24c16f()

    def _init_bl24c16f(self):
        logging.info("bl24c16f init...")

    def read_reg(self, addr, read_len):
        index = addr // 256
        offset = addr % 256
        reg = [offset]

        if index == 0 :
            params = self.i2c0.i2c_read(reg, read_len)
        elif index == 1 :
            params = self.i2c1.i2c_read(reg, read_len)
        elif index == 2 :
            params = self.i2c2.i2c_read(reg, read_len)
        elif index == 3 :
            params = self.i2c3.i2c_read(reg, read_len)
        elif index == 4 :
            params = self.i2c4.i2c_read(reg, read_len)
        elif index == 5 :
            params = self.i2c5.i2c_read(reg, read_len)
        elif index == 6 :
            params = self.i2c6.i2c_read(reg, read_len)
        elif index == 7 :
            params = self.i2c7.i2c_read(reg, read_len)

        return bytearray(params['response'])

    def write_reg(self, addr, data):
        if type(data) is not list:
            data = [data]

        index = addr // 256
        offset = addr % 256
        data.insert(0, offset)
        if index == 0 :
            self.i2c0.i2c_write(data)
        if index == 1 :
            self.i2c1.i2c_write(data)
        if index == 2 :
            self.i2c2.i2c_write(data)
        if index == 3 :
            self.i2c3.i2c_write(data)
        if index == 4 :
            self.i2c4.i2c_write(data)
        if index == 5 :
            self.i2c5.i2c_write(data)
        if index == 6 :
            self.i2c6.i2c_write(data)
        if index == 7 :
            self.i2c7.i2c_write(data)

    def setEepromDisable(self):
        self.write_reg(1, 255)

    def checkEepromFirstEnable(self):
        val = self.read_reg(1, 1)
        if int.from_bytes(val, 'little') != 255:
            return False
        else:
            return True
        
    def eepromReadHeader(self):
        pos = self.read_reg(0, 1)
        return int.from_bytes(pos, 'little')
    
    def eepromReadBody(self, pos):
        file_position = self.read_reg(pos*8, 4)
        base_position_e = self.read_reg(pos*8+4, 4)
        return {"file_position": int.from_bytes(file_position, 'little'), "base_position_e": struct.unpack('f', base_position_e)[0]}

def load_config(config):
    return BL24C16F(config)

def load_config_prefix(config):
    return BL24C16F(config)
