// Python CFFI bindings for fatfs
//
// Copyright (C) 2021 Eric Callahan <arksine.code@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h>                     // memset
#include <stdlib.h>                     // malloc
#include "fatfs_api.h"                  // python-fatfs prototypes
#include "../../lib/fatfs/ff.h"         // fatfs APIs
#include "../../lib/fatfs/diskio.h"     // fatfs media APIs (callbacks)

/* Callbacks */
static uint8_t (*python_disk_status)(void) = 0;
static uint8_t (*python_disk_initialize)(void) = 0;
static uint8_t (*python_disk_read)(uint8_t* buff, uint32_t sector,
                                   unsigned int count) = 0;
static uint8_t (*python_disk_write)(const uint8_t* buff, uint32_t sector,
                                    unsigned int count) = 0;
static uint8_t (*python_disk_ioctl)(uint8_t cmd, void* buff) = 0;
static uint32_t (*python_fattime)(void) = 0;

void __visible
fatfs_set_callbacks(
    uint8_t (*status_callback)(void),
    uint8_t (*init_callback)(void),
    uint8_t (*read_callback)(uint8_t*, uint32_t, unsigned int),
    uint8_t (*write_callback)(const uint8_t*, uint32_t, unsigned int),
    uint8_t (*ioctl_callback)(uint8_t, void*),
    uint32_t (*fattime_callback)(void)
)
{
    python_disk_status = status_callback;
    python_disk_initialize = init_callback;
    python_disk_read = read_callback;
    python_disk_write = write_callback;
    python_disk_ioctl = ioctl_callback;
    python_fattime = fattime_callback;
}

void __visible
fatfs_clear_callbacks(void)
{
    python_disk_status = 0;
    python_disk_initialize = 0;
    python_disk_read = 0;
    python_disk_write = 0;
    python_disk_ioctl = 0;
    python_fattime = 0;
}

/*  Callbacks from fatfs to python.  These methods are used to  */
/*  Access access the SD Card APIs                              */

/*  Get FAT Time  */
DWORD
get_fattime(void)
{
    if (python_fattime == 0)
    {
        // Return a default FATTIME of 1/1/2021
        return 41 << 25 | 1 << 21 | 1 << 16;
    }
    return python_fattime();
}

DSTATUS
disk_status(BYTE pdrv)
{
    if (python_disk_status != 0)
        return python_disk_status();

    return STA_NOINIT;
}

DSTATUS
disk_initialize(BYTE pdrv)
{
    if (python_disk_initialize != 0)
        return python_disk_initialize();
    return STA_NOINIT;
}


DRESULT
disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count)
{
    if (python_disk_read != 0)
        return python_disk_read(buff, sector, count);
    return RES_NOTRDY;
}


DRESULT
disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count)
{
    if (python_disk_write != 0)
        return python_disk_write(buff, sector, count);
    return RES_NOTRDY;
}

DRESULT
disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    if (python_disk_ioctl != 0)
        return python_disk_ioctl(cmd, buff);
    return RES_NOTRDY;
}

/*  Python-FatFS Interface Functions  */

// Wrapper around the FatFS FIL struct.  FIL is type defined
// anonymously, thus can't be forward declared and poses
// challenges with CFFI.
struct ff_file {
    FIL fobj;
};
static FATFS fs;

uint8_t __visible
fatfs_mount(void)
{
    FRESULT res;
    memset(&fs, 0, sizeof(fs));
    res = f_mount(&fs, "", 1);
    return res;
}

uint8_t __visible
fatfs_unmount(void)
{
    FRESULT res;
    res = f_unmount("");
    memset(&fs, 0, sizeof(fs));
    return res;
}

struct ff_file* __visible
fatfs_open(const char* path, uint8_t mode)
{
    FRESULT res;
    struct ff_file* fhdl = malloc(sizeof(*fhdl));
    memset(fhdl, 0, sizeof(*fhdl));
    res = f_open(&(fhdl->fobj), path, mode);
    if (res != FR_OK) {
        free(fhdl);
        fhdl = NULL;
    }
    return fhdl;
}

uint8_t __visible
fatfs_close(struct ff_file* fhdl)
{
    FRESULT res;
    res = f_close(&(fhdl->fobj));
    free(fhdl);
    return res;
}

int __visible
fatfs_read(struct ff_file* fhdl, void* rbuf, uint16_t btr)
{
    FRESULT res;
    UINT bytes_read;
    res = f_read(&(fhdl->fobj), rbuf, btr, &bytes_read);
    if (res != FR_OK) {
        return -res;
    }
    return bytes_read;
}

int __visible
fatfs_write(struct ff_file* fhdl, const void* wbuf, uint16_t btw)
{
    FRESULT res;
    UINT bytes_written;
    res = f_write(&(fhdl->fobj), wbuf, btw, &bytes_written);
    if (bytes_written < btw) {
        return -res;
    }
    return bytes_written;
}

// Remove a file or director
uint8_t __visible
fatfs_remove(const char* path)
{
    FRESULT res;
    res = f_unlink(path);
    return res;
}

uint8_t __visible
fatfs_get_fstats(struct ff_file_info* finfo, const char* path)
{
    FRESULT res;
    FILINFO nfo;
    res = f_stat(path, &nfo);
    if (res == FR_OK)
        memcpy(finfo, &nfo, sizeof(nfo));
    return res;
}

uint8_t __visible
fatfs_get_disk_info(struct ff_disk_info* dinfo)
{
    FRESULT res;
    res = f_getlabel("", dinfo->label, &(dinfo->serial_number));
    if (res != FR_OK)
        return res;
    dinfo->fs_type = fs.fs_type;
    return res;
}

uint8_t __visible
fatfs_list_dir(struct ff_file_info* flist, uint8_t max_size, char* path)
{
    FRESULT res;
    DIR dir;
    FILINFO nfo;
    res = f_opendir(&dir, path);
    if (res == FR_OK) {
        for (uint8_t i=0; i < max_size; i++) {
            res = f_readdir(&dir, &nfo);
            if (res != FR_OK ||nfo.fname[0] == 0)
                break;
            memcpy(&(flist[i]), &nfo, sizeof(nfo));
        }
    }
    return FR_OK;
}
