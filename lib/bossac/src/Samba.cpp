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
#include "Samba.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

// XMODEM definitions
#define BLK_SIZE    128
#define MAX_RETRIES 5
#define SOH         0x01
#define EOT         0x04
#define ACK         0x06
#define NAK         0x15
#define CAN         0x18
#define START       'C'

#define TIMEOUT_QUICK   100
#define TIMEOUT_NORMAL  1000
#define TIMEOUT_LONG    5000

#define min(a, b)   ((a) < (b) ? (a) : (b))

Samba::Samba() :
    _canChipErase(false),
    _canWriteBuffer(false),
    _canChecksumBuffer(false),
    _readBufferSize(0),
    _debug(false),
    _isUsb(false)
{
}

Samba::~Samba()
{
}

bool
Samba::init()
{
    uint8_t cmd[3];

    _port->timeout(TIMEOUT_QUICK);

    // Flush garbage
    uint8_t dummy[1024];
    _port->read(dummy, sizeof(dummy));

    if (!_isUsb)
    {
        if (_debug)
            printf("Send auto-baud\n");

        // RS-232 auto-baud sequence
        _port->put(0x80);
        _port->get();
        _port->put(0x80);
        _port->get();
        _port->put('#');
        _port->read(cmd, 3);
    }

    // Set binary mode
    if (_debug)
        printf("Set binary mode\n");
    cmd[0] = 'N';
    cmd[1] = '#';
    _port->write(cmd, 2);
    _port->read(cmd, 2);

    std::string ver;
    try
    {
        ver = version();
    }
    catch(SambaError& err)
    {
        return false;
    }

    std::size_t extIndex = ver.find("[Arduino:");
    if (extIndex != string::npos)
    {
        extIndex += 9;
        while (ver[extIndex] != ']')
        {
            switch (ver[extIndex])
            {
                case 'X': _canChipErase = true; break;
                case 'Y': _canWriteBuffer = true; break;
                case 'Z': _canChecksumBuffer = true; break;
            }
            extIndex++;
        }

        // All SAMD-based Arduino/AdaFruit boards have a bug in their bootloader
        // that trying to read 64 bytes or more over USB corrupts the data.
        // We must limit these boards to read chunks of 63 bytes.
        if (_isUsb)
            _readBufferSize = 63;
    }

    _port->timeout(TIMEOUT_NORMAL);

    return true;
}

bool
Samba::connect(SerialPort::Ptr port, int bps)
{
    _port = move(port);

    // Try to connect at a high speed if USB
    _isUsb = _port->isUsb();
    if (_isUsb)
    {
        if (_port->open(921600) && init())
        {
            if (_debug)
                printf("Connected at 921600 baud\n");
            return true;
        }
        else
        {
            _port->close();
        }
    }
    _isUsb = false;

    // Try the serial port at slower speed
    if (_port->open(bps) && init())
    {
        if (_debug)
            printf("Connected at %d baud\n", bps);
        return true;
    }

    disconnect();
    return false;
}

void
Samba::disconnect()
{
    _port->close();
    _port.release();
}

void
Samba::writeByte(uint32_t addr, uint8_t value)
{
    uint8_t cmd[14];

    if (_debug)
        printf("%s(addr=%#x,value=%#x)\n", __FUNCTION__, addr, value);

    snprintf((char*) cmd, sizeof(cmd), "O%08X,%02X#", addr, value);
    if (_port->write(cmd, sizeof(cmd) - 1) != sizeof(cmd) -  1)
        throw SambaError();

    // The SAM firmware has a bug that if the command and binary data
    // are received in the same USB data packet, then the firmware
    // gets confused.  Even though the writes are separated in the code,
    // USB drivers often do write combining which can put them together
    // in the same USB data packet.  To avoid this, we call the serial
    // port object's flush method before writing the data.
    if (_isUsb)
        _port->flush();
}

uint8_t
Samba::readByte(uint32_t addr)
{
    uint8_t cmd[13];
    uint8_t value;

    snprintf((char*) cmd, sizeof(cmd), "o%08X,4#", addr);
    if (_port->write(cmd, sizeof(cmd) - 1) != sizeof(cmd) - 1)
        throw SambaError();
    if (_port->read(cmd, sizeof(uint8_t)) != sizeof(uint8_t))
        throw SambaError();

    value = cmd[0];

    if (_debug)
        printf("%s(addr=%#x)=%#x\n", __FUNCTION__, addr, value);

    return value;
}


void
Samba::writeWord(uint32_t addr, uint32_t value)
{
    uint8_t cmd[20];

    if (_debug)
        printf("%s(addr=%#x,value=%#x)\n", __FUNCTION__, addr, value);

    snprintf((char*) cmd, sizeof(cmd), "W%08X,%08X#", addr, value);
    if (_port->write(cmd, sizeof(cmd) - 1) != sizeof(cmd) - 1)
        throw SambaError();

    // The SAM firmware has a bug that if the command and binary data
    // are received in the same USB data packet, then the firmware
    // gets confused.  Even though the writes are sperated in the code,
    // USB drivers often do write combining which can put them together
    // in the same USB data packet.  To avoid this, we call the serial
    // port object's flush method before writing the data.
    if (_isUsb)
        _port->flush();
}


uint32_t
Samba::readWord(uint32_t addr)
{
    uint8_t cmd[13];
    uint32_t value;

    snprintf((char*) cmd, sizeof(cmd), "w%08X,4#", addr);
    if (_port->write(cmd, sizeof(cmd) - 1) != sizeof(cmd) - 1)
        throw SambaError();
    if (_port->read(cmd, sizeof(uint32_t)) != sizeof(uint32_t))
        throw SambaError();

    value = (cmd[3] << 24 | cmd[2] << 16 | cmd[1] << 8 | cmd[0] << 0);

    if (_debug)
        printf("%s(addr=%#x)=%#x\n", __FUNCTION__, addr, value);

    return value;
}

static const uint16_t crc16Table[256] = {
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
    0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
    0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
    0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
    0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
    0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
    0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
    0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
    0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
    0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
    0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
    0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
    0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
    0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
    0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
    0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
    0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
    0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
    0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
    0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
    0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
    0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
    0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
    0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
    0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
    0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
    0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
    0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
    0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
    0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
    0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
    0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

uint16_t
Samba::crc16Calc(const uint8_t *data, int len)
{
    uint16_t crc16 = 0;

    while (len-- > 0)
        crc16 = (crc16 << 8) ^ crc16Table[((crc16 >> 8) ^ *(uint8_t*) data++) & 0xff];
    return crc16;
}

bool
Samba::crc16Check(const uint8_t *blk)
{
    uint16_t crc16;

    crc16 = blk[BLK_SIZE + 3] << 8 | blk[BLK_SIZE + 4];
    return (crc16Calc(&blk[3], BLK_SIZE) == crc16);
}

void
Samba::crc16Add(uint8_t *blk)
{
    uint16_t crc16;

    crc16 = crc16Calc(&blk[3], BLK_SIZE);
    blk[BLK_SIZE + 3] = (crc16 >> 8) & 0xff;
    blk[BLK_SIZE + 4] = crc16 & 0xff;
}

uint16_t
Samba::checksumCalc(uint8_t data, uint16_t crc16) {
    return (crc16 << 8) ^ crc16Table[((crc16 >> 8) ^ data) & 0xff];
}

void
Samba::readXmodem(uint8_t* buffer, int size)
{
    uint8_t blk[BLK_SIZE + 5];
    uint32_t blkNum = 1;
    int retries;
    int bytes;

    while (size > 0)
    {
        for (retries = 0; retries < MAX_RETRIES; retries++)
        {
            if (blkNum == 1)
                _port->put(START);

            bytes = _port->read(blk, sizeof(blk));
            if (bytes == sizeof(blk) &&
                blk[0] == SOH &&
                blk[1] == (blkNum & 0xff) &&
                crc16Check(blk))
                break;

            if (blkNum != 1)
                _port->put(NAK);
        }
        if (retries == MAX_RETRIES)
            throw SambaError();

        _port->put(ACK);

        memmove(buffer, &blk[3], min(size, BLK_SIZE));
        buffer += BLK_SIZE;
        size -= BLK_SIZE;
        blkNum++;
    }

    for (retries = 0; retries < MAX_RETRIES; retries++)
    {
        if (_port->get() == EOT)
        {
            _port->put(ACK);
            break;
        }
        _port->put(NAK);
    }
    if (retries == MAX_RETRIES)
        throw SambaError();
}

void
Samba::writeXmodem(const uint8_t* buffer, int size)
{
    uint8_t blk[BLK_SIZE + 5];
    uint32_t blkNum = 1;
    int retries;
    int bytes;

    for (retries = 0; retries < MAX_RETRIES; retries++)
    {
        if (_port->get() == START)
            break;
    }
    if (retries == MAX_RETRIES)
        throw SambaError();

    while (size > 0)
    {
        blk[0] = SOH;
        blk[1] = (blkNum & 0xff);
        blk[2] = ~(blkNum & 0xff);
        memmove(&blk[3], buffer, min(size, BLK_SIZE));
        if (size < BLK_SIZE)
            memset(&blk[3] + size, 0, BLK_SIZE - size);
        crc16Add(blk);

        for (retries = 0; retries < MAX_RETRIES; retries++)
        {
            bytes = _port->write(blk, sizeof(blk));
            if (bytes != sizeof(blk))
                throw SambaError();

            if (_port->get() == ACK)
                break;
        }

        if (retries == MAX_RETRIES)
            throw SambaError();

        buffer += BLK_SIZE;
        size -= BLK_SIZE;
        blkNum++;
    }

    for (retries = 0; retries < MAX_RETRIES; retries++)
    {
        _port->put(EOT);
        if (_port->get() == ACK)
            break;
    }
    if (retries == MAX_RETRIES)
        throw SambaError();
}

void
Samba::readBinary(uint8_t* buffer, int size)
{
    if (_port->read(buffer, size) != size)
        throw SambaError();
}

void
Samba::writeBinary(const uint8_t* buffer, int size)
{
    while (size)
    {
        int written = _port->write(buffer, size);
        if (written <= 0)
            throw SambaError();
        buffer += written;
        size -= written;
    }
}

void
Samba::read(uint32_t addr, uint8_t* buffer, int size)
{
    uint8_t cmd[20];
    int chunk;

    if (_debug)
        printf("%s(addr=%#x,size=%#x)\n", __FUNCTION__, addr, size);

    // The SAM firmware has a bug reading powers of 2 over 32 bytes
    // via USB.  If that is the case here, then read the first byte
    // with a readByte and then read one less than the requested size.
    if (_isUsb && _readBufferSize == 0 && size > 32 && !(size & (size - 1)))
    {
        *buffer = readByte(addr);
        addr++;
        buffer++;
        size--;
    }

    while (size > 0)
    {
        // Handle any limitations on the size of the read
        if (_readBufferSize > 0 && size > _readBufferSize)
            chunk = _readBufferSize;
        else
            chunk = size;

        snprintf((char*) cmd, sizeof(cmd), "R%08X,%08X#", addr, chunk);
        if (_port->write(cmd, sizeof(cmd) - 1) != sizeof(cmd) - 1)
            throw SambaError();

        if (_isUsb)
            readBinary(buffer, chunk);
        else
            readXmodem(buffer, chunk);

        size -= chunk;
        addr += chunk;
        buffer += chunk;
    }
}

void
Samba::write(uint32_t addr, const uint8_t* buffer, int size)
{
    uint8_t cmd[20];

    if (_debug)
        printf("%s(addr=%#x,size=%#x)\n", __FUNCTION__, addr, size);

    snprintf((char*) cmd, sizeof(cmd), "S%08X,%08X#", addr, size);
    if (_port->write(cmd, sizeof(cmd) - 1) != sizeof(cmd) - 1)
        throw SambaError();

    // The SAM firmware has a bug that if the command and binary data
    // are received in the same USB data packet, then the firmware
    // gets confused.  Even though the writes are separated in the code,
    // USB drivers often do write combining which can put them together
    // in the same USB data packet.  To avoid this, we call the serial
    // port object's flush method before writing the data.
    if (_isUsb)
    {
        _port->flush();
        writeBinary(buffer, size);
    }
    else
    {
        writeXmodem(buffer, size);
    }
}

void
Samba::go(uint32_t addr)
{
    uint8_t cmd[11];

    if (_debug)
        printf("%s(addr=%#x)\n", __FUNCTION__, addr);

    snprintf((char*) cmd, sizeof(cmd), "G%08X#", addr);
    if (_port->write(cmd, sizeof(cmd) - 1) != sizeof(cmd) - 1)
        throw SambaError();

    // The SAM firmware can get confused if another command is
    // received in the same USB data packet as the go command
    // so we flush after writing the command over USB.
    if (_isUsb)
        _port->flush();
}

std::string
Samba::version()
{
    uint8_t cmd[256];
    char* str;
    int size;
    int pos;

    cmd[0] = 'V';
    cmd[1] = '#';
    _port->write(cmd, 2);

    _port->timeout(TIMEOUT_QUICK);
    size = _port->read(cmd, sizeof(cmd) - 1);
    _port->timeout(TIMEOUT_NORMAL);
    if (size <= 0)
        throw SambaError();

    str = (char*) cmd;
    for (pos = 0; pos < size; pos++)
    {
        if (!isprint(str[pos]))
            break;
    }
    str[pos] = '\0';

    std::string ver(str);

    if (_debug)
        printf("%s()=%s\n", __FUNCTION__, ver.c_str());

    return ver;
}

void
Samba::chipErase(uint32_t start_addr)
{
    if (!_canChipErase)
        throw SambaError();

    uint8_t cmd[64];

    if (_debug)
        printf("%s(addr=%#x)\n", __FUNCTION__, start_addr);

    int l = snprintf((char*) cmd, sizeof(cmd), "X%08X#", start_addr);
    if (_port->write(cmd, l) != l)
        throw SambaError();
    _port->timeout(TIMEOUT_LONG);
    _port->read(cmd, 3); // Expects "X\n\r"
    _port->timeout(TIMEOUT_NORMAL);
    if (cmd[0] != 'X')
        throw SambaError();
}

void
Samba::writeBuffer(uint32_t src_addr, uint32_t dst_addr, uint32_t size)
{
    if (!_canWriteBuffer)
        throw SambaError();

    if (size > checksumBufferSize())
        throw SambaError();
            
    if (_debug)
        printf("%s(scr_addr=%#x, dst_addr=%#x, size=%#x)\n", __FUNCTION__, src_addr, dst_addr, size);

    uint8_t cmd[64];
    int l = snprintf((char*) cmd, sizeof(cmd), "Y%08X,0#", src_addr);
    if (_port->write(cmd, l) != l)
        throw SambaError();
    _port->timeout(TIMEOUT_QUICK);
    cmd[0] = 0;
    _port->read(cmd, 3); // Expects "Y\n\r"
    _port->timeout(TIMEOUT_NORMAL);
    if (cmd[0] != 'Y')
        throw SambaError();

    l = snprintf((char*) cmd, sizeof(cmd), "Y%08X,%08X#", dst_addr, size);
    if (_port->write(cmd, l) != l)
        throw SambaError();
    _port->timeout(TIMEOUT_LONG);
    cmd[0] = 0;
    _port->read(cmd, 3); // Expects "Y\n\r"
    _port->timeout(TIMEOUT_NORMAL);
    if (cmd[0] != 'Y')
        throw SambaError();
}

uint16_t
Samba::checksumBuffer(uint32_t start_addr, uint32_t size)
{
    if (!_canChecksumBuffer)
        throw SambaError();

    if (size > checksumBufferSize())
        throw SambaError();
        
    if (_debug)
        printf("%s(start_addr=%#x, size=%#x) = ", __FUNCTION__, start_addr, size);

    uint8_t cmd[64];
    int l = snprintf((char*) cmd, sizeof(cmd), "Z%08X,%08X#", start_addr, size);
    if (_port->write(cmd, l) != l)
        throw SambaError();
    _port->timeout(TIMEOUT_LONG);
    cmd[0] = 0;
    _port->read(cmd, 12); // Expects "Z00000000#\n\r"
    _port->timeout(TIMEOUT_NORMAL);
    if (cmd[0] != 'Z')
        throw SambaError();

    cmd[9] = 0;
    errno = 0;
    uint32_t res = strtol((char*) &cmd[1], NULL, 16);
    if (errno != 0)
        throw SambaError();
    if (_debug)
        printf("%x\n", res);
    return res;
}

