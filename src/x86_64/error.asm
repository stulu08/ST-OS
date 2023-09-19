global error

extern boot_panic

error:
    xor edi, edi
    mov dil, al
    call boot_panic