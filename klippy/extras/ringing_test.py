# A utility class to print the ringing (ghosting) test
#
# Copyright (C) 2022  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math

FIRST_LAYER_WIDTH_MULTIPLIER = 1.1
DEFAULT_SIZE = 100.0
TAN_TEST_ANGLE = 1.
DEFAULT_NOTCH_OFFSET_RATIO = .275
INFINITE_ACCEL = 1000000.0
LETTER_BAND_PART = 0.82

class RingingTest:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect", self._connect)
        self.size = config.getfloat('size', 0., minval=50.)
        self.height = config.getfloat('height', 60., above=0.)
        self.band = config.getfloat('band', 5., above=0.)
        self.notch = config.getfloat('notch', 1., above=0.)
        self.notch_offset = config.getfloat('notch_offset', 0.)
        self.velocity = config.getfloat('velocity', 80., above=0.)
        self.velocity_step = config.getfloat('velocity_step', 0.)
        self.accel_start = config.getfloat('accel_start', 1500., above=0.)
        self.accel_step = config.getfloat('accel_step', 500.)
        self.center_x = config.getfloat('center_x', None)
        self.center_y = config.getfloat('center_y', None)
        self.layer_height = config.getfloat('layer_height', 0.2, above=0.)
        self.first_layer_height = config.getfloat('first_layer_height', 0.2,
                                                  above=self.layer_height)
        self.perimeters = config.getint('perimeters', 2, minval=1)
        self.brim_width = config.getfloat('brim_width', 10.,
                                          minval=self.notch+2.)
        self.brim_velocity = config.getfloat('brim_velocity', 30., above=0.)
        self.filament_diameter = config.getfloat('filament_diameter', 1.75,
                                                 above=0.)
        self.deceleration_points = config.getint('deceleration_points', 100,
                                                 minval=10)
        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('PRINT_RINGING_TOWER',
                                    self.cmd_PRINT_RINGING_TOWER,
                                    desc=self.cmd_PRINT_RINGING_TOWER_help)
        self.progress = 0.
    def _connect(self):
        self.sdcard = self.printer.lookup_object('virtual_sdcard', None)
        if self.sdcard is None:
            raise self.printer.config_error(
                    'virtual_sdcard must be enabled for pa_test module to work')
        toolhead = self.printer.lookup_object('toolhead')
        kin_status = toolhead.get_kinematics().get_status(eventtime=None)
        self.center_x = self.center_x or .5 * (kin_status['axis_minimum'].x +
                                               kin_status['axis_maximum'].x)
        self.center_y = self.center_y or .5 * (kin_status['axis_minimum'].y +
                                               kin_status['axis_maximum'].y)
        if not self.size:
            # Restrict the size to a maximum of 80% of available space
            self.size = min(
                    DEFAULT_SIZE,
                    1.6 * (kin_status['axis_maximum'].x - self.center_x),
                    1.6 * (self.center_x - kin_status['axis_minimum'].x))
            if self.notch_offset:
                raise self.printer.config_error(
                        'notch_offset must not be set if size is not provided')
            self.notch_offset = self.size * DEFAULT_NOTCH_OFFSET_RATIO
        else:
            if not self.notch_offset:
                self.notch_offset = self.size * DEFAULT_NOTCH_OFFSET_RATIO
            if self.notch_offset < 2. or self.notch_offset > .5 * self.size:
                raise self.printer.config_error(
                        'notch_offset must not be in range [2.0, %.3f]' % (
                            .5 * self.size,))

    cmd_PRINT_RINGING_TOWER_help = "Start Ringing Tower print"
    def cmd_PRINT_RINGING_TOWER(self, gcmd):
        if self.is_active():
            raise gcmd.error('Ringing tower is already printing. If you want '
                             'to pause or cancel the print, you must enable '
                             '[pause_resume] module and call a corresponding '
                             'PAUSE/CANCEL_PRINT command.')
        self.gcmd = gcmd
        self.progress = 0.
        self.sdcard.print_with_gcode_provider(self)
    def handle_shutdown(self):
        # Nothing to do, no resources to free
        pass
    def get_stats(self, eventtime):
        if not self.is_active():
            return False, ''
        return True, 'Printing ringing tower'
    def get_status(self, eventtime):
        return {
            'file_path': self.get_name(),
            'progress': self.progress,
            'file_position': 0,
            'file_size': 0,
        }
    def is_active(self):
        return self.sdcard.get_gcode_provider() == self
    def get_name(self):
        return 'Ringing tower'
    def reset(self):
        self.progress = 0.
    def get_gcode(self):
        gcmd = self.gcmd
        nozzle = gcmd.get_float('NOZZLE')
        line_width = nozzle * 1.2

        # Get velocity and acceleration limits
        toolhead = self.printer.lookup_object('toolhead')
        systime = self.printer.get_reactor().monotonic()
        toolhead_info = toolhead.get_status(systime)
        old_max_accel = toolhead_info['max_accel']
        old_max_accel_to_decel = toolhead_info['max_accel_to_decel']
        old_max_velocity = toolhead_info['max_velocity']

        # Get tower params with overrides from the GCode command
        center_x = gcmd.get_float('CENTER_X', self.center_x)
        center_y = gcmd.get_float('CENTER_Y', self.center_y)
        size = gcmd.get_float('SIZE', self.size, minval=50.)
        perimeters = gcmd.get_int('PERIMETERS', self.perimeters, minval=1)
        brim_velocity = gcmd.get_float('BRIM_VELOCITY', self.brim_velocity,
                                       above=0.)
        filament_diameter = gcmd.get_float('FILAMENT_DIAMETER',
                                           self.filament_diameter, above=0.)
        layer_height = gcmd.get_float('LAYER_HEIGHT',
                                      self.layer_height, above=0.)
        first_layer_height = gcmd.get_float('FIRST_LAYER_HEIGHT',
                                            self.first_layer_height,
                                            above=layer_height)
        height = gcmd.get_float('HEIGHT', self.height, above=0.)
        band = gcmd.get_float('BAND', self.band, above=0.)
        notch = gcmd.get_float('NOTCH', self.notch, above=0.)
        notch_offset = gcmd.get_float(
                'NOTCH_OFFSET',
                self.notch_offset if size == self.size
                else size * DEFAULT_NOTCH_OFFSET_RATIO,
                above=2., maxval=.5*size)
        velocity_start = gcmd.get_float('VELOCITY', self.velocity, above=0.)
        velocity_step = gcmd.get_float('VELOCITY_STEP', self.velocity_step)
        accel_start = gcmd.get_float('ACCEL_START', self.accel_start, above=0.)
        accel_step = gcmd.get_float('ACCEL_STEP', self.accel_step)
        brim_width = gcmd.get_float('BRIM_WIDTH', self.brim_width)
        min_brim_width = (notch_offset - notch) / (1. + 1. / TAN_TEST_ANGLE)
        brim_width = max(brim_width,
                         min_brim_width + perimeters * line_width + 1.)
        final_gcode_id = gcmd.get('FINAL_GCODE_ID', None)
        deceleration_points = gcmd.get_int('DECELERATION_POINTS',
                                           self.deceleration_points, minval=10)

        logging.info("Starting ringing tower print of size %.3fx%.3fx%.3f mm"
                     " at (%.3f,%.3f)" % (size, size, height,
                                          center_x, center_y))
        inner_size = size - 2 * (line_width * perimeters + notch)
        inner_brim_size = inner_size - 2. * min_brim_width

        recipr_cos = math.sqrt(1. + TAN_TEST_ANGLE**2)

        def get_top_velocity():
            z = first_layer_height
            top_velocity = velocity_start
            while z < height - 0.00000001:
                band_part = math.fmod(z, band) / band
                notch_pos = notch_offset - notch / (1. - math.sqrt(0.75)) * (
                        1. - math.sqrt(1. - (band_part - 0.5)**2))
                max_accel = accel_start + accel_step * math.floor(z / band)
                if max_accel < .1:
                    msg = "All accelerations must be positive"
                    logging.warning(msg)
                    raise gcmd.error(msg)
                velocity = velocity_start + velocity_step * math.floor(z / band)
                if velocity < .1:
                    msg = "All velocities must be positive"
                    logging.warning(msg)
                    raise gcmd.error(msg)
                top_velocity = max(top_velocity, velocity)
                v_y = velocity * TAN_TEST_ANGLE
                t_y = v_y / max_accel
                d_x = velocity * t_y
                min_accel_dist_x = .5 * velocity**2 / max_accel * recipr_cos
                accel_dist_x = notch_pos - d_x - 1.0 - .5 * (size - inner_size)
                if accel_dist_x < min_accel_dist_x:
                    msg = ("Too high velocity %.2f mm/sec for %.0f mm/sec^2"
                           " acceleration" % (velocity, max_accel))
                    logging.warning(msg)
                    raise gcmd.error(msg)
                z += layer_height
            return top_velocity
        max_velocity = recipr_cos * get_top_velocity()
        if max_velocity > old_max_velocity:
            yield 'SET_VELOCITY_LIMIT VELOCITY=%.3f' % (max_velocity,)

        def gen_brim():
            first_layer_width = line_width * FIRST_LAYER_WIDTH_MULTIPLIER
            extr_r = 4. * first_layer_height * first_layer_width / (
                    math.pi * filament_diameter**2)
            brim_offset = .5 * inner_brim_size + brim_width
            start_x = center_x - brim_offset
            start_y = center_y - brim_offset
            start_z = first_layer_height
            yield 'SET_VELOCITY_LIMIT ACCEL=%.6f ACCEL_TO_DECEL=%.6f' % (
                    accel_start, .5 * accel_start)
            yield 'G1 X%.3f Y%.3f Z%.3f F%.f' % (
                    start_x, start_y, start_z,
                    max_velocity * 60.)
            while brim_offset > .5 * inner_brim_size:
                yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                        center_x - brim_offset, center_y + brim_offset,
                        2. * brim_offset * extr_r, brim_velocity * 60.)
                yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                        center_x + brim_offset, center_y + brim_offset,
                        2. * brim_offset * extr_r, brim_velocity * 60.)
                yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                        center_x + brim_offset, center_y - brim_offset,
                        2. * brim_offset * extr_r, brim_velocity * 60.)
                new_brim_offset = brim_offset - first_layer_width
                yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                        center_x - new_brim_offset,
                        center_y - brim_offset,
                        (brim_offset + new_brim_offset) * extr_r,
                        brim_velocity * 60.)
                brim_offset = new_brim_offset
            self.progress = start_z / height
        def gen_tower():
            prev_z = first_layer_height
            z = first_layer_height + layer_height
            extr_r = 4. * layer_height * line_width / (
                    math.pi * filament_diameter**2)
            letter_offset = .5 * (size - band) - notch_offset * TAN_TEST_ANGLE
            while z < height - 0.00000001:
                next_z = z + layer_height
                band_part = math.fmod(z, band) / band
                notch_pos = notch_offset - notch / (1. - math.sqrt(0.75)) * (
                        1. - math.sqrt(1. - (band_part - 0.5)**2))
                max_accel = accel_start + accel_step * math.floor(z / band)
                velocity = velocity_start + velocity_step * math.floor(z / band)
                v_y = velocity * TAN_TEST_ANGLE
                t_y = v_y / max_accel
                d_y = .5 * v_y * t_y
                d_x = velocity * t_y
                notch_other_side = (notch_pos - d_x) * TAN_TEST_ANGLE + d_y
                perimeter_offset = .5 * inner_size
                for i in range(perimeters):
                    # Move to the start of the perimeter
                    next_y_offset = (
                            perimeter_offset - notch_other_side
                            - (perimeter_offset - .5 * size) * TAN_TEST_ANGLE)
                    yield 'G1 X%.6f Y%.6f F%.3f' % (
                            center_x - perimeter_offset,
                            center_y - next_y_offset,
                            velocity * 60.)
                    yield 'G1 Z%.6f' % (z,)
                    # Print the perimiter loop
                    for notch_axis in [(1, 0), (0, 1), (-1, 0), (0, -1)]:
                        other_axis = (notch_axis[1], -notch_axis[0])
                        def rotated_G1(x, y, e, v):
                            return 'G1 X%.6f Y%.6f E%.9f F%.3f' % (
                                    center_x + notch_axis[0] * x
                                             + other_axis[0] * y,
                                    center_y + notch_axis[1] * x
                                             + other_axis[1] * y,
                                    e * extr_r, v * 60.)
                        yield ('SET_VELOCITY_LIMIT ACCEL=%.3f '
                               'ACCEL_TO_DECEL=%.3f' % (max_accel, max_accel))
                        # The extrusion flow of the lines at an agle is reduced
                        # by cos(angle) to maintain the correct spacing between
                        # the perimeters formed by those lines
                        yield rotated_G1(
                                notch_pos - d_x - 1.0 - .5 * size,
                                perimeter_offset - d_y - TAN_TEST_ANGLE,
                                (notch_pos - d_x - 1.0 - .5 * size
                                    + perimeter_offset),
                                recipr_cos * velocity)
                        yield ('SET_VELOCITY_LIMIT ACCEL=%.6f'
                                + ' ACCEL_TO_DECEL=%.6f') % (
                                        INFINITE_ACCEL, INFINITE_ACCEL)
                        yield rotated_G1(
                                notch_pos - d_x - .5 * size,
                                perimeter_offset - d_y,
                                1., recipr_cos * velocity)
                        old_x, old_y = d_x, d_y
                        for j in range(deceleration_points):
                            x = ((deceleration_points - j - 1) * d_x
                                    / deceleration_points)
                            t = (d_x - x) / velocity
                            y = d_y - t * (v_y - .5 * max_accel * t)
                            d = math.sqrt((x - old_x)**2 + (y - old_y)**2)
                            v = velocity * d / (old_x - x)
                            yield rotated_G1(
                                    notch_pos - x - .5 * size,
                                    perimeter_offset - y,
                                    (old_x - x), v)
                            old_x, old_y = x, y
                        yield ('SET_VELOCITY_LIMIT ACCEL=%.6f'
                                + ' ACCEL_TO_DECEL=%.6f') % (
                                        max_accel, .5 * max_accel)
                        if i < perimeters - 1 or (abs(band_part - .5) >=
                                                  .5 * LETTER_BAND_PART):
                            yield rotated_G1(
                                    next_y_offset, perimeter_offset,
                                    next_y_offset - notch_pos + .5 * size,
                                    velocity)
                            continue
                        # Emboss a letter
                        print_serif = (
                                max(math.fmod(next_z, band) / band - .5,
                                    .5 - math.fmod(prev_z, band) / band)
                                    >= .5 * LETTER_BAND_PART
                                and abs(band_part - .5) < .5 * LETTER_BAND_PART)
                        letter_width = abs(band_part - .5) * band + nozzle
                        if notch_axis[0] and band_part < .5:
                            # Bottom of Y letter
                            letter_width = nozzle
                        elem_size = nozzle
                        if print_serif:
                            letter_width += nozzle
                            elem_size += nozzle
                        letter_perimeter_offset = -.5 * nozzle
                        yield rotated_G1(
                                letter_offset - letter_width * .5,
                                perimeter_offset,
                                (letter_offset - letter_width * .5
                                    - notch_pos + .5 * size),
                                velocity)
                        yield rotated_G1(
                                letter_offset - letter_width * .5,
                                perimeter_offset - letter_perimeter_offset,
                                abs(letter_perimeter_offset), velocity)
                        if letter_width > 2. * elem_size:
                            yield rotated_G1(
                                    (letter_offset - letter_width * .5
                                        + elem_size),
                                    perimeter_offset - letter_perimeter_offset,
                                    elem_size, velocity)
                            yield rotated_G1(
                                    (letter_offset - letter_width * .5
                                        + elem_size),
                                    perimeter_offset,
                                    abs(letter_perimeter_offset), velocity)
                            yield rotated_G1(
                                    (letter_offset + letter_width * .5
                                        - elem_size),
                                    perimeter_offset,
                                    letter_width - 2. * elem_size, velocity)
                            yield rotated_G1(
                                    (letter_offset + letter_width * .5
                                        - elem_size),
                                    perimeter_offset - letter_perimeter_offset,
                                    abs(letter_perimeter_offset), velocity)
                            yield rotated_G1(
                                    letter_offset + letter_width * .5,
                                    perimeter_offset - letter_perimeter_offset,
                                    elem_size, velocity)
                        else:
                            yield rotated_G1(
                                    letter_offset + letter_width * .5,
                                    perimeter_offset - letter_perimeter_offset,
                                    letter_width, velocity)
                        yield rotated_G1(
                                letter_offset + letter_width * .5,
                                perimeter_offset,
                                abs(letter_perimeter_offset), velocity)
                        yield rotated_G1(
                                next_y_offset,
                                perimeter_offset,
                                (next_y_offset - letter_offset
                                    - letter_width * .5),
                                velocity)
                    perimeter_offset += line_width
                self.progress = z / height
                prev_z, z = z, next_z
            yield ('SET_VELOCITY_LIMIT ACCEL=%.3f ACCEL_TO_DECEL=%.f'
                    + ' VELOCITY=%.3f') % (
                            old_max_accel, old_max_accel_to_decel,
                            old_max_velocity)

        yield 'M83'
        yield 'G90'
        yield 'M220 S100'
        for line in gen_brim():
            yield line
        for line in gen_tower():
            yield line
        if final_gcode_id is not None:
            yield "UPDATE_DELAYED_GCODE ID='%s' DURATION=0.01" % (
                    final_gcode_id,)
        self.progress = 1.0

def load_config(config):
    return RingingTest(config)
