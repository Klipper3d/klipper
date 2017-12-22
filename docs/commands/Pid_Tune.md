# PID_TUNE - PID autotune

This command initiates a process of heating and cooling to determine the proper PID values for the specified hotend or the heated bed.

The result of the autotune is written into /tmp/klippy.log


### Usage

``` PID_TUNE [E<index>] [S<temp>] ```

| Argument     | Description |
| ------------ | ----------- |
| `[E<index>]` | Index of the heater |
| `[S<temp>]`  | Target temperature in degrees Celcius |