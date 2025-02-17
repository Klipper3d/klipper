from . import fan

PIN_MIN_TIME = 0.100

class ChamberFan:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.printer.load_object(config, 'heaters')
        self.heaters = []
        self.fan = fan.Fan(config)
        self.fan_speed = config.getfloat('fan_speed', default=1.,
                                         minval=0., maxval=1.)
        self.idle_speed = config.getfloat(
            'idle_speed', default=self.fan_speed, minval=0., maxval=1.)
        self.idle_timeout = config.getint("idle_timeout", default=30, minval=0)
        self.heater_names = config.getlist("heater", ())
        self.last_on = self.idle_timeout
        self.last_speed = 0.
    def handle_connect(self):
        # Heater lookup
        pheaters = self.printer.lookup_object('heaters')
        self.heaters = [pheaters.lookup_heater(n) for n in self.heater_names]
    def handle_ready(self):
        reactor = self.printer.get_reactor()
        reactor.register_timer(self.callback, reactor.monotonic()+PIN_MIN_TIME)
    def get_status(self, eventtime):
        return self.fan.get_status(eventtime)
    def callback(self, eventtime):
        speed = 0.
        active = False
        for heater in self.heaters:
            _, target_temp = heater.get_temp(eventtime)
            if target_temp:
                active = True
        if active:
            self.last_on = 0
            speed = self.fan_speed
        elif self.last_on < self.idle_timeout:
            speed = self.idle_speed
            self.last_on += 1
        if speed != self.last_speed:
            self.last_speed = speed
            curtime = self.printer.get_reactor().monotonic()
            print_time = self.fan.get_mcu().estimated_print_time(curtime)
            self.fan.set_speed(print_time + PIN_MIN_TIME, speed)
        return eventtime + 1.

def load_config_prefix(config):
    return ChamberFan(config)
