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

#include "D5xNvmFlash.h"


#define CMDEX_KEY       0xa500

#define NVM_REG_BASE    0x41004000

#define NVM_REG_CTRLA   0x00
#define NVM_REG_CTRLB   0x04
#define NVM_REG_INTFLAG 0x10
#define NVM_REG_STATUS  0x12
#define NVM_REG_ADDR    0x14
#define NVM_REG_RUNLOCK 0x18

#define NVM_CMD_EP      0x00
#define NVM_CMD_EB      0x01
#define NVM_CMD_WP      0x03
#define NVM_CMD_WQW     0x04
#define NVM_CMD_LR      0x11
#define NVM_CMD_UR      0x12
#define NVM_CMD_SSB     0x16
#define NVM_CMD_PBC     0x15

#define ERASE_BLOCK_PAGES 16 // pages

// NVM User Page
#define NVM_UP_ADDR                 0x804000
#define NVM_UP_SIZE                 (_size)
#define NVM_UP_BOD33_DISABLE_OFFSET 0x0
#define NVM_UP_BOD33_DISABLE_MASK   0x1
#define NVM_UP_BOD33_RESET_OFFSET   0x1
#define NVM_UP_BOD33_RESET_MASK     0x2
#define NVM_UP_NVM_LOCK_OFFSET      0x8

D5xNvmFlash::D5xNvmFlash(
    Samba& samba,
    const std::string& name,
    uint32_t pages,
    uint32_t size,
    uint32_t user,
    uint32_t stack)
    :
    Flash(samba, name, 0, pages, size, 1, 32, user, stack), _eraseAuto(true)
{
}

D5xNvmFlash::~D5xNvmFlash()
{
}

void
D5xNvmFlash::erase(uint32_t offset, uint32_t size)
{
    uint32_t eraseSize = _size * ERASE_BLOCK_PAGES;

    // Offset must be a multiple of the erase size
    if (offset % eraseSize)
        throw FlashEraseError();

    // Offset and size must be in range
    if (offset + size > totalSize())
        throw FlashEraseError();

    uint32_t eraseEnd = (offset + size + eraseSize - 1) / eraseSize;

    // Erase each erase size set of pages
    for (uint32_t eraseNum = offset / eraseSize; eraseNum < eraseEnd; eraseNum++)
    {
        // Issue erase command
        writeRegU32(NVM_REG_ADDR, eraseNum * eraseSize);
        command(NVM_CMD_EB);
    }
}

void
D5xNvmFlash::eraseAll(uint32_t offset)
{
    // Use the extended Samba command if available
    if (_samba.canChipErase())
    {
        _samba.chipErase(offset);
    }
    else
    {
        erase(offset, totalSize() - offset);
    }
}

void
D5xNvmFlash::waitReady()
{
    while ((readRegU16(NVM_REG_STATUS) & 0x1) == 0);
}

void
D5xNvmFlash::eraseAuto(bool enable)
{
    _eraseAuto = enable;
}

std::vector<bool>
D5xNvmFlash::getLockRegions()
{
    uint8_t lockBits = 0;
    uint32_t addr = NVM_UP_ADDR + NVM_UP_NVM_LOCK_OFFSET;
    std::vector<bool> regions(_lockRegions);

    for (uint32_t region = 0; region < _lockRegions; region++)
    {
        if (region % 8 == 0)
            lockBits = _samba.readByte(addr++);
        regions[region] = (lockBits & (1 << (region % 8))) == 0;
    }

    return regions;
}

bool
D5xNvmFlash::getSecurity()
{
    // There doesn't seem to be a way to read this
    return false;
}

bool
D5xNvmFlash::getBod()
{
    uint8_t byte = _samba.readByte(NVM_UP_ADDR + NVM_UP_BOD33_DISABLE_OFFSET);

    return (byte & NVM_UP_BOD33_DISABLE_MASK) == 0;
}

bool
D5xNvmFlash::getBor()
{
    uint8_t byte = _samba.readByte(NVM_UP_ADDR + NVM_UP_BOD33_RESET_OFFSET);

    return (byte & NVM_UP_BOD33_RESET_MASK) != 0;
}

bool
D5xNvmFlash::getBootFlash()
{
    return true;
}

void
D5xNvmFlash::readUserPage(std::unique_ptr<uint8_t[]>& userPage)
{
    if (!userPage)
    {
        userPage.reset(new uint8_t[NVM_UP_SIZE]);
        _samba.read(NVM_UP_ADDR, userPage.get(), NVM_UP_SIZE);
    }
}

void
D5xNvmFlash::writeOptions()
{
    std::unique_ptr<uint8_t[]> userPage;

    if (canBor() && _bor.isDirty() && _bor.get() != getBor())
    {
        readUserPage(userPage);
        if (_bor.get())
            userPage[NVM_UP_BOD33_RESET_OFFSET] |= NVM_UP_BOD33_RESET_MASK;
        else
            userPage[NVM_UP_BOD33_RESET_OFFSET] &= ~NVM_UP_BOD33_RESET_MASK;
    }
    if (canBod() && _bod.isDirty() && _bod.get() != getBod())
    {
        readUserPage(userPage);
        if (_bod.get())
            userPage[NVM_UP_BOD33_DISABLE_OFFSET] &= ~NVM_UP_BOD33_DISABLE_MASK;
        else
            userPage[NVM_UP_BOD33_DISABLE_OFFSET] |= NVM_UP_BOD33_DISABLE_MASK;
    }
    if (_regions.isDirty())
    {
        // Check if any lock bits are different from the current set
        std::vector<bool> current = getLockRegions();
        if (!equal(_regions.get().begin(), _regions.get().end(), current.begin()))
        {
            readUserPage(userPage);

            uint8_t* lockBits = &userPage[NVM_UP_NVM_LOCK_OFFSET];
            for (uint32_t region = 0; region < _regions.get().size(); region++)
            {
                if (_regions.get()[region])
                    lockBits[region / 8] &= ~(1 << (region % 8));
                else
                    lockBits[region / 8] |= (1 << (region % 8));
            }
        }
    }

    // Erase and write the user page if modified
    if (userPage)
    {
        // Configure manual page write and disable caches
        writeRegU16(NVM_REG_CTRLA, (readRegU16(NVM_REG_CTRLA) | (0x3 << 14)) & 0xffcf);

        // Erase user page
        writeRegU32(NVM_REG_ADDR, NVM_UP_ADDR);
        command(NVM_CMD_EP);

        // Write user page in quad-word chunks
        for (uint32_t offset = 0; offset < NVM_UP_SIZE; offset += 16)
        {
            // Load the buffer with the quad word
            loadBuffer(&userPage[offset], 16);

            // Clear page buffer
            command(NVM_CMD_PBC);

            // Copy quad word to page buffer
            _wordCopy.setDstAddr(NVM_UP_ADDR + offset);
            _wordCopy.setSrcAddr(_onBufferA ? _pageBufferA : _pageBufferB);
            _wordCopy.setWords(4);
            _onBufferA = !_onBufferA;
            waitReady();
            _wordCopy.runv();

            // Write the quad word
            writeRegU32(NVM_REG_ADDR, NVM_UP_ADDR + offset);
            command(NVM_CMD_WQW);
        }
    }

    // Always do security last
    if (_security.isDirty() && _security.get() == true && _security.get() != getSecurity())
    {
        command(NVM_CMD_SSB);
    }
}

void
D5xNvmFlash::writePage(uint32_t page)
{
    if (page >= _pages)
    {
        throw FlashPageError();
    }

    // Configure manual page write and disable caches
    writeRegU16(NVM_REG_CTRLA, (readRegU16(NVM_REG_CTRLA) | (0x3 << 14)) & 0xffcf);

    // Auto-erase if writing at the start of the erase page
    if (_eraseAuto && page % ERASE_BLOCK_PAGES == 0)
    {
        erase(page * _size, ERASE_BLOCK_PAGES * _size);
    }

    // Clear page bur
    command(NVM_CMD_PBC);

    uint32_t addr = _addr + (page * _size );

    _wordCopy.setDstAddr(addr);
    _wordCopy.setSrcAddr(_onBufferA ? _pageBufferA : _pageBufferB);
    _wordCopy.setWords(_size / sizeof(uint32_t));
    _onBufferA = !_onBufferA;
    waitReady();
    _wordCopy.runv();

    writeRegU32(NVM_REG_ADDR, addr);
    command(NVM_CMD_WP);
}

void
D5xNvmFlash::readPage(uint32_t page, uint8_t* buf)
{
    if (page >= _pages)
    {
        throw FlashPageError();
    }

    _samba.read(_addr + (page * _size), buf, _size);
}

uint16_t
D5xNvmFlash::readRegU16(uint8_t reg)
{
    return (uint16_t) _samba.readByte(NVM_REG_BASE + reg) |
           (_samba.readByte(NVM_REG_BASE + reg + 1) << 8);
}

void
D5xNvmFlash::writeRegU16(uint8_t reg, uint16_t value)
{
    _samba.writeByte(NVM_REG_BASE + reg, value & 0xff);
    _samba.writeByte(NVM_REG_BASE + reg + 1, value >> 8);
}

uint32_t
D5xNvmFlash::readRegU32(uint8_t reg)
{
    return _samba.readWord(NVM_REG_BASE + reg);
}

void
D5xNvmFlash::writeRegU32(uint8_t reg, uint32_t value)
{
    _samba.writeWord(NVM_REG_BASE + reg, value);
}

void
D5xNvmFlash::command(uint8_t cmd)
{
    waitReady();

    writeRegU32(NVM_REG_CTRLB, CMDEX_KEY | cmd);

    waitReady();

    if (readRegU16(NVM_REG_INTFLAG) & 0xce)
    {
        // Clear the error bits
        writeRegU16(NVM_REG_INTFLAG, 0xce);
        throw FlashCmdError();
    }
}

void
D5xNvmFlash::writeBuffer(uint32_t dst_addr, uint32_t size)
{
    // Auto-erase if writing at the start of the erase page
    if (_eraseAuto && ((dst_addr / _size) % ERASE_BLOCK_PAGES == 0))
        erase(dst_addr, size);

    // Call the base class method
    Flash::writeBuffer(dst_addr, size);
}
