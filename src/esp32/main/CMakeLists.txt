# Set KLIPPER_ROOT to the root path of klipper itself
get_filename_component(KLIPPER_ROOT "../../.." ABSOLUTE)

set(SOURCES
    # @todo: desglosar en básicos para bootstrap, esp32 y adicionales
    # ESP32 sources
    ${KLIPPER_ROOT}/src/esp32/main.c
    ${KLIPPER_ROOT}/src/esp32/gpio/gpio.c
    ${KLIPPER_ROOT}/src/esp32/gpio/gpio_in.c
    ${KLIPPER_ROOT}/src/esp32/gpio/gpio_out.c
    ${KLIPPER_ROOT}/src/esp32/gpio/shift_register.c
    ${KLIPPER_ROOT}/src/esp32/irq.c
    ${KLIPPER_ROOT}/src/esp32/serial.c
    ${KLIPPER_ROOT}/src/esp32/timer.c

        #    # Klipper sources
#    ${KLIPPER_ROOT}/src/adccmds.c
    ${KLIPPER_ROOT}/src/basecmd.c
#    ${KLIPPER_ROOT}/src/buttons.c
    ${KLIPPER_ROOT}/src/command.c
#    ${KLIPPER_ROOT}/src/debugcmds.c
#    ${KLIPPER_ROOT}/src/endstop.c
    ${KLIPPER_ROOT}/src/gpiocmds.c
#    ${KLIPPER_ROOT}/src/i2ccmds.c
#    ${KLIPPER_ROOT}/src/i2c_software.c
#    ${KLIPPER_ROOT}/src/initial_pins.c
#    ${KLIPPER_ROOT}/src/lcd_hd44780.c
#    ${KLIPPER_ROOT}/src/lcd_st7920.c
#    ${KLIPPER_ROOT}/src/neopixel.c
#    ${KLIPPER_ROOT}/src/pulse_counter.c
#    ${KLIPPER_ROOT}/src/pwmcmds.c
    ${KLIPPER_ROOT}/src/sched.c
#    ${KLIPPER_ROOT}/src/sensor_adxl345.c
#    ${KLIPPER_ROOT}/src/sensor_angle.c
#    ${KLIPPER_ROOT}/src/sensor_bulk.c
#    ${KLIPPER_ROOT}/src/sensor_ldc1612.c
#    ${KLIPPER_ROOT}/src/sensor_lis2dw.c
#    ${KLIPPER_ROOT}/src/sensor_mpu9250.c
#    ${KLIPPER_ROOT}/src/spicmds.c
#    ${KLIPPER_ROOT}/src/spi_software.c
    ${KLIPPER_ROOT}/src/stepper.c
#    ${KLIPPER_ROOT}/src/thermocouple.c
#    ${KLIPPER_ROOT}/src/tmcuart.c
    ${KLIPPER_ROOT}/src/trsync.c
    ${KLIPPER_ROOT}/src/generic/alloc.c
    ${KLIPPER_ROOT}/src/generic/crc16_ccitt.c
    ${KLIPPER_ROOT}/src/generic/serial_irq.c
    ${KLIPPER_ROOT}/src/generic/timer_irq.c
)

idf_component_register(
    SRCS
        ${SOURCES}
    INCLUDE_DIRS
        ${KLIPPER_ROOT}/src
        ${KLIPPER_ROOT}/src/esp32
        ${KLIPPER_ROOT}/src/generic
        ${KLIPPER_ROOT}/out
    REQUIRES
        driver
        esp_adc
        esp_driver_gpio
        esp_system
    LDFRAGMENTS "${KLIPPER_ROOT}/src/esp32/ldfragments.lf"
    WHOLE_ARCHIVE
)

# CTR Magic

# Add a placeholder object file so that
#  1. Ninja does not complain about not knowing how to build the file
#  2. We overwrite the object to avoid stale entries from previous builds
execute_process(
    COMMAND sh -c "echo | '${CMAKE_C_COMPILER}' -x c -c -o '${KLIPPER_ROOT}/out/compile_time_request.o' -"
    COMMAND_ERROR_IS_FATAL ANY
    WORKING_DIRECTORY ${KLIPPER_ROOT}
)

# Build the actual CTR object file, right before linking the whole project
find_package(Python3 REQUIRED COMPONENTS Interpreter)
idf_component_get_property(CTR_INCLUDE_DIRS ${COMPONENT_NAME} INCLUDE_DIRS)
list(TRANSFORM CTR_INCLUDE_DIRS PREPEND "-I")

add_custom_command(
    TARGET ${COMPONENT_LIB}
    PRE_LINK
    COMMAND
        # Find object files and extract sections
        ${KLIPPER_ROOT}/scripts/ctr/find_object_files "${KLIPPER_ROOT}/out/build"
            | ${KLIPPER_ROOT}/scripts/ctr/extract_sections "${CMAKE_OBJCOPY}"
            > "${KLIPPER_ROOT}/out/compile_time_request.txt" &&
        # Generate dictionary and C file
        ${Python3_EXECUTABLE} ${KLIPPER_ROOT}/scripts/buildcommands.py
            -d "${KLIPPER_ROOT}/out/klipper.dict"
            -t "${CMAKE_C_COMPILER};${CMAKE_ASM_COMPILER};${CMAKE_LINKER};${CMAKE_OBJCOPY};${CMAKE_OBJDUMP};${CMAKE_STRIP}"
            "${KLIPPER_ROOT}/out/compile_time_request.txt"
            "${KLIPPER_ROOT}/out/compile_time_request.c" &&
        # Build C file (it is linked by ESP-IDF in a later stage)
        ${CMAKE_C_COMPILER} -mlongcalls ${CTR_INCLUDE_DIRS}
            -c "${KLIPPER_ROOT}/out/compile_time_request.c"
            -o "${KLIPPER_ROOT}/out/compile_time_request.o"
    WORKING_DIRECTORY ${KLIPPER_ROOT}
    COMMENT "Generating compile_time_request.c from object files"
    VERBATIM
)

# Add the built library to the to-be-linked files
add_prebuilt_library(ctr "${KLIPPER_ROOT}/out/compile_time_request.o")
target_link_libraries(${COMPONENT_LIB} PRIVATE ctr)
