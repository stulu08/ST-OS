#pragma once
#include "console.h"

void cls();

void print(const char* msg);
void print_int(int32_t num);
void print_uint(uint32_t num);

void boot_panic(uint8_t code);
void kernel_panic(char* msg);

char* itoa(int value, char* result, int base);