# Bitbang Modbus RTU host layer for LYX9231
# Raw Modbus frame transport over software 8N1 UART
# License: GNU GPLv3

import logging
import time


######################################################################
# Mutex management for shared UART bus instances
######################################################################
class PrinterLYXUartMutexes:
    """Store per-mutex lock objects to prevent concurrent bus access"""

    def __init__(self):
        self.mcu_to_mutex = {}


def lookup_lyx_uart_mutex(mcu):
    """Retrieve or create reactor mutex for MCU bus operations"""
    printer = mcu.get_printer()
    pmutexes = printer.lookup_object('lyx_uart', None)
    if pmutexes is None:
        pmutexes = PrinterLYXUartMutexes()
        printer.add_object('lyx_uart', pmutexes)
    mutex = pmutexes.mcu_to_mutex.get(mcu)
    if mutex is None:
        mutex = printer.get_reactor().mutex()
        pmutexes.mcu_to_mutex[mcu] = mutex
    return mutex


LYX_DEFAULT_BAUD = 38400


######################################################################
# Low-level bitbang UART MCU command wrapper
######################################################################
class MCU_LYX_uart_bitbang:
    """Bind to MCU modbus_uart command and manage pin sharing"""

    def __init__(self, rx_pin_params, tx_pin_params):
        self.mcu = rx_pin_params['chip']
        self.mutex = lookup_lyx_uart_mutex(self.mcu)
        self.rx_pin = rx_pin_params['pin']
        self.tx_pin = tx_pin_params['pin']
        self.oid = self.mcu.create_oid()
        self.cmd_queue = self.mcu.alloc_command_queue()
        self.instances = {}
        self.send_cmd = None
        self.mcu.register_config_callback(self.build_config)

    def build_config(self):
        """Generate MCU config_modbus_uart command on startup"""
        bit_ticks = self.mcu.seconds_to_clock(1. / LYX_DEFAULT_BAUD)
        self.mcu.add_config_cmd(
            "config_modbus_uart oid=%d rx_pin=%s pull_up=%d tx_pin=%s bit_time=%d"
            % (self.oid, self.rx_pin, 1, self.tx_pin, bit_ticks))
        self.send_cmd = self.mcu.lookup_query_command(
            "modbus_uart_send oid=%c write=%*s read=%c",
            "modbus_uart_response oid=%c read=%*s", oid=self.oid,
            cq=self.cmd_queue, is_async=True)

    def register_instance(self, rx_pin_params, tx_pin_params, addr):
        """Validate multi-driver pin sharing rules"""
        if (rx_pin_params['pin'] != self.rx_pin
                or tx_pin_params['pin'] != self.tx_pin):
            raise self.mcu.get_printer().config_error(
                "Shared LYX uarts must use identical pins")
        if addr in self.instances:
            raise self.mcu.get_printer().config_error(
                "LYX uart Modbus addresses must be unique per bus")
        self.instances[addr] = True
        return addr

    # Modbus RTU CRC16 calculation (Modbus standard polynomial 0xA001)
    def _crc16(self, data):
        crc = 0xFFFF
        for b in data:
            crc ^= b
            for _ in range(8):
                if crc & 0x01:
                    crc = (crc >> 1) ^ 0xA001
                else:
                    crc >>= 1
        return crc & 0xFFFF

    def reg_read(self, slave_addr, reg_addr):
        """Send Modbus 0x03 read holding register request, validate CRC response"""
        msg = bytearray([
            slave_addr, 0x03,
            (reg_addr >> 8) & 0xFF, reg_addr & 0xFF,
            0x00, 0x01
        ])
        crc = self._crc16(msg)
        msg.append(crc & 0xFF)
        msg.append((crc >> 8) & 0xFF)
        import logging
        logging.info(f"[LYX HOST] send: len={len(msg)} hex={msg.hex()}")
        params = self.send_cmd.send([self.oid, bytes(msg), 7])
        logging.info(f"[LYX HOST] recv params: {params}")
        raw = params['read']
        logging.info(f"[LYX HOST] raw: len={len(raw)} hex={raw.hex()}")
        if len(raw) < 7 or raw[1] & 0x80:
            return {'data': None, '#receive_time': params.get('#receive_time', 0)}
        if self._crc16(raw[:-2]) != (raw[-1] << 8 | raw[-2]):
            return {'data': None, '#receive_time': params.get('#receive_time', 0)}
        value = (raw[3] << 8) | raw[4]
        return {'data': value, '#receive_time': params.get('#receive_time', 0)}

    def reg_write(self, slave_addr, reg_addr, value, print_time=None):
        """Send Modbus 0x06 single register write frame"""
        minclock = 0
        if print_time is not None:
            minclock = self.mcu.print_time_to_clock(print_time)
        msg = bytearray([
            slave_addr, 0x06,
            (reg_addr >> 8) & 0xFF, reg_addr & 0xFF,
            (value >> 8) & 0xFF, value & 0xFF
        ])
        crc = self._crc16(msg)
        msg.append(crc & 0xFF)
        msg.append((crc >> 8) & 0xFF)
        self.send_cmd.send([self.oid, bytes(msg), 8], minclock=minclock)

    def get_mcu(self):
        """Return bound MCU instance reference"""
        return self.mcu


def lookup_lyx_uart_bitbang(config):
    """Parse printer.cfg pin config, support single-wire shared pin mode"""
    ppins = config.get_printer().lookup_object("pins")
    # Single wire mode: tx/rx share identical GPIO pin
    bus_pin = ppins.lookup_pin(config.get('uart_pin'), share_type="lyx_single")
    rx_pin_params = bus_pin
    tx_pin_params = bus_pin
    addr = config.getint('uart_address', 1, minval=1, maxval=247)
    mcu_uart = rx_pin_params.get('class')
    if mcu_uart is None:
        mcu_uart = MCU_LYX_uart_bitbang(rx_pin_params, tx_pin_params)
        rx_pin_params['class'] = mcu_uart
    mcu_uart.register_instance(rx_pin_params, tx_pin_params)
    return addr, mcu_uart


######################################################################
# High-level register read/write API exposed to driver
######################################################################
class MCU_LYX_uart:
    """Upper layer register access wrapper for LYX9231 driver"""

    def __init__(self, config, name_to_reg, fields):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.name_to_reg = name_to_reg
        self.fields = fields
        self.addr, self.mcu_uart = lookup_lyx_uart_bitbang(config)
        self.mutex = self.mcu_uart.mutex

    def get_fields(self):
        """Return field helper instance for current driver"""
        return self.fields

    def _do_get_register(self, reg_name):
        """Retry read logic with limited iteration count"""
        reg = self.name_to_reg[reg_name]
        if self.printer.get_start_args().get('debugoutput') is not None:
            return {'data': 0, '#receive_time': 0.}
        for retry in range(100):
            print(retry)
            ret = self.mcu_uart.reg_read(self.addr, reg)
            if ret['data'] is not None:
                return ret
            # Small delay to avoid bus overload during retries
            time.sleep(0.001)
        raise self.printer.command_error(
            "Unable to read lyx uart '%s' register %s" % (self.name, reg_name))

    def get_register_raw(self, reg_name):
        """Thread-safe register read entry with mutex lock"""
        with self.mutex:
            return self._do_get_register(reg_name)

    def get_register(self, reg_name):
        """Simplified read interface returning raw integer value"""
        return self.get_register_raw(reg_name)['data']

    def set_register(self, reg_name, val, print_time=None):
        """Write register + readback validation with limited retries"""
        reg = self.name_to_reg[reg_name]
        val = int(val) & 0xFFFF
        if self.printer.get_start_args().get('debugoutput') is not None:
            return
        with self.mutex:
            # Transmit write frame only once per register update
            self.mcu_uart.reg_write(self.addr, val, print_time)
            # Delay for chip register refresh cycle
            time.sleep(0.005)
            # Limited readback retry to verify write success
            for retry in range(200):
                print(retry)
                readback = self.mcu_uart.reg_read(self.addr, reg)
                if readback['data'] == val:
                    return
                time.sleep(0.003)
            raise self.printer.command_error(
                "Unable to write lyx uart '%s' register %s" % (self.name, reg_name))

    def get_mcu(self):
        """Return bound MCU reference"""
        return self.mcu_uart.get_mcu()
