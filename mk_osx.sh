#!/bin/sh

. common.sh

PLATFORM="osx"

CFLAGS="-Wall -O2 -Isrc"

export MACOSX_DEPLOYMENT_TARGET="10.11.0"

clang -c -o ${NAME}.${PLATFORM}.${VERSION}.o ${CFLAGS} src/os-metrics.c
ar -qc lib${NAME}.${PLATFORM}.${VERSION}.a ${NAME}.${PLATFORM}.${VERSION}.o
ranlib lib${NAME}.${PLATFORM}.${VERSION}.a

