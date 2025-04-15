global lm_start
extern kernmain

section .text
bits 64
lm_start:
    mov cx, 0
    mov ss, cx
    mov ds, cx
    mov es, cx
    mov fs, cx
    mov gs, cx

    pop rbx
    pop rax

    mov rdi, rax
    mov rsi, rbx

    call kernmain

    hlt