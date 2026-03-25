# Transparent I2C routing through PCA9548A/TCA9548A muxes
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging

from . import bus

_LOG = logging.getLogger(__name__)

_ORIG_MCU_I2C_FROM_CONFIG = None


def _strip_comment(line: str) -> str:
    for sep in ("#", ";"):
        idx = line.find(sep)
        if idx >= 0:
            line = line[:idx]
    return line.strip()


def _parse_int_auto(s: str) -> int:
    # Accept "118", "0x76", "0X76"
    return int(s.strip(), 0)


def _patched_MCU_I2C_from_config(
        config,
        default_addr=None,
        default_speed=100000):
    dev = _ORIG_MCU_I2C_FROM_CONFIG(config, default_addr, default_speed)
    printer = config.get_printer()
    router = printer.lookup_object("i2c_router", None)
    if router is None:
        return dev
    return router.route_device(config, dev, default_addr)


class I2CRouter:
    """
    Routes I2C devices through PCA9548A/TCA9548A with minimal intervention.

    Mapping keys supported:

      1) Exact section name (best when duplicate addresses exist):
         "temperature_sensor chamber_1" = mux0:1

      2) Triplet (only when bus name exists and is unique):
         "mcu/i2c1a/0x76" = mux0:2

      3) Object-name alias (most "transparent"):
         "chamber_1" = mux0:1
         This matches any config section whose name part is "chamber_1",
         e.g. "temperature_sensor chamber_1", "aht10 chamber_1", etc.
    """

    def __init__(self, config):
        self._printer = config.get_printer()
        self._strict = config.getboolean("strict", False)
        self._debug = config.getboolean("debug", False)

        # key -> (mux_section_name, channel)
        self._map = {}

        raw_map = config.get("map", "").splitlines()
        for raw in raw_map:
            line = _strip_comment(raw)
            if not line:
                continue
            if "=" not in line:
                raise config.error(
                    "i2c_router: each map line must be 'KEY = muxname:channel'")
            key, val = [p.strip() for p in line.split("=", 1)]
            if ":" not in val:
                raise config.error(
                    "i2c_router: map value must be 'muxname:channel'")
            mux_name, ch_str = [p.strip() for p in val.split(":", 1)]
            ch = _parse_int_auto(ch_str)
            if ch < 0 or ch > 7:
                raise config.error("i2c_router: channel must be 0..7")
            self._map[key] = (mux_name, ch)

        gcode = self._printer.lookup_object("gcode")
        # Klipper disallows digits in extended command names -> use IIC not I2C
        gcode.register_command(
            "IIC_ROUTER_STATUS",
            self.cmd_IIC_ROUTER_STATUS,
            desc="Show IIC(I2C) router mappings and mux resolution.",
        )

    def _lookup_mux(self, mux_name: str):
        # Allow either "mux0" or "pca9548a mux0"
        objname = mux_name if mux_name.startswith(
            "pca9548a ") else f"pca9548a {mux_name}"
        mux = self._printer.lookup_object(objname, None)
        if mux is None:
            raise self._printer.config_error(
                f"i2c_router: mux '{objname}' not found")
        return mux

    def _effective_i2c_params(self, config, default_addr):
        section = config.get_name()

        # Many drivers default i2c_mcu to "mcu"
        mcu = config.get("i2c_mcu", "mcu")

        # Hardware I2C uses i2c_bus; software I2C typically does not set i2c_bus
        bus_name = config.get("i2c_bus", None)

        if default_addr is None:
            addr = config.getint("i2c_address")
        else:
            addr = config.getint("i2c_address", default_addr)

        return section, mcu, bus_name, addr

    @staticmethod
    def _name_alias(section: str):
        # "temperature_sensor chamber_1" -> "chamber_1"
        parts = section.split(None, 1)
        if len(parts) == 2:
            return parts[1].strip()
        return None

    def _wrap(self, mux_name: str, ch: int, dev, why: str, section: str):
        mux = self._lookup_mux(mux_name)
        if self._debug:
            _LOG.info(
                "i2c_router: routing '%s' via %s:%d (%s)",
                section,
                mux_name,
                ch,
                why)
        return mux.wrap_device(dev, ch)

    def route_device(self, config, dev, default_addr):
        section_name = config.get_name()

        # Never route the mux controller itself.
        if section_name.startswith("pca9548a "):
            return dev
        # Never route the router section.
        if section_name == "i2c_router":
            return dev

        section, mcu, bus_name, addr = self._effective_i2c_params(
            config, default_addr)

        # 1) Exact section mapping (handles duplicates)
        if section in self._map:
            mux_name, ch = self._map[section]
            return self._wrap(mux_name, ch, dev, "section", section)

        # 2) Triplet mapping (when bus_name exists)
        if bus_name:
            trip_dec = f"{mcu}/{bus_name}/{addr}"
            trip_hex = f"{mcu}/{bus_name}/0x{addr:02x}"

            if trip_dec in self._map:
                mux_name, ch = self._map[trip_dec]
                return self._wrap(mux_name, ch, dev, "triplet-dec", section)

            if trip_hex in self._map:
                mux_name, ch = self._map[trip_hex]
                return self._wrap(mux_name, ch, dev, "triplet-hex", section)

        # 3) Name-only alias mapping (most transparent)
        alias = self._name_alias(section)
        if alias and alias in self._map:
            mux_name, ch = self._map[alias]
            return self._wrap(mux_name, ch, dev, "alias", section)

        if self._strict:
            raise config.error(
                "i2c_router(strict): no route for '%s' "
                "(mcu=%s bus=%s addr=0x%02x)"
                % (section, mcu, bus_name, addr))

        if self._debug:
            _LOG.info(
                "i2c_router: no route for '%s' (passing through)",
                section)

        return dev

    # ---------- GCODE ----------
    def cmd_IIC_ROUTER_STATUS(self, gcmd):
        lines = []
        lines.append("IIC_ROUTER_STATUS:")
        lines.append(f"  strict: {self._strict}")
        lines.append(f"  debug: {self._debug}")
        if not self._map:
            lines.append("  map: (empty)")
        else:
            lines.append("  map:")
            for k, (mux, ch) in sorted(self._map.items(), key=lambda x: x[0]):
                objname = mux if mux.startswith(
                    "pca9548a ") else f"pca9548a {mux}"
                exists = self._printer.lookup_object(objname, None) is not None
                lines.append(
                    "    %s = %s:%s  (%s)"
                    % (k, mux, ch, "ok" if exists else "MISSING"))
        gcmd.respond_info("\n".join(lines))


def load_config(config):
    global _ORIG_MCU_I2C_FROM_CONFIG
    if _ORIG_MCU_I2C_FROM_CONFIG is None:
        _ORIG_MCU_I2C_FROM_CONFIG = bus.MCU_I2C_from_config
        bus.MCU_I2C_from_config = _patched_MCU_I2C_from_config
        _LOG.info("i2c_router: patched bus.MCU_I2C_from_config()")

    router = I2CRouter(config)
    config.get_printer().add_object("i2c_router", router)
    return router
