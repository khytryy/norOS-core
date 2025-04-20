kernelsource := $(shell find source/c/kernel -name '*.c')
kernelobject := $(patsubst source/c/kernel/%.c, build/c/kernel/%.o, $(kernelsource))

libheadersource := $(shell find source/c/lib -name '*.c')
libheaderobject := $(patsubst source/c/lib/%.c, build/c/lib/%.o, $(libheadersource))

asmssource := $(shell find source/asm -name '*.asm')
asmobjects := $(patsubst source/asm/%.asm, build/asm/%.o, $(asmssource))

object := $(libheaderobject) $(asmobjects) $(kernelobject)

$(shell mkdir -p build/c/kernel build/asm build/c/lib)

.PHONY: all
all: checkos kernel

.PHONY: checkos
checkos:
ifeq ($(OS),Windows_NT)
	@echo "[ !!! ] You're currently using a Windows machine. Its recommended to spin up a Linux VM" >&2
endif

build/c/kernel/%.o: source/c/kernel/%.c
	mkdir -p $(dir $@)
	x86_64-elf-gcc -c -I source/lib -ffreestanding -nostdlib $< -o $@

build/c/lib/%.o: source/c/lib/%.c
	mkdir -p $(dir $@)
	x86_64-elf-gcc -c -I source/lib -ffreestanding -nostdlib $< -o $@

build/asm/%.o: source/asm/%.asm
	mkdir -p $(dir $@)
	nasm -f elf64 $< -o $@

.PHONY: kernel
kernel: $(object)
	rm -rf serial.log
	mkdir -p bin/x86_64 bin/output targets/x86_64/iso/boot/grub
	x86_64-elf-ld -n -o bin/output/kernel.bin -T targets/x86_64/linker.ld $(object)
	cp bin/output/kernel.bin targets/x86_64/iso/boot/kernel.bin
	grub-mkrescue -o bin/x86_64/kernel.iso targets/x86_64/iso
	qemu-system-x86_64 -cdrom bin/x86_64/kernel.iso -serial file:serial.log
	cat serial.log

.PHONY: clean
clean:
	rm -rf build/*
	rm -rf dist/*
	rm -rf bin/*
	rm -rf targets/x86_64/iso/boot/kernel.bin
	rm -rf targets/x86_64/iso/boot/grub
	rm -f serial.log