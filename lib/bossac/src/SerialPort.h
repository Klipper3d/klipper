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
#ifndef _SERIALPORT_H
#define _SERIALPORT_H

#include <string>
#include <memory>
#include <stdint.h>

class SerialPort
{
public:
    SerialPort(const std::string& name) : _name(name) {}
    virtual ~SerialPort() {}

    enum Parity
    {
        ParityNone,
        ParityOdd,
        ParityEven,
    };

    enum StopBit
    {
        StopBitOne,
        StopBitOneFive,
        StopBitTwo,
    };

    virtual bool open(int baud = 115200,
                      int data = 8,
                      Parity parity = ParityNone,
                      StopBit stop = StopBitOne) = 0;
    virtual void close() = 0;

    virtual bool isUsb() = 0;

    virtual int read(uint8_t* data, int size) = 0;
    virtual int write(const uint8_t* data, int size) = 0;
    virtual int get() = 0;
    virtual int put(int c) = 0;

    virtual bool timeout(int millisecs) = 0;
    virtual void flush() = 0;
    virtual void setDTR(bool dtr) = 0;
    virtual void setRTS(bool rts) = 0;

    virtual std::string name() const { return _name; }

    typedef std::unique_ptr<SerialPort> Ptr;

protected:
    std::string _name;
};

#endif // _SERIALPORT_H
