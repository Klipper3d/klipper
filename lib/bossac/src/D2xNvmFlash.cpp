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

#include "D2xNvmFlash.h"

// CMDEX field should be 0xA5 to allow execution of any command.
#define CMDEX_KEY                       0xa500

// NVM ready bit mask
#define NVM_INT_STATUS_READY_MASK       0x1

// NVM status mask
#define NVM_CTRL_STATUS_MASK            0xFFEB

#define NVM_REG_BASE    0x41004000

#define NVM_REG_CTRLA   0x00
#define NVM_REG_CTRLB   0x04
#define NVM_REG_INTFLAG 0x14
#define NVM_REG_STATUS  0x18
#define NVM_REG_ADDR    0x1c
#define NVM_REG_LOCK    0x20

#define NVM_CMD_ER      0x02
#define NVM_CMD_WP      0x04
#define NVM_CMD_EAR     0x05
#define NVM_CMD_WAP     0x06
#define NVM_CMD_LR      0x40
#define NVM_CMD_UR      0x41
#define NVM_CMD_SSB     0x45
#define NVM_CMD_PBC     0x44

#define ERASE_ROW_PAGES 4 // pages

// NVM User Row
#define NVM_UR_ADDR                 0x804000
#define NVM_UR_SIZE                 (_size * ERASE_ROW_PAGES)
#define NVM_UR_BOD33_ENABLE_OFFSET  0x1
#define NVM_UR_BOD33_ENABLE_MASK    0x6
#define NVM_UR_BOD33_RESET_OFFSET   0x1
#define NVM_UR_BOD33_RESET_MASK     0x7
#define NVM_UR_NVM_LOCK_OFFSET      0x6

D2xNvmFlash::D2xNvmFlash(
    Samba& samba,
    const std::string& name,
    uint32_t pages,
    uint32_t size,
    uint32_t user,
    uint32_t stack)
    :
    Flash(samba, name, 0, pages, size, 1, 16, user, stack), _eraseAuto(true)
{
}

D2xNvmFlash::~D2xNvmFlash()
{
}

void
D2xNvmFlash::erase(uint32_t offset, uint32_t size)
{
    uint32_t eraseSize = _size * ERASE_ROW_PAGES;

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
        waitReady();

        // Clear error bits
        uint16_t statusReg = readReg(NVM_REG_STATUS);
        writeReg(NVM_REG_STATUS, statusReg | NVM_CTRL_STATUS_MASK);

        // Issue erase command
        uint32_t wordAddr = (eraseNum * eraseSize) / 2;
        writeReg(NVM_REG_ADDR, wordAddr);
        command(NVM_CMD_ER);
    }
}

void
D2xNvmFlash::eraseAll(uint32_t offset)
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
D2xNvmFlash::eraseAuto(bool enable)
{
    _eraseAuto = enable;
}

std::vector<bool>
D2xNvmFlash::getLockRegions()
{
    uint8_t lockBits = 0;
    uint32_t addr = NVM_UR_ADDR + NVM_UR_NVM_LOCK_OFFSET;
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
D2xNvmFlash::getSecurity()
{
    return (readReg(NVM_REG_STATUS) & 0x100) != 0;
}

bool
D2xNvmFlash::getBod()
{
    uint8_t byte = _samba.readByte(NVM_UR_ADDR + NVM_UR_BOD33_ENABLE_OFFSET);

    return (byte & NVM_UR_BOD33_ENABLE_MASK) != 0;
}

bool
D2xNvmFlash::getBor()
{
    uint8_t byte = _samba.readByte(NVM_UR_ADDR + NVM_UR_BOD33_RESET_OFFSET);

    return (byte & NVM_UR_BOD33_RESET_MASK) != 0;
}

bool
D2xNvmFlash::getBootFlash()
{
    return true;
}

void
D2xNvmFlash::readUserRow(std::unique_ptr<uint8_t[]>& userRow)
{
    if (!userRow)
    {
        userRow.reset(new uint8_t[NVM_UR_SIZE]);
        _samba.read(NVM_UR_ADDR, userRow.get(), NVM_UR_SIZE);
    }
}

void
D2xNvmFlash::writeOptions()
{
    std::unique_ptr<uint8_t[]> userRow;

    if (canBor() && _bor.isDirty() && _bor.get() != getBor())
    {
        readUserRow(userRow);
        if (_bor.get())
            userRow[NVM_UR_BOD33_RESET_OFFSET] |= NVM_UR_BOD33_RESET_MASK;
        else
            userRow[NVM_UR_BOD33_RESET_OFFSET] &= ~NVM_UR_BOD33_RESET_MASK;
    }
    if (canBod() && _bod.isDirty() && _bod.get() != getBod())
    {
        readUserRow(userRow);
        if (_bod.get())
            userRow[NVM_UR_BOD33_ENABLE_OFFSET] |= NVM_UR_BOD33_ENABLE_MASK;
        else
            userRow[NVM_UR_BOD33_ENABLE_OFFSET] &= ~NVM_UR_BOD33_ENABLE_MASK;
    }
    if (_regions.isDirty())
    {
        // Check if any lock bits are different from the current set
        std::vector<bool> current = getLockRegions();
        if (!equal(_regions.get().begin(), _regions.get().end(), current.begin()))
        {
            readUserRow(userRow);

            uint8_t* lockBits = &userRow[NVM_UR_NVM_LOCK_OFFSET];
            for (uint32_t region = 0; region < _regions.get().size(); region++)
            {
                if (_regions.get()[region])
                    lockBits[region / 8] &= ~(1 << (region % 8));
                else
                    lockBits[region / 8] |= (1 << (region % 8));
            }
        }
    }

    // Erase and write the user row if modified
    if (userRow)
    {
        // Disable cache and configure manual page write
        writeReg(NVM_REG_CTRLB, readReg(NVM_REG_CTRLB) | (0x1 << 18) | (0x1 << 7));

        // Erase user row
        writeReg(NVM_REG_ADDR, NVM_UR_ADDR / 2);
        command(NVM_CMD_EAR);

        // Write user row in page chunks
        for (uint32_t offset = 0; offset < NVM_UR_SIZE; offset += _size)
        {
            // Load the buffer with the page
            loadBuffer(&userRow[offset], _size);

            // Clear page buffer
            command(NVM_CMD_PBC);

            // Copy page to page buffer
            _wordCopy.setDstAddr(NVM_UR_ADDR + offset);
            _wordCopy.setSrcAddr(_onBufferA ? _pageBufferA : _pageBufferB);
            _onBufferA = !_onBufferA;
            waitReady();
            _wordCopy.runv();

            // Write the page
            writeReg(NVM_REG_ADDR, (NVM_UR_ADDR + offset) / 2);
            command(NVM_CMD_WAP);
        }
    }

    // Always do security last
    if (_security.isDirty() && _security.get() == true && _security.get() != getSecurity())
    {
        command(NVM_CMD_SSB);
    }
}

void
D2xNvmFlash::writePage(uint32_t page)
{
    if (page >= _pages)
    {
        throw FlashPageError();
    }

    // Disable cache and configure manual page write
    writeReg(NVM_REG_CTRLB, readReg(NVM_REG_CTRLB) | (0x1 << 18) | (0x1 << 7));

    // Auto-erase if writing at the start of the erase page
    if (_eraseAuto && page % ERASE_ROW_PAGES == 0)
        erase(page * _size, ERASE_ROW_PAGES * _size);

    // Clear page buffer
    command(NVM_CMD_PBC);

    // Compute the start address.
    uint32_t addr = _addr + (page * _size);

    _wordCopy.setDstAddr(addr);
    _wordCopy.setSrcAddr(_onBufferA ? _pageBufferA : _pageBufferB);
    _onBufferA = !_onBufferA;
    waitReady();
    _wordCopy.runv();

    writeReg(NVM_REG_ADDR, addr / 2);
    command(NVM_CMD_WP);
}

void
D2xNvmFlash::waitReady()
{
    while ((readReg(NVM_REG_INTFLAG) & 0x1) == 0);
}

void
D2xNvmFlash::readPage(uint32_t page, uint8_t* buf)
{
    if (page >= _pages)
    {
        throw FlashPageError();
    }

    _samba.read(_addr + (page * _size), buf, _size);
}

uint32_t
D2xNvmFlash::readReg(uint8_t reg)
{
    return _samba.readWord(NVM_REG_BASE + reg);
}

void
D2xNvmFlash::writeReg(uint8_t reg, uint32_t value)
{
    _samba.writeWord(NVM_REG_BASE + reg, value);
}

void
D2xNvmFlash::command(uint8_t cmd)
{
    waitReady();

    writeReg(NVM_REG_CTRLA, CMDEX_KEY | cmd);

    waitReady();

    if (readReg(NVM_REG_INTFLAG) & 0x2)
    {
        // Clear the error bit
        writeReg(NVM_REG_INTFLAG, 0x2);
        throw FlashCmdError();
    }
}

void
D2xNvmFlash::writeBuffer(uint32_t dst_addr, uint32_t size)
{
    // Auto-erase if enabled
    if (_eraseAuto)
        erase(dst_addr, size);

    // Call the base class method
    Flash::writeBuffer(dst_addr, size);
}
