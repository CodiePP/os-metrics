#!/bin/sh

. ./common.sh

PLATFORM="linux"

CFLAGS="-Wall -O2 -Isrc"

if [ -e lib${NAME}.${PLATFORM}.${VERSION}.a ]; then
  rm lib${NAME}.${PLATFORM}.${VERSION}.a
fi
gcc -c -o ${NAME}.${PLATFORM}.${VERSION}.o ${CFLAGS} src/os-metrics.c
ar -qc lib${NAME}.${PLATFORM}.${VERSION}.a ${NAME}.${PLATFORM}.${VERSION}.o
ranlib lib${NAME}.${PLATFORM}.${VERSION}.a
rm ${NAME}.${PLATFORM}.${VERSION}.o

