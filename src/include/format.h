#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>


#include "string.h"

enum {
	BIN = 2,
	OCT = 8,
	DEC = 10,
    HEX = 16,
};

char* to_string_int32(int32_t value, char* result, int32_t base);
char* to_string_uint32(uint32_t value, char* result, int32_t base);
char* to_string_int64(int64_t value, char* result, int32_t base);
char* to_string_uint64(uint64_t value, char* result, int32_t base);

int sprintf(char* str, const char* format, ...);
int snprintf(char* str, size_t n, const char* format, ...);

typedef void (*out_function_type)(char _char, void* buffer, size_t counter, size_t maxlen);

/// @brief 
/// @param out Defines the output
/// @param buffer Out location
/// @param count Max number of chars
/// @param format Format
/// @param va Arguments
int _vsnprintf(out_function_type out, char* buffer, size_t maxlen, const char *format, va_list va);