global start

extern long_mode_start
extern error

extern check_multiboot
extern store_multiboot_values

section .text
bits 32

start:
    mov esp, stack_top
    
    call check_multiboot
    call store_multiboot_values

    call check_cpuid
    call check_long_mode

    call setup_page_tables
    call enable_paging

    lgdt [gdt64.pointer]
    jmp gdt64.code_segment:long_mode_start

    hlt

check_cpuid:
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
    je .no_cpuid
    ret
.no_cpuid:
    mov al, 'C'
    jmp error

check_long_mode:
    mov eax, 0x80000000
    cpuid 
    cmp eax, 0x80000001
    jb .no_long_mode

    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz .no_long_mode
    ret
.no_long_mode:
    mov al, 'L'
    jmp error

setup_page_tables:
    mov eax, page_table_l3
    ; present, writable
    or eax, 0b11
    mov [page_table_l4], eax

    mov eax, page_table_l2
    ; present, writable
    or eax, 0b11
    mov [page_table_l3], eax

    ; for i = 0
    mov ecx, 0
.loop:

    mov eax, 0x200000 ; 2MiB
    mul ecx
    or eax, 0b10000011
    mov [page_table_l2 + ecx * 8], eax

    ; i++
    inc ecx 
    ; check if whole table is mapped and continue if not
    cmp ecx, 512
    jne .loop

    ret

enable_paging:
    ; pass page table loc to cpu
    mov eax, page_table_l4
    mov cr3, eax

    ; enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; enable long mode
    mov ecx, 0xc0000080
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
page_table_l4:
    resb 4096
page_table_l3:
    resb 4096
page_table_l2:
    resb 4096
stack_bottom:
    resb 4096 * 4
stack_top:

section .rodata
gdt64:
    dq 0
.code_segment: equ $ - gdt64
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)
.pointer:
    dw - gdt64 - 1
    dq gdt64