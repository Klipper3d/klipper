<!--
Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>

This file may be distributed under the terms of the GNU GPLv3 license.
-->

# PID_TUNE - PID autotune

This command initiates a process of heating and cooling to determine the proper PID values for the specified hotend or the heated bed.

The result of the autotune is written into /tmp/klippy.log


### Usage

``` PID_TUNE [E<index>] [S<temp>] ```

| Argument     | Description |
| ------------ | ----------- |
| `[E<index>]` | Index of the heater |
| `[S<temp>]`  | Target temperature in degrees Celcius |