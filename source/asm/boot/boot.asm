section .multiboot2_header
    align 8
header_start:
    dd 0xE85250D6 ; magic
    dd 0 ; architecture
    dd header_end - header_start ; header length
    dd -(0xE85250D6 + 0 + (header_end - header_start)) ; checksum

framebuffer_tag_start:
    dw 0x05 ; framebuffer tag
    dw 0x01 ; optional tag
    dd framebuffer_tag_end - framebuffer_tag_start ; tag length
    dd 1093 ; width
    dd 731 ; height
    dd 32 ; depth

framebuffer_tag_end:
    ; end tag
    align 8
    dw 0
    dw 0
    dd 8

header_end: