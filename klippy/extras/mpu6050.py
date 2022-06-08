import logging
from .motion_sensor import invensense

def load_config(config):
    return invensense.MPU6050(config)
