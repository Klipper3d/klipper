# SPDX-License-Identifier: GPL-3.0-or-later
class BMCUManager:
    def load_channel(self, device, channel, gcmd=None, tool=None,
                     prime_handshake=False, temperature_profile=None):
        self._require_standalone_operation('BMCU_LOAD', gcmd)
        channel = int(channel)
        if channel < 0 or channel > 3:
            raise BMCUError('BMCU channel must be within 0..3')
        endpoint = self._endpoint_for_channel(device, channel)
        if endpoint is None:
            raise BMCUError(
                '%s Channel %d is not connected to a printer head/extruder' %
                (device.name, channel + 1))
        if tool is None:
            tool = self._tool_for_channel(device, channel)
        try:
            tool = int(tool)
        except (TypeError, ValueError):
            tool = -1
        operation_label = ('T%d' % tool if tool >= 0 else
                           '%s Channel %d' % (device.name, channel + 1))
        route_key = self._route_key(device, channel)
        background_job = self._u1_background_jobs.get(endpoint.name)
        prefetched_u1_target = bool(
            endpoint.driver == 'snapmaker_u1' and
            isinstance(background_job, dict) and
            background_job.get('foreground_head_prefetched') and
            background_job.get('target_device') is device and
            int(background_job.get('target_channel', -1)) == channel and
            int(background_job.get('target_tool', -1)) == tool and
            background_job.get('endpoint') is endpoint and
            background_job.get('state') in ('handoff', 'ready', 'partial'))

        self._validate_endpoint_for_operation(
            endpoint, require_u1_ownership=False)
        self._reconcile_u1_pending_follower_commit(endpoint)
        self._reconcile_generic_pending_follower_commit(endpoint)
        occupied = self._loaded_devices_for_endpoint(
            endpoint.name, excluded_route=route_key)
        if len(occupied) > 1:
            raise BMCUError(
                'endpoint %s has multiple loaded Channels; reconcile manually' %
                endpoint.name)
        same_channel_follower = False
        if (self._route_state(device, channel) == protocol.ROUTE_LOADED and
                not occupied):
            detached_tail = bool(
                self._u1_tail_detached_matches(
                    endpoint, device, channel) or
                self._generic_tail_detached_matches(
                    endpoint, device, channel))
            if detached_tail:
                present = getattr(device, 'status', {}).get('present')
                same_channel_follower = bool(
                    isinstance(present, (list, tuple)) and
                    channel < len(present) and present[channel])
            if same_channel_follower:

                if (endpoint.driver == 'snapmaker_u1' and
                        not self._u1_tail_sensor_boundary_cleared(
                            endpoint, device, channel)):
                    raise BMCUError(
                        'new filament was inserted into exhausted Channel %d '
                        'before the old tail cleared the Snapmaker head sensor; '
                        'remove it and retry after the printer pauses at runout' %
                        (channel + 1))
            else:

                self._lock(device, endpoint, 'SELECT LOADED %s' % operation_label, channel=channel)
                phase = 'SELECT_LOADED_ENDPOINT'
                self._set_phase(device, phase)
                try:
                    self._arm_u1_persistent_hold(
                        endpoint, device, channel, 'select loaded %s' % operation_label)
                    self._validate_endpoint_for_operation(endpoint)
                    endpoint.suspend_managed_sensors()
                    endpoint.select()
                    endpoint.verify_selected()
                    device.set_motion(channel, protocol.MOTION_ON_USE)
                    self._commit_u1_route_ownership(
                        device, channel, protocol.ROUTE_LOADED)
                    endpoint.activate_runtime_sensor_takeover()
                    if hasattr(endpoint, 'sync_active_filament'):
                        endpoint.sync_active_filament(
                            self._channel_metadata(device, channel))
                    if tool >= 0:
                        self.loaded_tools[route_key] = tool
                        self.active_tool = tool
                        self._mark_print_route_loaded(
                            device, channel, tool)
                    else:
                        self.loaded_tools.pop(route_key, None)
                    self._save_runtime()
                    if gcmd:
                        gcmd.respond_info(
                            'BMCU selected already-loaded %s Channel %d on %s' %
                            (device.name, channel + 1, endpoint.name))
                except Exception as exc:
                    self._stop_on_failure(
                        device, endpoint, exc, phase, channel)
                    if gcmd:
                        raise gcmd.error(str(exc))
                    raise
                finally:
                    self._unlock(device, endpoint)
                if (detached_tail and tool >= 0 and
                        endpoint.driver == 'snapmaker_u1'):
                    self._rearm_snapmaker_detached_tail_monitor(
                        device, channel, tool, endpoint)
                if endpoint.driver == 'snapmaker_u1':
                    self._mark_u1_head_prepared(
                        endpoint.get('head_index', -1),
                        source='already-loaded BMCU route selected during print')

                return False
        source_device, source_channel = (
            (device, channel) if same_channel_follower else
            (occupied[0] if occupied else (None, None)))
        retain_selected_u1_head = (
            self._can_retain_selected_u1_head_for_swap(
                endpoint, source_device, source_channel,
                same_channel_follower=same_channel_follower))
        if source_device is not None:

            if endpoint.driver == 'snapmaker_u1':
                self._mark_u1_head_prepared(
                    endpoint.get('head_index', -1),
                    source='loaded BMCU source present before tool change')
            self._preempt_refill_for_toolchange(
                source_device, source_channel, endpoint)
        multi_device_switch = (
            source_device is not None and source_device.name != device.name)
        if multi_device_switch:
            self._lock_refill(source_device, device, endpoint, 'TOOL_SWITCH %s' % operation_label)
        else:
            self._lock(device, endpoint, 'LOAD %s' % operation_label, channel=channel)
        phase = 'PRECHECK'
        failure_device = device
        failure_channel = channel
        projection_snapshot = None
        path_calibration_sample = None
        path_calibration = None
        arrival = {}
        arrival_search_start_m = None
        arrival_preexisting_mm = 0.0
        target_physically_captured = False
        target_motion_started = False
        target_recovery_emptied = False
        release_new_u1_hold = False
        detached_source_handoff = False
        u1_hold_was_active = bool(
            endpoint.driver == 'snapmaker_u1' and
            self._u1_ownership_record(endpoint.name).get('persistent_hold'))
        self._set_phase(device, phase)
        load_completed = False
        load_temperature_finalized = False
        try:
            if (endpoint.driver == 'snapmaker_u1' and
                    isinstance(background_job, dict) and
                    background_job.get('target_device') is device and
                    int(background_job.get('target_channel', -1)) == channel):
                if (background_job.get('worker_scheduled') and
                        not background_job.get('worker_finished')):
                    raise BMCUError('U1 background worker has not released the route')
                background_job['foreground_consuming'] = True
            staged_record = self.prestaged.get(route_key)

            staged_for_target = bool(
                tool >= 0 and staged_record and
                staged_record.get('endpoint') == endpoint.name)
            if same_channel_follower:
                device.mark_unloaded(channel)
                device.refresh()
                if self._route_states_from_status(
                        device.status)[channel] != protocol.ROUTE_EMPTY:
                    raise BMCUError(
                        'same-Channel follower could not release the exhausted '
                        'upstream route before load')
            self._check_automatic_ready(
                device, channel, allow_uncertain=staged_for_target,
                allow_detached_handoff=same_channel_follower)
            self._arm_u1_persistent_hold(
                endpoint, device, channel, 'load %s' % operation_label)

            self._validate_endpoint_for_operation(endpoint)
            if source_device is not None:

                phase = 'UNLOAD_SOURCE'
                failure_device = source_device
                failure_channel = source_channel
                self._set_phase(source_device, phase)
                target_motion_started = True
                if self._ungripped_tail(
                        source_device, source_channel, endpoint):
                    if endpoint.driver == 'snapmaker_u1':
                        if same_channel_follower:
                            if not self._u1_tail_sensor_boundary_cleared(
                                    endpoint, device, channel):
                                raise BMCUError(
                                    'same-Channel follower cannot start until '
                                    'the old tail clears the Snapmaker head sensor')
                        else:
                            job = self._u1_background_jobs.get(endpoint.name)
                            source_prepared = bool(
                                isinstance(job, dict) and
                                job.get('source_tail_prepared') and
                                job.get('source_device') is source_device and
                                int(job.get('source_channel', -1)) ==
                                int(source_channel))
                            if not source_prepared:
                                self._prepare_snapmaker_ungripped_tail_handoff_locked(
                                    source_device, endpoint, source_channel,
                                    temperature_profile=(
                                        self._u1_active_temperature_profile()),
                                    restore_heater=False)
                    else:
                        self._prepare_generic_ungripped_tail_handoff_locked(
                            source_device, endpoint, source_channel)
                    detached_source_handoff = True
                else:
                    self._unload_locked(
                        source_device, endpoint, source_channel,
                        release_endpoint=False,
                        preserve_sensor_takeover=True,
                        retain_selected_endpoint=retain_selected_u1_head,
                        temperature_profile=self._u1_active_temperature_profile(),
                        reason='toolchange')
                failure_device = device
                failure_channel = channel
                phase = 'PRECHECK_TARGET'
                self._set_phase(device, phase)
            target_state = self._route_state(device, channel)
            if target_state == protocol.ROUTE_UNCERTAIN and not staged_for_target:
                raise BMCUError(
                    '%s Channel %d route is UNCERTAIN' %
                    (device.name, channel + 1))

            phase = 'SELECT_ENDPOINT'
            self._set_phase(device, phase)
            endpoint.suspend_managed_sensors()
            if retain_selected_u1_head:

                endpoint.verify_selected()
                logging.info(
                    'BMCU same-Head swap retained selected %s between Channels %d and %d',
                    endpoint.name, source_channel + 1, channel + 1)
            elif prefetched_u1_target:
                endpoint.verify_selected()
                logging.info(
                    'BMCU reused target Head %s selected during source pullback',
                    endpoint.name)
            else:
                endpoint.select()
                endpoint.verify_selected()
            if endpoint.driver == 'snapmaker_u1':
                staged = self.prestaged.get(route_key)
                if isinstance(staged, dict):
                    staged['target_selected'] = True
            capture_projection = getattr(
                endpoint, 'capture_active_filament_state', None)
            if callable(capture_projection):
                projection_snapshot = capture_projection()
            channel_metadata = self._channel_metadata(device, channel)
            material = channel_metadata.get('material', '')
            load_temperature_profile = self._u1_load_temperature_profile(
                endpoint, temperature_profile, material=material)
            logging.info(
                'BMCU load source: %s Channel %d -> %s; material=%s '
                'vendor=%s subtype=%s logical_tool=%s',
                device.name, channel + 1, endpoint.name,
                material or 'Unknown',
                channel_metadata.get('vendor', '') or 'generic',
                channel_metadata.get('subtype', '') or
                channel_metadata.get('profile_id', '') or 'generic',
                tool if tool >= 0 else 'manual')
            if hasattr(endpoint, 'sync_active_filament'):
                endpoint.sync_active_filament(channel_metadata)
            prepare_load_kwargs = {}
            if endpoint.driver == 'snapmaker_u1':

                prepare_load_kwargs['discard_position_prepared'] = bool(
                    retain_selected_u1_head or
                    (prefetched_u1_target and background_job.get(
                        'foreground_discard_position_prepared')))
            reuse_prepared_context = False
            if prefetched_u1_target:
                verifier = getattr(
                    endpoint, 'prepared_load_context_matches', None)
                reuse_prepared_context = bool(
                    callable(verifier) and
                    verifier(material, load_temperature_profile))
            if reuse_prepared_context:
                logging.info(
                    'BMCU reused nonblocking load temperature prepared during '
                    'source pullback for T%d on %s', tool, endpoint.name)
            else:
                self._endpoint_temperature_call(
                    endpoint, 'prepare_load', material, load_temperature_profile,
                    **prepare_load_kwargs)

            phase = 'ARRIVAL_SEARCH'
            self._set_phase(device, phase)
            is_prestaged = staged_for_target
            if is_prestaged:
                try:
                    arrival_preexisting_mm = max(
                        0.0, float(staged_record.get('distance_mm', 0.0) or 0.0))
                except (TypeError, ValueError, OverflowError):
                    arrival_preexisting_mm = 0.0
            entry_before = endpoint.sensor_detected('entry_sensor')
            configured_entry_sensor = str(
                endpoint.get('entry_sensor', '') or '').strip()
            if configured_entry_sensor and is_prestaged:
                if self._u1_has_authoritative_entry_sensor(endpoint):
                    entry_before = endpoint.require_entry_sensor_snapshot().get(
                        'physical_detected')
                staged_record['at_entry'] = entry_before is True
                staged_record['partial'] = entry_before is not True
                if entry_before is True:
                    staged_record['result'] = dict(
                        staged_record.get('result', {}),
                        ok=True, sensor_triggered=True,
                        controller_contact=False,
                        reason='owned_prestage_at_entry')
            partial_prestage = bool(
                is_prestaged and staged_record.get('partial') and
                not staged_record.get('at_entry'))
            staged_at_entry = bool(
                is_prestaged and staged_record.get('at_entry'))
            if partial_prestage and not staged_at_entry:
                max_route = float(endpoint.get(
                    'max_route_mm', self.max_route_mm) or self.max_route_mm)
                try:
                    staged_distance = float(
                        staged_record.get('distance_mm', 0.0) or 0.0)
                except (TypeError, ValueError, OverflowError):
                    staged_distance = 0.0
                if not math.isfinite(staged_distance):
                    staged_distance = 0.0
                remaining = max(50.0, max_route - max(0.0, staged_distance))
                final_search = float(endpoint.get(
                    'final_search_mm', 250.0) or 250.0)
                maximum_mm = min(max_route, max(final_search, remaining))
            else:
                maximum_mm = float(endpoint.get(
                    'final_search_mm' if is_prestaged else 'max_route_mm',
                    250.0 if is_prestaged else self.max_route_mm) or
                    (250.0 if is_prestaged else self.max_route_mm))
            contact_pct = self._device_loading_handoff_pct(
                device, endpoint)
            if partial_prestage and not staged_at_entry:
                if configured_entry_sensor:
                    logging.info(
                        'BMCU continuing partial T%d prestage on selected %s: '
                        'already %.1f mm, remaining entry-sensor '
                        'search %.1f mm',
                        int(tool), endpoint.name, max(0.0, staged_distance),
                        float(maximum_mm))
                else:
                    try:
                        partial_buffer_pct = int(
                            staged_record.get('buffer_pct', contact_pct))
                    except (TypeError, ValueError, OverflowError):
                        partial_buffer_pct = contact_pct
                    contact_pct = min(
                        94, max(contact_pct, partial_buffer_pct + 8))
                    logging.info(
                        'BMCU continuing partial T%d prestage on selected %s: '
                        'already %.1f mm, remaining search %.1f mm, controller '
                        'contact target %d%%',
                        int(tool), endpoint.name, max(0.0, staged_distance),
                        float(maximum_mm), int(contact_pct))
            timeout_s = float(endpoint.get(
                'contact_timeout', self.contact_timeout) or self.contact_timeout)
            timeout_s = self._effective_feed_timeout(
                device, maximum_mm, timeout_s)

            detached_follower_at_entry = False
            prepare_load_position = getattr(
                endpoint, 'prepare_load_position', None)
            if staged_at_entry or detached_follower_at_entry:

                if callable(prepare_load_position):
                    phase = 'POSITION_FOR_LOAD'
                    self._set_phase(device, phase)
                    prepare_load_position(material, load_temperature_profile)
                    phase = 'ARRIVAL_SEARCH'
                    self._set_phase(device, phase)
                arrival = (copy.deepcopy(staged_record.get('result', {}))
                           if staged_at_entry else {})
                arrival.update({
                    'ok': True,
                    'reason': (
                        'background_partial_reached_entry'
                        if partial_prestage and
                           not staged_record.get('at_entry') else
                        'background_prestaged_at_entry'
                        if staged_at_entry else
                        'detached_follower_already_at_entry'),
                })
            else:
                if entry_before is True and not is_prestaged:
                    raise BMCUError(
                        'RESIDUAL_FILAMENT: entry sensor is already active before load')
                target_motion_started = True
                if partial_prestage:

                    device.set_motion(channel, protocol.MOTION_IDLE)
                    device.refresh()
                    if int(device.status['motion'][channel]) != (
                            protocol.MOTION_IDLE):
                        raise BMCUError(
                            'U1_PRECHARGE_NOT_IDLE before selected-Head '
                            'arrival search')
                    logging.info(
                        'BMCU continuing partial T%d prestage on selected %s '
                        'with a separate SEND_OUT arrival search',
                        int(tool), endpoint.name)

                device.refresh()
                arrival_search_start_m = float(
                    device.status['meters'][channel])
                op_id, arrival_policy = (
                    self._start_endpoint_arrival_operation(
                        device, channel, endpoint, maximum_mm, contact_pct,
                        timeout_s, parked_precharge=False))

                if callable(prepare_load_position):
                    phase = 'POSITION_FOR_LOAD'
                    self._set_phase(device, phase)
                    prepare_load_position(material, load_temperature_profile)
                    phase = 'ARRIVAL_SEARCH'
                    self._set_phase(device, phase)
                if arrival_policy.get('sensor_authoritative'):
                    arrival = self._wait_sensor_arrival(
                        device, channel, endpoint, op_id, arrival_policy,
                        maximum_mm, poll_interval=0.050)
                else:
                    arrival = self._wait_feed_operation(
                        device, op_id, timeout_s + 2.0, endpoint,
                        'entry_sensor')
                arrival = self._resolve_endpoint_arrival_result(
                    device, channel, endpoint, arrival, arrival_policy,
                    timeout_s, allow_partial=False)
                if self.debug_enabled:
                    snapshot = endpoint.entry_debug_snapshot()
                    if snapshot.get('detailed'):
                        self._debug_log(
                            'U1 %s SEND_OUT result reason=%s ok=%d measured=%.1fmm '
                            'sensor_event=%d contact=%d buffer=%s%% motion=%s '
                            'entry_phase=%s source=%s enabled=%s public=%s callback=%s',
                            endpoint.name, arrival.get('reason'),
                            1 if arrival.get('ok') else 0,
                            float(arrival.get('measured_mm', 0.0) or 0.0),
                            1 if arrival.get('sensor_triggered') else 0,
                            1 if arrival.get('controller_contact') else 0,
                            device.status['buffer_pct'][channel],
                            device.status['motion'][channel],
                            snapshot.get('physical_detected'), snapshot.get('source'),
                            snapshot.get('enabled'), snapshot.get('public_detected'),
                            snapshot.get('callback_detected'))
                    else:
                        self._debug_log(
                            'BMCU generic %s SEND_OUT result reason=%s ok=%d '
                            'measured=%.1fmm contact=%d buffer=%s%% motion=%s '
                            'entry_sensor=%s',
                            endpoint.name, arrival.get('reason'),
                            1 if arrival.get('ok') else 0,
                            float(arrival.get('measured_mm', 0.0) or 0.0),
                            1 if arrival.get('controller_contact') else 0,
                            device.status['buffer_pct'][channel],
                            device.status['motion'][channel],
                            snapshot.get('detected'))
                if arrival.get('cancel_requested'):
                    raise BMCUError('U1_SEND_OUT_CANCELLED')
                if partial_prestage:
                    arrival['continued_from_neutral_precharge'] = True
                if not arrival.get('ok'):
                    raise BMCUError('ENDPOINT_NOT_REACHED: %s after %.1f mm' %
                                    (arrival.get('reason'),
                                     arrival.get('measured_mm', 0.0)))
                if (configured_entry_sensor and
                        not arrival.get('sensor_triggered')):
                    raise BMCUError(
                        'ENDPOINT_NOT_REACHED: %s stopped after %.1f mm at '
                        '%d%% buffer without entry-sensor confirmation' %
                        (arrival.get('reason'),
                         float(arrival.get('measured_mm', 0.0) or 0.0),
                         int(device.status['buffer_pct'][channel])))
                if partial_prestage:
                    try:
                        continuation_mm = float(
                            arrival.get('measured_mm', 0.0) or 0.0)
                    except (TypeError, ValueError, OverflowError):
                        continuation_mm = 0.0
                    if math.isfinite(continuation_mm):
                        staged_record['distance_mm'] = min(
                            max_route,
                            max(0.0, staged_distance) +
                            max(0.0, continuation_mm))
                    staged_record['buffer_pct'] = int(
                        device.status['buffer_pct'][channel])
                    staged_record['result'] = dict(arrival)
                if (not self._u1_has_authoritative_entry_sensor(endpoint) and
                        configured_entry_sensor and
                        endpoint.sensor_detected('entry_sensor') is not True):
                    raise BMCUError(
                        'ENDPOINT_NOT_REACHED: %s stopped after %.1f mm at '
                        '%d%% buffer without configured entry-sensor confirmation' %
                        (arrival.get('reason'),
                         float(arrival.get('measured_mm', 0.0) or 0.0),
                         int(device.status['buffer_pct'][channel])))

            generic_contract = (endpoint.driver == 'generic_single_extruder')
            if (not generic_contract and not is_prestaged and
                    not detached_source_handoff):
                path_calibration_sample = self._measure_path_calibration(
                    device, channel, endpoint, arrival)
            elif not generic_contract:
                self._clear_path_learning_observation(device, channel)
            phase = ('TOOLHEAD_PREPARATION' if generic_contract
                     else 'PREPARE_BITE')
            self._set_phase(device, phase)
            u1_pressure_evidence = None
            if self.debug_enabled and self._u1_has_authoritative_entry_sensor(endpoint):
                snapshot = endpoint.entry_debug_snapshot()
                self._debug_log(
                    'U1 %s PREPARE_BITE begin buffer=%s%% motion=%s '
                    'entry_phase=%s source=%s enabled=%s',
                    endpoint.name, device.status['buffer_pct'][channel],
                    device.status['motion'][channel],
                    snapshot.get('physical_detected'), snapshot.get('source'),
                    snapshot.get('enabled'))
            if self._u1_has_authoritative_entry_sensor(endpoint):

                device.set_motion(channel, protocol.MOTION_BEFORE_ON_USE)
            if not generic_contract:
                self._endpoint_temperature_call(
                    endpoint, 'ensure_bite_ready', material,
                    load_temperature_profile)
            if self._u1_has_authoritative_entry_sensor(endpoint):
                u1_pressure_evidence = self._wait_u1_firmware_pressure_hold(
                    device, channel, endpoint, timeout_s,
                    arrival_evidence=arrival)
            else:
                device.set_motion(channel, protocol.MOTION_BEFORE_ON_USE)

            if not generic_contract:
                phase = 'BITE'
                self._set_phase(device, phase)
                if endpoint.driver == 'snapmaker_u1':
                    self._drop_prestage_record(route_key, release_sensor=False)
            bite_ok = False
            bite_evidence = {}

            confirmed_head_advance_mm = 0.0
            if detached_source_handoff:
                if generic_contract:

                    endpoint.prepare_toolhead_for_use(material, reason='refill')
                    path_calibration_sample = self._measure_path_calibration(
                        device, channel, endpoint, arrival)
                    bite_ok = True
                    bite_evidence = {
                        'confirmation': 'printer_toolhead_prepare_macro',
                        'reason': 'refill',
                        'post_gears': endpoint.sensor_detected(
                            'post_gears_sensor'),
                    }
                else:

                    handoff_started_in_print = self._print_state() in (
                        'printing', 'paused', 'pause')
                    endpoint.record_load_coil_evidence(
                        coil_path_baseline=endpoint.capture_signal())
                    bite_evidence = (
                        self._capture_snapmaker_detached_tail_with_follower(
                            source_device, source_channel, device, channel,
                            endpoint, material=material,
                            cancel_check=lambda: bool(
                                handoff_started_in_print and
                                self._print_state() not in
                                ('printing', 'paused', 'pause'))))
                    bite_ok = bool(
                        isinstance(bite_evidence, dict) and
                        bite_evidence.get('captured'))
                    if bite_ok:
                        try:
                            confirmed_head_advance_mm += max(
                                0.0, float(
                                    bite_evidence.get('extruded_mm', 0.0)))
                        except (TypeError, ValueError, OverflowError):
                            pass
            else:
                if self._u1_has_authoritative_entry_sensor(endpoint):

                    native_signal_start = endpoint.capture_signal()
                    endpoint.record_load_coil_evidence(
                        coil_path_baseline=native_signal_start)
                    endpoint.extrude(self.bite_mm, self.bite_feed)
                    entry_after_bite = endpoint.entry_sensor_snapshot()
                    native_signal_delta = None
                    try:
                        buffer_after_bite = int(
                            device.status['buffer_pct'][channel])
                    except (TypeError, ValueError, KeyError, IndexError):
                        buffer_after_bite = -1
                    arrival_confirmed = bool(
                        arrival.get('sensor_triggered'))
                    bite_evidence = {
                        'attempt': 1,
                        'confirmation': 'u1_arrival_and_firmware_pressure',
                        'entry_sensor_phase_diagnostic': copy.deepcopy(
                            entry_after_bite),
                        'buffer_pct': buffer_after_bite,
                        'pressure_hold': copy.deepcopy(u1_pressure_evidence),
                        'arrival_motion': bool(arrival.get('sensor_triggered')),
                        'arrival_contact': bool(arrival.get('controller_contact')),
                        'native_signal_delta': native_signal_delta,
                    }

                    bite_ok = bool(
                        arrival_confirmed and
                        isinstance(u1_pressure_evidence, dict))
                    if bite_ok:
                        confirmed_head_advance_mm += float(self.bite_mm)
                elif generic_contract:
                    endpoint.prepare_toolhead_for_use(
                        material, reason=('toolchange' if source_device is not None else 'load'))
                    path_calibration_sample = self._measure_path_calibration(
                        device, channel, endpoint, arrival)
                    bite_ok = True
                    bite_evidence = {
                        'confirmation': 'printer_toolhead_load_macro',
                        'post_gears': endpoint.sensor_detected('post_gears_sensor'),
                    }
                else:
                    for attempt in range(self.bite_retries):
                        before_m = device.status['meters'][channel]
                        before_buffer = device.status['buffer_pct'][channel]
                        native_signal_start = endpoint.capture_signal()
                        endpoint.extrude(self.bite_mm, self.bite_feed)
                        device.refresh()
                        moved_mm = abs(
                            device.status['meters'][channel] - before_m) * 1000.0
                        buffer_drop = (
                            before_buffer - device.status['buffer_pct'][channel])
                        post_gears = endpoint.sensor_detected(
                            'post_gears_sensor')
                        native_signal_delta = endpoint.capture_signal_delta(
                            native_signal_start, material)
                        bite_evidence = {
                            'attempt': attempt + 1, 'encoder_mm': moved_mm,
                            'buffer_drop': buffer_drop,
                            'post_gears': post_gears,
                            'native_signal_delta': native_signal_delta,
                        }
                        if (post_gears is True or
                                moved_mm >=
                                self.bite_mm * self.bite_encoder_ratio or
                                buffer_drop >= self.bite_buffer_delta or
                                endpoint.capture_signal_ok(
                                    native_signal_delta, material)):
                            bite_ok = True
                            confirmed_head_advance_mm += float(self.bite_mm)
                            break
                        if self.retry_retract_mm > 0:
                            endpoint.extrude(
                                -self.retry_retract_mm, self.bite_feed)
                            device.set_motion(
                                channel, protocol.MOTION_SEND_OUT)
                            self.reactor.pause(
                                self.reactor.monotonic() + 0.25)
                            device.set_motion(
                                channel, protocol.MOTION_BEFORE_ON_USE)
            if self.debug_enabled:
                self._debug_log(
                    'U1 %s BITE result ok=%d evidence=%s', endpoint.name,
                    1 if bite_ok else 0,
                    json.dumps(bite_evidence, sort_keys=True, default=str))
            if not bite_ok:
                raise BMCUError(
                    'BITE_FAILED: %s' %
                    json.dumps(bite_evidence, sort_keys=True))
            if generic_contract:
                phase = 'TOOLHEAD_PREPARATION'
                self._set_phase(device, phase)
                entry_sensor_after_capture = endpoint.sensor_detected('entry_sensor')
                post_gears = endpoint.sensor_detected('post_gears_sensor')
                if str(endpoint.get('post_gears_sensor', '') or '').strip() and post_gears is False:
                    raise BMCUError(
                        'TOOLHEAD_LOAD_FAILED: configured post-gears sensor is not active after printer load macro')
                capture_moved = 0.0
                capture_mm = 0.0
                native_capture_delta = None
                target_physically_captured = True
            else:
                phase = 'CAPTURE'
                self._set_phase(device, phase)
                before_capture = device.status['meters'][channel]
                u1_readonly_path = self._u1_has_authoritative_entry_sensor(endpoint)
                capture_mm = float(self.capture_mm)
                native_capture_start = (
                    None if u1_readonly_path else endpoint.capture_signal())
                endpoint.extrude(capture_mm, self.capture_feed)
                entry_sensor_after_capture = (
                    endpoint.entry_sensor_snapshot()
                    if u1_readonly_path else
                    endpoint.sensor_detected('entry_sensor'))
                post_gears = endpoint.sensor_detected('post_gears_sensor')
                native_capture_end = (
                    None if u1_readonly_path else endpoint.capture_signal())
                native_capture_delta = None
                if native_capture_start is not None and native_capture_end is not None:
                    native_capture_delta = abs(
                        float(native_capture_end) - float(native_capture_start))
                if u1_readonly_path:

                    capture_moved = 0.0
                    if (not arrival.get('sensor_triggered') or
                            not isinstance(u1_pressure_evidence, dict)):
                        raise BMCUError(
                            'CAPTURE_FAILED: Snapmaker route lacks prior arrival and '
                            'firmware-pressure evidence')
                    logging.info(
                        'BMCU Snapmaker %s fixed capture accepted: arrival_ok=1 '
                        'buffer=%d%% phase=%s coil_boundary_query=skipped',
                        endpoint.name,
                        int(device.status['buffer_pct'][channel]),
                        entry_sensor_after_capture.get('physical_detected')
                        if isinstance(entry_sensor_after_capture, dict) else None)
                else:
                    device.refresh()
                    capture_moved = abs(
                        device.status['meters'][channel] - before_capture) * 1000.0
                    if (post_gears is not True and
                            capture_moved <
                            capture_mm * self.capture_encoder_ratio and
                            not endpoint.capture_signal_ok(
                                native_capture_delta, material)):
                        raise BMCUError(
                            'CAPTURE_FAILED: encoder moved %.2f mm; entry_sensor=%s '
                            'post_gears=%s native_signal_delta=%s' %
                            (capture_moved, entry_sensor_after_capture, post_gears,
                             native_capture_delta))
                if self.debug_enabled and endpoint.driver == 'snapmaker_u1':
                    self._debug_log(
                        'U1 %s CAPTURE completed buffer=%s%% entry=%s '
                        'coil_boundary_query=skipped confirmed_head_before=%.1fmm',
                        endpoint.name, device.status['buffer_pct'][channel],
                        (entry_sensor_after_capture.get('physical_detected')
                         if isinstance(entry_sensor_after_capture, dict)
                         else entry_sensor_after_capture),
                        confirmed_head_advance_mm)
                target_physically_captured = True
                confirmed_head_advance_mm += float(capture_mm)
                record_load_path_advance = getattr(
                    endpoint, 'record_load_path_advance', None)
                if callable(record_load_path_advance):
                    record_load_path_advance(confirmed_head_advance_mm)

            if detached_source_handoff:

                phase = 'COMMIT_FOLLOWER_ROUTE'
                self._set_phase(device, phase)
                if endpoint.driver == 'snapmaker_u1':
                    self._arm_u1_follower_commit(
                        endpoint, source_device, source_channel,
                        device, channel, logical_tool=tool)
                else:
                    self._arm_generic_follower_commit(
                        endpoint, source_device, source_channel,
                        device, channel, logical_tool=tool)
                device.mark_loaded(channel)
                device.refresh()
                raw_target = self._route_states_from_status(
                    device.status)[channel]
                if raw_target != protocol.ROUTE_LOADED:
                    raise BMCUError(
                        '%s Channel %d did not commit LOADED after detached-tail '
                        'follower capture' % (device.name, channel + 1))
                if endpoint.driver == 'snapmaker_u1':
                    self._finalize_snapmaker_tail_handoff_after_follower(
                        source_device, source_channel, endpoint,
                        follower_device=device, follower_channel=channel,
                        logical_tool=tool,
                        reason=('new BMCU follower physically captured and '
                                'committed LOADED before final prime'))
                else:
                    self._finalize_generic_tail_handoff_after_follower(
                        source_device, source_channel, endpoint,
                        follower_device=device, follower_channel=channel,
                        reason=('generic follower physically captured and '
                                'committed LOADED before final prime'))

            phase = ('BEFORE_ON_USE'
                     if endpoint.driver == 'snapmaker_u1' else 'TOOLHEAD_PREPARATION')
            self._set_phase(device, phase)
            load_ready_evidence = self._endpoint_temperature_call(
                endpoint, 'load_ready', material, load_temperature_profile)
            if self.debug_enabled and endpoint.driver == 'snapmaker_u1':
                self._debug_log(
                    'U1 %s BEFORE_ON_USE passive evidence accepted: %s',
                    endpoint.name,
                    json.dumps(load_ready_evidence or {}, sort_keys=True,
                               default=str))
            self._endpoint_temperature_call(
                endpoint, 'prime', material, load_temperature_profile)
            if self.debug_enabled and endpoint.driver == 'snapmaker_u1':
                self._debug_log(
                    'U1 %s prime completed; switching firmware motion '
                    'BEFORE_ON_USE -> ON_USE', endpoint.name)
            device.set_motion(channel, protocol.MOTION_ON_USE)
            self._clear_u1_tail_detached(
                endpoint, device, channel,
                'new BMCU source physically captured and loaded')
            self._commit_u1_route_ownership(
                device, channel, protocol.ROUTE_LOADED)
            endpoint.activate_runtime_sensor_takeover()
            if hasattr(endpoint, 'sync_active_filament'):
                endpoint.sync_active_filament(
                    self._channel_metadata(device, channel))
            if tool >= 0:
                self.loaded_tools[route_key] = tool
                self.active_tool = tool
                self._mark_print_route_loaded(device, channel, tool)
            else:
                self.loaded_tools.pop(route_key, None)
            self._save_runtime()
            self.prestaged.pop(route_key, None)
            if path_calibration_sample is not None:
                try:
                    path_calibration = self._commit_path_calibration(
                        device, channel, endpoint, path_calibration_sample,
                        reason='complete load, capture and prime succeeded')
                except Exception as calibration_exc:
                    logging.exception(
                        'BMCU could not persist route-length calibration for %s Channel %d',
                        device.name, channel + 1)
                    path_calibration = {
                        'error': str(calibration_exc)[:160],
                        'sample_mm': path_calibration_sample.get('sample_mm'),
                        'measurement': path_calibration_sample.get('measurement'),
                    }
            finish_load_temperature = getattr(
                endpoint, 'finish_load_temperature', None)
            if callable(finish_load_temperature):
                phase = 'RESTORE_WORKING_TEMPERATURE'
                self._set_phase(device, phase)
                if not finish_load_temperature(success=True):
                    raise BMCUError(
                        'Snapmaker Head %d did not restore its working '
                        'temperature after BMCU load' % (endpoint._head() + 1))
                load_temperature_finalized = True
            finish_load_position = getattr(
                endpoint, 'finish_load_position', None)
            if callable(finish_load_position):
                phase = 'SAFE_LOAD_EGRESS'
                self._set_phase(device, phase)
                if not finish_load_position():
                    raise BMCUError(
                        'Snapmaker Head %d did not reach the stock XY idle '
                        'position after BMCU load cleaning' %
                        (endpoint._head() + 1))
            if (prime_handshake and tool >= 0 and
                    endpoint.driver == 'snapmaker_u1'):

                self._u1_preextrude_primed_tools[tool] = (
                    self.reactor.monotonic())
            load_completed = True
            if endpoint.driver == 'snapmaker_u1':
                self._mark_u1_head_prepared(
                    endpoint.get('head_index', -1),
                    source=('BMCU T%d' % tool if tool >= 0 else
                            '%s Channel %d' % (device.name, channel + 1)))
            self.last_diagnostic = {
                'operation': 'load', 'tool': tool, 'device': device.name,
                'channel': channel, 'endpoint': endpoint.name,
                'arrival': arrival, 'bite': bite_evidence,
                'path_calibration': copy.deepcopy(path_calibration),
                'capture_encoder_mm': capture_moved,
                'capture_entry_sensor': entry_sensor_after_capture,
                'capture_native_signal_delta': native_capture_delta,
                'u1_pressure_hold': copy.deepcopy(u1_pressure_evidence),
                'confirmed_head_advance_mm': confirmed_head_advance_mm,
                'nozzle_path': {
                    'stock_total_mm': endpoint.config.get(
                        '_u1_last_nozzle_target_mm'),
                    'remaining_mm': endpoint.config.get(
                        '_u1_last_nozzle_search_mm'),
                    'coil_delta': endpoint.config.get(
                        '_u1_last_coil_delta'),
                    'coil_capture_delta': endpoint.config.get(
                        '_u1_last_coil_capture_delta'),
                    'coil_final_delta': endpoint.config.get(
                        '_u1_last_coil_final_delta'),
                    'coil_path_delta': endpoint.config.get(
                        '_u1_last_coil_path_delta'),
                    'coil_confirmed': endpoint.config.get(
                        '_u1_last_coil_confirmed'),
                } if endpoint.driver == 'snapmaker_u1' else {},
                'temperature_profile': copy.deepcopy(load_temperature_profile),
                'load_temperature': {
                    'finalized': bool(load_temperature_finalized),
                    'head': int(endpoint.get('head_index', -1)),
                },
            }
            if gcmd:
                if tool >= 0:
                    gcmd.respond_info(
                        'BMCU loaded T%d from %s Channel %d into %s' %
                        (tool, device.name, channel + 1, endpoint.name))
                else:
                    gcmd.respond_info(
                        'BMCU loaded %s Channel %d into %s' %
                        (device.name, channel + 1, endpoint.name))
        except Exception as exc:
            cancel_during_load = bool(
                self._u1_cancel_requested or
                (isinstance(arrival, dict) and
                 arrival.get('cancel_requested')))
            if (endpoint.driver == 'snapmaker_u1' and
                    not cancel_during_load and
                    (target_motion_started or arrival_preexisting_mm > 0.0) and
                    not target_physically_captured and
                    not detached_source_handoff and
                    phase in ('POSITION_FOR_LOAD', 'ARRIVAL_SEARCH',
                              'PREPARE_BITE')):
                try:
                    target_recovery_emptied = (
                        self._rollback_incomplete_u1_load_locked(
                            device, endpoint, channel, phase,
                            forward_mm=(arrival.get('measured_mm')
                                        if isinstance(arrival, dict) else None),
                            arrival_start_m=arrival_search_start_m,
                            preexisting_mm=arrival_preexisting_mm))
                except Exception as recovery_exc:

                    try:
                        device.stop_all()
                    except Exception:
                        pass
                    logging.exception(
                        'BMCU incomplete pre-BITE U1 load rollback failed for '
                        '%s Channel %d after %s: %s',
                        device.name, channel + 1, phase, recovery_exc)
            if (endpoint.driver == 'snapmaker_u1' and
                    not u1_hold_was_active and not target_motion_started):
                release_new_u1_hold = True
            if (projection_snapshot is not None and
                    not target_physically_captured and
                    not target_recovery_emptied):
                restore_projection = getattr(
                    endpoint, 'restore_active_filament_state', None)
                if callable(restore_projection):
                    try:
                        restore_projection(projection_snapshot)
                    except Exception:
                        logging.exception(
                            'BMCU could not roll back Endpoint %s material projection',
                            endpoint.name)
            if (endpoint.driver == 'snapmaker_u1' and tool >= 0 and
                    target_physically_captured and phase == 'BEFORE_ON_USE' and
                    self._print_state() in ('printing', 'paused', 'pause') and
                    'U1_TOOLHEAD_NOT_LOADED:' in str(exc)):
                try:
                    device.refresh()
                    recovery_route_state = self._route_states_from_status(
                        device.status)[channel]
                    if recovery_route_state in (
                            protocol.ROUTE_UNCERTAIN, protocol.ROUTE_LOADED):
                        self._u1_toolchange_resume_pending = {
                            'kind': 'before_on_use',
                            'phase': 'verify',
                            'tool': int(tool),
                            'device': device.name,
                            'channel': int(channel),
                            'endpoint': endpoint.name,
                            'route_state': int(recovery_route_state),
                            'coil_baseline': endpoint.config.get(
                                '_u1_last_coil_baseline'),
                            'coil_threshold': endpoint.config.get(
                                '_u1_last_coil_threshold'),
                        }
                        logging.warning(
                            'BMCU armed automatic U1 T%d BEFORE_ON_USE recovery '
                            'for %s Channel %d on normal RESUME',
                            int(tool), device.name, channel + 1)
                except Exception:
                    logging.exception(
                        'BMCU could not arm automatic U1 BEFORE_ON_USE recovery')
            preserve_target_hold = bool(
                getattr(exc, 'preserve_bmcu_hold', False))
            if (source_device is not None and
                    source_device is not failure_device and
                    not preserve_target_hold):
                try:
                    source_device.stop_all()
                except Exception:
                    logging.exception(
                        'BMCU could not stop source Channel after tool-switch failure')
            self._stop_on_failure(
                failure_device, endpoint, exc, phase, failure_channel,
                pause_print=not cancel_during_load)
            if gcmd:
                raise gcmd.error(str(exc))
            raise
        finally:
            finish_load_temperature = getattr(
                endpoint, 'finish_load_temperature', None)
            if (not load_temperature_finalized and
                    callable(finish_load_temperature)):
                try:
                    if not finish_load_temperature(success=False):
                        logging.error(
                            'BMCU could not restore the pre-load temperature '
                            'target on %s', endpoint.name)
                except Exception:
                    logging.exception(
                        'BMCU load temperature cleanup failed on %s',
                        endpoint.name)
            if multi_device_switch:
                self._unlock_refill(source_device, device, endpoint)
            else:
                self._unlock(device, endpoint)
            if release_new_u1_hold:
                try:
                    self._release_u1_persistent_hold_if_safe(
                        endpoint, 'load preflight failed before filament motion')
                except Exception:
                    logging.exception(
                        'BMCU could not release U1 ownership after load preflight failure')

        return True
