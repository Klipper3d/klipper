"""Ender card projection; never replace upstream's bmcu_panel module."""
import hashlib
import json
import re

from .bmcu_core.release import PACKAGE_VERSION


def channel_revision(device, channel, metadata):
    identity = [device.name, channel, getattr(device, 'uid', ''),
                device.status.get('session_id'),
                metadata.get('material', ''), metadata.get('color', '')]
    return hashlib.sha256(json.dumps(identity).encode('utf-8')).hexdigest()


def panel_status(integration):
    manager = integration.bmcu
    device = manager.devices_by_name.get(integration.device)
    online = bool(device is not None and all((
        getattr(device, 'connected', False), getattr(device, 'ready', False),
        getattr(device, 'runtime_configured', False),
        getattr(device, 'status_reconciled', False),
        getattr(device, 'firmware_compatible', False) is True,
        not getattr(device, 'suspended', False),
    )))
    pending = getattr(getattr(manager, 'refill', None), 'manual_pending', None)
    pending = pending if isinstance(pending, dict) else {}
    refill_pending = bool(pending
                          and pending.get('device') == integration.device
                          and pending.get('endpoint') == integration.destination
                          and pending.get('channel') in range(4))
    stats = integration.printer.lookup_object('print_stats', None)
    now = integration.reactor.monotonic()
    print_state = (stats.get_status(now).get('state')
                   if stats is not None else 'unknown')
    busy = bool(manager.active_operations
                or getattr(integration, 'manual_operation', None) is not None
                or getattr(integration, 'pending_tool_change', None)
                or getattr(manager, 'print_plan_open', False)
                or getattr(manager, 'print_map_active', False)
                or refill_pending)
    metadata_getter = getattr(manager, '_channel_metadata', None)
    route_getter = getattr(manager, '_route_state', None)
    channels = []
    for channel in range(4):
        metadata = (metadata_getter(device, channel)
                    if device is not None and callable(metadata_getter) else {})
        route = (route_getter(device, channel)
                 if device is not None and callable(route_getter) else 2)
        present = getattr(device, 'status', {}).get('present', [])
        detected = present[channel] if len(present) == 4 else None
        state = 'offline' if not online else 'uncertain'
        if online and route in (0, 1) and detected in (True, False, 0, 1):
            state = ('loaded' if route == 1 else
                     'parked' if detected else 'empty')
        channels.append({
            'channel': channel + 1, 'material': metadata.get('material', ''),
            'color': metadata.get('color', '#808080'), 'state': state,
            'revision': (channel_revision(device, channel, metadata)
                         if device else ''),
        })
    route_error = bool(getattr(manager, 'last_error', None)
                       or getattr(device, 'last_error', None)
                       or sum(ch['state'] == 'loaded' for ch in channels) > 1)
    if online and route_error:
        for channel in channels:
            channel['state'] = 'uncertain'
    return {
        'schema': 1, 'package_version': PACKAGE_VERSION,
        'required_firmware': '1.0.4', 'device': integration.device,
        'online': online, 'busy': busy,
        'can_edit': bool(online and not busy
                         and not integration.printer.is_shutdown()
                         and print_state in ('standby', 'complete', 'cancelled')
                         and callable(metadata_getter)
                         and callable(getattr(
                             manager, 'cmd_SET_FILAMENT', None))
                         and all(ch['state'] != 'uncertain'
                                 for ch in channels)),
        'channels': channels, 'refill_pending': refill_pending,
        'refill_channel': pending['channel'] + 1 if refill_pending else -1,
    }


def set_panel_filament(integration, gcmd):
    status = panel_status(integration)
    if not status['can_edit']:
        raise gcmd.error('The BMCU must be compatible, ready and idle to edit.')
    channel = gcmd.get_int('CHANNEL', minval=1, maxval=4)
    material = str(gcmd.get('MATERIAL', '')).upper()
    color = '#' + str(gcmd.get('COLOR', '')).lstrip('#').upper()
    if not re.fullmatch(r'[A-Z][A-Z0-9+_.-]{0,31}', material):
        raise gcmd.error('MATERIAL must be a short material identifier.')
    if not re.fullmatch(r'#[0-9A-F]{6}', color):
        raise gcmd.error('COLOR must be RRGGBB or #RRGGBB.')
    if gcmd.get('REVISION') != status['channels'][channel - 1]['revision']:
        raise gcmd.error('Channel data changed; reopen the filament editor.')
    command = integration.gcode.create_gcode_command(
        'BMCU_SET_FILAMENT', 'BMCU_SET_FILAMENT', {
            'DEVICE': integration.device, 'CHANNEL': str(channel - 1),
            'MATERIAL': material, 'COLOR': color,
        },
    )
    return integration.bmcu.cmd_SET_FILAMENT(command)
