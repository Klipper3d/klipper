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
#ifndef _DEVICE_H
#define _DEVICE_H

#include <exception>

#include "Samba.h"
#include "Flash.h"

class DeviceUnsupportedError : public std::exception
{
public:
    DeviceUnsupportedError() : exception() {};
    const char* what() const throw() { return "Device unsupported"; }
};

class Device
{
public:
    enum Family {
        FAMILY_NONE,

        FAMILY_SAM7S,
        FAMILY_SAM7SE,
        FAMILY_SAM7X,
        FAMILY_SAM7XC,
        FAMILY_SAM7L,

        FAMILY_SAM3N,
        FAMILY_SAM3S,
        FAMILY_SAM3U,
        FAMILY_SAM3X,
        FAMILY_SAM3A,

        FAMILY_SAM4S,
        FAMILY_SAM4E,

        FAMILY_SAM9XE,

        FAMILY_SAMD21,
        FAMILY_SAMR21,
        FAMILY_SAML21,

        FAMILY_SAMD51,
        FAMILY_SAME51,
        FAMILY_SAME53,
        FAMILY_SAME54,

        FAMILY_SAME70,
        FAMILY_SAMS70,
        FAMILY_SAMV70,
        FAMILY_SAMV71,
    };

    Device(Samba& samba) : _samba(samba), _flash(nullptr), _family(FAMILY_NONE) {}
    virtual ~Device() {}

    void create();

    Family getFamily() { return _family; }

    typedef std::unique_ptr<Flash> const FlashPtr;

    FlashPtr& getFlash() { return _flash; }

    void reset();

private:
    Samba& _samba;
    std::unique_ptr<Flash> _flash;
    Family _family;

    void readChipId(uint32_t& chipId, uint32_t& extChipId);
};

#endif // _DEVICE_H

