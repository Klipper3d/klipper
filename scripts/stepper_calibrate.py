#!/usr/bin/env python2
# Tool to read the current stepper calibration values and provide
# the user with appropriate values per user-provided measurements
#
# Copyright (C) 2019 Kyle Turpin <kyle@moturpin.com>
#
import configparser
config = configparser.ConfigParser()


def main():
    configFile = 'printer.cfg'
    config.read(configFile)
    steppers = {
        "X": {"step_distance": float(config['stepper_x']['step_distance']),
                      "expected": 0,
                      "measured": 0},
        "Y": {"step_distance": float(config['stepper_y']['step_distance']),
                      "expected": 0,
                      "measured": 0},
        "Z": {"step_distance": float(config['stepper_z']['step_distance']),
                    "expected": 0,
                    "measured": 0}
    }

    for key in steppers:
        steppers[key]["expected"]=float(input("What was your expected " + key +" dimension in mm? "))
        steppers[key]["measured"]=float(input("What was your measured " + key +" dimension in mm? "))
        print(key+" step distance: "+str(steppers[key]["step_distance"]))
        steppers[key]["step_distance"] = distance_correction(steppers[key])
        print(key+" step distance corrected: "+str(steppers[key]["step_distance"]))


def distance_correction(stepper):
    measured = stepper["measured"]
    expected = stepper["expected"]
    step_distance = stepper["step_distance"]
    new_distance = (stepper["measured"] /
                    stepper["expected"])*stepper["step_distance"]
    new_distance = round(new_distance, 6)
    return new_distance


main()
