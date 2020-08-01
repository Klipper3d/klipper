# Moonraker - Moonraker API server configuation and event relay
#
# Copyright (C) 2020 Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license
import os
import socket

class MoonrakerConfig:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.webhooks = self.printer.lookup_object('webhooks')

        # Register webhooks
        self.webhooks.register_endpoint(
            "moonraker/get_configuration", self._handle_config_request)

        # Load Server Config
        self.server_config = None
        self._load_server_config(config)

        # Attempt to load the pause_resume and display_status objects
        self.printer.load_object(config, "pause_resume")
        self.printer.load_object(config, "display_status")

        # Register Events
        self.printer.register_event_handler(
            "klippy:shutdown", self._handle_shutdown)

        gcode = self.printer.lookup_object('gcode')
        gcode.register_output_handler(self._handle_gcode_response)

    def _load_server_config(self, config):
        server_config = {}

        # Check Virtual SDCard is loaded
        if not config.has_section('virtual_sdcard'):
            raise config.error(
                "moonraker: The [virtual_sdcard] section "
                "must be present and configured in printer.cfg")
        vsd_cfg = config.getsection('virtual_sdcard')
        server_config['sd_path'] = vsd_cfg.get('path')

        # Authorization Config
        server_config['require_auth'] = config.getboolean('require_auth', True)
        server_config['enable_cors'] = config.getboolean('enable_cors', False)
        trusted_clients = config.get("trusted_clients", "")
        trusted_clients = [c.strip() for c in trusted_clients.split('\n')
                           if c.strip()]
        trusted_ips = []
        trusted_ranges = []
        for ip in trusted_clients:
            ip_parts = ip.rsplit('/', 1)
            is_range = len(ip_parts) == 2
            is_v4 = True
            # Check IPv4
            try:
                socket.inet_pton(socket.AF_INET, ip_parts[0])
            except socket.error:
                is_v4 = False
            if not is_v4:
                # Check IPv6
                try:
                    socket.inet_pton(socket.AF_INET6, ip_parts[0])
                except socket.error:
                    raise config.error(
                        "moonraker: Invalid value in trusted_clients: %s"
                        % (ip))
            if is_range:
                if not ip_parts[1].isdigit() or \
                        ip_parts[0][-2:] not in [".0", "::"]:
                    raise config.error(
                        "moonraker: Invalid range in trusted_clients: %s"
                        % (ip))
                trusted_ranges.append(ip)
            else:
                trusted_ips.append(ip)
        server_config['trusted_ips'] = trusted_ips
        server_config['trusted_ranges'] = trusted_ranges

        # Printer config file
        start_args = self.printer.get_start_args()
        server_config['printer_config_main'] = start_args['config_file']
        server_config['printer_config_path'] = config.get('config_path', None)
        server_config['klipper_path'] = os.path.normpath(os.path.join(
            os.path.dirname(__file__), "../.."))

        # Plugin Configuration.
        plugin_sections = config.get_prefix_sections('moonraker_plugin')
        for section in plugin_sections:
            plugin_config = {}
            name = "plugin_" + section.get_name().lower().split()[-1]
            options = section.get_prefix_options('')
            for opt in options:
                plugin_config[opt] = section.get(opt)
            server_config[name] = plugin_config

        self.server_config = server_config

    def _handle_shutdown(self):
        self.webhooks.call_remote_method("set_klippy_shutdown")

    def _handle_gcode_response(self, gc_response):
        self.webhooks.call_remote_method(
            "process_gcode_response", response=gc_response)

    def _handle_config_request(self, web_request):
        if self.server_config is None:
            raise web_request.error("Config not available")
        web_request.send(self.server_config)


def load_config(config):
    return MoonrakerConfig(config)
