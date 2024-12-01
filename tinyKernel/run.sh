#!/usr/bin/bash

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

qemu-system-i386 -kernel "$SCRIPT_DIR/build/myos.bin"
 