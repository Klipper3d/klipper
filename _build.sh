#!/bin/bash

# in case build is executed from outside current dir be a gem and change the dir
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd -P)"
cd $SCRIPT_DIR

cp .config.$1 .config
mkdir -p outfw/
make clean
make 
if [ "$1" = "btteddy" ]; then
    mv out/klipper.uf2 outfw/btteddy.uf2
else
    mv out/${1}*.bin outfw/
fi
