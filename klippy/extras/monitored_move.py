# Módulo para movimientos G1 con monitoreo de endstops
#
# Copyright (C) 2024 MicroLay
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging
import os
import time
import json

MAX_REASONABLE_STEP_LOSS_MM = 1.0

class MonitoredMove:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.printer.register_event_handler("klippy:connect", self._handle_connect)
        self.toolhead = None
        self.phoming = None
        self.gcode.register_command('MONITORED_G1', self.cmd_MONITORED_G1, desc=self.cmd_MONITORED_G1_help)
        self.gcode.register_command('SET_POSITION_LIMITS', self.cmd_SET_POSITION_LIMITS, desc=self.cmd_SET_POSITION_LIMITS_help)
        self.gcode.register_command('QUERY_ENDSTOPS_STATUS', self.cmd_QUERY_ENDSTOPS_STATUS, desc=self.cmd_QUERY_ENDSTOPS_STATUS_help)
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
            raise gcmd.error(f"No se pudo encontrar el rail para '{stepper_name}'. Rails disponibles: {rail_names}")
        
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
        
        # Obtener posición inicial
        initial_pos = self.toolhead.get_position()
        
        x = gcmd.get_float('X', initial_pos[0])
        y = gcmd.get_float('Y', initial_pos[1])
        z = gcmd.get_float('Z', initial_pos[2])
        f = gcmd.get_float('F', 60.0, above=0.)
        
        target_pos = [x, y, z]
        endstop = self._get_endstop_by_stepper(stepper_name, gcmd)
        
        movement_distance = target_pos[2] - initial_pos[2]
        # gcmd.respond_info(f"🎯 Movimiento monitoreado desde Z={initial_pos[2]:.3f} hacia Z={target_pos[2]:.3f} ({movement_distance:+.3f}mm)")
        # gcmd.respond_info(f"🔍 Monitoreando endstop de '{stepper_name}'")
        
        # Verificar estado inicial del endstop
        initial_endstop_state = False
        if hasattr(endstop, 'query_endstop'):
            initial_endstop_state = endstop.query_endstop(self.toolhead.get_last_move_time())
            # gcmd.respond_info(f"🔍 Estado inicial del endstop: {'ACTIVADO' if initial_endstop_state else 'LIBRE'}")
        
        # Variable para almacenar la posición final
        final_pos = None
        endstop_triggered = False
        
        # Guardar el tiempo inicial del movimiento
        start_time = self.toolhead.get_last_move_time()
        
        # Variable para rastrear si modificamos el endstop
        endstop_modified = False
        original_steppers = []
        
        try:
            speed_mms = f / 60.0
            
            # IMPORTANTE: Si estamos monitoreando stepper_y pero moviendo en Z,
            # necesitamos agregar temporalmente el stepper Z al endstop
            if stepper_name == 'stepper_y' and abs(movement_distance) > 0.01:
                # gcmd.respond_info("🔧 Configurando endstop Y para monitorear movimiento Z...")
                
                # Obtener el rail Z y sus steppers
                kin = self.toolhead.get_kinematics()
                z_steppers = []
                for rail in kin.rails:
                    if rail.get_name() == 'stepper_z':
                        z_steppers = rail.get_steppers()
                        break
                
                # Agregar temporalmente los steppers Z al endstop Y
                original_steppers = endstop.get_steppers()[:]
                endstop_modified = True
                for z_stepper in z_steppers:
                    endstop.add_stepper(z_stepper)
                    # gcmd.respond_info(f"  ➕ Agregado stepper: {z_stepper.get_name()}")
            
            # Intentar el movimiento con probing_move
            trigger_pos = self.phoming.probing_move(endstop, target_pos, speed_mms)
            
            # Si llegamos aquí sin excepción, el endstop se activó durante el movimiento
            # gcmd.respond_info(f"🎯 Endstop activado en X={trigger_pos[0]:.6f} Y={trigger_pos[1]:.6f} Z={trigger_pos[2]:.6f}")
            # gcmd.respond_info("✅ Movimiento detenido por endstop.")
            final_pos = trigger_pos
            endstop_triggered = True
            
        except self.printer.command_error as e:
            error_msg = str(e)
            
            # Obtener la posición actual después del intento
            current_pos = self.toolhead.get_position()
            
            if "triggered prior to movement" in error_msg:
                # Este error puede significar varias cosas, necesitamos investigar
                # gcmd.respond_info("⚠️ Klipper reporta 'triggered prior to movement'")
                
                # Intentar obtener la posición calculada por probing_move
                last_probe_pos = None
                if hasattr(self.phoming, 'get_last_probe_position'):
                    last_probe_pos = self.phoming.get_last_probe_position()
                    if last_probe_pos:
                        # gcmd.respond_info(f"📍 Posición calculada por probing_move: Z={last_probe_pos[2]:.6f}")
                        final_pos = last_probe_pos
                        endstop_triggered = True
                        
                        # Actualizar la posición del toolhead
                        self.toolhead.set_position(final_pos)
                        
                        # Verificar si realmente hubo movimiento
                        actual_movement = abs(final_pos[2] - initial_pos[2])
                        # gcmd.respond_info(f"📐 Movimiento detectado: {actual_movement:.3f}mm")
                        
                        if actual_movement < 0.01:
                            # gcmd.respond_info("⚠️ No hubo movimiento real (< 0.01mm)")
                            pass
                        else:
                            # gcmd.respond_info("✅ Confirmado: SÍ hubo movimiento")
                            pass
                
                if not last_probe_pos:
                    # Fallback al método anterior si no tenemos get_last_probe_position
                    gcmd.respond_info("⚠️ Usando método de diagnóstico alternativo...")
                    
                    # La posición del toolhead no se actualiza cuando probing_move falla
                    # Necesitamos calcularla basándonos en los steppers
                    kin = self.toolhead.get_kinematics()
                    
                    # Forzar actualización de la generación de pasos
                    self.toolhead.flush_step_generation()
                    
                    # Diagnóstico adicional
                    gcmd.respond_info("🔍 Diagnóstico de posiciones:")
                    
                    # Verificar cada stepper individualmente
                    for i, rail in enumerate(kin.rails[:3]):
                        axis_name = "XYZ"[i]
                        rail_name = rail.get_name()
                        for stepper in rail.get_steppers():
                            stepper_name = stepper.get_name()
                            cmd_pos = stepper.get_commanded_position()
                            mcu_pos = stepper.get_mcu_position()
                            gcmd.respond_info(f"  {axis_name} - {stepper_name}: cmd_pos={cmd_pos:.3f}, mcu_pos={mcu_pos}")
                    
                    # Obtener posición real de los steppers
                    kin_spos = {s.get_name(): s.get_commanded_position()
                               for s in kin.get_steppers()}
                    
                    # Calcular posición cartesiana desde posiciones de steppers
                    real_pos = kin.calc_position(kin_spos)
                    
                    # También intentar obtener la posición de otra manera
                    # Verificar el estado del endstop después del movimiento
                    final_endstop_state = False
                    if hasattr(endstop, 'query_endstop'):
                        final_endstop_state = endstop.query_endstop(self.toolhead.get_last_move_time())
                        gcmd.respond_info(f"🔍 Estado final del endstop: {'ACTIVADO' if final_endstop_state else 'LIBRE'}")
                    
                    # Calcular tiempo transcurrido
                    move_time = self.toolhead.get_last_move_time() - start_time
                    gcmd.respond_info(f"⏱️ Tiempo de movimiento: {move_time:.1f}s")
                    
                    # real_pos puede tener None en algunos ejes, usar posición actual para esos
                    final_pos = list(current_pos)
                    for i in range(3):
                        if real_pos[i] is not None:
                            final_pos[i] = real_pos[i]
                    
                    # Verificar si realmente nos movimos
                    actual_movement = abs(final_pos[2] - initial_pos[2])
                    
                    gcmd.respond_info(f"📍 Posición inicial: Z={initial_pos[2]:.3f}")
                    gcmd.respond_info(f"📍 Posición real (steppers): Z={final_pos[2]:.3f}")
                    gcmd.respond_info(f"📍 Movimiento real: {actual_movement:.3f}mm")
                    
                    # Si el endstop cambió de estado, asumimos que hubo movimiento
                    if not initial_endstop_state and final_endstop_state:
                        gcmd.respond_info("⚠️ El endstop cambió de LIBRE a ACTIVADO")
                        
                        # Para stepper_y usado como Z-max, calcular posición exacta
                        if stepper_name == 'stepper_y':
                            # Obtener el rail Z y sus steppers
                            z_rail = kin.rails[2]  # Rail Z
                            z_steppers = z_rail.get_steppers()
                            
                            if z_steppers:
                                # Obtener el primer stepper Z
                                z_stepper = z_steppers[0]
                                
                                try:
                                    # Forzar una consulta actualizada de la posición MCU
                                    # Esto es similar a lo que hace _query_mcu_position()
                                    gcmd.respond_info("🔄 Consultando posición actual del MCU...")
                                    
                                    # Método 1: Intentar obtener la posición actual del MCU
                                    # Primero, asegurarnos de que todos los movimientos estén procesados
                                    self.toolhead.flush_step_generation()
                                    self.toolhead.wait_moves()
                                    
                                    # Obtener la posición comandada actual (después del movimiento)
                                    final_cmd_pos = z_stepper.get_commanded_position()
                                    
                                    # También intentar obtener la posición MCU actual
                                    final_mcu_pos = z_stepper.get_mcu_position()
                                    
                                    gcmd.respond_info(f"📍 Posición comandada inicial: {initial_pos[2]:.3f}mm")
                                    gcmd.respond_info(f"📍 Posición comandada final: {final_cmd_pos:.3f}mm")
                                    gcmd.respond_info(f"🔢 Posición MCU final: {final_mcu_pos} pasos")
                                    
                                    # Método 2: Calcular basándonos en la diferencia de posiciones comandadas
                                    movement = final_cmd_pos - initial_pos[2]
                                    
                                    if abs(movement) > 0.01:
                                        # Hubo movimiento según las posiciones comandadas
                                        calculated_z = initial_pos[2] + movement
                                        gcmd.respond_info(f"📐 Movimiento detectado: {movement:.3f}mm")
                                        gcmd.respond_info(f"🎯 Posición calculada: Z={calculated_z:.3f}")
                                    else:
                                        # Si no hay diferencia en posiciones comandadas,
                                        # usar el position_max como fallback
                                        z_min, z_max = z_rail.get_range()
                                        calculated_z = z_max
                                        gcmd.respond_info(f"⚠️ Sin movimiento en posiciones comandadas")
                                        gcmd.respond_info(f"📐 Usando position_max: Z={calculated_z:.3f}")
                                    
                                    final_pos[2] = calculated_z
                                    endstop_triggered = True
                                    
                                    # Comparar con position_max
                                    z_min, z_max = z_rail.get_range()
                                    diff = calculated_z - z_max
                                    gcmd.respond_info(f"📏 Position_max esperado: Z={z_max:.3f}")
                                    gcmd.respond_info(f"📊 Diferencia detectada: {diff:+.3f}mm")
                                    
                                    # Actualizar la posición del toolhead
                                    self.toolhead.set_position(final_pos)
                                    
                                except Exception as e:
                                    gcmd.respond_info(f"⚠️ Error obteniendo posición: {str(e)}")
                                    # Fallback: asumir position_max
                                    z_min, z_max = z_rail.get_range()
                                    final_pos[2] = z_max
                                    gcmd.respond_info(f"📐 Fallback a position_max: Z={z_max:.3f}")
                                    endstop_triggered = True
                                    self.toolhead.set_position(final_pos)
                    elif actual_movement > 0.01:  # Si nos movimos más de 0.01mm
                        gcmd.respond_info(f"✅ Confirmado: SÍ hubo movimiento")
                        gcmd.respond_info(f"🎯 Endstop encontrado en: Z={final_pos[2]:.6f}")
                        endstop_triggered = True
                        # Actualizar la posición del toolhead para sincronizar
                        self.toolhead.set_position(final_pos)
                    else:
                        # Realmente no hubo movimiento
                        gcmd.respond_info("📍 Confirmado: NO hubo movimiento")
                        if initial_endstop_state:
                            gcmd.respond_info("💡 El endstop ya estaba activado desde el inicio")
                            final_pos = initial_pos
                            endstop_triggered = True
                        else:
                            gcmd.respond_info("❌ Error: endstop reporta activación sin movimiento real")
                            raise
                        
            elif "No trigger" in error_msg:
                # Movimiento completado sin activación
                gcmd.respond_info(f"⚠️ Movimiento completado sin activación del endstop")
                gcmd.respond_info(f"📍 Posición final: Z={current_pos[2]:.6f}")
                gcmd.respond_info("❌ No se pudo detectar el Z-max")
                final_pos = current_pos
                endstop_triggered = False
                
            elif "Move out of range" in error_msg:
                gcmd.respond_info("❌ Movimiento fuera de rango")
                gcmd.respond_info("💡 Usa SET_POSITION_LIMITS AXIS=Z MAX=150 primero")
                raise
                
            else:
                gcmd.respond_info(f"❌ Error inesperado: {error_msg}")
                raise
        
        finally:
            # Restaurar los steppers originales del endstop si los modificamos
            if endstop_modified and original_steppers is not None:
                # gcmd.respond_info("🔧 Restaurando configuración original del endstop...")
                # Primero, obtener los steppers actuales
                current_steppers = endstop.get_steppers()
                # Remover los steppers que agregamos (los que no estaban originalmente)
                for stepper in current_steppers:
                    if stepper not in original_steppers:
                        # No hay método remove_stepper, así que tenemos que hackear un poco
                        # Simplemente reportamos que deberíamos removerlo
                        pass  # gcmd.respond_info(f"  ⚠️ Nota: {stepper.get_name()} permanecerá asociado al endstop hasta el reinicio")
        
        # Si tenemos una posición final válida y es para stepper_y, calcular pérdida de pasos
        if final_pos is not None and stepper_name == 'stepper_y' and endstop_triggered:
            # Comparar con la posición esperada
            kin = self.toolhead.get_kinematics()
            z_rail = kin.rails[2]  # Rail Z
            z_min, z_max = z_rail.get_range()
            diferencia = final_pos[2] - z_max
            expected_z_max = float(f"{z_max:.6f}")
            detected_z_pos = float(f"{final_pos[2]:.6f}")
            delta_to_config = float(f"{diferencia:.6f}")
            measurement_type = "step_loss"
            step_loss_value = delta_to_config
            reference_mismatch_mm = None

            if abs(delta_to_config) > MAX_REASONABLE_STEP_LOSS_MM:
                measurement_type = "reference_mismatch"
                step_loss_value = None
                reference_mismatch_mm = delta_to_config
            
            # Reporte con posición absoluta y diferencia para base de datos
            gcmd.respond_info(f"Posición Z detectada: {detected_z_pos:.6f}mm")
            
            if measurement_type == "reference_mismatch":
                gcmd.respond_info(
                    f"⚠️ Desfase grande respecto a position_max ({expected_z_max:.6f}mm): "
                    f"{delta_to_config:+.6f}mm"
                )
                gcmd.respond_info(
                    "⚠️ Se registra como mismatch de referencia, no como pérdida de pasos real"
                )
            elif delta_to_config > 0:
                gcmd.respond_info(f"Pérdida de pasos BAJANDO: {delta_to_config:.6f}mm")
            elif delta_to_config < 0:
                gcmd.respond_info(f"Pérdida de pasos SUBIENDO: {delta_to_config:.6f}mm")
            else:
                gcmd.respond_info(f"Pérdida de pasos: {delta_to_config:.6f}mm")
            
            # Escribir los valores en un archivo para nanodlp
            try:
                # Crear directorio si no existe
                db_path = "/home/pi/nanodlp/db"
                if not os.path.exists(db_path):
                    os.makedirs(db_path)
                
                # Leer PlateID del archivo status.json
                plate_id = None
                status_file = "/home/pi/nanodlp/public/status.json"
                try:
                    if os.path.exists(status_file):
                        with open(status_file, 'r') as f:
                            status_data = json.load(f)
                            plate_id = status_data.get('PlateID', None)
                except Exception as e:
                    gcmd.respond_info(f"Advertencia: No se pudo leer PlateID: {str(e)}")
                
                # Archivo JSON para el historial
                json_file = os.path.join(db_path, "step_loss_history.json")
                
                # Crear objeto de medición
                measurement = {
                    "timestamp": time.time(),
                    "datetime": time.strftime("%Y-%m-%d %H:%M:%S"),
                    "measurement_type": measurement_type,
                    "step_loss": step_loss_value,
                    "reference_mismatch_mm": reference_mismatch_mm,
                    "expected_z_max": expected_z_max,
                    "z_position": detected_z_pos,
                    "delta_to_config": delta_to_config,
                    "plate_id": plate_id
                }
                
                # Leer historial existente o crear nuevo
                history = []
                if os.path.exists(json_file):
                    try:
                        with open(json_file, 'r') as f:
                            history = json.load(f)
                    except:
                        history = []
                
                # Agregar nueva medición
                history.append(measurement)
                
                # Guardar historial actualizado
                with open(json_file, 'w') as f:
                    json.dump(history, f, indent=2)
                
                gcmd.respond_info(f"Medición guardada en: {json_file}")
                
                # También guardar la última medición en un archivo separado
                latest_file = os.path.join(db_path, "step_loss_latest.json")
                with open(latest_file, 'w') as f:
                    json.dump(measurement, f, indent=2)
                    
            except Exception as e:
                gcmd.respond_info(f"Error al guardar archivo: {str(e)}")

    cmd_SET_POSITION_LIMITS_help = "Modifica temporalmente los límites de posición de un eje (equivalente a M211 de Marlin)"
    def cmd_SET_POSITION_LIMITS(self, gcmd):
        """Comando para modificar temporalmente los límites de posición."""
        if not self.toolhead:
            raise gcmd.error("SET_POSITION_LIMITS: Sistema no listo.")
        
        # Obtener la cinemática
        kin = self.toolhead.get_kinematics()
        
        # Obtener parámetros
        axis = gcmd.get('AXIS', None)
        min_val = gcmd.get_float('MIN', None)
        max_val = gcmd.get_float('MAX', None)
        reset = gcmd.get('RESET', None)
        
        if reset is not None:
            # Resetear a valores originales del config
            gcmd.respond_info("🔄 Restaurando límites originales...")
            for i, rail in enumerate(kin.rails[:3]):  # Solo X, Y, Z
                original_range = rail.get_range()
                kin.limits[i] = original_range
                axis_name = "XYZ"[i]
                gcmd.respond_info(f"✅ {axis_name}: [{original_range[0]:.3f}, {original_range[1]:.3f}]")
            return
        
        if not axis:
            # Mostrar límites actuales
            gcmd.respond_info("📏 Límites de posición actuales:")
            for i, limit in enumerate(kin.limits[:3]):
                axis_name = "XYZ"[i]
                if limit[0] <= limit[1]:  # Eje homedado
                    gcmd.respond_info(f"  {axis_name}: [{limit[0]:.3f}, {limit[1]:.3f}]")
                else:
                    gcmd.respond_info(f"  {axis_name}: [NO HOMEDADO]")
            gcmd.respond_info("💡 Uso: SET_POSITION_LIMITS AXIS=Z MAX=150")
            gcmd.respond_info("💡 Resetear: SET_POSITION_LIMITS RESET=1")
            return
        
        # Validar eje
        if axis.upper() not in 'XYZ':
            raise gcmd.error("AXIS debe ser X, Y o Z")
        
        axis_index = "XYZ".index(axis.upper())
        current_limits = kin.limits[axis_index]
        
        if current_limits[0] > current_limits[1]:
            raise gcmd.error(f"El eje {axis.upper()} no está homedado")
        
        # Obtener límites actuales
        current_min, current_max = current_limits
        
        # Aplicar nuevos valores
        new_min = min_val if min_val is not None else current_min
        new_max = max_val if max_val is not None else current_max
        
        # Validación básica
        if new_min >= new_max:
            raise gcmd.error("MIN debe ser menor que MAX")
        
        # Aplicar nuevos límites
        kin.limits[axis_index] = (new_min, new_max)
        
        gcmd.respond_info(f"✅ Límites de {axis.upper()} actualizados:")
        gcmd.respond_info(f"   Anterior: [{current_min:.3f}, {current_max:.3f}]")
        gcmd.respond_info(f"   Nuevo:    [{new_min:.3f}, {new_max:.3f}]")
        gcmd.respond_info("⚠️ Estos cambios son temporales hasta el próximo reinicio")

    cmd_QUERY_ENDSTOPS_STATUS_help = "Consulta el estado real de todos los endstops"
    def cmd_QUERY_ENDSTOPS_STATUS(self, gcmd):
        """Comando para consultar el estado real de todos los endstops."""
        if not self.toolhead:
            raise gcmd.error("QUERY_ENDSTOPS_STATUS: Sistema no listo.")
        
        # Obtener la cinemática y los rails
        kin = self.toolhead.get_kinematics()
        
        gcmd.respond_info("📏 Estado de endstops:")
        gcmd.respond_info(f"🕐 Tiempo actual: {self.toolhead.get_last_move_time():.3f}")
        
        # Verificar cada rail
        for i, rail in enumerate(kin.rails[:3]):  # Solo X, Y, Z
            axis_name = "XYZ"[i]
            rail_name = rail.get_name()
            endstops = rail.get_endstops()
            
            if endstops:
                for endstop, name in endstops:
                    try:
                        if hasattr(endstop, 'query_endstop'):
                            state = endstop.query_endstop(self.toolhead.get_last_move_time())
                            state_text = "ACTIVADO 🔴" if state else "LIBRE 🟢"
                            gcmd.respond_info(f"  {axis_name} ({rail_name}): {state_text}")
                        else:
                            gcmd.respond_info(f"  {axis_name} ({rail_name}): No consultable")
                    except Exception as e:
                        gcmd.respond_info(f"  {axis_name} ({rail_name}): Error - {str(e)}")
            else:
                gcmd.respond_info(f"  {axis_name} ({rail_name}): Sin endstop")
        
        # Posición actual
        current_pos = self.toolhead.get_position()
        gcmd.respond_info(f"📍 Posición actual: X={current_pos[0]:.3f} Y={current_pos[1]:.3f} Z={current_pos[2]:.3f}")

def load_config(config):
    return MonitoredMove(config) 
