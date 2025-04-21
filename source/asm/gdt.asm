section .text
global load_gdt
extern gDescriptorGDT

load_gdt:
    lgdt [gDescriptorGDT]

    mov rax, .reload_cs
    push 0x08
    push rax
    retfq

.reload_cs:
    ret