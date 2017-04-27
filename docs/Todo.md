There are several features still to be implemented in Klipper.  In no
particular order:

Host user interaction
=====================

* See if there is a better way to report errors. Octoprint sometimes
  doesn't highlight an error (one has to look in the terminal tab to
  find the error) and errors written to the log can be non-obvious to
  a user.

* Improve gcode interface:

 * Provide a better way to handle print nozzle z offsets. The M206
   command is cryptic to use and it is too easy to set the value
   incorrectly or to forget to set it.

 * Provide a way to temporarily disable endstop checks so that a user
   can issue commands that potentially move the head past
   position_min/position_max.

* Improve logging:

 * Possibly collate and report the statistics messages in the log in a
   more friendly way.

* Possibly support a mechanism for the host to limit maximum velocity
  so that the mcu is never requested to step at a higher rate than it
  can support.

Safety features
===============

* Support loading a valid step range into the firmware after
  homing. This would provide a sanity check in the firmware that would
  reduce the risk of the host commanding a stepper motor past its
  valid step range. To maintain high efficiency in the firmware, the
  firmware would only need to check periodically (eg, every 100ms)
  that the stepper is in range.

 * Possibly support periodically querying the endstop switches and use
   multiple step ranges depending on the switch state. This would
   enable runtime endstop detection. (However, it's unclear if runtime
   endstop detection is a good idea because of spurious signals caused
   by electrical noise.)

* Support validating that heaters are heating at expected rates. This
  can be useful to detect a sensor failure (eg, thermistor short) that
  could otherwise cause the PID to command excessive heating.

Testing features
================

* Complete the host based simulator. It's possible to compile the
  firmware for a "host simulator", but that simulator doesn't do
  anything currently. It would be useful to expand the code to support
  more error checks, kinematic simulations, and improved logging.

Documentation
=============

* Document and test running the host software on a Beagle Bone Black.

* Add documentation describing how to perform bed-leveling accurately
  in Klipper. Improve description of stepper phase based bed leveling.

Hardware features
=================

* Port firmware to more architectures:
 * Beagle Bone Black PRU
 * Smoothieboard / NXP LPC1769 (ARM cortex-M3)
 * Unix based scheduling; Unix based real-time scheduling

* Support for additional kinematics: scara, etc.

* Support shared motor enable GPIO lines.

* Support for multiple extruders.

* Support for bed-level probes.

* Possible support for touch panels attached to the micro-controller.
  (In general, it would be preferable to attach touch panels to the
  host system and have octoprint interact with the panel directly, but
  it would also be useful to handle panels already hardwired to the
  micro-controller.)

* Possibly support printers using multiple micro-controllers.

Misc features
=============

* Possibly use cubic functions instead of quadratic functions in step
  compression code.

* Possibly support a "feed forward PID" that takes into account the
  amount of plastic being extruded. If the extrude rate changes
  significantly during a print it can cause heating bumps that the PID
  overcompensates for. The temperature change due to the extrusion
  rate could be modeled to eliminate these bumps and make the
  extrusion temperature more consistent.
