#ifndef _LLTUI_ASSERT_H_
#define _LLTUI_ASSERT_H_

#include "lltui_config.h"

#include <stdlib.h>
#include <stdio.h>

#ifdef LLTUI_ASSERT_WITH_EXIT
    #define LLTUI_EXIT exit(-1); 
#else
    #define LLTUI_EXIT while(true){}
#endif


#ifdef LLTUI_ASSERT_ENABLE                              
    #define LLTUI_ASSERT(condition, str)                \
        do{                                             \
            if (condition) {                            \
                printf("LLTUI ASSERT: %s \n\r", str);   \
                LLTUI_EXIT                              \
            }                                           \
        }while(0);                                      
    
#else 
    #define LLTUI_ASSERT(condition, str) do {} while(0);
#endif

#endif