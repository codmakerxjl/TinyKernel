# 变量定义
ASM = nasm
CC = i686-elf-gcc
LD = i686-elf-gcc
ASM_FLAGS = -I ./include/gdt -f elf32 -g -F dwarf
C_FLAGS = -I ./include/drivers/video -I ./include/gdt -std=c99 -g -m32 -O3 -c -fno-exceptions -nostdlib -ffreestanding -Wall -Wextra -pedantic
LD_FLAGS = -m32 -T ./arch/i386/linker.ld -ffreestanding -nostdlib
LIBS = -lgcc

# 源文件
ASM_SRC = $(wildcard include/drivers/video/*.asm) \
		$(wildcard arch/i386/*.asm) \
		$(wildcard drivers/video/*.asm)

C_SRC = $(wildcard kernel/*.c) \
		$(wildcard gdt/*.c)

OBJ = $(patsubst %.c, %.o, $(C_SRC)) \
	$(patsubst %.asm, %.o, $(ASM_SRC))

TARGET = build/myos.bin

.PHONY: all clean 

# 主要目标
all: $(TARGET)

# 生成 .o 文件
%.o: %.asm
	@echo "Assembling $<..."
	$(ASM) $(ASM_FLAGS) -o $@ $<

%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(C_FLAGS) -o $@ $<

# 链接目标文件
$(TARGET): $(OBJ)
	@echo "Linking $@..."
	$(LD) $(LD_FLAGS) -o $@ $(OBJ) $(LIBS)

# 清理目标
clean:
	rm -rf $(OBJ) $(TARGET)