# Support for "neopixel" leds
#
# Copyright (C) 2019-2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

QUERY_TIME = .5

class PrinterSerialBridge:
    def __init__(self, config):
        # print(self)
        # print(config.mcu)

        self.printer = config.get_printer()

        self.printer.register_event_handler("klippy:ready", self.handle_ready)

        self.gcode = self.printer.lookup_object("gcode")
        self.gcode.register_command("SERIAL_BRIDGE_SEND", self.cmd_SERIAL_BRIDGE_SEND)

        self.gcode = self.printer.lookup_object("gcode")
        self.gcode.register_command("SERIAL_BRIDGE_STATS", self.cmd_SERIAL_BRIDGE_STATS)

        # name = config.get_name().split()[1]
        # self.mutex = self.printer.get_reactor().mutex()
        # # Configure neopixel
        ppins = self.printer.lookup_object("pins")
        pin_params = ppins.lookup_pin(config.get("pin"))
        self.mcu = pin_params['chip']
        self.oid = self.mcu.create_oid()
        # # self.pin = pin_params['pin']
        self.mcu.register_config_callback(self.build_config)
        # formats = {v: v for v in ["RGB", "GRB", "RGBW", "GRBW"]}
        # self.color_order = config.getchoice("color_order", formats, "GRB")
        # elem_size = len(self.color_order)
        # self.chain_count = config.getint('chain_count', 1, minval=1,
        #                                  maxval=MAX_MCU_SIZE//elem_size)
        # self.neopixel_update_cmd = self.neopixel_send_cmd = None
        # # Initial color
        # self.color_data = bytearray(self.chain_count * elem_size)
        # red = config.getfloat('initial_RED', 0., minval=0., maxval=1.)
        # green = config.getfloat('initial_GREEN', 0., minval=0., maxval=1.)
        # blue = config.getfloat('initial_BLUE', 0., minval=0., maxval=1.)
        # white = 0
        # if elem_size == 4:
        #     white = config.getfloat('initial_WHITE', 0., minval=0., maxval=1.)

        # reactor = self.printer.get_reactor()
        # eventtime = starttime = reactor.monotonic()

        # self.old_color_data = bytearray([d ^ 1 for d in self.color_data])
        # # Register commands
        # self.printer.register_event_handler("klippy:connect", self.send_data)
        # gcode = self.printer.lookup_object('gcode')
        # gcode.register_mux_command("SET_LED", "LED", name, self.cmd_SET_LED,
        #
        #                          desc=self.cmd_SET_LED_help)
        pass

    def cmd_SERIAL_BRIDGE_SEND(self, gcmd):
        self.serial_bridge_send_cmd.send([self.oid, gcmd.get("TEXT") + "\n"])
        print("SERIAL_BRIDGE_SEND" + gcmd.get("TEXT"))

    def cmd_SERIAL_BRIDGE_STATS(self, gcmd):
        self.serial_bridge_stats_cmd.send()
        print("SERIAL_BRIDGE_STATS" + gcmd.get("TEXT"))

    def build_config(self):
        print("config")

        # bmt = self.mcu.seconds_to_clock(BIT_MAX_TIME)
        # rmt = self.mcu.seconds_to_clock(RESET_MIN_TIME)
        rest_ticks = self.mcu.seconds_to_clock(QUERY_TIME)
        clock = self.mcu.get_query_slot(self.oid)
        self.mcu.add_config_cmd("command_config_serial_bridge oid=%d clock=%d rest_ticks=%d" % (self.oid, clock, rest_ticks))

        cmd_queue = self.mcu.alloc_command_queue()
        # self.neopixel_update_cmd = self.mcu.lookup_command(
        #     "serial_bridge line=%*s", cq=cmd_queue)

        self.mcu.register_response(self._handle_serial_bridge_response, "serial_bridge_response")
        self.serial_bridge_send_cmd = self.mcu.lookup_command(
            "serial_bridge_send oid=%c text=%*s",
            cq=cmd_queue)
        # self.serial_bridge_recv_cmd = self.mcu.lookup_query_command(
        #     "serial_bridge line=%*s", "serial_bridge_response resp=%s",
        #     cq=cmd_queue)
        #print(self.neopixel_update_cmd)

    def _handle_serial_bridge_response(self, params):
        print(params)
        pass
        # last_sequence = self.last_sequence
        # sequence = (last_sequence & ~0xffff) | params['sequence']
        # if sequence < last_sequence:
        #     sequence += 0x10000
        # self.last_sequence = sequence
        # raw_samples = self.raw_samples
        # if len(raw_samples) >= 300000:
        #     # Avoid filling up memory with too many samples
        #     return
        # raw_samples.append((sequence, params['data']))

    def handle_ready(self):
        #self.update_color_data()
        pass

    def update_color_data(self):
        resp = self.serial_bridge_recv_cmd.send("abc");
        print(resp["resp"])
        pass
    def send_data(self, print_time=None):
        old_data, new_data = self.old_color_data, self.color_data
        if new_data == old_data:
            return
        # Find the position of all changed bytes in this framebuffer
        diffs = [[i, 1] for i, (n, o) in enumerate(zip(new_data, old_data))
                 if n != o]
        # Batch together changes that are close to each other
        for i in range(len(diffs)-2, -1, -1):
            pos, count = diffs[i]
            nextpos, nextcount = diffs[i+1]
            if pos + 5 >= nextpos and nextcount < 16:
                diffs[i][1] = nextcount + (nextpos - pos)
                del diffs[i+1]
        # Transmit changes
        ucmd = self.neopixel_update_cmd.send
        for pos, count in diffs:git st
            ucmd([self.oid, pos, new_data[pos:pos+count]],
                 reqclock=BACKGROUND_PRIORITY_CLOCK)
        old_data[:] = new_data
        # Instruct mcu to update the LEDs
        minclock = 0
        if print_time is not None:
            minclock = self.mcu.print_time_to_clock(print_time)
        scmd = self.neopixel_send_cmd.send
        if self.printer.get_start_args().get('debugoutput') is not None:
            return
        for i in range(8):
            params = scmd([self.oid], minclock=minclock,
                          reqclock=BACKGROUND_PRIORITY_CLOCK)
            if params['success']:
                break
        else:
            logging.info("Neopixel update did not succeed")
    cmd_SET_LED_help = "Set the color of an LED"
    def cmd_SET_LED(self, gcmd):
        # Parse parameters
        red = gcmd.get_float('RED', 0., minval=0., maxval=1.)
        green = gcmd.get_float('GREEN', 0., minval=0., maxval=1.)
        blue = gcmd.get_float('BLUE', 0., minval=0., maxval=1.)
        white = gcmd.get_float('WHITE', 0., minval=0., maxval=1.)
        index = gcmd.get_int('INDEX', None, minval=1, maxval=self.chain_count)
        transmit = gcmd.get_int('TRANSMIT', 1)
        # Update and transmit data
        def reactor_bgfunc(print_time):
            with self.mutex:
                self.update_color_data(red, green, blue, white, index)
                if transmit:
                    self.send_data(print_time)
        def lookahead_bgfunc(print_time):
            reactor = self.printer.get_reactor()
            reactor.register_callback(lambda et: reactor_bgfunc(print_time))
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_lookahead_callback(lookahead_bgfunc)

def load_config_prefix(config):
    return PrinterSerialBridge(config)
