# Creality Ender 3 V3 SE stock display integration
#
# Copyright (C) 2026 Bernardo Costa
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import binascii
import logging
import math

from .model import validate_filename
from .tjc3224 import TJC3224
from .transport import DisplayTransport
from .ui import DisplayUI


class E3V3SEDisplayMacro:
    def __init__(self, config):
        self.printer = config.get_printer()
        parts = config.get_name().split()
        if len(parts) < 3 or parts[1].lower() != "macro":
            raise config.error(
                "Display macro sections must use "
                "[e3v3se_display macro <name>]")
        self.name = " ".join(parts[2:])
        self.label = config.get("label")
        gcode_macro = self.printer.load_object(config, "gcode_macro")
        self.template = gcode_macro.load_template(config, "gcode")
        display = self.printer.load_object(config, "e3v3se_display")
        display.add_macro(self)


class E3V3SEDisplay:
    UPDATE_INTERVAL = 1.0

    def __init__(self, config):
        self.config = config
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.gcode = self.printer.lookup_object("gcode")
        required = ("virtual_sdcard", "pause_resume", "display_status")
        missing = [section for section in required
                   if not config.has_section(section)]
        if missing:
            raise config.error(
                "[e3v3se_display] requires these config sections: %s"
                % ", ".join("[%s]" % name for name in missing))
        if not config.get("encoder_pins", None):
            raise config.error(
                "[e3v3se_display] requires encoder_pins (normally "
                "^PA12,^PA11)")
        if not config.get("click_pin", None):
            raise config.error(
                "[e3v3se_display] requires click_pin (normally ^!PB1)")

        self.virtual_sdcard = self.printer.load_object(
            config, "virtual_sdcard")
        self.pause_resume = self.printer.load_object(config, "pause_resume")
        self.display_status = self.printer.load_object(config, "display_status")
        self.manual_probe = self.printer.load_object(config, "manual_probe")
        self.macros = []
        self.ready = False
        self.last_status = {}
        self._manual_probe_was_active = False
        self.material = "PLA"
        self.operation = None
        self.bmcu_session = None
        self._resume_handler = None
        self.operation_result = (True, "")
        self.operation_message = ""
        self.language = config.getchoice(
            "language", {"en": "en", "pt_BR": "pt_BR"}, "en")
        self.operation_timer = self.reactor.register_timer(
            self._operation_event)
        self.heat_timeout = config.getfloat("heat_timeout", 300., above=0.)

        self.max_hotend_temp = config.getfloat(
            "max_hotend_temp", 260., minval=0.)
        self.max_bed_temp = config.getfloat(
            "max_bed_temp", 100., minval=0.)
        self.z_offset_min = config.getfloat("z_offset_min", -5.)
        self.z_offset_max = config.getfloat(
            "z_offset_max", 5., above=self.z_offset_min)
        self.preheat_pla_temps = (
            config.getint("preheat_pla_nozzle", 200, minval=0,
                          maxval=int(self.max_hotend_temp)),
            config.getint("preheat_pla_bed", 60, minval=0,
                          maxval=int(self.max_bed_temp)))
        self.preheat_petg_temps = (
            config.getint("preheat_petg_nozzle", 230, minval=0,
                          maxval=int(self.max_hotend_temp)),
            config.getint("preheat_petg_bed", 70, minval=0,
                          maxval=int(self.max_bed_temp)))
        self.preheat_tpu_temps = (
            config.getint("preheat_tpu_nozzle", 230, minval=0,
                          maxval=int(self.max_hotend_temp)),
            config.getint("preheat_tpu_bed", 50, minval=0,
                          maxval=int(self.max_bed_temp)))

        self.transport = DisplayTransport(config, self._handle_display_data)
        self.lcd = TJC3224(self.transport)
        self.ui = DisplayUI(self.lcd, self)
        from ..display.menu_keys import MenuKeys
        self.keys = MenuKeys(config, self._handle_key)
        self.update_timer = self.reactor.register_timer(self._update_event)

        self.gcode.register_command(
            "ENDER3V3SE_DISPLAY_REFRESH",
            self.cmd_ENDER3V3SE_DISPLAY_REFRESH,
            desc="Redraw the Ender 3 V3 SE stock display")
        self.gcode.register_command(
            "ENDER3V3SE_DISPLAY_STATUS", self.cmd_ENDER3V3SE_DISPLAY_STATUS,
            desc="Report the Ender 3 V3 SE display transport status")
        self.gcode.register_command("ENDER_FILAMENT",
                                    self.cmd_ENDER_FILAMENT)
        self.gcode.register_command("ENDER_FILAMENT_CHECK",
                                    self.cmd_ENDER_FILAMENT_CHECK)
        self.gcode.register_command("ENDER_FILAMENT_CANCEL",
                                    self.cmd_ENDER_FILAMENT_CANCEL)
        self.printer.register_event_handler("klippy:ready", self._handle_ready)
        self.printer.register_event_handler("klippy:disconnect",
                                            self._handle_disconnect)
        self.printer.register_event_handler("klippy:shutdown",
                                            self._handle_shutdown)
        self.printer.register_event_handler("klippy:notify_mcu_error",
                                            self._handle_mcu_error)

    def add_macro(self, macro):
        if any(item.name == macro.name for item in self.macros):
            raise self.printer.config_error(
                "Duplicate e3v3se display macro '%s'" % macro.name)
        self.macros.append(macro)

    def has_macros(self):
        return bool(self.macros)

    def get_macros(self):
        return list(self.macros)

    def _handle_ready(self):
        self.extruder = self.printer.lookup_object("extruder")
        self.heater_bed = self.printer.lookup_object("heater_bed")
        self.fan = self.printer.lookup_object("fan", None)
        self.toolhead = self.printer.lookup_object("toolhead")
        self.gcode_move = self.printer.lookup_object("gcode_move")
        self.print_stats = self.printer.lookup_object("print_stats")
        self.ready = True
        self.last_status = self._collect_status(self.reactor.monotonic())
        self.ui.status = self.last_status
        self.ui.initialize()
        self.reactor.update_timer(self.update_timer, self.reactor.NOW)
        # Install after ready handlers, including the BMCU refill wrapper.
        self.reactor.register_callback(self._install_resume_guard)

    def _install_resume_guard(self, eventtime):
        current = self.gcode.ready_gcode_handlers.get("RESUME")
        if current is None or current is self._resume_handler:
            return
        original = self.gcode.register_command("RESUME", None)
        if original is not None:
            # Endpoint setup can install a package hook after ready.
            # Capture each predecessor to avoid recursive wrapper chains.
            handler = lambda gcmd: self._guarded_resume(gcmd, original)
            self.gcode.register_command("RESUME", handler,
                                        desc="Resume after filament operations")
            # Extended commands are decorated by register_command itself.
            self._resume_handler = self.gcode.ready_gcode_handlers["RESUME"]

    def _guarded_resume(self, gcmd, original):
        if self.is_operation_busy():
            raise gcmd.error("Complete or cancel the filament operation")
        return original(gcmd)

    def _handle_disconnect(self):
        self.operation = None
        self.bmcu_session = None
        self.reactor.update_timer(self.operation_timer, self.reactor.NEVER)
        self.ready = False
        self.reactor.update_timer(self.update_timer, self.reactor.NEVER)

    def _handle_shutdown(self):
        if self.operation is not None:
            self.operation["cancelled"] = True
        if self.bmcu_session is not None:
            self.bmcu_session["cancelled"] = True
        if self.ready:
            message = self.printer.get_state_message()[0]
            self.ui.show_message("KLIPPER SHUTDOWN", message,
                                 self.ui.show_dashboard, self.ui.ERROR)

    def _handle_mcu_error(self, message, details):
        if self.ready:
            detail = details.get("error", message)
            self.ui.show_message("MCU ERROR", detail,
                                 self.ui.show_dashboard, self.ui.ERROR)

    def _handle_display_data(self, data):
        # The stock encoder is wired to the mainboard. Display responses are
        # retained for diagnostics, but are not used as UI input.
        logging.debug("E3V3SE display response: %s",
                      binascii.hexlify(data).decode("ascii"))

    @staticmethod
    def _tuple4(value):
        result = tuple(value)
        return (result + (0., 0., 0., 0.))[:4]

    def _collect_status(self, eventtime):
        hotend = self.extruder.get_status(eventtime)
        bed = self.heater_bed.get_status(eventtime)
        fan = self.fan.get_status(eventtime) if self.fan is not None else {}
        toolhead = self.toolhead.get_status(eventtime)
        gmove = self.gcode_move.get_status(eventtime)
        stats = self.print_stats.get_status(eventtime)
        sdcard = self.virtual_sdcard.get_status(eventtime)
        pause = self.pause_resume.get_status(eventtime)
        dstatus = self.display_status.get_status(eventtime)
        state_message, state_category = self.printer.get_state_message()
        status = {
            "hotend_temp": hotend.get("temperature", 0.),
            "hotend_target": hotend.get("target", 0.),
            "can_extrude": hotend.get("can_extrude", False),
            "bed_temp": bed.get("temperature", 0.),
            "bed_target": bed.get("target", 0.),
            "fan": fan.get("speed", 0.),
            "position": self._tuple4(gmove.get("gcode_position", (0.,) * 4)),
            "axis_minimum": self._tuple4(
                toolhead.get("axis_minimum", (None,) * 4)),
            "axis_maximum": self._tuple4(
                toolhead.get("axis_maximum", (None,) * 4)),
            "homed_axes": toolhead.get("homed_axes", ""),
            "z_offset": gmove.get("homing_origin", (0., 0., 0., 0.))[2],
            "speed_factor": gmove.get("speed_factor", 1.) * 100.,
            "extrude_factor": gmove.get("extrude_factor", 1.) * 100.,
            "print_state": stats.get("state", "standby"),
            "filename": stats.get("filename", ""),
            "print_duration": stats.get("print_duration", 0.),
            "progress": dstatus.get("progress", sdcard.get("progress", 0.)),
            "display_message": dstatus.get("message"),
            "paused": pause.get("is_paused", False),
            "state_category": state_category,
            "state_message": state_message,
            "material": self.material,
            "operation": self.operation_message,
            "operation_busy": self.is_operation_busy(),
        }
        status.update(self._bmcu_status(eventtime))
        return status

    def _bmcu_status(self, eventtime):
        result = {"bmcu_available": False, "bmcu_busy": False,
                  "bmcu_channel": None, "bmcu_label": "BMCU absent"}
        bmcu = self.printer.lookup_object("bmcu", None)
        if bmcu is None:
            return result
        try:
            status = bmcu.get_status(eventtime)
            result["bmcu_busy"] = bool(status.get("active_operations"))
            integration = self.printer.lookup_object(
                "bmcu_integracao_ender", None)
            if integration is not None:
                result["bmcu_busy"] |= integration.get_status(eventtime)[
                    "manual_busy"]
            version_ok = status.get("package_version") == "1.0.6"
            # Versioned read-only adapter: public device snapshots freeze
            # during prints/pauses in 1.0.6. Never refresh or send UART here.
            device = (getattr(bmcu, "devices_by_name", {}).get("bmcu0")
                      if version_ok else None)
            available = (device is not None and device.connected
                         and device.ready and device.runtime_configured
                         and not device.suspended)
            routes = device.status.get("route_state", []) if device else []
            uncertain = (len(routes) != 4 or 2 in routes
                         or bool(status.get("last_error")))
            loaded = [channel for channel in range(4)
                      if "bmcu0:%d" % channel in status.get("loaded_tools", {})]
            uncertain = uncertain or len(loaded) > 1
            result["bmcu_available"] = bool(available)
            result["bmcu_label"] = "BMCU offline"
            if status.get("package_version") != "1.0.6":
                result["bmcu_label"] = "BMCU: version"
            elif available:
                if uncertain:
                    result["bmcu_label"] = "BMCU: check route"
                elif len(loaded) == 1 and loaded[0] in range(4):
                    result["bmcu_channel"] = loaded[0]
                    result["bmcu_label"] = "Channel %d" % (loaded[0] + 1)
                elif loaded:
                    result["bmcu_label"] = "BMCU: conflict"
                else:
                    result["bmcu_label"] = "BMCU ready"
            result["bmcu_uncertain"] = bool(uncertain)
        except Exception:
            logging.exception("Unable to read BMCU display status")
            result["bmcu_label"] = "BMCU: failure"
        return result

    def bmcu_available(self):
        return self._bmcu_status(self.reactor.monotonic())["bmcu_available"]

    def is_operation_busy(self):
        return self.operation is not None or self.bmcu_session is not None

    def bmcu_load(self, channel):
        if channel not in range(4):
            return False, "Invalid BMCU channel"
        return self._start_bmcu("LOAD", channel)

    def bmcu_unload(self):
        status = self._bmcu_status(self.reactor.monotonic())
        if self.command_available("BMCU_ENDER_MANUAL"):
            return self._start_bmcu("UNLOAD", status["bmcu_channel"])
        if status["bmcu_channel"] is None:
            return False, "No confirmed loaded BMCU channel"
        return self._start_bmcu("UNLOAD", status["bmcu_channel"])

    def _start_bmcu(self, action, channel):
        self._install_resume_guard(self.reactor.monotonic())
        status = self._bmcu_status(self.reactor.monotonic())
        if self.is_operation_busy() or status["bmcu_busy"]:
            return False, "Filament operation in progress"
        if not status["bmcu_available"] or status.get("bmcu_uncertain"):
            return False, "BMCU unavailable or route needs verification"
        if self.print_stats.get_status(
                self.reactor.monotonic()).get("state") == "printing":
            return False, "Pause the print before using channels"
        manual = self.command_available("BMCU_ENDER_MANUAL")
        if not manual:
            validated = self._execute(
                "_ENDER_VALIDATE_BMCU\n_ENDER_VALIDATE_FILAMENT ACTION=LOAD\n"
                "_ENDER_VALIDATE_FILAMENT ACTION=UNLOAD")
            if not validated[0]:
                return validated
        session = {"action": action, "channel": channel, "cancelled": False,
                   "manual": manual}
        self.bmcu_session = session
        self.operation_message = ("BMCU: channel %d" % (channel + 1)
                                  if channel is not None
                                  else "Unloading filament")
        self.ui.show_operation()
        self.reactor.register_callback(
            lambda eventtime: self._run_bmcu(session))
        return True, ""

    def _run_bmcu(self, session):
        if self.bmcu_session is not session:
            return
        result = False, "Operation cancelled"
        if not session["cancelled"]:
            if session.get("manual"):
                script = "BMCU_ENDER_MANUAL ACTION=%s" % session["action"]
                if session["action"] == "LOAD":
                    script += " CHANNEL=%d" % (session["channel"] + 1)
            else:
                script = "BMCU_%s DEVICE=bmcu0 CHANNEL=%d" % (
                    session["action"], session["channel"])
            result = self._execute(script)
        if session["cancelled"]:
            stopped = self._execute("BMCU_STOP")
            result = (False, "Operation cancelled; check the BMCU route"
                      if stopped[0] else stopped[1])
        self.bmcu_session = None
        self.operation_message = "Finished" if result[0] else result[1]
        self.ui.show_message("FILAMENT" if result[0] else "BMCU FAILED",
                             self.operation_message, self.ui.show_prepare_menu,
                             self.ui.SUCCESS if result[0] else self.ui.ERROR)

    def _update_event(self, eventtime):
        if not self.ready:
            return self.reactor.NEVER
        try:
            self.transport.request_status()
            self.last_status = self._collect_status(eventtime)
            manual_status = self.manual_probe.get_status(eventtime)
            manual_active = manual_status.get("is_active", False)
            if manual_active:
                self.ui.status = self.last_status
                self.ui.update_manual_probe(manual_status.get("z_position"))
            else:
                if self._manual_probe_was_active:
                    self.ui.show_message(
                        "Z CALIBRATION", "Calibration finished or aborted",
                        self.ui.show_calibration_menu)
                self.ui.update(self.last_status)
            self._manual_probe_was_active = manual_active
        except Exception:
            # A display failure must never stop motion or the MCU link.
            logging.exception("E3V3SE display update failed")
        return eventtime + self.UPDATE_INTERVAL

    def _handle_key(self, key, eventtime):
        try:
            manual_status = self.manual_probe.get_status(eventtime)
            if manual_status.get("is_active", False):
                if key in ("up", "fast_up"):
                    step = 0.25 if key == "fast_up" else 0.05
                    self._execute("TESTZ Z=%.3f" % step)
                elif key in ("down", "fast_down"):
                    step = -0.25 if key == "fast_down" else -0.05
                    self._execute("TESTZ Z=%.3f" % step)
                elif key == "click":
                    self._execute("ACCEPT")
                elif key == "long_click":
                    self._execute("ABORT")
                return
            self.ui.handle_key(key)
        except Exception:
            logging.exception("E3V3SE display key handling failed")

    def _execute(self, script):
        try:
            self.gcode.run_script(script)
            return True, ""
        except self.printer.command_error as exc:
            logging.warning("E3V3SE display command failed: %s", exc)
            return False, str(exc)
        except Exception as exc:
            logging.exception("E3V3SE display command failed")
            return False, str(exc)

    def command_available(self, command):
        commands = self.gcode.get_status(
            self.reactor.monotonic()).get("commands", {})
        return command in commands

    def home_all(self):
        return self._execute("G28")

    def disable_motors(self):
        return self._execute("M84")

    def pause_print(self):
        return self._execute("PAUSE")

    def resume_print(self):
        if self.is_operation_busy():
            return False, "Complete or cancel the filament operation"
        return self._execute("RESUME")

    def cancel_print(self):
        self.cancel_operation()
        return self._execute("CANCEL_PRINT")

    def save_config(self):
        return self._execute("SAVE_CONFIG")

    def cooldown(self):
        self.cancel_operation()
        return self._execute("TURN_OFF_HEATERS\nM107")

    def _set_preheat(self, values):
        return self._execute(
            "SET_HEATER_TEMPERATURE HEATER=extruder TARGET=%d\n"
            "SET_HEATER_TEMPERATURE HEATER=heater_bed TARGET=%d" % values)

    def preheat_pla(self):
        self.material = "PLA"
        if self.command_available("PRE_AQUECER_PLA"):
            return self._execute("PRE_AQUECER_PLA")
        return self._set_preheat(self.preheat_pla_temps)

    def preheat_petg(self):
        self.material = "PETG"
        if self.command_available("PRE_AQUECER_PETG"):
            return self._execute("PRE_AQUECER_PETG")
        return self._set_preheat(self.preheat_petg_temps)

    def preheat_tpu(self):
        self.material = "TPU"
        if self.command_available("PRE_AQUECER_TPU"):
            return self._execute("PRE_AQUECER_TPU")
        return self._set_preheat(self.preheat_tpu_temps)

    def set_hotend(self, value):
        return self._execute(
            "SET_HEATER_TEMPERATURE HEATER=extruder TARGET=%.0f" % value)

    def set_bed(self, value):
        return self._execute(
            "SET_HEATER_TEMPERATURE HEATER=heater_bed TARGET=%.0f" % value)

    def set_fan(self, value):
        value = max(0., min(100., value))
        return self._execute("M106 S%d" % int(round(value * 2.55)))

    def set_speed_factor(self, value):
        return self._execute("M220 S%.0f" % value)

    def set_extrude_factor(self, value):
        return self._execute("M221 S%.0f" % value)

    def set_z_offset(self, delta):
        if "z" not in self.last_status.get("homed_axes", ""):
            return False, "Home Z before changing the live Z offset"
        return self._execute(
            "SET_GCODE_OFFSET Z_ADJUST=%.3f MOVE=1" % delta)

    def move_axis(self, axis, delta):
        if axis == "E":
            return self._begin_operation("EXTRUDE", delta=delta)
        if axis in "XYZ" and axis.lower() not in self.last_status.get(
                "homed_axes", ""):
            return False, "Home %s before moving it" % axis
        speed = 300 if axis in "ZE" else 3000
        setup = self._execute(
            "SAVE_GCODE_STATE NAME=E3V3SE_DISPLAY_MOVE\nG91\nM83")
        if not setup[0]:
            return setup
        move_result = self._execute("G1 %s%.3f F%d" % (axis, delta, speed))
        restore_result = self._execute(
            "RESTORE_GCODE_STATE NAME=E3V3SE_DISPLAY_MOVE")
        return move_result if not move_result[0] else restore_result

    def probe_calibrate(self):
        return self._execute("G28\nPROBE_CALIBRATE")

    def bed_mesh_calibrate(self):
        return self._execute("G28\nBED_MESH_CALIBRATE")

    def screws_tilt(self):
        return self._execute("G28\nSCREWS_TILT_CALCULATE")

    def load_filament(self):
        if not self.command_available("LOAD_FILAMENT"):
            return False, "Configure the LOAD_FILAMENT macro"
        if self.is_operation_busy():
            return False, "Filament operation in progress"
        return self._execute("LOAD_FILAMENT")

    def unload_filament(self):
        if self.bmcu_available():
            return self.bmcu_unload()
        if not self.command_available("UNLOAD_FILAMENT"):
            return False, "Configure the UNLOAD_FILAMENT macro"
        if self.is_operation_busy():
            return False, "Filament operation in progress"
        return self._execute("UNLOAD_FILAMENT")

    def _filament_temperature(self, requested=None, material=None):
        heater = self.extruder.get_heater()
        status = self.extruder.get_status(self.reactor.monotonic())
        minimum = max(1., heater.min_extrude_temp)
        maximum = min(self.max_hotend_temp, heater.max_temp)
        target = requested if requested is not None else status["target"]
        if requested is None and not minimum <= target <= maximum:
            material = material or self.material
            if material not in ("PLA", "PETG", "TPU"):
                raise self.printer.command_error(
                    "Set an appropriate target for material %s" % material)
            values = {"PLA": self.preheat_pla_temps,
                      "PETG": self.preheat_petg_temps,
                      "TPU": self.preheat_tpu_temps}[material]
            target = values[0]
        if (math.isnan(target) or math.isinf(target)
                or not minimum <= target <= maximum):
            raise self.printer.command_error(
                "Filament temperature outside the safe range")
        return target

    def _begin_operation(self, action, temperature=None, delta=0., wait=False,
                         material=None):
        self._install_resume_guard(self.reactor.monotonic())
        if self.operation is not None:
            return False, "Filament operation in progress"
        if self.bmcu_session is not None:
            if not wait:
                return False, "BMCU operation in progress"
            if self.bmcu_session["cancelled"]:
                return False, "BMCU operation cancelled"
        bmcu = self._bmcu_status(self.reactor.monotonic())
        if not wait and bmcu["bmcu_busy"]:
            return False, "BMCU is busy"
        if (not wait and action in ("LOAD", "UNLOAD")
                and self.printer.lookup_object("bmcu", None) is not None
                and (not bmcu["bmcu_available"]
                     or bmcu["bmcu_channel"] is not None
                     or bmcu.get("bmcu_uncertain"))):
            return False, "Use the BMCU channels and check the route"
        stats = self.print_stats.get_status(self.reactor.monotonic())
        if stats.get("state") == "printing" and not wait:
            return False, "Pause the print before moving filament"
        try:
            target = self._filament_temperature(temperature, material)
            heater = self.extruder.get_heater()
            old_target = self.extruder.get_status(
                self.reactor.monotonic())["target"]
            self.printer.lookup_object("heaters").set_temperature(
                heater, target)
        except self.printer.command_error as exc:
            return False, str(exc)
        self.operation = {
            "action": action, "target": target, "old_target": old_target,
            "deadline": self.reactor.monotonic() + self.heat_timeout,
            "delta": delta, "cancelled": False, "moving": False,
            "allow_printing": wait, "wait": wait,
        }
        self.operation_message = "Heating nozzle"
        self.operation_result = (True, "")
        self._show_operation()
        if wait:
            self.reactor.update_timer(self.operation_timer, self.reactor.NEVER)
            # Generic callbacks must finish before BMCU proceeds. This path
            # already owns the G-code mutex; do not acquire it recursively.
            operation = self.operation
            try:
                while self.operation is operation:
                    self._advance_operation(self.reactor.monotonic(), True)
                    if self.operation is operation:
                        self.reactor.pause(self.reactor.monotonic() + 0.25)
            except Exception as exc:
                logging.exception("E3V3SE filament callback failed")
                self._finish_operation(False, str(exc), operation)
            return operation["result"]
        self.reactor.update_timer(self.operation_timer, self.reactor.NOW)
        return True, ""

    def _operation_event(self, eventtime):
        operation = self.operation
        if operation is None or operation["wait"] or operation["moving"]:
            return self.reactor.NEVER
        try:
            self._advance_operation(eventtime)
        except Exception as exc:
            logging.exception("E3V3SE filament operation failed")
            self._finish_operation(False, str(exc), operation)
        current = self.operation
        # A yielded callback's return replaces update_timer's wake time.
        # Keep a newly started asynchronous operation scheduled too.
        return (self.reactor.monotonic() + 0.25
                if current is not None and not current["wait"]
                and not current["moving"]
                else self.reactor.NEVER)

    def _show_operation(self):
        try:
            self.ui.show_operation()
        except Exception:
            logging.exception("Unable to show filament operation")

    def _advance_operation(self, eventtime, from_command=False):
        operation = self.operation
        if operation is None or operation["moving"]:
            return
        status = self.extruder.get_status(eventtime)
        if operation["cancelled"] or self.printer.is_shutdown():
            self._finish_operation(False, "Operation cancelled", operation)
            return
        stats = self.print_stats.get_status(eventtime)
        if stats.get("state") == "printing" and not operation["allow_printing"]:
            self._finish_operation(False, "Print started; operation aborted",
                                   operation)
            return
        if status["target"] != operation["target"]:
            self._finish_operation(
                False, "Nozzle target changed; operation aborted", operation)
            return
        if eventtime >= operation["deadline"]:
            self._finish_operation(False, "Heating timed out", operation)
            return
        if (not status["can_extrude"]
                or status["temperature"] < operation["target"] - 1.):
            return
        operation["moving"] = True
        self.operation_message = {
            "LOAD": "Loading filament", "UNLOAD": "Unloading filament",
            "EXTRUDE": "Moving extruder",
        }[operation["action"]]
        self._show_operation()

        def run(script):
            if not from_command:
                return self._execute(script)
            try:
                self.gcode.run_script_from_command(script)
                return True, ""
            except Exception as exc:
                return False, str(exc)

        # Hold the mutex for save/motion/restore as a unit, including M400.
        # The synchronous callback path already owns it.
        if from_command:
            result = self._run_filament_body(operation, run)
        else:
            with self.gcode.get_mutex():
                # Acquiring the mutex can yield to another reactor callback.
                if self.operation is not operation:
                    return
                def locked_run(script):
                    try:
                        self.gcode.run_script_from_command(script)
                        return True, ""
                    except Exception as exc:
                        return False, str(exc)
                if operation["cancelled"]:
                    result = False, "Operation cancelled"
                else:
                    hotend = self.extruder.get_status(self.reactor.monotonic())
                    if hotend["target"] != operation["target"]:
                        result = (False,
                                  "Nozzle target changed; operation aborted")
                    elif not hotend["can_extrude"]:
                        result = False, "Extruder below minimum temperature"
                    else:
                        result = self._run_filament_body(operation, locked_run)
        if operation["cancelled"]:
            result = False, "Operation cancelled"
        self._finish_operation(result[0], result[1], operation)

    def _run_filament_body(self, operation, run):
        result = run("SAVE_GCODE_STATE NAME=E3V3SE_FILAMENT")
        if result[0]:
            try:
                result = run("M83\nG92 E0\nM220 S100\nM221 S100")
                if result[0] and operation["action"] == "EXTRUDE":
                    script = "G1 E%.3f F300\nM400" % operation["delta"]
                else:
                    script = "_ENDER_%s_BODY" % operation["action"]
                if result[0]:
                    result = run(script)
            finally:
                restored = run("RESTORE_GCODE_STATE NAME=E3V3SE_FILAMENT")
                if result[0]:
                    result = restored
        return result

    def _finish_operation(self, ok, message, operation=None):
        if operation is None:
            operation = self.operation
        if operation is None or self.operation is not operation:
            return
        self.reactor.update_timer(self.operation_timer, self.reactor.NEVER)
        # Do not overwrite a target explicitly changed by another client.
        heater = self.extruder.get_heater()
        status = self.extruder.get_status(self.reactor.monotonic())
        if (not ok and not self.printer.is_shutdown()
                and status["target"] == operation["target"]):
            self.printer.lookup_object("heaters").set_temperature(
                heater, operation["old_target"])
        self.operation = None
        self.operation_result = ok, message
        operation["result"] = self.operation_result
        self.operation_message = "Finished" if ok else message
        if self.bmcu_session is not None:
            self.operation_message = "BMCU: finishing"
            self._show_operation()
            return
        self.ui.show_message("FILAMENT" if ok else "FILAMENT FAILED",
                             self.operation_message, self.ui.show_prepare_menu,
                             self.ui.SUCCESS if ok else self.ui.ERROR)

    def cancel_operation(self):
        if self.bmcu_session is not None:
            self.bmcu_session["cancelled"] = True
            self.operation_message = "Cancelling BMCU"
        if self.operation is not None:
            self.operation["cancelled"] = True
            self.operation_message = "Cancelling operation"
            if not self.operation["moving"]:
                self._finish_operation(False, "Operation cancelled")
        return True, ""

    def cmd_ENDER_FILAMENT_CANCEL(self, gcmd):
        self.cancel_operation()

    def cmd_ENDER_FILAMENT_CHECK(self, gcmd):
        if self.operation is None or self.operation["cancelled"]:
            raise gcmd.error("Filament operation cancelled")
        status = self.extruder.get_status(self.reactor.monotonic())
        if status["target"] != self.operation["target"]:
            raise gcmd.error("Nozzle target changed; operation aborted")
        if not status["can_extrude"]:
            raise gcmd.error("Extruder below minimum temperature")

    def cmd_ENDER_FILAMENT(self, gcmd):
        action = gcmd.get("ACTION").upper()
        if action not in ("LOAD", "UNLOAD"):
            raise gcmd.error("Invalid filament action")
        temperature = gcmd.get_float("TEMP", None)
        wait = gcmd.get_int("WAIT", 0, minval=0, maxval=1)
        self.gcode.run_script_from_command(
            "_ENDER_VALIDATE_FILAMENT ACTION=%s" % action)
        material = gcmd.get("MATERIAL", None)
        if material is not None:
            material = material.upper()
        ok, message = self._begin_operation(
            action, temperature, wait=bool(wait), material=material)
        if not ok:
            raise gcmd.error(message)

    def list_files(self):
        return self.virtual_sdcard.get_file_list(True)

    def start_print(self, filename):
        if getattr(self, "operation", None) is not None or getattr(
                self, "bmcu_session", None) is not None:
            return False, "Complete or cancel the filament operation"
        try:
            filename = validate_filename(filename)
            gcmd = self.gcode.create_gcode_command(
                "SDCARD_PRINT_FILE", "SDCARD_PRINT_FILE", {
                    "FILENAME": filename})
            with self.gcode.get_mutex():
                self.virtual_sdcard.cmd_SDCARD_PRINT_FILE(gcmd)
            return True, ""
        except ValueError as exc:
            return False, str(exc)
        except self.printer.command_error as exc:
            logging.warning("E3V3SE display print start failed: %s", exc)
            return False, str(exc)
        except Exception as exc:
            logging.exception("E3V3SE display print start failed")
            return False, str(exc)

    def run_macro(self, macro):
        try:
            script = macro.template.render()
        except Exception as exc:
            logging.exception("Unable to render E3V3SE display macro")
            return False, str(exc)
        return self._execute(script)

    def cmd_ENDER3V3SE_DISPLAY_REFRESH(self, gcmd):
        if self.ready:
            self.ui.show_dashboard()

    def cmd_ENDER3V3SE_DISPLAY_STATUS(self, gcmd):
        status = self.transport.get_status()
        gcmd.respond_info(
            "ready=%s pending_bytes=%d dropped_frames=%d "
            "mcu_rx_overflows=%d mcu_tx_overflows=%d"
            % (status["ready"], status["pending_bytes"],
               status["dropped_frames"], status["mcu_rx_overflows"],
               status["mcu_tx_overflows"]))

    def get_status(self, eventtime):
        status = self.transport.get_status()
        status.update({"screen": self.ui.mode,
                       "display_ready": self.ready,
                       "material": self.material,
                       "language": self.language,
                       "operation_busy": self.is_operation_busy(),
                       "operation": self.operation_message})
        return status


def load_config(config):
    return E3V3SEDisplay(config)


def load_config_prefix(config):
    return E3V3SEDisplayMacro(config)
