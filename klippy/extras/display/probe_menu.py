# -*- coding: utf-8 -*-
# Menu based probing wizard
#
# Copyright (C) 2018  Janar Sööt <janar.soot@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os


class ProbeHelperMenu:
    def __init__(self, config, menu):
        self.printer = config.get_printer()
        self.toolhead = None
        # Load menu
        self.menu = menu
        self.menu.load_config(os.path.dirname(__file__), 'probe_menu.cfg')
        # menuitem names
        self.probe_menu_done = "__probe_helper_done"
        self.probe_menu_adjust_100 = "__probe_helper_adjust_1mm"
        self.probe_menu_adjust_010 = "__probe_helper_adjust_01mm"
        self.probe_menu_adjust_001 = "__probe_helper_adjust_001mm"
        self.probe_menu_moving = "__probe_helper_moving"
        # check menuitem
        self.menu.lookup_menuitem(self.probe_menu_done)
        self.menu.lookup_menuitem(self.probe_menu_adjust_100)
        self.menu.lookup_menuitem(self.probe_menu_adjust_010)
        self.menu.lookup_menuitem(self.probe_menu_adjust_001)
        self.menu.lookup_menuitem(self.probe_menu_moving)
        # Probing context
        self._adjust_card = self.probe_menu_adjust_100
        self._wizard_running = False
        self._end_status = 0
        self._points_current = 0
        self._points_count = 0
        # register itself for a printer_state callback
        self.printer.add_object('probe_menu', self)
        # Register event handlers
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        self.printer.register_event_handler("probe:start_manual_probing",
                                            self.handle_probing_start)
        self.printer.register_event_handler("probe:end_manual_probing",
                                            self.handle_probing_end)
        self.printer.register_event_handler("probe:finalize",
                                            self.handle_probe_finalize)
        self.printer.register_event_handler("menu:action:probe_helper",
                                            self.handle_menu_commands)

    def handle_ready(self):
        self.toolhead = self.printer.lookup_object('toolhead')

    def get_status(self, eventtime):
        return {
            'index': self._points_current+1,
            'length': self._points_count,
            'remaining': max(0, self._points_count-(self._points_current+1)),
            'end_status': self._end_status
        }

    def handle_menu_commands(self, cmd):
        if str(cmd) == 'card100':
            self._adjust_card = self.probe_menu_adjust_100
            self.menu.restart_root(self._adjust_card)
        elif str(cmd) == 'card010':
            self._adjust_card = self.probe_menu_adjust_010
            self.menu.restart_root(self._adjust_card)
        elif str(cmd) == 'card001':
            self._adjust_card = self.probe_menu_adjust_001
            self.menu.restart_root(self._adjust_card)
        elif str(cmd) == 'abort':
            pass

    def display_done(self, eventtime):
        self.menu.restart_root(self.probe_menu_done)

    def display_adjust(self, eventtime):
        self._adjust_card = self.probe_menu_adjust_100
        self.menu.restart_root(self._adjust_card)

    def display_moving(self, eventtime):
        self.menu.restart_root(self.probe_menu_moving)

    def close_probe_wizard(self, eventtime):
        self._adjust_card = self.probe_menu_adjust_100
        self._wizard_running = False
        self._end_status = 0
        self._points_current = 0
        self._points_count = 0
        # return to default menu
        self.menu.restart_root()

    def wait_toolhead_moves(self, eventtime, event_print_time):
        print_time, est_print_time, lookahead_empty = self.toolhead.check_busy(
            eventtime)
        if est_print_time >= event_print_time:
            self.display_adjust(eventtime)
        else:
            self.menu.after(0.5, self.wait_toolhead_moves, event_print_time)

    # probe event methods
    def handle_probing_start(self, event_print_time, points):
        self._points_current, self._points_count = points
        if not self._wizard_running:
            self._wizard_running = True
            self.menu.after(0, self.display_moving)
        self.menu.after(0, self.wait_toolhead_moves, event_print_time)

    def handle_probing_end(self):
        self.menu.after(0, self.display_moving)

    def handle_probe_finalize(self, success):
        self._end_status = success
        self.menu.after(0, self.display_done)
        self.menu.after(4., self.close_probe_wizard)


def load_config(config):
    return ProbeHelperMenu(config)
