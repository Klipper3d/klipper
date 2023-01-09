from Geometry3D import *
from . import probe
import asyncio

class AutoZPositions:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name()
        self.config = config

        self.z_steppers = []

        # using ProbePointsHelper to validate points config
        helper = probe.ProbePointsHelper(config, lambda *args: None)
        helper.minimum_points(3)

        probe_config = config.getsection('probe')
        speed = probe_config.getfloat('speed', 5.0, above=0.)
        self.lift_speed = probe_config.getfloat('lift_speed', speed, above=0.)
        self.z_delta = config.getfloat('z_delta', 3.0, above=0.)

        self.printer.register_event_handler("klippy:connect", self.handle_connect)

        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('AUTO_Z_POSITIONS',
                                    self.cmd_AUTO_Z_POSITIONS,
                                    desc=self.cmd_AUTO_Z_POSITIONS_help)
        self.gcode.register_command('PROBE_PLANE', self.cmd_PROBE_PLANE,
                                    desc=self.cmd_PROBE_PLANE_help)

    def handle_connect(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        z_steppers = [s for s in kin.get_steppers() if s.is_active_axis('z')]
        if len(z_steppers) != 3:
            raise self.printer.config_error("%s needs exactly 3 z steppers" %
                                            self.name)
        self.z_steppers = z_steppers

    cmd_AUTO_Z_POSITIONS_help = "Probe the bed to determine pivot points"
    def cmd_AUTO_Z_POSITIONS(self, gcmd):
        z_positions = asyncio.run(self.auto_z_positions(gcmd))
        gcmd.respond_info("z positions: %s" % z_positions)

        # TODO(ibash) store results for auto save, see pid_calibrate

    cmd_PROBE_PLANE_help = "Probe a single plane for debugging purposes"
    def cmd_PROBE_PLANE(self, gcmd):
        plane = asyncio.run(self.probe_plane(gcmd))
        gcmd.respond_info("probed plane: %s" % plane)

    async def auto_z_positions(self, gcmd):
        # algorithm
        # 1. probe a base plane (plane 1)
        # 2. for each z stepper:
        #   1. move the z stepper
        #   2. probe a plane (planes 2-4)
        #   3. move the z stepper back
        # 3. planes 2-4 intersected with the base plane (plane 1) produce a line
        # 4. each line intersected with the other produce a pivot point
        base = await self.probe_plane(gcmd)
        # distance = 3
        planes = [base]
        for stepper in self.z_steppers:
            self.move_z_stepper(stepper, self.z_delta)
            plane = await self.probe_plane(gcmd)
            planes.append(plane)
            self.move_z_stepper(stepper, -1 * self.z_delta)

        z_positions = self.compute_z_positions(planes)
        return z_positions

    async def probe_plane(self, gcmd):
        result = await self.probe_async(gcmd)
        points = result["points"]
        plane = Plane(points[0], points[1], points[2])
        return plane

    def probe_async(self, gcmd):
        future = asyncio.Future()

        def finalize(offsets, positions):
            points = [Point(x, y, z) for (x, y, z) in positions]
            future.set_result({
                "offsets": offsets,
                "points": points,
                "positions": positions,
            })

        helper = probe.ProbePointsHelper(self.config, finalize)
        helper.minimum_points(3)
        helper.start_probe(gcmd)

        return future

    def move_z_stepper(self, stepper, distance):
        stepper_name = stepper.get_name()
        self.gcode.run_script_from_command(
            "FORCE_MOVE STEPPER=%s DISTANCE=%.3f VELOCITY=%.3f" %
            (stepper_name, distance, self.lift_speed))

    def compute_z_positions(self, planes):
        # for a given stepper the location of a pivot point is the
        # intersection of the 3 planes where the stepper remained in a fixed
        # position, intuitively the planes move around the pivot point of a
        # stepper
        # e.g. given 3 steppers [stepper_z, stepper_z1, stepper_z2]
        # then pivot of stepper_z = intersection of planes 0, 2, 3
        # stepper_z1 = intersection of planes 0, 1, 3
        # stepper_z2 = intersection of planes 0, 1, 2

        # TODO(ibash) add probe offsets to final points in order to get the
        # correct coordinates, since these points are all "off" by the probe
        # amount (when setting probing location, it's in nozzle coordinates, not
        # probe coordinates...)
        points = [
            planes[0].intersection(planes[2]).intersection(planes[3]),
            planes[0].intersection(planes[1]).intersection(planes[3]),
            planes[0].intersection(planes[1]).intersection(planes[2]),
        ]

        return points

def load_config(config):
    return AutoZPositions(config)
