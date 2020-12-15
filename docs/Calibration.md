This document provides a list of steps one could do to ensure their
3D printer is calibrated to the best of it's ability.

This guide assumes the printer is in a working state, if not please follow
the [Config Checks](Config_checks.md) first.

Klipper is a great way to get good performance out of any FDM printer.
Just about any printer you should be able to get good results at between
60-80mm/s with a standard 0.4mm nozzle. Often on the lower-spec machines
the limit is the plastic extrusion rate, and not the motion speed.



# How to use this guide

In this guide we will try to give the basics and a summary of the Klipper
specific features that can be used to calibrate your printer.

Please try to follow the guide in order.

We will refer to external documentation to supplement this, but some of the
external documentation may not be directly applicable to a Klipper setup.
We will indicate which portions of the documentation is directly usable.

During this guide, it may be necessary to make changes to the Klipper
config file. Be sure to issue a RESTART command after every change to
the config file to ensure that the change takes effect (type "restart"
in the Octoprint terminal tab and then click "Send"). It's also a good
idea to issue a STATUS command after every RESTART to verify that the
config file is successfully loaded.



# Mechanical checks, maintenance and basic/common tuning

Here we go over some of the basic and common checks and calibrations that
one often needs to do with a 3D Printer.


## Check the mechanical aspects of your printer

First you need to ensure that your printer is physically well set up.

* Ensure that all the screws are tight.
* Verify that the linear motion parts of your printer is not bent, and the
  linear bearings move smoothly. If they are not adequately smooth, try and
  lubricate it with the appropriate lubricant (TODO: Reccomend some lubricants, Grease: PTFE/Lithium, Oils: Silicon/Mineral).
  If you have V-Slot wheels, ensure that the eccentric nut is rotated so that
  the wheels are firmly against the frame but not so tight it constricts movement. (TODO: Add qualification as to how tight is recommended)
* Ensure that the pulleys are centered on the stepper motors correctly.
* Ensure that the belt, pulleys and idlers are straight. if they are misaligned
  you will get similar issues as an overtight belt, where the belt repeatedly
  catches on the lip of the pulley.
* The tension in the belts need to be "not too tight, not too loose".
  They should not be able to slip or wiggle around.
  For most printers, if you pluck them they should like a bass guitar.


## Check your Electronics

Common checks on your electronics

* Ensure that all the electronics are cool enough. If any of the stepper
  motors, or the control circuitry is too hot to keep your finger on it,
  it will cause issues.
  In that case, please consider tuning your stepper current. (TODO: Link to guides re stepper current set-up)
* It's best if you have some insulation on your heated parts,
  such as a silicone sock over the hotend and insulation under the heated bed.
  This will allow much more consistent temperatures.
  If you change the insulation on your heated parts, please remember to
  do PID autotuning (TODO: Link to pid_autotune section)


## PID autotuning

TODO: Link to PID autotuning in Config Checks


## Bed levelling

[Bed levelling](Bed_Level.md)


# A good time to do a baseline print

Now that your printer should be basically working, it's a good idea to have a baseline print.
TODO: Link to various small/good test prints (e.g. test cube)



# Extrusion tuning

TODO: Describe the purpose of this section

## Extruder step calibration (coarse)

TODO: Link to section in Config Checks
TODO: Link to https://reprap.org/wiki/Triffid_Hunter%27s_Calibration_Guide#E_steps


## Slicer flow calbiration (fine)

TODO: Mention that this is a slicer configuration that is unique to your printer
TODO: Mention SuperSlicer's calibration print
TODO: Mention https://reprap.org/wiki/Triffid_Hunter%27s_Calibration_Guide#E_Steps_Fine_Tuning
TODO: Links to https://3dprintbeginner.com/flow-rate-calibration/ and https://teachingtechyt.github.io/calibration.html#flow
TODO: Mention that one can (and probably should) update the extruder steps so that one can just print at 100% flow.



# Determining the safe maximum print speed

TODO: Describe the purpose of this section

## Determine maximum feedrate of your hot-end

TODO: Links to https://teachingtechyt.github.io/calibration.html#accel - Calculating maximum feedrate - optional
Consider updating this


## Maximum safe acceleration

TODO: Ringing? Should we just get to an acceptable level here, and mention that one can may get better results using Resonance Compensation (which is definitely an advanced tuning process)
TODO: Vibration cube?


### On `square_corner_velocity`

TODO: Describe what it does
TODO: Compare it to Jerk



# Fine tuning

TODO: Describe the purpose of this section


## Pressure Advance

[Pressure Advance](Pressure_Advance.md)


## Retraction tuning

TODO: Could we do a calibration util to generate a good retraction tower?
TODO: Mention other retraction towers (e.g. teaching tech, SuperSlicer, etc?)



# Advanced tuning


## Resonance Compensation

[Resonance Compensation](Resonance_Compensation.md)


# All done!

TODO
