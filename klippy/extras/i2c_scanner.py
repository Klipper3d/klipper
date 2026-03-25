# I2C bus scanner/prober for MCU (STM32, etc)
#
# Provides:
#   SCAN_IIC  NAME=<name> [START=<addr>] [END=<addr>]
#            [READ_LEN=<n>] [WRITE=<hexbytes>]
#            [MUX=<0|1>] [MUX_ADDR=<addr>]
#            [MUX_CH=<n|ALL|list>] [MUX_ONLY=<0|1>]
#
# PCA9548A/TCA9548A mux scan support (opt-in via config or MUX=1)
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging
import mcu
from . import bus

_LOG = logging.getLogger(__name__)


def _has_param(gcmd, key):
    return gcmd.get(key, None) is not None


def _parse_int(gcmd, key, default=None, minval=None, maxval=None):
    sval = gcmd.get(key, None)
    if sval is None:
        if default is None:
            raise gcmd.error("Missing parameter: %s" % (key,))
        val = default
    else:
        try:
            val = int(str(sval), 0)
        except Exception:
            raise gcmd.error("Invalid %s: %s" % (key, sval))
    if minval is not None and val < minval:
        raise gcmd.error("%s must be >= %d" % (key, minval))
    if maxval is not None and val > maxval:
        raise gcmd.error("%s must be <= %d" % (key, maxval))
    return val


def _parse_hex_bytes(s):
    if s is None or str(s).strip() == "":
        return bytearray()
    t = str(s).replace(",", " ").replace("0x", "").replace("0X", "").strip()
    parts = [p for p in t.split() if p]
    out = bytearray()
    for p in parts:
        if len(p) % 2 == 1:
            p = "0" + p
        for i in range(0, len(p), 2):
            out.append(int(p[i:i + 2], 16))
    return out


def _parse_int_list(text, minval=None, maxval=None):
    if text is None:
        return []
    s = str(text).strip()
    if not s:
        return []
    tokens = [t for t in s.replace(",", " ").split() if t]
    out = []
    for t in tokens:
        v = int(t, 0)
        if minval is not None and v < minval:
            raise ValueError("value %d < %d" % (v, minval))
        if maxval is not None and v > maxval:
            raise ValueError("value %d > %d" % (v, maxval))
        out.append(v)
    return out


def _parse_mux_channels(text):
    if text is None:
        return None
    s = str(text).strip().upper()
    if not s:
        return None
    if s == "ALL":
        return list(range(8))
    chans = _parse_int_list(s, minval=0, maxval=7)
    seen = set()
    out = []
    for c in chans:
        if c not in seen:
            seen.add(c)
            out.append(c)
    return out


def _cfg_int_anybase(config, key, default):
    sval = config.get(key, None)
    if sval is None:
        return default
    try:
        return int(str(sval), 0)
    except Exception as e:
        raise config.error("Invalid %s: %s" % (key, str(e)))


class _NullContext:
    """No-op context manager used when no pca9548a lock is available."""

    def __enter__(self):
        return self

    def __exit__(self, *_):
        pass


_NULL_CTX = _NullContext()


class I2CScanner:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.name = config.get_name().split()[-1]

        self.i2c_speed = config.getint("i2c_speed", 100000, minval=1000)

        mcu_name = config.get("i2c_mcu", "mcu")
        self.mcu = mcu.get_printer_mcu(self.printer, mcu_name)
        ppins = self.printer.lookup_object("pins")

        self._is_sw = config.get("i2c_software_scl_pin", None) is not None
        self._bus_id = None
        self._bus_name = None
        self._scl_pin = None
        self._sda_pin = None

        if self._is_sw:
            scl = config.get("i2c_software_scl_pin")
            sda = config.get("i2c_software_sda_pin")
            sclp = ppins.lookup_pin(scl, share_type="i2c_software_scl_pin")
            sdap = ppins.lookup_pin(sda, share_type="i2c_software_sda_pin")
            if sclp["chip"] != self.mcu or sdap["chip"] != self.mcu:
                raise config.error(
                    "i2c software pins must be on same mcu (%s)" %
                    (mcu_name,))
            self._scl_pin = sclp["pin"]
            self._sda_pin = sdap["pin"]
        else:
            self._bus_name = config.get("i2c_bus", None)
            if self._bus_name is None:
                raise config.error(
                    "Must specify i2c_bus or i2c_software_* pins")

        # ---- Mux options (safe defaults) ----
        self.mux_enable_default = config.getboolean("mux_enable", False)

        mux_addrs_txt = config.get("mux_addresses", None)
        if mux_addrs_txt is None:
            self.mux_addresses = [a for a in range(0x70, 0x78)]
        else:
            addrs = _parse_int_list(mux_addrs_txt, minval=0, maxval=127)
            seen = set()
            self.mux_addresses = [
                a for a in addrs if not (
                    a in seen or seen.add(a))]

        mux_ch_txt = config.get("mux_channels", "0,1,2,3,4,5,6,7")
        chans = _parse_mux_channels(mux_ch_txt)
        self.mux_channels = chans if chans is not None else list(range(8))

        # Health check: one fast probe per channel before full scan.
        # WARNING: if enabled you MUST set mux_health_addr to the address of
        # a device that is actually present on every channel you want to scan.
        # The default 0x7F is a reserved SMBus / 10-bit-addressing prefix;
        # probing it causes every channel to be skipped (nothing ACKs) and can
        # lock the I2C hardware on RP2040/STM32.  Default is now False.
        self.mux_health_check = config.getboolean("mux_health_check", False)
        self.mux_health_addr = _cfg_int_anybase(
            config, "mux_health_addr", 0x70) & 0x7F

        # Disable mux between channels (helps isolate "bad" channels).
        self.mux_disable_between_channels = config.getboolean(
            "mux_disable_between_channels", True)

        # Small delays to avoid hammering the MCU / bus.
        self.mux_channel_settle_s = config.getfloat(
            "mux_channel_settle", 0.002, minval=0.0, maxval=0.5)
        self.mux_post_disable_s = config.getfloat(
            "mux_post_disable", 0.001, minval=0.0, maxval=0.5)

        # Rate-limit repeated scans (prevents "spam scan" from killing MCU).
        self.scan_min_interval_s = config.getfloat(
            "scan_min_interval", 0.25, minval=0.0, maxval=10.0)
        self._last_scan_time = 0.0

        # ---- PCA9548A coordination ----
        # Prevent races with sensor timer callbacks.
        # List the PCA9548A section names (without the "pca9548a " prefix) that
        # share the same mux ICs as this scanner.  The scanner will hold the
        # pca9548a reactor-mutex while scanning each channel so that sensor
        # reads cannot change the mux state mid-scan.
        # Example:  mux_sections: iic_test_pcb
        mux_sections_txt = config.get("mux_sections", "")
        self._mux_section_names = [
            s.strip()
            for s in mux_sections_txt.replace(",", " ").split()
            if s.strip()
        ]
        # addr -> pca9548a object; populated in _handle_connect
        self._pca_objects = {}

        # Single I2C oid
        self.oid = self.mcu.create_oid()
        self.mcu.add_config_cmd("config_i2c oid=%d" % (self.oid,))

        self.cmd_queue = self.mcu.alloc_command_queue()
        self._set_bus_cmd = None
        self._xfer_qcmd = None
        self._write_cmd = None
        self._read_qcmd = None
        self._sw_pulse_ticks = None
        self._sw_rate = None

        self.last_found = []
        self.last_range = (0x03, 0x77)
        self.last_mux = []

        self.mcu.register_config_callback(self._build_config)
        self.printer.register_event_handler(
            "klippy:connect", self._handle_connect)

        gcode = self.printer.lookup_object("gcode")
        gcode.register_mux_command(
            "SCAN_IIC", "NAME", self.name, self.cmd_SCAN_IIC,
            desc="Scan I2C bus on MCU and report responding addresses"
        )
        self.printer.add_object("i2c_scanner " + self.name, self)

    def _handle_connect(self):
        """Look up PCA9548A objects after all modules load."""
        for name in self._mux_section_names:
            full = "pca9548a " + name
            obj = self.printer.lookup_object(full, None)
            if obj is None:
                _LOG.warning(
                    "i2c_scanner %s: mux_sections entry '%s' not found",
                    self.name,
                    full)
                continue
            try:
                addr = obj._i2c.i2c_address & 0x7F
            except Exception:
                _LOG.warning("i2c_scanner %s: could not read address from '%s'",
                             self.name, full)
                continue
            self._pca_objects[addr] = obj
            _LOG.info("i2c_scanner %s: coordinating with %s (addr=0x%02X)",
                      self.name, full, addr)

    def _sleep(self, seconds):
        if seconds <= 0.0:
            return
        waketime = self.reactor.monotonic() + float(seconds)
        self.reactor.pause(waketime)

    def _build_config(self):
        if self._is_sw:
            if self.mcu.try_lookup_command(
                "i2c_set_sw_bus oid=%c scl_pin=%u "
                "sda_pin=%u pulse_ticks=%u address=%u"
            ):
                pulse_ticks = self.mcu.seconds_to_clock(
                    1.0 / self.i2c_speed / 2.0)
                self.mcu.add_config_cmd(
                    "i2c_set_sw_bus oid=%d scl_pin=%s "
                    "sda_pin=%s pulse_ticks=%d address=%d" %
                    (self.oid, self._scl_pin, self._sda_pin, pulse_ticks, 0))
                self._set_bus_cmd = self.mcu.lookup_command(
                    "i2c_set_sw_bus oid=%c scl_pin=%u "
                    "sda_pin=%u pulse_ticks=%u address=%u",
                    cq=self.cmd_queue)
                self._sw_pulse_ticks = pulse_ticks
            else:
                self.mcu.add_config_cmd(
                    "i2c_set_software_bus oid=%d scl_pin=%s "
                    "sda_pin=%s rate=%d address=%d" %
                    (self.oid, self._scl_pin, self._sda_pin, self.i2c_speed, 0))
                self._set_bus_cmd = self.mcu.lookup_command(
                    "i2c_set_software_bus oid=%c scl_pin=%u "
                    "sda_pin=%u rate=%u address=%u",
                    cq=self.cmd_queue)
                self._sw_rate = self.i2c_speed
        else:
            self._bus_id = bus.resolve_bus_name(
                self.mcu, "i2c_bus", self._bus_name)
            self.mcu.add_config_cmd(
                "i2c_set_bus oid=%d i2c_bus=%s rate=%d address=%d"
                % (self.oid, self._bus_id, self.i2c_speed, 0)
            )
            self._set_bus_cmd = self.mcu.lookup_command(
                "i2c_set_bus oid=%c i2c_bus=%u rate=%u address=%u",
                cq=self.cmd_queue
            )

        # Safe transfer path
        if self.mcu.try_lookup_command(
                "i2c_transfer oid=%c write=%*s read_len=%u"):
            self._xfer_qcmd = self.mcu.lookup_query_command(
                "i2c_transfer oid=%c write=%*s read_len=%u",
                "i2c_response oid=%c i2c_bus_status=%c response=%*s",
                oid=self.oid, cq=self.cmd_queue
            )
        else:
            self._write_cmd = self.mcu.lookup_command(
                "i2c_write oid=%c data=%*s", cq=self.cmd_queue
            )
            self._read_qcmd = self.mcu.lookup_query_command(
                "i2c_read oid=%c reg=%*s read_len=%u",
                "i2c_read_response oid=%c response=%*s",
                oid=self.oid, cq=self.cmd_queue
            )

    def _set_addr(self, addr):
        addr &= 0x7F
        if self._is_sw:
            if self._sw_pulse_ticks is not None:
                self._set_bus_cmd.send([self.oid, self._scl_pin, self._sda_pin,
                                        self._sw_pulse_ticks, addr])
            else:
                self._set_bus_cmd.send([self.oid, self._scl_pin, self._sda_pin,
                                        self._sw_rate, addr])
        else:
            self._set_bus_cmd.send(
                [self.oid, self._bus_id, self.i2c_speed, addr])

    def _xfer(self, addr, write_bytes, read_len):
        self._set_addr(addr)
        try:
            if self._xfer_qcmd is not None:
                params = self._xfer_qcmd.send(
                    [self.oid, write_bytes, read_len], retry=False)
                status = params.get("i2c_bus_status", "UNKNOWN")
                resp = params.get("response", b"")
                return (status == "SUCCESS"), status, resp

            # Fallback
            if read_len > 0:
                params = self._read_qcmd.send(
                    [self.oid, write_bytes, read_len], retry=False)
                resp = params.get("response", b"")
                return True, "SUCCESS", resp
            self._write_cmd.send([self.oid, write_bytes], retry=False)
            return True, "SUCCESS", b""
        except Exception as e:
            # CommandError, Timeout, etc.
            return False, "ERROR:%s" % (type(e).__name__,), b""

    def _probe(self, addr, write_bytes, read_len):
        wb = bytearray(write_bytes or b"")
        rl = int(read_len or 0)

        # Empty write + zero read is a no-op on some MCU backends and may
        # incorrectly report success for every address on the bus. Force a
        # minimal read so presence probing performs a real I2C transaction.
        if not wb and rl == 0:
            rl = 1

        ok, status, _ = self._xfer(addr, wb, rl)
        return ok, status

    def _probe_with_fallback(self, addr, write_bytes, read_len):
        """Probe an address, falling back to a write+read if bare read fails.

        Some SMBus devices (e.g. MLX90614) require a register-address write
        before they will respond to a read.  A plain "Receive Byte" (bare read)
        causes them to NACK.  When the caller uses default probe parameters
        (empty write, zero read_len), we first try the standard bare read and,
        if that fails non-fatally, retry with write=[0x00] + read=1 byte.
        This second probe covers devices that need a register-select write first
        without disturbing the data content of any register (register 0x00 is
        safe to read on MLX90614, BME280, SHT3x, etc.).

        Custom probe parameters (user-specified WRITE= or READ_LEN=) are passed
        through unchanged — no fallback.
        """
        ok, st = self._probe(addr, write_bytes, read_len)
        if ok:
            return True, st

        # Only attempt fallback when using the default probe
        # (empty write, zero read).
        # If the caller supplied explicit write bytes or a specific read length,
        # respect those and don't override.
        if not self._is_fatal_status(st) and not write_bytes and read_len == 0:
            ok2, _ = self._probe(addr, bytearray([0x00]), 1)
            if ok2:
                return True, "SUCCESS(fallback)"

        return False, st

    # ---- PCA9548A/TCA9548A helpers ----

    def _mux_write_state(self, mux_addr, state_byte):
        state = int(state_byte) & 0xFF
        wb = bytearray([state])
        ok, status, _ = self._xfer(mux_addr, wb, 0)
        return ok, status

    def _mux_disable(self, mux_addr, pca):
        """Deselect all mux channels, keeping pca._selected in sync."""
        self._mux_write_state(mux_addr, 0x00)
        if pca is not None:
            pca._selected = None

    def _is_fatal_status(self, status):
        # If we hit CommandError, stop sending more traffic immediately.
        if status is None:
            return False
        s = str(status)
        return s.startswith("ERROR:CommandError") or s.startswith(
            "ERROR:TimeoutError")

    def _scan_behind_mux(self, gcmd, mux_addr, channels, start, end,
                         write_bytes, read_len, avoid_addrs):
        mux_results = {"addr": mux_addr, "error": None, "channels": {}}

        # Look up the PCA9548A object that controls this mux address so we can
        # hold its reactor-mutex during each channel scan.  This prevents sensor
        # timer callbacks from changing the mux channel underneath us, which
        # would (a) cause us to probe the wrong channel and (b) produce
        # START_READ_NACK errors in concurrent sensor reads.
        pca = self._pca_objects.get(mux_addr & 0x7F)
        lock_ctx = pca._lock if pca is not None else _NULL_CTX

        for ch in channels:
            with lock_ctx:
                # --- Select channel (while holding pca lock) ---
                ok_sel, st_sel = self._mux_write_state(mux_addr, (1 << ch))
                if ok_sel and pca is not None:
                    pca._selected = ch  # keep pca cache in sync

                if not ok_sel:
                    mux_results["channels"][str(ch)] = {
                        "found": [], "error": st_sel}
                    if self._is_fatal_status(st_sel):
                        mux_results["error"] = st_sel
                    # Isolate the channel before releasing the lock
                    if self.mux_disable_between_channels:
                        self._mux_disable(mux_addr, pca)
                        self._sleep(self.mux_post_disable_s)
                    # Lock is released here (end of with block)
                    if mux_results["error"] is not None:
                        break
                    continue

                # Settle while holding the lock so no other callback can steal
                # the channel from us.
                self._sleep(self.mux_channel_settle_s)

                # Optional health check (1 transfer only)
                if self.mux_health_check:
                    ok_h, st_h = self._probe(
                        self.mux_health_addr, bytearray(), 0)
                    if not ok_h:
                        mux_results["channels"][str(ch)] = {
                            "found": [], "error": st_h}
                        if self.mux_disable_between_channels:
                            self._mux_disable(mux_addr, pca)
                            self._sleep(self.mux_post_disable_s)
                        if self._is_fatal_status(st_h):
                            mux_results["error"] = st_h
                            break
                        continue

                # --- Scan all addresses on this channel ---
                found = []
                ch_error = None
                for addr in range(start, end + 1):
                    if addr in avoid_addrs:
                        continue
                    ok2, st2 = self._probe_with_fallback(
                        addr, write_bytes, read_len)
                    if ok2:
                        found.append(addr)
                    else:
                        if self._is_fatal_status(st2):
                            ch_error = st2
                            mux_results["error"] = st2
                            break

                mux_results["channels"][str(ch)] = {
                    "found": found, "error": ch_error}

                # Disable between channels to isolate problematic branches
                if self.mux_disable_between_channels:
                    self._mux_disable(mux_addr, pca)
                    self._sleep(self.mux_post_disable_s)
                # Lock is released here (end of with block)

            if mux_results["error"] is not None:
                break

        # Ensure mux disabled at end (best effort, no lock needed)
        self._mux_write_state(mux_addr, 0x00)
        if pca is not None:
            pca._selected = None
        return mux_results

    def cmd_SCAN_IIC(self, gcmd):
        try:
            # Rate limit (prevents scan-spam from killing MCU)
            now = self.reactor.monotonic()
            if self.scan_min_interval_s > 0.0 and (
                    now - self._last_scan_time) < self.scan_min_interval_s:
                raise gcmd.error("SCAN_IIC throttled: wait %.2fs" % (
                    self.scan_min_interval_s - (now - self._last_scan_time),))
            self._last_scan_time = now

            start = _parse_int(
                gcmd,
                "START",
                default=0x03,
                minval=0,
                maxval=127)
            end = _parse_int(gcmd, "END", default=0x77, minval=0, maxval=127)
            if end < start:
                raise gcmd.error("END must be >= START")
            read_len = _parse_int(
                gcmd, "READ_LEN", default=0, minval=0, maxval=32)

            write_bytes = _parse_hex_bytes(gcmd.get("WRITE", None))

            mux_enable = self.mux_enable_default
            if _has_param(gcmd, "MUX"):
                mux_enable = bool(
                    _parse_int(
                        gcmd,
                        "MUX",
                        default=0,
                        minval=0,
                        maxval=1))

            mux_only = False
            if _has_param(gcmd, "MUX_ONLY"):
                mux_only = bool(
                    _parse_int(
                        gcmd,
                        "MUX_ONLY",
                        default=0,
                        minval=0,
                        maxval=1))

            mux_addr_forced = None
            if _has_param(gcmd, "MUX_ADDR"):
                mux_addr_forced = _parse_int(
                    gcmd, "MUX_ADDR", default=None, minval=0, maxval=127)

            mux_channels = self.mux_channels
            if _has_param(gcmd, "MUX_CH"):
                chans = _parse_mux_channels(gcmd.get("MUX_CH", None))
                if chans is not None:
                    mux_channels = chans

            if mux_enable and self._xfer_qcmd is None:
                raise gcmd.error(
                    "MUX scanning requires MCU command i2c_transfer.")

            found = []
            mux_results = []
            candidate_mux = set(self.mux_addresses)

            # --- Deselect all known mux channels before the base scan ---
            # If a sensor read left the mux pointing at a channel (e.g. ch=7
            # for SCD30), the base scan would erroneously find behind-mux
            # devices.  Deselecting here (while holding each pca lock in turn)
            # gives a clean slate.
            if mux_enable:
                for mux_addr_pre in self.mux_addresses:
                    pca_pre = self._pca_objects.get(mux_addr_pre & 0x7F)
                    lock_pre = (
                        pca_pre._lock if pca_pre is not None else _NULL_CTX
                    )
                    with lock_pre:
                        self._mux_disable(mux_addr_pre, pca_pre)
                self._sleep(self.mux_post_disable_s)

            # --- Base scan (unless mux_only) ---
            if not mux_only:
                for addr in range(start, end + 1):
                    ok, st = self._probe(addr, write_bytes, read_len)
                    if ok:
                        found.append(addr)
                    elif self._is_fatal_status(st):
                        raise gcmd.error(
                            "I2C transport error during base scan: %s" %
                            (st,))

            # --- Mux scan ---
            if mux_enable:
                if mux_addr_forced is not None:
                    mux_to_scan = [mux_addr_forced]
                else:
                    mux_to_scan = [a for a in found if a in candidate_mux]

                # de-dup keep order
                seen = set()
                mux_to_scan = [
                    a for a in mux_to_scan if not (
                        a in seen or seen.add(a))]

                # avoid_addrs: skip the mux ICs themselves AND any device
                # already found on the main bus (prevents ghost readings).
                avoid_addrs = candidate_mux | set(found)
                if mux_addr_forced is not None:
                    avoid_addrs.add(mux_addr_forced)

                for ma in mux_to_scan:
                    okm, stm = self._probe(ma, bytearray(), 0)
                    if not okm:
                        mux_results.append(
                            {"addr": ma, "error": stm, "channels": {}})
                        if self._is_fatal_status(stm):
                            break
                        continue

                    mr = self._scan_behind_mux(
                        gcmd, ma, mux_channels, start, end,
                        write_bytes, read_len, avoid_addrs)
                    mux_results.append(mr)
                    if mr.get("error") is not None and self._is_fatal_status(
                            mr["error"]):
                        break

                # If we saw fatal transport errors, stop now (prevents
                # alloc_chunk spiral)
                for mr in mux_results:
                    ferr = mr.get("error")
                    if ferr is not None and self._is_fatal_status(ferr):
                        raise gcmd.error(
                            "I2C transport error while scanning mux "
                            "0x%02X: %s. This usually means a bad mux "
                            "channel (SDA/SCL held) -> fix wiring or "
                            "exclude channels via mux_channels / MUX_CH. "
                            "Then run FIRMWARE_RESTART." %
                            (mr["addr"], ferr))

            self.last_found = found
            self.last_range = (start, end)
            self.last_mux = mux_results

            # Output
            lines = []
            if not mux_only:
                if found:
                    lines.append(
                        "SCAN_IIC name=%s range=0x%02X..0x%02X found=%s" % (
                            self.name, start, end,
                            " ".join(["0x%02X" % a for a in found])))
                else:
                    lines.append(
                        "SCAN_IIC name=%s range=0x%02X..0x%02X found=none"
                        % (self.name, start, end)
                    )

            if mux_enable:
                if not mux_results:
                    lines.append("MUX_SCAN name=%s mux=none" % (self.name,))
                else:
                    for mr in mux_results:
                        ma = mr["addr"]
                        if mr.get("error") and not mr.get("channels"):
                            lines.append(
                                "MUX_SCAN mux=0x%02X error=%s" %
                                (ma, mr["error"]))
                            continue
                        chmap = mr.get("channels", {}) or {}
                        for ch_s, info in chmap.items():
                            err = info.get("error")
                            fnd = info.get("found", [])
                            if err:
                                lines.append(
                                    "MUX_SCAN mux=0x%02X ch=%s error=%s" %
                                    (ma, ch_s, err))
                            else:
                                if fnd:
                                    lines.append(
                                        "MUX_SCAN mux=0x%02X ch=%s found=%s"
                                        % (
                                            ma, ch_s,
                                            " ".join(
                                                ["0x%02X" % a for a in fnd]
                                            )))
                                else:
                                    lines.append(
                                        "MUX_SCAN mux=0x%02X ch=%s found=none" %
                                        (ma, ch_s))

            gcmd.respond_info("\n".join(lines))

        except Exception as e:
            _LOG.exception("SCAN_IIC failed")
            raise gcmd.error("SCAN_IIC failed: %s" % (str(e),))

    def get_status(self, eventtime):
        mux_status = []
        for mr in self.last_mux:
            ma = mr.get("addr", 0)
            entry = {
                "addr": "0x%02X" %
                (ma,), "error": mr.get(
                    "error", None), "channels": {}}
            chs = mr.get("channels", {}) or {}
            for ch_s, info in chs.items():
                entry["channels"][str(ch_s)] = {
                    "found": [
                        "0x%02X" % a
                        for a in (info.get("found", []) or [])
                    ],
                    "error": info.get("error", None),
                }
            mux_status.append(entry)

        return {
            "last_found": ["0x%02X" % a for a in self.last_found],
            "last_start": "0x%02X" % self.last_range[0],
            "last_end": "0x%02X" % self.last_range[1],
            "last_mux": mux_status,
        }


def load_config_prefix(config):
    return I2CScanner(config)
