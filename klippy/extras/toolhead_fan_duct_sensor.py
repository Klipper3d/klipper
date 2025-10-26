import logging
import typing


class ToolheadFanDuctSensor:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.reactor = self.printer.get_reactor()
        self.gcode = self.printer.lookup_object("gcode")

        # * Register fan duct sensor
        duct_sensor_pin = config.get("sensor_pin")
        buttons = self.printer.load_object(config, "buttons")
        buttons.register_buttons([duct_sensor_pin], self.fan_duct_sensor_handler)

    def fan_duct_sensor_handler(self, eventtime, state):
        if not state:
            return

        if state:
            # * Stop the printer
            self.printer.invoke_shutdown(
                msg="Toolhead Fan duct is currently open, close the fan duct and restart"
            )


def load_config_prefix(config):
    return ToolheadFanDuctSensor(config)
