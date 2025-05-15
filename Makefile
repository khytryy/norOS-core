# paths
ARCH           := x86_64
BUILD_DIR      := build
BIN_DIR        := bin/output
ISO_DIR        := grub/boot/grub/iso
OSELF     	   := $(BIN_DIR)/kernel.elf
ISO        	   := $(BIN_DIR)/norOS.iso
LD_SCRIPT  	   := link/$(ARCH)/linker.ld

# tools
AS             := nasm
CC             := x86_64-elf-gcc
LD             := x86_64-elf-ld
CFLAGS         := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m64 -Iinclude \
				$(foreach dir, $(shell find include drivers -type d), -I$(dir))
LDFLAGS        := -T $(LD_SCRIPT) -nostdlib

# source files
ASM_SRC 	   := $(shell find arch/asm -name '*.asm')
C_SRC   	   := $(shell find arch/c drivers include -name '*.c')

# object files
ASM_OBJ 	   := $(patsubst %, $(BUILD_DIR)/%.o, $(basename $(ASM_SRC)))
C_OBJ   	   := $(patsubst %, $(BUILD_DIR)/%.o, $(basename $(C_SRC)))
OBJ    	       := $(ASM_OBJ) $(C_OBJ)

# Default target
all: $(ISO)

# link the kernel
$(OSELF): $(OBJ)
	mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $@ $^

# build the final iso
$(ISO): $(OSELF)
	mkdir -p $(ISO_DIR)
	cp $< $(ISO_DIR)/kernel.elf
	grub-mkrescue -o $@ loader

#	qemu-system-x86_64 -cdrom bin/output/norOS.iso

# pattern rules
$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.asm
	mkdir -p $(dir $@)
	$(AS) -f elf64 $< -o $@

# clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

run:
	qemu-system-x86_64 -cdrom bin/output/norOS.iso -serial mon:stdio

.PHONY: all clean