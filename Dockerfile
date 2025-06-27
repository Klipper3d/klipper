## Get Klipper Source and Build venv
##
FROM python:3.11-bookworm AS build

RUN apt update \
 && apt install -y cmake \
 && apt clean

WORKDIR /opt
COPY ./requirements-prind.txt ./requirements-prind.txt

RUN python -m venv venv \
 && venv/bin/pip install -r ./requirements-prind.txt


COPY . ./klipper
RUN cd klipper \
 && echo -n $(git describe --tags)-voltera > klippy/.version \
 && rm -rf .git

RUN venv/bin/pip install -r klipper/scripts/klippy-requirements.txt \
 && venv/bin/python -m compileall klipper/klippy \
 && venv/bin/python klipper/klippy/chelper/__init__.py

###
## Klippy Runtime Image
##
FROM python:3.11-slim-bookworm AS run

WORKDIR /opt
RUN groupadd klipper --gid 1000 \
 && useradd klipper --uid 1000 --gid klipper \
 && usermod klipper --append --groups dialout \
 && usermod klipper --append --groups tty

RUN mkdir -p printer_data/run printer_data/gcodes printer_data/logs printer_data/config \
 && chown -R klipper:klipper /opt/*

COPY --chown=klipper:klipper health.py ./

COPY --chown=klipper:klipper --from=build /opt/klipper ./klipper
COPY --chown=klipper:klipper --from=build /opt/venv ./venv

USER klipper
VOLUME ["/opt/printer_data/run", "/opt/printer_data/gcodes", "/opt/printer_data/logs", "/opt/printer_data/config"]
ENTRYPOINT ["/opt/venv/bin/python", "klipper/klippy/klippy.py"]
CMD ["-I", "printer_data/run/klipper.tty", "-a", "printer_data/run/klipper.sock", "printer_data/config/printer.cfg"]
###


# ## Image for building MCU code including other tooling
# ##
# FROM debian:bookworm AS tools

# WORKDIR /opt
# ARG DEBIAN_FRONTEND=noninteractive
# RUN apt update \
#  && apt install -y \
#       sudo \
#       virtualenv python3-dev libffi-dev build-essential pkg-config\
#       libncurses-dev \
#       avrdude gcc-avr binutils-avr avr-libc \
#       stm32flash dfu-util libnewlib-arm-none-eabi \
#       gcc-arm-none-eabi binutils-arm-none-eabi libusb-1.0-0 libusb-1.0-0-dev \
#       python3-numpy python3-matplotlib \
#  && apt clean

# COPY --from=build /opt/klipper ./klipper
# COPY --from=build /opt/venv ./venv
# ###


# ## Build the klipper_mcu binary
# ##
# FROM tools AS build-hostmcu

# WORKDIR /opt

# COPY config.hostmcu klipper/.config
# RUN cd klipper \
#  && make flash
# ###


# ## Runtime image for the klipper_mcu binary
# ##
# FROM debian:bookworm-slim AS hostmcu

# RUN apt update \
#  && apt install -y gpiod \
#  && apt clean

# COPY --from=build-hostmcu /usr/local/bin/klipper_mcu /usr/local/bin/klipper_mcu

# VOLUME ["/opt/printer_data/run"]
# ENTRYPOINT ["/usr/local/bin/klipper_mcu"]
# CMD ["-I", "/opt/printer_data/run/klipper_host_mcu.tty"]
# ###


# ## Build Simulavr and klipper.elf
# ## For manual builds, not uploaded to the docker registry
# FROM tools AS build-simulavr

# RUN apt update \
#  && apt install -y \
#       git cmake swig \
#       python3-dev \
#  && apt clean

# WORKDIR /opt/klipper
# COPY config.simulavr .config
# RUN make

# WORKDIR /opt
# RUN mkdir run
# RUN groupadd simulavr --gid 1000 \
#  && useradd simulavr --uid 1000 --gid simulavr \
#  && usermod simulavr --append --groups dialout

# RUN git clone git://git.savannah.nongnu.org/simulavr.git \
#  && cd simulavr \
#  && git checkout release-1.1.0 \
#  && make python \
#  && make build \
#  && chown -R simulavr:simulavr /opt/simulavr

# USER simulavr
# ENV PYTHONPATH=/opt/simulavr/build/pysimulavr/
# VOLUME ["/opt/printer_data/run"]
# ENTRYPOINT ["klipper/scripts/avrsim.py"]
# CMD ["-p", "/opt/printer_data/run/simulavr.tty", "/opt/klipper/out/klipper.elf"]