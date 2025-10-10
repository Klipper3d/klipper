#!/usr/bin/env python3
# Tool to enter a USB bootloader and flash Klipper
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import sys, os, re, subprocess, optparse, time, fcntl, termios, struct

class error(Exception):
    pass

# Attempt to enter bootloader via 1200 baud request
def enter_bootloader(device):
    try:
        f = open(device, 'rb')
        fd = f.fileno()
        fcntl.ioctl(fd, termios.TIOCMBIS, struct.pack('I', termios.TIOCM_DTR))
        t = termios.tcgetattr(fd)
        t[4] = t[5] = termios.B1200
        sys.stderr.write("Entering bootloader on %s\n" % (device,))
        termios.tcsetattr(fd, termios.TCSANOW, t)
        fcntl.ioctl(fd, termios.TIOCMBIC, struct.pack('I', termios.TIOCM_DTR))
        f.close()
    except (IOError, OSError) as e:
        pass

# Translate a serial device name to a stable serial name in /dev/serial/by-path/
def translate_serial_to_tty(device):
    ttyname = os.path.realpath(device)
    if not os.path.exists('/dev/serial/by-path/'):
        raise error("Unable to find serial 'by-path' folder")
    for fname in os.listdir('/dev/serial/by-path/'):
        fname = '/dev/serial/by-path/' + fname
        if os.path.realpath(fname) == ttyname:
            return ttyname, fname
    return ttyname, ttyname

# Translate a serial device name to a usb path (suitable for dfu-util)
def translate_serial_to_usb_path(device):
    realdev = os.path.realpath(device)
    fname = os.path.basename(realdev)
    try:
        lname = os.readlink("/sys/class/tty/" + fname)
    except OSError as e:
        raise error("Unable to find tty device")
    ttypath_r = re.compile(r".*/usb\d+.*/(?P<path>\d+-[0-9.]+):\d+\.\d+/.*")
    m = ttypath_r.match(lname)
    if m is None:
        raise error("Unable to find tty usb device")
    devpath = os.path.realpath("/sys/class/tty/%s/device" % (fname,))
    return m.group("path"), devpath

# Wait for a given path to appear
def wait_path(path, alt_path=None):
    time.sleep(.100)
    start_alt_path = None
    end_time = time.time() + 4.0
    while 1:
        time.sleep(0.100)
        cur_time = time.time()
        if os.path.exists(path):
            sys.stderr.write("Device reconnect on %s\n" % (path,))
            time.sleep(0.100)
            return path
        if alt_path is not None and os.path.exists(alt_path):
            if start_alt_path is None:
                start_alt_path = cur_time
                continue
            if cur_time >= start_alt_path + 0.300:
                sys.stderr.write("Device reconnect on alt path %s\n" % (
                    alt_path,))
                return alt_path
        if cur_time > end_time:
            return path

CANBOOT_ID ="1d50:6177"

def detect_canboot(devpath):
    usbdir = os.path.dirname(devpath)
    try:
        with open(os.path.join(usbdir, "idVendor")) as f:
            vid = f.read().strip().lower()
        with open(os.path.join(usbdir, "idProduct")) as f:
            pid = f.read().strip().lower()
    except Exception:
        return False
    usbid = "%s:%s" % (vid, pid)
    return usbid == CANBOOT_ID

def call_flashcan(device, binfile):
    try:
        import serial
    except ModuleNotFoundError:
        sys.stderr.write(
            "Python's pyserial module is required to update. Install\n"
            "with the following command:\n"
            "   %s -m pip install pyserial\n\n" % (sys.executable,)
        )
        sys.exit(-1)
    args = [sys.executable, "lib/canboot/flash_can.py", "-d",
            device, "-f", binfile]
    sys.stderr.write(" ".join(args) + '\n\n')
    res = subprocess.call(args)
    if res != 0:
        sys.stderr.write("Error running flash_can.py\n")
        sys.exit(-1)

def flash_canboot(options, binfile):
    ttyname, pathname = translate_serial_to_tty(options.device)
    call_flashcan(pathname, binfile)

# Flash via a call to bossac
def flash_bossac(device, binfile, extra_flags=[]):
    ttyname, pathname = translate_serial_to_tty(device)
    enter_bootloader(pathname)
    pathname = wait_path(pathname, ttyname)
    baseargs = ["lib/bossac/bin/bossac", "-U", "-p", pathname]
    args = baseargs + extra_flags + ["-w", binfile, "-v"]
    sys.stderr.write(" ".join(args) + '\n\n')
    res = subprocess.call(args)
    if res != 0:
        raise error("Error running bossac")
    if "-R" not in extra_flags:
        args = baseargs + ["-b", "-R"]
        try:
            subprocess.check_output(args, stderr=subprocess.STDOUT)
            if "-b" not in extra_flags:
                wait_path(pathname)
                subprocess.check_output(args, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            pass

# Invoke the dfu-util program
def call_dfuutil(flags, binfile, sudo):
    args = ["dfu-util"] + flags + ["-D", binfile]
    if sudo:
        args.insert(0, "sudo")
    sys.stderr.write(" ".join(args) + '\n\n')
    res = subprocess.call(args)
    if res != 0:
        raise error("Error running dfu-util")

# Flash via a call to dfu-util
def flash_dfuutil(device, binfile, extra_flags=[], sudo=True):
    hexfmt_r = re.compile(r"^[a-fA-F0-9]{4}:[a-fA-F0-9]{4}$")
    if hexfmt_r.match(device.strip()):
        call_dfuutil(["-d", ","+device.strip()] + extra_flags, binfile, sudo)
        return
    ttyname, serbypath = translate_serial_to_tty(device)
    buspath, devpath = translate_serial_to_usb_path(device)
    enter_bootloader(device)
    pathname = wait_path(devpath)
    if detect_canboot(devpath):
        call_flashcan(serbypath, binfile)
    else:
        call_dfuutil(["-p", buspath] + extra_flags, binfile, sudo)

def call_hidflash(binfile, sudo):
    args = ["lib/hidflash/hid-flash", binfile]
    if sudo:
        args.insert(0, "sudo")
    sys.stderr.write(" ".join(args) + '\n\n')
    res = subprocess.call(args)
    if res != 0:
        raise error("Error running hid-flash")

# Flash via call to hid-flash
def flash_hidflash(device, binfile, sudo=True):
    hexfmt_r = re.compile(r"^[a-fA-F0-9]{4}:[a-fA-F0-9]{4}$")
    if hexfmt_r.match(device.strip()):
        call_hidflash(binfile, sudo)
        return
    ttyname, serbypath = translate_serial_to_tty(device)
    buspath, devpath = translate_serial_to_usb_path(device)
    enter_bootloader(device)
    pathname = wait_path(devpath)
    if detect_canboot(devpath):
        call_flashcan(serbypath, binfile)
    else:
        call_hidflash(binfile, sudo)

# Call Klipper modified "picoboot"
def call_picoboot(bus, addr, binfile, sudo):
    args = ["lib/rp2040_flash/rp2040_flash", binfile]
    if bus is not None:
        args.extend([bus, addr])
    if sudo:
        args.insert(0, "sudo")
    sys.stderr.write(" ".join(args) + '\n\n')
    res = subprocess.call(args)
    if res != 0:
        raise error("Error running rp2040_flash")

# Flash via Klipper modified "picoboot"
def flash_picoboot(device, binfile, sudo):
    ttyname, serbypath = translate_serial_to_tty(device)
    buspath, devpath = translate_serial_to_usb_path(device)
    # We need one level up to get access to busnum/devnum files
    usbdir = os.path.dirname(devpath)
    enter_bootloader(device)
    wait_path(usbdir + "/busnum")
    with open(usbdir + "/busnum") as f:
        bus = f.read().strip()
    with open(usbdir + "/devnum") as f:
        addr = f.read().strip()
    if detect_canboot(devpath):
        call_flashcan(serbypath, binfile)
    else:
        call_picoboot(bus, addr, binfile, sudo)


######################################################################
# Device specific helpers
######################################################################

def flash_atsam3(options, binfile):
    try:
        flash_bossac(options.device, binfile, ["-e", "-b"])
    except error as e:
        sys.stderr.write("Failed to flash to %s: %s\n" % (
            options.device, str(e)))
        sys.exit(-1)

def flash_atsam4(options, binfile):
    try:
        flash_bossac(options.device, binfile, ["-e"])
    except error as e:
        sys.stderr.write("Failed to flash to %s: %s\n" % (
            options.device, str(e)))
        sys.exit(-1)

def flash_atsamd(options, binfile):
    extra_flags = ["--offset=0x%x" % (options.start,), "-b", "-R"]
    try:
        flash_bossac(options.device, binfile, extra_flags)
    except error as e:
        sys.stderr.write("Failed to flash to %s: %s\n" % (
            options.device, str(e)))
        sys.exit(-1)

SMOOTHIE_HELP = """
Failed to flash to %s: %s

If flashing Klipper to a Smoothieboard for the first time it may be
necessary to manually place the board into "bootloader mode" - press
and hold the "Play button" and then press and release the "Reset
button".

When a Smoothieboard is in bootloader mode it can be flashed with the
following command:
  make flash FLASH_DEVICE=1d50:6015

Alternatively, one can flash a Smoothieboard via SD card - copy the
"out/klipper.bin" file to a file named "firmware.bin" on an SD card
and then restart the Smoothieboard with that SD card.

"""

def flash_lpc176x(options, binfile):
    try:
        flash_dfuutil(options.device, binfile, [], options.sudo)
    except error as e:
        sys.stderr.write(SMOOTHIE_HELP % (options.device, str(e)))
        sys.exit(-1)

STM32F1_HELP = """
Failed to flash to %s: %s

If the device is already in bootloader mode it can be flashed with the
following command:
  make flash FLASH_DEVICE=1eaf:0003
  OR
  make flash FLASH_DEVICE=1209:beba

If attempting to flash via 3.3V serial, then use:
  make serialflash FLASH_DEVICE=%s

"""

def flash_stm32f1(options, binfile):
    try:
        if options.start == 0x8000800:
            flash_hidflash(options.device, binfile, options.sudo)
        else:
            flash_dfuutil(options.device, binfile, ["-R", "-a", "2"],
                          options.sudo)
    except error as e:
        sys.stderr.write(STM32F1_HELP % (
            options.device, str(e), options.device))
        sys.exit(-1)

STM32F4_HELP = """
Failed to flash to %s: %s

If the device is already in bootloader mode it can be flashed with the
following command:
  make flash FLASH_DEVICE=0483:df11
  OR
  make flash FLASH_DEVICE=1209:beba

If attempting to flash via 3.3V serial, then use:
  make serialflash FLASH_DEVICE=%s

"""

def flash_stm32f4(options, binfile):
    start = "0x%x:leave" % (options.start,)
    try:
        if options.start == 0x8004000:
            flash_hidflash(options.device, binfile, options.sudo)
        else:
            flash_dfuutil(options.device, binfile,
                          ["-R", "-a", "0", "-s", start], options.sudo)
    except error as e:
        sys.stderr.write(STM32F4_HELP % (
            options.device, str(e), options.device))
        sys.exit(-1)

RP2040_HELP = """
Failed to flash to %s: %s

If the device is already in bootloader mode it can be flashed with the
following command:
  make flash FLASH_DEVICE=%s

Alternatively, one can flash rp2040 boards like the Pico by manually
entering bootloader mode(hold bootsel button during powerup), mount the
device as a usb drive, and copy klipper.uf2 to the device.

"""

def flash_rp2040(options, binfile):
    rawdev = "2e8a:0003"
    if options.mcutype == 'rp2350':
        rawdev = "2e8a:000f"
    try:
        if options.device.lower() == rawdev:
            call_picoboot(None, None, binfile, options.sudo)
        else:
            flash_picoboot(options.device, binfile, options.sudo)
    except error as e:
        sys.stderr.write(RP2040_HELP % (options.device, str(e), rawdev))
        sys.exit(-1)

MCUTYPES = {
    'sam3': flash_atsam3, 'sam4': flash_atsam4, 'same70': flash_atsam4,
    'samd': flash_atsamd, 'same5': flash_atsamd,
    'lpc176': flash_lpc176x, 'stm32f103': flash_stm32f1,
    'stm32f4': flash_stm32f4, 'stm32f042': flash_stm32f4,
    'stm32f070': flash_stm32f4, 'stm32f072': flash_stm32f4,
    'stm32g0b1': flash_stm32f4, 'stm32f7': flash_stm32f4,
    'stm32h7': flash_stm32f4, 'stm32l4': flash_stm32f4,
    'stm32g4': flash_stm32f4, 'rp2': flash_rp2040,
}


######################################################################
# Startup
######################################################################

def main():
    usage = "%prog [options] -t <type> -d <device> <klipper.bin>"
    opts = optparse.OptionParser(usage)
    opts.add_option("-t", "--type", type="string", dest="mcutype",
                    help="micro-controller type")
    opts.add_option("-d", "--device", type="string", dest="device",
                    help="serial port device")
    opts.add_option("-s", "--start", type="int", dest="start",
                    help="start address in flash")
    opts.add_option("--no-sudo", action="store_false", dest="sudo",
                    default=True, help="do not run sudo")
    options, args = opts.parse_args()
    if len(args) != 1:
        opts.error("Incorrect number of arguments")
    flash_func = None
    if options.mcutype:
        for prefix, func in MCUTYPES.items():
            if options.mcutype.startswith(prefix):
                flash_func = func
                break
    if flash_func is None:
        opts.error("USB flashing is not supported for MCU '%s'"
                   % (options.mcutype,))
    if not options.device:
        sys.stderr.write("\nPlease specify FLASH_DEVICE\n\n")
        sys.exit(-1)
    flash_func(options, args[0])

if __name__ == '__main__':
    main()
