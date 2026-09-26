#ifndef _LLTUI_COLOR_H_
#define _LLTUI_COLOR_H_

#include <inttypes.h>

#define LLTUI_BLACK     30
#define LLTUI_RED       31
#define LLTUI_GREEN     32
#define LLTUI_YELLOW    33
#define LLTUI_BLUE      34
#define LLTUI_MAGENTA   35
#define LLTUI_CYAN      36
#define LLTUI_WHITE     37
#define LLTUI_DEFAULT   38

typedef struct {
    uint8_t foreground;
    uint8_t background;
}lltui_color;

#endif