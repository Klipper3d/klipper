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
#ifndef _EFCFLASH_H
#define _EFCFLASH_H

#include <stdint.h>
#include <exception>

#include "Flash.h"

class EfcFlash : public Flash
{
public:
    EfcFlash(Samba& samba,
             const std::string& name,
             uint32_t addr,
             uint32_t pages,
             uint32_t size,
             uint32_t planes,
             uint32_t lockRegions,
             uint32_t user,
             uint32_t stack,
             bool canBootFlash);
    virtual ~EfcFlash();

    void eraseAll(uint32_t offset);
    void eraseAuto(bool enable);

    std::vector<bool> getLockRegions();

    bool getSecurity();

    bool getBod();
    bool canBod() { return true; }

    bool getBor();
    bool canBor() { return true; }

    bool getBootFlash();
    bool canBootFlash() { return _canBootFlash; }

    void writeOptions();

    void writePage(uint32_t page);
    void readPage(uint32_t page, uint8_t* data);

private:
    bool _canBootFlash;

    void waitFSR(int seconds = 1);
    void writeFCR0(uint8_t cmd, uint32_t arg);
    void writeFCR1(uint8_t cmd, uint32_t arg);
    uint32_t readFSR0();
    uint32_t readFSR1();
};

#endif // _EFCFLASH_H
