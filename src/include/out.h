#pragma once
#include "console.h"

void cls();

void print(const char* msg);

enum {
	BIN = 2,
	OCT = 8,
	DEC = 10,
    HEX = 16,
};

void print_int32(int32_t num, int32_t base);
void print_uint32(uint32_t num, int32_t base);
void print_int64(int64_t num, int32_t base);
void print_uint64(uint64_t num, int32_t base);

void boot_panic(uint8_t code);
void kernel_panic(char* msg);

char* to_string_int32(int32_t value, char* result, int32_t base);
char* to_string_uint32(uint32_t value, char* result, int32_t base);
char* to_string_int64(int64_t value, char* result, int32_t base);
char* to_string_uint64(uint64_t value, char* result, int32_t base);