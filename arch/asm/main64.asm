global x64Start
extern kernmain

section .text
bits 64
x64Start:
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