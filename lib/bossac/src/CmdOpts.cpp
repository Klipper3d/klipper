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
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <assert.h>

#include "CmdOpts.h"

CmdOpts::CmdOpts(int argc, char* argv[], int numOpts, Option* opts) :
    _argc(argc), _argv(argv), _numOpts(numOpts), _opts(opts)
{
}

CmdOpts::~CmdOpts()
{
}

void
CmdOpts::usage(FILE* out)
{
    int optIdx;
    char name[40];
    const char* start;
    const char* end;

    for (optIdx = 0; optIdx < _numOpts; optIdx++)
    {
        if (_opts[optIdx].arg.has == ArgOptional)
            snprintf(name, sizeof(name), "  -%c, --%s[=%s]",
                    _opts[optIdx].letter,
                    _opts[optIdx].name,
                    _opts[optIdx].arg.name);
        else if (_opts[optIdx].arg.has == ArgRequired)
            snprintf(name, sizeof(name), "  -%c, --%s=%s",
                     _opts[optIdx].letter,
                     _opts[optIdx].name,
                     _opts[optIdx].arg.name);
        else
            snprintf(name, sizeof(name), "  -%c, --%s",
                     _opts[optIdx].letter,
                     _opts[optIdx].name);

        fprintf(out, "%-23s ", name);

        start = _opts[optIdx].help;
        while ((end = strchr(start, '\n')))
        {
            fwrite(start, end - start + 1, 1, out);
            fprintf(out, "%24s", "");
            start = end + 1;
        }
        fprintf(out, "%s\n", start);
    }
}

int
CmdOpts::parse()
{
    struct option long_opts[_numOpts + 1];
    char optstring[_numOpts * 3 + 1];
    char* optPtr = optstring;
    int optIdx;
    int rc;

    for (optIdx = 0; optIdx < _numOpts; optIdx++)
    {
        *_opts[optIdx].present = false;

        *optPtr++ = _opts[optIdx].letter;
        long_opts[optIdx].name = _opts[optIdx].name;
        switch (_opts[optIdx].arg.has)
        {
        default:
        case ArgNone:
            long_opts[optIdx].has_arg = no_argument;
            break;
        case ArgOptional:
            long_opts[optIdx].has_arg = optional_argument;
            *optPtr++ = ':';
            *optPtr++ = ':';
            break;
        case ArgRequired:
            long_opts[optIdx].has_arg = required_argument;
            *optPtr++ = ':';
            break;
        }
        long_opts[optIdx].flag = NULL;
        long_opts[optIdx].val = 0;
    }

    memset(&long_opts[_numOpts], 0, sizeof(long_opts[_numOpts]));
    *optPtr = '\0';
    optIdx = 0;
    while ((rc = getopt_long(_argc, _argv, optstring, long_opts, &optIdx)) != -1)
    {
        if (rc == '?')
            return -1;

        if (rc != 0)
           optIdx = find(rc);

        assert(optIdx >= 0 && optIdx < _numOpts);
        *_opts[optIdx].present = true;
        if (_opts[optIdx].arg.has != ArgNone && optarg)
        {
            switch (_opts[optIdx].arg.type)
            {
            case ArgInt:
                *_opts[optIdx].arg.value.intPtr = strtol(optarg, NULL, 0);
                break;
            default:
            case ArgString:
                *_opts[optIdx].arg.value.strPtr = optarg;
                break;
            }
        }
    }

    return optind;
}

int
CmdOpts::find(char letter)
{
    int optIdx;

    for (optIdx = 0; optIdx < _numOpts; optIdx++)
        if (_opts[optIdx].letter == letter)
            break;

    return optIdx;
}
