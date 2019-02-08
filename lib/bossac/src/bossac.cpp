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
#include <string>
#include <exception>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>

#include "CmdOpts.h"
#include "Samba.h"
#include "PortFactory.h"
#include "Device.h"
#include "Flasher.h"

using namespace std;

class BossaConfig
{
public:
    BossaConfig();
    virtual ~BossaConfig() {}

    bool erase;
    bool write;
    bool read;
    bool verify;
    bool offset;
    bool reset;
    bool port;
    bool boot;
    bool bor;
    bool bod;
    bool lock;
    bool unlock;
    bool security;
    bool info;
    bool debug;
    bool help;
    bool usbPort;
    bool arduinoErase;

    int readArg;
    int offsetArg;
    string portArg;
    int bootArg;
    int bodArg;
    int borArg;
    string lockArg;
    string unlockArg;
    bool usbPortArg;
};

BossaConfig::BossaConfig()
{
    erase = false;
    write = false;
    read = false;
    verify = false;
    port = false;
    boot = false;
    bod = false;
    bor = false;
    lock = false;
    security = false;
    info = false;
    help = false;
    usbPort = false;
    arduinoErase = false;

    readArg = 0;
    offsetArg = 0;
    bootArg = 1;
    bodArg = 1;
    borArg = 1;
    usbPortArg=1;

    reset = false;
}

class BossaObserver : public FlasherObserver
{
public:
    BossaObserver() : _lastTicks(-1) {}
    virtual ~BossaObserver() {}
    
    virtual void onStatus(const char *message, ...);
    virtual void onProgress(int num, int div);
private:
    int _lastTicks;
};

void
BossaObserver::onStatus(const char *message, ...)
{
    va_list ap;
    
    va_start(ap, message);
    vprintf(message, ap);
    va_end(ap);
}

void
BossaObserver::onProgress(int num, int div)
{
    int ticks;
    int bars = 30;

    ticks = num * bars / div;
    
    if (ticks == _lastTicks)
        return;
    
    printf("\r[");
    while (ticks-- > 0)
    {
        putchar('=');
        bars--;
    }
    while (bars-- > 0)
    {
        putchar(' ');
    }
    printf("] %d%% (%d/%d pages)", num * 100 / div, num, div);
    fflush(stdout);
    
    _lastTicks = 0;
}

static BossaConfig config;
static Option opts[] =
{
    {
      'e', "erase", &config.erase,
      { ArgNone },
      "erase the entire flash starting at the offset"
    },
    {
      'w', "write", &config.write,
      { ArgNone },
      "write FILE to the flash; accelerated when\n"
      "combined with erase option"
    },
    {
      'r', "read", &config.read,
      { ArgOptional, ArgInt, "SIZE", { &config.readArg } },
      "read SIZE from flash and store in FILE;\n"
      "read entire flash if SIZE not specified"
    },
    {
      'v', "verify", &config.verify,
      { ArgNone },
      "verify FILE matches flash contents"
    },
    {
      'o', "offset", &config.offset,
      { ArgRequired, ArgInt, "OFFSET", { &config.offsetArg } },
      "start erase/write/read/verify operation at flash OFFSET;\n"
      "OFFSET must be aligned to a flash page boundary"
    },
    {
      'p', "port", &config.port,
      { ArgRequired, ArgString, "PORT", { &config.portArg } },
      "use serial PORT to communicate to device;\n"
      "default behavior is to use first serial port"
    },
    {
      'b', "boot", &config.boot,
      { ArgOptional, ArgInt, "BOOL", { &config.bootArg } },
      "boot from ROM if BOOL is 0;\n"
      "boot from FLASH if BOOL is 1 [default];\n"
      "option is ignored on unsupported devices"
    },
    {
      'c', "bod", &config.bod,
      { ArgOptional, ArgInt, "BOOL", { &config.bodArg } },
      "no brownout detection if BOOL is 0;\n"
      "brownout detection is on if BOOL is 1 [default]"
    },
    {
      't', "bor", &config.bor,
      { ArgOptional, ArgInt, "BOOL", { &config.borArg } },
      "no brownout reset if BOOL is 0;\n"
      "brownout reset is on if BOOL is 1 [default]"
    },
    {
      'l', "lock", &config.lock,
      { ArgOptional, ArgString, "REGION", { &config.lockArg } },
      "lock the flash REGION as a comma-separated list;\n"
      "lock all if not given [default]"
    },
    {
      'u', "unlock", &config.unlock,
      { ArgOptional, ArgString, "REGION", { &config.unlockArg } },
      "unlock the flash REGION as a comma-separated list;\n"
      "unlock all if not given [default]"
    },
    {
      's', "security", &config.security,
      { ArgNone },
      "set the flash security flag"
    },
    {
      'i', "info", &config.info,
      { ArgNone },
      "display device information"
    },
    {
      'd', "debug", &config.debug,
      { ArgNone },
      "print debug messages"
    },
    {
      'h', "help", &config.help,
      { ArgNone },
      "display this help text"
    },
    {
      'U', "usb-port", &config.usbPort,
      { ArgOptional, ArgInt, "BOOL", { &config.usbPortArg } },
      "force serial port detection to USB if BOOL is 1 [default]\n"
      "or to RS-232 if BOOL is 0"
    },
    {
      'R', "reset", &config.reset,
      { ArgNone },
      "reset CPU (if supported)"
    },
    {
      'a', "arduino-erase", &config.arduinoErase,
      { ArgNone },
      "erase and reset via Arduino 1200 baud hack"
    }
};

int
help(const char* program)
{
    fprintf(stderr, "Try '%s -h' or '%s --help' for more information\n", program, program);
    return 1;
}

static struct timeval start_time;

void
timer_start()
{
    gettimeofday(&start_time, NULL);
}

float
timer_stop()
{
    struct timeval end;
    gettimeofday(&end, NULL);
    return (end.tv_sec - start_time.tv_sec) + (end.tv_usec - start_time.tv_usec) / 1000000.0;
}

int
main(int argc, char* argv[])
{
    int args;
    char* pos;
    CmdOpts cmd(argc, argv, sizeof(opts) / sizeof(opts[0]), opts);

    if ((pos = strrchr(argv[0], '/')) || (pos = strrchr(argv[0], '\\')))
        argv[0] = pos + 1;

    if (argc <= 1)
    {
        fprintf(stderr, "%s: you must specify at least one option\n", argv[0]);
        return help(argv[0]);
    }

    args = cmd.parse();
    if (args < 0)
        return help(argv[0]);

    if (config.read && (config.write || config.verify))
    {
        fprintf(stderr, "%s: read option is exclusive of write or verify\n", argv[0]);
        return help(argv[0]);
    }

    if (config.read || config.write || config.verify)
    {
        if (args == argc)
        {
            fprintf(stderr, "%s: missing file\n", argv[0]);
            return help(argv[0]);
        }
        argc--;
    }
    if (args != argc)
    {
        fprintf(stderr, "%s: extra arguments found\n", argv[0]);
        return help(argv[0]);
    }

    if (config.help)
    {
        printf("Usage: %s [OPTION...] [FILE]\n", argv[0]);
        printf("Basic Open Source SAM-BA Application (BOSSA) Version " VERSION "\n"
               "Flash programmer for Atmel SAM devices.\n"
               "Copyright (c) 2011-2018 ShumaTech (http://www.shumatech.com)\n"
               "\n"
               "Examples:\n"
               "  bossac -e -w -v -b image.bin   # Erase flash, write flash with image.bin,\n"
               "                                 # verify the write, and set boot from flash\n"
               "  bossac -r0x10000 image.bin     # Read 64KB from flash and store in image.bin\n"
              );
        printf("\nOptions:\n");
        cmd.usage(stdout);
        printf("\nReport bugs to <bugs@shumatech.com>\n");
        return 1;
    }

    try
    {
        Samba samba;
        PortFactory portFactory;

        if (config.debug)
            samba.setDebug(true);

        if (!config.port)
            config.portArg = portFactory.def();

        if (config.arduinoErase)
        {
            SerialPort::Ptr port;
            port = portFactory.create(config.portArg, config.usbPortArg != 0);

            if(!port->open(1200))
            {
                fprintf(stderr, "Failed to open port at 1200bps\n");
                return 1;
            }

            port->setRTS(true);
            port->setDTR(false);
            port->close();
        }

        if (config.portArg.empty())
        {
            fprintf(stderr, "No serial ports available\n");
            return 1;
        }

        bool res;
        if (config.usbPort)
            res = samba.connect(portFactory.create(config.portArg, config.usbPortArg != 0));
        else
            res = samba.connect(portFactory.create(config.portArg));
        if (!res)
        {
            fprintf(stderr, "No device found on %s\n", config.portArg.c_str());
            return 1;
        }

        Device device(samba);
        device.create();

        Device::FlashPtr& flash = device.getFlash();

        BossaObserver observer;
        Flasher flasher(samba, device, observer);

        if (config.info)
        {
            FlasherInfo info;
            flasher.info(info);
            info.print();
        }

        if (config.unlock)
            flasher.lock(config.unlockArg, false);

        if (config.erase)
        {
            timer_start();
            flasher.erase(config.offsetArg);
            printf("\nDone in %5.3f seconds\n", timer_stop());
        }

        if (config.write)
        {
            timer_start();
            flasher.write(argv[args], config.offsetArg);
            printf("\nDone in %5.3f seconds\n", timer_stop());
        }

        if (config.verify)
        {
            uint32_t pageErrors;
            uint32_t totalErrors;

            timer_start();
            if (!flasher.verify(argv[args], pageErrors, totalErrors, config.offsetArg))
            {
                printf("\nVerify failed\nPage errors: %d\nByte errors: %d\n",
                    pageErrors, totalErrors);
                return 2;
            }

            printf("\nVerify successful\nDone in %5.3f seconds\n", timer_stop());
        }

        if (config.read)
        {
            timer_start();
            flasher.read(argv[args], config.readArg, config.offsetArg);
            printf("\nDone in %5.3f seconds\n", timer_stop());
        }

        if (config.boot)
        {
            printf("Set boot flash %s\n", config.bootArg ? "true" : "false");
            flash->setBootFlash(config.bootArg);
        }

        if (config.bod)
        {
            printf("Set brownout detect %s\n", config.bodArg ? "true" : "false");
            flash->setBod(config.bodArg);
        }

        if (config.bor)
        {
            printf("Set brownout reset %s\n", config.borArg ? "true" : "false");
            flash->setBor(config.borArg);
        }

        if (config.security)
        {
            printf("Set security\n");
            flash->setSecurity();
        }

        if (config.lock)
            flasher.lock(config.lockArg, true);

        flash->writeOptions();

        if (config.reset)
            device.reset();
    }
    catch (exception& e)
    {
        fprintf(stderr, "\n%s\n", e.what());
        return 1;
    }
    catch(...)
    {
        fprintf(stderr, "\nUnhandled exception\n");
        return 1;
    }

    return 0;
}

