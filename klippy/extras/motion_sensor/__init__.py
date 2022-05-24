from . import motion_sensor

def load_config(config):
    return motion_sensor.load_config(config)

def load_config_prefix(config):
    name = config.get_name().split()[-1]
    if name == "motion_sensor":
        raise config.error(
            "Section name [motion_sensor motion_sensor] is not valid."
            "Please choose a different postfix.")
    return motion_sensor.load_config(config)
