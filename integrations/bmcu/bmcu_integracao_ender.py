import logging
import math

from .bmcu_core import protocol
from .bmcu_core.release import PACKAGE_VERSION


class EnderIntegration:
    def __init__(self, config):
        if PACKAGE_VERSION != '1.0.6':
            raise config.error('This integration requires BMCU-Klipper 1.0.6.')
        self.printer = config.get_printer()
        self.bmcu = self.printer.load_object(config, 'bmcu')
        self.measurement = self.printer.load_object(config, 'bmcu_medicao')
        self.gcode = self.printer.lookup_object('gcode')
        self.reactor = self.printer.get_reactor()
        self.device = config.get('dispositivo', 'bmcu0')
        self.destination = config.get('destino', 'extruder')
        self.capture = config.getfloat('captura_mm', 20.0, above=0.0)
        self.capture_speed = config.getfloat('captura_f', 180.0, above=0.0)
        self.advance = config.getfloat('avanco_bico_mm', 80.0, above=0.0)
        self.purge_distance = config.getfloat('purga_mm', 70.0, above=0.0)
        self.advance_speed = config.getfloat('avanco_f', 120.0, above=0.0)
        self.pullback_speed = config.getfloat('recuo_f', 600.0, above=0.0)
        self.pullback = config.getfloat(
            'recuo_apos_switch_mm', 55.0, above=0.0
        )
        self.timeout = config.getfloat('tempo_switch', 60.0, above=0.0)
        self.purge_x = config.getfloat('x_purga', -6.0)
        self.wipe_x = config.getfloat('x_limpeza', -4.0)
        self.purge_y = config.getfloat('y_purga', 123.0)
        self.z_lift = config.getfloat('alivio_z', 2.0, above=0.0)
        self.wipe_passes = config.getint('limpezas', 3, minval=1)
        self.temporary_pullbacks = {}
        self.original_wait_pullback = self.bmcu._wait_pullback_safe
        self.bmcu._wait_pullback_safe = self.wait_pullback
        self.gcode.register_command(
            'BMCU_ENDER_CARREGAR', self.protect(self.load)
        )
        self.gcode.register_command(
            'BMCU_ENDER_LIBERAR', self.protect(self.release)
        )
        self.gcode.register_command(
            'BMCU_TROCAR_ENDER', self.protect(self.change_tool)
        )
        self.printer.register_event_handler('klippy:ready', self.ready)

    def ready(self):
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
                return function(gcmd)
            except Exception as error:
                raise gcmd.error(str(error)) from error

        return execute

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
        result = self.original_wait_pullback(device, channel, *args, **kwargs)
        self.reactor.pause(self.reactor.monotonic() + 0.150)
        device.refresh()
        measured = (
            record['start'] - float(device.status['meters'][channel])
        ) * 1000.0
        try:
            if device.status['session_id'] != record['session']:
                raise RuntimeError('The BMCU restarted during pullback.')
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
            device.stop_all()
            raise
        result = dict(result)
        result['encoder_mm'] = measured
        result['after_switch_encoder_mm'] = measured
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

    def adapt_operation(self, previous):
        def execute(gcmd):
            try:
                return previous(gcmd)
            finally:
                self.restore_pullbacks()

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
                'BMCU_TROCAR_ENDER TOOL=%d' % tool
            )

        return execute

    def execute(self, text):
        with self.bmcu.printer_critical_section('integracao_ender'):
            self.gcode.run_script_from_command(text)

    def sensor(self, destination):
        status = destination.sensor_status('entry_sensor')
        if not status or status.get('filament_detected') not in (True, False):
            raise RuntimeError(
                ('The entry sensor did not return a valid reading.')
            )
        return bool(status['filament_detected'])

    def context(self, gcmd, load):
        device = self.bmcu.devices_by_name.get(self.device)
        if device is None or not device.ready:
            raise RuntimeError('The BMCU is not ready.')
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
            channel = int(device.status.get('now_channel', -1))
        if not 0 <= channel < 4:
            raise RuntimeError('The BMCU did not identify the active channel.')
        expected = (
            protocol.MOTION_IDLE if load else protocol.MOTION_BEFORE_PULL_BACK
        )
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

    def verify_session(self, device, session, channel):
        device.refresh()
        if device.status['session_id'] != session:
            raise RuntimeError('The BMCU restarted during the tool change.')
        if not device.status['present'][channel]:
            raise RuntimeError('The channel lost filament detection.')

    def position_head(self):
        toolhead = self.printer.lookup_object('toolhead')
        status = toolhead.get_status(self.reactor.monotonic())
        if not all(axis in status['homed_axes'] for axis in 'xyz'):
            raise RuntimeError(
                ('Run G28 before loading or unloading filament.')
            )
        z = min(
            toolhead.get_position()[2] + self.z_lift, status['axis_maximum'][2]
        )
        self.execute(
            (
                'M400\nG90\nG1 Z%.6f F300\nG1 X%.6f F1200\n'
                'G1 Y%.6f F1200\n'
                'M400'
            )
            % (z, self.purge_x, self.purge_y)
        )
        extruder = self.printer.lookup_object('extruder')
        status = extruder.get_status(self.reactor.monotonic())
        if not status.get('can_extrude'):
            raise RuntimeError('Heat the nozzle before changing filament.')

    def move_filament(self, mm, speed, chunk=10.0):
        extruder = self.printer.lookup_object('extruder')
        limit = min(chunk, extruder.max_e_dist)
        if limit <= 0:
            raise RuntimeError('The extrusion limit is set to zero.')
        count = int(math.ceil(abs(mm) / limit))
        if count:
            for _ in range(count):
                self.execute('G1 E%.9f F%.9f\nM400' % (mm / count, speed))

    def wipe(self, device, destination, channel, session):
        device.set_motion(channel, protocol.MOTION_ON_USE)
        remaining = self.purge_distance
        while remaining > 0.000001:
            self.verify_session(device, session, channel)
            if not self.sensor(destination):
                raise RuntimeError('The entry sensor cleared during purging.')
            step = min(10.0, remaining)
            self.move_filament(step, self.advance_speed)
            remaining -= step
        self.execute('G4 P1000')
        for _ in range(self.wipe_passes):
            self.execute(
                'G1 X%.6f F600\nG1 X%.6f F600\nM400'
                % (self.wipe_x, self.purge_x)
            )
        self.execute('G1 X%.6f F600\nM400' % self.wipe_x)

    def load(self, gcmd):
        device = None
        saved = False
        completed = False
        try:
            device, destination, channel = self.context(gcmd, True)
            session = device.status['session_id']
            device.stop_all()
            self.restore_pullbacks()
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
            while remaining > 0.000001:
                self.verify_session(device, session, channel)
                if not self.sensor(destination):
                    raise RuntimeError(
                        'The entry sensor cleared during loading.'
                    )
                device.set_motion(channel, protocol.MOTION_BEFORE_ON_USE)
                self.move_filament(min(10.0, remaining), self.advance_speed)
                device.stop_all()
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
        saved = False
        try:
            device, destination, channel = self.context(gcmd, False)
            session = device.status['session_id']
            self.execute('SAVE_GCODE_STATE NAME=BMCU_ENDER_RETIRADA')
            saved = True
            self.position_head()
            self.execute('M83\nM220 S100\nM221 S100')
            if not self.sensor(destination):
                raise RuntimeError(
                    'The entry sensor was already clear before unloading.'
                )
            marker = float(device.status['meters'][channel])
            deadline = self.reactor.monotonic() + self.timeout
            total = 0.0
            while self.sensor(destination):
                if self.reactor.monotonic() >= deadline:
                    raise RuntimeError(
                        (
                            'The entry sensor did not clear within the '
                            'configured '
                            'timeout.'
                        )
                    )
                self.move_filament(-2.0, self.pullback_speed, chunk=2.0)
                total += 2.0
                self.verify_session(device, session, channel)
                current = float(device.status['meters'][channel])
                if (marker - current) * 1000.0 >= 25.0 and self.sensor(
                    destination
                ):
                    device.stop_all()
                    self.reactor.pause(self.reactor.monotonic() + 0.050)
                    device.set_motion(
                        channel, protocol.MOTION_BEFORE_PULL_BACK
                    )
                    marker = current
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
            device.stop_all()
            self.reactor.pause(self.reactor.monotonic() + 0.050)
            device.refresh()
            self.temporary_pullbacks[device.name] = {
                'channel': channel,
                'start': float(device.status['meters'][channel]),
                'session': session,
            }
            self.bmcu._send_device_runtime_config(
                device, channel_retract_override=(channel, self.pullback)
            )
        except Exception as error:
            raise gcmd.error('Unload interrupted: %s' % error) from error
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
        tool = gcmd.get_int('TOOL', minval=1, maxval=4)
        purge = gcmd.get_int('PURGAR_SE_CARREGADO', 0, minval=0, maxval=1)
        head = self.printer.lookup_object('toolhead').get_status(
            self.reactor.monotonic()
        )
        if not all(axis in head['homed_axes'] for axis in 'xyz'):
            raise gcmd.error('Run G28 before changing filament.')
        if (
            not self.printer.lookup_object('extruder')
            .get_status(self.reactor.monotonic())
            .get('can_extrude')
        ):
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
                device = self.bmcu.devices_by_name[self.device]
                channel = tool - 1
                destination = self.bmcu._endpoint_for_channel(device, channel)
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
