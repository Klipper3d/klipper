// Python CFFI definitions for fatfs
//
// Copyright (C) 2021 Eric Callahan <arksine.code@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.
#ifndef FATFS_API_H
#define FATFS_API_H

#include <stdint.h> // uint8_t

#define __visible __attribute__((externally_visible))

void fatfs_set_callbacks(
    uint8_t (*status_callback)(void),
    uint8_t (*init_callback)(void),
    uint8_t (*read_callback)(uint8_t*, uint32_t, unsigned int),
    uint8_t (*write_callback)(const uint8_t*, uint32_t, unsigned int),
    uint8_t (*ioctl_callback)(uint8_t, void*),
    uint32_t (*fattime_callback)(void));
void fatfs_clear_callbacks(void);

struct ff_file;

struct ff_file_info {
    uint32_t size;
    uint16_t modified_date;
    uint16_t modified_time;
    uint8_t  attrs;
    char     name[13];
};

struct ff_disk_info {
    char     label[12];
    uint32_t serial_number;
    uint8_t  fs_type;
};

uint8_t fatfs_mount(void);
uint8_t fatfs_unmount(void);
struct ff_file* fatfs_open(const char* path, uint8_t mode);
uint8_t fatfs_close(struct ff_file* fhdl);
int fatfs_read(struct ff_file* fhdl, void* rbuf, uint16_t btr);
int fatfs_write(struct ff_file* fhdl, const void* wbuf, uint16_t btw);
uint8_t fatfs_remove(const char* path);
uint8_t fatfs_get_fstats(struct ff_file_info* finfo, const char* path);
uint8_t fatfs_get_disk_info(struct ff_disk_info* dinfo);
uint8_t fatfs_list_dir(struct ff_file_info* flist, uint8_t max_size,
                       char* path);

#endif
