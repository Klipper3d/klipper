from typing import TYPE_CHECKING
from threading import Lock

if TYPE_CHECKING:
    from ..configfile import ConfigWrapper
    from ..gcode import GCodeCommand
    from ..klippy import Printer
    from ..reactor import Reactor
    from .heaters import Heater, PrinterHeaters


class ScheduleStep:
    def __init__(self, start_temperature: float, target_temperature: float, rate: float, hold: float):
        self.start_temperature = start_temperature
        self.target_temperature = target_temperature
        self.rate = rate
        self.hold = hold

        delta_temp = self.target_temperature - self.start_temperature
        ramp_time = delta_temp / self.rate * 3600
        self.duration = ramp_time + self.hold * 60

        self.start_time = None
        self.end_time = None


class HeatersScheduler:
    def __init__(self, config: 'ConfigWrapper'):
        self.printer: 'Printer' = config.get_printer()
        self.reactor: 'Reactor' = self.printer.get_reactor()
        self._heater_names: list[str] = config.getlist("heaters")
        self.heaters: list['Heater'] = []
        self.update_interval = config.getfloat("update_interval", 1.0)

        self.schedule: list[ScheduleStep] = []
        self.step_start_time = 0.
        self.paused = True
        self.lock = Lock()
        self.timer = self.reactor.register_timer(self._update_targets)

        # Register commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("HEATING_SCHEDULE_SHOW", self.cmd_HEATING_SCHEDULE_SHOW,
                               desc="Show the current heating schedule")
        gcode.register_command("HEATING_SCHEDULE_START", self.cmd_HEATING_SCHEDULE_START,
                               desc="Start the heating schedule")
        gcode.register_command("HEATING_SCHEDULE_PAUSE", self.cmd_HEATING_SCHEDULE_PAUSE,
                               desc="Pause the heating schedule")
        gcode.register_command("HEATING_SCHEDULE_ADD_STEP", self.cmd_HEATING_SCHEDULE_ADD_STEP,
                               desc="Add step to the heating schedule")
        gcode.register_command("HEATER_OVERRIDE", self.cmd_HEATER_OVERRIDE,
                               desc="Override the PWM value for a heater")

        self.printer.register_event_handler("klippy:connect", self._handle_connect)
        self.printer.register_event_handler("gcode:request_restart", self.flush_schedule)

    def _handle_connect(self):
        pheaters: 'PrinterHeaters' = self.printer.lookup_object('heaters')
        self.heaters = [pheaters.lookup_heater(heater_name) for heater_name in self._heater_names]
        self.reactor.update_timer(self.timer, self.reactor.NOW)

    def _update_targets(self, eventtime):
        if self.paused:
            with self.lock:
                if self.schedule and self.schedule[0].start_time:
                    self.schedule[0].start_time += self.update_interval
                    self.schedule[0].end_time += self.update_interval
            return eventtime + self.update_interval

        with self.lock:
            if not self.schedule:
                return eventtime + self.update_interval

            step = self.schedule[0]
            if not step.start_time:
                step.start_time = eventtime
                step.end_time = step.start_time + step.duration

            current_target = step.start_temperature + (step.rate / 3600) * (eventtime - step.start_time)
            if step.rate < 0:
                current_target = max(current_target, step.target_temperature)
            else:
                current_target = min(current_target, step.target_temperature)

            if eventtime >= step.end_time:
                self.schedule.pop(0)

        for heater in self.heaters:
            heater.set_temp(current_target)
        return eventtime + self.update_interval

    def flush_schedule(self):
        with self.lock:
            self.schedule = []

    def pause(self):
        self.paused = True

    def resume(self):
        self.paused = False

    def cmd_HEATING_SCHEDULE_PAUSE(self, _gcmd: 'GCodeCommand'):
        self.pause()

    def cmd_HEATING_SCHEDULE_START(self, _gcmd: 'GCodeCommand'):
        self.resume()

    def cmd_HEATING_SCHEDULE_SHOW(self, gcmd: 'GCodeCommand'):
        lines = ["| step |     rate |  target |    hold |  time |"]
        hours = minutes = seconds = 0
        with self.lock:
            for i, step in enumerate(self.schedule):
                seconds += step.duration
                minutes += seconds // 60
                seconds %= 60
                hours += minutes // 60
                minutes %= 60

                lines.append(
                    f"|{str(i).ljust(5)} "
                    f"|{str(round(step.rate)).ljust(4)} °C/h "
                    f"|{str(round(step.target_temperature)).ljust(5)} °C "
                    f"|{str(step.hold).ljust(4) + ' min ' if step.hold else '       - '}"
                    f"|{str(hours).ljust(3)}:{str(minutes).zfill(2)} |")

        msg = "".join(lines)
        did_ack = gcmd.ack(msg)
        if not did_ack:
            gcmd.respond_raw(msg)

    def cmd_HEATING_SCHEDULE_ADD_STEP(self, gcmd: 'GCodeCommand'):
        target = gcmd.get("TARGET")
        rate = gcmd.get("RATE")
        start_temp = gcmd.get_float("FROM", default=0.)
        hold = gcmd.get_float("HOLD", default=0.)
        # TODO: convert to bool
        clear = gcmd.get("CLEAR", default=False)
        start = gcmd.get("START", default=False)
        self.add_heating_schedule_step(target, rate, start_temp, hold, clear, start)

    def add_heating_schedule_step(self, target: float, rate: float, start_temp: float = None, hold: float = 0., clear: bool = False, start: bool = True):
        if not start_temp:
            with self.lock:
                if self.schedule:
                    start_temp = self.schedule[-1].target_temperature
            if not start_temp:
                # TODO: determine start_temp based on lowest current temperature of heaters
                start_temp = 10.

        step = ScheduleStep(start_temperature=start_temp, target_temperature=target, rate=rate, hold=hold)

        with self.lock:
            if clear:
                self.schedule = [step]
            else:
                self.schedule.append(step)

        if start:
            self.paused = False

    def cmd_HEATER_OVERRIDE(self, gcmd: 'GCodeCommand'):
        heater_name = gcmd.get('HEATER')
        pwm_value = gcmd.get_float('PWM')

        pheaters = self.printer.lookup_object('heaters')
        heater = pheaters.lookup_heater(heater_name)
        self.override_heater(heater, pwm_value)

    def override_heater(self, heater: 'Heater', pwm_value: float):
        eventtime = self.reactor.monotonic()
        self.pause()
        heater.set_pwm(eventtime, pwm_value, override=True)


def load_config(config):
    return HeatersScheduler(config)
