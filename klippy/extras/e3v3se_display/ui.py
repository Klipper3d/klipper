# Text based UI for the Ender 3 V3 SE stock display
#
# Copyright (C) 2026 Bernardo Costa
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import textwrap

from .model import FileBrowser
from .tjc3224 import safe_text_bytes, utf8_scroll
from .translations import translate


class MenuItem:
    def __init__(self, label, callback=None, enabled=True):
        self.label = label
        self.callback = callback
        self.enabled = enabled


class DisplayUI:
    PRINT_STATES = {
        "standby": "Ready", "printing": "Printing", "paused": "Paused",
        "complete": "Complete", "cancelled": "Cancelled", "error": "Error",
    }
    BACKGROUND = 0x0841
    PANEL = 0x10a2
    PANEL_ALT = 0x18e3
    FOREGROUND = 0xffff
    MUTED = 0x9cf3
    ACCENT = 0x07ff
    WARNING = 0xfd20
    ERROR = 0xf800
    SUCCESS = 0x07e0

    HEADER_HEIGHT = 35
    BODY_TOP = 40
    ROW_HEIGHT = 38
    ROW_COUNT = 6
    FOOTER_TOP = 278

    def __init__(self, lcd, owner):
        self.lcd = lcd
        self.owner = owner
        self.language = owner.language
        self.mode = "dashboard"
        self.status = {}
        self.items = []
        self.selected = 0
        self.scroll = 0
        self.title = ""
        self.back_callback = None
        self.adjustment = None
        self.browser = FileBrowser()
        self.tick = 0
        self._last_signature = None
        self._last_footer = None
        self._last_print_state = None

    def initialize(self):
        self.lcd.set_backlight(0x40)
        self.lcd.clear(self.BACKGROUND)
        self.show_dashboard()

    def _fill(self, color, x0, y0, x1, y1):
        self.lcd.rectangle(color, x0, y0, x1, y1, filled=True)

    def _text(self, x, y, value, *args):
        self.lcd.text(x, y, translate(value, self.language), *args)

    def _header(self, title, color=None):
        color = self.ACCENT if color is None else color
        self._fill(self.PANEL, 0, 0, 239, self.HEADER_HEIGHT)
        self._text(8, 9, title, color, self.PANEL,
                      self.lcd.FONT_8X16, True, 27)
        self.lcd.line(color, 0, self.HEADER_HEIGHT, 239, self.HEADER_HEIGHT)

    def _footer_signature(self):
        return (int(self.status.get("hotend_temp", 0)),
                int(self.status.get("hotend_target", 0)),
                int(self.status.get("bed_temp", 0)),
                int(self.status.get("bed_target", 0)),
                self.status.get("display_message") or
                self.status.get("bmcu_label", "BMCU unavailable"))

    def _draw_footer(self, force=False):
        signature = self._footer_signature()
        if not force and signature == self._last_footer:
            return
        self._last_footer = signature
        self._fill(self.PANEL, 0, self.FOOTER_TOP, 239, 319)
        hotend = "N %d/%dC" % (signature[0], signature[1])
        bed = "B %d/%dC" % (signature[2], signature[3])
        self._text(6, 284, hotend, self.FOREGROUND, self.PANEL,
                      self.lcd.FONT_8X16, True, 13)
        self._text(128, 284, bed, self.FOREGROUND, self.PANEL,
                      self.lcd.FONT_8X16, True, 12)
        message = signature[4]
        if message:
            self._text(6, 302, message, self.ACCENT, self.PANEL,
                          self.lcd.FONT_8X16, True, 28)

    def show_dashboard(self):
        self.mode = "dashboard"
        self.items = []
        self.adjustment = None
        self._last_signature = None
        self._draw_dashboard(force=True)

    def _dashboard_signature(self):
        pos = self.status.get("position", (0., 0., 0., 0.))
        return (
            self.status.get("print_state", "standby"),
            self.status.get("paused", False),
            int(self.status.get("hotend_temp", 0)),
            int(self.status.get("hotend_target", 0)),
            int(self.status.get("bed_temp", 0)),
            int(self.status.get("bed_target", 0)),
            round(pos[0], 2), round(pos[1], 2), round(pos[2], 2),
            int(self.status.get("progress", 0) * 100),
            self.status.get("filename", ""),
            self.status.get("display_message") or "",
            self.tick // 2,
        )

    def _draw_dashboard(self, force=False):
        signature = self._dashboard_signature()
        if not force and signature == self._last_signature:
            return
        self._last_signature = signature
        self.lcd.clear(self.BACKGROUND)
        self._header("ENDER 3 V3 SE")
        print_state = ("Paused" if signature[1] else
                       self.PRINT_STATES.get(signature[0], signature[0]))
        state_color = self.ACCENT
        if signature[0] in ("error", "cancelled"):
            state_color = self.ERROR
        elif signature[0] == "paused" or signature[1]:
            state_color = self.WARNING
        elif signature[0] == "complete":
            state_color = self.SUCCESS
        self._text(8, 48, "STATE", self.MUTED, self.BACKGROUND,
                      self.lcd.FONT_8X16, True, 8)
        self._text(72, 48, print_state, state_color, self.BACKGROUND,
                      self.lcd.FONT_8X16, True, 18)
        self._text(8, 78, "NOZZLE %d / %d C" % (signature[2], signature[3]),
                      self.FOREGROUND, self.BACKGROUND,
                      self.lcd.FONT_8X16, True, 27)
        self._text(8, 101, "BED    %d / %d C" % (signature[4], signature[5]),
                      self.FOREGROUND, self.BACKGROUND,
                      self.lcd.FONT_8X16, True, 27)
        self._text(8, 132, "X %7.2f  Y %7.2f" % (signature[6], signature[7]),
                      self.FOREGROUND, self.BACKGROUND,
                      self.lcd.FONT_8X16, True, 29)
        self._text(8, 154, "Z %7.2f" % (signature[8],),
                      self.FOREGROUND, self.BACKGROUND,
                      self.lcd.FONT_8X16, True, 18)
        self._text(120, 154, self.status.get("bmcu_label", "BMCU absent"),
                      self.ACCENT, self.BACKGROUND,
                      self.lcd.FONT_8X16, True, 14)
        progress = max(0, min(100, signature[9]))
        self._fill(self.PANEL_ALT, 8, 184, 231, 207)
        if progress:
            self._fill(self.ACCENT, 8, 184, 8 + int(223 * progress / 100.), 207)
        self._text(96, 188, "%3d%%" % progress,
                      self.FOREGROUND, self.PANEL_ALT,
                      self.lcd.FONT_8X16, False, 5)
        filename = signature[10] or "Ready"
        filename = utf8_scroll(filename, 28, signature[12])
        self._text(8, 221, filename, self.FOREGROUND, self.BACKGROUND,
                      self.lcd.FONT_8X16, True, 28)
        message = signature[11] or "Click for menu"
        self._text(8, 248, message, self.MUTED, self.BACKGROUND,
                      self.lcd.FONT_8X16, True, 28)
        self._draw_footer(force=True)

    def _back_item(self, callback):
        return MenuItem("< Back", callback)

    def show_main_menu(self):
        state = self.status.get("print_state", "standby")
        paused = self.status.get("paused", False) or state == "paused"
        printing = state in ("printing", "paused")
        items = []
        if printing:
            if paused:
                items.append(MenuItem(
                    "Resume print", lambda: self._run_and_report(
                        self.owner.resume_print, "Print resumed")))
            else:
                items.append(MenuItem(
                    "Pause print", lambda: self._run_and_report(
                        self.owner.pause_print, "Print paused")))
            items.append(MenuItem("Tune", self.show_tune_menu))
            if paused:
                items.append(MenuItem("Filament", self.show_prepare_menu))
            items.append(MenuItem("Cancel print", self.confirm_cancel))
        else:
            items.append(MenuItem("Print", self.show_files))
            items.append(MenuItem("Filament", self.show_filament_menu))
            items.append(MenuItem("Prepare", self.show_prepare_menu))
            items.append(MenuItem("Move", self.show_move_menu))
            items.append(MenuItem("Temperature", self.show_temperature_menu))
            items.append(MenuItem("Calibration", self.show_calibration_menu))
            if self.owner.has_macros():
                items.append(MenuItem("Macros", self.show_macros_menu))
        items.append(self._back_item(self.show_dashboard))
        self.show_menu("MAIN MENU", items, self.show_dashboard)

    def show_menu(self, title, items, back_callback=None):
        for item in items:
            item.label = translate(item.label, self.language)
        self.mode = "menu"
        self.title = title
        self.items = items
        self.selected = 0
        self.scroll = 0
        self.back_callback = back_callback
        self.adjustment = None
        self._draw_menu()

    def _draw_menu(self):
        self.lcd.clear(self.BACKGROUND)
        self._header(self.title)
        first = self.scroll
        visible = self.items[first:first + self.ROW_COUNT]
        for row in range(self.ROW_COUNT):
            y0 = self.BODY_TOP + row * self.ROW_HEIGHT
            self._fill(self.BACKGROUND, 0, y0, 239, y0 + self.ROW_HEIGHT - 2)
            if row >= len(visible):
                continue
            item_index = first + row
            item = visible[row]
            selected = item_index == self.selected
            background = self.PANEL_ALT if selected else self.BACKGROUND
            color = self.ACCENT if selected else self.FOREGROUND
            if not item.enabled:
                color = self.MUTED
            self._fill(background, 4, y0 + 2, 235, y0 + self.ROW_HEIGHT - 4)
            label = item.label
            if selected:
                label = utf8_scroll(label, 27, self.tick // 2)
            self._text(12, y0 + 11, label, color, background,
                          self.lcd.FONT_8X16, True, 27)
        self._draw_footer(force=True)

    def _run_and_report(self, callback, success_message=None):
        ok, message = callback()
        if not ok:
            self.show_message("COMMAND FAILED", message, self.show_main_menu,
                              self.ERROR)
        elif self.owner.is_operation_busy():
            self.show_operation()
        elif success_message:
            self.show_message("COMMAND STARTED", success_message,
                              self.show_main_menu, self.SUCCESS)
        return ok

    def show_prepare_menu(self):
        items = [
            MenuItem("Home all", lambda: self._run_and_report(
                self.owner.home_all, "Homing all axes")),
            MenuItem("Preheat PLA", lambda: self._run_and_report(
                self.owner.preheat_pla, "PLA preheat selected")),
            MenuItem("Preheat PETG", lambda: self._run_and_report(
                self.owner.preheat_petg, "PETG preheat selected")),
            MenuItem("Preheat TPU", lambda: self._run_and_report(
                self.owner.preheat_tpu, "TPU preheat selected")),
            MenuItem("Cooldown", lambda: self._run_and_report(
                self.owner.cooldown, "Heaters and fan disabled")),
            MenuItem("Load filament", self.request_load),
            MenuItem("Unload filament", lambda: self._run_and_report(
                self.owner.unload_filament, "Unload macro started")),
            MenuItem("BMCU channels", self.show_bmcu_menu),
            MenuItem("Disable motors", lambda: self._run_and_report(
                self.owner.disable_motors, "Motors disabled")),
            self._back_item(self.show_main_menu),
        ]
        self.show_menu("PREPARE", items, self.show_main_menu)

    def show_filament_menu(self):
        items = [
            MenuItem("Preheat PLA", lambda: self._run_and_report(
                self.owner.preheat_pla, "PLA preheat selected")),
            MenuItem("Preheat PETG", lambda: self._run_and_report(
                self.owner.preheat_petg, "PETG preheat selected")),
            MenuItem("Preheat TPU", lambda: self._run_and_report(
                self.owner.preheat_tpu, "TPU preheat selected")),
            MenuItem("Load filament", self.request_load),
            MenuItem("Unload filament", lambda: self._run_and_report(
                self.owner.unload_filament, "Unload macro started")),
            MenuItem("Cooldown", lambda: self._run_and_report(
                self.owner.cooldown, "Heaters and fan disabled")),
            self._back_item(self.show_main_menu),
        ]
        self.show_menu("FILAMENT", items, self.show_main_menu)

    def request_load(self):
        if self.owner.bmcu_available():
            self.show_bmcu_menu()
        else:
            self._run_and_report(self.owner.load_filament, "Load macro started")

    def show_bmcu_menu(self):
        available = self.owner.bmcu_available()
        items = [MenuItem("Channel %d: load" % (channel + 1),
                          lambda c=channel: self._run_and_report(
                              lambda: self.owner.bmcu_load(c)), available)
                 for channel in range(4)]
        items.append(MenuItem("Unload active channel", lambda:
                              self._run_and_report(self.owner.bmcu_unload),
                              available))
        if not available:
            items.append(MenuItem("BMCU unavailable", None, False))
        items.append(self._back_item(self.show_prepare_menu))
        self.show_menu("BMCU CHANNELS", items, self.show_prepare_menu)

    def _axis_adjust(self, axis, step):
        status = self.status
        pos = status.get("position", (0., 0., 0., 0.))
        axis_index = "XYZE".index(axis)
        value = pos[axis_index] if axis != "E" else 0.
        minimum = None
        maximum = None
        if axis != "E":
            axis_min = status.get("axis_minimum", (None, None, None, None))
            axis_max = status.get("axis_maximum", (None, None, None, None))
            minimum = axis_min[axis_index]
            maximum = axis_max[axis_index]
        self.show_adjustment(
            "MOVE %s" % axis, value, step, minimum, maximum,
            lambda delta: self.owner.move_axis(axis, delta), " mm", 2,
            self.show_move_menu, relative=True)

    def show_move_menu(self):
        items = [
            MenuItem("Move X (1 mm)", lambda: self._axis_adjust("X", 1.0)),
            MenuItem("Move Y (1 mm)", lambda: self._axis_adjust("Y", 1.0)),
            MenuItem("Move Z (0.1 mm)", lambda: self._axis_adjust("Z", 0.1)),
            MenuItem("Extrude (5 mm)", lambda: self._axis_adjust("E", 5.0)),
            self._back_item(self.show_main_menu),
        ]
        self.show_menu("MOVE", items, self.show_main_menu)

    def _target_adjust(self, kind, title, value, step, maximum, callback, unit):
        self.show_adjustment(title, value, step, 0, maximum, callback, unit, 0,
                             self.show_temperature_menu)

    def show_temperature_menu(self):
        items = [
            MenuItem("Nozzle target", lambda: self._target_adjust(
                "hotend", "NOZZLE", self.status.get("hotend_target", 0), 5,
                self.owner.max_hotend_temp, self.owner.set_hotend, " C")),
            MenuItem("Bed target", lambda: self._target_adjust(
                "bed", "BED", self.status.get("bed_target", 0), 5,
                self.owner.max_bed_temp, self.owner.set_bed, " C")),
            MenuItem("Fan", lambda: self.show_adjustment(
                "FAN", self.status.get("fan", 0) * 100, 10, 0, 100,
                self.owner.set_fan, " %", 0, self.show_temperature_menu)),
            MenuItem("Preheat PLA", lambda: self._run_and_report(
                self.owner.preheat_pla, "PLA preheat selected")),
            MenuItem("Preheat PETG", lambda: self._run_and_report(
                self.owner.preheat_petg, "PETG preheat selected")),
            MenuItem("Preheat TPU", lambda: self._run_and_report(
                self.owner.preheat_tpu, "TPU preheat selected")),
            MenuItem("Cooldown", lambda: self._run_and_report(
                self.owner.cooldown, "Heaters and fan disabled")),
            self._back_item(self.show_main_menu),
        ]
        self.show_menu("TEMPERATURE", items, self.show_main_menu)

    def show_tune_menu(self):
        items = [
            MenuItem("Speed factor", lambda: self.show_adjustment(
                "SPEED", self.status.get("speed_factor", 100), 10, 10, 300,
                self.owner.set_speed_factor, " %", 0, self.show_tune_menu)),
            MenuItem("Flow factor", lambda: self.show_adjustment(
                "FLOW", self.status.get("extrude_factor", 100), 5, 50, 150,
                self.owner.set_extrude_factor, " %", 0, self.show_tune_menu)),
            MenuItem("Fan", lambda: self.show_adjustment(
                "FAN", self.status.get("fan", 0) * 100, 10, 0, 100,
                self.owner.set_fan, " %", 0, self.show_tune_menu)),
            MenuItem("Z offset", lambda: self.show_adjustment(
                "Z OFFSET", self.status.get("z_offset", 0), 0.01,
                self.owner.z_offset_min, self.owner.z_offset_max,
                self.owner.set_z_offset, " mm", 2, self.show_tune_menu,
                relative=True)),
            self._back_item(self.show_main_menu),
        ]
        self.show_menu("TUNE", items, self.show_main_menu)

    def show_calibration_menu(self):
        items = [
            MenuItem("CR-Touch Z offset", self.confirm_probe_calibrate,
                     self.owner.command_available("PROBE_CALIBRATE")),
            MenuItem("Bed mesh", self.confirm_bed_mesh,
                     self.owner.command_available("BED_MESH_CALIBRATE")),
            MenuItem("Screws tilt", lambda: self._run_and_report(
                self.owner.screws_tilt, "Screw tilt calculation started"),
                self.owner.command_available("SCREWS_TILT_CALCULATE")),
            MenuItem("Save config", self.confirm_save_config),
            self._back_item(self.show_main_menu),
        ]
        self.show_menu("CALIBRATION", items, self.show_main_menu)

    def show_macros_menu(self):
        items = []
        for macro in self.owner.get_macros():
            items.append(MenuItem(
                macro.label,
                lambda selected=macro: self._run_and_report(
                    lambda: self.owner.run_macro(selected),
                    "%s started" % selected.label)))
        if not items:
            items.append(MenuItem("No display macros", None, False))
        items.append(self._back_item(self.show_main_menu))
        self.show_menu("MACROS", items, self.show_main_menu)

    def show_files(self):
        try:
            entries = self.browser.update(self.owner.list_files())
        except Exception as exc:
            self.show_message("FILES UNAVAILABLE", str(exc),
                              self.show_main_menu, self.ERROR)
            return
        items = []
        if self.browser.path:
            items.append(MenuItem("< Parent", self._files_back))
        for entry in entries:
            label = "[%s]" % entry.name if entry.is_dir else entry.name
            items.append(MenuItem(
                label, lambda selected=entry: self._select_file(selected)))
        if not entries:
            items.append(MenuItem("No G-code files", None, False))
        items.append(self._back_item(self.show_main_menu))
        title = self.browser.path or "PRINT FILES"
        self.show_menu(title, items, self.show_main_menu)

    def _files_back(self):
        if self.browser.back():
            self.show_files()
        else:
            self.show_main_menu()

    def _select_file(self, entry):
        path = self.browser.enter(entry)
        if path is None:
            self.show_files()
            return
        self.show_confirmation(
            "START PRINT?", entry.name,
            lambda: self._run_and_report(
                lambda: self.owner.start_print(path), "Print started"),
            self.show_files)

    def confirm_cancel(self):
        self.show_confirmation(
            "CANCEL PRINT?", "This cannot be undone",
            lambda: self._run_and_report(self.owner.cancel_print,
                                         "Print cancelled"),
            self.show_main_menu)

    def confirm_probe_calibrate(self):
        self.show_confirmation(
            "CALIBRATE Z?", "Clear the bed first",
            lambda: self._run_and_report(self.owner.probe_calibrate,
                                         "Homing, then manual probe"),
            self.show_calibration_menu)

    def confirm_bed_mesh(self):
        self.show_confirmation(
            "BUILD MESH?", "Clear the bed first",
            lambda: self._run_and_report(self.owner.bed_mesh_calibrate,
                                         "Homing, then bed mesh"),
            self.show_calibration_menu)

    def confirm_save_config(self):
        self.show_confirmation(
            "SAVE CONFIG?", "Klipper will restart",
            lambda: self._run_and_report(self.owner.save_config),
            self.show_calibration_menu)

    def show_confirmation(self, title, detail, confirm_callback, back_callback):
        items = [MenuItem("Confirm", confirm_callback),
                 MenuItem("Cancel", back_callback)]
        self.show_menu(title, items, back_callback)
        self._text(8, 244, detail, self.WARNING, self.BACKGROUND,
                      self.lcd.FONT_8X16, True, 28)

    def show_adjustment(self, title, value, step, minimum, maximum, callback,
                        unit, decimals, back_callback, relative=False):
        self.mode = "adjustment"
        self.title = title
        self.back_callback = back_callback
        self.adjustment = {
            "value": float(value), "step": float(step),
            "minimum": minimum, "maximum": maximum,
            "callback": callback, "unit": unit, "decimals": decimals,
            "relative": relative,
        }
        self._draw_adjustment()

    def _draw_adjustment(self):
        adjustment = self.adjustment
        self.lcd.clear(self.BACKGROUND)
        self._header(self.title)
        fmt = "%%.%df%%s" % adjustment["decimals"]
        value = fmt % (adjustment["value"], adjustment["unit"])
        self._text(48, 104, value, self.ACCENT, self.BACKGROUND,
                      self.lcd.FONT_16X32, True, 18)
        self._text(22, 172, "Rotate to adjust", self.FOREGROUND,
                      self.BACKGROUND, self.lcd.FONT_8X16, True, 24)
        self._text(22, 198, "Click to return", self.MUTED,
                      self.BACKGROUND, self.lcd.FONT_8X16, True, 24)
        self._draw_footer(force=True)

    def _adjust(self, direction, fast=False):
        adjustment = self.adjustment
        delta = direction * adjustment["step"] * (5 if fast else 1)
        value = adjustment["value"] + delta
        if adjustment["minimum"] is not None:
            value = max(float(adjustment["minimum"]), value)
        if adjustment["maximum"] is not None:
            value = min(float(adjustment["maximum"]), value)
        if adjustment["relative"]:
            command_value = value - adjustment["value"]
            if not command_value:
                return
        else:
            command_value = value
        ok, message = adjustment["callback"](command_value)
        if not ok:
            self.show_message("COMMAND FAILED", message,
                              self.back_callback, self.ERROR)
            return
        adjustment["value"] = value
        if not self.owner.is_operation_busy():
            self._draw_adjustment()

    def show_operation(self):
        self.mode = "operation"
        self.lcd.clear(self.BACKGROUND)
        self._header("FILAMENT", self.WARNING)
        self._text(8, 65, self.owner.operation_message, self.FOREGROUND,
                      self.BACKGROUND, self.lcd.FONT_8X16, True, 28)
        self._text(8, 115, "Click to cancel", self.WARNING,
                      self.BACKGROUND, self.lcd.FONT_8X16, True, 28)
        self._text(8, 152, "Wait for motion to stop", self.MUTED,
                      self.BACKGROUND, self.lcd.FONT_8X16, True, 28)
        self._draw_footer(force=True)

    def show_message(self, title, message, back_callback=None, color=None):
        self.mode = "message"
        self.title = title
        self.back_callback = back_callback or self.show_dashboard
        self.items = []
        self.adjustment = None
        self.lcd.clear(self.BACKGROUND)
        self._header(title, color or self.ACCENT)
        lines = textwrap.wrap(translate(message, self.language), 27) or [""]
        for index, line in enumerate(lines[:8]):
            self._text(8, 54 + index * 25, line, self.FOREGROUND,
                          self.BACKGROUND, self.lcd.FONT_8X16, True, 28)
        self._text(8, 252, "Click to return", self.MUTED, self.BACKGROUND,
                      self.lcd.FONT_8X16, True, 24)
        self._draw_footer(force=True)

    def show_manual_probe(self, z_position):
        self.mode = "manual_probe"
        self.lcd.clear(self.BACKGROUND)
        self._header("Z CALIBRATION", self.WARNING)
        self.update_manual_probe(z_position, force=True)

    def update_manual_probe(self, z_position, force=False):
        if self.mode != "manual_probe":
            self.show_manual_probe(z_position)
            return
        self._fill(self.BACKGROUND, 0, 45, 239, 276)
        value = "Z %.3f mm" % (z_position if z_position is not None else 0.)
        self._text(54, 72, value, self.ACCENT, self.BACKGROUND,
                      self.lcd.FONT_12X24, True, 16)
        self._text(12, 126, "Turn: move 0.05 mm", self.FOREGROUND,
                      self.BACKGROUND, self.lcd.FONT_8X16, True, 27)
        self._text(12, 156, "Click: ACCEPT", self.SUCCESS,
                      self.BACKGROUND, self.lcd.FONT_8X16, True, 27)
        self._text(12, 186, "Hold: ABORT", self.ERROR,
                      self.BACKGROUND, self.lcd.FONT_8X16, True, 27)
        self._draw_footer(force=force)

    def update(self, status):
        self.status = status
        self.tick += 1
        print_state = status.get("print_state", "standby")
        if (self.mode != "operation"
                and print_state in ("printing", "paused")
                and self._last_print_state not in ("printing", "paused")):
            self.show_dashboard()
        self._last_print_state = print_state
        if self.mode == "dashboard":
            self._draw_dashboard()
        elif self.mode == "menu":
            selected = self.items[self.selected] if self.items else None
            if (selected is not None
                    and len(safe_text_bytes(selected.label, 255)) > 27
                    and self.tick % 2 == 0):
                self._draw_menu()
            else:
                self._draw_footer()
        elif self.mode in ("adjustment", "message", "operation"):
            self._draw_footer()

    def handle_key(self, key):
        if self.mode == "operation":
            if key in ("click", "long_click"):
                self.owner.cancel_operation()
            return
        if self.mode == "dashboard":
            if key in ("click", "long_click"):
                self.show_main_menu()
            return
        if self.mode == "manual_probe":
            return
        if self.mode == "adjustment":
            if key in ("up", "fast_up"):
                self._adjust(1, key == "fast_up")
            elif key in ("down", "fast_down"):
                self._adjust(-1, key == "fast_down")
            elif key in ("click", "long_click"):
                self.back_callback()
            return
        if self.mode == "message":
            if key in ("click", "long_click"):
                self.back_callback()
            return
        if self.mode != "menu" or not self.items:
            return
        if key in ("up", "fast_up"):
            amount = 3 if key == "fast_up" else 1
            self.selected = (self.selected + amount) % len(self.items)
        elif key in ("down", "fast_down"):
            amount = 3 if key == "fast_down" else 1
            self.selected = (self.selected - amount) % len(self.items)
        elif key == "long_click":
            if self.back_callback is not None:
                self.back_callback()
            return
        elif key == "click":
            item = self.items[self.selected]
            if item.enabled and item.callback is not None:
                item.callback()
            return
        else:
            return
        if self.selected < self.scroll:
            self.scroll = self.selected
        elif self.selected >= self.scroll + self.ROW_COUNT:
            self.scroll = self.selected - self.ROW_COUNT + 1
        self._draw_menu()
