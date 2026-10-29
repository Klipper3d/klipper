import logging
import math

from .bmcu_core import protocol
from .bmcu_core.release import PACKAGE_VERSION


class ArrivalMeasurement:
    def __init__(self, config):
        if PACKAGE_VERSION != '1.0.6':
            raise config.error(
                'Arrival measurement requires BMCU-Klipper 1.0.6.'
            )
        self.manager = config.get_printer().load_object(config, 'bmcu')
        self.capture = None
        self.capture_failed = False
        self.manager.gcode.register_command(
            'BMCU_MEDIR_CHEGADA',
            self.measure,
            desc=(
                'Feed to the entry sensor and measure the remaining '
                'distance.'
            ),
        )
        self.manager.gcode.register_command(
            'BMCU_TESTAR_EXTRUSAO',
            self.test_extrusion,
            desc=('Test extrusion with assisted filament feeding.'),
        )
        self.manager.gcode.register_command(
            'BMCU_LIMITES_EXTRUSAO',
            self.extrusion_limits,
            desc='Show configured extruder limits.',
        )

    def measure(self, gcmd):
        try:
            return self._measure(gcmd)
        except (RuntimeError, OSError) as error:
            raise gcmd.error(
                'Could not measure arrival: %s' % error
            ) from error

    def _measure(self, gcmd):
        manager = self.manager
        manager._require_standalone_operation('BMCU_MEDIR_CHEGADA', gcmd)
        channel = gcmd.get_int('CANAL', minval=1, maxval=4) - 1
        limit = manager._finite_gcmd_float(
            gcmd, 'LIMITE', 1500.0, minval=10.0, maxval=1500.0
        )
        if not gcmd.get_int('CAMINHO_LIVRE', 0, minval=0, maxval=1):
            raise gcmd.error(
                (
                    (
                        'Remove the previous filament from the extruder and '
                        'confirm CAMINHO_LIVRE=1.'
                    )
                )
            )
        if (
            manager._print_state() in ('printing', 'paused', 'pause')
            or manager.print_plan_open
            or manager.print_map_active
            or manager.active_operations
            or manager.prestaged
        ):
            raise gcmd.error('The printer and BMCU must be idle.')
        device = manager._require_device(gcmd)
        destination = manager._endpoint_for_channel(device, channel)
        if (
            destination is None
            or destination.driver != 'generic_single_extruder'
        ):
            raise gcmd.error(
                ('The channel must be assigned to the generic extruder.')
            )
        manager._check_automatic_ready(device, channel)
        device.refresh()
        manager._check_automatic_ready(device, channel)
        manager._require_exclusive_endpoint_route(device, destination, channel)
        if manager._route_state(device, channel) != protocol.ROUTE_EMPTY:
            raise gcmd.error('The channel must be EMPTY before measurement.')
        if any(device.status.get('motion', [])):
            raise gcmd.error('The BMCU is moving.')
        sensor_status = destination.sensor_status('entry_sensor')
        if not sensor_status or not sensor_status.get('enabled', True):
            raise gcmd.error(
                ('The entry sensor must be available and enabled.')
            )
        if sensor_status.get('filament_detected') is not False:
            raise gcmd.error(
                ('The entry sensor must be clear before measurement.')
            )
        if device.status['buffer_pct'][channel] >= 70:
            raise gcmd.error('Release the channel buffer before measurement.')
        manager._lock(device, destination, 'MEDICAO_CHEGADA', channel=channel)
        previous_speed = None
        changed_configuration = False
        try:
            previous_speed = device.config_get(protocol.CONFIG_LOAD_SPEED_MMS)
            changed_configuration = True
            device.config_set(protocol.CONFIG_LOAD_SPEED_MMS, 10.0)
            deadline = limit / 10.0 + 10.0
            gcmd.respond_info(
                (
                    (
                        'Channel %d: feed at 10 mm/s to the entry sensor; '
                        'limit %.0f '
                        'mm.'
                    )
                )
                % (channel + 1, limit)
            )
            operation = device.start_feed_distance(
                channel, limit, 70, int(deadline * 1000)
            )
            result = manager._wait_feed_operation(
                device,
                operation,
                deadline + 2.0,
                destination,
                'entry_sensor',
                poll_interval=0.050,
            )
            measured = float(result.get('measured_mm', float('nan')))
            if (
                not result.get('ok')
                or not result.get('sensor_triggered')
                or not math.isfinite(measured)
                or measured < 0
            ):
                raise gcmd.error(
                    ('Arrival was not confirmed: %s; measured travel: %s mm.')
                    % (result.get('reason'), measured)
                )
        except Exception as error:
            raise gcmd.error('Measurement interrupted: %s' % error) from error
        finally:
            try:
                if changed_configuration:
                    try:
                        device.stop_all()
                    except Exception:
                        logging.exception(
                            'Could not confirm that the BMCU stopped.'
                        )
                        raise gcmd.error(
                            (
                                (
                                    'Could not confirm motor stop; disconnect '
                                    'BMCU power.'
                                )
                            )
                        )
                    device.config_set(
                        protocol.CONFIG_LOAD_SPEED_MMS, previous_speed
                    )
            finally:
                manager._unlock(device, destination)
        manager.last_diagnostic = {
            'operation': 'medicao_chegada',
            'device': device.name,
            'channel': channel,
            'endpoint': destination.name,
            'measured_mm': measured,
            'sensor_triggered': True,
            'origem': 'posicao_atual',
        }
        gcmd.respond_info(
            (
                (
                    'Entry sensor triggered. Channel %d measured remaining '
                    'travel: %.2f '
                    'mm.'
                )
            )
            % (channel + 1, measured)
        )
        self.capture = None
        self.capture_failed = False

    def test_extrusion(self, gcmd):
        try:
            return self._test_extrusion(gcmd)
        except (RuntimeError, OSError) as error:
            raise gcmd.error('Could not test extrusion: %s' % error) from error

    def _test_extrusion(self, gcmd):
        manager = self.manager
        manager._require_standalone_operation('BMCU_TESTAR_EXTRUSAO', gcmd)
        channel = gcmd.get_int('CANAL', minval=1, maxval=4) - 1
        advance = manager._finite_gcmd_float(gcmd, 'MM', 5.0, above=0.0)
        extruder_first = gcmd.get_int(
            'EXTRUSORA_PRIMEIRO', 0, minval=0, maxval=1
        )
        speed = manager._finite_gcmd_float(
            gcmd,
            'VELOCIDADE',
            120.0 if extruder_first else 180.0,
            above=0.0,
        )
        if (
            manager._print_state() in ('printing', 'paused', 'pause')
            or manager.print_plan_open
            or manager.print_map_active
            or manager.active_operations
            or manager.prestaged
        ):
            raise gcmd.error('The printer and BMCU must be idle.')
        device = manager._require_device(gcmd)
        destination = manager._endpoint_for_channel(device, channel)
        if (
            destination is None
            or destination.driver != 'generic_single_extruder'
        ):
            raise gcmd.error(
                ('The channel must be assigned to the generic extruder.')
            )
        device.refresh()
        manager._check_automatic_ready(device, channel, allow_uncertain=True)
        manager._require_exclusive_endpoint_route(device, destination, channel)
        if manager._route_state(device, channel) != protocol.ROUTE_UNCERTAIN:
            raise gcmd.error(
                (
                    (
                        'This test requires the UNCERTAIN route left by entry '
                        'sensor arrival.'
                    )
                )
            )
        if any(device.status.get('motion', [])):
            raise gcmd.error('The BMCU is moving.')
        sensor = destination.sensor_status('entry_sensor')
        if (
            not sensor
            or not sensor.get('enabled', True)
            or sensor.get('filament_detected') is not True
        ):
            raise gcmd.error(
                ('A ponta precisa estar acionando o switch ' 'de chegada.')
            )
        extruder = manager.printer.lookup_object(
            destination.get('extruder', 'extruder')
        )
        if not extruder.get_status(manager.reactor.monotonic()).get(
            'can_extrude'
        ):
            raise gcmd.error(
                ('Heat the nozzle to the material temperature before testing.')
            )
        identity = (
            device.name,
            device.status['session_id'],
            channel,
            destination.name,
        )
        self.capture_failed = False
        manager._lock(device, destination, 'TESTE_EXTRUSAO', channel=channel)
        previous_speed = None
        changed_configuration = False
        try:
            previous_speed = device.config_get(protocol.CONFIG_LOAD_SPEED_MMS)
            changed_configuration = True
            device.config_set(protocol.CONFIG_LOAD_SPEED_MMS, 10.0)
            if extruder_first:
                self._capturar_extrusora_primeiro(
                    device, destination, channel, advance, speed
                )
            else:
                device.set_motion(channel, protocol.MOTION_BEFORE_ON_USE)
                with manager.printer_critical_section('teste_extrusao'):
                    manager.gcode.run_script_from_command(
                        'SAVE_GCODE_STATE NAME=BMCU_MEDICAO_EXTRUSAO'
                    )
                    try:
                        manager.gcode.run_script_from_command(
                            'M83\nM220 S100\nM221 S100'
                        )
                        self._advance_extruder(advance, speed, extruder)
                        manager.gcode.run_script_from_command('M400')
                    finally:
                        manager.gcode.run_script_from_command(
                            'RESTORE_GCODE_STATE NAME=BMCU_MEDICAO_EXTRUSAO'
                        )
        except Exception as error:
            self.capture_failed = True
            raise gcmd.error(
                'Test interrupted; the accumulated distance is invalid: %s'
                % error
            ) from error
        finally:
            try:
                if changed_configuration:
                    try:
                        device.stop_all()
                        device.config_set(
                            protocol.CONFIG_LOAD_SPEED_MMS, previous_speed
                        )
                    except Exception as error:
                        self.capture_failed = True
                        raise gcmd.error(
                            'Failed to stop assisted feeding: %s' % error
                        ) from error
            finally:
                manager._unlock(device, destination)
        self.capture = {'identificacao': identity, 'total_mm': advance}
        manager.last_diagnostic = {
            'operation': 'teste_extrusao',
            'device': device.name,
            'channel': channel,
            'endpoint': destination.name,
            'commanded_mm': advance,
            'requested_feedrate': speed,
            'extruder_first': bool(extruder_first),
        }
        gcmd.respond_info(
            (
                (
                    'Channel %d: attempting %.3f mm at F%.3f; configured '
                    'extruder limit '
                    'F%.3f.'
                )
            )
            % (channel + 1, advance, speed, extruder.max_e_velocity * 60.0)
        )

    def extrusion_limits(self, gcmd):
        extruder = self.manager.printer.lookup_object('extruder')
        gcmd.respond_info(
            (
                (
                    'Configured limit: %.3f mm/s, VELOCIDADE=%.3f mm/min; '
                    'maximum internal move: %.3f '
                    'mm.'
                )
            )
            % (
                extruder.max_e_velocity,
                extruder.max_e_velocity * 60.0,
                extruder.max_e_dist,
            )
        )

    def _advance_extruder(self, advance, speed, extruder):
        if extruder.max_e_dist <= 0.0:
            raise RuntimeError(('max_extrude_only_distance is set to zero.'))
        count = math.ceil(advance / extruder.max_e_dist)
        for _ in range(count):
            self.manager.gcode.run_script_from_command(
                'G1 E%.9f F%.9f' % (advance / count, speed)
            )

    def _capturar_extrusora_primeiro(
        self,
        device,
        destination,
        channel,
        advance,
        speed,
        sensor_gerenciado=False,
    ):
        manager = self.manager
        execute = manager.gcode.run_script_from_command
        head = manager.printer.lookup_object('toolhead')
        extruder = manager.printer.lookup_object(
            destination.get('extruder', 'extruder')
        )
        if extruder.max_e_dist <= 0.0:
            raise RuntimeError(('max_extrude_only_distance is set to zero.'))
        actual_speed = min(speed / 60.0, extruder.max_e_velocity)
        initial = min(
            1.0, advance / 4.0, actual_speed * 0.5, extruder.max_e_dist
        )
        next_time = min(
            advance - initial, actual_speed * 1.5, extruder.max_e_dist
        )
        remaining = advance - initial - next_time
        execute(('M400\nSAVE_GCODE_STATE ' 'NAME=BMCU_MEDICAO_EXTRUSAO'))
        try:
            timing = []
            execute(
                'M83\nM220 S100\nM221 S100\nG1 E%.9f F%.9f' % (initial, speed)
            )
            head.register_lookahead_callback(timing.append)
            execute('G1 E%.9f F%.9f' % (next_time, speed))
            head.register_lookahead_callback(timing.append)
            head.flush_step_generation()
            if len(timing) != 2 or not all(math.isfinite(t) for t in timing):
                raise RuntimeError(('Could not obtain extruder move timing.'))
            deadline = (
                manager.reactor.monotonic() + initial / actual_speed + 5.0
            )
            while True:
                now = manager.reactor.monotonic()
                if now >= deadline:
                    raise RuntimeError(
                        'The extruder did not start within the expected time.'
                    )
                if head.mcu.estimated_print_time(now) >= timing[0]:
                    break
                manager.reactor.pause(now + 0.010)
            if head.mcu.estimated_print_time(now) >= timing[1]:
                raise RuntimeError(
                    (
                        (
                            'The extruder motion window ended before BMCU '
                            'assistance.'
                        )
                    )
                )
            sensor = destination.sensor_status('entry_sensor')
            if (
                not sensor
                or (not sensor_gerenciado and not sensor.get('enabled', True))
                or sensor.get('filament_detected') is not True
            ):
                raise RuntimeError(
                    (
                        (
                            'The entry sensor lost filament detection during '
                            'capture.'
                        )
                    )
                )
            device.set_motion(channel, protocol.MOTION_BEFORE_ON_USE)
            if (
                head.mcu.estimated_print_time(manager.reactor.monotonic())
                >= timing[1]
            ):
                raise RuntimeError(
                    (
                        (
                            'The BMCU responded after the initial extruder '
                            'motion window.'
                        )
                    )
                )
            if remaining > 0.0:
                self._advance_extruder(remaining, speed, extruder)
                head.register_lookahead_callback(timing.append)
                head.flush_step_generation()
            end = timing[-1]
            if not math.isfinite(end):
                raise RuntimeError(
                    ('Could not obtain the extruder move end time.')
                )
            stop_time = end - min(2.0, advance / 2.0) / actual_speed
            deadline = (
                manager.reactor.monotonic() + advance / actual_speed + 5.0
            )
            while True:
                now = manager.reactor.monotonic()
                if now >= deadline:
                    raise RuntimeError(
                        ('Could not track the move to stop the BMCU.')
                    )
                if head.mcu.estimated_print_time(now) >= stop_time:
                    break
                manager.reactor.pause(now + 0.010)
            device.stop_all()
            if (
                head.mcu.estimated_print_time(manager.reactor.monotonic())
                >= end
            ):
                raise RuntimeError(
                    (
                        (
                            'Could not confirm BMCU stop before the extruder '
                            'move ended.'
                        )
                    )
                )
            with manager.printer_critical_section('teste_captura'):
                execute('M400')
        finally:
            execute('RESTORE_GCODE_STATE NAME=BMCU_MEDICAO_EXTRUSAO')


class AssistedUnload:
    def __init__(self, measurement):
        self.manager = measurement.manager
        self.manager.gcode.register_command(
            'BMCU_RETIRAR_ATE_SWITCH',
            self.execute,
            desc=(
                (
                    'Retract with assistance until the entry sensor clears, '
                    'then park the '
                    'filament.'
                )
            ),
        )

    def sensor(self, destination):
        status = destination.sensor_status('entry_sensor')
        if not status or not status.get('enabled', True):
            raise RuntimeError(
                ('The entry sensor must be available and enabled.')
            )
        value = status.get('filament_detected')
        if value is not True and value is not False:
            raise RuntimeError('The sensor did not return a valid state.')
        return value

    def execute(self, gcmd):
        manager = self.manager
        manager._require_standalone_operation('BMCU_RETIRAR_ATE_SWITCH', gcmd)
        channel = gcmd.get_int('CANAL', 2, minval=1, maxval=4) - 1
        speed = manager._finite_gcmd_float(
            gcmd, 'VELOCIDADE', 600.0, above=0.0
        )
        after = manager._finite_gcmd_float(gcmd, 'APOS', 55.0, above=0.0)
        timeout = manager._finite_gcmd_float(gcmd, 'TEMPO', 60.0, above=0.0)
        if (
            manager._print_state() in ('printing', 'paused', 'pause')
            or manager.print_plan_open
            or manager.print_map_active
            or manager.active_operations
            or manager.prestaged
        ):
            raise gcmd.error('This test requires an idle printer and BMCU.')
        device = manager._require_device(gcmd)
        destination = manager._endpoint_for_channel(device, channel)
        if (
            destination is None
            or destination.driver != 'generic_single_extruder'
        ):
            raise gcmd.error(
                ('The channel must be assigned to the generic extruder.')
            )
        device.refresh()
        manager._check_automatic_ready(device, channel, allow_uncertain=True)
        manager._require_exclusive_endpoint_route(device, destination, channel)
        if manager._route_state(device, channel) not in (
            protocol.ROUTE_LOADED,
            protocol.ROUTE_UNCERTAIN,
        ):
            raise gcmd.error(
                ('The channel must be loaded or partially unloaded.')
            )
        extruder = manager.printer.lookup_object(
            destination.get('extruder', 'extruder')
        )
        if not extruder.get_status(manager.reactor.monotonic()).get(
            'can_extrude'
        ):
            raise gcmd.error(
                (
                    (
                        'Heat the nozzle to the material temperature before '
                        'unloading.'
                    )
                )
            )
        if extruder.max_e_dist <= 0:
            raise gcmd.error(('The extruder move limit is set to zero.'))
        try:
            if not self.sensor(destination):
                raise gcmd.error(
                    (
                        (
                            'The entry sensor is already clear; no new '
                            'transition can be '
                            'measured.'
                        )
                    )
                )
        except RuntimeError as error:
            raise gcmd.error(str(error)) from error

        execute = manager.gcode.run_script_from_command
        configuration = {}
        saved_state = False
        primary_error = None
        extruder_total = 0.0
        measured = 0.0
        session = device.status['session_id']
        manager._lock(
            device, destination, 'RETIRADA_ATE_SWITCH', channel=channel
        )
        try:
            execute('M400')
            execute('SAVE_GCODE_STATE NAME=BMCU_RETIRADA_ASSISTIDA')
            saved_state = True
            execute('M83\nM220 S100\nM221 S100')
            configuration[protocol.CONFIG_PULL_SPEED_MMS] = device.config_get(
                protocol.CONFIG_PULL_SPEED_MMS
            )
            configuration[protocol.CONFIG_PULL_SPEED_END_MMS] = (
                device.config_get(protocol.CONFIG_PULL_SPEED_END_MMS)
            )
            device.set_motion(channel, protocol.MOTION_BEFORE_PULL_BACK)
            deadline = manager.reactor.monotonic() + timeout
            step = min(2.0, extruder.max_e_dist)
            gcmd.respond_info(
                (
                    (
                        'Channel %d: retracting the extruder until the entry '
                        'sensor clears, with BMCU '
                        'assistance.'
                    )
                )
                % (channel + 1)
            )
            while self.sensor(destination):
                if manager.reactor.monotonic() >= deadline:
                    raise RuntimeError(
                        'The entry sensor did not clear within %.1f seconds.'
                        % timeout
                    )
                with manager.printer_critical_section('retirada_ate_switch'):
                    execute('G1 E-%.9f F%.9f\nM400' % (step, speed))
                extruder_total += step
                device.refresh()
                if device.status['session_id'] != session:
                    raise RuntimeError('The BMCU restarted during unloading.')

            device.stop_all()
            execute('M400')
            device.refresh()
            start = float(device.status['meters'][channel])
            gcmd.respond_info(
                (
                    (
                        'Entry sensor clear. Extruder stopped; retracting '
                        'another %.1f mm through '
                        'BMCU.'
                    )
                )
                % after
            )
            device.config_set(protocol.CONFIG_PULL_SPEED_END_MMS, 4.0)
            device.config_set(protocol.CONFIG_PULL_SPEED_MMS, 10.0)
            device.set_motion(channel, protocol.MOTION_PULL_BACK)
            deadline = manager.reactor.monotonic() + timeout
            while True:
                device.refresh()
                if device.status['session_id'] != session:
                    raise RuntimeError('The BMCU restarted during pullback.')
                measured = (
                    start - float(device.status['meters'][channel])
                ) * 1000.0
                if not math.isfinite(measured) or measured < -2.0:
                    raise RuntimeError(
                        'The encoder did not confirm the retraction direction.'
                    )
                if self.sensor(destination):
                    raise RuntimeError(
                        (
                            (
                                'The entry sensor detected filament again '
                                'during pullback.'
                            )
                        )
                    )
                if measured >= after:
                    break
                if manager.reactor.monotonic() >= deadline:
                    raise RuntimeError(
                        (
                            (
                                'The encoder measured only %.2f mm after the '
                                'entry sensor '
                                'cleared.'
                            )
                        )
                        % measured
                    )
                manager.reactor.pause(manager.reactor.monotonic() + 0.050)
        except Exception as error:
            primary_error = error
        finally:
            try:
                try:
                    device.stop_all()
                except Exception as error:
                    logging.exception(
                        'Could not confirm that the BMCU stopped.'
                    )
                    primary_error = RuntimeError(
                        'BMCU stop was not confirmed: %s' % error
                    )
                for key, value in configuration.items():
                    try:
                        device.config_set(key, value)
                    except Exception as error:
                        logging.exception('Could not restore the BMCU speed.')
                        if primary_error is None:
                            primary_error = error
                if saved_state:
                    try:
                        execute(
                            (
                                'M400\nRESTORE_GCODE_STATE '
                                'NAME=BMCU_RETIRADA_ASSISTIDA'
                            )
                        )
                    except Exception as error:
                        if primary_error is None:
                            primary_error = error
            finally:
                manager._unlock(device, destination)
        if primary_error is not None:
            raise gcmd.error(
                ('Unload interrupted; do not load another channel yet: %s')
                % primary_error
            ) from primary_error
        execute(
            ('BMCU_ROUTE_CONFIRM DEVICE=%s CHANNEL=%d ' 'STATE=PARKED')
            % (device.name, channel)
        )
        manager.last_diagnostic = {
            'operation': 'retirada_ate_switch',
            'device': device.name,
            'channel': channel,
            'commanded_extruder_mm': extruder_total,
            'retracted_after_switch_mm': measured,
            'sensor_triggered': False,
        }
        gcmd.respond_info(
            (
                (
                    'Channel %d parked. Extruder retraction commanded: %.2f '
                    'mm; pullback after the entry sensor: %.2f '
                    'mm.'
                )
            )
            % (channel + 1, extruder_total, measured)
        )


def load_config(config):
    measurement = ArrivalMeasurement(config)
    measurement.assisted_unload = AssistedUnload(measurement)
    return measurement
