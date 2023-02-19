# A utility class to run the Pressure Advance test
#
# Copyright (C) 2022  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math

FIRST_LAYER_WIDTH_MULTIPLIER = 1.2
DEFAULT_X_SIZE = 50.0
DEFAULT_Y_SIZE = 40.0
MAX_YX_SIZE_RATIO = 0.8
SLOW_NOTCH_SIZE = 10.0
SEAM_GAP_RATIO = 0.10
SEAM_EXTRA_WIPE_RATIO = 1.1
VERY_SLOW_SEG = 0.01

class PATest:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect", self._connect)
        self.size_x = config.getfloat('size_x', 0., minval=0.)
        self.size_y = config.getfloat('size_y', 0., minval=0.)
        if self.size_x or self.size_y:
            if self.size_x < SLOW_NOTCH_SIZE * 4:
                raise config.error(
                        'size_x must be at least %.1f' % (SLOW_NOTCH_SIZE * 4,))
            if self.size_y < SLOW_NOTCH_SIZE * 3:
                raise config.error(
                        'size_y must be at least %.1f' % (SLOW_NOTCH_SIZE * 3,))
            max_size_y = MAX_YX_SIZE_RATIO * self.size_x
            if self.size_y > max_size_y:
                raise config.error('Too large size_y value, maximum is %.3f' % (
                    max_size_y,))
        self.height = config.getfloat('height', 50., above=0.)
        self.origin_x = config.getfloat('origin_x', None)
        self.origin_y = config.getfloat('origin_y', None)
        self.layer_height = config.getfloat('layer_height', 0.2, above=0.)
        self.first_layer_height = config.getfloat('first_layer_height', 0.2,
                                                  above=self.layer_height)
        self.perimeters = config.getint('perimeters', 2, minval=1)
        self.brim_width = config.getfloat('brim_width', 10., minval=2.)
        self.slow_velocity = config.getfloat('slow_velocity', 25., above=0.)
        self.medium_velocity = config.getfloat('medium_velocity', 50.,
                                               above=self.slow_velocity)
        self.fast_velocity = config.getfloat('fast_velocity', 80.,
                                             above=self.medium_velocity)
        self.filament_diameter = config.getfloat('filament_diameter', 1.75,
                                                 above=0.)
        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('PRINT_PA_TOWER',
                                    self.cmd_PRINT_PA_TOWER,
                                    desc=self.cmd_PRINT_PA_TOWER_help)
        self.progress = 0.
    def _connect(self):
        self.sdcard = self.printer.lookup_object('virtual_sdcard', None)
        if self.sdcard is None:
            raise self.printer.config_error(
                    'virtual_sdcard must be enabled for pa_test module to work')
        toolhead = self.printer.lookup_object('toolhead')
        kin_status = toolhead.get_kinematics().get_status(eventtime=None)
        self.origin_x = self.origin_x or .5 * (kin_status['axis_minimum'].x +
                                               kin_status['axis_maximum'].x)
        self.origin_y = self.origin_y or .5 * (kin_status['axis_minimum'].y +
                                               kin_status['axis_maximum'].y)
        if not self.size_x and not self.size_y:
            # Restrict the size to a maximum of 80% of available space
            self.size_x = min(
                    DEFAULT_X_SIZE,
                    1.6 * (kin_status['axis_maximum'].x - self.origin_x),
                    1.6 * (self.origin_x - kin_status['axis_minimum'].x))
            self.size_y = min(
                    DEFAULT_Y_SIZE,
                    1.6 * (kin_status['axis_maximum'].y - self.origin_y),
                    1.6 * (self.origin_x - kin_status['axis_minimum'].y))
    cmd_PRINT_PA_TOWER_help = "Start Pressure Advance Tower print"
    def cmd_PRINT_PA_TOWER(self, gcmd):
        if self.is_active():
            raise gcmd.error('PA tower is already printing. If you want to '
                             'pause or cancel the print, you must enable '
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
        return True, 'Printing PA tower'
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
        return 'PA tower'
    def reset(self):
        self.progress = 0.
    def get_gcode(self):
        gcmd = self.gcmd
        nozzle = gcmd.get_float('NOZZLE')

        # Check extruder temperature
        target_temp = gcmd.get_float('TARGET_TEMP')
        toolhead = self.printer.lookup_object('toolhead')
        extruder_heater = toolhead.get_extruder().get_heater()
        systime = self.printer.get_reactor().monotonic()
        heater_status = extruder_heater.get_status(systime)
        if target_temp != heater_status['target']:
            raise gcmd.error(
                    'Extruder target temp %.1f does not match the expected '
                    'TARGET_TEMP=%.1f. Make sure to preheat the nozzle to '
                    'the expected temperature (e.g. with M109 gcode in a '
                    'gcode_macro prior to calling PRINT_PA_TOWER)' % (
                        heater_status['target'], target_temp))
        toolhead_status = toolhead.get_status(systime)

        # Get tower params with overrides from the GCode command
        origin_x = gcmd.get_float('ORIGIN_X', self.origin_x)
        origin_y = gcmd.get_float('ORIGIN_Y', self.origin_y)
        size_x = gcmd.get_float('SIZE_X', self.size_x, minval=SLOW_NOTCH_SIZE*4)
        size_y = gcmd.get_float('SIZE_Y', self.size_y, minval=SLOW_NOTCH_SIZE*3,
                                maxval=MAX_YX_SIZE_RATIO*size_x)
        perimeters = gcmd.get_int('PERIMETERS', self.perimeters, minval=1)
        slow_velocity = gcmd.get_float('SLOW_VELOCITY', self.slow_velocity,
                                       above=0.)
        medium_velocity = gcmd.get_float('MEDIUM_VELOCITY',
                                         self.medium_velocity,
                                         above=slow_velocity)
        fast_velocity = gcmd.get_float('FAST_VELOCITY', self.fast_velocity,
                                       above=medium_velocity)
        scv_velocity = gcmd.get_float('SCV_VELOCITY',
                                      toolhead_status['square_corner_velocity'],
                                      above=0.)
        filament_diameter = gcmd.get_float('FILAMENT_DIAMETER',
                                           self.filament_diameter, above=0.)
        layer_height = gcmd.get_float('LAYER_HEIGHT',
                                      self.layer_height, above=0.)
        first_layer_height = gcmd.get_float('FIRST_LAYER_HEIGHT',
                                            self.first_layer_height,
                                            above=layer_height)
        height = gcmd.get_float('HEIGHT', self.height, above=0.)
        step_height = gcmd.get_float('STEP_HEIGHT', 0., minval=0.)
        brim_width = gcmd.get_float('BRIM_WIDTH', self.brim_width, above=nozzle)
        final_gcode_id = gcmd.get('FINAL_GCODE_ID', None)

        logging.info("Starting PA tower print of size %.3f x %.3f x %.3f mm"
                     " at (%.3f,%.3f)" % (size_x, size_y, height,
                                          origin_x, origin_y))
        inner_size_x = size_x - 2 * nozzle * perimeters
        inner_size_y = size_y - 2 * nozzle * perimeters

        def gen_brim():
            first_layer_width = nozzle * FIRST_LAYER_WIDTH_MULTIPLIER
            extr_r = 4. * first_layer_height * first_layer_width / (
                    math.pi * filament_diameter**2)
            brim_x_offset = .5 * inner_size_x + brim_width
            brim_y_offset = .5 * inner_size_y + brim_width
            start_x = origin_x - brim_x_offset
            start_y = origin_y - brim_y_offset
            start_z = first_layer_height
            yield 'G1 X%.3f Y%.3f Z%.3f F%.f' % (
                    start_x, start_y, start_z, fast_velocity * 60.)
            while brim_x_offset > .5 * (inner_size_x - first_layer_width):
                yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                        origin_x - brim_x_offset, origin_y + brim_y_offset,
                        2. * brim_y_offset * extr_r, slow_velocity * 60.)
                yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                        origin_x + brim_x_offset, origin_y + brim_y_offset,
                        2. * brim_x_offset * extr_r, slow_velocity * 60.)
                yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                        origin_x + brim_x_offset, origin_y - brim_y_offset,
                        2. * brim_y_offset * extr_r, slow_velocity * 60.)
                new_brim_x_offset = brim_x_offset - first_layer_width
                new_brim_y_offset = brim_y_offset - first_layer_width
                yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                        origin_x - new_brim_x_offset,
                        origin_y - brim_y_offset,
                        (brim_x_offset + new_brim_x_offset) * extr_r,
                        slow_velocity * 60.)
                brim_x_offset = new_brim_x_offset
                brim_y_offset = new_brim_y_offset
            yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                    origin_x - brim_x_offset, origin_y + brim_y_offset,
                    2. * brim_y_offset * extr_r, slow_velocity * 60.)
            yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                    origin_x, origin_y + brim_y_offset,
                    brim_x_offset * extr_r, slow_velocity * 60.)
            self.progress = start_z / height
        def gen_tower():
            last_z = first_layer_height
            z = first_layer_height + layer_height
            x_switching_pos = size_x / 3.
            while z < height - 0.00000001:
                line_width = nozzle
                perimeter_x_offset = .5 * inner_size_x
                perimeter_y_offset = .5 * inner_size_y
                if (step_height and math.floor(z / step_height) >
                                        math.floor(last_z / step_height)):
                    # Generate a bit thicker wall for better visual separation
                    # of bands with different values of Pressure Advance params
                    line_width += 0.6 * nozzle / perimeters
                    extra_offs = .5 * (line_width - nozzle) * (perimeters - 1)
                    perimeter_x_offset -= extra_offs
                    perimeter_y_offset -= extra_offs
                extr_r = 4. * layer_height * line_width / (
                        math.pi * filament_diameter**2)
                # Move to the start of the perimeter
                yield 'G1 X%.3f Y%.3f F%.f' % (
                        origin_x, origin_y + perimeter_y_offset,
                        fast_velocity * 60.)
                ## Move the nozzle up
                yield 'G1 Z%.6f F%.f' % (z, slow_velocity * 60.)
                for i in range(perimeters):
                    # Print the perimiter loop alternating velocities
                    yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                            origin_x - x_switching_pos,
                            origin_y + perimeter_y_offset,
                            x_switching_pos * extr_r,
                            fast_velocity * 60.)
                    yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                            origin_x - perimeter_x_offset,
                            origin_y + perimeter_y_offset,
                            (perimeter_x_offset - x_switching_pos) * extr_r,
                            medium_velocity * 60.)
                    yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                            origin_x - perimeter_x_offset,
                            origin_y + .5 * SLOW_NOTCH_SIZE,
                            (perimeter_y_offset
                                - .5 * SLOW_NOTCH_SIZE) * extr_r,
                            fast_velocity * 60.)
                    yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                            origin_x - perimeter_x_offset,
                            origin_y - .5 * SLOW_NOTCH_SIZE,
                            SLOW_NOTCH_SIZE * extr_r,
                            slow_velocity * 60.)
                    yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                            origin_x - perimeter_x_offset,
                            origin_y - perimeter_y_offset,
                            (perimeter_y_offset
                                - .5 * SLOW_NOTCH_SIZE) * extr_r,
                            fast_velocity * 60.)
                    yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                            origin_x - x_switching_pos,
                            origin_y - perimeter_y_offset,
                            (perimeter_x_offset - x_switching_pos) * extr_r,
                            fast_velocity * 60.)
                    yield 'G4 P0.001'
                    yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                            origin_x - .5 * SLOW_NOTCH_SIZE,
                            origin_y - perimeter_y_offset,
                            (x_switching_pos - .5 * SLOW_NOTCH_SIZE) * extr_r,
                            fast_velocity * 60.)
                    yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                            origin_x + .5 * SLOW_NOTCH_SIZE,
                            origin_y - perimeter_y_offset,
                            SLOW_NOTCH_SIZE * extr_r,
                            slow_velocity * 60.)
                    yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                            origin_x + x_switching_pos,
                            origin_y - perimeter_y_offset,
                            (x_switching_pos - .5 * SLOW_NOTCH_SIZE) * extr_r,
                            fast_velocity * 60.)
                    yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                            origin_x + x_switching_pos + VERY_SLOW_SEG,
                            origin_y - perimeter_y_offset,
                            VERY_SLOW_SEG * extr_r,
                            scv_velocity * 60.)
                    yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                            origin_x + perimeter_x_offset,
                            origin_y - perimeter_y_offset,
                            (perimeter_x_offset - x_switching_pos
                                - VERY_SLOW_SEG) * extr_r,
                            fast_velocity * 60.)
                    yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                            origin_x + perimeter_x_offset,
                            origin_y - .5 * SLOW_NOTCH_SIZE,
                            (perimeter_y_offset
                                - .5 * SLOW_NOTCH_SIZE) * extr_r,
                            fast_velocity * 60.)
                    yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                            origin_x + perimeter_x_offset,
                            origin_y + .5 * SLOW_NOTCH_SIZE,
                            SLOW_NOTCH_SIZE * extr_r,
                            medium_velocity * 60.)
                    yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                            origin_x + perimeter_x_offset,
                            origin_y + perimeter_y_offset,
                            (perimeter_y_offset
                                - .5 * SLOW_NOTCH_SIZE) * extr_r,
                            fast_velocity * 60.)
                    yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                            origin_x + x_switching_pos,
                            origin_y + perimeter_y_offset,
                            (perimeter_x_offset - x_switching_pos) * extr_r,
                            medium_velocity * 60.)
                    yield 'G1 X%.3f Y%.3f E%.6f F%.f' % (
                            origin_x + nozzle * SEAM_GAP_RATIO,
                            origin_y + perimeter_y_offset,
                            (x_switching_pos
                                - nozzle * SEAM_GAP_RATIO) * extr_r,
                            fast_velocity * 60.)
                    if i < perimeters - 1:
                        # Switch to the next perimeter
                        perimeter_x_offset += line_width
                        perimeter_y_offset += line_width
                        yield 'G1 X%.3f Y%.3f F%.f' % (
                                origin_x, origin_y + perimeter_y_offset,
                                fast_velocity * 60.)
                    else:
                        # Hide the seam a bit
                        yield 'G1 X%.3f Y%.3f F%.f' % (
                                origin_x - nozzle * SEAM_EXTRA_WIPE_RATIO,
                                origin_y + perimeter_y_offset,
                                fast_velocity * 60.)
                self.progress = z / height
                last_z = z
                z += layer_height

        yield 'M83'
        yield 'G90'
        for line in gen_brim():
            yield line
        for line in gen_tower():
            yield line
        if final_gcode_id is not None:
            yield "UPDATE_DELAYED_GCODE ID='%s' DURATION=0.01" % (
                    final_gcode_id,)
        self.progress = 1.0

def load_config(config):
    return PATest(config)
