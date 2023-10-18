#include "out.h"

void puts(const char* msg){
    console_print_str(msg);
}
void putchar(uint8_t c) {
    console_print_char(c);
}

static inline void out_char(char _char, void* buffer, size_t counter, size_t maxlen){
    (void)buffer; (void)counter; (void)maxlen;
    if(_char){
        putchar((uint8_t)_char);
    }
}

int printf(const char *format, ...){
    va_list va;
    va_start(va, format);
    int ret = _vsnprintf(out_char, NULL, 0, format, va);
    va_end(va);
    return ret;
}

void cls(){
    console_print_clear();
}

void boot_panic(uint8_t code){
    console_print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    console_print_str("\nBoot-Error: ");
    console_print_char(code);
    asm(
        "hlt"
        );
}

void reset_color(){
    console_print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
}

void kernel_panic(char* msg){
    console_print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    console_print_str("\nKernel-Error: ");
    console_print_str(msg);
}
void total_kernel_panic(char* msg){
    kernel_panic(msg);
    asm(
        "hlt"
    );
}

void print_int32(int32_t num, int32_t base){
    char buf[] = "Error: To long to parse         ";
    console_print_str(to_string_int32(num, buf, base));
}
void print_uint32(uint32_t num, int32_t base){
    char buf[] = "Error: To long to parse         ";
    console_print_str(to_string_uint32(num, buf, base));
}
void print_int64(int64_t num, int32_t base){
    char buf[] = "Error: To long to parse                                         ";
    console_print_str(to_string_int64(num, buf, base));
}
void print_uint64(uint64_t num, int32_t base){
    char buf[] = "Error: To long to parse                                         ";
    console_print_str(to_string_uint64(num, buf, base));
}