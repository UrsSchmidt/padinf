#!/bin/bash

if [ -f '/usr/local/bin/padinf' ]; then
    rm '/usr/local/bin/padinf'
fi

if [ -f '/usr/local/share/man/man1/padinf.1' ]; then
    rm '/usr/local/share/man/man1/padinf.1'
    mandb -q
fi
