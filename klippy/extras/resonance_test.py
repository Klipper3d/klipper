# Archivo: klipper/klippy/extras/resonance_test.py

# Copyright (C) 2024 MicroLay
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging, math

class TestAxis:
    def __init__(self, axis='z'):
        self._name = axis
        self._vib_dir = (0., 0., 1.) if axis == 'z' else (0., 0., 0.)
    
    def get_name(self):
        return self._name
    
    def get_point(self, l):
        return (0., 0., self._vib_dir[2] * l)

class VibrationGenerator:
    def __init__(self, config):
        self.min_freq = config.getfloat('min_freq', 5., minval=1.)
        self.max_freq = config.getfloat('max_freq', 135., minval=self.min_freq, maxval=100000.)
        self.accel_per_hz = config.getfloat('accel_per_hz', 60., above=0.)
        self.hz_per_sec = config.getfloat('hz_per_sec', 1., minval=0.1, maxval=100.)
        self.default_amplitude = config.getfloat('amplitude', 1., above=0.)  # Amplitud en mm
    
    def prepare_test(self, gcmd):
        self.freq_start = gcmd.get_float("FREQ_START", self.min_freq, minval=1.)
        self.freq_end = gcmd.get_float("FREQ_END", self.max_freq, minval=self.freq_start, maxval=100000.)
        self.test_amplitude = gcmd.get_float("AMPLITUDE", self.default_amplitude, above=0.)
        self.test_hz_per_sec = gcmd.get_float("HZ_PER_SEC", self.hz_per_sec, above=0., maxval=100.)
        
        # Calcular aceleración basada en la amplitud deseada
        # Para un movimiento sinusoidal: a = (2πf)²A
        # donde f es la frecuencia y A es la amplitud
        self.test_accel_per_hz = (2. * math.pi) ** 2 * self.test_amplitude
    
    def gen_test(self):
        freq = self.freq_start
        res = []
        sign = 1.
        time = 0.
        # Asegurar un incremento mínimo para evitar quedarse atascado
        min_freq_increment = 0.01  # Incremento mínimo de 0.01 Hz
        
        # Caso especial: si freq_start == freq_end, generar al menos un ciclo
        if abs(self.freq_start - self.freq_end) < 0.00001:
            t_seg = .25 / freq
            accel = self.test_accel_per_hz * freq * freq  # a = (2πf)²A
            
            # Generar al menos 4 puntos (un ciclo completo)
            time += t_seg
            res.append((time, sign * accel, freq))
            time += t_seg
            res.append((time, -sign * accel, freq))
            time += t_seg
            res.append((time, -sign * accel, freq))
            time += t_seg
            res.append((time, sign * accel, freq))
            return res
        
        # Usar una condición más estricta para asegurar la terminación
        while freq < self.freq_end:
            t_seg = .25 / freq
            accel = self.test_accel_per_hz * freq * freq  # a = (2πf)²A
            time += t_seg
            res.append((time, sign * accel, freq))
            time += t_seg
            res.append((time, -sign * accel, freq))
            
            # Calcular el incremento basado en el algoritmo original
            freq_increment = 2. * t_seg * self.test_hz_per_sec
            
            # Asegurar que el incremento no sea demasiado pequeño
            if freq_increment < min_freq_increment:
                freq_increment = min_freq_increment
                
            # Prevenir incrementos demasiado grandes para frecuencias muy bajas
            if freq_increment > (self.freq_end - self.freq_start) / 10.0:
                freq_increment = (self.freq_end - self.freq_start) / 10.0
                
            freq += freq_increment
            
            # Asegurar que la última iteración llegue exactamente a freq_end
            if freq > self.freq_end - freq_increment and freq < self.freq_end:
                freq = self.freq_end
                
            sign = -sign
            
        # Añadir la frecuencia final si no está ya
        if len(res) > 0 and res[-1][2] < self.freq_end:
            t_seg = .25 / self.freq_end
            accel = self.test_accel_per_hz * self.freq_end * self.freq_end
            time += t_seg
            res.append((time, sign * accel, self.freq_end))
            time += t_seg
            res.append((time, -sign * accel, self.freq_end))
            
        return res

class ResonanceTest:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.axis = TestAxis('z')
        self.generator = VibrationGenerator(config)
        
        # Registrar comandos
        self.gcode.register_command(
            'RESONANCE_TEST_START',
            self.cmd_RESONANCE_TEST_START,
            desc=self.cmd_RESONANCE_TEST_START_help
        )
        self.gcode.register_command(
            'RESONANCE_TEST_STOP',
            self.cmd_RESONANCE_TEST_STOP,
            desc=self.cmd_RESONANCE_TEST_STOP_help
        )
        self.gcode.register_command(
            'RESONANCE_TEST_FIXED',
            self.cmd_RESONANCE_TEST_FIXED,
            desc=self.cmd_RESONANCE_TEST_FIXED_help
        )
        
        # Variables para el control de la prueba
        self.is_testing = False
    
    def _check_axis_homed(self, toolhead, axis, skip_home):
        status = toolhead.get_status(self.printer.get_reactor().monotonic())
        homed_axes = status.get('homed_axes', '')
        if axis in homed_axes or skip_home:
            return True
        return False
    
    cmd_RESONANCE_TEST_START_help = "Inicia la prueba de resonancia con movimientos oscilatorios"
    def cmd_RESONANCE_TEST_START(self, gcmd):
        if self.is_testing:
            raise gcmd.error("Ya hay una prueba de resonancia en curso")
        
        # Preparar la prueba
        self.generator.prepare_test(gcmd)
        test_seq = self.generator.gen_test()
        
        # Verificar que la secuencia de prueba no esté vacía
        if not test_seq:
            raise gcmd.error("No se pudo generar una secuencia de prueba válida con los parámetros proporcionados")
        
        # Obtener parámetros para controlar el homing
        no_home = gcmd.get_int("NO_HOME", 0)
        skip_pre_home = gcmd.get_int("SKIP_HOME", 0)
        
        # Obtener objetos necesarios
        reactor = self.printer.get_reactor()
        toolhead = self.printer.lookup_object('toolhead')
        
        # Verificar si Z está homeado, si es necesario
        if not self._check_axis_homed(toolhead, 'z', skip_pre_home):
            if not skip_pre_home:
                gcmd.respond_info("Ejecutando G28 Z antes de la prueba...")
                self.gcode.run_script_from_command("G28 Z")
            else:
                gcmd.respond_info("ADVERTENCIA: Eje Z no está homeado pero SKIP_HOME=1 fue especificado")
        
        # Guardar configuración actual
        systime = reactor.monotonic()
        toolhead_info = toolhead.get_status(systime)
        old_max_accel = toolhead_info['max_accel']
        old_max_z_accel = toolhead_info.get('max_z_accel', old_max_accel)
        old_max_velocity = toolhead_info['max_velocity']
        old_max_z_velocity = toolhead_info.get('max_z_velocity', old_max_velocity)
        
        # Obtener 'minimum_cruise_ratio' solo si existe, de lo contrario usar un valor predeterminado
        old_minimum_cruise_ratio = toolhead_info.get('minimum_cruise_ratio', 0.5)
        
        # Calcular aceleración máxima necesaria
        max_accel = max([abs(a) for _, a, _ in test_seq]) if test_seq else old_max_accel
        
        # Variables para restauración
        tmc_z = None
        old_thresh = None
        
        try:
            self.is_testing = True
            
            # Configurar límites de velocidad más altos para la prueba
            self.gcode.run_script_from_command(
                "SET_VELOCITY_LIMIT ACCEL=%.3f ACCEL_TO_DECEL=%.3f SQUARE_CORNER_VELOCITY=5 VELOCITY=500"
                % (max_accel, max_accel))
            
            # Deshabilitar el modo stealthchop durante la prueba
            tmc_z = self.printer.lookup_object('tmc5160 stepper_z', None)
            if tmc_z is not None:
                try:
                    old_thresh = tmc_z.get_register("TPWMTHRS")
                    tmc_z.set_register("TPWMTHRS", 0)
                except:
                    gcmd.respond_info("No se pudo modificar TPWMTHRS")
            
            # Obtener posición actual
            X, Y, Z, E = toolhead.get_position()
            
            # Ejecutar secuencia de prueba
            last_v = last_t = last_freq = 0.
            gcmd.respond_info("Iniciando prueba de resonancia de %.1f Hz a %.1f Hz" % 
                             (self.generator.freq_start, self.generator.freq_end))
            
            # Mostrar el número total de pasos para dar feedback de progreso
            total_steps = len(test_seq)
            current_step = 0
            
            for next_t, accel, freq in test_seq:
                current_step += 1
                if not self.is_testing:
                    break
                
                t_seg = next_t - last_t
                toolhead.cmd_M204(self.gcode.create_gcode_command(
                    "M204", "M204", {"S": abs(accel)}))
                
                # Calcular nueva velocidad y posición
                v = last_v + accel * t_seg
                abs_v = abs(v)
                if abs_v < 0.000001:
                    v = abs_v = 0.
                abs_last_v = abs(last_v)
                
                # Calcular desplazamiento
                v2 = v * v
                last_v2 = last_v * last_v
                half_inv_accel = .5 / accel if accel != 0 else 0
                d = (v2 - last_v2) * half_inv_accel
                _, _, dZ = self.axis.get_point(d)
                nZ = Z + dZ
                
                # Ejecutar movimiento
                toolhead.limit_next_junction_speed(abs_last_v)
                if v * last_v < 0:
                    # El movimiento primero se detiene y luego cambia de dirección
                    d_decel = -last_v2 * half_inv_accel
                    _, _, decel_Z = self.axis.get_point(d_decel)
                    toolhead.move([X, Y, Z + decel_Z, E], abs_last_v)
                    toolhead.move([X, Y, nZ, E], abs_v)
                else:
                    toolhead.move([X, Y, nZ, E], max(abs_v, abs_last_v))
                
                # Actualizar estado y mostrar progreso
                if math.floor(freq) > math.floor(last_freq):
                    progress = int((current_step / total_steps) * 100)
                    gcmd.respond_info("Probando frecuencia %.1f Hz (Progreso: %d%%)" % (freq, progress))
                
                Z = nZ
                last_t = next_t
                last_v = v
                last_freq = freq
            
            # Desacelerar al final si es necesario
            if last_v:
                d_decel = -.5 * last_v2 / old_max_accel
                _, _, decel_Z = self.axis.get_point(d_decel)
                toolhead.cmd_M204(self.gcode.create_gcode_command(
                    "M204", "M204", {"S": old_max_accel}))
                toolhead.move([X, Y, Z + decel_Z, E], abs(last_v))
            
            # Mensaje de finalización
            gcmd.respond_info("¡Prueba de resonancia completada con éxito!")
        
        finally:
            self.is_testing = False
            # Restaurar configuración original
            self.gcode.run_script_from_command(
                "SET_VELOCITY_LIMIT ACCEL=%.3f VELOCITY=%.3f SQUARE_CORNER_VELOCITY=5"
                % (old_max_accel, old_max_velocity))
            
            # Restaurar stealthchop si estaba activo
            if tmc_z is not None and old_thresh is not None:
                try:
                    tmc_z.set_register("TPWMTHRS", old_thresh)
                except:
                    gcmd.respond_info("No se pudo restaurar TPWMTHRS")
            
            # Volver a home en Z solo si no se especificó NO_HOME=1
            if not no_home:
                self.gcode.run_script_from_command("G28 Z")
                gcmd.respond_info("Eje Z homeado después de la prueba")
            else:
                gcmd.respond_info("Se omitió el homing del eje Z según lo solicitado")
            
            # Mensaje final incluso si hubo algún error
            if not gcmd.get_int("SILENT", 0):
                gcmd.respond_info("Prueba de resonancia finalizada y configuración restaurada")
    
    cmd_RESONANCE_TEST_STOP_help = "Detiene la prueba de resonancia en curso"
    def cmd_RESONANCE_TEST_STOP(self, gcmd):
        if not self.is_testing:
            gcmd.respond_info("No hay ninguna prueba de resonancia en curso")
            return
        self.is_testing = False
        gcmd.respond_info("Prueba de resonancia detenida")

    def _setup_test_conditions(self, gcmd, max_accel):
        # Obtener objetos necesarios
        reactor = self.printer.get_reactor()
        toolhead = self.printer.lookup_object('toolhead')
        
        # Guardar configuración actual
        systime = reactor.monotonic()
        toolhead_info = toolhead.get_status(systime)
        old_max_accel = toolhead_info['max_accel']
        old_max_velocity = toolhead_info['max_velocity']
        
        # Variables para restauración
        tmc_z = None
        old_thresh = None
        
        try:
            # Configurar límites de velocidad más altos para la prueba
            self.gcode.run_script_from_command(
                "SET_VELOCITY_LIMIT ACCEL=%.3f ACCEL_TO_DECEL=%.3f SQUARE_CORNER_VELOCITY=5 VELOCITY=500"
                % (max_accel, max_accel))
            
            # Deshabilitar el modo stealthchop durante la prueba
            tmc_z = self.printer.lookup_object('tmc5160 stepper_z', None)
            if tmc_z is not None:
                try:
                    old_thresh = tmc_z.get_register("TPWMTHRS")
                    tmc_z.set_register("TPWMTHRS", 0)
                except:
                    gcmd.respond_info("No se pudo modificar TPWMTHRS")
            
            return toolhead, old_max_accel, old_max_velocity, tmc_z, old_thresh
            
        except Exception as e:
            if tmc_z is not None and old_thresh is not None:
                try:
                    tmc_z.set_register("TPWMTHRS", old_thresh)
                except:
                    pass
            raise
    
    def _restore_settings(self, gcmd, old_max_accel, old_max_velocity, tmc_z, old_thresh):
        # Restaurar configuración original
        self.gcode.run_script_from_command(
            "SET_VELOCITY_LIMIT ACCEL=%.3f VELOCITY=%.3f SQUARE_CORNER_VELOCITY=5"
            % (old_max_accel, old_max_velocity))
        
        # Restaurar stealthchop si estaba activo
        if tmc_z is not None and old_thresh is not None:
            try:
                tmc_z.set_register("TPWMTHRS", old_thresh)
            except:
                gcmd.respond_info("No se pudo restaurar TPWMTHRS")
        
        # Volver a home en Z solo si no se especificó NO_HOME=1
        no_home = gcmd.get_int("NO_HOME", 0)
        skip_pre_home = gcmd.get_int("SKIP_HOME", 0)
        
        if not no_home and not skip_pre_home:
            self.gcode.run_script_from_command("G28 Z")
            gcmd.respond_info("Eje Z homeado después de la prueba")
        else:
            gcmd.respond_info("Se omitió el homing del eje Z según lo solicitado")
    
    cmd_RESONANCE_TEST_FIXED_help = "Ejecuta un movimiento oscilatorio con frecuencia fija"
    def cmd_RESONANCE_TEST_FIXED(self, gcmd):
        if self.is_testing:
            raise gcmd.error("Ya hay una prueba de resonancia en curso")
        
        # Obtener parámetros para controlar el homing
        skip_pre_home = gcmd.get_int("SKIP_HOME", 0)
        
        # Obtener y validar parámetros
        freq = gcmd.get_float('FREQ', above=0.)
        if freq > 100000:
            raise gcmd.error("Frecuencia demasiado alta. Máximo recomendado: 100000 Hz")
        
        accel_per_hz = gcmd.get_float('ACCEL_PER_HZ', 60., above=0.)
        duration = gcmd.get_float('DURATION', 5., above=0.)
        
        # Calcular aceleración basada en ACCEL_PER_HZ
        accel = accel_per_hz * freq
        
        # Obtener toolhead
        toolhead = self.printer.lookup_object('toolhead')
        
        # Verificar si Z está homeado, si es necesario
        if not self._check_axis_homed(toolhead, 'z', skip_pre_home):
            if not skip_pre_home:
                gcmd.respond_info("Ejecutando G28 Z antes de la prueba...")
                self.gcode.run_script_from_command("G28 Z")
            else:
                gcmd.respond_info("ADVERTENCIA: Eje Z no está homeado pero SKIP_HOME=1 fue especificado")
        
        try:
            self.is_testing = True
            
            # Configurar condiciones de prueba
            toolhead, old_max_accel, old_max_velocity, tmc_z, old_thresh = self._setup_test_conditions(gcmd, accel)
            gcmd.respond_info("Configuración inicial completada")
            
            # Obtener el stepper Z
            kin = toolhead.get_kinematics()
            steppers = [s for s in kin.get_steppers() if s.get_name() == 'stepper_z']
            if not steppers:
                raise gcmd.error("No se encontró el stepper Z")
            stepper = steppers[0]
            
            # Obtener posición actual
            X, Y, Z, E = toolhead.get_position()
            gcmd.respond_info("Posición inicial: X=%.3f Y=%.3f Z=%.3f" % (X, Y, Z))
            
            # Calcular parámetros del movimiento
            period = 1. / freq  # Periodo en segundos
            t_seg = period / 4.  # Tiempo por segmento (1/4 del periodo)
            mcu = stepper.get_mcu()
            print_time = mcu.estimated_print_time(toolhead.get_last_move_time())
            clock = mcu.print_time_to_clock(print_time)
            
            # Calcular tiempos en ciclos de reloj
            cycle_ticks = mcu.seconds_to_clock(period)
            
            # Generar comandos de movimiento
            gcmd.respond_info("Generando comandos de movimiento...")
            
            # Obtener el comando de paso
            step_cmd = mcu.lookup_command(
                "queue_step oid=%c interval=%u count=%hu add=%hi",
                "queue_step oid=%c interval=%u count=%hu add=%hi")
            
            # Calcular parámetros de movimiento
            interval = cycle_ticks // 4  # Dividir el ciclo en 4 partes
            steps = 100  # Número de pasos por segmento
            
            # Enviar comandos de movimiento
            for i in range(int(duration * freq)):
                # Primer cuarto (hacia arriba)
                step_cmd.send([stepper.get_oid(), interval, steps, 1])
                # Segundo cuarto (desaceleración)
                step_cmd.send([stepper.get_oid(), interval, steps, -1])
                # Tercer cuarto (hacia abajo)
                step_cmd.send([stepper.get_oid(), interval, steps, -1])
                # Cuarto cuarto (desaceleración)
                step_cmd.send([stepper.get_oid(), interval, steps, 1])
                
                if i % 10 == 0:
                    gcmd.respond_info("Ciclo %d enviado" % i)
            
            gcmd.respond_info("Comandos de movimiento enviados")
            
        finally:
            self.is_testing = False
            self._restore_settings(gcmd, old_max_accel, old_max_velocity, tmc_z, old_thresh)

def load_config(config):
    return ResonanceTest(config)
