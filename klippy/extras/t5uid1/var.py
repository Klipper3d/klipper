# Variable class
#
# Copyright (C) 2020  Desuuuu <contact@desuuuu.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import struct

TYPES_LEN = {
    'int16': 2,
    'uint16': 2,
    'int32': 4,
    'uint32': 4,
    'float': 4
}

TYPES_FMT = {
    'int16': '>h',
    'uint16': '>H',
    'int32': '>i',
    'uint32': '>I',
    'float': '>f'
}

class T5UID1_Var:
    def __init__(self, gcode_macro, input_context, output_context, config):
        self.printer = config.get_printer()

        self.gcode = self.printer.lookup_object('gcode')

        name_parts = config.get_name().split()
        if len(name_parts) != 2:
            raise config.error("Section name '%s' is not valid"
                               % (config.get_name(),))
        self.name = name_parts[1]

        self.type = config.get('type')
        if self.type not in ["input", "output"]:
            raise config.error("Invalid type '%s' in section '%s'"
                               % (self.type, config.get_name()))

        address = config.get('address')
        try:
            self.address = int(address, 16)
            if self.address < 0x1000 or self.address > 0xffff:
                raise
        except:
            raise config.error("Invalid address '%s' in section '%s'"
                               % (address, config.get_name()))

        data_types = list(TYPES_LEN.keys())
        data_types.append('str')
        if self.type == "input":
            data_types.append('none')

        self.data_type = config.get('data_type')
        if (self.data_type.startswith('array[')
            and self.data_type.endswith(']')
            and self.type == "output"
            and self.data_type != "str"):
            self.data_type = self.data_type[6:-1]
            self.array_len = config.getint('array_len', minval=2, maxval=32)
        else:
            self.array_len = 1
        if self.data_type not in data_types:
            raise config.error("Invalid data_type '%s' in section '%s'"
                               % (self.data_type, config.get_name()))

        if self.data_type == "none":
            self.data_len = 0
        elif self.data_type == "str":
            self.data_len = config.getint('data_len', minval=1, maxval=32)
        else:
            self.data_len = TYPES_LEN[self.data_type]

        if self.type == "input":
            self._template = gcode_macro.load_template(config, 'script')
            self._context = input_context
            self.run_as_gcode = config.getboolean('run_as_gcode', False)
        elif self.type == "output":
            self._template = gcode_macro.load_template(config, 'script')
            self._context = output_context
            self.run_as_gcode = False

    def data_received(self, data):
        if self.type != "input":
            raise Exception("not an input")

        context = self._template.create_template_context()
        context.update(self._context)

        if self.data_type != "none" and self.data_len != 0:
            received_len = len(data)
            if self.data_type == "str":
                buf = bytearray()
                for i in range(received_len):
                    if i >= self.data_len:
                        break
                    if data[i] == 0x00:
                        break
                    if (i + 1 < received_len
                        and data[i] == 0xff and data[i + 1] == 0xff):
                        break
                    buf.append(data[i])
                data = str(buf.decode('ascii'))
            elif received_len != self.data_len:
                raise ValueError("Expected %d bytes, got %d"
                                 % (self.data_len, received_len))
            else:
                data = struct.unpack(TYPES_FMT[self.data_type], data)[0]
            context.update({ 'data': data })

        result = self._template.render(context).strip()
        if self.run_as_gcode and len(result) > 0:
            self.gcode.run_script_from_command(result)

    def prepare_data(self):
        if self.type != "output":
            raise Exception("not an output")

        context = self._template.create_template_context()
        context.update(self._context)
        result = self._template.render(context)

        if self.data_type == "str":
            result = bytearray(result.replace('\n', ''), "ascii")
            target_len = self.data_len
            if target_len % 2 != 0:
                target_len += 1
            extra_bytes = len(result) - target_len
            if extra_bytes > 0:
                result = result[:target_len]
            elif extra_bytes < 0:
                result.extend([0] * abs(extra_bytes))
        else:
            parts = result.strip().split(',')
            result = bytearray()
            count = 0
            for part in parts:
                part = part.strip()
                if len(part) == 0:
                    continue
                if self.data_type == "float":
                    part = float(part)
                else:
                    part = int(part)
                result.extend(struct.pack(TYPES_FMT[self.data_type], part))
                count += 1
            if count != self.array_len:
                raise ValueError("Expected %d values, got %d"
                                 % (self.array_len, count))

        return result
