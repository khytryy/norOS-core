global start
extern lm_start

section .text
bits 32
start:
    mov esp, stack_top

    push 0
    push eax
    push 0
    push ebx

    call mbcheck
    call cpuidcheck
    call x64check

    call ptsetup
    call epaging

    lgdt [gdt64_pointer]        ; Loading GDT pointer

    jmp gdt64_code_sel:lm_start ; Jump to 64-bit code segment

    hlt

mbcheck:
    cmp eax, 0x36d76289
    jne .nomb
    ret
.nomb:
    cli
    hlt_loop:
        in al, 0x64
        test al, 0x02
        jnz hlt_loop

    mov al, 0xFE
    out 0x64, al
    hlt

cpuidcheck:
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    cmp eax, ecx
    je .nocpuid
    ret
.nocpuid:
    hlt

x64check:
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb .nox64

    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz .nox64
    ret
.nox64:
    hlt

ptsetup:
    mov eax, ptl3
    or eax, 0b11
    mov [ptl4], eax
    
    mov eax, ptl2
    or eax, 0b11
    mov [ptl3], eax

    mov ecx, 0 ; counter
.loop:
    mov eax, 0x200000 ; 2MiB
    mul ecx
    or eax, 0b10000011
    mov [ptl2 + ecx * 8], eax

    inc ecx
    cmp ecx, 512
    jne .loop

    ret

epaging:
    ; pass page table location to cpu
    mov eax, ptl4
    mov cr3, eax

    ; enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; x64 mode
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; enable paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret

section .bss
align 4096
ptl4:
    resb 4096
ptl3:
    resb 4096
ptl2:
    resb 4096
stack_bottom:
    resb 4096 * 4
stack_top:

section .rodata
gdt64:
    dq 0                                  ; Null descriptor
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) ; Code: exec/read, long mode
    dq (1 << 44) | (1 << 47)              ; Data: read/write

gdt64_code:
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) ; Code segment (exec/read, long mode)
gdt64_data:
    dq (1 << 44) | (1 << 47)              ; Data segment (read/write)

gdt64_pointer:
    dw $ - gdt64 - 1               ; GDT size (offset from gdt64 to pointer)
    dq gdt64                       ; GDT base address

; Segment selectors (offsets into GDT)
gdt64_code_sel equ gdt64_code - gdt64
gdt64_data_sel equ gdt64_data - gdt64