import logging
import motion_sensor.adxl345

def load_config(config):
    return ADXL345(config)

class ADXL345 (motion_sensor.adxl345.ADXL345):
    def __init__(self, config):
        super(motion_sensor.adxl345.ADXL345, self).__init__(config)
        logging.warn(
            """The config section [adxl345] has been replaced by
             [motion_sensor] with 'chip: adxl345'.  Please update your
             printer.cfg to ensure future updates do not break your
             configuration.
            """)
