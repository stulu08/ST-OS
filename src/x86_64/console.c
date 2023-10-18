#include "console.h"

const size_t NUM_COLS = 80;
const size_t NUM_ROWS = 25;

struct Char {
    uint8_t character;
    uint8_t color;
};
typedef struct Char Char;

Char* buffer = (Char*)0xb8000;
size_t col = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | (PRINT_COLOR_BLACK << 4);

void console_clear_row(size_t row){
    Char empty;
    empty.character = ' ';
    empty.color = color;

    for(size_t col = 0; col < NUM_COLS; col++){
        buffer[col+NUM_COLS*row] = empty;
    }
}

void console_clear_current_row(){
    console_clear_row(row);
}

void console_print_clear(){
    for(size_t i = 0; i < NUM_ROWS; i++){
        console_clear_row(i);
    }
}

void console_print_newline(){
    col = 0;
    if (row < NUM_ROWS - 1){
        row++;
        return;
    }

    for(size_t row = 1; row < NUM_ROWS; row++){
        for(size_t col = 0; col < NUM_COLS; col++){
            Char character = buffer[col + NUM_COLS * row];
            buffer[col+NUM_COLS*(row-1)] = character;    
        }
    }

    console_clear_row(NUM_COLS - 1);
}


void console_print_char(char character){
    switch (character)
    {
    case '\r':
    case '\n':
        console_print_newline();
        return;
    case '\t':
        for(uint8_t i = 0; i < TAB_SIZE; i++)
            console_print_char(' ');
        return;
    case '\b':
        if(col > 0)
            col--;
        else if(row > 0)
            row--;
        return;
    default:
        break;
    }

    if(col > NUM_COLS){
        console_print_newline();
    }

    Char ch;
    ch.character = (uint8_t)character;
    ch.color = color;

    buffer[col+NUM_COLS*row] = ch;
    col++;
}

void console_print_str(const char* str){
    for(size_t i = 0; 1; i++){
        char character = (uint8_t)str[i];

        if(character == '\0')
            return;
        
        console_print_char(character);
    }
}

void console_print_set_color(uint8_t fg, uint8_t bg) {
    color = fg | (bg << 4);
}