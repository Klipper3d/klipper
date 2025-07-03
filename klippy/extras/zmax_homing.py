# Módulo para homing del eje Z hacia Zmax usando el endstop de Y
#
# Copyright (C) 2024 MicroLay
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging

class YEndstopWrapper:
    """Wrapper para el endstop de Y que es compatible con probing_move()"""
    def __init__(self, y_endstop):
        self.y_endstop = y_endstop
        # Delegar métodos del endstop original
        self.get_mcu = y_endstop.get_mcu
        self.add_stepper = y_endstop.add_stepper
        self.get_steppers = y_endstop.get_steppers
        self.home_start = y_endstop.home_start
        self.home_wait = y_endstop.home_wait
        self.query_endstop = y_endstop.query_endstop

class ZMaxHomingAlt:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        
        # Registrar para eventos
        self.printer.register_event_handler("klippy:connect", self._handle_connect)
        
        # Variables
        self.toolhead = None
        self.y_endstop = None
        self.y_endstop_wrapper = None
        self.z_stepper = None
        self.phoming = None
        
        # Configuración
        self.speed = config.getfloat('speed', 5.0, above=0.)
        self.second_speed = config.getfloat('second_speed', 2.0, above=0.)
        self.retract_dist = config.getfloat('retract_dist', 3.0, minval=0.)
        self.retract_speed = config.getfloat('retract_speed', 3.0, above=0.)
        self.final_retract = config.getboolean('final_retract', False)
        
        # Registrar comandos
        self.gcode.register_command(
            'ZMAX_HOME',
            self.cmd_ZMAX_HOME,
            desc=self.cmd_ZMAX_HOME_help
        )
        
        logging.info(f"ZMaxHoming: Initialized with speeds - homing:{self.speed} mm/s, second:{self.second_speed} mm/s")
    
    def _handle_connect(self):
        # Obtener referencias necesarias
        self.toolhead = self.printer.lookup_object('toolhead')
        self.phoming = self.printer.lookup_object('homing')
        
        # Obtener el endstop de Y usando la forma estándar
        kin = self.toolhead.get_kinematics()
        if hasattr(kin, 'rails'):
            for rail in kin.rails:
                if rail.get_name() == "stepper_y":
                    endstops = rail.get_endstops()
                    if endstops:
                        self.y_endstop = endstops[0][0]
                        # Crear wrapper compatible con probing_move()
                        self.y_endstop_wrapper = YEndstopWrapper(self.y_endstop)
                        logging.info(f"ZMaxHoming: Using Y endstop for Z-max homing")
                        break
            
            # Obtener el stepper Z
            for rail in kin.rails:
                if rail.get_name() == "stepper_z":
                    self.z_stepper = rail
                    z_max_cfg = self.z_stepper.get_range()[1]
                    logging.info(f"ZMaxHoming: Z-max position from config: {z_max_cfg}mm")
                    break
        
        if self.y_endstop is None:
            raise self.printer.config_error("No se encontró el endstop de Y")
        if self.z_stepper is None:
            raise self.printer.config_error("No se encontró el stepper Z")
    
    def _continuous_probe_move(self, target_pos, speed):
        """
        Realiza un movimiento continuo usando la función oficial probing_move() de Klipper.
        Esta es la misma función que usan todos los módulos de sondeo oficiales.
        """
        try:
            trigger_pos = self.phoming.probing_move(self.y_endstop_wrapper, target_pos, speed)
            # Mensaje en el momento exacto de detección
            gcode = self.printer.lookup_object('gcode')
            gcode.respond_info(f"🎯 Y-endstop ACTIVADO en Z={trigger_pos[2]:.3f}mm")
            return trigger_pos
        except self.printer.command_error as e:
            if "No trigger" in str(e):
                raise self.printer.command_error(
                    f"El endstop Y no se activó durante el movimiento hacia Z={target_pos[2]:.3f}. "
                    "Verifica que la plataforma pueda alcanzar el final de carrera.")
            raise

    def _measure_platform_travel(self, gcmd, start_pos, z_max_cfg):
        """
        Mide el recorrido total de la plataforma desde Z-max hasta Z-min + position_endstop
        """
        try:
            gcmd.respond_info("=== INICIANDO MEDICIÓN DE RECORRIDO ===")
            
            # Obtener configuración del stepper Z
            z_min_cfg = self.z_stepper.get_range()[0]
            
            # Obtener position_endstop del stepper Z
            z_endstop_pos = 0.0  # Valor por defecto
            try:
                # Buscar en la configuración del stepper Z
                kin = self.toolhead.get_kinematics()
                if hasattr(kin, 'rails'):
                    for rail in kin.rails:
                        if rail.get_name() == "stepper_z":
                            endstops = rail.get_endstops()
                            if endstops:
                                # Obtener position_endstop del endstop Z
                                z_endstop = endstops[0][0]
                                if hasattr(z_endstop, 'position_endstop'):
                                    z_endstop_pos = z_endstop.position_endstop
                                    break
            except Exception as e:
                gcmd.respond_info(f"Advertencia: No se pudo obtener position_endstop: {e}")
                
            gcmd.respond_info(f"Configuración Z: min={z_min_cfg:.3f}, max={z_max_cfg:.3f}, position_endstop={z_endstop_pos:.3f}")
            
            # Posición inicial (Z-max)
            start_z = start_pos[2]
            gcmd.respond_info(f"Posición inicial (Z-max): {start_z:.3f}mm")
            
            # Mover a Z-min
            gcmd.respond_info("Moviendo a Z-min...")
            min_pos = list(start_pos)
            min_pos[2] = z_min_cfg
            self.toolhead.move(min_pos, self.speed)
            self.toolhead.wait_moves()
            gcmd.respond_info(f"En Z-min: {z_min_cfg:.3f}mm")
            
            # Mover posición adicional según position_endstop
            target_z = z_min_cfg - abs(z_endstop_pos)
            gcmd.respond_info(f"Bajando {abs(z_endstop_pos):.3f}mm adicionales (position_endstop)...")
            final_pos = list(min_pos)
            final_pos[2] = target_z
            self.toolhead.move(final_pos, self.speed)
            self.toolhead.wait_moves()
            
            # Calcular recorrido total
            total_travel = start_z - target_z
            config_travel = z_max_cfg - (z_min_cfg - abs(z_endstop_pos))
            
            gcmd.respond_info("=== RESULTADOS DE MEDICIÓN ===")
            gcmd.respond_info(f"📏 Recorrido REAL medido: {total_travel:.3f}mm")
            gcmd.respond_info(f"📐 Recorrido CONFIG teórico: {config_travel:.3f}mm")
            gcmd.respond_info(f"📊 Diferencia: {abs(total_travel - config_travel):.3f}mm")
            
            if abs(total_travel - config_travel) > 0.5:
                gcmd.respond_info("⚠️  ATENCIÓN: Diferencia > 0.5mm - Revisar montaje/configuración")
            else:
                gcmd.respond_info("✅ Medición dentro de tolerancia")
                
        except Exception as e:
            gcmd.respond_info(f"❌ Error durante medición: {e}")

    cmd_ZMAX_HOME_help = "Realiza el homing del eje Z hacia Zmax usando el endstop de Y. Usa MEASURE=1 para medir el recorrido total de la plataforma."
    def cmd_ZMAX_HOME(self, gcmd):
        if not all([self.toolhead, self.y_endstop_wrapper, self.z_stepper, self.phoming]):
            raise gcmd.error("ZMAX_HOME: Sistema no inicializado correctamente")

        # Obtener parámetros
        speed = gcmd.get_float('SPEED', self.speed, above=0.)
        second_speed = gcmd.get_float('SECOND_SPEED', self.second_speed, above=0.)
        retract_dist = gcmd.get_float('RETRACT_DIST', self.retract_dist, minval=0.)
        final_retract = gcmd.get_int('FINAL_RETRACT', -1)
        if final_retract == -1:
            final_retract = self.final_retract
        else:
            final_retract = bool(final_retract)
        measure_travel = gcmd.get_int('MEASURE', 0, minval=0, maxval=1)

# gcmd.respond_info(f"ZMAX_HOME: Iniciando movimiento continuo (velocidad: {speed} mm/s)")

        # Obtener configuración Z
        z_max_cfg = self.z_stepper.get_range()[1]

        # Verificar estado de Z
        curtime = self.printer.get_reactor().monotonic()
        z_homed = 'z' in self.toolhead.get_status(curtime)['homed_axes']
        
        # Detectar si ya estamos en Z-max desde el inicio
        if not z_homed and self.y_endstop.query_endstop(0):
            pos = self.toolhead.get_position()
            pos[2] = z_max_cfg  # Establecer directamente en Z-max
            self.toolhead.set_position(pos, homing_axes=('z',))
            z_homed = True  # Marcar como homeado
        elif not z_homed:
            # Homear Z temporalmente en posición actual
            pos = self.toolhead.get_position()
            self.toolhead.set_position(pos, homing_axes=('z',))

        # Configurar modo absoluto
        gcode_move = self.printer.lookup_object('gcode_move')
        gcode_state = gcode_move.get_status()['absolute_coordinates']
        self.gcode.run_script_from_command("G90")

        try:
            # Variables para detección de pérdida de pasos
            initial_z_pos = None
            detected_z_pos = None
            
            # Obtener posición inicial
            current_pos = self.toolhead.get_position()
            initial_z_pos = current_pos[2]
            
            # Detectar si el endstop ya está activado al inicio
            endstop_initially_triggered = self.y_endstop.query_endstop(0)
            
            # Solo considerar que está "ya activado" si estamos cerca de Z-max
            if endstop_initially_triggered and current_pos[2] > (z_max_cfg - 10.0):
                # Ya estamos en contacto con el endstop cerca de Z-max
                gcmd.respond_info(f"🎯 Y-endstop YA ACTIVADO en Z={current_pos[2]:.3f}mm")
                detected_z_pos = current_pos[2]
                
            else:
                # Endstop no activado - hacer búsqueda rápida primero
                
                # --- Búsqueda rápida continua ---
                movepos = list(current_pos)
                movepos[2] = z_max_cfg
                
                try:
                    first_trigger_pos = self._continuous_probe_move(movepos, speed)
                    detected_z_pos = first_trigger_pos[2]  # Guardar posición REAL donde se detectó
                except self.printer.command_error as e:
                    if "Probe triggered prior to movement" in str(e):
                        # El endstop se activó inmediatamente al iniciar el movimiento
                        gcmd.respond_info(f"🎯 Y-endstop YA ACTIVADO en Z={current_pos[2]:.3f}mm")
                        detected_z_pos = current_pos[2]
                    else:
                        raise

            # --- Calcular pérdida de pasos ANTES de cambiar posición ---
            if initial_z_pos is not None and detected_z_pos is not None:
                expected_travel = z_max_cfg - initial_z_pos
                actual_travel = detected_z_pos - initial_z_pos
                step_loss = actual_travel - expected_travel
                
                if abs(step_loss) > 0.01:
                    if step_loss < 0:
                        gcmd.respond_info(f"⚠️ PÉRDIDA DE PASOS: {step_loss:.3f}mm (perdió pasos BAJANDO)")
                    else:
                        gcmd.respond_info(f"⚠️ PÉRDIDA DE PASOS: +{step_loss:.3f}mm (perdió pasos SUBIENDO)")
                else:
                    gcmd.respond_info(f"✅ Sin pérdida de pasos (+{step_loss:.3f}mm)")

            # --- "Engañar" a Klipper estableciendo Z=Z-max para poder retraer ---
            fake_pos = self.toolhead.get_position()
            fake_pos[2] = z_max_cfg
            self.toolhead.set_position(fake_pos)

            # --- Retracción desde Z-max ---
            retract_pos = list(fake_pos)
            retract_pos[2] = z_max_cfg - retract_dist
            self.toolhead.move(retract_pos, self.retract_speed)
            self.toolhead.wait_moves()
            
            # Verificar que el endstop se liberó
            if self.y_endstop.query_endstop(0):
                raise gcmd.error("Endstop Y sigue activado después de retracción. Homing fallido.")

            # --- Búsqueda fina continua ---
            movepos = list(retract_pos)
            movepos[2] = z_max_cfg
            
            try:
                final_trigger_pos = self._continuous_probe_move(movepos, second_speed)
            except self.printer.command_error as e:
                if "Probe triggered prior to movement" in str(e):
                    # El endstop se activó inmediatamente, usar posición actual
                    current_pos = self.toolhead.get_position()
                    gcmd.respond_info(f"🎯 Y-endstop ACTIVADO en Z={current_pos[2]:.3f}mm")
                    final_trigger_pos = current_pos
                else:
                    raise

            # --- Establecer posición final en Z-max ---
            final_pos = list(final_trigger_pos)
            final_pos[2] = z_max_cfg

            if not z_homed:
                self.toolhead.set_position(final_pos, homing_axes=('z',))
            else:
                self.toolhead.set_position(final_pos)
                
            gcmd.respond_info(f"✅ Posición Z establecida en {final_pos[2]:.3f}mm")
            
            # Medición silenciosa de pérdida de pasos (sin reportar)

            # --- Retracción final opcional ---
            if final_retract and retract_dist > 0:
                retract_pos = list(final_pos)
                retract_pos[2] -= retract_dist
                z_min_cfg = self.z_stepper.get_range()[0]
                if retract_pos[2] < z_min_cfg:
                    retract_pos[2] = z_min_cfg
                self.toolhead.move(retract_pos, self.retract_speed)
                self.toolhead.wait_moves()

            # --- Medición del recorrido total (opcional) ---
            if measure_travel:
                self._measure_platform_travel(gcmd, final_pos, z_max_cfg)

        finally:
            # Restaurar estado Gcode
            if not gcode_state:
                self.gcode.run_script_from_command("G91")

def load_config(config):
    return ZMaxHomingAlt(config)