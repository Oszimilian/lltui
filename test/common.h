#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>

void print_lltui_buffer(char* buf, uint32_t len);
void read_lltui_buffer(char** buf, uint32_t* len, uint32_t max_len);

#endif