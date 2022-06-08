import logging
from .motion_sensor import invensense

def load_config(config):
    return invensense.MPU9250(config)
