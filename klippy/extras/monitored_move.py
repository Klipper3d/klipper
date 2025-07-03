# Módulo para movimientos G1 con monitoreo de endstops
#
# Copyright (C) 2024 MicroLay
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging

class MonitoredMove:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.printer.register_event_handler("klippy:connect", self._handle_connect)
        self.toolhead = None
        self.phoming = None
        self.gcode.register_command('MONITORED_G1', self.cmd_MONITORED_G1, desc=self.cmd_MONITORED_G1_help)
        logging.info("MonitoredMove: Initialized")

    def _handle_connect(self):
        self.toolhead = self.printer.lookup_object('toolhead')
        self.phoming = self.printer.lookup_object('homing')

    def _get_endstop_by_stepper(self, stepper_name, gcmd):
        """Obtiene el endstop de un stepper buscándolo a través del rail de su eje."""
        kin = self.toolhead.get_kinematics()
        target_rail = None
        for rail in kin.rails:
            if rail.get_name() == stepper_name:
                target_rail = rail
                break
        
        if target_rail is None:
            rail_names = [r.get_name() for r in kin.rails]
            raise gcmd.error(f"No se pudo encontrar el rail para el stepper '{stepper_name}'. Rails disponibles: {rail_names}")
        
        endstops = target_rail.get_endstops()
        if not endstops:
            raise gcmd.error(f"El rail del stepper '{stepper_name}' no tiene endstops configurados.")
            
        return endstops[0][0]

    cmd_MONITORED_G1_help = "Realiza un movimiento G1 monitoreando el endstop de un stepper"
    def cmd_MONITORED_G1(self, gcmd):
        if not all([self.toolhead, self.phoming]):
            raise gcmd.error("MONITORED_G1: Sistema no listo.")

        stepper_name = gcmd.get('STEPPER')
        if not stepper_name:
            raise gcmd.error("El parámetro 'STEPPER' es requerido (ej: STEPPER=stepper_y).")

        x = gcmd.get_float('X', gcmd.get_command_parameters().get('X', None))
        y = gcmd.get_float('Y', gcmd.get_command_parameters().get('Y', None))
        z = gcmd.get_float('Z', gcmd.get_command_parameters().get('Z', None))
        f = gcmd.get_float('F', 60.0, above=0.)
        
        current_pos = self.toolhead.get_position()
        target_pos = list(current_pos)
        if x is not None: target_pos[0] = x
        if y is not None: target_pos[1] = y
        if z is not None: target_pos[2] = z
            
        endstop = self._get_endstop_by_stepper(stepper_name, gcmd)
        
        gcmd.respond_info(f"🎯 Movimiento monitoreado hacia Z={target_pos[2]:.3f}. Monitoreando '{stepper_name}'.")
        
        try:
            # Corrección: Pasar el endstop directamente a probing_move
            trigger_pos = self.phoming.probing_move(endstop, target_pos, f/60.0)
            
            gcmd.respond_info(f"🎯 Endstop activado en Z={trigger_pos[2]:.3f}")
            self.gcode.run_script_from_command(f"G92 Z{trigger_pos[2]}")
            gcmd.respond_info("✅ Movimiento detenido. Posición actualizada.")

        except self.printer.command_error as e:
            if "No trigger" in str(e):
                gcmd.respond_info("✅ Movimiento completado sin activación de endstop.")
                self.gcode.run_script_from_command(f"G92 Z{target_pos[2]}")
                gcmd.respond_info("✅ Posición actualizada al objetivo.")
            elif "triggered prior to movement" in str(e):
                raise gcmd.error("El endstop ya estaba activado antes de empezar el movimiento.")
            else:
                raise

def load_config(config):
    return MonitoredMove(config) 