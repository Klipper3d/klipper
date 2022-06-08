import logging
from .motion_sensor import adxl345

def load_config(config):
    return adxl345.ADXL345(config)