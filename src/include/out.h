#pragma once
#include "console.h"
#include "format.h"
#include "string.h"

void cls();

void puts(const char* str);
void putchar(uint8_t c);
int printf(const char* format, ...);

void print_int32(int32_t num, int32_t base);
void print_uint32(uint32_t num, int32_t base);
void print_int64(int64_t num, int32_t base);
void print_uint64(uint64_t num, int32_t base);

void reset_color();

void boot_panic(uint8_t code);
void kernel_panic(char* msg);