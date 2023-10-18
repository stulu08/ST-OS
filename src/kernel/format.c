#include "format.h"

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
     // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    uint32_t tmp_value;

    do {
        tmp_value = value;
        value /= (uint32_t)base;
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
char* to_string_uint64(uint64_t value, char* result, int32_t base){
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    uint64_t tmp_value;

    do {
        tmp_value = value;
        value /= (uint64_t)base;
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

size_t _vsnprint(out_function_type out, char* buffer, size_t counter, size_t maxlen, const char *str){
    for(size_t i = 0; 1; i++){
        char c = (uint8_t)str[i];
        if(c == '\0')
            return counter;
		out(c, buffer, counter, maxlen);
        counter++;
    }
    return counter;
}

int _vsnprintf(out_function_type out, char* buffer, size_t maxlen, const char *format, va_list va) {
	const size_t len = strlen(format);
    size_t counter = 0;
    // temp buffer
    char* tempBuff = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	for(size_t i = 0; i < len; i++){
		char c = format[i];
		if (c != '%'){
			out(c, buffer, counter, maxlen);
            counter++;
			continue;
		}

		c = format[i+1];
		char c2 = '\0';
		char c3 = '\0';
		char c4 = '\0';
		if(i < len -2){
			c2 = format[i+2];

			if(i < len -3){
				c3 = format[i+3];

				if(i < len -4){
					c4 = format[i+4];
				}
			}
		}
		
		switch (c) {
			case 'd':
                // d[x]
				if(c2 == 'x'){
                    counter = _vsnprint(out, buffer, counter, maxlen, to_string_int32(va_arg(va, int32_t), tempBuff, HEX));
					i += 2;
					break;
				}
                counter = _vsnprint(out, buffer, counter, maxlen, to_string_int32(va_arg(va, int32_t), tempBuff, DEC));
				i += 1;
				break;
			case 'l': {
				if(c2 == 'l'){
					if(c3 == 'u'){
						// llu[x]
                        if(c2 == 'x'){
                            counter = _vsnprint(out, buffer, counter, maxlen, to_string_uint64(va_arg(va, uint64_t), tempBuff, HEX));
                            i += 4;
                            break;
                        }
                        counter = _vsnprint(out, buffer, counter, maxlen, to_string_uint64(va_arg(va, uint64_t), tempBuff, DEC));
                        i += 3;
                        break;
					}
					// ll[x]
                    if(c2 == 'x'){
                        counter = _vsnprint(out, buffer, counter, maxlen, to_string_int64(va_arg(va, int64_t), tempBuff, HEX));
                        i += 3;
                        break;
                    }
                    counter = _vsnprint(out, buffer, counter, maxlen, to_string_int64(va_arg(va, int64_t), tempBuff, DEC));
                    i += 2;
                    break;
				}else if(c2 == 'u'){
					// lu[x]
                    if(c2 == 'x'){
                        counter = _vsnprint(out, buffer, counter, maxlen, to_string_uint32(va_arg(va, uint32_t), tempBuff, HEX));
                        i += 3;
                        break;
                    }
                    counter = _vsnprint(out, buffer, counter, maxlen, to_string_uint32(va_arg(va, uint32_t), tempBuff, DEC));
                    i += 2;
                    break;
				}
				// l[x]
				if(c2 == 'x'){
                    counter = _vsnprint(out, buffer, counter, maxlen, to_string_int32(va_arg(va, int32_t), tempBuff, HEX));
					i += 2;
					break;
				}
                counter = _vsnprint(out, buffer, counter, maxlen, to_string_int32(va_arg(va, int32_t), tempBuff, DEC));
				i += 1;
				break;
			}
			case 's': 
                counter = _vsnprint(out, buffer, counter, maxlen, va_arg(va, char*));
				i += 1;
				break;
			case 'p':
                counter = _vsnprint(out, buffer, counter, maxlen, to_string_uint64(va_arg(va, uint64_t), tempBuff, HEX));
				i += 1;
				break;
			case 'c':
			default:
                out((char)va_arg(va, int), buffer, counter, maxlen);
                counter++;
				i += 1;
				break;
                
		}
	}
    out((char)0, buffer, counter < maxlen ? counter : maxlen - 1, maxlen);
    return counter;
}

static inline void out_buffer(char _char, void* buffer, size_t counter, size_t maxlen){
    if(buffer){
        ((char*)buffer)[counter] = _char;
    }
}
static inline void out_buffern(char _char, void* buffer, size_t counter, size_t maxlen){
    if(buffer && counter < maxlen){
        ((char*)buffer)[counter] = _char;
    }
}

int sprintf(char* str, const char* format, ...) {
    va_list va;
    va_start(va, format);
    int ret = _vsnprintf(out_buffer, str, 0, format, va);
    va_end(va);
    return ret;
}
int snprintf(char* str, size_t n, const char* format, ...) {
    va_list va;
    va_start(va, format);
    int ret = _vsnprintf(out_buffern, str, n, format, va);
    va_end(va);
    return ret;
}