#ifndef _ADDRESSES_H
#define _ADDRESSES_H

#define ROM_START               0x00000000 // same as ROM_BASE in addressmap.h
#define ROM_END_RP2040          0x00004000
#define ROM_END_RP2350          0x00008000
// todo amy based on what sort of elf (also this breaks RP2040 builds?)
#define FLASH_START             0x10000000 // same as XIP_MAIN_BASE in addressmap.h
#define FLASH_END_RP2040        0x11000000 // +32 MiB -- remainder has no external devices mapped
#define FLASH_END_RP2350        0x12000000 // +32 MiB -- remainder has no external devices mapped
// todo amy based on what sort of elf
#define XIP_SRAM_START_RP2040   0x15000000
#define XIP_SRAM_END_RP2040     0x15004000
#define XIP_SRAM_START_RP2350   0x13ffc000 // same as XIP_SRAM_BASE in addressmap.h
#define XIP_SRAM_END_RP2350     0x14000000 // same as XIP_SRAM_END in addressmap.h

#define SRAM_START              0x20000000 // same as SRAM_BASE in addressmap.h
#define SRAM_END_RP2040         0x20042000
#define SRAM_END_RP2350         0x20082000
// todo amy no more banked alias
#define MAIN_RAM_BANKED_START   0x21000000
#define MAIN_RAM_BANKED_END     0x21040000


#ifdef __cplusplus

#include <cstdint>
#include <vector>

#ifdef _WIN32
#undef IGNORE
#endif

// Address ranges for RP2040/RP2350
struct address_range {
    enum type {
        CONTENTS,     // may have contents
        NO_CONTENTS,  // must be uninitialized
        IGNORE        // will be ignored
    };
    address_range(uint32_t from, uint32_t to, type type) : from(from), to(to), type(type) {}
    address_range() : address_range(0, 0, IGNORE) {}
    uint32_t from;
    uint32_t to;
    type type;
};

typedef std::vector<address_range> address_ranges;


const address_ranges rp2040_address_ranges_flash {
    address_range(FLASH_START, FLASH_END_RP2040, address_range::type::CONTENTS),
    address_range(SRAM_START, SRAM_END_RP2040, address_range::type::NO_CONTENTS),
    address_range(MAIN_RAM_BANKED_START, MAIN_RAM_BANKED_END, address_range::type::NO_CONTENTS)
};

const address_ranges rp2040_address_ranges_ram {
    address_range(SRAM_START, SRAM_END_RP2040, address_range::type::CONTENTS),
    address_range(XIP_SRAM_START_RP2040, XIP_SRAM_END_RP2040, address_range::type::CONTENTS),
    address_range(ROM_START, ROM_END_RP2040, address_range::type::IGNORE) // for now we ignore the bootrom if present
};

const address_ranges rp2350_address_ranges_flash {
    address_range(FLASH_START, FLASH_END_RP2350, address_range::type::CONTENTS),
    address_range(SRAM_START, SRAM_END_RP2350, address_range::type::NO_CONTENTS),
    address_range(MAIN_RAM_BANKED_START, MAIN_RAM_BANKED_END, address_range::type::NO_CONTENTS)
};

const address_ranges rp2350_address_ranges_ram {
    address_range(SRAM_START, SRAM_END_RP2350, address_range::type::CONTENTS),
    address_range(XIP_SRAM_START_RP2350, XIP_SRAM_END_RP2350, address_range::type::CONTENTS),
    address_range(ROM_START, ROM_END_RP2350, address_range::type::IGNORE) // for now we ignore the bootrom if present
};

static bool is_address_valid(const address_ranges& valid_ranges, uint32_t addr) {
    for(const auto& range : valid_ranges) {
        if (range.from <= addr && range.to > addr) {
            return true;
        }
    }
    return false;
}

static bool is_address_initialized(const address_ranges& valid_ranges, uint32_t addr) {
    for(const auto& range : valid_ranges) {
        if (range.from <= addr && range.to > addr) {
            return address_range::type::CONTENTS == range.type;
        }
    }
    return false;
}

#endif
#endif
