#pragma once
#include <stdint.h>

typedef struct {
	uint16_t    isr_low;                    // the lower 16 bits of the ISR's address
	uint16_t    kernel_cs;                  // the GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t	    ist;                        // the IST in the TSS that the CPU will load into RSP; set to zero for now
	uint8_t     attributes;                 // type and attributes; see the IDT page
	uint16_t    isr_mid;                    // the higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t    isr_high;                   // the higher 32 bits of the ISR's address
	uint32_t    reserved;                   // set to zero
} __attribute__((packed)) idtEntryT;

__attribute__((aligned(0x10))) 
static idtEntryT idt[256];

typedef struct {
	uint16_t	limit;
	uint64_t	base;
} __attribute__((packed)) idtrT;

static idtrT idtr;

__attribute__((noreturn))
void exception_handler(void);