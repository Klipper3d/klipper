# BDsensor 
  Bed Distance Sensor 

**1. Connecting BDsensor** 

1.1 Attach the sensor cable to the mainboard or the CAN bus toolhead board.

the wires CKL and SDA of BDsensor can be connected to any GPIO pins of your board. You can also connect the BDsensor cable into the EXP1 or Bltouch port directly (except all the boards from MKS), for example:
```
BLtouch    |    BDsensor
 GND      -->     GND
 5V       -->     5V
 S        -->     CLK/SCL    (Input)
 GND      -->     GND
 Zmin     -->     SDA    (Input/Output) 
```  
Since some of the pins in the connectors on the mainboard maybe not connected to the gpios of MCU directly (e.g. there maybe a filter capacitor on them or isolated by mosfet, diode or optocoupler, but it's OK if they are isolated by resistors or pullup/pulldown by resistors), they cannot work with BDsensor. and the firmware will report connection error. For example 
* The connectors for FANs and Heaters are isolated by mosfet,  
* The connectors for temperature thermistors and Endstops/Probe in some boards like MKS are normally connected to GND by filtered capacitors, 

1.2 Mount the BDsensor near to the hotend, verify that the BDsensor is mounted at the correct height, the coil of the sensor should be roughly 0.4mm to 2 mm above the nozzle.

**3. Configuration** 
 
*  Copy the section `[BDsensor]` into your printer.cfg and modify the `sda_pin` and `scl_pin` to yours, also do not forgot to disable other probe section like BLtouch,Don't use aliases for the board pins.
```
[BDsensor] 
sda_pin: PB1 # example of connecting to Creality V4.2.7
scl_pin: PB0
#scl_pin:MKS_THR:gpio20 # example of connecting to CAN module like MKS THR42 
#sda_pin:MKS_THR:gpio11 
#scl_pin = host:gpio17 # example of connecting to GPIO on RaspberryPi
#sda_pin = host:gpio27
delay: 20 # 20us per pulse, this value should be >=18 but must be below 30
z_offset:0 # this `z_offset` must be set to 0. 
z_adjust:0.0 # z axis adjustment, replace the z_offset function. within -0.3 to 0.3mm
x_offset: -34
y_offset: 0
no_stop_probe: # fast probe that the toolhead will not stop at the probe point,disable it by commenting out.
position_endstop: 0.8 #the Z axis will be triggered after it measured the distance is below this value(mm) while homing z, recommend value is 0.5~1.0
#speed:0.8 # this speed only works for the z tilt and PROBE_ACCURACY command.
```

* Modify the endstop_pin in section `[stepper_z]`, and disable the `position_endstop`, Slow down the speed of z homing,
e.g.  `homing_speed: 5 `, `second_homing_speed: 1`

```
[stepper_z]
endstop_pin: probe:z_virtual_endstop
#position_endstop: 0.5
homing_speed: 5
second_homing_speed: 1
...
```

* Make sure there are `[safe_z_home]`,`[bed_mesh]`, `[force_move]` in your printer.cfg . 

Modify the `horizontal_move_z` to 1(0.7~1.0mm is recommended) in section `[bed_mesh]` and `[quad_gantry_level]`.

```
[safe_z_home]
...

[bed_mesh]
horizontal_move_z:1 # 0.7~1.0mm is recommended
algorithm: bicubic  # this should be bicubic if the bed mesh points count is > 6*6
...

[quad_gantry_level]
horizontal_move_z:1 # 0.7~1.0mm is recommended
...

[force_move] 
enable_force_move: true   # required by the command SET_KINEMATIC_POSITION in the calibration step below.

```

**4. After installing, please check it by sending the following gcode command**
```
BDSENSOR_VERSION  or M102   S-1     # Read sensor information
BDSENSOR_DISTANCE or M102   S-2     # Read one distance value
```
Check the connection by `M102 S-1`. Here is an example of the return message
```
Send: M102 S-1
Recv: V1.0 pandapi3d.com
```
if it return blank or 1024, please check the connection and wire order, check the schematic to make sure there is no signal filter capacitor or diode on the gpio.

**5. Calibration:**

 * Clean the nozzle, send command `SET_KINEMATIC_POSITION Z=200 ` then move the Z axis down in the menu until the nozzle just touches the bed plate (The BDsensor will use this position as the zero position, so the `z_offset` is no needed that's the reason the value is 0 in the section [BDsensor]).

 * Sending gcode command `M102 S-6`. 

 * After that you can check whether the BDsensor has been calibrated successful by `M102 S-5` that will return the raw calibration data which stored in the BDsensor. If the first raw calibration data returned by the M102 S-5 is greater than 400, that means the sensor is mounted too high and needs to be remounted closer to the bed, the recommend value of first data is around 100. also make sure that the value of second data is greater than the first data by more than 10, else adjust the height of z and do calibarte again.


**6. Check with Endstop**

Please do not Homing Z until you have checked this step or else the nozzle maybe cashes the printer bed.

This is the return message after sending M119 command(Reporting endstop status).
```
Send: M119
Recv: x:open y:open z:open
```

  Put a metal piece close enough to the sensor, send M119 again.
 
```
Send: M119
Recv: x:open y:open z:TRIGGERED
```
  If all above steps are right, then you can homing z axis now.

  **7. Check Accuracy:**

  * Move the z axis to 1mm after homing then read the distance value from BDsensor by M102 S-2, the returned value should be about 1mm too, e.g. 0.98~1.02mm.
```
G28
G1 Z1
```
waiting the toolhead stops then 
```
M102 S-2
```

 * Check if the zero position is as expected by `G1 Z0` after ` G28 `, if not please calibrate again.

**Gcode Command for BDsensor:**
```
BDSENSOR_VERSION                   # equals M102 S-1 , read the version of BDsensor
BDSENSOR_CALIBRATE                 # equals M102 S-6 , calibrate the BDsensor
BDSENSOR_READ_CALIBRATION          # equals M102 S-5 , read the raw calibrate data
BDSENSOR_DISTANCE                  # equals M102 S-2 , read the distance value measured by BDsensor
BDSENSOR_SET                       # set value, now only for setting z_adjust, for example BDSENSOR_SET z_adjust=0.1
```

**Note**: 

* If the first raw calibration data returned by the M102 S-5 is greater than 400, that means the sensor is mounted too high and needs to be remounted closer to the bed, the recommend value of first data is 100. also make sure that the value of second data is greater than the first data by more than 10
    + `FAQ: if calibration data begins with 1 and second value is 9 and third 24 what that means?`
    - `that means the resolution between 0-0.1mm is only 9,and the 0.1-0.2mm is 15.so recommend to calibate again let the first resolution 0-0.1mm bigger than 10`

* Please don't forget to run G28 after `Z_tilt` or `quad_gantry_level` for those commands will adjust the Z axis height.

* Gotta capitalized and lowercase the section name `[BDsensor]` correctly else the klipper will report `Unknown pin chip name 'probe'`

<details ><summary >$\color[RGB]{55,27,200} FAQ$</summary>

1. `Unknown pin chip name 'probe'`

* Check if there is a file `BDsensor.py` in the `~/klipper/klippy/extras/`, if not please install again by running `cd  ~/Bed_Distance_sensor/klipper/` and `./install_BDsensor.sh` 

* One common reason is the updating of Fluidd or Mainsail that will remove the BDsensor.py file in Klipper, so need to run `./install_BDsensor.sh` again.

* Check the section name `[BDsensor]` is correct, note the capitalized and lowercase.
 
2. Report: ` Bed Distance Sensor, out of range.:3.90` while doing `quad_gantry_level` after `Z_tilt`

* Please home again the printer because the Z_tilt has adjusted the Z axis height.
 
3. Report : ` Probe triggered prior to movement ` while doing `Z_tilt`

* Please increase the `horizontal_move_z` in the section `[z_tilt]`, that will move the z higher before probing.

4. The nozzle cashes to the bed while homing

* Make sure you have check the endstop by M119.
* Make sure the BDsensor has been calibrated again after install.
* The BDsensor is used as the default homing sensor, do not homing with other endstop.

5. Report Error while G28: ` Unknown value 'probe' in enumeration 'pin'`


</details>


