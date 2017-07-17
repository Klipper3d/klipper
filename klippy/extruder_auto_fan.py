# Printer fan support
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import fan
class ExtruderAutoFan:
    printer_ready=False
    
    def __init__(self,printer,config):
        self.activation_temp=float(config.get('activation_temp'))
        
        target_extruder=config.get('target_extruder')
        if not target_extruder or target_extruder=='extruder':
            target_extruder='extruder0'
        
        self.fan=fan.PrinterFan(printer,config)
        self.heater = printer.objects.get('extruder0').get_heater()
        self.toolhead=printer.objects.get('toolhead')
        
        printer.reactor.register_timer(self.check_temp,printer.reactor.NOW)
        
    def fan_on(self):
        print_time = self.toolhead.get_last_move_time()
        self.fan.set_speed(print_time,1.0)
    def fan_off(self):
        print_time = self.toolhead.get_last_move_time()
        self.fan.set_speed(print_time,0.0)

    def check_temp(self,_):
        if self.printer_ready:
            current_temp,target_temp=self.heater.get_temp()
            if current_temp>=self.activation_temp:
                self.fan_on()
            else:
                self.fan_off()


def add_printer_objects(printer, config):
    def add_extruder_fan(printer,config,extruder_fan_name):
        if config.has_section(extruder_fan_name):
            printer.add_object(extruder_fan_name, ExtruderAutoFan(printer, config.getsection(extruder_fan_name)))
            return True
        return False
            
    for i in range(99):
        if not add_extruder_fan(printer,config,'extruder_auto_fan%d' % (i,)) and not i:
            add_extruder_fan(printer,config,'extruder_auto_fan')

