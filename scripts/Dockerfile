# Find the run target below for an example showing how it's possible to install Klipper in Docker.

# But first, here is the docker builder environment target. Use this to build klipper from any
# platform without installing locally the required packages and compilers. This is especially usefull
# on old linux distribution and on non-linux OS'es.
# IMPORTANT: The docker build must be run from the root of the repo. To use docker builder env
# either prefix your command with ./scripts/with-docker-builder-env.sh like this:
#       ./scripts/with-docker-builder-env.sh make menuconfig
# or invoke it manually like so:
#       docker build . -f scripts/Dockerfile --target builder -t klipper-builder
#       docker run -it --rm -u `id -u`:`id -g` -v `pwd`:`pwd`:rw -w `pwd` klipper-builder make menuconfig
FROM ubuntu:20.04 as builder
ENV DEBIAN_FRONTEND=noninteractive
# TZ hack to bypass timezone selection prompt
ENV TZ=GMT
RUN ln -snf "/usr/share/zoneinfo/$TZ" /etc/localtime
RUN echo "$TZ" > /etc/timezone
# Install klipper build deps
RUN apt-get update && apt-get install -y --no-install-recommends \
        avr-libc \
        binutils-arm-none-eabi \
        bison \
        build-essential \
        flex \
        gcc-arm-none-eabi \
        gcc-avr \
        libffi-dev \
        libgmp-dev \
        libmpc-dev \
        libmpfr-dev \
        libnewlib-arm-none-eabi \
        pv \
        python2-dev \
        texinfo \
        virtualenv \
    && rm -rf /var/lib/apt/lists/*
# Install gnupru toolchain
RUN apt-get update && apt-get install -y --no-install-recommends \
        curl \
        git \
        wget \
    && rm -rf /var/lib/apt/lists/*
RUN cd /usr/local/src/ && \
    git config --global user.email "you@example.com" && \
    git config --global user.name "Your Name" && \
    git clone https://github.com/dinuxbg/gnupru -b 2023.01 --depth 1 && \
    cd gnupru && \
    ./download-and-prepare.sh 2>&1 && \
    PREFIX=/usr/local ./build.sh 2>&1 && \
    rm -rf /usr/local/src/gnupru
# Install or1k-linux-musl toolchain
RUN cd /usr/local/src/ && \
    curl https://more.musl.cc/10/x86_64-linux-musl/or1k-linux-musl-cross.tgz -o or1k-linux-musl-cross-10.tgz && \
    tar -xf or1k-linux-musl-cross-10.tgz -C /usr/local/ && \
    rm or1k-linux-musl-cross-10.tgz
ENV PATH="${PATH}:/usr/local/or1k-linux-musl-cross/bin"


# This is an example Dockerfile showing how it's possible to install Klipper in Docker.
# IMPORTANT: The docker build must be run from the root of the repo, either copy the
# Dockerfile to the root, or run docker build with "-f", for example:
#       docker build . -f scripts/Dockerfile -t klipper
# Note that the host still needs to run Linux to connect the printers serial port to
# the container.
# When running, the serial port of your printer should be connected, including an
# argument such as:
#       --device /dev/ttyUSB0:/dev/ttyUSB0
# It's also required that your control program (eg: OctoPrint) be included in the same
# container as Docker does not allow sharing of the virtual serial port outside the
# container.
# The config should be in a file named "printer.cfg" in a directory mounted at:
#       /home/klippy/.config/
# For more Dockerfile examples with Klipper (and Octoprint) see:
# https://github.com/sillyfrog/OctoPrint-Klipper-mjpg-Dockerfile
FROM ubuntu:18.04 as run

RUN apt-get update && \
    apt-get install -y sudo

# Create user
RUN useradd -ms /bin/bash klippy && adduser klippy dialout
USER klippy

#This fixes issues with the volume command setting wrong permissions
RUN mkdir /home/klippy/.config
VOLUME /home/klippy/.config

### Klipper setup ###
WORKDIR /home/klippy

COPY . klipper/
USER root
RUN echo 'klippy ALL=(ALL:ALL) NOPASSWD: ALL' > /etc/sudoers.d/klippy && \
    chown klippy:klippy -R klipper
# This is to allow the install script to run without error
RUN ln -s /bin/true /bin/systemctl
USER klippy
RUN ./klipper/scripts/install-ubuntu-18.04.sh
# Clean up install script workaround
RUN sudo rm -f /bin/systemctl

CMD ["/home/klippy/klippy-env/bin/python", "/home/klippy/klipper/klippy/klippy.py", "/home/klippy/.config/printer.cfg"]
