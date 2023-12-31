# Support for "serial bridge"
#
# Copyright (C) 2019-2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, re

QUERY_TIME = 0.2

class SerialBridge:
    def __init__(self, config):
        self.mcus = {}
        self.configs = []
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object("gcode")
        self.gcode.register_command("SERIAL_BRIDGE_SEND",
            self.cmd_SERIAL_BRIDGE_SEND,
            desc="Send a message to a uart bridge")
        self.gcode.register_command("SERIAL_BRIDGE_LIST_CONFIGS",
            self.cmd_SERIAL_BRIDGE_LIST_CONFIGS,
            desc="List Available serial configs")
        self.gcode.register_command("SERIAL_BRIDGE_LIST_BRIDGES",
            self.cmd_SERIAL_BRIDGE_LIST_BRIDGES,
            desc="List current bridges")
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        self.printer.register_event_handler("klippy:disconnect",
            self.handle_disconnect)
        self.bridges = {}

    def handle_ready(self):
        self._ready = True

        self.mcus = self.printer.lookup_objects('mcu')

        self.configs = []
        for n, mcu in self.mcus:
            constants = mcu.get_constants()
            configs= (
                ["%s=%s" % (k, v) for k,v in constants.items() \
                    if k.startswith("SERIAL_BRIDGE_CONFIG")])

            self.configs.extend(configs)
            logging.info("Serial bridge: available configs for %s: " % (n)
                 + ", ".join(configs))

    def handle_disconnect(self):
        pass

    def setup_bridge(self, bridge):
        self.bridges[bridge.name.split()[-1]] = bridge

    def cmd_SERIAL_BRIDGE_LIST_CONFIGS(self, gcmd):
        gcmd.respond_info((", ".join(self.configs)))

    def cmd_SERIAL_BRIDGE_LIST_BRIDGES(self, gcmd):
        gcmd.respond_info((", ".join(self.bridges.keys())))

    def cmd_SERIAL_BRIDGE_SEND(self, gcmd):
        text = gcmd.get("TEXT")
        bridge = gcmd.get("BRIDGE")

        if not bridge:
            gcmd.respond_info("BRIDGE is required")
            return

        if bridge not in self.bridges:
            gcmd.respond_info("BRIDGE not found")
            return

        self.bridges[bridge].send_serial(
            self.perform_replacement(gcmd.get("TEXT")))

    def get_configs(self):
        return self.configs

    def perform_replacement(self, input_string):
        # Find all occurrences of "\x" followed by two hexadecimal digits
        hex_matches = re.finditer(r'\\x([0-9a-fA-F]{2})', input_string)

        # Replace each matched hex sequence with its corresponding bytes
        replaced_bytes = bytearray()
        last_index = 0

        for match in hex_matches:
            hex_value = match.group(1)
            byte_value = bytearray.fromhex(hex_value)
            replaced_bytes.extend(byte_value)
            last_index = match.end()

        replaced_bytes.extend(input_string[last_index:].encode('utf-8'))

        return replaced_bytes

class PrinterSerialBridge:
    def __init__(self, config):
        self.callbacks = []
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.eol = config.get('eol', default='\n')
        self._ready = False
        self.baud = config.getint("baud", 115200)
        self.serial_config = config.getint("config", 4)
        self._logging = config.getboolean("logging", False)

        self.reactor = self.printer.get_reactor()
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        self.printer.register_event_handler("klippy:disconnect",
            self.handle_disconnect)

        ppins = self.printer.lookup_object("pins")
        pin_params = ppins.lookup_pin(config.get("tx_pin"))
        rx_pin_params = ppins.lookup_pin(config.get("rx_pin"))
        self.mcu = pin_params['chip']
        self.oid = self.mcu.create_oid()
        self.mcu.register_config_callback(self.build_config)

        self.input_buffer = ""

        self.serial_bridge = self.printer.load_object(config, "serial_bridge")
        self.serial_bridge.setup_bridge(self)

    def register_callback(self, callback):
        self.callbacks.append(callback)

    def chunkstring(self, msg, length):
        return (msg[0+i:length+i] for i in range(0, len(msg), length))

    def send_text(self, msg):
        self.send_serial(msg.encode('utf-8'))

    def send_serial(self, msg):
        if not self._ready:
            self.warn("Can't send message in a disconnected state")
            return

        chunks = self.chunkstring(
            msg + self.serial_bridge.perform_replacement(self.eol), 40)
        for chunk in chunks:
            byte_debug = ' '.join(['0x{:02x}'.format(byte) for byte in chunk])
            self.log("Sending message: " + byte_debug)
            self.serial_bridge_send_cmd.send([self.oid, chunk, 4])

    def build_config(self):
        rest_ticks = self.mcu.seconds_to_clock(QUERY_TIME)
        clock = self.mcu.get_query_slot(self.oid)
        self.mcu.add_config_cmd(
            "command_config_serial_bridge oid=%d clock=%d rest_ticks=%d "\
                "config=%d baud=%d" % (
                    self.oid, clock, rest_ticks, self.serial_config, self.baud
                ))

        cmd_queue = self.mcu.alloc_command_queue()

        self.mcu.register_response(self._handle_serial_bridge_response,
            "serial_bridge_response", self.oid)
        self.serial_bridge_send_cmd = self.mcu.lookup_command(
            "serial_bridge_send oid=%c text=%*s",
            cq=cmd_queue)

    def _handle_serial_bridge_response(self, params):
        data = params["text"]

        data = bytearray(data)

        for callback in self.callbacks:
            callback(data)

    def handle_ready(self):
        self.log("Ready")
        self._ready = True

    def handle_disconnect(self):
        self._ready = False

    def log(self, msg, *args, **kwargs):
        if self._logging:
            logging.info("SERIAL BRIDGE %s: " % (self.name) + str(msg) )

    def warn(self, msg, *args, **kwargs):
        logging.warning("SERIAL BRIDGE %s: " % (self.name) + str(msg))

def load_config(config):
    return SerialBridge(config)

def load_config_prefix(config):
    return PrinterSerialBridge(config)
