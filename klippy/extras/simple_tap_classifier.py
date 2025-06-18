from . import load_cell_probe
from .load_cell_probe import TapValidationError


# Simple tap classifier rules:
# 1) The compression force must greater than the trigger force
# 2) The decompression force should be at least 2/3s of the trigger force
# 3) The baseline force, before and after the tap, should differ by
#    less than 20% of the compression force.
# 4) Check that any configured angles are inside the bounds set in the config
class SimpleTapClassifier(load_cell_probe.TapClassifierModule):
    ANGLE_NAMES = ["compression_start_angle_range",
                   "compression_end_angle_range",
                   "decompression_start_angle_range",
                   "decompression_end_angle_range"]

    def __init__(self, config):
        printer = config.get_printer()
        self._min_decomp_force_pct = 0.01 * config.getfloat(
            'min_decompression_force_percentage',
            minval=20., default=66.6, maxval=100.)
        self._max_baseline_change_pct = 0.01 * config.getfloat(
            'max_baseline_force_change_percentage',
            above=0., maxval=50., default=20.)
        self._angle_configs = []
        for angle_name in self.ANGLE_NAMES:
            angle_config = config.getfloatlist(angle_name, count=2,
                                               default=None)
            self._angle_configs.append(angle_config)
            if angle_config is None:
                continue
            if angle_config[0] >= angle_config[1]:
                raise printer.config_error(
                    "%s must be in min, max order" % (angle_name,))
            for angle in angle_config:
                if angle < 0 or angle > 180:
                    raise printer.config_error(
                        "%s must be in the range 0 to 180" % (angle_name,))

    def classify(self, tap_analysis):
        # This module cant rescue bad data
        if not tap_analysis.is_valid():
            return

        trigger_force = tap_analysis.get_trigger_force()
        raw_angles = tap_analysis.get_tap_angles()


        # compression line check
        tap_points = tap_analysis.get_tap_points()
        comp_start = tap_points[1]
        comp_end = tap_points[2]
        compression_force = abs(comp_end.force - comp_start.force)
        if compression_force < trigger_force:
            raise TapValidationError("LOW_COMPRESSION_FORCE",
                "Compression force was less than the trigger force")

        # decompression line check
        decomp_start = tap_points[3]
        decomp_end = tap_points[4]
        decompression_force = abs(decomp_start.force - decomp_end.force)
        min_decompression_force = trigger_force * self._min_decomp_force_pct
        if decompression_force < min_decompression_force:
            raise TapValidationError("LOW_DECOMPRESSION_FORCE",
                "Force dropped too much before the pullback move")

        # baseline force check
        baseline_force_delta = abs(comp_start.force - decomp_end.force)
        max_baseline_delta = compression_force * self._max_baseline_change_pct
        if baseline_force_delta > max_baseline_delta:
            raise TapValidationError("BASELINE_FORCE_INCONSISTENT",
                "The baseline force before and after the tap are inconsistent")

        # find the absolute supplementary angle so the measured angles are
        # the interior angles of the tap. These feel like the obvious angles to
        # label as a human, since they are between two lines actually depicted
        # in the graph:
        #       |---------\
        #       | ce    ds \
        #    cs |           \ de
        #  -----|            \------
        angles = [180. - abs(angle) for angle in raw_angles]
        # check all configured angles
        for i in range(len(self.ANGLE_NAMES)):
            angle_config = self._angle_configs[i]
            if angle_config is None:
                continue
            min_angle = angle_config[0]
            max_angle = angle_config[1]
            angle = angles[i]
            if not (min_angle <= angle <= max_angle):
                name = self.ANGLE_NAMES[i]
                raise TapValidationError(name.upper() + "_ERROR",
                    "%s constraint violated: %f < %f < %f"
                    % (name, min_angle, angle, max_angle))


def load_config(config):
    return SimpleTapClassifier(config)
