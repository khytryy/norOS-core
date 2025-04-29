#include <stdint.h>
#include "../lib/drivers/serial/serial.h"
#include "../lib/io.h"
#include "../lib/intr/pic.h"
#include "../lib/icons.h"
#include "../lib/drivers/cpuvendor/vendor.h"
#include "../lib/drivers/fb/fb.h"
#include "../lib/string.h"

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
    newline();
    serialBegin(); // serial initialization
    fbInit(addr); // framebuffer driver initialization



    serialWrite(KERNEL_ICON_SUCCESS);
    serialWrite("job done\n");

    serialWrite("norOS kernel [Version 42925]\n");
    serialWrite("Welcome to norOS!\n");
    newline();
    serialWrite("Running on: ");
    const char* manu = getcpumanu();

    if (strcmp(manu, "AuthenticAMD") == 0) {
        serialWrite("AMD ");
        serialWrite(getcpumodel());
    } else if (strcmp(manu, "GenuineIntel") == 0) {
        serialWrite("Intel ");
        serialWrite(getcpumodel());
    } else {
        serialWrite("Unknown CPU");
    }   
    newline();

    fbplot(0, 0, 0x00FF0000);
}