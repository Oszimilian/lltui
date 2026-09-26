#include "lltui_string.h"


uint32_t lltui_string_attache_number(char* str, uint32_t pos) {
    uint32_t len = 0;

    do{
        str[len++] = (char)('0' + pos % 10);
        pos /= 10;
    }while(pos > 0);

    return len;
}

void lltui_string_swap_character(char* str, uint32_t len) {
    int i = 0;
    int k = len - 1;

    for (; i < k; i++, k--) {
        char tmp = str[i];
        str[i] = str[k];
        str[k] = tmp;
    }
}