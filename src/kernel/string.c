#include "string.h"

size_t strlen(const char *str) {
    size_t res;
    for (; *(str + res); res++);
    return res;
}