#ifndef _LLTUI_WIDGET_H_
#define _LLTUI_WIDGET_H_

#include <stdbool.h>

#include "lltui_pos.h"
#include "lltui_ctx.h"
#include "lltui_color.h"

typedef enum {
    lltui_visible,
    lltui_shadowd
}lltui_widget_visibility;

typedef enum {
    lltui_textfield,
    lltui_line,
}lltui_widget_type;



typedef struct lltui_widget{
    lltui_widget_type widget_type;
    lltui_widget_visibility visability;
    lltui_pos start_pos;
    lltui_pos end_pos;
    lltui_color color;
    bool updated;
    union {
        struct {
            int32_t desc;
        }textfield;
        struct {
            int32_t desc;
        }line;
    }type;
}lltui_widget;

int32_t lltui_widget_create(lltui_ctx* ctx, lltui_pos start_pos, lltui_pos end_pos, lltui_widget_type type);
void lltui_widget_set_pos(lltui_ctx* ctx, int32_t descriptor, lltui_pos start_pos, lltui_pos end_pos);

void lltui_widget_set_text(lltui_ctx* ctx, int32_t descriptor, char* str);

void lltui_widget_info(lltui_ctx* ctx, int32_t descriptor);
void lltui_widget_print(lltui_ctx* ctx, int32_t descriptor);

void lltui_widget_shadow(lltui_ctx* ctx, int32_t descriptor);
void lltui_widget_show(lltui_ctx* ctx, int32_t descriptor);

void lltui_widget_color_foreground(lltui_ctx* ctx, int32_t descriptor, uint8_t color);
void lltui_widget_color_background(lltui_ctx* ctx, int32_t descriptor, uint8_t color);



#endif