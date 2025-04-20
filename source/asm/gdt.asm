bits 64

global i686GDTload
i686GDTload:
    push rbp
    mov rbp, rsp

    mov rax, [rbp + 8]     ; descriptor pointer (64-bit)
    lgdt [rax]             ; load GDT

    mov rsp, rbp
    pop rbp
    ret
