#ifndef __BYTEORDER_H
#define __BYTEORDER_H

#include <stdint.h> // uint32_t

#define swab16(x) __builtin_bswap16(x)
#define swab32(x) __builtin_bswap32(x)
#define swab64(x) __builtin_bswap64(x)

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

static inline uint16_t cpu_to_le16(uint16_t x) {
    return x;
}
static inline uint32_t cpu_to_le32(uint32_t x) {
    return x;
}
static inline uint64_t cpu_to_le64(uint64_t x) {
    return x;
}
static inline uint16_t le16_to_cpu(uint16_t x) {
    return x;
}
static inline uint32_t le32_to_cpu(uint32_t x) {
    return x;
}
static inline uint64_t le64_to_cpu(uint64_t x) {
    return x;
}

static inline uint16_t cpu_to_be16(uint16_t x) {
    return swab16(x);
}
static inline uint32_t cpu_to_be32(uint32_t x) {
    return swab32(x);
}
static inline uint64_t cpu_to_be64(uint64_t x) {
    return swab64(x);
}
static inline uint16_t be16_to_cpu(uint16_t x) {
    return swab16(x);
}
static inline uint32_t be32_to_cpu(uint32_t x) {
    return swab32(x);
}
static inline uint64_t be64_to_cpu(uint64_t x) {
    return swab64(x);
}

#else // big endian

static inline uint16_t cpu_to_le16(uint16_t x) {
    return swab16(x);
}
static inline uint32_t cpu_to_le32(uint32_t x) {
    return swab32(x);
}
static inline uint64_t cpu_to_le64(uint64_t x) {
    return swab64(x);
}
static inline uint16_t le16_to_cpu(uint16_t x) {
    return swab16(x);
}
static inline uint32_t le32_to_cpu(uint32_t x) {
    return swab32(x);
}
static inline uint64_t le64_to_cpu(uint64_t x) {
    return swab64(x);
}

static inline uint16_t cpu_to_be16(uint16_t x) {
    return x;
}
static inline uint32_t cpu_to_be32(uint32_t x) {
    return x;
}
static inline uint64_t cpu_to_be64(uint64_t x) {
    return x;
}
static inline uint16_t be16_to_cpu(uint16_t x) {
    return x;
}
static inline uint32_t be32_to_cpu(uint32_t x) {
    return x;
}
static inline uint64_t be64_to_cpu(uint64_t x) {
    return x;
}

#endif

#endif // byteorder.h
