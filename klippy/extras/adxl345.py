import logging
from .motion_sensor import adxl345

def load_config(config):
    return ADXL345Legacy(config)

class ADXL345Legacy (adxl345.ADXL345):
    def __init__(self, config):
        super(ADXL345Legacy, self).__init__(config)
        logging.warn(
            """The config section [adxl345] has been replaced by
             [motion_sensor] with 'chip: adxl345'.  Please update your
             printer.cfg to ensure future updates do not break your
             configuration.
            """)
