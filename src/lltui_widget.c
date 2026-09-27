#include "lltui_widget.h"
#include "lltui_arena.h"
#include "lltui_assert.h"
#include "lltui_print.h"
#include "lltui_cursor.h"
#include "lltui_string.h"
#include <string.h>

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

        case lltui_intfield:
            widget->type.integer.desc = -1;
            widget->updated = false;
            break;

        case lltui_floatfield:
            widget->type.floatingpoint.desc = -1;
            widget->updated = false;
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

static lltui_string lltui_widget_get_string(lltui_ctx* ctx, lltui_widget* widget, int32_t* desc) {
    uint32_t max_len = lltui_pos_abs_diff_x(widget->start_pos, widget->end_pos);

    if (*desc == -1) {
        *desc = lltui_arena_malloc(&ctx->widget_arena, max_len + 1);
    }

    lltui_string str;
    lltui_string_init(&str, (char*)lltui_arena_get_ref(&ctx->widget_arena, *desc), max_len + 1);

    return str;
}

void lltui_widget_set_text(lltui_ctx* ctx, int32_t descriptor, char* text) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");

    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);
    LLTUI_ASSERT(widget->widget_type != lltui_textfield, "descriptor is not a textfield widget");

    lltui_string str = lltui_widget_get_string(ctx, widget, &widget->type.textfield.desc);

    // cut the text if it is wider than the field
    for (; *text != '\0'; text++) {
        str = lltui_string_attach_char(str, *text);
        if (str.error) break;
    }

    widget->updated = true;
}

void lltui_widget_set_integer(lltui_ctx* ctx, int32_t descriptor, int32_t value) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");

    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);
    LLTUI_ASSERT(widget->widget_type != lltui_intfield, "descriptor is not a lltui_intfield widget");

    lltui_string str = lltui_widget_get_string(ctx, widget, &widget->type.integer.desc);

    str = lltui_string_attach_int(str, value);
    if (str.error) {
        str = lltui_string_attach_char(str, '#');
    }

    widget->updated = true;
}

void lltui_widget_set_float(lltui_ctx* ctx, int32_t descriptor, float value) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");

    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);
    LLTUI_ASSERT(widget->widget_type != lltui_floatfield, "descriptor is not a lltui_floatfield widget");

    lltui_string str = lltui_widget_get_string(ctx, widget, &widget->type.floatingpoint.desc);

    str = lltui_string_attach_float(str, value, LLTUI_WIDGET_FLOAT_PRECISION);
    if (str.error) {
        str = lltui_string_attach_char(str, '#');
    }

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

    if (widget->visability == lltui_visible) {
        lltui_cursor_color(ctx, widget->color);
    } else {
        lltui_cursor_color(ctx, ctx->color);
    }
    

    switch (widget->widget_type)
    {
        case lltui_floatfield:
        case lltui_intfield:
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
            if (widget->visability == lltui_shadowd) break;
            lltui_cursor_draw_line(ctx, widget->start_pos, widget->end_pos);
            break;

        case lltui_corner:
            lltui_cursor_move(ctx, widget->start_pos);
            
            if (widget->visability == lltui_shadowd) break;
            lltui_cursor_draw_corner(ctx, widget->type.corner.corner_type);
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



void lltui_widget_color_foreground(lltui_ctx* ctx, int32_t descriptor, uint8_t color) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");
    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);

    widget->color.foreground = color;

    if (widget->widget_type == lltui_box) {
        for (uint8_t i = 0; i < 4; i++) {
            lltui_widget_color_foreground(ctx, widget->type.box.corners[i], widget->color.foreground);
            lltui_widget_color_foreground(ctx, widget->type.box.lines[i], widget->color.foreground);            
        }
    }

    widget->updated = true;
}

void lltui_widget_color_background(lltui_ctx* ctx, int32_t descriptor, uint8_t color) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");
    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);

    widget->color.background = color + 10;

    if (widget->widget_type == lltui_box) {
        for (uint8_t i = 0; i < 4; i++) {
            lltui_widget_color_background(ctx, widget->type.box.corners[i], widget->color.background);
            lltui_widget_color_background(ctx, widget->type.box.lines[i], widget->color.background);            
        }
    }


    widget->updated = true;
}

void lltui_widget_show(lltui_ctx* ctx, int32_t descriptor) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");
    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);

    if (widget->widget_type == lltui_box) {
        for (uint8_t i = 0; i < 4; i++) {
            lltui_widget_show(ctx, widget->type.box.corners[i]);
            lltui_widget_show(ctx, widget->type.box.lines[i]);            
        }
    }

    widget->visability = lltui_visible;
    widget->updated = true;
}

void lltui_widget_shadow(lltui_ctx* ctx, int32_t descriptor) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");
    lltui_widget* widget = (lltui_widget*)lltui_arena_get_ref(&ctx->widget_arena, descriptor);

    if (widget->widget_type == lltui_box) {
        for (uint8_t i = 0; i < 4; i++) {
            lltui_widget_shadow(ctx, widget->type.box.corners[i]);
            lltui_widget_shadow(ctx, widget->type.box.lines[i]);            
        }
    }

    widget->visability = lltui_shadowd;
    widget->updated = true;
}

