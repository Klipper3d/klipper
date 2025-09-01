#!/bin/bash
# Check if a binary appears to have a software library divide operator

CFGFILE="$1"
ELFOBJ="$2"
OBJDUMP=objdump

objdump -t ${ELFOBJ} | grep -Eq '\<(__[a-z0-9]*div|__[a-z0-9]*mod)'
if [ $? -eq 0 ]; then

    if grep -Eq '^CONFIG_HAVE_SOFTWARE_DIVIDE_REQUIRED=y$' ${CFGFILE}; then
        echo ""
        echo "Software divide detected and that is normal for this chip"
        echo ""
        exit 0
    fi

    echo ""
    echo "ERROR: A software run-time divide operation was found"
    echo ""
    exit 99
fi
