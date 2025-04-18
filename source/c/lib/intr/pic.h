#pragma once

#include <stdint.h>
#include "../io.h"

#define P1_CMD 0x20 /* master PIC */
#define P1_DAT 0x21

#define P2_CMD 0xA0 /* slave PIC */
#define P2_DAT 0xA1

#define P_EOI 0x20

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

static inline void io_wait(void); // I/O Delay
void piceoi(uint8_t irq); // End-Of-Interrupt
void picdisable();
void picremap(int offset1, int offset2); // Remaps the PIC
void picsetmask(uint8_t IRQline); // Masks IRQ