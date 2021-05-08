import logging

class TemperatureWaitEquilibrium:
    def __init__(self, config):
        self.printer = config.get_printer()
        # Register commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("TEMPERATURE_WAIT_EQUILIBRIUM",
            self.cmd_TEMPERATURE_EQUILIBRIUM,
            desc=self.cmd_TEMPERATURE_EQUILIBRIUM_help)

    def _tempRateLeastSquares(self, temp_values):
        x_sum = 0.0
        y_sum = 0.0
        xy_sum = 0.0
        x_squared_sum = 0.0
        n = len(temp_values)

        # sums
        for i in range(n):
            x = temp_values[i][0]
            y = temp_values[i][1]
            x_sum += x
            y_sum += y
            xy_sum += (x * y)
            x_squared_sum += (x * x)

        # Calculate m for the line equation y = (m * x) + b
        # this is the slope of the line which is the rate of change of the
        # temprature in degrees/second
        return abs((xy_sum - x_sum * y_sum / n) /
            (x_squared_sum - x_sum * x_sum / n))

    cmd_TEMPERATURE_EQUILIBRIUM_help = "Wait for a temperature on a sensor to" \
        "reach thermal equilibrium"
    def cmd_TEMPERATURE_EQUILIBRIUM(self, gcmd):
        sensor_name = gcmd.get('SENSOR')
        heaters = self.printer.lookup_object('heaters')
        sensors = heaters.available_sensors
        if sensor_name not in sensors:
            raise gcmd.error("Unknown sensor '%s'" % (sensor_name,))
        if sensor_name in heaters.heaters:
            raise gcmd.error("Sensor '%s' sensor is bound to a heater" %
                                (sensor_name,))
        sensor = self.printer.lookup_object(sensor_name)
        # The default rate is 1 degree of change per minute, the minimum is 0.1
        target_rate_per_minute = gcmd.get_float('RATE',
                                    default = 1.0, minval = 0.1)
        min_data_points = 5     # To speed up "hot" print starts this algorithm
                                # only waits 5s before using the rate
        timespan = gcmd.get_float('TIMESPAN', default = 30,
                                     minval = min_data_points, maxval = 60)

        # if debugging, immediately return
        if self.printer.get_start_args().get('debugoutput') is not None:
            return

        temp_data = []
        reactor = self.printer.get_reactor()
        start_time = reactor.monotonic()
        eventtime = start_time
        sensor_display_name = sensor_name.split()[-1]
        while not self.printer.is_shutdown():
            temp, _ = sensor.get_temp(eventtime)
            temp_data.append((eventtime, temp))
            if len(temp_data) > timespan:
                temp_data.pop(0)

            if len(temp_data) >= 2:
                rate_per_minute = round(self._tempRateLeastSquares(temp_data)
                                        * 60, 2)
                if len(temp_data) >= min_data_points and \
                        rate_per_minute <= target_rate_per_minute:
                    time_taken = round(eventtime - start_time, 0)
                    gcmd.respond_raw('Temperature equilibrium reached in ' \
                        '{0}s at {1}C '.format(time_taken, round(temp, 1)))
                    return
                gcmd.respond_raw('{0}:{1}C @ {2}C/min /{3}C/min'.format(
                    sensor_display_name, round(temp, 1), rate_per_minute,
                    target_rate_per_minute))
            eventtime = reactor.pause(eventtime + 1.)

def load_config(config):
    return TemperatureWaitEquilibrium(config)
