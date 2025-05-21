import math, logging
# Motor database, contains specifications for stepper motors.

# R is coil resistance, Ohms
# L is coil inductance, Henries
# T is holding torque, Nm (be careful about units here)
# I is nominal rated current, Amps


class MotorConstants:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.R = config.getfloat('resistance', minval=0.)
        self.L = config.getfloat('inductance', minval=0.)
        self.T = config.getfloat('holding_torque', minval=0.)
        self.S = config.getint('steps_per_revolution', minval=0)
        self.I = config.getfloat('max_current', minval=0.)
        self.cbemf = self.T / (2.0 * self.I)
    def pwmgrad(self, fclk=12.5e6, steps=0, volts=24.0):
        if steps==0:
            steps=self.S
        return int(math.ceil(self.cbemf * 2 * math.pi * fclk  * 1.46 / (volts * 256.0 * steps)))
    def pwmofs(self, volts=24.0, current=0.0):
        I = current if current > 0.0 else self.I
        return int(math.ceil(374 * self.R * I / volts))
    # Maximum revolutions per second before PWM maxes out.
    def maxpwmrps(self, fclk=12.5e6, steps=0, volts=24.0, current=0.0):
        if steps==0:
            steps=self.S
        return (255 - self.pwmofs(volts, current)) / ( math.pi * self.pwmgrad(fclk, steps))
    def hysteresis(self, extra=0, fclk=12.5e6, volts=24.0, current=0.0, tbl=1, toff=0):
        I = current if current > 0.0 else self.I
        logging.info("autotune_tmc seting hysteresis based on %s V", volts)
        tblank = 16.0 * (1.5 ** tbl) / fclk
        tsd = (12.0 + 32.0 * toff) / fclk
        dcoilblank = volts * tblank / self.L
        dcoilsd = self.R * I * 2.0 * tsd / self.L
        logging.info("dcoilblank = %f, dcoilsd = %f", dcoilblank, dcoilsd)
        hysteresis = extra + int(math.ceil(max(0.5 + ((dcoilblank + dcoilsd) * 2 * 248 * 32 / I) / 32 - 8, -2)))
        htotal = min(hysteresis, 14)
        hstrt = max(min(htotal, 8), 1)
        hend = min(htotal - hstrt, 12)
        logging.info("hysteresis = %d, htotal = %d, hstrt = %d, hend = %d", hysteresis, htotal, hstrt, hend)
        return hstrt - 1, hend + 3


def load_config_prefix(config):
    return MotorConstants(config)

