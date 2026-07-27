#include "lltui_widget.h"
#include "lltui_arena.h"
#include "lltui_assert.h"
#include "lltui_print.h"
#include "lltui_cursor.h"
#include "string.h"

int32_t lltui_widget_textfield_create(lltui_ctx* ctx, lltui_pos pos) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");

    int32_t descriptor = lltui_arena_malloc(&ctx->widget_arena, sizeof(lltui_widget));
    lltui_widget_textfield_pos_set(ctx, descriptor, pos);

    lltui_widget_show(ctx, descriptor);

    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);
    widget->type.textfield.last_len = 0;
    widget->type.textfield.len = 0;

    lltui_widget_color_background(ctx, descriptor, LLTUI_BLACK);
    lltui_widget_color_foreground(ctx, descriptor, LLTUI_WHITE);

    return descriptor;
}

void lltui_widget_textfield_pos_set(lltui_ctx* ctx, int32_t descriptor, lltui_pos pos) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");
    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);

    LLTUI_ASSERT(widget->widget_type != lltui_textfield, "descriptor is not a textfield widget");
    widget->start_pos = pos;
}

void lltui_widget_textfield_text_set(lltui_ctx* ctx, int32_t descriptor, char* str) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");
    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);

    LLTUI_ASSERT(widget->widget_type != lltui_textfield, "descriptor is not a textfield widget");

    widget->type.textfield.last_len = widget->type.textfield.len;

    uint32_t text_len = strlen(str);
    widget->type.textfield.desc =  lltui_arena_malloc(&ctx->widget_arena, text_len + 1);

    char* data = (char*)lltui_arena_get_ref(&ctx->widget_arena, widget->type.textfield.desc);
    memcpy(data, str, text_len);
    data[text_len + 1] = '\0';

    widget->type.textfield.len = text_len;
}


void lltui_widget_info(lltui_ctx* ctx, int32_t descriptor) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");
    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);

    switch (widget->widget_type)
    {
        case lltui_textfield: 
            LLTUI_PRINTF("TYPE: TEXTFIELD \n"); 
            LLTUI_PRINTF("\tVALUE: %s \n", (char*)lltui_arena_get_ref(&ctx->widget_arena, widget->type.textfield.desc));

            break;
        case lltui_line: LLTUI_PRINTF("TYPE: LINE"); break;

    
        default: break;
    }

    LLTUI_PRINTF("\tPOS: (%d, %d) \n", widget->start_pos.x, widget->start_pos.y);
    LLTUI_PRINTF("\tVISIBILITY: %s \n", (widget->visability == lltui_visible) ? "visible" : "shadowed");
}

uint32_t max(uint32_t a, uint32_t b){
    return (a > b)? a : b;
}

void lltui_widget_print(lltui_ctx* ctx, int32_t descriptor) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");
    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);

    lltui_cursor_move(ctx, widget->start_pos);

    lltui_cursor_color(ctx, widget->color);

    switch (widget->widget_type)
    {
        case lltui_textfield: 
            char* c = (char*)lltui_arena_get_ref(&ctx->widget_arena, widget->type.textfield.desc);
            int32_t diff = widget->type.textfield.last_len - widget->type.textfield.len;
            if (widget->visability == lltui_shadowd || diff > 0) {
                ctx->cb.tx_cb(NULL, max(widget->type.textfield.len, widget->type.textfield.last_len));
                lltui_cursor_move(ctx, widget->start_pos);
            }
            if (widget->visability == lltui_visible) {
                ctx->cb.tx_cb(c, strlen(c));
            }
            break;

        case lltui_line: LLTUI_PRINTF("TYPE: LINE"); break;

    
        default: break;
    }
}

void lltui_widget_show(lltui_ctx* ctx, int32_t descriptor) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");
    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);

    widget->visability = lltui_visible;
}

void lltui_widget_color_foreground(lltui_ctx* ctx, int32_t descriptor, uint8_t color) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");
    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);

    widget->color.foreground = color;
}

void lltui_widget_color_background(lltui_ctx* ctx, int32_t descriptor, uint8_t color) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");
    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);

    widget->color.background = color + 10;
}

void lltui_widget_shadow(lltui_ctx* ctx, int32_t descriptor) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");
    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);

    widget->visability = lltui_shadowd;
}