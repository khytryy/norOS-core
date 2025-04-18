#include "serial.h"

int serialBegin() {
    outb(COM1 + 1, 0x00); // disable interrupts
    outb(COM1 + 3, 0x80); // enable DLAB
    outb(COM1 + 0, 0x03); // set divisor to 3, 38400 baud
    outb(COM1 + 1, 0x00); // high byte
    outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7); // enable fifo, clear them, with 14-byte threshold
  
    outb(COM1 + 4, 0x0B); // irqs enabled, rts/dsr set
    outb(COM1 + 4, 0x1E); // set in loopback mode
    outb(COM1 + 0, 0xAE); // test the serial chip
  
    // check if the same byte received
    if(inb(COM1 + 0) != 0xAE) {
      return 1;
    }
  
    // if not, set it in normal operation mode
    outb(COM1 + 4, 0x0F);
    return 0;
}
  
int is_transmit_empty() {
    return inb(COM1 + 5) & 0x20;
}
  
void serialWriteChar(char l) {
    while (is_transmit_empty() == 0);
    outb(COM1, l);
}
  
void serialWrite(const char* string) {
    while (*string) {
      serialWriteChar(*string++);
    }
}

void itoa(unsigned long value, char* str, int base) {
    char* ptr = str, *ptr1 = str, tmp_char;
    unsigned long tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789ABCDEF"[tmp_value - value * base];
    } while (value);

    *ptr-- = '\0';

    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}