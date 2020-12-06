# MMU2 support.
#
# Copyright (C) 2018  Trevor Jones <trevorjones141@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import pins, mcu

FLASH_DELAY = 0.05


class MMU2:
    sr_pins = {"stepper_1_dir": 0, "stepper_1_enable": 1, # PULLEY/IDLER
               "stepper_2_dir": 2, "stepper_2_enable": 3, # SELECTOR
               "stepper_3_dir": 4, "stepper_3_enable": 5, # IDLER
               "green_led_1": 8, "red_led_1": 9,
               "green_led_2": 10, "red_led_2": 11,
               "green_led_3": 12, "red_led_3": 13,
               "green_led_4": 14, "red_led_4": 15,
               "green_led_5": 6, "red_led_5": 7}

    mmu_leds = [sr_pins["green_led_1"], sr_pins["red_led_1"],
                sr_pins["green_led_2"], sr_pins["red_led_2"],
                sr_pins["green_led_3"], sr_pins["red_led_3"],
                sr_pins["green_led_4"], sr_pins["red_led_4"],
                sr_pins["green_led_5"], sr_pins["red_led_5"]]

    mmu_steppers_enable = [sr_pins["stepper_1_enable"],
                           sr_pins["stepper_2_enable"],
                           sr_pins["stepper_3_enable"]]

    mmu_steppers_dir    = [sr_pins["stepper_1_dir"],
                           sr_pins["stepper_2_dir"],
                           sr_pins["stepper_3_dir"]]

    mmu_led_mask = 0b1111111111000000
    mmu_stepper_mask = 0b0000000000111111
    all_off = 0b0000000000000000
    all_on = 0b1111111111111111

    def __init__(self, config):
        logging.info("Initializing Klippy MMU2 Module")
        self._printer = config.get_printer()
        self._mcu_name = config.get('mcu', None)
        if self._mcu_name is None:
            raise config.error("mmu2 mcu not specified")
        self.reactor = self._printer.get_reactor()
        self._shift_register = None
        self._mcu = mcu.get_printer_mcu(self._printer, self._mcu_name)
        self._mcu.register_config_callback(self._build_config)
        # led start test
        self._rs_led = 0
        self._rs_led_loop = 0
        self._rs_led_loop_last = -1
        # dir
        self._steppers_move = [0.0, 0.0, 0.0]
        self._steppers = {  config.get('gear_stepper'):0,
                            config.get('selector_stepper', None):1,
                            config.get('idler_stepper'):2}
        self._oldir = [0, 0, 0]
        # printer objects
        self._printer.register_event_handler("klippy:ready", self.handle_ready)
        # Register commands
        self._gcode = self._printer.lookup_object('gcode')
        self._gcode.register_command('MMU_SET_LED',
                                    self.cmd_MMU_SET_LED)
        self._gcode.register_command('MMU_SET_STEPPER',
                                    self.cmd_MMU_SET_STEPPER)
        self._gcode.register_command('MMU_MOVE_STEPPER',
                                    self.cmd_MMU_MOVE_STEPPER)

    def _build_config(self):
        self._shift_register = self._printer.lookup_object("hc595 %s"
                                                            % self._mcu_name)

    def handle_ready(self):
        self._shift_register.write_bits(self.all_off)
        #self.reactor.register_timer(self._ready_led_sequence, self.reactor.NOW)
        self.reactor.register_timer(self._mmu_led_sequence, self.reactor.NOW)

    def _ready_led_sequence(self, time):
        self._shift_register.set_bit(self.mmu_leds[self._rs_led], 1)
        self._rs_led += 1
        if self._rs_led is 10:
            self.reactor.register_timer(self._clear_led,
                                                      (time + FLASH_DELAY * 5))
        return time + FLASH_DELAY if self._rs_led < 10 else self.reactor.NEVER

    def _clear_led(self, time):
        self._shift_register.clear_bits(self.mmu_led_mask)
        return self.reactor.NEVER

    def _mmu_led_sequence(self, time):
        if(self._rs_led_loop_last >= 0):
            self._shift_register.set_bit(self.mmu_leds[self._rs_led_loop_last],
                                         0)
        self._shift_register.set_bit(self.mmu_leds[self._rs_led], 1)
        self._rs_led_loop_last = self._rs_led
        if ((self._rs_led_loop % 2) == 0):
            self._rs_led += 1
        else:
            self._rs_led -= 1
        if self._rs_led == 10:
            self._rs_led_loop += 1
            self._rs_led -= 1
        if self._rs_led == -1:
            self._rs_led_loop += 1
            self._rs_led += 1
        if self._rs_led_loop == 3:
            self.reactor.register_timer(self._clear_led, (time + FLASH_DELAY))
        if self._rs_led_loop < 3:
            return time + FLASH_DELAY
        else:
            return self.reactor.NEVER

    def cmd_MMU_SET_LED(self, params):
        led = params.get_int('NUM')
        value = params.get_int('VALUE')
        if led < 0 or led > 9:
            raise self._printer.command_error("NUM must be between 0 and 9")
        if value < 0 or value > 1:
            raise self._printer.command_error("VALUE must be between 0 or 1")
        self._shift_register.set_bit(self.mmu_leds[led], value)

    def cmd_MMU_SET_STEPPER(self, params):
        stepper = params.get_int('NUM')
        stepper_dir = params.get_int('DIR', None)
        stepper_enable = params.get_int('ENABLE', None)
        if stepper < 0 or stepper > 3:
            raise self._printer.command_error("NUM must be between 0 and 3")
        if stepper_dir is not None:
            if stepper_dir < 0 or stepper_dir > 1:
                raise self._printer.command_error("DIR must be between 0 or 1")
            else:
                self._shift_register.set_bit(self.mmu_steppers_dir[stepper],
                                             stepper_dir)
        if stepper_enable is not None:
            if stepper_enable < 0 or stepper_enable > 1:
                raise self._printer.command_error(
                                               "ENABLE must be between 0 or 1")
            else:
                self._shift_register.set_bit(self.mmu_steppers_enable[stepper],
                                             (stepper_enable+1)%2)

    def cmd_MMU_MOVE_STEPPER(self, gcmd):
        cmd = gcmd.get_commandline().split(" ", 1)[1]
        stepper = gcmd.get('STEPPER')
        num = self._steppers[stepper]
        setpos = gcmd.get_float('SET_POSITION', None)
        if gcmd.get_float('MOVE', None) is not None:
            movepos = gcmd.get_float('MOVE')
            if movepos >= self._steppers_move[num] and self._oldir[num] == 0:
                self._gcode.run_script_from_command("M400")
                self._shift_register.set_bit(self.mmu_steppers_dir[num], 1)
                self._oldir[num] = 1
            elif movepos < self._steppers_move[num] and self._oldir[num] == 1:
                self._gcode.run_script_from_command("M400")
                self._shift_register.set_bit(self.mmu_steppers_dir[num], 0)
                self._oldir[num] = 0
            self._steppers_move[num] = movepos
        self._gcode.run_script_from_command("MANUAL_STEPPER %s"%cmd)
        if setpos is not None:
            self._steppers_move[num] = setpos

def load_config(config):
    return MMU2(config)
