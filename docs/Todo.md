Klipper is currently in an experimental state. There are several
features still to be implemented.  In no particular order:

Host user interaction
=====================

* Improve error reporting:

 * Improve error checks on config file values. If a parameter is
   incorrect or missing a more friendly error message should be
   presented to the user.

 * Warn on any unused config settings (to catch spelling errors or
   fields defined in an incorrect section).

 * Find a better way to report errors. Octoprint sometimes doesn't
   highlight an error (one has to look in the terminal tab to find the
   error) and errors written to the log can be non-obvious to a user.

 * Implement timeouts on homing. The host currently waits forever if
   an endstop is not hit during a homing operation.

* Improve startup:

 * Provide startup scripts so that Klippy can startup at system
   bootup.

 * Possibly rework the startup order of Klippy communication with
   octoprint and the mcu. Currently Klippy only starts communication
   with Octoprint after it connects with the firmware. However, this
   limits the ability for Klippy to communicate connection errors to
   the user. It also makes it difficult for Klippy to reconnect to the
   firmware and/or restart the firmware if it needs to.

 * Support clearing MCU shutdown events from the host without having
   to restart the firmware.

 * Allow loading of a new config without having to restart the mcu.

* Improve gcode interface:

 * Support ASCII based commands in addition to common gcode
   commands. It would be useful to support high-level commands such as
   "query_endstops", "pid_autotune", and "help" instead of having to
   invent cryptic gcode commands (eg, "M119" and "M303").

 * Provide a better way to handle print nozzle z offsets. The M206
   command is cryptic to use and it is too easy to set the value
   incorrectly or to forget to set it.

 * Provide a way to temporarily disable endstop checks so that a user
   can issue commands that potentially move the head past
   position_min/position_max.

* Improve logging:

 * Route errors from the host C code to the main log file.

 * Automatically roll Klippy log files. The default log file should
   have the current date in the log file name.

 * Report the Klippy git version in log file. Log the contents of the
   config file at startup.

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

* Verify the endstop is no longer triggered after retraction.

* Possibly implement host based checking on the ratio between extrude
  amount and head movement.
 * Enforce acceleration and speed limits on extruder stepper motor.

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

* Document the kinematic formulas in Klippy. Document how acceleration
  and jerk limits are enforced.

* Document how one can tune the pressure advance setting.

Hardware features
=================

* Support ADC on Arduino Due.

* Port firmware to more architectures:
 * Beagle Bone Black PRU
 * Smoothieboard / NXP LPC1769 (ARM cortex-M3)
 * Unix based scheduling; Unix based real-time scheduling

* Support for additional kinematics: delta, scara, corexy, etc.

* Support shared motor enable GPIO lines.

* Support for multiple extruders.

* Support for bed-level probes.

* Possible support for touch panels attached to the micro-controller.
  (In general, it would be preferable to attach touch panels to the
  host system and have octoprint interact with the panel directly, but
  it would also be useful to handle panels already hardwired to the
  micro-controller.)

* The raspberry pi has the ability to cut power to its USB ports. This
  feature is useful for resetting micro-controllers that are powered
  over USB. It would be useful to have a high-level command interface
  in Klippy to request a micro-controller reset via this mechanism.

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
