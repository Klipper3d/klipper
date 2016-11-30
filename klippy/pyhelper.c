// Helper functions for C / Python interface
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <errno.h> // errno
#include <stdint.h> // uint8_t
#include <stdio.h> // fprintf
#include <string.h> // strerror
#include <sys/time.h> // gettimeofday
#include <time.h> // struct timespec
#include "pyhelper.h" // get_time

// Return the current system time as a double
double
get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.;
}

// Fill a 'struct timespec' with a system time stored in a double
struct timespec
fill_time(double time)
{
    time_t t = time;
    return (struct timespec) {t, (time - t)*1000000000. };
}

// Report 'errno' in a message written to stderr
void
report_errno(char *where, int rc)
{
    int e = errno;
    fprintf(stderr, "Got error %d in %s: (%d)%s\n", rc, where, e, strerror(e));
}

// Return a hex character for a given number
#define GETHEX(x) ((x) < 10 ? '0' + (x) : 'e' + (x) - 10)

// Translate a binary string into an ASCII string with escape sequences
char *
dump_string(char *outbuf, int outbuf_size, char *inbuf, int inbuf_size)
{
    char *outend = &outbuf[outbuf_size-5], *o = outbuf;
    uint8_t *inend = (void*)&inbuf[inbuf_size], *p = (void*)inbuf;
    while (p < inend && o < outend) {
        uint8_t c = *p++;
        if (c > 31 && c < 127 && c != '\\') {
            *o++ = c;
            continue;
        }
        *o++ = '\\';
        *o++ = 'x';
        *o++ = GETHEX(c >> 4);
        *o++ = GETHEX(c & 0x0f);
    }
    *o = '\0';
    return outbuf;
}
