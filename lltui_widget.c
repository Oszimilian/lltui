#include "lltui_widget.h"
#include "lltui_arena.h"
#include "lltui_assert.h"
#include "lltui_print.h"
#include "lltui_cursor.h"
#include "string.h"

int32_t lltui_widget_create(lltui_ctx* ctx, lltui_pos start_pos, lltui_pos end_pos, lltui_widget_type type) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");

    int32_t descriptor = lltui_arena_malloc(&ctx->widget_arena, sizeof(lltui_widget));
    lltui_widget_show(ctx, descriptor);

    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);
    lltui_widget_set_pos(ctx, descriptor, start_pos, end_pos);

    switch (type) {
        case lltui_textfield: widget->type.textfield.desc = -1; break;
        default: break;
    }

    lltui_widget_color_background(ctx, descriptor, LLTUI_BLACK);
    lltui_widget_color_foreground(ctx, descriptor, LLTUI_WHITE);

    widget->updated = false;

    return descriptor;
}

void lltui_widget_set_pos(lltui_ctx* ctx, int32_t descriptor, lltui_pos start_pos, lltui_pos end_pos) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");

    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);
    LLTUI_ASSERT(widget->widget_type != lltui_textfield, "descriptor is not a textfield widget");

    widget->start_pos = start_pos;
    widget->end_pos = end_pos;

    ctx->lowest_pos = lltui_pos_get_lowest(ctx->lowest_pos, start_pos);
}

void lltui_widget_set_text(lltui_ctx* ctx, int32_t descriptor, char* str) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");

    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);
    LLTUI_ASSERT(widget->widget_type != lltui_textfield, "descriptor is not a textfield widget");

    uint32_t max_len = lltui_pos_abs_diff_x(widget->start_pos, widget->end_pos);
    uint32_t text_len = strlen(str);
    if (text_len > max_len) {
        text_len = max_len;
    }

    if (widget->type.textfield.desc == -1) {
        widget->type.textfield.desc =  lltui_arena_malloc(&ctx->widget_arena, max_len + 1);
    }

    char* data = (char*)lltui_arena_get_ref(&ctx->widget_arena, widget->type.textfield.desc);
    strncpy(data, str, max_len);

    widget->updated = true;
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

    LLTUI_PRINTF("\tSTART_POS: (%d, %d) \n", widget->start_pos.x, widget->start_pos.y);
    LLTUI_PRINTF("\tEND_POS: (%d, %d) \n", widget->end_pos.x, widget->end_pos.y);
    LLTUI_PRINTF("\tVISIBILITY: %s \n", (widget->visability == lltui_visible) ? "visible" : "shadowed");
}

void lltui_widget_print(lltui_ctx* ctx, int32_t descriptor) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");
    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);

    if(widget->updated == false) return;

    lltui_cursor_move(ctx, widget->start_pos);

    lltui_cursor_color(ctx, widget->color);

    switch (widget->widget_type)
    {
        case lltui_textfield:
            //lltui_cursor_clear_line(ctx, widget->start_pos, widget->end_pos);
            char* c = (char*)lltui_arena_get_ref(&ctx->widget_arena, widget->type.textfield.desc);
            if (widget->visability == lltui_visible) {
                ctx->cb.tx_cb(c, strlen(c));
            }
            break;

        case lltui_line: LLTUI_PRINTF("TYPE: LINE"); break;

    
        default: break;
    }

    widget->updated = false;
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