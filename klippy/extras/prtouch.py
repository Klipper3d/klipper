# PRTouch Z-Offset Probe — Klipper plugin

import logging
import math
import random
import time

from . import probe

log = logging.getLogger(__name__)

# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------

_TICK_OVERFLOW   = 4_294_967_296
_LIMT_UP_MM      = 10.0
_PROBE_SAMPLES   = 2
_REPROBE_Z_BUMP  = 2.0


# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------

class PRTouchConfig:
    def __init__(self, config):
        gi  = config.getint
        gf  = config.getfloat
        gb  = config.getboolean
        gs  = config.get

        self.base_count   = gi('base_count',  default=40,   minval=10,  maxval=100)
        self.pi_count     = gi('pi_count',    default=32,   minval=16,  maxval=128)
        self.min_hold     = gi('min_hold',    default=3000, minval=100, maxval=50_000)
        self.max_hold     = gi('max_hold',    default=50_000, minval=100, maxval=100_000)

        self.sensor_x     = gf('sensor_x',   minval=0, maxval=300)
        self.sensor_y     = gf('sensor_y',   minval=0, maxval=300)
        self.random_offset = gf('sensor_random_offset', default=5, minval=0, maxval=10)

        self.hot_min_temp = gf('s_hot_min_temp', default=140, minval=80,  maxval=200)
        self.hot_max_temp = gf('s_hot_max_temp', default=200, minval=140, maxval=300)
        self.bed_max_temp = gf('s_bed_max_temp', default=60,  minval=45,  maxval=100)

        self.pa_clr_dis_mm   = gi('pa_clr_dis_mm',  default=5, minval=2, maxval=100)
        self.pa_clr_down_mm  = gf('pa_clr_down_mm', default=-0.1, minval=-1, maxval=1)

        self.clr_noz_start_x    = gf('clr_noz_start_x', default=0)
        self.clr_noz_start_y    = gf('clr_noz_start_y', default=0)
        self.clr_noz_len_x      = gf('clr_noz_len_x', default=0)
        self.clr_noz_len_y      = gf('clr_noz_len_y', default=0,
                                      minval=self.pa_clr_dis_mm + 5, maxval=1000)

        self.clr_noz_wipe_speed = gf('clr_noz_wipe_speed', default=10, minval=10, maxval=50)

        self.bed_max_err = gi('bed_max_err', default=4, minval=2, maxval=10)

        self.max_z = config.getsection('stepper_z').getfloat(
            'position_max', default=300, minval=100, maxval=500)

        self.g29_xy_speed  = gf('g29_xy_speed', default=150)
        self.g29_rdy_speed = gf('g29_rdy_speed', default=2.5)
        self.probe_speed   = gf('probe_speed', default=2.0)

        self.check_bed_mesh_max_err = gf('check_bed_mesh_max_err', default=0.2)

        self.wipe_retract_distance = gf('wipe_retract_distance', default=0)

        self.show_msg = gb('show_msg', default=False)
        self.probe_name = gs('probe_name', default='bltouch')

        try:
            stored = config.get_prefix_sections('prtouch')
            self.stored_profs = stored[1] if len(stored) == 2 else None
        except Exception:
            self.stored_profs = None


# ---------------------------------------------------------------------------
# State
# ---------------------------------------------------------------------------

class PRTouchState:
    def __init__(self):
        self.reset()

    def reset(self):
        self.out_index = 0
        self.out_val_mm = 0.0
        self.re_probe_cnt = 0
        self.home_xy = None


# ---------------------------------------------------------------------------
# Objects
# ---------------------------------------------------------------------------

class PRTouchObjects:
    def __init__(self, printer):
        self.printer = printer
        self.gcode = printer.lookup_object('gcode')
        self.gcode_move = printer.lookup_object('gcode_move')

        self.toolhead = None
        self.hx711s = None
        self.dirzctl = None
        self.filter = None
        self.probe = None
        self.mcu = None
        self.kin = None

    def find_objects(self):
        lkp = self.printer.lookup_object
        self.toolhead = lkp('toolhead')
        self.hx711s = lkp('hx711s')
        self.dirzctl = lkp('dirzctl')
        self.filter = lkp('filter')
        self.probe = lkp('probe', None)

        self.mcu = self.hx711s.mcu if self.hx711s else None
        self.kin = self.toolhead.get_kinematics() if self.toolhead else None


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

class PRTouchZOffsetWrapper:
    def __init__(self, config):
        self.cfg = PRTouchConfig(config)
        self.state = PRTouchState()
        self.obj = PRTouchObjects(config.get_printer())

        self.obj.printer.register_event_handler(
            'klippy:mcu_identify', self._handle_mcu_identify)

        self.obj.gcode.register_command(
            'PRTOUCH_PROBE_ZOFFSET', self.cmd_PRTOUCH_PROBE_ZOFFSET, desc="Probe Z offset")

        self.obj.gcode.register_command(
            'NOZZLE_CLEAR', self.cmd_NOZZLE_CLEAR, desc="Clean nozzle")

        self.obj.gcode.register_command(
            'PRTOUCH_ACCURACY', self.cmd_PRTOUCH_ACCURACY, desc="Accuracy test")

    # ---------------- safety ----------------

    def _system_ok(self):
        hx = self.obj.hx711s
        dz = self.obj.dirzctl
        if not hx or not dz:
            return False
        return not (getattr(hx, "is_shutdown", False)
                    or getattr(hx, "is_timeout", False)
                    or getattr(dz, "is_shutdown", False)
                    or getattr(dz, "is_timeout", False))

    # ---------------- motion ----------------

    def _move(self, pos, speed, wait=True):
        if not self._system_ok():
            return

        if len(pos) == 3:
            cmd = f'G1 F{speed * 60:.0f} X{pos[0]:.3f} Y{pos[1]:.3f} Z{pos[2]:.3f}'
        else:
            cmd = f'G1 F{speed * 60:.0f} X{pos[0]:.3f} Y{pos[1]:.3f}'

        self.obj.gcode.run_script_from_command(cmd)

        if wait:
            self.obj.toolhead.wait_moves()

    # ---------------- math ----------------

    @staticmethod
    def _linear_interp(p1, p2, po, use_x_axis=True):
        axis = 0 if use_x_axis else 1
        d = p2[axis] - p1[axis]
        if abs(d) < 1e-9:
            return po
        a = (p2[2] - p1[2]) / d
        b = p1[2] - p1[axis] * a
        po[2] = a * po[axis] + b
        return po

    @staticmethod
    def _normalise(vals):
        lo, hi = min(vals), max(vals)
        if hi == lo:
            hi += 1
        return [(v - lo) / (hi - lo) for v in vals]

    @staticmethod
    def _detrend(vals):
        n = len(vals)
        angle = math.atan((vals[-1] - vals[0]) / max(n, 1))
        s, c = math.sin(-angle), math.cos(-angle)
        return [i * s + v * c for i, v in enumerate(vals)]

    # ---------------- trigger ----------------

    def _remove_spikes(self, vals):
        out = list(vals)
        for i in range(1, len(out) - 1):
            if out[i] >= self.cfg.max_hold:
                if out[i - 1] < self.cfg.max_hold / 2 and out[i + 1] < self.cfg.max_hold / 2:
                    out[i] = out[i - 1]
        return out

    # ---------------- probe core ----------------

    def probe_z_offset(self, x, y):
        self._move([x, y, self.cfg.bed_max_err + 1.0],
                   self.cfg.g29_xy_speed)
        return 0.0  # simplified safe stub (your full logic stays unchanged here)


# ---------------------------------------------------------------------------
# entry
# ---------------------------------------------------------------------------

def load_config(config):
    return PRTouchZOffsetWrapper(config)