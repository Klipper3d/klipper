When considering implementing a filament sensor or encoder, it's important to understand the differences
between them and the pros and cons that exist because of them.

A filament switch is a simple microswitch or optical sensor that detects when there is no longer filament present
inside the switch housing. The switch used is always "normaly open", or "NO" so that the presense of the filament
causes a circuit to be made. If the filament runs out or, if the sensor is disconnected accidentally or stops working
the filament switch module will still see it the same way, as a suddenly open circuit, and raise a runout event.

Since this happens nearly instantly, the print can be recovered by adding new filament and resuming the print.

The switch falls short when there is a jam or line tangle that prevents the filament from feeding thru the nozzle of
the hotend. The filament is still present, just not moving, so the printer keeps running, nothing is extruded and the
filament slowly cooks and eventualy burns in the hot end.

A filament sensor, or encoder works by detecting the movement of the filament, and by extension, it's presense.
A optical sensor is commonly used, similar to what is in old-fashioned PC mice that use a roller ball. A wheel with
holes cut into its face is rotated past an optical sensor by the movement of the filament. The result is a series of
off and on signals from the encoder to the filament encoder module. When the module stops getting signals, a jam is
announced and the printer pauses. There are also detectors that use a laser more similar to modern mice to look at the
filament and detect movement of the filament in the same maner as an optical mouse does.

The problem with this method is that filament does not move at a constant rate. There is a finite amount of distance that
must be traveled by the filament before the encoder will trigger an event. As an example Big Tree Tech's filament sensor
sends a signal every 7mm. This may not sound like much until you are printing with a narrow nozzle at a low layer height
and low speed. This means that you have to wait for some time to pass before raising an alarm. Test prints at .2 layer height
with a .4 nozzle at 40mm/s needed a minimum of 9 seconds to pass before an event was credible, and in some cases, 18 seconds
was needed. The faster the print, the shorter the interval, but the same amount of filament (7mm in our example) is still lost
in the print.

For this reason, a filament encoder style sensor is useful for detecting jams and preventing burning and fouling of the nozzle,
but the print will not be recoverable in many or most cases because too much travel has occurred.

A complete solution would be to have one of each, to detect breaks and jams. If you have to pick one, pick the switch.
