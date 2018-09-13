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
#ifndef _FLASH_H
#define _FLASH_H

#include <stdint.h>
#include <vector>
#include <memory>
#include <exception>

#include "Samba.h"
#include "WordCopyApplet.h"

class FlashPageError : public std::exception
{
public:
    FlashPageError() : exception() {};
    const char* what() const throw() { return "Invalid flash page"; }
};

class FlashRegionError : public std::exception
{
public:
    FlashRegionError() : exception() {};
    const char* what() const throw() { return "Invalid lock region"; }
};

class FlashLockError : public std::exception
{
public:
    FlashLockError() : exception() {};
    const char* what() const throw() { return "Flash page is locked"; }
};

class FlashCmdError : public std::exception
{
public:
    FlashCmdError() : exception() {};
    const char* what() const throw() { return "Flash command failed"; }
};

class FlashTimeoutError : public std::exception
{
public:
    FlashTimeoutError() : exception() {};
    const char* what() const throw() { return "Flash command timeout"; }
};

class BootFlashError : public std::exception
{
public:
    BootFlashError() : exception() {};
    const char* what() const throw() { return "Unable to clear boot flash for this device"; }

};

class FlashEraseError : public std::exception
{
public:
    FlashEraseError() : exception() {};
    const char* what() const throw() { return "Flash erase failed"; }

};

template<class T>
class FlashOption
{
public:
    FlashOption() : _dirty(false) {}
    virtual ~FlashOption() {}
    void set(const T& value) { _value = value; _dirty = true; }
    const T& get() { return _value; }
    bool isDirty() { return _dirty; }
    
private:
    T    _value;
    bool _dirty;
};

class Flash
{
public:
    Flash(Samba& samba,
          const std::string& name,
          uint32_t addr,                 // Flash base address
          uint32_t pages,                // Number of pages
          uint32_t size,                 // Page size in bytes
          uint32_t planes,               // Number of flash planes
          uint32_t lockRegions,          // Number of flash lock regions
          uint32_t user,                 // Address in SRAM where the applet and buffers will be placed
          uint32_t stack);               // Address in SRAM where the applet stack will be placed
    virtual ~Flash() {}

    const std::string& name() { return _name; }

    virtual uint32_t address() { return _addr; }
    virtual uint32_t pageSize() { return _size; }
    virtual uint32_t numPages() { return _pages; }
    virtual uint32_t numPlanes() { return _planes; }
    virtual uint32_t totalSize() { return _size * _pages; }
    virtual uint32_t lockRegions() { return _lockRegions; }

    virtual void eraseAll(uint32_t offset) = 0;
    virtual void eraseAuto(bool enable) = 0;

    virtual std::vector<bool> getLockRegions() = 0;
    virtual void setLockRegions(const std::vector<bool>& regions);

    virtual bool getSecurity() = 0;
    virtual void setSecurity();

    virtual bool getBod() = 0;
    virtual void setBod(bool enable);
    virtual bool canBod() = 0;

    virtual bool getBor() = 0;
    virtual void setBor(bool enable);
    virtual bool canBor() = 0;

    virtual bool getBootFlash() = 0;
    virtual void setBootFlash(bool enable);
    virtual bool canBootFlash() = 0;

    virtual void writeOptions() = 0;

    virtual void writePage(uint32_t page) = 0;
    virtual void readPage(uint32_t page, uint8_t* data) = 0;

    virtual void writeBuffer(uint32_t dst_addr, uint32_t size);
    virtual void loadBuffer(const uint8_t* data, uint16_t size);

protected:
    Samba& _samba;
    std::string _name;
    uint32_t _addr;
    uint32_t _pages;
    uint32_t _size;
    uint32_t _planes;
    uint32_t _lockRegions;
    uint32_t _user;
    WordCopyApplet _wordCopy;

    FlashOption<bool> _bootFlash;
    FlashOption< std::vector<bool> > _regions;
    FlashOption<bool> _bod;
    FlashOption<bool> _bor;
    FlashOption<bool> _security;

    bool _onBufferA;
    uint32_t _pageBufferA;
    uint32_t _pageBufferB;
};

#endif // _FLASH_H
