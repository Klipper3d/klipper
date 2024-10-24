# Virtual SDCard print stat tracking
#
# Copyright (C) 2020  Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class PrintStats:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode_move = self.printer.load_object(config, 'gcode_move')
        self.toolhead = None
        self.reactor = self.printer.get_reactor()
        self.material_type = ''
        self.material_unit = ''
        self.reset()

        # Register events
        self.printer.register_event_handler("klippy:mcu_identify",
                                            self._init_delayed_stats)
        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            "SET_PRINT_STATS_INFO", self.cmd_SET_PRINT_STATS_INFO,
            desc=self.cmd_SET_PRINT_STATS_INFO_help)
    def _init_delayed_stats(self):
        if self.toolhead is None:
            self.toolhead = self.printer.lookup_object('toolhead')
        if self.toolhead:
            if self.toolhead.manufacturing_process == 'FDM':
                self.material_type = 'filament'
                self.material_unit = 'mm'
            elif self.toolhead.manufacturing_process in ('SLA', 'mSLA', 'DLP'):
                self.material_type = 'resin'
                self.material_unit = 'ml'
    def _update_filament_usage(self, eventtime):
        if self.toolhead.manufacturing_process == 'FDM':
            gc_status = self.gcode_move.get_status(eventtime)
            cur_epos = gc_status['position'].e
            self.material_used += (cur_epos - self.last_epos) \
                / gc_status['extrude_factor']
            self.last_epos = cur_epos
    def set_current_file(self, filename):
        self.reset()
        self.filename = filename
    def note_start(self):
        curtime = self.reactor.monotonic()
        if self.print_start_time is None:
            self.print_start_time = curtime
        elif self.last_pause_time is not None:
            # Update pause time duration
            pause_duration = curtime - self.last_pause_time
            self.prev_pause_duration += pause_duration
            self.last_pause_time = None
        # Reset last e-position
        gc_status = self.gcode_move.get_status(curtime)
        self.last_epos = gc_status['position'].e
        self.state = "printing"
        self.error_message = ""
    def note_pause(self):
        if self.last_pause_time is None:
            curtime = self.reactor.monotonic()
            self.last_pause_time = curtime
            # update filament usage
            self._update_filament_usage(curtime)
        if self.state != "error":
            self.state = "paused"
    def note_complete(self):
        self._note_finish("complete")
    def note_error(self, message):
        self._note_finish("error", message)
    def note_cancel(self):
        self._note_finish("cancelled")
    def _note_finish(self, state, error_message = ""):
        if self.print_start_time is None:
            return
        self.state = state
        self.error_message = error_message
        eventtime = self.reactor.monotonic()
        self.total_duration = eventtime - self.print_start_time
        if self.material_used < 0.0000001:
            # No positive extusion detected during print
            self.init_duration = self.total_duration - \
                self.prev_pause_duration
        self.print_start_time = None
    cmd_SET_PRINT_STATS_INFO_help = "Pass slicer info like layer act and " \
                                    "total to klipper"
    def cmd_SET_PRINT_STATS_INFO(self, gcmd):
        """
        Sets print stats info

        Syntax: SET_PRINT_STATS_INFO TOTAL_LAYER=[count]
                                     CURRENT_LAYER=[number]
                                     MATERIAL_NAME=["name"]
                                     MATERIAL_UNIT=[unit]
                                     MATERIAL_TOTAL=[total]
                                     CONSUME_MATERIAL=[amount]

        TOTAL_LAYER: Total layer count
        CURRENT_LAYER: Current printing layer number
        MATERIAL_NAME: Name of the material being used
        MATERIAL_UNIT: Material unit
        MATERIAL_TOTAL: Total material this print will consume
        CONSUME_MATERIAL: Consume material and increment the used material
        @param gcmd:
        @return:
        """
        total_layer = gcmd.get_int("TOTAL_LAYER", self.info_total_layer, \
                                   minval=0)
        current_layer = gcmd.get_int("CURRENT_LAYER", self.info_current_layer, \
                                     minval=0)
        material_name = gcmd.get("MATERIAL_NAME", None)
        material_unit = gcmd.get("MATERIAL_UNIT", None)
        material_total = gcmd.get_float("MATERIAL_TOTAL", -1., 0.)
        consume_material = gcmd.get_float("CONSUME_MATERIAL", 0., 0.)


        if total_layer == 0:
            self.info_total_layer = None
            self.info_current_layer = None
        elif total_layer != self.info_total_layer:
            self.info_total_layer = total_layer
            self.info_current_layer = 0

        if self.info_total_layer is not None and \
                current_layer is not None and \
                current_layer != self.info_current_layer:
            self.info_current_layer = min(current_layer, self.info_total_layer)

        if material_name:
            self.material_name = material_name

        if material_unit:
            self.material_unit = material_unit

        if material_total >= 0:
            self.material_total = material_total

        if consume_material > 0:
            self.last_material_used = consume_material
            self.material_used += consume_material
            if self.material_used > self.material_total:
                self.material_total = self.material_used

    def reset(self):
        self.filename = self.error_message = ""
        self.state = "standby"
        self.prev_pause_duration = self.last_epos = 0.
        self.material_used = self.total_duration = 0.
        self.material_total = self.last_material_used = 0.
        self.material_name = ''
        self.print_start_time = self.last_pause_time = None
        self.init_duration = 0.
        self.info_total_layer = None
        self.info_current_layer = None

        if self.toolhead is not None:
            self._init_delayed_stats()

    def get_status(self, eventtime):
        time_paused = self.prev_pause_duration
        if self.print_start_time is not None:
            if self.last_pause_time is not None:
                # Calculate the total time spent paused during the print
                time_paused += eventtime - self.last_pause_time
            else:
                # Accumulate filament if not paused
                self._update_filament_usage(eventtime)
            self.total_duration = eventtime - self.print_start_time
            if self.material_used < 0.0000001:
                # Track duration prior to extrusion
                self.init_duration = self.total_duration - time_paused
        print_duration = self.total_duration - self.init_duration - time_paused
        return {
            'filename': self.filename,
            'total_duration': self.total_duration,
            'print_duration': print_duration,
            'filament_used': self.material_used,  # Deprecated to: material_used
            'material_type': self.material_type,  # Material type, eg: filament
            'material_name': self.material_name,  # Material name by user
            'material_unit': self.material_unit,  # Material measure unit
            'material_total': self.material_total,  # Total material in a print
            'material_used': self.material_used,  # Amount of used material
            'state': self.state,
            'message': self.error_message,
            'info': {'total_layer': self.info_total_layer,
                     'current_layer': self.info_current_layer,
                     'last_material_used': self.last_material_used}
        }

def load_config(config):
    return PrintStats(config)
