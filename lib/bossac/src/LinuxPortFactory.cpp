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
#include "LinuxPortFactory.h"
#include "PosixSerialPort.h"

#include <string.h>
#include <stdio.h>

#include <string>

LinuxPortFactory::LinuxPortFactory()
{
    _dir = opendir("/dev");
}

LinuxPortFactory::~LinuxPortFactory()
{
    if (_dir)
        closedir(_dir);
}

SerialPort::Ptr
LinuxPortFactory::create(const std::string& name)
{
    bool isUsb = false;

    if (name.find("ttyUSB") != std::string::npos ||
        name.find("ttyACM") != std::string::npos)
        isUsb = true;

    return create(name, isUsb);
}

SerialPort::Ptr
LinuxPortFactory::create(const std::string& name, bool isUsb)
{
    return SerialPort::Ptr(new PosixSerialPort(name, isUsb));
}

std::string
LinuxPortFactory::begin()
{
    if (!_dir)
        return end();

    rewinddir(_dir);

    return next();
}

std::string
LinuxPortFactory::next()
{
    struct dirent* entry;

    if (!_dir)
        return end();

    while ((entry = readdir(_dir)))
    {
        if (strncmp("ttyUSB", entry->d_name, sizeof("ttyUSB") - 1) == 0)
            return std::string(entry->d_name);
        else if (strncmp("ttyACM", entry->d_name, sizeof("ttyACM") - 1) == 0)
            return std::string(entry->d_name);
        else if (strncmp("ttyS", entry->d_name, sizeof("ttyS") - 1) == 0)
            return std::string(entry->d_name);
    }

    return end();
}

std::string
LinuxPortFactory::end()
{
    return std::string();
}

std::string
LinuxPortFactory::def()
{
    return std::string("/dev/ttyACM0");
}

