# HX711 load cell sensor

This document describes HX711 load cell sensor module.

To use the HX711 load cell sensor, read
[Config Reference](Config_Reference.md#hx711) and
[G-Code documentation](G-Codes.md#hx711).

## Calibration procedure

To get sensor value you can use **QUERY_HX711** command in terminal.

1. Load the cell with a known weight.

2. Query the latest raw value.

3. Repeat steps 1 and 2 at least one time with a different weight.

4. When you have enought calibration points you can create the calibration.
The format is a list of <raw_value>,<value> pairs separated by semicolons ';'.
e.g.: 0,10;100;1010;1050;10510

5. Save the calibration data in config parameter `calib_data`.

The conversion is unit agnostic. This means you can convert the
raw values in grams, pounds, or even percentage if you like.

For example:
Let's assume you are weighing your filament spool with this sensor.
You gather calibration data with an empty spool(520) and with a full(18657) spool.
When you enter `520,0;18657,100` as `calib_data`, you will receive a
percentage value indicating how full the spool is.

## Example
A simple example to read the latest value of 'hx711 test':

```
[gcode_macro hx711]
gcode:
  {% set res = printer["hx711 test"] %}
  { action_respond_info(res.value) }
  { action_respond_info(res.raw_value) }
```
