import copy
import logging
import math

from .bmcu_core import protocol
from .bmcu_core.release import PACKAGE_VERSION, REQUIRED_FIRMWARE
from .bmcu_cortador_ender import CortadorEnder
from .bmcu_english_api import public_diagnostics, require_english_parameters
from . import bmcu_ender_panel


class EnderIntegration:
    def __init__(self, config):
        if PACKAGE_VERSION != '1.1.2':
            raise config.error('This integration requires BMCU-Klipper 1.1.2.')
        if REQUIRED_FIRMWARE != (1, 0, 4):
            raise config.error('This integration requires BMCU firmware 1.0.4.')
        self.printer = config.get_printer()
        self.bmcu = self.printer.load_object(config, 'bmcu')
        self.measurement = self.printer.load_object(config, 'bmcu_measurement')
        self.gcode = self.printer.lookup_object('gcode')
        self.reactor = self.printer.get_reactor()
        self.device = config.get('device', 'bmcu0')
        self.destination = config.get('destination', 'extruder')

        def finite_float(name, default, **limits):
            value = config.getfloat(name, default, **limits)
            if not math.isfinite(value):
                raise config.error('%s must be a finite number.' % name)
            return value

        self.capture = finite_float('capture_mm', 20.0, above=0.0)
        self.capture_speed = finite_float('capture_feedrate', 180.0, above=0.0)
        self.advance = finite_float('nozzle_feed_mm', 80.0, above=0.0)
        self.purge_distance = finite_float('purge_mm', 35.0, above=0.0)
        self.advance_speed = finite_float('feed_feedrate', 120.0, above=0.0)
        self.pullback_speed = finite_float('retract_feedrate', 300.0, above=0.0)
        self.unload_buffer_limit = finite_float(
            'unload_buffer_limit', 70.0, minval=0.0, maxval=100.0
        )
        self.unload_buffer_resume = finite_float(
            'unload_buffer_resume', 65.0, minval=0.0, maxval=100.0
        )
        if self.unload_buffer_resume >= self.unload_buffer_limit:
            raise config.error('Unload buffer resume must be below its limit.')
        self.unload_buffer_timeout = finite_float(
            'unload_buffer_timeout', 3.0, above=0.0
        )
        self.unload_progress_timeout = finite_float(
            'unload_progress_timeout', 3.0, above=0.0
        )
        self.unload_step = finite_float('unload_step_mm', 1.0, above=0.0)
        self.unload_slack = finite_float('unload_slack_mm', 2.0, above=0.0)
        self.pullback = finite_float(
            'pullback_after_sensor_mm', 55.0, above=0.0
        )
        self.timeout = finite_float('sensor_timeout', 60.0, above=0.0)
        self.purge_x = finite_float('purge_x', -6.0)
        self.wipe_x = finite_float('wipe_x', -3.0)
        self.wipe_speed = finite_float('wipe_feedrate', 600.0, above=0.0)
        self.purge_travel_speed = finite_float(
            'purge_travel_feedrate', 1200.0, above=0.0
        )
        self.preheat_park_threshold = finite_float(
            'preheat_park_threshold', 150.0, minval=0.0
        )
        self.purge_y = finite_float('purge_y', 123.0)
        self.z_lift = finite_float('z_lift', 2.0, above=0.0)
        self.wipe_passes = config.getint('wipes', 6, minval=1)
        self.temporary_pullbacks = {}
        self.manual_operation = None
        self.pending_tool_change = None
        self.recovering_tool_change = False
        self.last_unload_timing = {}
        self.cortador = CortadorEnder(self)
        self.posicionamento_pendente = None
        self.chamada_temperatura_original = getattr(
            self.bmcu, '_endpoint_temperature_call', None
        )
        if callable(self.chamada_temperatura_original):
            self.bmcu._endpoint_temperature_call = self.antecipar_carga
        self.original_wait_pullback = self.bmcu._wait_pullback_safe
        self.bmcu._wait_pullback_safe = self.wait_pullback
        self.gcode.register_command(
            'BMCU_ENDER_LOAD', self.protect(self.load)
        )
        self.gcode.register_command(
            'BMCU_ENDER_UNLOAD', self.protect(self.release)
        )
        self.gcode.register_command(
            'BMCU_CHANGE_TOOL_ENDER', self.protect(self.change_tool)
        )
        self.gcode.register_command(
            'BMCU_ENDER_MANUAL', self.protect(self.manual_filament)
        )
        self.gcode.register_command(
            'BMCU_CONFIRM_CLEAR_PATH',
            self.protect(self.confirm_recovery_path),
        )
        self.gcode.register_command(
            'BMCU_ENDER_SERVO', self.protect(self.cortador.servo_manual)
        )
        self.gcode.register_command(
            'BMCU_ENDER_TEST_CUTTER', self.protect(self.cortador.testar)
        )
        self.gcode.register_command(
            'BMCU_ENDER_RESET', self.protect(self.resetar_bmcu)
        )
        self.gcode.register_command(
            'BMCU_ENDER_MOVE_TO_PURGE',
            self.protect(self.posicionar_para_aquecimento),
        )
        self.gcode.register_command(
            'BMCU_ENDER_SET_FILAMENT', self.protect(self.set_panel_filament)
        )
        self.printer.register_event_handler('klippy:ready', self.ready)
        for event in ('stepper_enable:motor_off', 'homing:home_rails_begin'):
            self.printer.register_event_handler(
                event, self.invalidate_recovery
            )

    def ready(self):
        for name in ('M104', 'M109', 'SET_HEATER_TEMPERATURE'):
            previous = self.gcode.register_command(name, None)
            if previous is None:
                raise self.printer.config_error(
                    'Command %s was not found.' % name
                )
            self.gcode.register_command(
                name, self.guard_hotend_temperature(name, previous)
            )
        homing_anterior = self.gcode.register_command('G28', None)
        if homing_anterior is None:
            raise self.printer.config_error('Command G28 was not found.')
        self.gcode.register_command(
            'G28', self.protect(self.validar_homing_cortador(homing_anterior))
        )
        for name in ('RESUME', 'CANCEL_PRINT', 'SDCARD_RESET_FILE'):
            previous = self.gcode.register_command(name, None)
            if previous is None:
                raise self.printer.config_error(
                    'Command %s was not found.' % name
                )
            self.gcode.register_command(
                name,
                self.protect(
                    self.resume_print(previous)
                    if name == 'RESUME'
                    else self.cancel_recovery(previous)
                ),
            )
        for name in (
            'BMCU_LOAD',
            'BMCU_UNLOAD',
            'BMCU_TOOL_CHANGE',
            'BMCU_PRINT_END',
        ):
            previous = self.gcode.register_command(name, None)
            if previous is None:
                raise self.printer.config_error(
                    'Command %s was not found.' % name
                )
            self.gcode.register_command(
                name, self.protect(self.adapt_operation(previous))
            )
        for index in range(4):
            name = 'T%d' % index
            previous = self.gcode.register_command(name, None)
            if previous is None:
                previous = self.native_tool(index)
            self.gcode.register_command(
                name, self.protect(self.adapt_tool(index, previous))
            )

    def protect(self, function):
        def execute(gcmd):
            try:
                require_english_parameters(gcmd)
                return function(gcmd)
            except Exception as error:
                raise gcmd.error(str(error)) from error

        return execute

    def validar_homing_cortador(self, anterior):
        def executar(gcmd):
            perfil = self.cortador.preparar()
            if not perfil:
                return anterior(gcmd)
            self.cortador.validar_homing(perfil)
            continuar = anterior
            homing_seguro = self.printer.lookup_object('safe_z_home', None)
            if homing_seguro is not None and anterior == homing_seguro.cmd_G28:
                # The first segment performs safe_z_home's initial lift;
                # continue its native axis/probe logic without lifting twice.
                continuar = homing_seguro.home_axes
            parametros = dict(gcmd.get_command_parameters())
            todos = not any(eixo in parametros for eixo in 'XYZ')
            comuns = {
                chave: valor for chave, valor in parametros.items()
                if chave not in ('X', 'Y', 'Z')
            }
            # Call the captured handler, not the registered wrapper: X must
            # finish at its real endstop before any servo command is issued.
            resultado = anterior(self.gcode.create_gcode_command(
                'G28', 'G28', dict(comuns, X='0')
            ))
            self.cortador.recolher_para_homing(perfil)
            restantes = {
                eixo: parametros.get(eixo, '0') for eixo in 'YZ'
                if todos or eixo in parametros
            }
            if restantes:
                return continuar(self.gcode.create_gcode_command(
                    'G28', 'G28', dict(comuns, **restantes)
                ))
            return resultado

        return executar

    def native_tool(self, index):
        def execute(gcmd):
            return self.gcode.run_script_from_command(
                'BMCU_TOOL_CHANGE TOOL=%d' % index
            )

        return execute

    def restore_pullbacks(self):
        for name in tuple(self.temporary_pullbacks):
            device = self.bmcu.devices_by_name[name]
            device.stop_all()
            self.bmcu._send_device_runtime_config(device)
            self.temporary_pullbacks.pop(name)

    def wait_pullback(self, device, channel, *args, **kwargs):
        record = self.temporary_pullbacks.get(device.name)
        if record is None or record['channel'] != channel:
            return self.original_wait_pullback(
                device, channel, *args, **kwargs
            )
        kwargs['start_m'] = record['start']
        try:
            result = self.original_wait_pullback(
                device, channel, *args, **kwargs
            )
        except Exception:
            self.record_parking_timing(record, 'parking_failed')
            raise
        try:
            self.reactor.pause(self.reactor.monotonic() + 0.150)
            device.refresh()
            measured = (
                record['start'] - float(device.status['meters'][channel])
            ) * 1000.0
            if device.status['session_id'] != record['session']:
                raise RuntimeError('The BMCU restarted during pullback.')
            if any(device.status['motion']) or any(device.status['motor_pwm']):
                raise RuntimeError(
                    'The BMCU did not remain idle after parking.'
                )
            if not math.isfinite(measured) or measured + 0.1 < self.pullback:
                raise RuntimeError(
                    (
                        (
                            'The encoder confirmed only %.2f mm after the '
                            'entry sensor; the next channel will not be '
                            'loaded.'
                        )
                    )
                    % measured
                )
            if self.sensor(self.bmcu.endpoints[self.destination]):
                raise RuntimeError(
                    (
                        (
                            'The entry sensor detected filament again during '
                            'pullback.'
                        )
                    )
                )
        except Exception:
            self.record_parking_timing(record, 'parking_failed')
            device.stop_all()
            raise
        result = dict(result)
        result['encoder_mm'] = measured
        result['after_switch_encoder_mm'] = measured
        timing = self.record_parking_timing(record, 'complete')
        if timing is not None:
            self.gcode.respond_info(
                'Channel %d unload timing: positioning %.2f s; tip %.2f s; '
                'sensor release %.2f s; feeder parking %.2f s; total %.2f s.'
                % (
                    channel + 1,
                    timing['position_seconds'],
                    timing['tip_seconds'],
                    timing['release_seconds'],
                    timing['parking_seconds'],
                    timing['total_seconds'],
                )
            )
        self.gcode.respond_info(
            (
                (
                    'Channel %d: encoder-confirmed pullback after the entry '
                    'sensor: %.2f '
                    'mm.'
                )
            )
            % (channel + 1, measured)
        )
        return result

    def record_parking_timing(self, record, result):
        timing = record.get('timing')
        if timing is None:
            return None
        now = self.reactor.monotonic()
        timing['parking_seconds'] = now - record['parking_started']
        timing['total_seconds'] = now - record['unload_started']
        timing['result'] = result
        self.last_unload_timing = dict(timing)
        return timing

    def adapt_operation(self, previous):
        def execute(gcmd):
            try:
                return previous(gcmd)
            finally:
                self.restore_pullbacks()
                self.posicionamento_pendente = None

        return execute

    def adapt_tool(self, index, previous):
        def execute(gcmd):
            if 'B' not in gcmd.get_command_parameters():
                if not self.bmcu.print_map_active:
                    return previous(gcmd)
                tool = index + 1
                if not self.bmcu.print_tools.get(str(tool)):
                    raise gcmd.error(
                        (
                            'Orca tool %d is missing from the committed BMCU '
                            'print map.'
                        )
                        % tool
                    )
            else:
                tool = gcmd.get_int('B', minval=1, maxval=4)
            if tool != index + 1:
                raise gcmd.error(
                    ('The Orca index and BMCU channel do not match.')
                )
            return self.gcode.run_script_from_command(
                'BMCU_CHANGE_TOOL_ENDER TOOL=%d' % tool
            )

        return execute

    def execute(self, text):
        with self.bmcu.printer_critical_section('integracao_ender'):
            self.gcode.run_script_from_command(text)

    def sensor(self, destination):
        if getattr(self, 'manual_operation', None) is not None:
            self.check_manual_operation()
        status = destination.sensor_status('entry_sensor')
        if not status or status.get('filament_detected') not in (True, False):
            raise RuntimeError(
                ('The entry sensor did not return a valid reading.')
            )
        return bool(status['filament_detected'])

    def get_status(self, eventtime):
        pending = getattr(self, 'pending_tool_change', None)
        return {
            'manual_busy': self.manual_operation is not None,
            'recovery_pending': pending is not None,
            'recovery_tool': pending['tool'] if pending else -1,
            'recovery_channel': (
                pending.get('failed_channel') if pending else -1),
            'recovery_error': pending['error'] if pending else '',
            'unload_timing': public_diagnostics(
                getattr(self, 'last_unload_timing', {})),
            'cutter_uncertain': getattr(self.cortador, 'incerto', False),
            'last_cut': public_diagnostics(self.cortador.ultimo_corte),
            'panel': bmcu_ender_panel.panel_status(self),
        }

    def set_panel_filament(self, gcmd):
        return bmcu_ender_panel.set_panel_filament(self, gcmd)

    def resetar_bmcu(self, gcmd):
        if gcmd.get_int('CONFIRM', 0, minval=0, maxval=1) != 1:
            raise gcmd.error(
                'Manually remove filament from the hotend and shared path; '
                'then use RESET_BMCU CONFIRM=1.'
            )
        estado = (
            self.printer.lookup_object('print_stats')
            .get_status(self.reactor.monotonic())
            .get('state')
        )
        if estado in ('printing', 'paused', 'pause'):
            raise gcmd.error('Cancel the print before resetting BMCU.')
        if self.bmcu.active_operations or self.manual_operation is not None:
            raise gcmd.error('Wait for operations to finish before resetting.')
        if len(self.bmcu.devices) != 1:
            raise gcmd.error('This reset requires exactly one BMCU.')
        dispositivo = self.bmcu.devices_by_name.get(self.device)
        destino = self.bmcu.endpoints.get(self.destination)
        if dispositivo is None or not dispositivo.ready or destino is None:
            raise gcmd.error('The BMCU or endpoint is not ready.')
        if self.sensor(destino):
            raise gcmd.error('The entry sensor still detects filament.')
        for canal in range(4):
            if (
                self.bmcu._endpoint_for_channel(dispositivo, canal)
                is not destino
            ):
                raise gcmd.error('A channel is assigned to another endpoint.')
        self.gcode.run_script_from_command('BMCU_STOP')
        self.restore_pullbacks()
        dispositivo.refresh()
        sessao = dispositivo.status['session_id']
        resultados = []
        for canal in range(4):
            dispositivo.refresh()
            if dispositivo.status['session_id'] != sessao or self.sensor(
                destino
            ):
                raise gcmd.error('Physical state changed; reset interrupted.')
            if any(dispositivo.status['motor_pwm']) or any(
                int(movimento) != protocol.MOTION_IDLE
                for movimento in dispositivo.status['motion']
            ):
                raise gcmd.error('The BMCU is still moving.')
            presentes = dispositivo.status.get('present')
            if not isinstance(presentes, (list, tuple)) or len(presentes) != 4:
                raise gcmd.error(
                    'The four channel sensor readings are missing.')
            if any(valor not in (False, True, 0, 1) for valor in presentes):
                raise gcmd.error('Invalid BMCU channel sensor readings.')
            confirmado = 'PARKED' if presentes[canal] else 'EMPTY'
            self.gcode.run_script_from_command(
                'BMCU_ROUTE_CONFIRM DEVICE=%s CHANNEL=%d STATE=%s'
                % (dispositivo.name, canal, confirmado)
            )
            resultados.append('%d=%s' % (canal + 1, confirmado))
        dispositivo.refresh()
        if (
            self.sensor(destino)
            or dispositivo.status['session_id'] != sessao
            or any(
                self.bmcu._route_state(dispositivo, canal)
                != protocol.ROUTE_EMPTY
                for canal in range(4)
            )
        ):
            raise gcmd.error(
                'Could not confirm that all filament paths are clear.'
            )
        self.gcode.run_script_from_command('BMCU_PRINT_END MODE=KEEP CLEAR=1')
        if self.bmcu.print_map_active or self.bmcu.print_plan_open:
            raise gcmd.error('The previous print session still needs recovery.')
        self.pending_tool_change = None
        self.posicionamento_pendente = None
        gcmd.respond_info(
            'Hotend confirmed unloaded; no filament was moved. '
            + ', '.join(resultados)
        )

    def antecipar_carga(self, destino, comando, *args, **kwargs):
        resultado = self.chamada_temperatura_original(
            destino, comando, *args, **kwargs
        )
        if (
            comando != 'prepare_load'
            or destino is not self.bmcu.endpoints.get(self.destination)
        ):
            return resultado
        perfil = self.cortador.preparar()
        if perfil is None or not perfil.get('posicionamento_paralelo'):
            return resultado
        if (
            len(self.bmcu.devices) != 1
            or self.posicionamento_pendente is not None
        ):
            return resultado
        dispositivo = self.bmcu.devices_by_name[self.device]
        dispositivo.refresh()
        if self.sensor(destino) or any(
            self.bmcu._route_state(dispositivo, canal) != protocol.ROUTE_EMPTY
            for canal in range(4)
        ):
            return resultado
        verificar_cauda = getattr(self.bmcu, '_durable_tail_route', None)
        if callable(verificar_cauda) and any(
            verificar_cauda(dispositivo, canal) is not None
            for canal in range(4)
        ):
            return resultado
        cabecote = self.printer.lookup_object('toolhead')
        estado = cabecote.get_status(self.reactor.monotonic())
        if not all(eixo in estado['homed_axes'] for eixo in 'xyz'):
            return resultado
        if (
            not self.printer.lookup_object('extruder')
            .get_status(self.reactor.monotonic())
            .get('can_extrude')
        ):
            return resultado
        alvo = self.posicao_purga(cabecote, estado)
        self.cortador.recolher(perfil)
        with self.bmcu.printer_critical_section('posicionamento_carga'):
            cabecote.manual_move([None, None, alvo[2]], 5)
            cabecote.manual_move(
                [self.purge_x, None, None], self.purge_travel_speed / 60
            )
            cabecote.manual_move(
                [None, self.purge_y, None], self.purge_travel_speed / 60
            )
            cabecote.get_last_move_time()
        self.posicionamento_pendente = alvo
        return resultado

    def cancel_recovery(self, previous):
        def execute(gcmd):
            self.pending_tool_change = None
            return previous(gcmd)

        return execute

    def invalidate_recovery(self, *args):
        pending = getattr(self, 'pending_tool_change', None)
        if pending is not None:
            pending['position_lost'] = True

    def hold_failed_tool_change(self, gcmd, tool, error, nozzle_target):
        if getattr(self, 'recovering_tool_change', False):
            return False
        sd = self.printer.lookup_object('virtual_sdcard', None)
        pause = self.printer.lookup_object('pause_resume', None)
        if (
            sd is None
            or pause is None
            or sd.current_file is None
            or not sd.is_cmd_from_sd()
            or self.printer.is_shutdown()
        ):
            return False
        stats = self.printer.lookup_object('print_stats').get_status(
            self.reactor.monotonic()
        )
        if stats['state'] not in ('printing', 'paused'):
            return False
        move = self.printer.lookup_object('gcode_move')
        saved = copy.deepcopy(move.saved_states['BMCU_ENDER_TROCA'])
        try:
            self.bmcu.devices_by_name[self.device].stop_all()
        except Exception:
            logging.exception(
                'Could not confirm the BMCU stopped before pausing.'
            )
        last_error = self.bmcu.get_status(self.reactor.monotonic()).get(
            'last_error'
        )
        self.pending_tool_change = {
            'tool': tool,
            'error': str(error),
            'file': sd.current_file,
            'position': sd.get_file_position(),
            'state': saved,
            'nozzle_target': nozzle_target,
            'session': self.bmcu.devices_by_name[self.device].status[
                'session_id'
            ],
            'failed_channel': (
                last_error.get('channel')
                if isinstance(last_error, dict)
                and last_error.get('device') == self.device
                else None
            ),
        }
        pause.cmd_PAUSE(gcmd)
        move.saved_states['PAUSE_STATE'] = copy.deepcopy(saved)
        gcmd.respond_info(
            'Print paused with tool T%d pending: %s. Check the filament path, '
            'then RESUME will finish this tool change before printing.'
            % (tool, error)
        )
        return True

    def validate_print_recovery(self, check_routes=True):
        pending = self.pending_tool_change
        if pending is None:
            raise RuntimeError('There is no pending print recovery.')
        sd = self.printer.lookup_object('virtual_sdcard')
        pause = self.printer.lookup_object('pause_resume')
        if (
            not pause.is_paused
            or sd.is_active()
            or sd.current_file is not pending['file']
            or sd.file_position != pending['position']
            or self.printer.is_shutdown()
            or pending.get('position_lost')
        ):
            raise RuntimeError(
                'The paused print changed; recovery is blocked.'
            )
        if self.bmcu.active_operations:
            raise RuntimeError('A BMCU operation is still active.')
        head = self.printer.lookup_object('toolhead').get_status(
            self.reactor.monotonic()
        )
        if not all(axis in head['homed_axes'] for axis in 'xyz'):
            raise RuntimeError('Position was lost; print recovery is blocked.')
        move = self.printer.lookup_object('gcode_move')
        if list(move.homing_position) != pending['state']['homing_position']:
            raise RuntimeError(
                'The coordinate offset changed during recovery.'
            )
        device = self.bmcu.devices_by_name[self.device]
        if not device.ready or getattr(device, 'last_error', ''):
            raise RuntimeError('The BMCU is not ready for recovery.')
        device.refresh()
        if device.status['session_id'] != pending['session']:
            raise RuntimeError(
                'The BMCU session changed; print recovery is blocked.'
            )
        if not check_routes:
            return device
        routes = [self.bmcu._route_state(device, ch) for ch in range(4)]
        if protocol.ROUTE_UNCERTAIN in routes:
            raise RuntimeError(
                'An uncertain filament route needs physical recovery; '
                'the print remains paused.'
            )
        loaded = routes.count(protocol.ROUTE_LOADED)
        detected = self.sensor(self.bmcu.endpoints[self.destination])
        if loaded > 1 or detected != (loaded == 1):
            raise RuntimeError(
                'The entry sensor and filament routes disagree.'
            )

    def confirm_recovery_path(self, gcmd):
        channel = gcmd.get_int('CHANNEL', minval=1, maxval=4) - 1
        if gcmd.get_int('CONFIRM', 0, minval=0, maxval=1) != 1:
            raise gcmd.error(
                'Physically clear the shared path and park this filament '
                'behind the entry sensor before CONFIRM=1.'
            )
        device = self.validate_print_recovery(check_routes=False)
        if self.pending_tool_change.get('failed_channel') != channel:
            raise gcmd.error('This channel does not match the paused failure.')
        destination = self.bmcu.endpoints[self.destination]
        if self.bmcu._durable_tail_route(device, channel) is not None:
            raise gcmd.error(
                'A detached filament tail requires separate recovery.'
            )
        self.bmcu._lock(
            device, destination, 'ENDER PRINT RECOVERY', channel=channel
        )
        try:
            previous = copy.deepcopy(device.refresh())
            if (
                self.sensor(destination)
                or any(
                    int(value) != protocol.MOTION_IDLE
                    for value in previous['motion']
                )
                or any(int(value) != 0 for value in previous['motor_pwm'])
                or any(
                    self.bmcu._route_state(device, ch) != protocol.ROUTE_EMPTY
                    for ch in range(4)
                    if ch != channel
                )
            ):
                raise RuntimeError(
                    'The shared path is not confirmed clear and idle.'
                )
            route = self.bmcu._route_key(device, channel)
            self.bmcu._uncertain_routes.add(route)
            device.mark_unloaded(channel)
            current = copy.deepcopy(device.refresh())
            if (
                self.bmcu._route_states_from_status(current)[channel]
                != protocol.ROUTE_EMPTY
            ):
                raise RuntimeError('The BMCU did not confirm an empty route.')
            self.bmcu.loaded_tools.pop(route, None)
            self.bmcu.active_tool = -1
            self.bmcu._unmark_print_route(device, channel)
            self.bmcu._save_runtime()
            self.bmcu.device_status_changed(device, previous, current)
            self.bmcu._uncertain_routes.discard(route)
            gcmd.respond_info(
                'Channel %d route confirmed clear. Print mapping preserved; '
                'RESUME will finish the pending tool change.' % (channel + 1)
            )
        finally:
            self.bmcu._unlock(device, destination)

    def resume_print(self, previous):
        def execute(gcmd):
            pending = getattr(self, 'pending_tool_change', None)
            if pending is None:
                return previous(gcmd)
            self.validate_print_recovery()
            self.recovering_tool_change = True
            try:
                self.heat_manual_filament()
                self.gcode.run_script_from_command(
                    'BMCU_CHANGE_TOOL_ENDER TOOL=%d PURGE_IF_LOADED=1'
                    % pending['tool']
                )
                device, channel, destination = self.bmcu._resolve_tool(
                    pending['tool']
                )
                device.refresh()
                if (
                    self.bmcu._route_state(device, channel)
                    != protocol.ROUTE_LOADED
                    or not self.sensor(destination)
                    or self.bmcu.get_status(self.reactor.monotonic()).get(
                        'loaded_tool'
                    )
                    != pending['tool']
                ):
                    raise RuntimeError(
                        'The pending filament was not confirmed.'
                    )
                self.execute('M109 S%.3f' % pending['nozzle_target'])
            except Exception as error:
                pending['error'] = str(error)
                self.bmcu.devices_by_name[self.device].stop_all()
                raise
            finally:
                self.recovering_tool_change = False
            move = self.printer.lookup_object('gcode_move')
            move.saved_states['PAUSE_STATE'] = copy.deepcopy(pending['state'])
            result = previous(gcmd)
            self.pending_tool_change = None
            return result

        return execute

    def manual_context(self):
        if self.printer.lookup_object('print_stats').get_status(
            self.reactor.monotonic()
        ).get('state') in ('printing', 'paused'):
            raise RuntimeError(
                'Finish or cancel the print before using channels.'
            )
        if self.bmcu.active_operations or self.bmcu.print_plan_open:
            raise RuntimeError(
                'A BMCU operation or print plan is in progress.'
            )
        if self.bmcu.print_map_active:
            raise RuntimeError(
                'Finish the BMCU print context before using channels.'
            )
        device = self.bmcu.devices_by_name.get(self.device)
        destination = self.bmcu.endpoints.get(self.destination)
        if (
            device is None
            or not device.ready
            or destination is None
            or getattr(device, 'last_error', '')
        ):
            raise RuntimeError('The BMCU is not ready.')
        device.refresh()
        if len(self.bmcu.devices) != 1:
            raise RuntimeError('Manual controls require one BMCU.')
        loaded = []
        for channel in range(4):
            route = self.bmcu._route_state(device, channel)
            if route == protocol.ROUTE_UNCERTAIN:
                raise RuntimeError(
                    'Check the uncertain BMCU route before continuing.'
                )
            if route == protocol.ROUTE_LOADED:
                loaded.append(channel)
        if len(loaded) > 1:
            raise RuntimeError('More than one BMCU channel is marked loaded.')
        current = loaded[0] if loaded else None
        detected = self.sensor(destination)
        if detected != (current is not None):
            raise RuntimeError(
                'The entry sensor and loaded BMCU route disagree.'
            )
        return device, destination, current

    def check_manual_operation(self):
        if self.printer.is_shutdown():
            raise RuntimeError('The printer is shut down.')
        if getattr(self, 'recovering_tool_change', False):
            self.validate_print_recovery()
            return
        display = self.printer.lookup_object('e3v3se_display', None)
        session = getattr(display, 'bmcu_session', None)
        if session and session.get('cancelled'):
            raise RuntimeError('Filament operation cancelled.')
        if self.printer.lookup_object('print_stats').get_status(
            self.reactor.monotonic()
        ).get('state') in ('printing', 'paused'):
            raise RuntimeError(
                'A print started during the filament operation.'
            )

    def heat_manual_filament(self):
        self.check_manual_operation()

        def check_parking():
            # Normal manual cancellation is cached and safe inside motion's
            # critical section. Recovery validation can refresh UART, so keep
            # its existing full checks before and after parking instead.
            if not getattr(self, 'recovering_tool_change', False):
                self.check_manual_operation()

        self.prepare_preheat(
            230.0, allow_paused=getattr(self, 'recovering_tool_change', False),
            check=check_parking,
        )
        self.check_manual_operation()
        self.gcode.run_script_from_command(
            'SET_HEATER_TEMPERATURE HEATER=extruder TARGET=230'
        )
        extruder = self.printer.lookup_object('extruder')
        deadline = self.reactor.monotonic() + 300.0
        while True:
            self.check_manual_operation()
            status = extruder.get_status(self.reactor.monotonic())
            if status['target'] != 230.0:
                raise RuntimeError('Nozzle target changed; operation aborted.')
            if status['can_extrude'] and status['temperature'] >= 229.0:
                return
            if self.reactor.monotonic() >= deadline:
                raise RuntimeError('Heating timed out.')
            self.reactor.pause(self.reactor.monotonic() + 0.250)

    def manual_filament(self, gcmd):
        action = gcmd.get('ACTION').upper()
        if action not in ('LOAD', 'UNLOAD'):
            raise gcmd.error('Invalid filament action.')
        channel = (
            gcmd.get_int('CHANNEL', minval=1, maxval=4) - 1
            if action == 'LOAD'
            else None
        )
        if self.manual_operation is not None:
            raise gcmd.error('Filament operation in progress.')
        perfil_cortador = self.cortador.preparar()
        device, destination, current = self.manual_context()
        if action == 'UNLOAD' and current is None:
            gcmd.respond_info('No filament is loaded; nothing to unload.')
            return
        if action == 'LOAD':
            if (
                self.bmcu._endpoint_for_channel(device, channel)
                is not destination
            ):
                raise gcmd.error('Channel assigned to another extruder.')
            if not device.status['present'][channel]:
                raise gcmd.error(
                    'Insert filament into channel %d first.' % (channel + 1)
                )
            if current == channel:
                gcmd.respond_info(
                    'Channel %d is already loaded.' % (channel + 1)
                )
                return
        head = self.printer.lookup_object('toolhead').get_status(
            self.reactor.monotonic()
        )
        if not all(axis in head['homed_axes'] for axis in 'xyz') or (
            perfil_cortador and not self.cortador.recolhido
        ):
            # Home before heating or claiming the filament operation, which
            # intentionally blocks external G28. Keep the same X-first guard.
            self.execute('G28')
        self.manual_operation = action
        saved = False
        try:
            self.heat_manual_filament()
            self.manual_context()
            self.execute('SAVE_GCODE_STATE NAME=BMCU_ENDER_MANUAL')
            saved = True
            head = self.printer.lookup_object('toolhead').get_status(
                self.reactor.monotonic()
            )
            if not all(axis in head['homed_axes'] for axis in 'xyz'):
                self.execute('G28')
            self.check_manual_operation()
            if perfil_cortador is None:
                self.position_head()
            self.gcode.run_script_from_command(
                'SET_FILAMENT_SENSOR SENSOR=bmcu_entry ENABLE=1'
            )
            if current is not None:
                self.gcode.run_script_from_command(
                    'BMCU_UNLOAD DEVICE=%s CHANNEL=%d' % (device.name, current)
                )
                self.check_manual_operation()
                _, _, remaining = self.manual_context()
                if remaining is not None:
                    raise RuntimeError('The previous filament did not unload.')
            if action == 'LOAD':
                self.gcode.run_script_from_command(
                    'BMCU_LOAD DEVICE=%s CHANNEL=%d' % (device.name, channel)
                )
                self.check_manual_operation()
                _, _, loaded = self.manual_context()
                if loaded != channel:
                    raise RuntimeError('The requested channel was not loaded.')
        except Exception:
            device.stop_all()
            raise
        finally:
            try:
                self.restore_pullbacks()
            finally:
                try:
                    if saved:
                        self.execute(
                            'RESTORE_GCODE_STATE NAME=BMCU_ENDER_MANUAL'
                        )
                finally:
                    self.manual_operation = None

    def context(self, gcmd, load):
        device = self.bmcu.devices_by_name.get(self.device)
        if device is None or not device.ready:
            raise RuntimeError('The BMCU is not ready.')
        if getattr(device, 'firmware_compatible', False) is not True:
            raise RuntimeError('This integration requires BMCU firmware 1.0.4.')
        operation = self.bmcu.active_operations.get(device.name)
        if not operation:
            raise RuntimeError(
                ('This command must be called by a native BMCU operation.')
            )
        destination = self.bmcu.endpoints.get(self.destination)
        if destination is None or destination.name != self.destination:
            raise RuntimeError('Incompatible extruder endpoint.')
        if gcmd.get('ENDPOINT', self.destination) != self.destination:
            raise RuntimeError('This operation belongs to another extruder.')
        device.refresh()
        if load:
            context = getattr(destination, '_ender_load_context', None)
            if (
                not context
                or context[0] != device.name
                or operation.get('phase') != 'TOOLHEAD_PREPARATION'
            ):
                raise RuntimeError(
                    ('Extruder-first capture is not installed.')
                )
            channel = int(context[1])
        else:
            context = getattr(destination, '_ender_unload_context', None)
            if context is None:
                channel = int(device.status.get('now_channel', -1))
            else:
                destination._ender_unload_context = None
                if (
                    context[0] != device.name
                    or context[2] != device.status['session_id']
                ):
                    raise RuntimeError('The native unload context changed.')
                channel = int(context[1])
        if not 0 <= channel < 4:
            raise RuntimeError('The BMCU did not identify the active channel.')
        expected = (
            protocol.MOTION_IDLE if load or context is not None
            else protocol.MOTION_BEFORE_PULL_BACK
        )
        if not load and operation.get('phase') != 'CUT_OR_FORM':
            raise RuntimeError('The native unloading phase does not match.')
        if int(device.status['motion'][channel]) != expected:
            raise RuntimeError(
                ('The native feeding phase does not match this command.')
            )
        if (
            self.bmcu._endpoint_for_channel(device, channel).name
            != destination.name
        ):
            raise RuntimeError('Channel assigned to another extruder.')
        return device, destination, channel

    def verify_session(self, device, session, channel, refresh=True):
        if refresh:
            device.refresh()
        if device.status['session_id'] != session:
            raise RuntimeError('The BMCU restarted during the tool change.')
        if not device.status['present'][channel]:
            raise RuntimeError('The channel lost filament detection.')

    def wait_unload_buffer(
        self, device, destination, channel, session, refresh=True
    ):
        self.verify_session(device, session, channel, refresh=refresh)
        if device.status['buffer_pct'][channel] <= self.unload_buffer_limit:
            return
        self.gcode.respond_info(
            'Channel %d: pausing extruder retraction while the feeder '
            'relieves buffer compression.' % (channel + 1)
        )
        deadline = self.reactor.monotonic() + self.unload_buffer_timeout
        while device.status['buffer_pct'][channel] > self.unload_buffer_resume:
            if not self.sensor(destination):
                return
            if self.reactor.monotonic() >= deadline:
                raise RuntimeError('The unload buffer did not recover.')
            self.reactor.pause(self.reactor.monotonic() + 0.100)
            self.verify_session(device, session, channel)

    def unload_encoder_gap(self, device, channel, tracking):
        measured = (
            float(device.status['meters'][channel]) - tracking['start']
        ) * 1000.0
        gap = measured - tracking['commanded']
        if not math.isfinite(gap):
            raise RuntimeError('Invalid withdrawal encoder position.')
        if gap > self.unload_slack + 0.05:
            raise RuntimeError(
                'Feeder/extruder mismatch exceeded the withdrawal allowance; '
                'encoder lag %.2f mm.' % gap
            )
        return gap, measured

    def wait_unload_encoder(
        self,
        device,
        destination,
        channel,
        session,
        tracking,
        step,
        refresh=True,
    ):
        deadline = self.reactor.monotonic() + self.unload_progress_timeout
        announced = False
        while self.sensor(destination):
            self.verify_session(device, session, channel, refresh=refresh)
            if device.status['motion'][channel] != (
                protocol.MOTION_BEFORE_PULL_BACK
            ):
                raise RuntimeError('Feeder withdrawal assistance stopped.')
            gap, measured = self.unload_encoder_gap(device, channel, tracking)
            if gap + step <= self.unload_slack + 0.000001:
                return
            if not announced:
                self.gcode.respond_info(
                    'Channel %d: extruder waiting for feeder pullback; '
                    'encoder lag %.2f mm.' % (channel + 1, gap)
                )
                announced = True
            if self.reactor.monotonic() >= deadline:
                raise RuntimeError(
                    'Feeder did not follow extruder withdrawal; '
                    'encoder lag %.2f mm, commanded net %.2f mm, '
                    'measured net %.2f mm.'
                    % (gap, tracking['commanded'], measured)
                )
            self.reactor.pause(self.reactor.monotonic() + 0.100)
            refresh = True

    def registrar_sincronismo(self, device, channel, tracking, etapa):
        diagnostico = tracking.get('diagnostico')
        if diagnostico is None:
            return
        status = device.status
        recuo_encoder = (
            tracking['start'] - float(status['meters'][channel])
        ) * 1000.0
        diagnostico[etapa] = {
            'tempo_s': round(
                self.reactor.monotonic() - diagnostico['inicio_s'], 3
            ),
            'recuo_extrusora_comandado_mm': round(-tracking['commanded'], 3),
            'recuo_encoder_mm': round(recuo_encoder, 3),
            'buffer_pct': status['buffer_pct'][channel],
            'pwm': status['motor_pwm'][channel],
            'movimento': status['motion'][channel],
        }

    def verificar_alimentador_parado(
        self, device, destination, channel, session
    ):
        self.verify_session(device, session, channel)
        if any(device.status['motion']) or any(device.status['motor_pwm']):
            raise RuntimeError('The BMCU is not stopped for cutting.')
        if not self.sensor(destination):
            raise RuntimeError('The entry sensor lost filament before cutting.')

    def recuar_com_alimentador_parado(
        self, device, destination, channel, session, tracking, distancia
    ):
        if not 0 <= distancia <= self.unload_slack:
            raise RuntimeError(
                'Pre-cut retraction exceeds the configured buffer allowance.'
            )
        extruder = self.printer.lookup_object('extruder')
        limite = min(
            self.unload_step, self.unload_slack / 2, extruder.max_e_dist
        )
        if limite <= 0:
            raise RuntimeError('The extrusion limit is set to zero.')

        def verificar():
            self.verificar_alimentador_parado(
                device, destination, channel, session
            )
            if device.status['buffer_pct'][channel] >= self.unload_buffer_limit:
                raise RuntimeError(
                    'The buffer compressed during pre-cut retract.')

        restante = distancia
        verificar()
        while restante > 0.000001:
            passo = min(restante, limite)
            self.registrar_sincronismo(device, channel, tracking, 'antes_passo')
            diagnostico = tracking['diagnostico']
            diagnostico.setdefault(
                'antes_primeiro_passo', diagnostico['antes_passo']
            )
            inicio = self.reactor.monotonic()
            self.move_filament(-passo, self.pullback_speed, chunk=passo)
            diagnostico['tempo_movimentos_s'] += (
                self.reactor.monotonic() - inicio
            )
            tracking['commanded'] -= passo
            restante -= passo
            try:
                verificar()
            finally:
                self.registrar_sincronismo(
                    device, channel, tracking, 'depois_passo'
                )

    def move_unload_filament(
        self,
        device,
        destination,
        channel,
        session,
        tracking,
        mm,
        speed,
        allow_clear=False,
    ):
        mode = protocol.MOTION_BEFORE_PULL_BACK
        extruder = self.printer.lookup_object('extruder')
        limit = min(
            self.unload_step, self.unload_slack / 2, extruder.max_e_dist
        )
        remaining = abs(mm)
        completed = 0.0
        while remaining > 0.000001:
            self.wait_unload_buffer(
                device,
                destination,
                channel,
                session,
                refresh=(completed == 0.0),
            )
            step = min(limit, remaining)
            if mm < 0:
                self.wait_unload_encoder(
                    device,
                    destination,
                    channel,
                    session,
                    tracking,
                    step,
                    refresh=False,
                )
            if not self.sensor(destination):
                if allow_clear:
                    return completed
                raise RuntimeError(
                    'Filament left the entry sensor during tip forming.'
                )
            self.verify_session(device, session, channel, refresh=False)
            if device.status['motion'][channel] != mode:
                raise RuntimeError(
                    'Feeder assistance changed during withdrawal.'
                )
            signed = -step if mm < 0 else step
            diagnostico = tracking.get('diagnostico')
            if diagnostico is not None:
                self.registrar_sincronismo(
                    device, channel, tracking, 'antes_passo'
                )
                diagnostico.setdefault(
                    'antes_primeiro_passo', diagnostico['antes_passo']
                )
                inicio_movimento = self.reactor.monotonic()
            self.move_filament(signed, speed, chunk=limit)
            tracking['commanded'] += signed
            if diagnostico is not None:
                diagnostico['tempo_movimentos_s'] += (
                    self.reactor.monotonic() - inicio_movimento
                )
            completed += signed
            remaining -= step
            try:
                self.wait_unload_buffer(device, destination, channel, session)
            finally:
                self.registrar_sincronismo(
                    device, channel, tracking, 'depois_passo'
                )
            if not self.sensor(destination):
                if allow_clear:
                    return completed
                raise RuntimeError(
                    'Filament left the entry sensor during tip forming.'
                )
            if device.status['motion'][channel] != mode:
                raise RuntimeError('Feeder withdrawal assistance stopped.')
            self.unload_encoder_gap(device, channel, tracking)
        return completed

    def guard_hotend_temperature(self, name, previous):
        def execute(gcmd):
            if name == 'SET_HEATER_TEMPERATURE':
                if gcmd.get('HEATER', None) != 'extruder':
                    return previous(gcmd)
                target = gcmd.get_float('TARGET', 0.0, minval=0.0)
            else:
                if 'T' in gcmd.get_command_parameters():
                    if gcmd.get_int('T', minval=0) != 0:
                        return previous(gcmd)
                elif (
                    self.printer.lookup_object('toolhead').get_extruder() is not
                    self.printer.lookup_object('extruder')
                ):
                    return previous(gcmd)
                target = gcmd.get_float('S', 0.0, minval=0.0)
            try:
                self.prepare_preheat(target)
            except RuntimeError as error:
                raise gcmd.error(str(error)) from error
            return previous(gcmd)
        return execute

    def prepare_preheat(self, target, allow_paused=False, check=None):
        if not math.isfinite(target):
            raise RuntimeError('Nozzle temperature must be a finite number.')
        if target <= self.preheat_park_threshold:
            return
        stats = self.printer.lookup_object('print_stats')
        state = stats.get_status(self.reactor.monotonic()).get('state')
        if state == 'printing' or (
            state in ('paused', 'pause') and not allow_paused
        ):
            # Slicer/tuning temperature changes must never move a live print.
            return
        if self.printer.is_shutdown():
            raise RuntimeError('The printer is shut down.')
        extruder = self.printer.lookup_object('extruder')
        heater = extruder.get_heater()
        if not heater.min_temp <= target <= heater.max_temp:
            raise RuntimeError('Nozzle temperature is outside the heater limits.')
        profile = self.cortador.preparar()
        if profile and not self.cortador.recolhido:
            raise RuntimeError('Retract and check the stop before positioning.')
        previous_target = extruder.get_status(
            self.reactor.monotonic()
        )['target']

        def verify():
            if check is not None:
                check()
            if stats.get_status(self.reactor.monotonic()).get('state') != state:
                raise RuntimeError('Print state changed during preheat positioning.')
            if extruder.get_status(self.reactor.monotonic())['target'] != previous_target:
                raise RuntimeError('Nozzle target changed; operation aborted')
            if self.printer.is_shutdown():
                raise RuntimeError('The printer is shut down.')

        toolhead = self.printer.lookup_object('toolhead')
        toolhead.wait_moves()
        verify()
        status = toolhead.get_status(self.reactor.monotonic())
        if not all(axis in status['homed_axes'] for axis in 'xyz'):
            raise RuntimeError('Run G28 before preheating above the parking threshold.')
        position = toolhead.get_position()
        if all(math.isclose(position[i], value, abs_tol=0.01)
               for i, value in enumerate((self.purge_x, self.purge_y))):
            return
        expected = self.position_head(exigir_aquecimento=False, check=verify)
        verify()
        if not all(math.isclose(toolhead.get_position()[i], expected[i],
                                abs_tol=0.01) for i in range(3)):
            raise RuntimeError('Preheat positioning did not reach the purge position.')

    def posicionar_para_aquecimento(self, gcmd):
        if self.bmcu.active_operations or self.manual_operation is not None:
            raise gcmd.error('Wait for the BMCU operation to finish.')
        perfil = self.cortador.preparar()
        if perfil and not self.cortador.recolhido:
            raise gcmd.error('Retract and check the stop before positioning.')
        self.position_head(exigir_aquecimento=False)

    def posicao_purga(self, cabecote, estado):
        if not all(axis in estado['homed_axes'] for axis in 'xyz'):
            raise RuntimeError('Run G28 before loading or unloading filament.')
        minimo, maximo = estado['axis_minimum'], estado['axis_maximum']
        z = cabecote.get_position()[2] + self.z_lift
        alvo = [self.purge_x, self.purge_y, z]
        if not all(minimo[i] <= alvo[i] <= maximo[i] for i in range(3)) or not (
            minimo[0] <= self.wipe_x <= maximo[0]
        ):
            raise RuntimeError(
                'Purge/wipe position is outside the travel limits.')
        return alvo

    def position_head(self, exigir_aquecimento=True, check=None):
        if check is not None:
            check()
        toolhead = self.printer.lookup_object('toolhead')
        pendente = getattr(self, 'posicionamento_pendente', None)
        if pendente is not None:
            try:
                toolhead.wait_moves()
                if check is not None:
                    check()
                if not all(
                    math.isclose(
                        toolhead.get_position()[i], pendente[i], abs_tol=0.01
                    )
                    for i in range(3)
                ):
                    raise RuntimeError(
                        'Parallel positioning was interrupted.'
                    )
                status = toolhead.get_status(self.reactor.monotonic())
                if not all(eixo in status['homed_axes'] for eixo in 'xyz'):
                    raise RuntimeError('Axis homing reference was lost.')
                if (
                    not self.printer.lookup_object('extruder')
                    .get_status(self.reactor.monotonic())
                    .get('can_extrude')
                ):
                    raise RuntimeError(
                        'The extruder cooled during arrival.'
                    )
                return list(pendente)
            finally:
                self.posicionamento_pendente = None
        extruder = self.printer.lookup_object('extruder')
        status = extruder.get_status(self.reactor.monotonic())
        if exigir_aquecimento and not status.get('can_extrude'):
            raise RuntimeError('Heat the nozzle before changing filament.')
        toolhead.wait_moves()
        if check is not None:
            check()
        estado = toolhead.get_status(self.reactor.monotonic())
        alvo = self.posicao_purga(toolhead, estado)
        self.cortador.mover(
            toolhead, [None, None, alvo[2]], 300, verificar=check
        )
        self.cortador.mover(
            toolhead, [self.purge_x, None, None], self.purge_travel_speed,
            verificar=check,
        )
        self.cortador.mover(
            toolhead, [None, self.purge_y, None], self.purge_travel_speed,
            verificar=check,
        )
        return alvo

    def move_filament(self, mm, speed, chunk=10.0):
        extruder = self.printer.lookup_object('extruder')
        limit = min(chunk, extruder.max_e_dist)
        if limit <= 0:
            raise RuntimeError('The extrusion limit is set to zero.')
        count = int(math.ceil(abs(mm) / limit))
        if count:
            for _ in range(count):
                self.execute('G1 E%.9f F%.9f\nM400' % (mm / count, speed))

    def tip_settings(self):
        macro = self.printer.lookup_object(
            'gcode_macro _BMCU_ENDER_TIP_SETTINGS', None
        )
        if macro is None:
            return None
        settings = dict(macro.get_status(self.reactor.monotonic()))
        if not settings.get('enabled'):
            return None
        names = (
            'temperature',
            'separation_mm',
            'separation_f',
            'cooling_position_mm',
            'cooling_length_mm',
            'cooling_f',
            'cooling_final_f',
            'cooling_moves',
            'pause_ms',
        )
        for name in names:
            value = float(settings[name])
            if not math.isfinite(value) or value < 0:
                raise RuntimeError('Invalid tip-forming setting: %s.' % name)
            settings[name] = value
        if (
            settings['separation_mm'] <= 0
            or settings['separation_f'] <= 0
            or settings['cooling_f'] <= 0
            or settings['cooling_final_f'] <= 0
            or settings['cooling_length_mm'] <= 0
            or settings['cooling_position_mm'] < settings['separation_mm']
            or settings['cooling_moves'] != int(settings['cooling_moves'])
            or settings['cooling_moves'] < 1
        ):
            raise RuntimeError('Invalid tip-forming distances or speeds.')
        extruder = self.printer.lookup_object('extruder')
        if (
            max(
                settings['separation_f'],
                settings['cooling_f'],
                settings['cooling_final_f'],
            )
            > extruder.max_e_velocity * 60.0
        ):
            raise RuntimeError('Tip-forming speed exceeds the extruder limit.')
        heater = extruder.get_heater()
        if settings['temperature'] and not (
            heater.min_extrude_temp
            < settings['temperature']
            < heater.get_temp_range()[1]
        ):
            raise RuntimeError('Tip-forming temperature is outside limits.')
        if max(settings['separation_mm'], settings['cooling_length_mm']) > (
            extruder.max_e_dist
        ):
            raise RuntimeError(
                'Tip-forming moves exceed max_extrude_only_distance.'
            )
        return settings

    def form_tip(self, device, destination, channel, session, tracking):
        settings = self.tip_settings()
        if settings is None:
            return 0.0
        extruder = self.printer.lookup_object('extruder')
        previous = extruder.get_status(self.reactor.monotonic())
        temperature = settings['temperature']
        changed_temperature = False
        changed_pressure = False
        try:
            if temperature:
                self.execute(
                    'SET_HEATER_TEMPERATURE HEATER=extruder TARGET=%.3f'
                    % temperature
                )
                changed_temperature = True
                deadline = self.reactor.monotonic() + 180.0
                while True:
                    if not self.sensor(destination):
                        raise RuntimeError('Filament left before tip forming.')
                    self.verify_session(device, session, channel)
                    status = extruder.get_status(self.reactor.monotonic())
                    if status['target'] != temperature:
                        raise RuntimeError('Tip-forming target changed.')
                    if (
                        abs(status['temperature'] - temperature) <= 1.0
                        and status['can_extrude']
                    ):
                        break
                    if self.reactor.monotonic() >= deadline:
                        raise RuntimeError(
                            'Tip-forming temperature timed out.'
                        )
                    self.reactor.pause(self.reactor.monotonic() + 0.250)
            self.execute('SET_PRESSURE_ADVANCE EXTRUDER=extruder ADVANCE=0')
            changed_pressure = True
            if not self.sensor(destination):
                raise RuntimeError('Filament left before tip forming.')
            device.set_motion(channel, protocol.MOTION_BEFORE_PULL_BACK)
            self.move_unload_filament(
                device,
                destination,
                channel,
                session,
                tracking,
                -settings['separation_mm'],
                settings['separation_f'],
            )
            remaining = (
                settings['cooling_position_mm']
                + settings['cooling_length_mm']
                - settings['separation_mm']
            )
            for fraction, factor in ((0.7, 1.0), (0.2, 0.5), (0.1, 0.3)):
                self.verify_session(device, session, channel)
                if not self.sensor(destination):
                    raise RuntimeError(
                        'Cooling position passed the entry sensor; '
                        'reduce the tip-forming distances.'
                    )
                self.move_unload_filament(
                    device,
                    destination,
                    channel,
                    session,
                    tracking,
                    -remaining * fraction,
                    settings['cooling_f'] * factor,
                )
            count = int(settings['cooling_moves'])
            for index in range(count):
                self.verify_session(device, session, channel)
                if not self.sensor(destination):
                    raise RuntimeError('Filament left during tip cooling.')
                increment = (
                    settings['cooling_final_f'] - settings['cooling_f']
                ) / (2 * count - 1)
                forward = settings['cooling_f'] + increment * 2 * index
                self.move_unload_filament(
                    device,
                    destination,
                    channel,
                    session,
                    tracking,
                    settings['cooling_length_mm'],
                    forward,
                )
                self.move_unload_filament(
                    device,
                    destination,
                    channel,
                    session,
                    tracking,
                    -settings['cooling_length_mm'],
                    forward + increment,
                )
            deadline = self.reactor.monotonic() + settings['pause_ms'] / 1000
            while self.reactor.monotonic() < deadline:
                if not self.sensor(destination):
                    raise RuntimeError('Filament left during tip cooling.')
                self.reactor.pause(
                    min(deadline, self.reactor.monotonic() + 0.100)
                )
            self.verify_session(device, session, channel)
            if not self.sensor(destination):
                raise RuntimeError('Filament left during tip cooling.')
            self.gcode.respond_info(
                'Channel %d: tip-forming moves completed; '
                'inspect the tip after unloading.' % (channel + 1)
            )
            return (
                settings['cooling_position_mm'] + settings['cooling_length_mm']
            )
        finally:
            if changed_pressure:
                self.execute(
                    'SET_PRESSURE_ADVANCE EXTRUDER=extruder ADVANCE=%.9f '
                    'SMOOTH_TIME=%.9f'
                    % (
                        previous.get('pressure_advance', 0.0),
                        previous.get('smooth_time', 0.040),
                    )
                )
            if changed_temperature:
                current = extruder.get_status(self.reactor.monotonic())
                if current['target'] == temperature:
                    self.execute(
                        'SET_HEATER_TEMPERATURE HEATER=extruder TARGET=%.3f'
                        % previous['target']
                    )

    def wipe(self, device, destination, channel, session):
        perfil = self.cortador.preparar()
        velocidade = (
            perfil['velocidade_purga_f'] if perfil else self.advance_speed
        )
        extruder = self.printer.lookup_object('extruder')

        def verify():
            self.verify_session(device, session, channel)
            if not self.sensor(destination):
                raise RuntimeError('The entry sensor cleared during purging.')
            if not extruder.get_status(self.reactor.monotonic()).get(
                'can_extrude'
            ):
                raise RuntimeError('The extruder cooled during purge/wiping.')
            if device.status['motion'][channel] != protocol.MOTION_ON_USE:
                raise RuntimeError(
                    'Feeder assistance stopped during purge/wiping.'
                )

        verify()
        device.set_motion(channel, protocol.MOTION_ON_USE)
        remaining = self.purge_distance
        while remaining > 0.000001:
            verify()
            step = min(10.0, remaining)
            self.move_filament(step, velocidade)
            remaining -= step
        verify()
        self.execute('G4 P1000')
        verify()
        cabecote = self.printer.lookup_object('toolhead')
        for _ in range(self.wipe_passes):
            self.cortador.mover(
                cabecote, [self.wipe_x, None, None], self.wipe_speed
            )
            verify()
            self.cortador.mover(
                cabecote, [self.purge_x, None, None], self.wipe_speed
            )
            verify()
        self.cortador.mover(
            cabecote, [self.wipe_x, None, None], self.wipe_speed
        )
        verify()

    def load(self, gcmd):
        device = None
        saved = False
        completed = False
        try:
            device, destination, channel = self.context(gcmd, True)
            session = device.status['session_id']
            device.stop_all()
            self.restore_pullbacks()
            perfil = self.cortador.preparar()
            if perfil:
                self.cortador.recolher(perfil)
            self.execute('SAVE_GCODE_STATE NAME=BMCU_ENDER_CARGA')
            saved = True
            self.position_head()
            self.execute('M83\nM220 S100\nM221 S100')
            if not self.sensor(destination):
                raise RuntimeError(
                    'The filament did not trigger the entry sensor.'
                )
            self.measurement._capturar_extrusora_primeiro(
                device,
                destination,
                channel,
                self.capture,
                self.capture_speed,
                sensor_gerenciado=True,
            )
            remaining = self.advance
            self.verify_session(device, session, channel)
            if not self.sensor(destination):
                raise RuntimeError(
                    'The entry sensor cleared during loading.'
                )
            device.set_motion(channel, protocol.MOTION_ON_USE)
            while True:
                self.verify_session(device, session, channel)
                if not self.sensor(destination):
                    raise RuntimeError(
                        'The entry sensor cleared during loading.'
                    )
                if device.status['motion'][channel] != protocol.MOTION_ON_USE:
                    raise RuntimeError(
                        'Feeder assistance stopped during loading.'
                    )
                if remaining <= 0.000001:
                    break
                self.move_filament(
                    min(10.0, remaining),
                    (
                        perfil['velocidade_avanco_f']
                        if perfil
                        else self.advance_speed
                    ),
                )
                remaining -= min(10.0, remaining)
            self.wipe(device, destination, channel, session)
            completed = True
            gcmd.respond_info(
                (
                    (
                        'Channel %d: capture %.1f mm, nozzle feed %.1f mm and '
                        'purge %.1f mm '
                        'completed.'
                    )
                )
                % (
                    channel + 1,
                    self.capture,
                    self.advance,
                    self.purge_distance,
                )
            )
        except Exception as error:
            raise gcmd.error('Load interrupted: %s' % error) from error
        finally:
            try:
                if device is not None and not completed:
                    device.stop_all()
            finally:
                if saved:
                    self.execute('RESTORE_GCODE_STATE NAME=BMCU_ENDER_CARGA')

    def release(self, gcmd):
        device = None
        channel = None
        search_start = None
        total = 0.0
        tracking = None
        saved = False
        started = self.reactor.monotonic()
        timing = {'result': 'in_progress'}
        try:
            device, destination, channel = self.context(gcmd, False)
            session = device.status['session_id']
            device.stop_all()
            perfil = self.cortador.preparar()
            if perfil and not self.printer.lookup_object(
                'extruder'
            ).get_status(self.reactor.monotonic()).get('can_extrude'):
                raise RuntimeError(
                    'Heat the nozzle before cutting and unloading filament.'
                )
            self.execute('SAVE_GCODE_STATE NAME=BMCU_ENDER_RETIRADA')
            saved = True
            if perfil is None:
                self.position_head()
            self.execute('M83\nM220 S100\nM221 S100')
            if not self.sensor(destination):
                raise RuntimeError(
                    'The entry sensor was already clear before unloading.'
                )
            self.verify_session(device, session, channel)
            tracking = {
                'start': float(device.status['meters'][channel]),
                'commanded': 0.0,
            }
            timing['channel'] = channel + 1
            timing['position_seconds'] = self.reactor.monotonic() - started
            tip_started = self.reactor.monotonic()
            if perfil:

                def verificar_corte():
                    self.verificar_alimentador_parado(
                        device, destination, channel, session
                    )

                recuo_antes = perfil['recuo_antes_corte_mm']

                def recuar_antes_do_corte():
                    diagnostico = {
                        'inicio_s': self.reactor.monotonic(),
                        'tempo_movimentos_s': 0.0,
                    }
                    tracking['diagnostico'] = diagnostico
                    timing['sincronismo_pre_corte'] = diagnostico
                    self.registrar_sincronismo(
                        device, channel, tracking, 'antes_assistencia'
                    )
                    self.recuar_com_alimentador_parado(
                        device, destination, channel, session, tracking,
                        recuo_antes,
                    )
                    timing['pre_cut_retract_mm'] = recuo_antes

                self.cortador.cortar(
                    perfil, verificar_corte, retornar=False,
                    antes_do_corte=(
                        recuar_antes_do_corte if recuo_antes else None
                    ),
                )
                timing['cut_seconds'] = self.reactor.monotonic() - tip_started
                self.position_head()
            else:
                total = self.form_tip(
                    device, destination, channel, session, tracking
                )
            timing['tip_seconds'] = self.reactor.monotonic() - tip_started
            release_started = self.reactor.monotonic()
            device.refresh()
            marker = float(device.status['meters'][channel])
            if perfil:
                tracking = {'start': marker, 'commanded': 0.0}
                tracking['diagnostico'] = timing['sincronismo_pos_corte'] = {
                    'inicio_s': release_started,
                    'tempo_movimentos_s': 0.0,
                }
                self.registrar_sincronismo(
                    device, channel, tracking, 'antes_assistencia'
                )
            search_start = progress_marker = marker
            progress_at = self.reactor.monotonic()
            device.set_motion(channel, protocol.MOTION_BEFORE_PULL_BACK)
            deadline = self.reactor.monotonic() + self.timeout
            while self.sensor(destination):
                if self.reactor.monotonic() >= deadline:
                    raise RuntimeError(
                        (
                            'The entry sensor did not clear within the '
                            'configured '
                            'timeout.'
                        )
                    )
                moved = self.move_unload_filament(
                    device,
                    destination,
                    channel,
                    session,
                    tracking,
                    -2.0,
                    (
                        perfil['velocidade_recuo_f']
                        if perfil
                        else self.pullback_speed
                    ),
                    allow_clear=True,
                )
                total += abs(moved)
                self.verify_session(device, session, channel, refresh=False)
                current = float(device.status['meters'][channel])
                if (progress_marker - current) * 1000.0 >= 1.0:
                    progress_marker = current
                    progress_at = self.reactor.monotonic()
                elif (
                    self.sensor(destination)
                    and self.reactor.monotonic() - progress_at
                    >= self.unload_progress_timeout
                ):
                    raise RuntimeError(
                        'No encoder pullback progress while the entry '
                        'sensor remains triggered.'
                    )
                if (marker - current) * 1000.0 >= 25.0 and self.sensor(
                    destination
                ):
                    device.stop_all()
                    self.reactor.pause(self.reactor.monotonic() + 0.050)
                    device.set_motion(
                        channel, protocol.MOTION_BEFORE_PULL_BACK
                    )
                    marker = current
            device.stop_all()
            self.reactor.pause(self.reactor.monotonic() + 0.050)
            self.verify_session(device, session, channel)
            if self.sensor(destination):
                raise RuntimeError('The entry sensor detected filament again.')
            gcmd.respond_info(
                (
                    (
                        'Channel %d: entry sensor cleared after %.1f mm of '
                        'extrusion; BMCU will retract another %.1f '
                        'mm.'
                    )
                )
                % (channel + 1, total, self.pullback)
            )
            timing['release_seconds'] = (
                self.reactor.monotonic() - release_started
            )
            timing['result'] = 'awaiting_parking'
            self.last_unload_timing = dict(timing)
            self.temporary_pullbacks[device.name] = {
                'channel': channel,
                'start': float(device.status['meters'][channel]),
                'session': session,
                'timing': timing,
                'unload_started': started,
                'parking_started': self.reactor.monotonic(),
            }
            self.bmcu._send_device_runtime_config(
                device, channel_retract_override=(channel, self.pullback)
            )
        except Exception as error:
            timing['result'] = 'failed'
            timing['total_seconds'] = self.reactor.monotonic() - started
            self.last_unload_timing = dict(timing)
            evidence = ''
            if device is not None and channel is not None:
                status = device.status
                evidence = ' Channel %d; buffer %s%%; PWM %s; motion %s.' % (
                    channel + 1,
                    status['buffer_pct'][channel],
                    status['motor_pwm'][channel],
                    status['motion'][channel],
                )
                if search_start is not None:
                    evidence += (
                        ' Commanded net retract %.1f mm; encoder pullback '
                        'after forming %.2f mm.'
                        % (
                            total,
                            (search_start - float(status['meters'][channel]))
                            * 1000.0,
                        )
                    )
                if tracking is not None:
                    comandado = (
                        tracking['commanded']
                        - timing.get('pre_cut_retract_mm', 0.0)
                        if search_start is not None else tracking['commanded']
                    )
                    evidence += (
                        ' Total commanded net %.2f mm.' % comandado
                    )
            raise gcmd.error(
                'Unload interrupted: %s%s' % (error, evidence)
            ) from error
        finally:
            try:
                if device is not None:
                    device.stop_all()
                    self.reactor.pause(self.reactor.monotonic() + 0.050)
            finally:
                if saved:
                    self.execute(
                        'RESTORE_GCODE_STATE NAME=BMCU_ENDER_RETIRADA'
                    )

    def change_tool(self, gcmd):
        if getattr(
            self, 'pending_tool_change', None
        ) is not None and not getattr(self, 'recovering_tool_change', False):
            raise gcmd.error(
                'A tool change is pending; use RESUME to recover it.'
            )
        tool = gcmd.get_int('TOOL', minval=1, maxval=4)
        purge = gcmd.get_int('PURGE_IF_LOADED', 0, minval=0, maxval=1)
        head = self.printer.lookup_object('toolhead').get_status(
            self.reactor.monotonic()
        )
        if not all(axis in head['homed_axes'] for axis in 'xyz'):
            raise gcmd.error('Run G28 before changing filament.')
        extruder_status = self.printer.lookup_object('extruder').get_status(
            self.reactor.monotonic()
        )
        if not extruder_status.get('can_extrude'):
            raise gcmd.error('Heat the nozzle before changing filament.')
        status = self.printer.lookup_object('gcode_move').get_status(
            self.reactor.monotonic()
        )
        position = list(status['gcode_position'])
        current = self.bmcu.get_status(self.reactor.monotonic()).get(
            'loaded_tool'
        )
        self.execute('M400\nSAVE_GCODE_STATE NAME=BMCU_ENDER_TROCA')
        success = False
        try:
            self.gcode.run_script_from_command(
                'BMCU_TOOL_CHANGE TOOL=%d' % tool
            )
            if purge and current == tool:
                device, channel, destination = self.bmcu._resolve_tool(tool)
                device.refresh()
                self.position_head()
                self.execute('M83\nM220 S100\nM221 S100')
                self.wipe(
                    device,
                    destination,
                    channel,
                    device.status['session_id'],
                )
            self.execute(
                'G90\nG1 X%.9f Y%.9f F6000\nG1 Z%.9f F300\nM400'
                % tuple(position[:3])
            )
            success = True
        except Exception as error:
            if not self.hold_failed_tool_change(
                gcmd, tool, error, extruder_status.get('target', 230.0)
            ):
                raise
        finally:
            if not success:
                try:
                    self.bmcu.devices_by_name[self.device].stop_all()
                except Exception:
                    logging.exception(
                        'Could not confirm that the BMCU stopped.'
                    )
            self.execute('RESTORE_GCODE_STATE NAME=BMCU_ENDER_TROCA')


def load_config(config):
    return EnderIntegration(config)
