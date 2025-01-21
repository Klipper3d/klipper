# FatFS CFFI support
#
# Copyright (C) 2021 Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os
import sys
KLIPPER_DIR = os.path.abspath(os.path.join(
    os.path.dirname(__file__), "../../"))
sys.path.append(os.path.join(KLIPPER_DIR, "klippy"))
import chelper

DEST_LIB = "fatfs.so"
FATFS_DIR = os.path.join(KLIPPER_DIR, "lib/fatfs")
FATFS_SRC = ["ff.c", "ffsystem.c", "ffunicode.c"]
SPI_FLASH_SRC = ["fatfs_api.c"]
FATFS_HEADERS = ["diskio.h", "ff.h", "ffconf.h"]
SPI_FLASH_HEADERS = ["fatfs_api.h"]

FATFS_CDEFS = """
    void fatfs_set_callbacks(
        uint8_t (*status_callback)(void),
        uint8_t (*init_callback)(void),
        uint8_t (*read_callback)(uint8_t*, uint32_t, unsigned int),
        uint8_t (*write_callback)(const uint8_t*, uint32_t, unsigned int),
        uint8_t (*ioctl_callback)(uint8_t, void*),
        uint32_t (*fattime_callback)(void));
    void fatfs_clear_callbacks(void);

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
"""

fatfs_ffi_main = None
fatfs_ffi_lib = None

def check_fatfs_build(printfunc=lambda o: o):
    printfunc("Checking FatFS CFFI Build...\n")
    ffi_main, ffi_lib = chelper.get_ffi()
    srcdir = os.path.dirname(os.path.realpath(__file__))
    srcfiles = chelper.get_abs_files(FATFS_DIR, FATFS_SRC)
    srcfiles.extend(chelper.get_abs_files(srcdir, SPI_FLASH_SRC))
    ofiles = chelper.get_abs_files(FATFS_DIR, FATFS_HEADERS)
    ofiles.extend(chelper.get_abs_files(srcdir, SPI_FLASH_HEADERS))
    destlib = os.path.join(srcdir, DEST_LIB)
    if chelper.check_build_code(srcfiles+ofiles+[__file__], destlib):
        if chelper.check_gcc_option(chelper.SSE_FLAGS):
            cmd = "%s %s %s" % (chelper.GCC_CMD, chelper.SSE_FLAGS,
                                chelper.COMPILE_ARGS)
        else:
            cmd = "%s %s" % (chelper.GCC_CMD, chelper.COMPILE_ARGS)
        printfunc("Building FatFS shared library...")
        chelper.do_build_code(cmd % (destlib, ' '.join(srcfiles)))
        printfunc("Done\n")
    global fatfs_ffi_main, fatfs_ffi_lib
    ffi_main.cdef(FATFS_CDEFS)
    fatfs_ffi_lib = ffi_main.dlopen(destlib)
    fatfs_ffi_main = ffi_main

def get_fatfs_ffi():
    global fatfs_ffi_main, fatfs_ffi_lib
    if fatfs_ffi_main is None:
        check_fatfs_build()
    return fatfs_ffi_main, fatfs_ffi_lib
