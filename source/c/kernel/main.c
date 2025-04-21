#include <stdint.h>
#include "../lib/multiboot2/mb2.h"
#include "../lib/drivers/serial/serial.h"
#include "../lib/io.h"
#include "../lib/intr/pic.h"
#include "../lib/gdt/gdt.h"
#include "../lib/icons.h"
#include "../lib/drivers/cpuvendor/vendor.h"

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
    
    // do some cool stuff here

    serialBegin();
    


    serialWrite(KERNEL_ICON_SUCCESS);
    serialWrite("job done\n");

    serialWrite("norOS kernel [Version 42125]\n");
    serialWrite("Welcome to norOS!\n");
    serialWrite("\n");
    serialWrite("Running on: ");
    serialWrite(getmodel());
    serialWrite("\n");


}