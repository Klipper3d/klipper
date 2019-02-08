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

#ifndef _SAMBA_H
#define _SAMBA_H

#include <string>
#include <stdint.h>
#include <exception>
#include <memory>

#include "SerialPort.h"

class SambaError : public std::exception
{
public:
    SambaError() : exception() {};
    const char* what() const throw() { return "SAM-BA operation failed"; }
};



class Samba
{
public:
    Samba();
    virtual ~Samba();

    bool connect(SerialPort::Ptr port, int bps = 115200);
    void disconnect();

    void writeByte(uint32_t addr, uint8_t value);
    uint8_t readByte(uint32_t addr);

    void writeWord(uint32_t addr, uint32_t value);
    uint32_t readWord(uint32_t addr);

    void write(uint32_t addr, const uint8_t* buffer, int size);
    void read(uint32_t addr, uint8_t* buffer, int size);

    void go(uint32_t addr);

    std::string version();

    void chipId(uint32_t& chipId, uint32_t& extChipId);

    void setDebug(bool debug) { _debug = debug; }

    const SerialPort& getSerialPort() { return *_port; }

    void reset();

    // Extended SAM-BA functions
    bool canChipErase() { return _canChipErase; }
    void chipErase(uint32_t start_addr);

    bool canWriteBuffer() { return _canWriteBuffer; }
    void writeBuffer(uint32_t src_addr, uint32_t dst_addr, uint32_t size);
    uint32_t writeBufferSize() { return 4096; }
    
    bool canChecksumBuffer() { return _canChecksumBuffer; }
    uint16_t checksumBuffer(uint32_t start_addr, uint32_t size);
    uint32_t checksumBufferSize() { return 4096; }
    uint16_t checksumCalc(uint8_t c, uint16_t crc);

private:
    bool _canChipErase;
    bool _canWriteBuffer;
    bool _canChecksumBuffer;
    int _readBufferSize;
    bool _debug;
    bool _isUsb;
    SerialPort::Ptr _port;

    bool init();

    uint16_t crc16Calc(const uint8_t *data, int len);
    bool crc16Check(const uint8_t *blk);
    void crc16Add(uint8_t *blk);
    void writeXmodem(const uint8_t* buffer, int size);
    void readXmodem(uint8_t* buffer, int size);

    void writeBinary(const uint8_t* buffer, int size);
    void readBinary(uint8_t* buffer, int size);

};


#endif // _SAMBA_H
