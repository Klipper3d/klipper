# i2c_adc

Additional Documentation for the i2c_adc module (klippy/extras/i2c_adc.py).

This module creates a virtual chip that can be accessed as an adc pin.
# The adc pin can be accessed as "external_adc_name:"
e.g. in case of [i2c_adc my_adc] the pin can be accessed as "my_adc:"

The following ADCs should be compatible out of the box:

    MCP3421 - MCP3428
    ADS1013 - ADS1015

The module was tested with MCP3421 and ADS1015 on a raspberry pi.
Other Chips can be configured by changing the configuration bytes in the i2c_adc.py.

## Communication with the chip / Writing the configuration register

The information below describes how to configure the adc chips.

### ADS10XX: ##

The ADS10XX chip writes two bytes (sixteen bits) of information to the configuration register.

    # Second Byte: Address pointer
    # 0 0 0 0 0 0 P1 P2
    #             0  0: Conversion register
    #             0  1: Config register
    #             1  0: Lo_threshold
    #             1  1: Hi_threshold

    # Config register
    # 0  0  0  0  0  0  0  0
    # 15 14 13 12 11 10 9  8
    # OS [  MUX ] [ PGA  ] Mode
    # ----------------------
    # 0  0  0  0  0  0  0  0
    # 7  6  5  4  3  2  1  0
    # [  DR ] CM  CP CL [CQ]
    #
    # OS: Operational Status (write 1 for single shot)
    # MUX:                  000: AIN0+AIN1 (def)
    # Input Multiplexer     001: AIN0+AIN3
    # (ADS1015 only)        010: AIN1+AIN3
    #                       011: AIN2+AIN3
    #                       100: AIN0+GND
    #                       101: AIN1+GND
    #                       110: AIN2+GND
    #                       111: AIN3+GND
    # PGA:                  000: FSR = +- 6.144 V (Gain 0.25)
    # No function on        001: FSR = +- 4.096 V (Gain 0.5)
    # ADS1013               010: FSR = +- 2.048 V (def)
    #                       011: FSR = +- 1.024 V (Gain 2)
    #                       100: FSR = +- 0.512 V (Gain 4)
    #                       101: FSR = +- 0.256 V (Gain 8)
    #                       110: FSR = +- 0.256 V
    #                       111: FSR = +- 0.256 V
    # Mode:                 0: continuous conversion
    #                       1: single shot
    # DR:                    000: 128 SPS
    # Data rate             001: 250 SPS
    #                       010: 490 SPS
    #                       011: 920 SPS
    #                       100: 1600 SPS (def)
    #                       101: 2400 SPS
    #                       110: 3300 SPS
    #                       111: 3300 SPS


### MCP34XX: ##

The ADS10XX chip writes only one bytes (eight bits) of information to the configuration register.

    # 7th bit: OS (1 start new conversion, no function in continuous)
    # 6+5th bit: address bits (not used in mcp3425)
    # 4th bit: mode (0: one shot mode, 1: continuous)
    # 3rd+2nd bit: sample rate (10: 15 ms, 16 bit)
    # 1st+0th bit: gain selection (00: gain = 1)
