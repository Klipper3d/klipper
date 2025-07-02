# Módulo para homing del eje Z hacia Zmax usando el endstop de Y
#
# Copyright (C) 2024 MicroLay
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging

class ZMaxHomingAlt:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        
        # Registrar para eventos
        self.printer.register_event_handler("klippy:connect", self._handle_connect)
        
        # Variables
        self.toolhead = None
        self.y_endstop = None
        self.z_stepper = None
        self.endstop_triggered = False
        self.trigger_position = None
        
        # Configuración
        # Velocidades más bajas por defecto
        self.speed = config.getfloat('speed', 5.0, above=0.)
        self.second_speed = config.getfloat('second_speed', 2.0, above=0.)
        self.retract_dist = config.getfloat('retract_dist', 2.0, minval=0.)
        self.retract_speed = config.getfloat('retract_speed', 3.0, above=0.)
        # Cambiar a False por defecto para que se quede en Zmax
        self.final_retract = config.getboolean('final_retract', False)
        # Evitar que se apaguen los motores al completar
        self.disable_motors = config.getboolean('disable_motors', False)
        
        # Registrar comandos
        self.gcode.register_command(
            'ZMAX_HOME',
            self.cmd_ZMAX_HOME,
            desc=self.cmd_ZMAX_HOME_help
        )
        self.gcode.register_command(
            'MEASURE_Z_LENGTH',
            self.cmd_MEASURE_Z_LENGTH,
            desc=self.cmd_MEASURE_Z_LENGTH_help
        )
        
        # Mensaje de inicio
        logging.info(f"ZMaxHoming: Initialized with speeds - homing:{self.speed} mm/s, second:{self.second_speed} mm/s, retract:{self.retract_speed} mm/s")
        logging.info(f"ZMaxHoming: final_retract={self.final_retract}, disable_motors={self.disable_motors}")
    
    def _handle_connect(self):
        # Obtener el toolhead
        self.toolhead = self.printer.lookup_object('toolhead')
        
        # Obtener el endstop de Y
        kin = self.toolhead.get_kinematics()
        if hasattr(kin, 'rails'):
            # Buscar el endstop de Y
            for rail in kin.rails:
                if rail.get_name() == "stepper_y":
                    endstops = rail.get_endstops()
                    if endstops:
                        self.y_endstop = endstops[0][0]
                        logging.info(f"ZMaxHoming: Using Y endstop for Z-max homing")
                        break
            
            # Buscar el stepper Z
            for rail in kin.rails:
                if rail.get_name() == "stepper_z":
                    self.z_stepper = rail
                    # Obtener la posición máxima de Z
                    z_max_cfg = self.z_stepper.get_range()[1]
                    logging.info(f"ZMaxHoming: Z-max position from config: {z_max_cfg}mm")
                    break
        
        if self.y_endstop is None:
            raise self.printer.config_error("No se encontró el endstop de Y")
        if self.z_stepper is None:
            raise self.printer.config_error("No se encontró el stepper Z")
    
    def _monitor_endstop(self):
        """Monitorea el endstop Y. Si se activa por primera vez, guarda la posición y actualiza el flag."""
        # Solo comprobar si aún no hemos detectado el trigger en esta secuencia
        if not self.endstop_triggered:
            if self.y_endstop.query_endstop(0):
                self.endstop_triggered = True
                # Capturar la posición inmediatamente al detectar el trigger
                self.trigger_position = self.toolhead.get_position()
                self.gcode.respond_info(f"!!! Endstop Y TRIGGERED at Z={self.trigger_position[2]:.3f} !!!")
                # No forzamos la parada aquí, devolvemos True y el llamador decide
                return True # Triggered NOW
        # Si ya estaba triggered antes, o si query_endstop dio False, devolvemos el estado actual del flag
        return self.endstop_triggered
    
    def _safe_move_z(self, pos, speed, increment):
        """Realiza un movimiento seguro en Z con verificación de endstop (pre y post move)"""
        curpos = self.toolhead.get_position()
        target_z = pos[2]
        z_max_cfg = self.z_stepper.get_range()[1]

        # Comprobación inicial antes de cualquier movimiento
        if self._monitor_endstop():
            self.gcode.respond_info(f"Endstop triggered before starting safe_move_z at Z={curpos[2]:.3f}")
            return True

        while curpos[2] < target_z:
            # Comprobar límites antes de calcular el siguiente paso
            # Usar una pequeña tolerancia para evitar sobrepasar z_max_cfg
            if curpos[2] >= z_max_cfg - 0.01:
                 self.gcode.respond_info(f"Approaching Z max limit ({z_max_cfg:.3f}) during safe move. Stopping at Z={curpos[2]:.3f}.")
                 break # Detenerse antes de exceder

            # Calcular siguiente posición, sin sobrepasar target_z ni z_max_cfg
            next_z = min(curpos[2] + increment, target_z, z_max_cfg - 0.01)
            # Evitar movimientos extremadamente pequeños si el incremento es muy bajo o ya estamos en el objetivo
            if abs(next_z - curpos[2]) < 0.001:
                 self.gcode.respond_info(f"Safe move increment too small ({increment:.3f}mm) or target reached at Z={curpos[2]:.3f}. Stopping.")
                 break

            next_pos = list(curpos)
            next_pos[2] = next_z

            # --- Comprobación Pre-movimiento --- (Dentro del bucle)
            if self._monitor_endstop():
                # Endstop activado entre el último movimiento y esta comprobación
                # La posición ya fue capturada por _monitor_endstop
                self.gcode.respond_info(f"Endstop check (pre-move): TRIGGERED at Z={self.trigger_position[2]:.3f} (current Z={curpos[2]:.3f})")
                return True

            try:
                # Realizar el movimiento
                # self.gcode.respond_info(f"Moving Z: {curpos[2]:.3f} -> {next_z:.3f} (incr: {increment:.3f}, target: {target_z:.3f})") # Debug
                self.toolhead.move(next_pos, speed)
                self.toolhead.wait_moves() # Asegurar que el movimiento se complete ANTES de la comprobación post-movimiento

                # --- Comprobación Post-movimiento --- (CRÍTICO)
                if self._monitor_endstop():
                    # Endstop activado durante o inmediatamente después del movimiento
                    # La posición ya fue capturada por _monitor_endstop
                    self.gcode.respond_info(f"Endstop check (post-move): TRIGGERED at Z={self.trigger_position[2]:.3f} (intended move Z={next_z:.3f})")
                    return True

                # Actualizar posición para la siguiente iteración si no hubo trigger
                curpos = next_pos # Actualizar a la posición a la que nos acabamos de mover

            except Exception as e:
                self.gcode.respond_info(f"Error during toolhead.move in _safe_move_z: {str(e)}")
                # Comprobar endstop una última vez en caso de error durante el procesamiento del movimiento
                return self._monitor_endstop()

        # Bucle terminado (alcanzó target_z, z_max_cfg, o incremento muy pequeño)
        # Comprobación final por si se activó justo cuando la condición del bucle se volvió falsa
        final_triggered_state = self._monitor_endstop()
        if not final_triggered_state:
             self.gcode.respond_info(f"safe_move_z finished loop. TargetZ: {target_z:.3f}, FinalZ: {curpos[2]:.3f}. Endstop triggered flag: {self.endstop_triggered}")

        return self.endstop_triggered # Devolver el estado final del flag
    
    def _find_z_max(self, gcmd, speed, second_speed, retract_dist, z_homed):
        """Encuentra la posición Zmax. Retorna la posición del trigger si z_homed, o [None, None, z_max_cfg] si no."""
        toolhead = self.toolhead
        z_max_cfg = self.z_stepper.get_range()[1]
        z_min_cfg = self.z_stepper.get_range()[0]  # Añadido para verificar límites

        self.endstop_triggered = False
        self.trigger_position = None # Reset trigger position
        toolhead.flush_step_generation()
        toolhead.dwell(0.001)

        # Verificar si el endstop ya está activado al inicio
        initial_endstop_state = self.y_endstop.query_endstop(0)
        
        if initial_endstop_state:
            gcmd.respond_info("Endstop Y ya activado al inicio del procedimiento")
            # Si el endstop ya está activado, registramos la posición actual
            current_pos = toolhead.get_position()
            self.endstop_triggered = True
            self.trigger_position = list(current_pos)  # Usar list() para crear una copia
            
            # Si necesitamos retracción y podemos retroceder sin salir del rango válido
            safe_retract_dist = min(retract_dist, current_pos[2] - z_min_cfg)
            
            if safe_retract_dist > 0:
                gcmd.respond_info(f"Retrayendo {safe_retract_dist}mm para desactivar el endstop")
                retract_pos = list(current_pos)
                retract_pos[2] -= safe_retract_dist
                try:
                    toolhead.move(retract_pos, speed)
                    toolhead.wait_moves()
                    
                    # Verificar si el endstop se desactivó
                    if not self.y_endstop.query_endstop(0):
                        gcmd.respond_info("Endstop desactivado después de la retracción")
                        self.endstop_triggered = False
                    else:
                        gcmd.respond_info("ADVERTENCIA: Endstop sigue activado después de retracción")
                except Exception as e:
                    gcmd.respond_info(f"Error durante retracción inicial: {str(e)}")
            else:
                gcmd.respond_info("No se puede retroceder más sin salir del rango válido")
        
        # Si el endstop sigue activado después de intentar retracción, no podemos continuar con normalidad
        if initial_endstop_state and self.endstop_triggered:
            gcmd.respond_info("El endstop sigue activado, asumiendo que ya estamos en posición Z-max")
            if not z_homed:
                # Para Z no homeado, retornamos la posición máxima de configuración con formato completo
                current_pos = toolhead.get_position()
                # Asegurar que current_pos tiene 4 elementos
                if len(current_pos) < 4:
                    current_pos = list(current_pos) + [0.0] * (4 - len(current_pos))
                return [current_pos[0], current_pos[1], z_max_cfg, current_pos[3]]
            else:
                # Para Z homeado, retornamos la posición actual como trigger final
                return self.trigger_position or toolhead.get_position()

        # Obtener la posición actual
        current_pos = toolhead.get_position()
        current_z = current_pos[2]
        
        # Asegurar que current_pos tiene 4 elementos
        if len(current_pos) < 4:
            current_pos = list(current_pos) + [0.0] * (4 - len(current_pos))
            gcmd.respond_info("Extendiendo posición actual para incluir el valor E")
        
        # Si estamos cerca de Zmax, proceder directamente a una aproximación directa
        if z_homed and current_z > 70.0:  # Si estamos a más de 70mm (cerca de Zmax que es ~79mm según los logs)
            gcmd.respond_info(f"Posición actual Z={current_z:.3f} cercana a Zmax, intentando aproximación directa...")
            # Aproximación directa a Z máximo
            try:
                move_pos = list(current_pos)
                move_pos[2] = z_max_cfg
                toolhead.move(move_pos, speed/2)  # Velocidad reducida para mayor precisión
                toolhead.wait_moves()
                
                # Verificar si el endstop se activó
                if self.y_endstop.query_endstop(0):
                    gcmd.respond_info("Endstop activado después de aproximación directa a Zmax")
                    self.endstop_triggered = True
                    self.trigger_position = toolhead.get_position()
                    
                    # Hacemos una retracción pequeña para liberar presión sobre el endstop
                    retract_pos = list(self.trigger_position)
                    retract_pos[2] -= min(1.0, retract_dist)  # Retracción más pequeña (1mm o menos)
                    toolhead.move(retract_pos, speed)
                    toolhead.wait_moves()
                    
                    # Aproximación final lenta y precisa
                    gcmd.respond_info("Realizando aproximación final de precisión...")
                    move_pos = list(retract_pos)
                    move_pos[2] = z_max_cfg
                    toolhead.move(move_pos, second_speed/2)  # Velocidad ultra-reducida
                    toolhead.wait_moves()
                    
                    # Verificar estado final
                    final_triggered = self.y_endstop.query_endstop(0)
                    gcmd.respond_info(f"Estado final del endstop: {'ACTIVADO' if final_triggered else 'NO ACTIVADO'}")
                    
                    if final_triggered:
                        self.endstop_triggered = True
                        self.trigger_position = toolhead.get_position()
                        return self.trigger_position
                    else:
                        # Si por alguna razón no está activado, usar posición máxima de configuración
                        gcmd.respond_info("Endstop no activado en aproximación final, usando posición máxima de configuración")
                        # Usar posición actual pero con Z máximo, asegurando los 4 elementos
                        pos = toolhead.get_position()
                        if len(pos) < 4:
                            pos = list(pos) + [0.0] * (4 - len(pos))
                        self.trigger_position = [pos[0], pos[1], z_max_cfg, pos[3]]
                        return self.trigger_position
                else:
                    # Si no se activó con la aproximación directa, intentamos una búsqueda normal
                    gcmd.respond_info("Endstop no activado con aproximación directa, intentando búsqueda normal")
                    # No retornamos, continuamos con la búsqueda normal
            except Exception as e:
                gcmd.respond_info(f"Error durante aproximación directa: {str(e)}")
                # Continuamos con búsqueda normal si falla la aproximación directa

        if z_homed:
            # --- Homing Z cuando ya está homeado (lógica original adaptada) ---
            gcmd.respond_info("Iniciando búsqueda rápida (absoluta)...")
            curpos = toolhead.get_position()
            # Asegurar que curpos tiene 4 elementos
            if len(curpos) < 4:
                curpos = list(curpos) + [0.0] * (4 - len(curpos))
                
            movepos = list(curpos)
            # Asegurarse de que la posición objetivo no sea igual a la actual si ya estamos cerca del máximo
            if abs(curpos[2] - z_max_cfg) < 0.1:
                 movepos[2] = curpos[2] - 0.2 # Moverse ligeramente hacia abajo si ya estamos en el límite
            else:
                 # Si Z ya está homeado, empezar desde una posición más baja para asegurar que
                 # el endstop se active durante el movimiento
                 target_z = max(z_max_cfg - 5.0, curpos[2])
                 # Nunca bajar más de 5mm desde la posición actual
                 target_z = max(target_z, curpos[2] - 5.0)
                 # Nunca subir si ya estamos en una posición alta
                 target_z = min(target_z, z_max_cfg - 0.1)
                 movepos[2] = target_z
                 gcmd.respond_info(f"Posición actual Z: {curpos[2]:.3f}, posición objetivo Z: {target_z:.3f}")

             # Verificar si el endstop ya estaba activado antes de mover
            if self.y_endstop.query_endstop(0):
                gcmd.respond_info("Endstop Y ya activado antes de iniciar movimiento")
                self.endstop_triggered = True
                self.trigger_position = list(curpos)  # Usar la posición actual
                # No necesitamos hacer el movimiento inicial, continuamos con la retracción
            else:
                # Mover y verificar si se activa el endstop
                if not self._safe_move_z(movepos, speed, 1.0): # Incrementos de 1mm
                    # Si safe_move_z retorna False pero el endstop SÍ se activó, significa que se activó inmediatamente.
                    if not self.endstop_triggered:
                        # Verificar una última vez por si acaso
                        if self.y_endstop.query_endstop(0):
                            gcmd.respond_info("Endstop se activó en verificación final después de no detectarse en búsqueda")
                            self.endstop_triggered = True
                            self.trigger_position = toolhead.get_position()
                        else:
                            # No se encontró el endstop, pero podemos estar en Zmax
                            # Intentar mover un poco más arriba directamente como último recurso
                            gcmd.respond_info("Intentando aproximación directa a Zmax como último recurso...")
                            final_pos = list(toolhead.get_position())
                            # Asegurar que final_pos tiene 4 elementos
                            if len(final_pos) < 4:
                                final_pos = list(final_pos) + [0.0] * (4 - len(final_pos))
                                
                            # Intentar subir directamente a Zmax
                            final_pos[2] = z_max_cfg
                            try:
                                toolhead.move(final_pos, speed/2)
                                toolhead.wait_moves()
                                # Verificar si ahora el endstop está activado
                                if self.y_endstop.query_endstop(0):
                                    gcmd.respond_info("Endstop activado después de movimiento directo a Zmax")
                                    self.endstop_triggered = True
                                    self.trigger_position = toolhead.get_position()
                                else:
                                    # Intento adicional: moverse un poco más allá de Zmax por si el final de carrera está ligeramente más alto
                                    try:
                                        gcmd.respond_info("Último intento: movimiento extendido más allá de Zmax...")
                                        extended_pos = list(final_pos)
                                        extended_pos[2] = z_max_cfg + 0.2  # 0.2mm más allá del máximo configurado
                                        toolhead.move(extended_pos, speed/4)  # Velocidad muy baja
                                        toolhead.wait_moves()
                                        # Verificación final
                                        if self.y_endstop.query_endstop(0):
                                            gcmd.respond_info("Endstop activado en movimiento extendido")
                                            self.endstop_triggered = True
                                            self.trigger_position = toolhead.get_position()
                                        else:
                                            raise gcmd.error("No se detectó el endstop después de intentar todas las opciones")
                                    except Exception as e:
                                        if "Move out of range" in str(e):
                                            gcmd.respond_info("Límite de movimiento alcanzado en intento extendido. Asumiendo Zmax.")
                                            self.endstop_triggered = True
                                            # Crear trigger_position con 4 elementos, incluyendo E de la posición actual
                                            self.trigger_position = [final_pos[0], final_pos[1], z_max_cfg, final_pos[3]]
                                        else:
                                            raise gcmd.error(f"No se detectó el endstop durante la búsqueda rápida: {str(e)}")
                            except Exception as e:
                                if "Move out of range" in str(e):
                                    gcmd.respond_info("Límite de movimiento alcanzado. Asumiendo posición Zmax.")
                                    self.endstop_triggered = True
                                    # Crear trigger_position con 4 elementos, incluyendo E de la posición actual
                                    self.trigger_position = [final_pos[0], final_pos[1], z_max_cfg, final_pos[3]]
                                else:
                                    raise gcmd.error(f"No se detectó el endstop durante la búsqueda rápida: {str(e)}")
                    else:
                        gcmd.respond_info("Endstop detectado inmediatamente en búsqueda rápida.")
                        # Si no hay trigger_position guardado aún (pasó en el primer check), tomar la posición actual
                        if not self.trigger_position:
                             self.trigger_position = toolhead.get_position()


            # Si _safe_move_z terminó porque se alcanzó el objetivo sin trigger (raro), verificar una vez más
            if not self.endstop_triggered:
                if self._monitor_endstop():
                    gcmd.respond_info("Endstop detectado al final del movimiento rápido.")
                else:
                     # Si realmente no se disparó, es un error
                     raise gcmd.error("Se alcanzó el objetivo de movimiento rápido sin detectar el endstop.")


            # Asegurar que tenemos una posición de trigger
            if not self.trigger_position:
                # Si por alguna razón no se guardó, tomar la posición actual como referencia (menos preciso)
                self.trigger_position = toolhead.get_position()
                gcmd.respond_info("No se guardó la posición exacta del trigger rápido, usando la posición actual.")

            # Asegurar que trigger_position tiene 4 elementos
            if len(self.trigger_position) < 4:
                # Obtener el valor E actual
                current_pos = toolhead.get_position()
                e_value = 0.0
                if len(current_pos) >= 4:
                    e_value = current_pos[3]
                # Extender trigger_position con el valor E
                self.trigger_position = list(self.trigger_position) + [0.0] * (4 - len(self.trigger_position))
                self.trigger_position[3] = e_value
                gcmd.respond_info("Extendiendo posición de trigger para incluir valor E")

            first_trigger_pos = list(self.trigger_position) # Guardar posición donde se activó el endstop

            # Retraer
            gcmd.respond_info(f"Primer trigger detectado en Z={first_trigger_pos[2]:.3f}. Retrayendo {retract_dist}mm...")
            retract_pos = list(first_trigger_pos)
            retract_pos[2] -= retract_dist
            # Asegurar que la retracción no vaya por debajo de position_min
            if retract_pos[2] < z_min_cfg:
                 retract_pos[2] = z_min_cfg
                 gcmd.respond_info(f"Ajustando retracción a Z={retract_pos[2]:.3f} para no exceder Zmin")
            toolhead.move(retract_pos, speed) # Retraer a velocidad normal
            toolhead.wait_moves()

            # Búsqueda lenta
            gcmd.respond_info("Iniciando aproximación fina (absoluta)...")
            self.endstop_triggered = False # Resetear para la segunda búsqueda
            self.trigger_position = None
            toolhead.flush_step_generation()
            toolhead.dwell(0.001)

            # Mover lentamente hacia z_max de nuevo desde la posición retraída
            movepos = list(retract_pos)
             # Asegurarse de que la posición objetivo no sea igual a la actual
            if abs(retract_pos[2] - z_max_cfg) < 0.01:
                 movepos[2] = retract_pos[2] - 0.02 # Moverse ligeramente hacia abajo si ya estamos en el límite
            else:
                movepos[2] = z_max_cfg - 0.1

            if not self._safe_move_z(movepos, second_speed, 0.02): # Incrementos de 0.02mm
                # Si safe_move_z retorna False pero el endstop SÍ se activó
                if not self.endstop_triggered:
                     # Si falla durante la aproximación lenta (raro, a menos que haya problema con el endstop), usar el primer trigger
                     gcmd.respond_info("No se detectó el endstop en la aproximación fina. Usando posición del primer trigger.")
                     self.trigger_position = first_trigger_pos
                else:
                     gcmd.respond_info("Endstop detectado inmediatamente en aproximación fina.")
                     if not self.trigger_position:
                          self.trigger_position = toolhead.get_position() # Posición actual como fallback

            # Verificar de nuevo si se alcanzó el objetivo sin trigger
            if not self.endstop_triggered:
                if self._monitor_endstop():
                    gcmd.respond_info("Endstop detectado al final de la aproximación fina.")
                else:
                    # Si no se activó, usar la posición del primer trigger como fallback más seguro
                    gcmd.respond_info("Se alcanzó el objetivo de aproximación fina sin detectar el endstop. Usando posición del primer trigger.")
                    self.trigger_position = first_trigger_pos


            if not self.trigger_position:
                 # Fallback final: usar el primer trigger si todo lo demás falla
                 gcmd.respond_info("No se registró la posición del trigger en la aproximación fina. Usando el primer trigger.")
                 self.trigger_position = first_trigger_pos

            # Verificar que trigger_position tiene 4 elementos antes de retornar
            if len(self.trigger_position) < 4:
                # Obtener valor E actual
                current_pos = toolhead.get_position()
                e_value = 0.0
                if len(current_pos) >= 4:
                    e_value = current_pos[3]
                # Extender trigger_position
                self.trigger_position = list(self.trigger_position) + [0.0] * (4 - len(self.trigger_position))
                self.trigger_position[3] = e_value
                gcmd.respond_info("Extendiendo posición de trigger final para incluir valor E")

            gcmd.respond_info(f"Trigger final detectado en Z={self.trigger_position[2]:.3f}")
            return self.trigger_position # Retornar la posición medida

        else:
            # --- Homing Z cuando no está homeado (marcar Z como homeado temporalmente) ---
            gcmd.respond_info("Eje Z no homeado. Marcando Z como homeado temporalmente para permitir movimiento...")
            # Obtener posición actual (puede ser [0,0,0] o la última conocida antes del reinicio)
            current_pos = toolhead.get_position()
            # Asegurar que current_pos tiene 4 elementos
            if len(current_pos) < 4:
                current_pos = list(current_pos) + [0.0] * (4 - len(current_pos))
                
            # Marcar solo Z como homeado en su posición actual
            toolhead.set_position(current_pos, homing_axes=('z',))
            toolhead.wait_moves() # Asegurar que el estado se actualiza
            gcmd.respond_info(f"Z temporalmente homeado en {current_pos[2]:.3f}. Iniciando búsqueda ZMAX...")

            # Si el endstop ya estaba activado y no pudimos desactivarlo, simplemente retornamos
            if initial_endstop_state and self.endstop_triggered:
                gcmd.respond_info("Ya en posición Z-max, omitiendo búsqueda")
                # Retornar posición con formato completo (4 elementos)
                return [current_pos[0], current_pos[1], z_max_cfg, current_pos[3]]

            # Ahora que Z está "homeado", podemos usar la lógica de movimiento absoluto
            # Realizar búsqueda rápida
            gcmd.respond_info("Iniciando búsqueda rápida (absoluta simulada)...")
            movepos = list(current_pos) # Usar la posición actual "falsa" como punto de partida
            movepos[2] = z_max_cfg - 0.1 # Objetivo justo debajo del máximo configurado

            # Usamos _safe_move_z, que ahora funcionará porque Z está marcado como homeado
            if not self._safe_move_z(movepos, speed, 1.0):
                if not self.endstop_triggered:
                    raise gcmd.error("No se detectó el endstop durante la búsqueda rápida (Z no homeado)")
                else:
                     gcmd.respond_info("Endstop detectado inmediatamente en búsqueda rápida (Z no homeado).")
                     if not self.trigger_position:
                         self.trigger_position = toolhead.get_position()

            if not self.endstop_triggered:
                 if self._monitor_endstop():
                     gcmd.respond_info("Endstop detectado al final del movimiento rápido (Z no homeado).")
                 else:
                     raise gcmd.error("Se alcanzó el objetivo de movimiento rápido sin detectar el endstop (Z no homeado).")

            if not self.trigger_position:
                 self.trigger_position = toolhead.get_position()
                 gcmd.respond_info("No se guardó la posición exacta del trigger rápido (Z no homeado), usando la posición actual.")

            # Asegurar que trigger_position tiene 4 elementos
            if len(self.trigger_position) < 4:
                self.trigger_position = list(self.trigger_position) + [0.0] * (4 - len(self.trigger_position))
                if len(current_pos) >= 4:
                    self.trigger_position[3] = current_pos[3]  # Copiar E de la posición actual
                gcmd.respond_info("Extendiendo posición de trigger para incluir valor E (Z no homeado)")

            first_trigger_pos = list(self.trigger_position)

            # Retraer (ahora se puede usar toolhead.move)
            gcmd.respond_info(f"Primer trigger (Z no homeado) detectado en Z={first_trigger_pos[2]:.3f}. Retrayendo {retract_dist}mm...")
            retract_pos = list(first_trigger_pos)
            retract_pos[2] -= retract_dist
            # Verificar límites para evitar movimientos fuera de rango
            if retract_pos[2] < z_min_cfg:
                retract_pos[2] = z_min_cfg
                gcmd.respond_info(f"Ajustando retracción a Z={retract_pos[2]:.3f} para no exceder Zmin")
            toolhead.move(retract_pos, speed)
            toolhead.wait_moves()

            # Búsqueda lenta
            gcmd.respond_info("Iniciando aproximación fina (absoluta simulada)...")
            self.endstop_triggered = False # Resetear para la segunda búsqueda
            self.trigger_position = None
            toolhead.flush_step_generation()
            toolhead.dwell(0.001)

            movepos = list(retract_pos)
            movepos[2] = z_max_cfg - 0.1 # Objetivo de nuevo justo debajo del máximo

            if not self._safe_move_z(movepos, second_speed, 0.02):
                 if not self.endstop_triggered:
                     gcmd.respond_info("No se detectó el endstop en la aproximación fina (Z no homeado). Usando posición del primer trigger.")
                     self.trigger_position = first_trigger_pos
                 else:
                     gcmd.respond_info("Endstop detectado inmediatamente en aproximación fina (Z no homeado).")
                     if not self.trigger_position:
                          self.trigger_position = toolhead.get_position()

            if not self.endstop_triggered:
                 if self._monitor_endstop():
                     gcmd.respond_info("Endstop detectado al final de la aproximación fina (Z no homeado).")
                 else:
                     gcmd.respond_info("Se alcanzó el objetivo de aproximación fina sin detectar el endstop (Z no homeado). Usando posición del primer trigger.")
                     self.trigger_position = first_trigger_pos

            if not self.trigger_position:
                  gcmd.respond_info("No se registró la posición del trigger en la aproximación fina (Z no homeado). Usando el primer trigger.")
                  self.trigger_position = first_trigger_pos

            # Asegurar que trigger_position tiene 4 elementos
            if len(self.trigger_position) < 4:
                self.trigger_position = list(self.trigger_position) + [0.0] * (4 - len(self.trigger_position))
                if len(current_pos) >= 4:
                    self.trigger_position[3] = current_pos[3]  # Copiar E de la posición actual
                gcmd.respond_info("Extendiendo posición de trigger final para incluir valor E (Z no homeado)")

            gcmd.respond_info(f"Trigger final (Z no homeado) detectado en Z={self.trigger_position[2]:.3f}")
            # Retornar posición con formato completo (4 elementos)
            return [current_pos[0], current_pos[1], z_max_cfg, current_pos[3]]

    cmd_ZMAX_HOME_help = "Realiza el homing del eje Z hacia Zmax usando el endstop de Y"
    def cmd_ZMAX_HOME(self, gcmd):
        toolhead = self.toolhead
        if self.y_endstop is None:
            raise gcmd.error("ZMAX_HOME: Endstop de Y no inicializado")

        # Obtener parámetros
        speed = gcmd.get_float('SPEED', self.speed, above=0.)
        second_speed = gcmd.get_float('SECOND_SPEED', self.second_speed, above=0.)
        retract_dist = gcmd.get_float('RETRACT_DIST', self.retract_dist, minval=0.)
        retract_speed = gcmd.get_float('RETRACT_SPEED', self.retract_speed, above=0.)
        
        # Usar la configuración del archivo printer.cfg
        # Por defecto mantiene el valor en printer.cfg, sólo lo cambia si se especifica en el comando
        final_retract = gcmd.get_int('FINAL_RETRACT', -1)
        if final_retract == -1:
            # Si no se especificó en el comando, usar el valor configurado
            final_retract = self.final_retract
        else:
            # Si se especificó en el comando, convertir de entero a booleano
            final_retract = final_retract != 0
            
        # Mostrar estado para debug
        gcmd.respond_info(f"Configuración: final_retract={final_retract} (valor en archivo={self.final_retract})")

        # Verificar estado de homing
        status = toolhead.get_status(self.printer.get_reactor().monotonic())
        z_homed = 'z' in status['homed_axes']
        x_homed = 'x' in status['homed_axes']
        y_homed = 'y' in status['homed_axes']

        gcmd.respond_info(f"ZMAX_HOME: Iniciando con velocidad:{speed} mm/s, velocidad fina:{second_speed} mm/s")
        if not z_homed:
            gcmd.respond_info("Eje Z no homeado. Realizando ZMAX_HOME relativo.")
        else:
            gcmd.respond_info("ZMAX_HOME: Iniciando búsqueda (eje Z ya homeado)...")

        # Guardar estado Gcode (posición relativa/absoluta)
        gcode_move = self.printer.lookup_object('gcode_move')
        gcode_state = gcode_move.get_status()['absolute_coordinates']
        
        # Verificar estado inicial del endstop
        initial_endstop_state = self.y_endstop.query_endstop(0)
        gcmd.respond_info(f"Estado inicial del endstop Y: {'ACTIVADO' if initial_endstop_state else 'NO ACTIVADO'}")

        try:
            # Encontrar Zmax
            # Pasamos retract_dist a _find_z_max
            final_pos_data = self._find_z_max(gcmd, speed, second_speed, retract_dist, z_homed)

            # Verificar que final_pos_data no sea None y tenga el formato esperado
            if final_pos_data is None:
                raise gcmd.error("Error: _find_z_max devolvió None")
                
            # Asegurar que final_pos_data sea una lista con al menos 3 elementos
            if not isinstance(final_pos_data, (list, tuple)) or len(final_pos_data) < 3:
                # Si no es una lista válida, usar la posición actual como fallback
                gcmd.respond_info("Formato de posición inválido, usando posición actual")
                final_pos_data = toolhead.get_position()
            
            # Obtener la posición Z máxima de la configuración
            z_max_cfg = self.z_stepper.get_range()[1]

            # Determinar la posición final Z
            # Si Z no estaba homeado, la posición final es z_max_cfg.
            # Si Z estaba homeado, usamos la posición medida del trigger devuelta por _find_z_max.
            final_z = z_max_cfg if not z_homed or final_pos_data[2] is None else final_pos_data[2]

            # Obtener posición actual completa (X, Y, Z, E)
            current_pos = toolhead.get_position()  # Llama a esto DESPUÉS de _find_z_max que puede cambiar el modo G90/G91
            
            # Asegurar que current_pos tiene 4 elementos (X, Y, Z, E)
            if len(current_pos) < 4:
                gcmd.respond_info(f"Advertencia: la posición actual no tiene 4 elementos: {current_pos}")
                # Extender la lista a 4 elementos añadiendo 0 para E si es necesario
                current_pos = list(current_pos) + [0.0] * (4 - len(current_pos))
            
            # Crear nueva posición manteniendo X, Y y E originales, solo cambiando Z
            new_pos = list(current_pos)  # Copia completa de la posición actual (incluye E)
            
            # Actualizar X e Y según el estado de homing
            new_pos[0] = current_pos[0] if x_homed else 0.0  # Usar 0 si no está homeado X
            new_pos[1] = current_pos[1] if y_homed else 0.0  # Usar 0 si no está homeado Y
            new_pos[2] = final_z  # Establecer Z a la posición final determinada

            if not z_homed:
                # Si Z no estaba homeado, marcarlo como homeado ahora en z_max_cfg
                toolhead.set_position(new_pos, homing_axes=('z',))
                gcmd.respond_info(f"ZMAX_HOME: Eje Z homeado en {final_z:.3f}mm")
            else:
                # Si Z ya estaba homeado, simplemente establecer su nueva posición medida
                toolhead.set_position(new_pos)
                gcmd.respond_info(f"ZMAX_HOME: Posición Z establecida a {final_z:.3f}mm")

            # Realizar retracción final para liberar presión SOLO si está habilitado
            if final_retract and retract_dist > 0:
                try:
                    # Crear una copia nueva de new_pos para la retracción
                    retract_pos = list(new_pos)  # Mantener la posición completa (4 elementos)
                    retract_pos[2] = final_z - retract_dist
                    
                    # Verificar límites de Z
                    z_min_cfg = self.z_stepper.get_range()[0]
                    if retract_pos[2] < z_min_cfg:
                        retract_pos[2] = z_min_cfg
                        gcmd.respond_info(f"Ajustando retracción final a Z={retract_pos[2]:.3f} para no exceder Zmin")
                        
                    gcmd.respond_info(f"ZMAX_HOME: Retracción final a Z={retract_pos[2]:.3f}mm")
                    toolhead.move(retract_pos, retract_speed)
                    toolhead.wait_moves()
                except Exception as e:
                    # Si falla la retracción final, solo mostramos advertencia pero no abortamos
                    gcmd.respond_info(f"Error en retracción final: {str(e)}")
            else:
                # Informar que no se realiza retracción final (quedándose en Zmax)
                gcmd.respond_info(f"ZMAX_HOME: Sin retracción final, quedando en posición Zmax ({final_z:.3f}mm)")

            # Restaurar estado Gcode
            if not gcode_state:
                self.gcode.run_script_from_command("G91")
                
            # Mensaje final de éxito
            gcmd.respond_info(f"ZMAX_HOME completado con éxito. Eje Z establecido a {final_z:.3f}mm")

        except Exception as e:
            # Asegurarse de volver a modo absoluto en caso de error
            try:
                self.gcode.run_script_from_command("G90")
            except Exception:
                pass # Ignorar errores durante la limpieza
            # Restaurar estado Gcode anterior si es posible
            finally:
                 if not gcode_state:
                     try:
                         self.gcode.run_script_from_command("G91")
                     except Exception:
                         pass
            # Si hay un error, desactivar motores solo si está configurado para hacerlo
            if self.disable_motors:
                stepper_enable = self.printer.lookup_object('stepper_enable')
                stepper_enable.motor_off()
                gcmd.respond_info("Motores desactivados debido a error.")
                
            raise gcmd.error(f"Error en ZMAX_HOME: {str(e)}")

    cmd_MEASURE_Z_LENGTH_help = "Mide la longitud real del eje Z entre Zmin y Zmax"
    def cmd_MEASURE_Z_LENGTH(self, gcmd):
        try:
            # Verificar que el sistema está listo
            if self.y_endstop is None:
                raise gcmd.error("MEASURE_Z_LENGTH: Endstop de Y no inicializado")
            
            # Primero hacer home en Zmin usando comandos directos
            gcmd.respond_info("Iniciando medición - Buscando Zmin...")
            
            # Obtener el objeto homing
            toolhead = self.printer.lookup_object('toolhead')
            homing = self.printer.lookup_object('homing')
            
            if homing is None:
                raise gcmd.error("No se pudo acceder al módulo de homing")
            
            # Realizar homing en Z
            try:
                gcmd.respond_info("Ejecutando homing en Z (G28 Z)...")
                self.gcode.run_script_from_command("G28 Z")
                gcmd.respond_info("Homing Z completado")
            except Exception as e:
                raise gcmd.error(f"Error durante homing Z: {str(e)}")
            
            # Esperar a que se complete el movimiento
            toolhead.wait_moves()
            
            # Obtener la posición después del home
            z_min_pos = toolhead.get_position()[2]
            gcmd.respond_info(f"Posición Zmin: {z_min_pos}mm")

            # Mover a una posición intermedia: 5mm por encima de Zmin
            speed = gcmd.get_float('SPEED', 15, above=0.)
            self.gcode.run_script_from_command("G91")
            curpos = list(toolhead.get_position())
            movepos = list(curpos)
            movepos[2] = z_min_pos + 5
            toolhead.move(movepos, speed)
            toolhead.wait_moves()

            # Fase 1: Aproximación gruesa en tramos de 1mm
            coarse_travel = 0.0
            max_coarse = 300  # límite de seguridad en mm
            gcmd.respond_info("Iniciando aproximación gruesa: incrementos de 1mm...")
            while coarse_travel < max_coarse:
                if self.y_endstop.query_endstop(0):
                    gcmd.respond_info(f"Endstop Y activado en aproximación gruesa tras {coarse_travel:.2f}mm")
                    break
                self.gcode.run_script_from_command(f"G1 Z1 F{speed*60}")
                toolhead.wait_moves()
                coarse_travel += 1.0
            else:
                raise gcmd.error("No se activó el endstop Y durante la aproximación gruesa.")

            # Retroceder 2mm para iniciar la aproximación fina
            gcmd.respond_info("Retrocediendo 2mm para aproximación fina...")
            self.gcode.run_script_from_command(f"G1 Z-2 F{speed*60}")
            toolhead.wait_moves()

            # Fase 2: Aproximación fina en tramos de 0.1mm
            fine_speed = gcmd.get_float('SECOND_SPEED', 2, above=0.)
            fine_travel = 0.0
            max_fine = 50  # límite de seguridad en mm
            gcmd.respond_info("Iniciando aproximación fina: incrementos de 0.1mm...")
            while fine_travel < max_fine:
                if self.y_endstop.query_endstop(0):
                    gcmd.respond_info(f"Endstop Y activado en aproximación fina tras {fine_travel:.2f}mm")
                    break
                self.gcode.run_script_from_command(f"G1 Z0.1 F{fine_speed*60}")
                toolhead.wait_moves()
                fine_travel += 0.1
            else:
                raise gcmd.error("No se activó el endstop Y durante la aproximación fina.")

            # Fase 3: Aproximación ultra fina en tramos de 0.01mm
            gcmd.respond_info("Iniciando fase ultra fina: retrocediendo 0.2mm para iniciar...")
            self.gcode.run_script_from_command(f"G1 Z-0.2 F{fine_speed*60}")
            toolhead.wait_moves()

            micro_travel = 0.0
            max_micro = 10  # límite de seguridad en mm para la fase ultra fina
            gcmd.respond_info("Iniciando fase ultra fina: incrementos de 0.01mm...")
            while micro_travel < max_micro:
                if self.y_endstop.query_endstop(0):
                    gcmd.respond_info(f"Endstop Y activado en fase ultra fina tras {micro_travel:.2f}mm")
                    break
                self.gcode.run_script_from_command(f"G1 Z0.01 F{fine_speed*60}")
                toolhead.wait_moves()
                micro_travel += 0.01
            else:
                raise gcmd.error("No se activó el endstop Y durante la fase ultra fina.")

            self.gcode.run_script_from_command("G90")

            # Calcular la longitud total medida desde la posición intermedia
            # Fórmula: (coarse_travel - 2.2mm) + fine_travel + micro_travel + 1mm
            z_length = (coarse_travel - 2.2) + fine_travel + micro_travel + 1.0

            gcmd.respond_info(
                f"Medición completada:\n"
                f"  Posición Zmin: {z_min_pos:.3f}mm\n"
                f"  Longitud total del eje Z: {z_length:.3f}mm"
            )
            
        except Exception as e:
            raise gcmd.error(f"Error en MEASURE_Z_LENGTH: {str(e)}")

def load_config(config):
    return ZMaxHomingAlt(config)