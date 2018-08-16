///////////////////////////////////////////////////////////////////////////////
// BOSSA
//
// Copyright (c) 2011-2018, ShumaTech
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the <organization> nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///////////////////////////////////////////////////////////////////////////////
#include "PosixSerialPort.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>

#include <string>

#ifndef B460800
#define B460800 460800
#endif
#ifndef B921600
#define B921600 921600
#endif

PosixSerialPort::PosixSerialPort(const std::string& name, bool isUsb) :
    SerialPort(name), _devfd(-1), _isUsb(isUsb), _timeout(0),
    _autoFlush(false)
{
}

PosixSerialPort::~PosixSerialPort()
{
    if (_devfd >= 0)
        ::close(_devfd);
}

bool
PosixSerialPort::open(int baud,
                      int data,
                      SerialPort::Parity parity,
                      SerialPort::StopBit stop)
{
    struct termios options;
    speed_t speed;
    // Try opening port assuming _name is full path. If it fails
    // try "/dev/" + _name
    _devfd = ::open(_name.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (_devfd == -1)
    {
        std::string dev("/dev/");
        dev += _name;
        _devfd = ::open(dev.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
        if (_devfd == -1)
            return false;
    }

    if (tcgetattr(_devfd, &options) == -1)
    {
        close();
        return false;
    }

    switch (baud)
    {
    case 1200:
        speed = B1200;
        break;
    case 9600:
        speed = B9600;
        break;
    case 19200:
        speed = B19200;
        break;
    case 38400:
        speed = B38400;
        break;
    case 57600:
        speed = B57600;
        break;
    case 115200:
        speed = B115200;
        break;
    case 230400:
        speed = B230400;
        break;
    case 460800:
        speed = B460800;
        break;
    case 921600:
        speed = B921600;
        break;
    default:
        close();
        return false;
    }

    if (cfsetispeed(&options, speed) || cfsetospeed(&options, speed))
    {
        close();
        return false;
    }

    options.c_cflag |= (CLOCAL | CREAD);

    switch (data)
    {
        case 8:
            options.c_cflag &= ~CSIZE;
            options.c_cflag |= CS8;
            break;
        case 7:
            options.c_cflag &= ~CSIZE;
            options.c_cflag |= CS7;
            break;
        default:
            close();
            return false;
    }

    switch (parity)
    {
        case SerialPort::ParityNone:
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~PARODD;
            options.c_iflag &= ~(INPCK | ISTRIP);
            break;
        case SerialPort::ParityOdd:
            options.c_cflag |= PARENB;
            options.c_cflag |= PARODD;
            options.c_iflag |= (INPCK | ISTRIP);
            break;
        case SerialPort::ParityEven:
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            options.c_iflag |= (INPCK | ISTRIP);
            break;
        default:
            close();
            return false;
    }

    switch (stop)
    {
    case StopBitOne:
        options.c_cflag &= ~CSTOPB;
        break;
    case StopBitTwo:
        options.c_cflag |= CSTOPB;
        break;
    default:
        close();
        return false;
    }

    // No hardware flow control
    options.c_cflag &= ~CRTSCTS;

    // No software flow control
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    // Raw input
    options.c_iflag &= ~(BRKINT | ICRNL);
    options.c_lflag &= ~(ICANON | IEXTEN | ECHO | ECHOE | ISIG);

    // Raw output
    options.c_oflag &= ~OPOST;

    // No wait time
    options.c_cc[VMIN]  = 0;
    options.c_cc[VTIME] = 0;

    if (tcsetattr(_devfd, TCSANOW, &options))
    {
        close();
        return false;
    }

    return true;
}

void
PosixSerialPort::close()
{
    if (_devfd >= 0)
        ::close(_devfd);
    _devfd = -1;
}

int
PosixSerialPort::read(uint8_t* buffer, int len)
{
    fd_set fds;
    struct timeval tv;
    int numread = 0;
    int retval;

    if (_devfd == -1)
        return -1;

    while (numread < len)
    {
        FD_ZERO(&fds);
        FD_SET(_devfd, &fds);

        tv.tv_sec  = _timeout / 1000;
        tv.tv_usec = (_timeout % 1000) * 1000;

        retval = select(_devfd + 1, &fds, NULL, NULL, &tv);

        if (retval < 0)
        {
            return -1;
        }
        else if (retval == 0)
        {
            return numread;
        }
        else if (FD_ISSET(_devfd, &fds))
        {
            retval = ::read(_devfd, (uint8_t*) buffer + numread, len - numread);
            if (retval < 0)
                return -1;
            numread += retval;
        }
    }

    return numread;
}

int
PosixSerialPort::write(const uint8_t* buffer, int len)
{
    if (_devfd == -1)
        return -1;

    int res = ::write(_devfd, buffer, len);
    // Used on macos to avoid upload errors
    if (_autoFlush)
        flush();
    return res;
}

int
PosixSerialPort::get()
{
    uint8_t byte;

    if (_devfd == -1)
        return -1;

    if (read(&byte, 1) != 1)
        return -1;

    return byte;
}

int
PosixSerialPort::put(int c)
{
    uint8_t byte;

    byte = c;
    return write(&byte, 1);
}

void
PosixSerialPort::flush()
{
    // There isn't a reliable way to flush on a file descriptor
    // so we just wait it out.  One millisecond is the USB poll
    // interval so that should cover it.
    usleep(1000);
}

bool
PosixSerialPort::timeout(int millisecs)
{
    _timeout = millisecs;
    return true;
}

void
PosixSerialPort::setDTR(bool dtr)
{
    if (_devfd == -1)
        return;

    int iFlags = TIOCM_DTR;

    ioctl(_devfd, (dtr ? TIOCMBIS : TIOCMBIC), &iFlags);
}

void
PosixSerialPort::setRTS(bool rts)
{
    if (_devfd == -1)
        return;

    int iFlags = TIOCM_RTS;

    ioctl(_devfd, (rts ? TIOCMBIS : TIOCMBIC), &iFlags);
}

void
PosixSerialPort::setAutoFlush(bool autoflush)
{
    _autoFlush = autoflush;
}
