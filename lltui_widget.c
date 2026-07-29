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
        case lltui_textfield: 
            widget->type.textfield.desc = -1;
            widget->updated = false;
            break;

        case lltui_line: 
            widget->type.textfield.desc = -1;
            widget->updated = true;
            break;

        case lltui_corner:
            widget->updated = true;
            break;

        case lltui_box:
            // corners
            lltui_pos corner_pos = lltui_pos_get_corner(start_pos, end_pos, lltui_up_left);
            widget->type.box.corners[0] = lltui_widget_create(ctx, corner_pos, corner_pos, lltui_corner);
            lltui_widget_set_corner_type(ctx, widget->type.box.corners[0], lltui_up_left);

            corner_pos = lltui_pos_get_corner(start_pos, end_pos, lltui_up_right);
            widget->type.box.corners[1] = lltui_widget_create(ctx, corner_pos, corner_pos, lltui_corner);
            lltui_widget_set_corner_type(ctx, widget->type.box.corners[1], lltui_up_right);

            corner_pos = lltui_pos_get_corner(start_pos, end_pos, lltui_down_left);
            widget->type.box.corners[2] = lltui_widget_create(ctx, corner_pos, corner_pos, lltui_corner);
            lltui_widget_set_corner_type(ctx, widget->type.box.corners[2], lltui_down_left);

            corner_pos = lltui_pos_get_corner(start_pos, end_pos, lltui_down_right);
            widget->type.box.corners[3] = lltui_widget_create(ctx, corner_pos, corner_pos, lltui_corner);
            lltui_widget_set_corner_type(ctx, widget->type.box.corners[3], lltui_down_right);

            // edges
            corner_pos = lltui_pos_get_corner(start_pos, end_pos, lltui_up_right);
            widget->type.box.lines[0] = lltui_widget_create(ctx, start_pos, corner_pos, lltui_line);

            widget->type.box.lines[1] = lltui_widget_create(ctx, corner_pos, end_pos, lltui_line);

            corner_pos = lltui_pos_get_corner(start_pos, end_pos, lltui_down_left);
            widget->type.box.lines[2] = lltui_widget_create(ctx, corner_pos, end_pos, lltui_line);

            widget->type.box.lines[3] = lltui_widget_create(ctx, start_pos, corner_pos, lltui_line);

            widget->updated = true;
            break;

        default: break;
    }

    widget->widget_type = type;

    lltui_widget_color_background(ctx, descriptor, LLTUI_BLACK);
    lltui_widget_color_foreground(ctx, descriptor, LLTUI_WHITE);

    

    return descriptor;
}

void lltui_widget_set_corner_type(lltui_ctx* ctx, int32_t descriptor, lltui_corner_type type) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");

    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);
    LLTUI_ASSERT(widget->widget_type != lltui_corner, "descriptor is not a corner widget");

    widget->type.corner.corner_type = type;

    widget->updated = true;
}

void lltui_widget_set_pos(lltui_ctx* ctx, int32_t descriptor, lltui_pos start_pos, lltui_pos end_pos) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");

    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);
    LLTUI_ASSERT(widget->widget_type != lltui_textfield, "descriptor is not a textfield widget");

    widget->start_pos = start_pos;
    widget->end_pos = end_pos;

    lltui_pos tmp_lowest = lltui_pos_get_lowest(start_pos, end_pos);
    ctx->lowest_pos = lltui_pos_get_lowest(ctx->lowest_pos, tmp_lowest);
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

    lltui_cursor_color(ctx, widget->color);

    switch (widget->widget_type)
    {
        case lltui_textfield:
            lltui_cursor_clear_line(ctx, widget->start_pos, widget->end_pos);
            lltui_cursor_move(ctx, widget->start_pos);
            char* c = (char*)lltui_arena_get_ref(&ctx->widget_arena, widget->type.textfield.desc);
            if (widget->visability == lltui_visible) {
                ctx->cb.tx_cb(c, strlen(c));
            }
            break;

        case lltui_line:
            lltui_cursor_clear_line(ctx, widget->start_pos, widget->end_pos);
            lltui_cursor_move(ctx, widget->start_pos);
            lltui_cursor_draw_line(ctx, widget->start_pos, widget->end_pos);
            break;

        case lltui_corner:
            lltui_cursor_move(ctx, widget->start_pos);
            lltui_cursor_draw_corner(ctx, widget->start_pos, widget->type.corner.corner_type);
            break;

        case lltui_box:
            for (uint8_t i = 0; i < 4; i++) {
                lltui_widget_print(ctx, widget->type.box.lines[i]);
            }

            for (uint8_t i = 0; i < 4; i++) {
                lltui_widget_print(ctx, widget->type.box.corners[i]);
            }
            break;

    
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