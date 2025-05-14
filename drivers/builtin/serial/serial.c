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
  
void serialWrite(char* string) {
    while (*string) {
      serialWriteChar(*string++);
    }
}

void itoa_hex(unsigned int value, char* buffer) {
    const char* hex_digits = "0123456789ABCDEF";
    int i = 0;

    // special case for 0
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i++] = '\0';
        return;
    }

    // converter to hex in reverse
    while (value > 0) {
        buffer[i++] = hex_digits[value % 16];
        value /= 16;
    }

    // null terminator
    buffer[i] = '\0';

    // reverse buffer
    for (int j = 0; j < i / 2; j++) {
        char tmp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = tmp;
    }
}

void newline() {
    serialWrite("\n");
}

void itoa_dec(unsigned int value, char* buffer) {
    int i = 0;
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }

    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    buffer[i] = '\0';

    // Reverse
    for (int j = 0; j < i / 2; j++) {
        char tmp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = tmp;
    }
}
