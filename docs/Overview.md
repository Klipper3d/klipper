Welcome to the Klipper documentation. There are two parts to Klipper -
code that runs on a micro-controller and code that runs on a "host"
machine. The host code is intended to run on a low-cost
general-purpose machine such as a Raspberry Pi, while the
micro-controller code is intended to run on commodity micro-controller
chips. Read [features](Features.md) for reasons to use Klipper. See
[installation](Installation.md) to get started with Klipper. See
[config checks](Config_checks.md) for a guide to verify basic pin
settings in the config file.

The Klipper configuration is stored in a simple text file on the host
machine. The [config/example.cfg](../config/example.cfg) file serves
as a reference for the config file. The
[Pressure Advance](Pressure_Advance.md) document contains information
on tuning the pressure advance config.

The [kinematics](Kinematics.md) document provides some technical
details on how Klipper implements motion. The [FAQ](FAQ.md) answers
some common questions. The [G-Codes](G-Codes.md) document lists
currently supported run-time commands.

The history of Klipper releases is available at
[releases](Releases.md). See [contact](Contact.md) for information on
bug reporting and general communication with the developers.

Developer Documentation
=======================

There are also several documents available for developers interested
in understanding how Klipper works. Start with the
[code overview](Code_Overview.md) document - it provides information
on the structure and layout of the Klipper code. See the
[contributing](CONTRIBUTING.md) document to submit improvements to Klipper.

See [protocol](Protocol.md) for information on the low-level messaging
protocol between host and micro-controller. See also
[MCU commands](MCU_Commands.md) for a description of low-level
commands implemented in the micro-controller software.

See [debugging](Debugging.md) for information on how to test and debug
Klipper.

See [todo](Todo.md) for information on possible future code features.
