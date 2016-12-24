# Low level unix utility functions
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os, pty, fcntl, termios, signal, logging, subprocess, traceback, shlex

# Return the SIGINT interrupt handler back to the OS default
def fix_sigint():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
fix_sigint()

# Set a file-descriptor as non-blocking
def set_nonblock(fd):
    fcntl.fcntl(fd, fcntl.F_SETFL
                , fcntl.fcntl(fd, fcntl.F_GETFL) | os.O_NONBLOCK)

# Clear HUPCL flag
def clear_hupcl(fd):
    attrs = termios.tcgetattr(fd)
    attrs[2] = attrs[2] & ~termios.HUPCL
    termios.tcsetattr(fd, termios.TCSADRAIN, attrs)

# Support for creating a pseudo-tty for emulating a serial port
def create_pty(ptyname):
    mfd, sfd = pty.openpty()
    try:
        os.unlink(ptyname)
    except os.error:
        pass
    os.symlink(os.ttyname(sfd), ptyname)
    fcntl.fcntl(mfd, fcntl.F_SETFL
                , fcntl.fcntl(mfd, fcntl.F_GETFL) | os.O_NONBLOCK)
    old = termios.tcgetattr(mfd)
    old[3] = old[3] & ~termios.ECHO
    termios.tcsetattr(mfd, termios.TCSADRAIN, old)
    return mfd

def report_git_version():
    # Obtain version info from "git" program
    if not os.path.exists('.git'):
        logging.debug("No '.git' file/directory found")
        return
    prog = "git describe --tags --long --dirty"
    try:
        process = subprocess.Popen(shlex.split(prog), stdout=subprocess.PIPE)
        output = process.communicate()[0]
        retcode = process.poll()
    except OSError:
        logging.debug("Exception on run: %s" % (traceback.format_exc(),))
        return
    ver = output.strip()
    logging.info("Git version: %s" % (repr(ver),))
