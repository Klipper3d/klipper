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
#ifndef _APPLET_H
#define _APPLET_H

#include <stdint.h>

#include "Samba.h"

class Applet
{
public:
    Applet(Samba& samba,
           uint32_t addr,
           uint8_t* code,
           uint32_t size,
           uint32_t start,
           uint32_t stack,
           uint32_t reset);
    virtual ~Applet() {}

    virtual uint32_t size() { return _size; }
    virtual uint32_t addr() { return _addr; }

    virtual void setStack(uint32_t stack);

    virtual void run(); // To be used for Thumb-1 based devices (ARM7TDMI, ARM9)
    virtual void runv(); // To be used for Thumb-2 based devices (Cortex-Mx)

protected:
    Samba& _samba;
    uint32_t _addr; // Address in device SRAM where will be placed the applet
    uint32_t _size; // Applet size
    uint32_t _start; //
    uint32_t _stack; // Applet stack address in device SRAM
    uint32_t _reset;
};

#endif // _APPLET_H
