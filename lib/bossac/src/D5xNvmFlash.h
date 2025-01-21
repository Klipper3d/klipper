///////////////////////////////////////////////////////////////////////////////
// BOSSA
//
// Copyright (c) 2018, ShumaTech
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////

#ifndef _D5XNVMFLASH_H
#define _D5XNVMFLASH_H

#include <stdint.h>
#include <exception>

#include "Flash.h"

class D5xNvmFlash : public Flash
{
public:
    D5xNvmFlash(
        Samba& samba,
        const std::string& name,
        uint32_t pages,
        uint32_t size,
        uint32_t user,
        uint32_t stack);

    virtual ~D5xNvmFlash();

    void eraseAll(uint32_t offset);
    void eraseAuto(bool enable);

    std::vector<bool> getLockRegions();

    bool getSecurity();

    bool getBod();
    bool canBod() { return true; }

    bool getBor();
    bool canBor() { return true; }

    bool getBootFlash();
    bool canBootFlash() { return false; }

    void writeOptions();

    void writePage(uint32_t page);
    void readPage(uint32_t page, uint8_t* data);

    void writeBuffer(uint32_t dst_addr, uint32_t size);

protected:
    bool     _eraseAuto;

    uint16_t readRegU16(uint8_t reg);
    void writeRegU16(uint8_t reg, uint16_t value);
    uint32_t readRegU32(uint8_t reg);
    void writeRegU32(uint8_t reg, uint32_t value);

    void waitReady();
    void command(uint8_t cmd);
    void erase(uint32_t offset, uint32_t size);
    void checkError();
    void readUserPage(std::unique_ptr<uint8_t[]>& userPage);
};

#endif // _D5XNVMFLASH_H
