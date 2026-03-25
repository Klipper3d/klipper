# klippy/extras/pca9548a.py
#
# PCA9548A / TCA9548A I2C multiplexer support (multi-instance safe)
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging

from . import bus

_LOG = logging.getLogger(__name__)

_DEFAULT_MUX_I2C_ADDR = 112  # 0x70

# Register gcode commands only once, even if multiple mux instances exist.
_GCODE_REGISTERED = False


def _patch_i2c_no_shutdown(dev, printer):
    """Patch dev.i2c_transfer (instance override) to raise instead of invoke_shutdown.

    bus.py's i2c_transfer calls printer.invoke_shutdown() on any non-SUCCESS I2C
    status (NACK, timeout, etc.).  invoke_shutdown does NOT raise a Python exception
    — it queues a Klipper restart.  Sensor drivers' `except Exception:` blocks
    cannot protect against it.

    For devices behind a mux, transient NACKs on the first read after a channel
    switch must be catchable exceptions so the sensor skips one polling cycle
    rather than killing the printer.

    IMPORTANT: i2c_transfer_cmd is None at config-load time (set only during MCU
    _build_config at klippy:connect).  The lookup is done lazily inside
    safe_transfer so the patch works even though wrap_device is called at config time.
    """
    def safe_transfer(write, read_len=0, minclock=0, reqclock=0, retry=True):
        cmd = getattr(dev, 'i2c_transfer_cmd', None)
        if cmd is None:
            # Before MCU connect: delegate to the class method.
            # No real I2C traffic happens at this stage.
            return type(dev).i2c_transfer(dev, write, read_len,
                                          minclock, reqclock, retry)
        if dev.mcu.is_fileoutput():
            return cmd.send([dev.oid, write, read_len],
                            minclock=minclock, reqclock=reqclock)
        param = cmd.send([dev.oid, write, read_len],
                         minclock=minclock, reqclock=reqclock, retry=retry)
        status = param.get("i2c_bus_status", "UNKNOWN")
        if status == "SUCCESS":
            return param
        raise Exception("I2C addr %d error %s" % (dev.i2c_address, status))

    # Set as instance attribute: Python finds instance-dict entries before
    # non-data descriptors (unbound methods), so self.i2c_transfer(...) inside
    # MCU_I2C.i2c_write / i2c_read / i2c_write_read calls this closure.
    dev.i2c_transfer = safe_transfer


class _MuxedI2CDevice:
    """Wrap an MCU_I2C device so that a mux channel is selected before each transfer."""

    def __init__(self, printer, mux, channel, dev):
        self._printer = printer
        self._mux = mux
        self._channel = int(channel)
        self._dev = dev

        self._pending = []
        self._connected = False
        self._printer.register_event_handler("klippy:connect", self._handle_connect)

    def _handle_connect(self):
        self._connected = True
        if not self._pending:
            return
        pending = self._pending
        self._pending = []
        for method_name, args, kwargs in pending:
            getattr(self, method_name)(*args, **kwargs)

    def _call(self, method_name, *args, **kwargs):
        if not self._connected:
            self._pending.append((method_name, args, kwargs))
            return None
        return self._mux._with_channel(self._channel, getattr(self._dev, method_name),
                                       *args, **kwargs)

    def i2c_write(self, *args, **kwargs):
        return self._call("i2c_write", *args, **kwargs)

    def i2c_read(self, *args, **kwargs):
        return self._call("i2c_read", *args, **kwargs)

    def i2c_write_read(self, *args, **kwargs):
        return self._call("i2c_write_read", *args, **kwargs)

    def __getattr__(self, name):
        return getattr(self._dev, name)


class PCA9548A:
    def __init__(self, config):
        self._printer = config.get_printer()
        self._section_name = config.get_name()          # e.g. "pca9548a mux0"
        self._mux_name = self._section_name.split(None, 1)[1] if " " in self._section_name else self._section_name

        self._lock = self._printer.get_reactor().mutex()
        self._selected = None

        # IMPORTANT: mux control device must be direct and never routed through itself.
        self._i2c = bus.MCU_I2C_from_config(
            config,
            default_addr=_DEFAULT_MUX_I2C_ADDR,
            default_speed=config.getint("i2c_speed", 100000),
        )

        # Register global gcode commands once
        global _GCODE_REGISTERED
        if not _GCODE_REGISTERED:
            _GCODE_REGISTERED = True
            gcode = self._printer.lookup_object("gcode")
            gcode.register_command(
                "IIC_MUX_SELECT",
                self._cmd_IIC_MUX_SELECT,
                desc="Select mux channel: IIC_MUX_SELECT MUX=<name> CHANNEL=<0..7>"
            )
            gcode.register_command(
                "IIC_MUX_STATUS",
                self._cmd_IIC_MUX_STATUS,
                desc="Show mux status: IIC_MUX_STATUS [MUX=<name>]"
            )

    def _set_channel_nolock(self, channel):
        if channel < 0 or channel > 7:
            raise ValueError("PCA9548A channel must be 0..7")
        mask = 1 << channel
        # PCA9548A: writing a byte selects exactly those channels whose bits are set.
        # Writing 0x01 for ch0 implicitly deselects ch1-7; no separate 0x00 write needed.
        self._i2c.i2c_write([mask])
        self._selected = channel

    def _with_channel(self, channel, fn, *args, **kwargs):
        with self._lock:
            # Skip the mux write if the channel is already selected.
            # PCA9548A retains its register across reads, so this is safe as long
            # as _lock prevents concurrent channel switches.
            if self._selected != channel:
                self._set_channel_nolock(channel)
            return fn(*args, **kwargs)

    def wrap_device(self, dev, channel):
        if channel < 0 or channel > 7:
            raise ValueError("PCA9548A channel must be 0..7")
        _patch_i2c_no_shutdown(dev, self._printer)
        return _MuxedI2CDevice(self._printer, self, channel, dev)

    # --------- Global GCODE handlers (look up mux instance by name) ---------

    def _lookup_mux_instance(self, mux_name):
        # Accept "mux0" or full "pca9548a mux0"
        objname = mux_name if mux_name.startswith("pca9548a ") else f"pca9548a {mux_name}"
        mux = self._printer.lookup_object(objname, None)
        if mux is None:
            raise self._printer.config_error(f"IIC_MUX_*: mux '{objname}' not found")
        return mux

    def _cmd_IIC_MUX_SELECT(self, gcmd):
        mux_name = gcmd.get("MUX", None)
        if not mux_name:
            raise gcmd.error("IIC_MUX_SELECT requires MUX=<name>")
        channel = gcmd.get_int("CHANNEL", minval=0, maxval=7)
        mux = self._lookup_mux_instance(mux_name)
        with mux._lock:
            mux._set_channel_nolock(channel)
        gcmd.respond_info(f"{mux_name}: selected channel {channel}")

    def _cmd_IIC_MUX_STATUS(self, gcmd):
        mux_name = gcmd.get("MUX", None)
        if mux_name:
            mux = self._lookup_mux_instance(mux_name)
            gcmd.respond_info(f"{mux_name}: selected channel {mux._selected}")
            return

        gcmd.respond_info("IIC_MUX_STATUS: specify MUX=<name>, e.g. IIC_MUX_STATUS MUX=mux0")


def load_config_prefix(config):
    return PCA9548A(config)
