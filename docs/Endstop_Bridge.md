# Endstop Bridge

Endstop Bridge sends an endstop or probe trigger directly from one MCU to
another over a GPIO signal wire. The MCU controlling the motors receives
the trigger and stops the associated steppers locally, without waiting for
the host to relay the stop notification.

The goal is to reduce cross-MCU homing and probing latency and improve
reliability when the host has limited processing capacity or is heavily
loaded. Multiple trigger sources can also share the same wire, **one at a
time**, reducing wiring and GPIO usage.

## How it works

Consider a printer with X, Y and Z switches connected to a toolhead MCU,
while the motors are controlled by the main MCU:

```text
                 toolhead mcu                          main mcu
          +------------------------+          +------------------------+
X endstop-+-> input_x_pin          |          |                        |
Y endstop-+-> input_y_pin          |          |                        |
Z endstop-+-> input_z_pin          |          |                        |
          |           |            |          |                        |
          |   Select ONE input     |          |                        |
          |           v            | ONE wire |                        |
          |      output_pin -------+----------+-> receive_pin          |
          |                        |          |           v            |
          |                        |          | Stop homing motor(s)   |
          +------------------------+          +------------------------+
```

For an X homing move:

1. Before motion, the bridge checks that the receiver can read both active
   and inactive levels on the wire.
2. The toolhead MCU selects the X switch as the source.
3. When the switch triggers, the toolhead MCU changes the wire's level.
4. The main MCU detects that level and stops the steppers associated with
   X homing.
5. The bridge is released when the operation finishes.

Y and Z can then use the same wire during their own homing moves. Selection
is automatic; no manual source-selection G-code is needed.

The wire carries only the selected source's trigger state. It does not
identify an axis, transmit pressure readings, or carry several independent
signals at once. There is no fixed three-input limit.

## Wiring requirements

- All digital inputs sharing a bridge must connect to the sending MCU.
- Connect the sending MCU's output GPIO to an input GPIO on the receiving
  MCU. Verify compatible voltage levels and a common ground.
- All steppers stopped by a bridged endstop must be controlled by the
  receiving MCU. For CoreXY homing, this includes both participating motors.
- Keep the normal power and MCU communication connections. "One wire"
  refers only to the additional trigger signal.
- Reserve the bridge pins for this feature; do not assign them to another
  output, probe module, or direct endstop configuration.
- Both MCUs and Klippy must run versions that include Endstop Bridge.

## Example: X, Y and Z sharing one wire

This example assumes a sender named `toolhead` and the default `mcu` as
the receiver. Connect sender PA15 to receiver PC7. Switches are connected
to ground when triggered, so the input examples use pull-ups and inversion.

**The pins are illustrative. Choose pins and polarities for your hardware.**
Merge these fragments into your existing configuration; keep the remaining
MCU and stepper settings and do not duplicate existing sections.

```ini
[endstop_bridge sync]
input_x_pin: ^!toolhead:PB0
input_y_pin: ^!toolhead:PB1
input_z_pin: ^!toolhead:PB2
output_pin: !toolhead:PA15
receive_pin: ^!PC7

[stepper_x]
endstop_pin: bridge:sync:x

[stepper_y]
endstop_pin: bridge:sync:y

[stepper_z]
endstop_pin: bridge:sync:z
```

Here, `sync` names the shared connection, and `x`, `y` and `z` name
its selectable inputs. `!` inverts a pin's logical level and `^` enables
an input pull-up. Do not add these modifiers to the virtual endstop names.

The option format is `input_<name>_pin`. Input names contain lowercase
letters, digits and underscores; they are labels, not a fixed XYZ list.
For example, add `input_aux_pin: ^!toolhead:PB3` and reference it as
`bridge:sync:aux`. Each input must use its own source GPIO. Bridge names
contain letters, digits, underscores or hyphens. List inputs first, then
the output and receiver, to make the signal direction easy to follow;
option order does not change behavior.

Home the axes in separate moves: `G28 X`, then `G28 Y`, then `G28 Z`.
A combined `G28` is suitable only if the printer's homing sequence uses
the shared inputs one at a time. Concurrent use is rejected. Delta homing
and independently triggered multi-Z alignment cannot share one bridge
when they need to monitor multiple endstops simultaneously.

### Optional sampling settings

Normal configurations need none of these options. Advanced users can
override them in the bridge section (sender settings apply to all its
digital inputs):

```ini
[endstop_bridge sync]
# ... input_<name>_pin, output_pin and receive_pin ...
poll_interval: 0.000050
period: 0.000050
filter_count: 2
```

- `period`: sender input sampling interval in seconds; default 50 us,
  allowed range 10 us to 10 ms.
- `filter_count`: consecutive active samples required to assert the output;
  default 2, allowed range 1 to 255. One inactive sample clears it.
- `poll_interval`: receiver idle sampling interval in seconds; default
  50 us, allowed range 10 us to 10 ms. Native endstop confirmation sampling
  also applies.

Sampling and filtering add latency. Digital inputs are not latched, so
pulses shorter than the sampling and filtering window may be missed.

## Sharing the wire with a load-cell probe

A native Klipper `load_cell_probe` can use the same bridge as the digital
inputs. For example, X and Y can use switches while Z uses a pressure probe.

Keep the bridge and X/Y inputs from the example above. Omit the digital
`input_z_pin` option, and merge these settings into an
otherwise configured and calibrated load-cell probe:

```ini
[load_cell_probe]
# Keep the sensor, calibration, z_offset and force-limit settings.
trigger_bridge: sync

[stepper_z]
endstop_pin: probe:z_virtual_endstop
```

The load-cell ADC and bridge output must be on the same MCU. Once the native
probe logic detects contact, the sender asserts the wire and the receiver
stops the probe move. Sensor faults and analog trigger errors also assert
the wire; the host subsequently checks the error so it is not reported as
a successful contact.

Native sampling, filtering, tare and calibration remain in use. Pressure
readings still travel over the normal communication link. Omitting
`trigger_bridge` retains the normal probe synchronization path. This
feature does not include Creality's PRTouch algorithm.

## Checking the connection

With the printer stationary, run:

```text
TEST_ENDSTOP_BRIDGE BRIDGE=sync
```

This drives the wire to both logical levels and checks the receiver. It
does not move motors, test the switches, or calibrate the load cell.
The same two-level check runs before a bridged homing or probing move.
Do not connect an actuator or another device that could react dangerously
to these test levels.

`QUERY_ENDSTOPS` reads the individual digital inputs without changing
the selected source. It reads each registered input's current logical GPIO
level, not its homing filter history, and does not verify the bridge wire.
The load-cell probe does not implement `QUERY_PROBE` in this version.
Its virtual Z endstop may report `open` as a placeholder, which must not be
interpreted as a measurement of pressure or proof that contact is absent.

Before using the bridge, verify pin polarity and both wire levels with
motors disabled. Then test each axis separately at low speed with clearance
and an operator ready to stop the printer.

## Limitations

The bridge is not a safety-rated link. A wire break or sender power loss
after the connection check may prevent a trigger from reaching the receiver.
Host coordination and communication watchdogs remain necessary.

## Related implementations

Creality's K1, K1 Max, K1C, K1 SE and CR-10 SE use GPIO signaling for PRTouch
pressure triggers. Creality Hi uses `io_remap` for a digital endstop.
These are examples of hardware trigger transport, not claims that all
these printers support shared XYZ inputs.

- [K1 series firmware](https://github.com/CrealityOfficial/K1_Series_Klipper/blob/main/src/prtouch_v2.c)
- [CR-10 SE configuration](https://github.com/CrealityOfficial/CR-10SE_Klipper/blob/main/config/F003/printer.cfg)
- [Hi configuration](https://github.com/CrealityOfficial/Hi_Klipper/blob/main/config/F018_CR4NU200360C20/printer.cfg)
