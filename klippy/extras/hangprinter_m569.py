# Minimal Hangprinter compatibility helpers for RepRapFirmware-style M569 gcodes.
import math
import shlex


MIN_FORCE_MODE_THRESHOLD_N = 0.001


def _parse_motor_address(raw_value):
    if raw_value is None:
        return None
    text = str(raw_value).strip()
    if not text:
        return None
    parts = text.split('.', 1)
    can_address = int(parts[0])
    driver_index = 0
    if len(parts) > 1 and parts[1]:
        driver_index = int(parts[1])
    return {
        'can_address': can_address,
        'driver': driver_index,
    }


def _motor_address_key(motor_address):
    if motor_address is None:
        return None
    return '%d.%d' % (
        motor_address['can_address'],
        motor_address.get('driver', 0),
    )


def _parse_driver_list(raw_value):
    descriptors = []
    for item in str(raw_value).split(':'):
        text = item.strip()
        if not text:
            continue
        descriptors.append(text)
    return descriptors


def _parse_float_list(raw_value):
    values = []
    for item in str(raw_value).split(':'):
        text = item.strip()
        if not text:
            continue
        values.append(float(text))
    return values


def _get_param_text(gcmd, name):
    value = gcmd.get(name, None)
    if value is not None:
        return str(value)

    raw_getter = getattr(gcmd, 'get_raw_command_parameters', None)
    if not callable(raw_getter):
        return None

    rawparams = raw_getter()
    if not rawparams:
        return None

    prefix = name.upper()
    for token in shlex.split(rawparams, posix=True):
        upper = token.upper()
        if upper.startswith(prefix + '='):
            return token[len(prefix) + 1:]
        if upper.startswith(prefix):
            return token[len(prefix):]
    return None


class HangprinterM569:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.driver_states = {}
        self.gcode.register_command('M569.3', self.cmd_M569_3,
                                    desc=self.cmd_M569_3_help)
        self.gcode.register_command('M569.4', self.cmd_M569_4,
                                    desc=self.cmd_M569_4_help)

    cmd_M569_3_help = "Read motor encoder positions (placeholder for terminal-side simulation bridge)"
    cmd_M569_4_help = "Set motor torque mode from requested line force (placeholder driver backend)"

    def _get_driver_lookup(self):
        toolhead = self.printer.lookup_object('toolhead', None)
        if toolhead is None:
            return {
                'by_name': {},
                'by_address': {},
                'has_configured_addresses': False,
            }
        kin = toolhead.get_kinematics()
        steppers = list(kin.get_steppers())
        motor_descriptors = list(getattr(kin, 'm569_driver_descriptors', []))
        flex_helper = getattr(kin, 'flex_helper', None)
        mechanical_advantage = list(getattr(flex_helper, 'mechanical_advantage', []))
        driver_lookup = {
            'by_name': {},
            'by_address': {},
            'has_configured_addresses': False,
        }
        for index, stepper in enumerate(steppers):
            descriptor = motor_descriptors[index] if index < len(motor_descriptors) else {}
            stepper_name = descriptor.get('stepper_name') or stepper.get_name()
            motor_address = descriptor.get('motor_address')
            driver_key = _motor_address_key(motor_address) or stepper_name
            rotation_distance_mm, _steps_per_rotation = stepper.get_rotation_distance()
            invert_dir, _orig_invert_dir = stepper.get_dir_inverted()
            mech_adv = 1
            if index < len(mechanical_advantage) and mechanical_advantage[index]:
                mech_adv = mechanical_advantage[index]
            driver_entry = {
                'stepper_name': stepper_name,
                'motor_address': motor_address,
                'driver_key': driver_key,
                'has_motor_address': motor_address is not None,
                'mechanical_advantage': mech_adv,
                'rotation_distance_mm': rotation_distance_mm,
                'invert_dir': bool(invert_dir),
            }
            driver_lookup['by_name'][stepper_name] = driver_entry
            if motor_address is not None:
                driver_lookup['by_address'][_motor_address_key(motor_address)] = driver_entry
                driver_lookup['has_configured_addresses'] = True
        return driver_lookup

    def _resolve_driver_token(self, token, driver_lookup):
        text = str(token).strip()
        if not text:
            return {'error': f"{text} did not match any stepper name or m569_address."}

        by_name = driver_lookup.get('by_name', {})
        by_address = driver_lookup.get('by_address', {})

        name_match = by_name.get(text)
        if name_match is not None:
            if driver_lookup.get('has_configured_addresses') and not name_match.get('has_motor_address'):
                return {'error': 'm569_address not configured for %s' % (name_match['stepper_name'],)}
            return {'descriptor': name_match}

        address_match = None
        try:
            address_match = by_address.get(_motor_address_key(_parse_motor_address(text)))
        except (TypeError, ValueError):
            address_match = None
        if address_match is not None:
            return {'descriptor': address_match}

        return {'error': f"{text} did not match any stepper name or m569_address."}

    def _compute_torque_from_force(self, driver_config, force_newtons):
        position_mode = abs(force_newtons) < MIN_FORCE_MODE_THRESHOLD_N
        if position_mode:
            return 0., True
        mech_adv = driver_config['mechanical_advantage'] or 1
        line_tension_n = force_newtons / mech_adv
        # Klipper's winch stepper rotation_distance is line mm per motor
        # rotation, so it already folds spool radius and gearing into one
        # effective radius.
        effective_radius_m = (
            abs(driver_config['rotation_distance_mm']) / (2.0 * math.pi)
        ) * 0.001
        torque_nm = abs(line_tension_n * effective_radius_m)
        if not driver_config['invert_dir']:
            torque_nm = -torque_nm
        return torque_nm, False

    def _set_placeholder_driver_state(self, driver_key, force_newtons,
                                      torque_nm, position_mode):
        self.driver_states[driver_key] = {
            'force_newtons': force_newtons,
            'torque_nm': 0. if position_mode else torque_nm,
            'position_mode': bool(position_mode),
        }

    def cmd_M569_3(self, gcmd):
        p_value = _get_param_text(gcmd, 'P')
        if p_value is None:
            gcmd.respond_raw("Error: M569: missing parameter 'P'")
            return
        try:
            drivers = _parse_driver_list(p_value)
        except (TypeError, ValueError):
            gcmd.respond_raw("Error: M569.3 unable to parse parameters")
            return
        if not drivers:
            gcmd.respond_raw("Error: M569: missing parameter 'P'")
            return
        driver_lookup = self._get_driver_lookup()
        for descriptor in drivers:
            resolution = self._resolve_driver_token(descriptor, driver_lookup)
            if 'error' in resolution:
                gcmd.respond_raw(resolution['error'])
                return
        # The terminal bridge overrides this placeholder reply with simulated
        # encoder angles when hp-sim is attached over gcode_ws.
        gcmd.respond_raw("Error: M569.3: Message not received")

    def cmd_M569_4(self, gcmd):
        p_value = _get_param_text(gcmd, 'P')
        if p_value is None:
            gcmd.respond_raw("Error: M569: missing parameter 'P'")
            return
        t_value = _get_param_text(gcmd, 'T')
        if t_value is None:
            gcmd.respond_raw("Error: M569.4 missing parameter 'T'")
            return
        try:
            drivers = _parse_driver_list(p_value)
            forces = _parse_float_list(t_value)
        except (TypeError, ValueError):
            gcmd.respond_raw("Error: M569.4 unable to parse parameters")
            return
        if not drivers:
            gcmd.respond_raw("Error: M569: missing parameter 'P'")
            return
        if len(forces) != len(drivers):
            gcmd.respond_raw("M569.4 requires one T value per P")
            return
        driver_lookup = self._get_driver_lookup()
        response_parts = []
        for descriptor, force_newtons in zip(drivers, forces):
            resolution = self._resolve_driver_token(descriptor, driver_lookup)
            if 'error' in resolution:
                gcmd.respond_raw(resolution['error'])
                return
            driver_config = resolution['descriptor']
            torque_nm, position_mode = self._compute_torque_from_force(
                driver_config, force_newtons)
            self._set_placeholder_driver_state(
                driver_config['driver_key'], force_newtons, torque_nm, position_mode)
            if position_mode:
                response_parts.append("pos_mode, ")
            else:
                response_parts.append("%.6f Nm, " % (torque_nm,))
        gcmd.respond_raw("".join(response_parts))


def load_config(config):
    return HangprinterM569(config)
