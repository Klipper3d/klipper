# LYX stepper driver UART (Modbus RTU) communication
#
# Author: zylo117
# Based on tmc_uart.py architecture
# License: GNU GPLv3

import logging

######################################################################
# Modbus RTU over tmcuart bitbang
######################################################################

# Share mutexes - only one active command per mcu at a time
class PrinterLYXUartMutexes:
    def __init__(self):
        self.mcu_to_mutex = {}

def lookup_lyx_uart_mutex(mcu):
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

class MCU_LYX_uart_bitbang:
    def __init__(self, rx_pin_params, tx_pin_params):
        self.mcu = rx_pin_params['chip']
        self.mutex = lookup_lyx_uart_mutex(self.mcu)
        self.rx_pin = rx_pin_params['pin']
        self.tx_pin = tx_pin_params['pin']
        self.oid = self.mcu.create_oid()
        self.cmd_queue = self.mcu.alloc_command_queue()
        self.instances = {}
        self.tmcuart_send_cmd = None
        self.mcu.register_config_callback(self.build_config)

    def build_config(self):
        baud = LYX_DEFAULT_BAUD
        bit_ticks = self.mcu.seconds_to_clock(1. / baud)

        # 重用tmc的和固件的软uart通信
        self.mcu.add_config_cmd(
            "config_tmcuart oid=%d rx_pin=%s pull_up=%d tx_pin=%s bit_time=%d"
            % (self.oid, self.rx_pin, 0, self.tx_pin, bit_ticks))
        self.tmcuart_send_cmd = self.mcu.lookup_query_command(
            "tmcuart_send oid=%c write=%*s read=%c",
            "tmcuart_response oid=%c read=%*s", oid=self.oid,
            cq=self.cmd_queue, is_async=True)

    def register_instance(self, rx_pin_params, tx_pin_params, addr):
        if (rx_pin_params['pin'] != self.rx_pin
            or tx_pin_params['pin'] != self.tx_pin):
            raise self.mcu.get_printer().config_error(
                "Shared LYX uarts must use the same pins")
        if addr in self.instances:
            raise self.mcu.get_printer().config_error(
                "LYX uart addresses must be unique on shared bus")
        self.instances[addr] = True
        return addr

    # ----------------------------------------------------------------
    # 标准UART 8N1 位流打包/解包（和TMC逻辑完全一致，因为都是8N1）
    # ----------------------------------------------------------------
    def _add_serial_bits(self, data):
        # 每个字节扩展为10位：1起始位(低) + 8数据位(LSB在前) + 1停止位(高)
        out = 0
        pos = 0
        for d in data:
            b = (d << 1) | 0x200  # bit0=0(起始), bit1-8=data, bit9=1(停止)
            out |= (b << pos)
            pos += 10
        res = bytearray()
        for i in range((pos + 7) // 8):
            res.append((out >> (i * 8)) & 0xff)
        return res

    def _extract_serial_bits(self, data, byte_count):
        # 从bit流中提取出byte_count个有效字节（去掉起始停止位）
        if len(data) * 8 < byte_count * 10:
            return None
        bitstream = 0
        for i, b in enumerate(data):
            bitstream |= b << (i * 8)
        result = bytearray()
        for i in range(byte_count):
            pos = i * 10
            byte_val = (bitstream >> (pos + 1)) & 0xff  # 跳过起始位
            result.append(byte_val)
        return result

    # ----------------------------------------------------------------
    # Modbus RTU 编解码
    # ----------------------------------------------------------------
    def _calc_crc16(self, data):
        # Modbus RTU CRC16, polynomial 0xA001
        crc = 0xFFFF
        for b in data:
            crc ^= b
            for _ in range(8):
                if crc & 0x01:
                    crc = (crc >> 1) ^ 0xA001
                else:
                    crc >>= 1
        return crc & 0xFFFF

    def _encode_read(self, addr, reg_addr, count=1):
        # 功能码 0x03：读保持寄存器
        msg = bytearray([addr, 0x03,
                         (reg_addr >> 8) & 0xff, reg_addr & 0xff,
                         (count >> 8) & 0xff, count & 0xff])
        crc = self._calc_crc16(msg)
        msg.append(crc & 0xff)
        msg.append((crc >> 8) & 0xff)
        return self._add_serial_bits(msg)

    def _encode_write_single(self, addr, reg_addr, value):
        # 功能码 0x06：写单个寄存器
        msg = bytearray([addr, 0x06,
                         (reg_addr >> 8) & 0xff, reg_addr & 0xff,
                         (value >> 8) & 0xff, value & 0xff])
        crc = self._calc_crc16(msg)
        msg.append(crc & 0xff)
        msg.append((crc >> 8) & 0xff)
        return self._add_serial_bits(msg)

    def _decode_read_response(self, raw_data, reg_count=1):
        # 应答帧：地址(1) + 0x03(1) + 字节数(1) + 数据(N*2) + CRC(2)
        expected_bytes = 5 + reg_count * 2
        payload = self._extract_serial_bits(raw_data, expected_bytes)
        if payload is None:
            return None
        # 校验CRC
        recv_crc = payload[-2] | (payload[-1] << 8)
        calc_crc = self._calc_crc16(payload[:-2])
        if recv_crc != calc_crc:
            return None
        # 提取寄存器值（16位大端）
        values = []
        for i in range(reg_count):
            offset = 3 + i * 2
            values.append((payload[offset] << 8) | payload[offset + 1])
        return values

    def _decode_write_response(self, raw_data):
        # 写应答和请求帧格式相同，共8字节
        payload = self._extract_serial_bits(raw_data, 8)
        if payload is None:
            return None
        recv_crc = payload[-2] | (payload[-1] << 8)
        calc_crc = self._calc_crc16(payload[:-2])
        if recv_crc != calc_crc:
            return None
        return True

    # ----------------------------------------------------------------
    # 寄存器读写接口
    # ----------------------------------------------------------------
    def reg_read(self, addr, reg_addr):
        msg = self._encode_read(addr, reg_addr, 1)
        # 应答总字节数：8字节Modbus帧 → 8*10=80bit → 10字节raw数据
        read_raw_len = 7
        params = self.tmcuart_send_cmd.send([self.oid, msg, read_raw_len])
        values = self._decode_read_response(params['read'], 1)
        if values is None:
            return {'data': None, '#receive_time': params['#receive_time']}
        return {'data': values[0], '#receive_time': params['#receive_time']}

    def reg_write(self, addr, reg_addr, value, print_time=None):
        minclock = 0
        if print_time is not None:
            minclock = self.mcu.print_time_to_clock(print_time)
        msg = self._encode_write_single(addr, reg_addr, value)
        # 写应答也是8字节 → 10字节raw数据
        self.tmcuart_send_cmd.send([self.oid, msg, 10], minclock=minclock)

    def get_mcu(self):
        return self.mcu

# Lookup a (possibly shared) lyx uart
def lookup_lyx_uart_bitbang(config):
    ppins = config.get_printer().lookup_object("pins")
    rx_pin_params = ppins.lookup_pin(config.get('uart_pin'),
                                     share_type="lyx_uart_rx")
    tx_pin_desc = config.get('tx_pin', None)
    if tx_pin_desc is None:
        tx_pin_params = rx_pin_params
    else:
        tx_pin_params = ppins.lookup_pin(tx_pin_desc, share_type="lyx_uart_tx")
    if rx_pin_params['chip'] is not tx_pin_params['chip']:
        raise ppins.error("LYX uart rx and tx pins must be on the same mcu")

    addr = config.getint('uart_address', 1, minval=1, maxval=247)
    mcu_uart = rx_pin_params.get('class')
    if mcu_uart is None:
        mcu_uart = MCU_LYX_uart_bitbang(rx_pin_params, tx_pin_params)
        rx_pin_params['class'] = mcu_uart
    mcu_uart.register_instance(rx_pin_params, tx_pin_params, addr)
    return addr, mcu_uart

######################################################################
# High-level UART wrapper
######################################################################

class MCU_LYX_uart:
    def __init__(self, config, name_to_reg, fields):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.name_to_reg = name_to_reg
        self.fields = fields
        self.addr, self.mcu_uart = lookup_lyx_uart_bitbang(config)
        self.mutex = self.mcu_uart.mutex
        # 寄存器名→地址映射表
        self.name_to_reg = name_to_reg

    def get_fields(self):
        return self.fields

    def _do_get_register(self, reg_name):
        reg = self.name_to_reg[reg_name]
        if self.printer.get_start_args().get('debugoutput') is not None:
            return {'data': 0, '#receive_time': 0.}
        for retry in range(5):
            val = self.mcu_uart.reg_read(self.addr, reg)
            if val['data'] is not None:
                return val
        raise self.printer.command_error(
            "Unable to read lyx uart '%s' register %s" % (self.name, reg_name))

    def get_register_raw(self, reg_name):
        with self.mutex:
            return self._do_get_register(reg_name)

    def get_register(self, reg_name):
        return self.get_register_raw(reg_name)['data']

    def set_register(self, reg_name, val, print_time=None):
        reg = self.name_to_reg[reg_name]
        val = int(val) & 0xFFFF  # LYX registers are 16-bit
        if self.printer.get_start_args().get('debugoutput') is not None:
            return
        with self.mutex:
            for retry in range(5):
                self.mcu_uart.reg_write(self.addr, reg, val, print_time)
                # Verify write
                readback = self.mcu_uart.reg_read(self.addr, reg)
                if readback['data'] == val:
                    return
        raise self.printer.command_error(
            "Unable to write lyx uart '%s' register %s" % (self.name, reg_name))

    def get_mcu(self):
        return self.mcu_uart.get_mcu()

    def get_lyx_frequency(self):
        return 40000.
