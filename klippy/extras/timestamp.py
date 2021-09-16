# Timestamp for Klipper
#
# Copyright (C) 2021 Mike Gabriel <tlcbuilders@hotmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

from datetime import *

class timestamp:
    def __init__(self, config):
        self.printer = config.get_printer()

        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('TIMESTAMP', self.cmd_timestamp) 
        self.gcode.register_command('TIME', self.cmd_time) 
        self.gcode.register_command('HOUR', self.cmd_hour) 
        self.gcode.register_command('MINUTE', self.cmd_minute) 
        self.gcode.register_command('SECOND', self.cmd_second) 
#
    def cmd_timestamp(self, gcmd):
        # get current timestamp information
        cmd_timestamp = self.timestamp()  
        cmd_timestamp_stamp = cmd_timestamp[0]

        # put timestamp in string
        ts = "Timestamp: " + cmd_timestamp_stamp 

        # respond to console
        gcmd.respond_info(ts)
#
    def cmd_time(self, gcmd):
        # get current timestamp information
        cmd_time = self.timestamp()  
        cmd_time_time = cmd_time[1]

        # put time in string
        t = "Time: " + cmd_time_time 

        # respond to console
        gcmd.respond_info(t)
#
    def cmd_hour(self, gcmd):
        # get current timestamp information
        cmd_hour = self.timestamp()  
        cmd_hour_hour = cmd_hour[2]
        if len(str(cmd_hour_hour)) == 1:
            str_cmd_hour_hour = "0" + str(cmd_hour_hour)
        else:
            str_cmd_hour_hour = str(cmd_hour_hour)

        # put hour in string
        h = "Hour: " + str_cmd_hour_hour

        # respond to console
        gcmd.respond_info(h)

    def cmd_minute(self, gcmd):
        # get current timestamp information
        cmd_minute = self.timestamp()  
        cmd_minute_minute = cmd_minute[3] 
        if len(str(cmd_minute_minute)) == 1:
            str_cmd_minute_minute = "0" + str(cmd_minute_minute)
        else:
            str_cmd_minute_minute = str(cmd_minute_minute)
        # put minute in string
        m = "Minute: " + str_cmd_minute_minute

        # respond to console
        gcmd.respond_info(m)
#
    def cmd_second(self, gcmd):
        # get current timestamp information
        cmd_second = self.timestamp()  
        cmd_second_second = cmd_second[4] 
        if len(str(cmd_second_second)) == 1:
            str_cmd_second_second = "0" + str(cmd_second_second)
        else:
            str_cmd_second_second = str(cmd_second_second)

        # put second in string
        s = "Second: " + str_cmd_second_second

        # respond to console
        gcmd.respond_info(s)
#
    def timestamp(self):
        # get current timestamp
        time_stamp = datetime.now()
        stamp = str(time_stamp)

        # break the timestamp into separate components
        time = time_stamp.strftime("%X")
        hour = time_stamp.hour
        minute = time_stamp.minute
        second = time_stamp.second
        time = (stamp, time, hour, minute, second)
        return time 

    def get_status(self,eventtime):
        time = self.timestamp() 
        stamp, time, hour, minute, second = time[0:]  
        return {'Timestamp': stamp, 'Time': time, 'Hour': hour, 'Minute': minute, 'Second': second} 

def load_config(config):
    return timestamp(config)

