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



# Mechanical tuning


## Check the mechanical aspects of your printer

First you need to ensure that your printer is physically well set up.

### Frame maintenance

Verify that the following is all good:

* Tighten screws
* Frame stiffness: if in question add corner stiffening measures
* Check linear movement components: play, friction, smoothness, greasing, etc…
* Check to see that the idlers are not on a bent rod.
* Ensure that the pulleys are centered on the stepper motors correctly.
* Ensure that the belt, pulleys and idlers are straight. if they are misaligned
  you will get similar issues as an overtight belt, where the belt repeatedly
  catches on the lip of the pulley.
* The tension in the belts need to be "not too tight, not too loose".
  They should not be able to slip or wiggle around.
  For most printers, if you pluck them they should like a bass guitar.


### Electronics & safety

Also, on the electronics:

* Ensure that all the electronics are cool. If any of the stepper motors, or
  the control circuitry is too hot to keep your finger on it will cause issues.
* Verify that your wiring is in good order, and has strain relief applied on
  moving parts. It's better that the wiring doesn't bend near a plug, and best
  if the movement is over a large portion of the cabling so that you don't
  develop a weak spot.
* An undersized powersupply can cause all kinds of intermittent print quality
  issues, such as irregular underextrusion or temperature fluctuations.
  Diagnosing and fixing such issues is outside of the scope of this guide.


Even though Klipper does its best to ensure safety by detecting common failure
issues that could be dangerous and caused by machine failure, you are dealing
with a high-power device. Ensure you have a strategy to manage a dangerous
situation if any develops.

Have a smoke detector installed correctly near your 3D printer so you can catch
any fires early enough if they occur before any real damage gets done.

It's best if you have some insulation on your hotend and heated bed.
This will allow much more consistent temperatures, and help prevent burning
your fingers accidentally.


## PID autotuning

TODO


## Bed levelling

TODO

 or Resonance Compensation

# A good time to do a baseline print

TODO



# Extrusion tuning


## Extruder step calibration (coarse)

TODO


## Slicer flow calbiration (fine)

TODO



# Determining the safe maximum print speed


## Determine maximum feedrate of your hot-end

TODO


## Maximum safe acceleration

TODO


### On `square_corner_velocity`

TODO


# Fine tuning


## Resonance Compensation

TODO


## Pressure Advance

TODO


## Retraction tuning

TODO



# All done!

TODO
