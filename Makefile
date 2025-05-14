# paths
SRC_KERNEL := source/c/kernel
SRC_LIB    := source/c/lib
SRC_ASM    := source/asm
BUILD_KERNEL := build/c/kernel
BUILD_LIB    := build/c/lib
BUILD_ASM    := build/asm
KERNEL_BIN := bin/output/kernel.bin

MAKEFLAGS += -rR

# tc flags
CC      := x86_64-elf-gcc
LD      := x86_64-elf-ld
NASM    := nasm
CFLAGS  := -ffreestanding -nostdlib -m64 -mcmodel=kernel -fno-pie -fno-pic -fno-pie -g -fno-stack-protector -mcmodel=large \
           -mno-sse -mno-mmx -mno-sse2 -mno-80387 -mno-fp-ret-in-387 -Werror -I source/lib
NASMFLAGS := -f elf64

# source files
KERNEL_SRC := $(shell find $(SRC_KERNEL) -name '*.c')
LIB_SRC    := $(shell find $(SRC_LIB) -name '*.c')
ASM_SRC    := $(shell find $(SRC_ASM) -name '*.asm')

KERNEL_OBJ := $(patsubst $(SRC_KERNEL)/%.c, $(BUILD_KERNEL)/%.o, $(KERNEL_SRC))
LIB_OBJ    := $(patsubst $(SRC_LIB)/%.c, $(BUILD_LIB)/%.o, $(LIB_SRC))
ASM_OBJ    := $(patsubst $(SRC_ASM)/%.asm, $(BUILD_ASM)/%.o, $(ASM_SRC))

OBJECTS := $(KERNEL_OBJ) $(LIB_OBJ) $(ASM_OBJ)

# build rules
.PHONY: all kernel clean

# compiling
$(BUILD_KERNEL)/%.o: $(SRC_KERNEL)/%.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_LIB)/%.o: $(SRC_LIB)/%.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_ASM)/%.o: $(SRC_ASM)/%.asm
	mkdir -p $(dir $@)
	$(NASM) $(NASMFLAGS) $< -o $@

kernel: $(OBJECTS)
	rm -rf serial.log
	mkdir -p bin/x86_64 bin/output targets/x86_64/iso/boot/grub
	$(LD) -n -o $(KERNEL_BIN) -T targets/x86_64/linker.ld $(OBJECTS)
	cp $(KERNEL_BIN) targets/x86_64/iso/boot/kernel.bin
	grub-mkrescue -o bin/x86_64/kernel.iso targets/x86_64/iso
# to enable debugging, put this into the last line of qemu -s -S -d int
	qemu-system-x86_64 -cdrom bin/x86_64/kernel.iso -serial mon:stdio

clean:
	rm -rf build/* bin/* dist/* serial.log targets/x86_64/iso/boot/kernel.bin
