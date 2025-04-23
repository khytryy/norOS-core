#include <stdint.h>
#include "../lib/drivers/serial/serial.h"
#include "../lib/io.h"
#include "../lib/intr/pic.h"
#include "../lib/gdt/gdt.h"
#include "../lib/icons.h"
#include "../lib/drivers/cpuvendor/vendor.h"
#include "../lib/drivers/fb/fb.h"

framebuffer_t framebuffer;

void kernmain(uint32_t magic, void *addr) {
    if (magic == 0x36d76289) {
        serialWrite(KERNEL_ICON_INFO);
        serialWrite("booted by multiboot2\n");
    } else {
        serialWrite(KERNEL_ICON_FAILURE);
        serialWrite("didnt boot by multiboot2\n");
        while (1);
    }

    picremap(0x20, 0x28);
    serialWrite(KERNEL_ICON_INFO);
    serialWrite("interrupt controller remapped\n");
    serialWrite(KERNEL_JOB);
    serialWrite("starting up...\n");
    serialWrite("\n");
    serialBegin(); // serial initialization
    fbInit(addr); // framebuffer driver initialization
    serialWrite(KERNEL_ICON_OK);
    serialWrite("fb :: started up successfully\n");


    serialWrite(KERNEL_ICON_SUCCESS);
    serialWrite("job done\n");

    serialWrite("norOS kernel [Version 42325]\n");
    serialWrite("Welcome to norOS!\n");
    serialWrite("\n");
    serialWrite("Running on: ");
    serialWrite(getmodel());
    serialWrite("\n");

    char fbaddress[32];
    itoa_hex((uintptr_t)framebuffer.address, fbaddress);
    serialWrite(KERNEL_ICON_INFO);
    serialWrite("fb :: address: 0x");
    serialWrite(fbaddress);
    serialWrite("\n");

}