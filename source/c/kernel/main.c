#include <stdint.h>
#include "../lib/multiboot2/mb2.h"
#include "../lib/drivers/serial/serial.h"
#include "../lib/io.h"
#include "../lib/intr/pic.h"
#include "../lib/gdt/gdt.h"

void kernmain(uint32_t magic, void *addr) {
    if (magic == 0x36d76289) {
        serialWrite(ICON_SUCCESS);
        serialWrite(" magic matches\n");
    } else {
        serialWrite(ICON_FAILURE);
        serialWrite(" magic doesn't match\n");
        while (1);
    }

    picremap(0x20, 0x28);
    serialWrite(ICON_SUCCESS);
    serialWrite(" pic remapped\n");

    i686GDTload(&gDescriptorGDT, 0x08, 0x10);
    
}
