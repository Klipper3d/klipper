#!/usr/bin/python3
import configparser
config = configparser.ConfigParser()


def main():
    configFile = 'printer.cfg'
    config.read(configFile)
    steppers = {
        "extruder": {"step_distance": float(config['extruder']['step_distance']),
                    "starting": 0,
                    "ending": 0,
                    "expected":100}
    }

    for key in steppers:
        steppers[key]["starting"]=float(input("What was your starting dimension in mm? "))
        steppers[key]["ending"]=float(input("What was your ending dimension in mm? "))
        steppers[key]["expected"]=float(input("What was your expected extrusion in mm? "))
        print(key+" step distance: "+str(steppers[key]["step_distance"]))
        steppers[key]["step_distance"] = distance_correction(steppers[key])
        print(key+" step distance corrected: "+str(steppers[key]["step_distance"]))


def distance_correction(stepper):
    #actual/expected*(current_step_distance) = new_step_distance
    starting = stepper["starting"]
    ending = stepper["ending"]
    measured = starting - ending
    expected = stepper["expected"]
    step_distance = stepper["step_distance"]
    new_distance = (measured /
                    stepper["expected"])*stepper["step_distance"]
    new_distance = round(new_distance, 6)
    return new_distance


main()
