#!/bin/bash

CC=gcc
CFLAGS="-Wall -Wextra -std=c11"

SRC="ramfs.c"
OUT="ramfs"

rm -f $OUT

$CC $CFLAGS -o $OUT $SRC

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "You can now run the program using ./$OUT"
else
    echo "Build failed!"
fi
