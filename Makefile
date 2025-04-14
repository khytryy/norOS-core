kernelsource := $(shell find source/c/kernel -name '*.c')
kernelobject := $(patsubst source/c/kernel/%.c, build/c/kernel/%.o, $(kernelsource))

libheadersource := $(shell find source/c/lib -name '*.c')
libheaderobject := $(patsubst source/c/lib/%.c, build/c/lib/%.o, $(libheadersource))

asmssource := $(shell find source/asm -name '*.asm')
asmobjects := $(patsubst source/asm/%.asm, build/asm/%.o, $(asmssource))

object := $(libheaderobject) $(asmobjects) $(kernelobject)

.PHONY: all
all: kernel

$(shell mkdir -p build/c/kernel build/asm build/c/lib)

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
	mkdir -p dist/x86_64 disto/output targets/x86_64/iso/boot/grub
	x86_64-elf-ld -n -o disto/output/kernel.bin -T targets/x86_64/linker.ld $(object)
	cp disto/output/kernel.bin targets/x86_64/iso/boot/kernel.bin
	grub-mkrescue -o dist/x86_64/kernel.iso targets/x86_64/iso
	qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso -serial file:serial.log

.PHONY: clean
clean:
	rm -rf build/*
	rm -rf dist/*
	rm -rf disto/*
	rm -rf targets/x86_64/iso/boot/kernel.bin
	rm -rf targets/x86_64/iso/boot/grub
	rm -f serial.log