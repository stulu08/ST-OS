#include "out.h"

void cls(){
    console_print_clear();
}

void print(const char* msg){
    console_print_str(msg);
}

void boot_panic(uint8_t code){
    console_print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    console_print_str("\nBoot-Error: ");
    console_print_char(code);
    asm(
        "hlt"
        );
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
    char buf[] = "Error: To long to parse";
    console_print_str(to_string_int32(num, buf, base));
}
void print_uint32(uint32_t num, int32_t base){
    char buf[] = "Error: To long to parse";
    console_print_str(to_string_uint32(num, buf, base));
}
void print_int64(int64_t num, int32_t base){
    char buf[] = "Error: To long to parse";
    console_print_str(to_string_int64(num, buf, base));
}
void print_uint64(uint64_t num, int32_t base){
    char buf[] = "Error: To long to parse";
    console_print_str(to_string_uint64(num, buf, base));
}

char* to_string_int32(int32_t value, char* result, int32_t base){
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int32_t tmp_value;

    do {
        tmp_value = value;
        value /= (int32_t)base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
  
    // Reverse the string
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}
char* to_string_int64(int64_t value, char* result, int32_t base){
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int64_t tmp_value;

    do {
        tmp_value = value;
        value /= (int64_t)base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
  
    // Reverse the string
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}
char* to_string_uint32(uint32_t value, char* result, int32_t base){
    return to_string_int64((int64_t)value, result, base);
}
char* to_string_uint64(uint64_t value, char* result, int32_t base){
    return to_string_int64((int64_t)value, result, base);
}