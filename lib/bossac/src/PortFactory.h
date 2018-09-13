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
#ifndef _PORTFACTORY_H
#define _PORTFACTORY_H

#include <string>

#include "SerialPort.h"

class PortFactoryBase
{
public:
    PortFactoryBase() {}
    virtual ~PortFactoryBase() {}

    virtual std::string begin() = 0;
    virtual std::string end() = 0;
    virtual std::string next() = 0;
    virtual std::string def() = 0;

    virtual SerialPort::Ptr create(const std::string& name) = 0;
    virtual SerialPort::Ptr create(const std::string& name, bool isUsb) = 0;
};

#if defined(__WIN32__)
#include "WinPortFactory.h"
typedef WinPortFactory PortFactory;
#elif defined(__linux__)
#include "LinuxPortFactory.h"
typedef LinuxPortFactory PortFactory;
#elif defined(__APPLE__)
#include "OSXPortFactory.h"
typedef OSXPortFactory PortFactory;
#elif defined(__OpenBSD__) || defined(__FreeBSD__)
// This is likely to work (but not tested) for the other BSDs as well
#include "BSDPortFactory.h"
typedef BSDPortFactory PortFactory;
#else
#error "Platform is not supported"
#endif

#endif // _PORTFACTORY_H
