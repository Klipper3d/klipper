# Duet2 Wifi/Ethernet pinmap
#
# Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

duet2_pinmap = {
    # FANS
    'FAN0': 'PC23', 'FAN1': 'PC26', 'FAN2': 'PA0',
    # TMC2660 Drives
    'X_DIR': 'PD11', 'X_STEP': 'PD6', 'X_SPI_EN': 'PD14', 'X_STOP': 'PC14',
    'Y_DIR': 'PD12', 'Y_STEP': 'PD7', 'Y_SPI_EN': 'PC9', 'Y_STOP': 'PA2',
    'Z_DIR': 'PD13', 'Z_STEP': 'PD8', 'Z_SPI_EN': 'PC10', 'Z_STOP': 'PD29',
    'E0_DIR': 'PA1', 'E0_STEP': 'PD5', 'E0_SPI_EN': 'PC17', 'E0_STOP': 'PD10',
    'E1_DIR': 'PD9', 'E1_STEP': 'PD4', 'E1_SPI_EN': 'PC25', 'E1_STOP': 'PC16',
    'E2_DIR': 'PD28', 'E2_STEP': 'PD2', 'E2_SPI_EN': 'PD23', 'E2_STOP': 'PE0',
    'E3_DIR': 'PD22', 'E3_STEP': 'PD1', 'E3_SPI_EN': 'PD24', 'E3_STOP': 'PE1',
    'E4_DIR': 'PD16', 'E4_STEP': 'PD0', 'E4_SPI_EN': 'PD25', 'E4_STOP': 'PE2',
    'E5_DIR': 'PD17', 'E5_STEP': 'PD3', 'E5_SPI_EN': 'PD26', 'E5_STOP': 'PE3',
    'TMC_EN': 'PC6',
    # Thermistors
    'BED_TEMP': 'PC13', 'E0_TEMP': 'PC15', 'E1_TEMP': 'PC12',
    'E2_TEMP': 'PC29', 'E3_TEMP': 'PC30', 'E4_TEMP': 'PC31',
    'E5_TEMP': 'PC27', 'E6_TEMP' : 'PA18',
    # Heaters (HEATER0 = bed)
    'BED_HEAT': 'PA19', 'E0_HEAT': 'PA20', 'E1_HEAT': 'PA16', 'E2_HEAT': 'PC3',
    'E3_HEAT': 'PC5', 'E4_HEAT': 'PC8', 'E5_HEAT': 'PC11', 'E6_HEAT': 'PA15',
    # LCD
    'ENC_SW': 'PA7', 'ENC_A': 'PA8', 'ENC_B': 'PC7','LCD_E': 'PA25',
    'LCD_RS': 'PC28', 'LCD_DB7': 'PD18', 'LCD_DB6': 'PD19', 'LCD_DB5': 'PD20',
    'LCD_DB4': 'PD21',
    # SPI0 CS
    'SPI0_CS0': 'PC24', 'SPI0_CS1': 'PB2', 'SPI0_CS2': 'PC18',
    'SPI0_CS3': 'PC19', 'SPI0_CS4': 'PC20', 'SPI0_CS5' : 'PA24',
    'SPI0_CS6' : 'PE1', 'SPI0_CS7': 'PE2', 'SPI0_CS8': 'PE3',
    # Misc
    'ZPROBE_IN': 'PC1',  'PS_ON': 'PD15', 'LED_ONBOARD': 'PC2',
    'SX1509_IRQ': 'PA17', 'DUEX_SG_TST': 'PE0',
}

def update_pinmap(pins, mcu_type):
    if mcu_type != 'sam4e8e':
        raise error("Duet2 Wifi/Ethernet pinmap can only be used on sam4e8e boards")
    for pin, gpio in duet2_pinmap.items():
        pins[pin] = pins[gpio]
    return pins