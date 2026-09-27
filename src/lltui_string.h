#ifndef _LLTUI_STRING_H_
#define _LLTUI_STRING_H_

#include <inttypes.h>
#include <stdbool.h>

// max_len is the size of the underlying buffer including the '\0'.
// All attach functions take the string by value and return the updated string:
//      str = lltui_string_attach_char(str, 'x');
// If something does not fit, nothing of it is attached and error is set.
typedef struct {
    char* data;
    uint32_t len;
    uint32_t max_len;
    bool error;
}lltui_string;

void lltui_string_init(lltui_string* str, char* ref, uint32_t max_len);

lltui_string lltui_string_attach_char(lltui_string str, char c);
lltui_string lltui_string_attach_str(lltui_string str, const char* s);
lltui_string lltui_string_attach_number(lltui_string str, uint32_t value);
lltui_string lltui_string_attach_int(lltui_string str, int32_t value);
lltui_string lltui_string_attach_float(lltui_string str, float value, uint8_t precision);

void lltui_string_swap_last_characters(lltui_string str, uint32_t len);

#endif
