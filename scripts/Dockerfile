# This is an example Dockerfile showing how it's possible to run Klipper in Docker.

# Building the Image
#
# Run 'docker build -f scripts/Dockerfile --target run -t klipper' from the root of this 
# Repository. This builds a runtime Image that contains klippy with its Dependencies. 
#
# Run 'docker build -f scripts/Dockerfile --target mcu -t klipper' from the root of this
# Repository to Build an Image that contains everything you need to build and flash your MCU.

# Building and flashing MCU code
#
# If you already have a build config, copy it to 'build.config' in your current directory or
# create an empty File named 'build.config'. Repace the serial port at '--device' with your MCUs
# Device.
# Running the following command will execute
# * make menuconfig
# * make
# * make flash
#
# docker run \
#   --rm \
#   --volume $(pwd)/build.config:/opt/klipper/.config \
#   --volume $(pwd)/out:/opt/klipper/out \
#   --interactive \
#   --tty \
#   --device /dev/ttyUSB0:/dev/ttyUSB0
#   klipper:mcu \
#     bash -c "cd /opt/klipper; make menuconfig && make && make flash"

# When intending to use this Image with moonraker, the serial Port of your Printer should
# be attached to the Container via the '--device' directive. 
# Adding the 'run' Volume to moonraker grants it access to the klippy-provided socket
# which is located in /opt/run/klipper.sock and needed for communication between both processes.
# Example:
#
# docker run klipper \
#   --device /dev/ttyUSB0:/dev/ttyUSB0 \
#   -v $(pwd)/config:/opt/cfg \
#   -v $(pwd)/gcode:/opt/gcode \
#   -v $(pwd)/run:/opt/run
#
# docker run moonraker \
#   --device /dev/ttyUSB0:/dev/ttyUSB0 \
#   -v $(pwd)/config:/opt/cfg \
#   -v $(pwd)/gcode:/opt/gcode \
#   -v $(pwd)/run:/opt/run \

# If you intend on using OctoPrint as your Webinterface, you may want to start
# this Container in privileged mode and ommit the '--device' directive.
# Running this Container in privileged mode bears security implications, as privileged
# Mode grants processes within the Container root access to devices of the Host machine.
# When Running OctoPrint also in privileged mode, it is possible for Octoprint to access
# the tty Interface created by klippy in /opt/run/klipper.tty as long as it also has 
# the 'run' Volume mounted.
# Example:
#
# docker run klipper \
#   --privileged \
#   -v $(pwd)/config:/opt/cfg \
#   -v $(pwd)/gcode:/opt/gcode \
#   -v $(pwd)/run:/opt/run \
#   -v /dev:/dev
#
# docker run octoprint/octoprint:slim \
#   --privileged \
#   -v $(pwd)/octoprint:/octoprint \
#   -v $(pwd)/run:/opt/run \
#   -v /dev:/dev

# See https://github.com/mkuf/prind for further examples

# Get Code and Build venv
FROM python:2 as build

WORKDIR /opt
COPY . klipper

RUN virtualenv -p python2 venv \
 && venv/bin/pip install -r klipper/scripts/klippy-requirements.txt \
 && venv/bin/python -m compileall klipper/klippy \
 && venv/bin/python klipper/klippy/chelper/__init__.py

# Runtime Image
FROM python:2-slim as run

WORKDIR /opt
COPY --from=build /opt/klipper ./klipper
COPY --from=build /opt/venv ./venv

RUN mkdir run cfg gcode
RUN groupadd klipper --gid 1000 \
 && useradd klipper --uid 1000 --gid klipper \
 && usermod klipper --append --groups dialout \
 && chown -R klipper:klipper /opt/*

USER klipper
VOLUME ["/opt/run", "/opt/cfg", "/opt/gcode"]
ENTRYPOINT ["/opt/venv/bin/python", "klipper/klippy/klippy.py"]
CMD ["-I", "run/klipper.tty", "-a", "run/klipper.sock", "cfg/printer.cfg"]

# For building MCU Code
FROM ubuntu:18.04 as mcu

WORKDIR /opt
COPY --from=build /opt/klipper ./klipper
COPY --from=build /opt/venv ./venv

RUN apt update \
 && apt install -y \
      virtualenv python-dev libffi-dev build-essential \
      libncurses-dev \
      libusb-dev \
      avrdude gcc-avr binutils-avr avr-libc \
      stm32flash libnewlib-arm-none-eabi \
      gcc-arm-none-eabi binutils-arm-none-eabi libusb-1.0 \
 && apt clean
