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
#include "EfcFlash.h"

#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#define EFC_KEY         0x5a

#define EFC0_FMR        0xffffff60
#define EFC0_FCR        0xffffff64
#define EFC0_FSR        0xffffff68

#define EFC1_FMR        0xffffff70
#define EFC1_FCR        0xffffff74
#define EFC1_FSR        0xffffff78

#define EFC_FCMD_WP     0x1
#define EFC_FCMD_SLB    0x2
#define EFC_FCMD_WPL    0x3
#define EFC_FCMD_CLB    0x4
#define EFC_FCMD_EA     0x8
#define EFC_FCMD_SGPB   0xb
#define EFC_FCMD_CGPB   0xd
#define EFC_FCMD_SSB    0xf

EfcFlash::EfcFlash(Samba& samba,
                   const std::string& name,
                   uint32_t addr,
                   uint32_t pages,
                   uint32_t size,
                   uint32_t planes,
                   uint32_t lockRegions,
                   uint32_t user,
                   uint32_t stack,
                   bool canBootFlash)
    : Flash(samba, name, addr, pages, size, planes, lockRegions, user, stack),
      _canBootFlash(canBootFlash)
{
    assert(planes == 1 || planes == 2);
    assert(pages <= planes * 1024);
    assert(lockRegions <= 32);

    eraseAuto(true);
}

EfcFlash::~EfcFlash()
{
}

void
EfcFlash::eraseAll(uint32_t offset)
{
    if (offset != 0)
        throw FlashEraseError();

    waitFSR();
    writeFCR0(EFC_FCMD_EA, 0);
    if (_planes == 2)
    {
        waitFSR();
        writeFCR0(EFC_FCMD_EA, _pages / 2);
    }
}

void
EfcFlash::eraseAuto(bool enable)
{
    uint32_t fmr;

    waitFSR();
    fmr = _samba.readWord(EFC0_FMR);
    if (enable)
        fmr &= ~(1 << 7);
    else
        fmr |= (1 << 7);

    _samba.writeWord(EFC0_FMR, fmr);
    if (_planes == 2)
    {
        waitFSR();
        _samba.writeWord(EFC1_FMR, fmr);
    }
}

std::vector<bool>
EfcFlash::getLockRegions()
{
    std::vector<bool> regions(_lockRegions);
    uint32_t fsr0;
    uint32_t fsr1;

    fsr0 = readFSR0();
    if (_planes == 2)
        fsr1 = readFSR1();
    else
        fsr1 = 0;

    for (uint32_t region = 0; region < _lockRegions; region++)
    {
        if (_planes == 2 && region >= _lockRegions / 2)
            regions[region] = (fsr1 & (1 << (16 + region - _lockRegions / 2))) != 0;
        else
            regions[region] = (fsr0 & (1 << (16 + region))) != 0;
    }

    return regions;
}

bool
EfcFlash::getSecurity()
{
    return (readFSR0() & (1 << 4));
}

bool
EfcFlash::getBod()
{
    return (readFSR0() & (1 << 8));
}

bool
EfcFlash::getBor()
{
    return (readFSR0() & (2 << 8));
}

bool
EfcFlash::getBootFlash()
{
    if (!_canBootFlash)
        return false;

    return (readFSR0() & (1 << 10));
}

void
EfcFlash::writeOptions()
{
    if (canBootFlash() && _bootFlash.isDirty() && _bootFlash.get() != getBootFlash())
    {
        waitFSR();
        writeFCR0(_bootFlash.get() ? EFC_FCMD_SGPB : EFC_FCMD_CGPB, 2);
    }
    if (canBor() && _bor.isDirty() && _bor.get() != getBor())
    {
        waitFSR();
        writeFCR0(_bor.get() ? EFC_FCMD_SGPB : EFC_FCMD_CGPB, 1);
    }
    if (canBod() && _bod.isDirty() && _bod.get() != getBod())
    {
        waitFSR();
        writeFCR0(_bod.get() ? EFC_FCMD_SGPB : EFC_FCMD_CGPB, 0);
    }
    if (_regions.isDirty())
    {
        uint32_t page;
        std::vector<bool> current;

        current = getLockRegions();

        for (uint32_t region = 0; region < _regions.get().size(); region++)
        {
            if (_regions.get()[region] != current[region])
            {
                if (_planes == 2 && region >= _lockRegions / 2)
                {
                    page = (region - _lockRegions / 2) * _pages / _lockRegions;
                    waitFSR();
                    writeFCR1(_regions.get()[region] ? EFC_FCMD_SLB : EFC_FCMD_CLB, page);
                }
                else
                {
                    page = region * _pages / _lockRegions;
                    waitFSR();
                    writeFCR0(_regions.get()[region] ? EFC_FCMD_SLB : EFC_FCMD_CLB, page);
                }
            }
        }
    }
    if (_security.isDirty() && _security.get() == true && _security.get() != getSecurity())
    {
        waitFSR();
        writeFCR0(EFC_FCMD_SSB, 0);
    }
}

void
EfcFlash::writePage(uint32_t page)
{
    if (page >= _pages)
        throw FlashPageError();

    _wordCopy.setDstAddr(_addr + page * _size);
    _wordCopy.setSrcAddr(_onBufferA ? _pageBufferA : _pageBufferB);
    _onBufferA = !_onBufferA;
    waitFSR();
    _wordCopy.run();
    if (_planes == 2 && page >= _pages / 2)
        writeFCR1(EFC_FCMD_WP, page - _pages / 2);
    else
        writeFCR0(EFC_FCMD_WP, page);
}

void
EfcFlash::readPage(uint32_t page, uint8_t* data)
{
    if (page >= _pages)
        throw FlashPageError();

    waitFSR();
    _samba.read(_addr + page * _size, data, _size);
}

void
EfcFlash::waitFSR(int seconds)
{
    int tries = seconds * 1000;
    uint32_t fsr0;
    uint32_t fsr1 = 0x1;

    while (tries-- > 0)
    {
        fsr0 = readFSR0();
        if (fsr0 & 0x2)
            throw FlashCmdError();
        if (fsr0 & 0x4)
            throw FlashLockError();

        if (_planes == 2)
        {
            fsr1 = readFSR1();
            if (fsr1 & 0x2)
                throw FlashCmdError();
            if (fsr1 & 0x4)
                throw FlashLockError();
        }
        if (fsr0 & fsr1 & 0x1)
            break;
        usleep(1000);
    }
    if (tries == 0)
        throw FlashTimeoutError();
}

void
EfcFlash::writeFCR0(uint8_t cmd, uint32_t arg)
{
    _samba.writeWord(EFC0_FCR, (EFC_KEY << 24) | (arg << 8) | cmd);
}

void
EfcFlash::writeFCR1(uint8_t cmd, uint32_t arg)
{
    _samba.writeWord(EFC1_FCR, (EFC_KEY << 24) | (arg << 8) | cmd);
}

uint32_t
EfcFlash::readFSR0()
{
    return _samba.readWord(EFC0_FSR);
}

uint32_t
EfcFlash::readFSR1()
{
    return _samba.readWord(EFC1_FSR);
}
