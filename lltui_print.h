#ifndef _LLTUI_PRINT_H_
#define _LLTUI_PRINT_H_

#include "lltui_config.h"


#ifdef LLTUI_PRINTF_ENABLE
    #define LLTUI_PRINTF(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define LLTUI_PRINTF(fmt, ...) do {} while(0)
#endif

#endif