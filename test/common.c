#include "common.h"
#include <stdio.h>
#include <unistd.h>



void print_lltui_buffer(char* buf, uint32_t len) {
    
    for (uint32_t i = 0; i < len; i++) {
        if (buf != NULL) {
            putchar(buf[i]);
        } else {
            putchar(' ');
        }
    }

    fflush(stdout);
}

void read_lltui_buffer(char** buf, uint32_t* len, uint32_t max_len) {

    *len = read(STDIN_FILENO, *buf, max_len);
}
