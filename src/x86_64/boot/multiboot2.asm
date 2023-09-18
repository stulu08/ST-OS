global check_multiboot
global store_multiboot_values
global load_multiboot_values

extern error

multiboot_tag:
    DD 0
    DD 0
multiboot_magic:
    DD 0
    DD 0


check_multiboot:
    ; magic number for multiboot
    cmp eax, 0x36d76289
    jne .no_multiboot
    ret
.no_multiboot:
    mov al, 'M'
    jmp error

; first two function parameter are in eax and ebx (32 bit)
; first two function parameter are in edi and esi (64 bit)

; store the 32 bit values
store_multiboot_values:
    ; eax contains the multiboot2 magic number which we already checked above but for conveniance i also store it
    mov [multiboot_magic], eax
    ; ebx contains address of the multiboot2 information struct
    mov [multiboot_tag], ebx
    ret

; parse the 32 bit values into 64 bit mode
load_multiboot_values:
    mov edi, [multiboot_magic]
    mov esi, [multiboot_tag]
    ret