# LCD demo code for the hd44780 display
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

BACKGROUND_PRIORITY_CLOCK = 0x7fffffff00000000

class HD44780:
    def __init__(self, config):
        self.printer = config.get_printer()
        # pin config
        ppins = self.printer.lookup_object('pins')
        pins = [ppins.lookup_pin('digital_out', config.get(name + '_pin'))
                for name in ['rs', 'e', 'd4', 'd5', 'd6', 'd7']]
        mcu = None
        for pin_params in pins:
            if mcu is not None and pin_params['chip'] != mcu:
                raise ppins.error("hd44780 all pins must be on same mcu")
            mcu = pin_params['chip']
            if pin_params['invert']:
                raise pins.error("hd44780 can not invert pin")
        self.mcu = mcu
        self.mcu.add_config_cmd(
            "config_hd44780 oid=%d rs_pin=%s e_pin=%s"
            " d4_pin=%s d5_pin=%s d6_pin=%s d7_pin=%s" % (
                self.mcu.create_oid(), pins[0]['pin'], pins[1]['pin'],
                pins[2]['pin'], pins[3]['pin'], pins[4]['pin'], pins[5]['pin']))
        self.mcu.add_config_object(self)
        self.cmd_queue = self.mcu.alloc_command_queue()
        self.send_data_cmd = self.send_cmds_cmd = None
        # framebuffers
        self.text_framebuffers = [
            [bytearray([' ']*20), bytearray([0x00]*20), 0x80, 0],
            [bytearray([' ']*20), bytearray([0x00]*20), 0x80, 0x40],
            [bytearray([' ']*20), bytearray([0x00]*20), 0x80, 20],
            [bytearray([' ']*20), bytearray([0x00]*20), 0x80, 0x40 + 20]]
        self.sprite_framebuffer = [
            [bytearray([0x00]*40), bytearray([0xff]*40), 0x40, 0]]
        self.graphics_framebuffer = [
            [bytearray([0x00]*16), bytearray([0xff]*16), 0x00, 0] * 64]
        self.pending_framebuffers = (
            self.sprite_framebuffer + self.text_framebuffers)
    def build_config(self):
        self.send_cmds_cmd = self.mcu.lookup_command(
            "hd44780_send_cmds cmds=%*s")
        self.send_data_cmd = self.mcu.lookup_command(
            "hd44780_send_data data=%*s")
    def send(self, cmd_type, data):
        self.mcu.send(cmd_type.encode(data),
                      reqclock=BACKGROUND_PRIORITY_CLOCK, cq=self.cmd_queue)
    def flush(self):
        for fb in self.pending_framebuffers:
            new_data, old_data, cmd, offset = fb
            diffs = [[i, 1] for i, (nd, od) in enumerate(zip(new_data, old_data))
                     if nd != od]
            for i in range(len(diffs)-2, -1, -1):
                if diffs[i][0] + 4 >= diffs[i+1][0] and diffs[i+1][1] < 16:
                    diffs[i][1] = diffs[i+1][1] + diffs[i+1][0] - diffs[i][0]
                    del diffs[i+1]
            for pos, count in diffs:
                self.send(self.send_cmds_cmd, [cmd + offset + pos])
                self.send(self.send_data_cmd, new_data[pos:pos+count])
            fb[1] = new_data
        del self.pending_framebuffers[:]
    def init(self):
        reactor = self.printer.get_reactor()
        for cmd in [0x33, 0x33, 0x33, 0x22]:
            self.mcu.send(self.send_cmds_cmd.encode([cmd]), cq=self.cmd_queue)
            reactor.pause(reactor.monotonic() + .100)
        self.send(self.send_cmds_cmd, [0x08, 0x06, 0x10, 0x0c])
        self.flush()

class PrinterLCD:
    def __init__(self, config):
        printer = config.get_printer()
        self.lcd_chip = HD44780(config)
        # work timer
        self.reactor = printer.get_reactor()
        self.work_timer = self.reactor.register_timer(self.work_event)
        self.init_timer = self.reactor.register_timer(self.init_event)
        self.position = 0
    def printer_state(self, state):
        if state == 'ready':
            self.reactor.update_timer(self.init_timer, self.reactor.NOW)
    def init_event(self, eventtime):
        self.reactor.unregister_timer(self.init_timer)
        self.init_timer = None
        self.lcd_chip.init()
        self.reactor.update_timer(self.work_timer, self.reactor.NOW)
        return self.reactor.NEVER
    def update_framebuffer(self, fb, pos, data):
        new_fb_data = fb[0]
        len_fb_data = len(new_fb_data)
        if new_fb_data is not fb[1]:
            # Some changes are already pending to this buffer
            new_fb_data[pos:pos+len(data)] = data
        else:
            new_fb_data = bytearray(fb[1])
            new_fb_data[pos:pos+len(data)] = data
            if new_fb_data == fb[1]:
                # No update
                return
            fb[0] = new_fb_data
            self.lcd_chip.pending_framebuffers.append(fb)
        if len(new_fb_data) > len_fb_data:
            del new_fb_data[len_fb_data:]
    def write_str(self, x, y, data):
        self.update_framebuffer(self.lcd_chip.text_framebuffers[y], x, data)
    def work_event(self, eventtime):
        x = self.position % 20
        y = self.position // 20
        self.write_str(x, y, " #")
        self.lcd_chip.flush()
        self.position = (self.position + 1) % 80
        return eventtime + .100

def load_config(config):
    return PrinterLCD(config)
