#include <stdint.h>
#include <serial/serial.h>
#include <fb/fb.h>

void kernmain(uint32_t magic, void *addr) {
    if (magic == 0x36d76289) {
        serialWrite("valid magic!\n");
    } else {
        while(1);
    }

    fbInit(addr);
    
}