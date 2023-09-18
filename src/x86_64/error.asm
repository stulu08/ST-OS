global error

extern boot_panic

errorEnd:
    dw 1

error:
    mov [errorEnd], al
    mov edi, [errorEnd]
    call boot_panic
    hlt