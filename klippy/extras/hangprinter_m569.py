# Minimal Hangprinter compatibility helpers for RepRapFirmware-style M569 gcodes.
import math


MIN_FORCE_MODE_THRESHOLD_N = 0.001
FIRST_HANGPRINTER_DRIVER_ADDRESS = 40


def _parse_driver_list(raw_value):
    descriptors = []
    for item in str(raw_value).split(':'):
        text = item.strip()
        if not text:
            continue
        parts = text.split('.', 1)
        can_address = int(parts[0])
        driver_index = 0
        if len(parts) > 1 and parts[1]:
            driver_index = int(parts[1])
        descriptors.append({
            'can_address': can_address,
            'driver': driver_index,
        })
    return descriptors


def _parse_float_list(raw_value):
    values = []
    for item in str(raw_value).split(':'):
        text = item.strip()
        if not text:
            continue
        values.append(float(text))
    return values


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

    def _get_driver_configs(self):
        toolhead = self.printer.lookup_object('toolhead', None)
        if toolhead is None:
            return {}
        kin = toolhead.get_kinematics()
        steppers = list(kin.get_steppers())
        motor_descriptors = list(getattr(kin, 'm569_driver_descriptors', []))
        flex_helper = getattr(kin, 'flex_helper', None)
        mechanical_advantage = list(getattr(flex_helper, 'mechanical_advantage', []))
        driver_configs = {}
        for index, stepper in enumerate(steppers):
            descriptor = motor_descriptors[index] if index < len(motor_descriptors) else {
                'can_address': FIRST_HANGPRINTER_DRIVER_ADDRESS + index,
                'driver': 0,
            }
            can_address = descriptor['can_address']
            rotation_distance_mm, _steps_per_rotation = stepper.get_rotation_distance()
            invert_dir, _orig_invert_dir = stepper.get_dir_inverted()
            mech_adv = 1
            if index < len(mechanical_advantage) and mechanical_advantage[index]:
                mech_adv = mechanical_advantage[index]
            driver_configs[can_address] = {
                'can_address': can_address,
                'driver': descriptor.get('driver', 0),
                'mechanical_advantage': mech_adv,
                'rotation_distance_mm': rotation_distance_mm,
                'invert_dir': bool(invert_dir),
            }
        return driver_configs

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

    def _set_placeholder_driver_state(self, driver_address, force_newtons,
                                      torque_nm, position_mode):
        self.driver_states[driver_address] = {
            'force_newtons': force_newtons,
            'torque_nm': 0. if position_mode else torque_nm,
            'position_mode': bool(position_mode),
        }

    def cmd_M569_3(self, gcmd):
        if gcmd.get('P', None) is None:
            gcmd.respond_raw("Error: M569: missing parameter 'P'")
            return
        # The terminal bridge overrides this placeholder reply with simulated
        # encoder angles when hp-sim is attached over gcode_ws.
        gcmd.respond_raw("Error: M569.3: Message not received")

    def cmd_M569_4(self, gcmd):
        if gcmd.get('P', None) is None:
            gcmd.respond_raw("Error: M569: missing parameter 'P'")
            return
        if gcmd.get('T', None) is None:
            gcmd.respond_raw("Error: M569.4 missing parameter 'T'")
            return
        try:
            drivers = _parse_driver_list(gcmd.get('P'))
            forces = _parse_float_list(gcmd.get('T'))
        except (TypeError, ValueError):
            gcmd.respond_raw("Error: M569.4 unable to parse parameters")
            return
        if not drivers:
            gcmd.respond_raw("Error: M569: missing parameter 'P'")
            return
        if len(forces) != len(drivers):
            gcmd.respond_raw("M569.4 requires one T value per P")
            return
        driver_configs = self._get_driver_configs()
        response_parts = []
        for descriptor, force_newtons in zip(drivers, forces):
            if descriptor['driver'] != 0:
                gcmd.respond_raw(
                    "Error: M569.4 only supports local driver index 0 placeholders")
                return
            driver_config = driver_configs.get(descriptor['can_address'])
            if driver_config is None:
                gcmd.respond_raw(
                    "Error: M569.4 driver not configured: %d.%d"
                    % (descriptor['can_address'], descriptor['driver']))
                return
            torque_nm, position_mode = self._compute_torque_from_force(
                driver_config, force_newtons)
            self._set_placeholder_driver_state(
                descriptor['can_address'], force_newtons, torque_nm, position_mode)
            if position_mode:
                response_parts.append("pos_mode, ")
            else:
                response_parts.append("%.6f Nm, " % (torque_nm,))
        gcmd.respond_raw("".join(response_parts))


def load_config(config):
    return HangprinterM569(config)
