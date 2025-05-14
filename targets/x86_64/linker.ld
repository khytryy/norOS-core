ENTRY(start)  /* Ensure that the entry point is 'start' */

SECTIONS {
    . = 0x100000;  /* Kernel load address */

    /* Place the Multiboot2 header at the beginning */
    .multiboot2_header : {
        *(.multiboot2_header)  /* Multiboot2 header section */
    }

    /* Kernel text section */
    .text ALIGN(16) : { *(.text) }

    /* Read-only data (constants, etc.) */
    .rodata ALIGN(16) : { *(.rodata) }

    /* Initialized data section */
    .data ALIGN(16) : { *(.data) }

    /* Uninitialized data (BSS) section */
    .bss ALIGN(4K) : {
        *(COMMON)
        *(.bss)
    }

    /* Stack section */
    .stack ALIGN(16) : {
        *(.stack)
    }
}
