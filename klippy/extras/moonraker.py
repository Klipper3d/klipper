# Moonraker - Moonraker API server event relay
#
# Copyright (C) 2020 Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license
import os
import logging
import sys

class MoonrakerEnable:
    def __init__(self, config):
        printer = config.get_printer()
        self.webhooks = printer.lookup_object('webhooks')

        # Check Virtual SDCard is loaded
        if not config.has_section('virtual_sdcard'):
            raise config.error(
                "moonraker: The [virtual_sdcard] section "
                "must be present and configured in printer.cfg")
        vsd_cfg = config.getsection('virtual_sdcard')
        start_args = printer.get_start_args()
        sd_path = vsd_cfg.get('path')
        cfg_file = start_args.get('config_file')
        klipper_path = os.path.normpath(os.path.join(
            os.path.dirname(__file__), "../.."))

        # Attempt to load the pause_resume and display_status objects
        printer.load_object(config, "pause_resume")
        printer.load_object(config, "display_status")

        # Register Events
        printer.register_event_handler(
            "klippy:shutdown", self._handle_shutdown)

        # Register webhooks
        self.webhooks.register_static_path("sd_path", sd_path)
        self.webhooks.register_static_path("printer.cfg", cfg_file)
        self.webhooks.register_static_path("klippy_env", sys.executable)
        self.webhooks.register_static_path("klipper_path", klipper_path)
        self.webhooks.register_endpoint(
            "moonraker/check_available", self._handle_check_available)

        gcode = printer.lookup_object('gcode')
        gcode.register_output_handler(self._handle_gcode_response)

    def _handle_shutdown(self):
        self.webhooks.call_remote_method("set_klippy_shutdown")

    def _handle_gcode_response(self, gc_response):
        self.webhooks.call_remote_method(
            "process_gcode_response", response=gc_response)

    def _handle_check_available(self, web_request):
        logging.info("moonraker:  Connection Established")


def load_config(config):
    return MoonrakerEnable(config)
