global long_mode_start

extern error
extern kernel_main
extern load_multiboot_values

section .text
bits 64
long_mode_start:
    mov eax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call load_multiboot_values
    call kernel_main
    hlt