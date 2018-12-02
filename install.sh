#!/bin/bash

if [ ! -f 'padinf' ]; then
    ./build.sh
fi

if [ -f 'padinf' ]; then
    if [ -d '/usr/local/bin/' ]; then
        cp 'padinf' '/usr/local/bin/'
    fi
fi

if [ -d '/usr/local/share/man/man1/' ]; then
    cp 'padinf.1' '/usr/local/share/man/man1/'
    mandb -q
fi
