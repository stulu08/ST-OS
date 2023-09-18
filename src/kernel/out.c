#include "out.h"

void cls(){
    console_print_clear();
}

void print(const char* msg){
    console_print_str(msg);
}
void print_int(int32_t num){
    char buf[] = "X00000000";
    itoa(num, buf, 10);
    if(buf[0] == 'X'){
        console_print_str("Error: To long to parse");
        return;
    }
    console_print_str(buf);
}
void print_uint(uint32_t num){
    print_int((int32_t)num);
}

void boot_panic(uint8_t code){
    console_print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    console_print_str("\nBoot-Error: ");
    console_print_char(code);
}

void kernel_panic(char* msg){
    console_print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    console_print_str("\nKernel-Error: ");
    console_print_str(msg);
}


char* itoa(int32_t value, char* result, int32_t base){
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int32_t tmp_value;

    do {
        tmp_value = value;
        value /= base;
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