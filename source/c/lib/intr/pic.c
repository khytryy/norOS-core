#include "pic.h"

static inline void io_wait(void) {
    // Port 0x80 is traditionally used for a short delay.
    // Writing to it has no side effects.
    __asm__ __volatile__ ("outb %%al, $0x80" : : "a"(0));
}

void piceoi(uint8_t irq) {
	if(irq >= 8)
		outb(P2_CMD, P_EOI);
	
	outb(P1_CMD, P_EOI);
}

void picdisable() {
    outb(P1_DAT, 0xff);
    outb(P2_DAT, 0xff);
}

void picremap(int offset1, int offset2) {
	outb(P1_CMD, ICW1_INIT | ICW1_ICW4); // Initialization sequence in cascade mode
	io_wait();
	outb(P2_CMD, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(P1_DAT, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(P2_DAT, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(P1_DAT, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(P2_DAT, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
	
	outb(P1_DAT, ICW4_8086);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	io_wait();
	outb(P2_DAT, ICW4_8086);
	io_wait();

	// Unmask both PICs.
	outb(P1_DAT, 0);
	outb(P2_DAT, 0);

}

void picsetmask(uint8_t IRQline) { /* Masks IRQ */
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = P1_DAT;
    } else {
        port = P2_DAT;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);        
}