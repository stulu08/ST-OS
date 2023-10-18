#pragma once

#include <stdint.h>
#include <stddef.h>

#define TAB_SIZE 4

enum {
    PRINT_COLOR_BLACK = 0,
	PRINT_COLOR_BLUE = 1,
	PRINT_COLOR_GREEN = 2,
	PRINT_COLOR_CYAN = 3,
	PRINT_COLOR_RED = 4,
	PRINT_COLOR_MAGENTA = 5,
	PRINT_COLOR_BROWN = 6,
	PRINT_COLOR_LIGHT_GRAY = 7,
	PRINT_COLOR_DARK_GRAY = 8,
	PRINT_COLOR_LIGHT_BLUE = 9,
	PRINT_COLOR_LIGHT_GREEN = 10,
	PRINT_COLOR_LIGHT_CYAN = 11,
	PRINT_COLOR_LIGHT_RED = 12,
	PRINT_COLOR_PINK = 13,
	PRINT_COLOR_YELLOW = 14,
	PRINT_COLOR_WHITE = 15,
};

void console_clear_row(size_t row);
void console_clear_current_row();
void console_print_clear();
void console_print_newline();
void console_print_char(char character);
void console_print_str(const char* str);
void console_print_set_color(uint8_t fg, uint8_t bg);

void boot_panic(uint8_t code);
void kernel_panic(char* msg);
void total_kernel_panic(char* msg);
