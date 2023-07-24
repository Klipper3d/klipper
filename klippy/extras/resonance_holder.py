#You have to have [resonance_holder] in your printer.cfg.
#The command is HOLD_RESONANCE AXIS=<axis> FREQ=int SECONDS=<seconds>
#Place the following in klippy/extras/resonance_holder.py and restart klipper (not firmware restart) to gain the module.

import logging, math, os, time

class TestAxis:
    def __init__(self, axis=None, vib_dir=None):
        if axis is None:
            self._name = "axis=%.3f,%.3f" % (vib_dir[0], vib_dir[1])
        else:
            self._name = axis
        if vib_dir is None:
            self._vib_dir = (1., 0.) if axis == 'x' else (0., 1.)
        else:
            s = math.sqrt(sum([d*d for d in vib_dir]))
            self._vib_dir = [d / s for d in vib_dir]
    def matches(self, chip_axis):
        if self._vib_dir[0] and 'x' in chip_axis:
            return True
        if self._vib_dir[1] and 'y' in chip_axis:
            return True
        return False
    def get_name(self):
        return self._name
    def get_point(self, l):
        return (self._vib_dir[0] * l, self._vib_dir[1] * l)

def _parse_axis(gcmd, raw_axis):
    if raw_axis is None:
        return None
    raw_axis = raw_axis.lower()
    if raw_axis in ['x', 'y']:
        return TestAxis(axis=raw_axis)
    dirs = raw_axis.split(',')
    if len(dirs) != 2:
        raise gcmd.error("Invalid format of axis '%s'" % (raw_axis,))
    try:
        dir_x = float(dirs[0].strip())
        dir_y = float(dirs[1].strip())
    except:
        raise gcmd.error(
                "Unable to parse axis direction '%s'" % (raw_axis,))
    return TestAxis(vib_dir=(dir_x, dir_y))


class ResonanceHolder:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.accel_per_hz = config.getfloat('accel_per_hz', 75., above=0.)
        self.gcode.register_command("HOLD_RESONANCE", self.cmd_HOLD_RESONANCE, desc=self.cmd_HOLD_RESONANCE_help)

    def hold(self, gcmd, axis, seconds, freq):
        '''holds a resonance for N seconds
           resonance code taken from klipper's test_resonances command'''
        end = time.time() + seconds
        toolhead = self.printer.lookup_object('toolhead')
        X, Y, Z, E = toolhead.get_position()
        sign = 1.
        input_shaper = self.printer.lookup_object('input_shaper', None)
        if input_shaper is not None and not gcmd.get_int('INPUT_SHAPING', 0):
            input_shaper.disable_shaping()
            gcmd.respond_info("Disabled [input_shaper] for resonance holding")
        else:
            input_shaper = None
        gcmd.respond_info("starting freq %i for %i seconds" % (freq, seconds))
        while time.time() < end:
            t_seg = .25 / freq
            accel = self.accel_per_hz * freq
            max_v = accel * t_seg
            toolhead.cmd_M204(self.gcode.create_gcode_command(
                "M204", "M204", {"S": accel}))
            L = .5 * accel * t_seg**2
            dX, dY = axis.get_point(L)
            nX = X + sign * dX
            nY = Y + sign * dY
            toolhead.move([nX, nY, Z, E], max_v)
            toolhead.move([X, Y, Z, E], max_v)
            sign = -sign
        gcmd.respond_info("DONE")


    cmd_HOLD_RESONANCE_help = ("Holds resonance for n seconds")
    def cmd_HOLD_RESONANCE(self, gcmd):
        axis = _parse_axis(gcmd, gcmd.get("AXIS").lower())
        freq = gcmd.get("FREQ", parser=int)
        seconds = gcmd.get("SECONDS", parser=int)
        self.hold(gcmd, axis, seconds, freq)

def load_config(config):
    return ResonanceHolder(config)
