# SPI Flash board definitions
#
# Copyright (C) 2021 Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

###########################################################
#
# Board Definitions
#
###########################################################

BOARD_DEFS = {
    'generic-lpc1768': {
        'mcu': "lpc1768",
        'spi_bus': "ssp1",
        "cs_pin": "P0.6"
    },
    'generic-lpc1769': {
        'mcu': "lpc1769",
        'spi_bus': "ssp1",
        "cs_pin": "P0.6"
    },
    'btt-skr-mini': {
        'mcu': "stm32f103xe",
        'spi_bus': "spi1",
        "cs_pin": "PA4"
    },
    'btt-skr-mini-v3': {
        'mcu': "stm32g0b1xx",
        'spi_bus': "spi1",
        "cs_pin": "PA4"
    },
    'btt-skr-mini-v3-b0': {
        'mcu': "stm32g0b0xx",
        'spi_bus': "spi1",
        "cs_pin": "PA4"
    },
    'flyboard-mini': {
        'mcu': "stm32f103xe",
        'spi_bus': "spi2",
        "cs_pin": "PB12",
        "current_firmware_path": "FLY.CUR"
    },
    'mks-robin-e3': {
        'mcu': "stm32f103xe",
        'spi_bus': "spi2",
        "cs_pin": "PA15",
        "conversion_script": "scripts/update_mks_robin.py",
        "firmware_path": "Robin_e3.bin",
        "current_firmware_path": "Robin_e3.cur"
    },
    'btt-octopus-f407-v1': {
        'mcu': "stm32f407xx",
        'spi_bus': "swspi",
        'spi_pins': "PC8,PD2,PC12",
        'cs_pin': "PC11",
        'skip_verify': True
    },
    'btt-octopus-f429-v1': {
        'mcu': "stm32f429xx",
        'spi_bus': "swspi",
        'spi_pins': "PC8,PD2,PC12",
        'cs_pin': "PC11",
        'skip_verify': True
    },
    'btt-octopus-f446-v1': {
        'mcu': "stm32f446xx",
        'spi_bus': "swspi",
        'spi_pins': "PC8,PD2,PC12",
        'cs_pin': "PC11",
        'skip_verify': True
    },
    'btt-skr-pro': {
        'mcu': "stm32f407xx",
        'spi_bus': "swspi",
        'spi_pins': "PA6,PB5,PA5",
        "cs_pin": "PA4"
    },
    'btt-gtr': {
        'mcu': "stm32f407xx",
        'spi_bus': "spi1",
        "cs_pin": "PA4"
    },
    'fysetc-spider': {
        'mcu': "stm32f446xx",
        'spi_bus': "spi1",
        "cs_pin": "PA4",
        "current_firmware_path": "OLD.BIN"
    },
    'btt-skr-se-bx': {
        'mcu': 'stm32h743xx',
        'spi_bus': 'spi3a',
        'cs_pin': 'PA15'
    },
    'btt-skr-3-h743': {
        'mcu': 'stm32h743xx',
        'spi_bus': 'swspi',
        'spi_pins': "PC8,PD2,PC12",
        'cs_pin': 'PC11',
        'skip_verify': True
    },
    'btt-skr-3-h723': {
        'mcu': 'stm32h723xx',
        'spi_bus': 'swspi',
        'spi_pins': "PC8,PD2,PC12",
        'cs_pin': 'PC11',
        'skip_verify': True
    },
    'creality-v4.2.2': {
        'mcu': "stm32f103xe",
        'spi_bus': "swspi",
        'spi_pins': "PC8,PD2,PC12",
        'cs_pin': "PC11",
        'skip_verify': True
    },
    'monster8': {
        'mcu': "stm32f407xx",
        'spi_bus': "spi3a",
        "cs_pin": "PC9"
    },
    'fly-gemini-v2': {
        'mcu': "stm32f405xx",
        'spi_bus': "spi1",
        "cs_pin": "PA4"
    },
    'fysetc-cheetah': {
        'mcu': "stm32f401xc",
        'spi_bus': "spi1",
        "cs_pin": "PA4",
        "current_firmware_path": "OLD.BIN"
    },
    'btt-skrat': {
        'mcu': "stm32g0b1xx",
        'spi_bus': "spi1",
        "cs_pin": "PB8"
    },
    'chitu-v6': {
        'mcu': "stm32f103xe",
        'spi_bus': "swspi",
        'spi_pins': "PC8,PD2,PC12",
        "cs_pin": "PC11",
        #'sdio_bus': 'sdio',
        "conversion_script": "scripts/update_chitu.py",
        "firmware_path": "update.cbd",
        'skip_verify': True
    }
}

###########################################################
#
# Board Definition Aliases
#
###########################################################

BOARD_ALIASES = {
    'btt-skr-v1.1': BOARD_DEFS['generic-lpc1768'],
    'btt-skr-v1.3': BOARD_DEFS['generic-lpc1768'],
    'btt-skr-v1.4': BOARD_DEFS['generic-lpc1768'],
    'mks-sgenl-v1': BOARD_DEFS['generic-lpc1768'],
    'mks-sbase': BOARD_DEFS['generic-lpc1768'],
    'smoothieboard-v1': BOARD_DEFS['generic-lpc1769'],
    'btt-skr-turbo-v1.4': BOARD_DEFS['generic-lpc1769'],
    'btt-skr-e3-turbo': BOARD_DEFS['generic-lpc1769'],
    'mks-sgenl-v2': BOARD_DEFS['generic-lpc1769'],
    'btt-skr-mini-v1.1': BOARD_DEFS['btt-skr-mini'],
    'btt-skr-mini-e3-v1': BOARD_DEFS['btt-skr-mini'],
    'btt-skr-mini-e3-v1.2': BOARD_DEFS['btt-skr-mini'],
    'btt-skr-mini-e3-v2': BOARD_DEFS['btt-skr-mini'],
    'btt-skr-mini-e3-v3': BOARD_DEFS['btt-skr-mini-v3'],
    'btt-skr-mini-e3-v3-b0': BOARD_DEFS['btt-skr-mini-v3-b0'],
    'btt-skr-mini-mz': BOARD_DEFS['btt-skr-mini'],
    'btt-skr-e3-dip': BOARD_DEFS['btt-skr-mini'],
    'btt002-v1': BOARD_DEFS['btt-skr-mini'],
    'creality-v4.2.7': BOARD_DEFS['creality-v4.2.2'],
    'btt-skr-2-f407': BOARD_DEFS['btt-octopus-f407-v1'],
    'btt-skr-2-f429': BOARD_DEFS['btt-octopus-f429-v1'],
    'btt-octopus-f407-v1.0': BOARD_DEFS['btt-octopus-f407-v1'],
    'btt-octopus-f407-v1.1': BOARD_DEFS['btt-octopus-f407-v1'],
    'btt-octopus-f429-v1.0': BOARD_DEFS['btt-octopus-f429-v1'],
    'btt-octopus-f429-v1.1': BOARD_DEFS['btt-octopus-f429-v1'],
    'btt-octopus-f446-v1.0': BOARD_DEFS['btt-octopus-f446-v1'],
    'btt-octopus-f446-v1.1': BOARD_DEFS['btt-octopus-f446-v1'],
    'btt-octopus-pro-f429-v1.0': BOARD_DEFS['btt-octopus-f429-v1'],
    'btt-octopus-pro-f446-v1.0': BOARD_DEFS['btt-octopus-f446-v1'],
    'btt-octopus-pro-h723-v1.1': BOARD_DEFS['btt-skr-3-h723'],
    'btt-skr-pro-v1.1': BOARD_DEFS['btt-skr-pro'],
    'btt-skr-pro-v1.2': BOARD_DEFS['btt-skr-pro'],
    'btt-gtr-v1': BOARD_DEFS['btt-gtr'],
    'mks-robin-e3d': BOARD_DEFS['mks-robin-e3'],
    'fysetc-cheetah-v2': BOARD_DEFS['fysetc-cheetah'],
    'fysetc-spider-v1': BOARD_DEFS['fysetc-spider'],
    'fysetc-s6-v1.2': BOARD_DEFS['fysetc-spider'],
    'fysetc-s6-v2': BOARD_DEFS['fysetc-spider'],
    'robin_v3': BOARD_DEFS['monster8'],
    'btt-skrat-v1.0': BOARD_DEFS['btt-skrat'],
    'chitu-v6': BOARD_DEFS['chitu-v6']
}

def list_boards():
    return sorted(list(BOARD_DEFS.keys()) + list(BOARD_ALIASES.keys()))

def lookup_board(name):
    name = name.lower()
    bdef = BOARD_ALIASES.get(name, BOARD_DEFS.get(name, None))
    if bdef is not None:
        return dict(bdef)
    return None
