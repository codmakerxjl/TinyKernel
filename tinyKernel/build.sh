#!/usr/bin/bash
nasm -f elf32 -g -F dwarf -o testfont.o ../include/drivers/video/testfont.asm
nasm -I ../include/gdt -f elf32 -g -F dwarf -o boot.o ../arch/i386/boot.asm
nasm -I ../include/gdt -f elf32 -g -F dwarf -o vesadrv.o ../drivers/video/vesadrv.asm
i686-elf-gcc -I ../include/drivers/video -I ../include/gdt -std=c99 -g -m32 -O3 -c -fno-exceptions -nostdlib -ffreestanding -Wall -Wextra -o  kernel.o ../kernel/kernel.c
i686-elf-gcc -I ../include/gdt -std=c99 -g -m32 -O3 -c -fno-exceptions -nostdlib -ffreestanding -Wall -Wextra -pedantic -o gdt.o ../gdt/gdt.c -lgcc
i686-elf-gcc -m32 -T ../arch/i386/linker.ld -ffreestanding -nostdlib -o myos.bin boot.o kernel.o gdt.o vesadrv.o testfont.o -lgcc

qemu-system-i386 -kernel myos.bin