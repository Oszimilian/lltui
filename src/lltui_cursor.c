#include "lltui_cursor.h"

#include "lltui_assert.h"
#include "lltui_makros.h"
#include "lltui_pos.h"
#include "lltui_string.h"

static char str[128] = {0};


void lltui_cursor_move(lltui_ctx* ctx, lltui_pos pos) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");

    lltui_string tmp;
    lltui_string_init(&tmp, &str[0], ARRAY_SIZE(str));

    tmp = lltui_string_attach_str(tmp, "\e[");
    tmp = lltui_string_attach_number(tmp, pos.y);
    tmp = lltui_string_attach_char(tmp, ';');
    tmp = lltui_string_attach_number(tmp, pos.x);
    tmp = lltui_string_attach_char(tmp, 'H');

    LLTUI_ASSERT(tmp.error, "lltui_cursor_move buffer overflow");

    ctx->cb.tx_cb(tmp.data, tmp.len);
}

void lltui_cursor_clear_window(lltui_ctx* ctx) {
    char cmd[] = "\e[2J\e[H";
    ctx->cb.tx_cb(cmd, ARRAY_SIZE(cmd) - 1);
}

void lltui_cursor_color(lltui_ctx* ctx, lltui_color color) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");

    lltui_string tmp;
    lltui_string_init(&tmp, &str[0], ARRAY_SIZE(str));

    //foreground
    tmp = lltui_string_attach_str(tmp, "\e[");
    tmp = lltui_string_attach_number(tmp, color.foreground);
    tmp = lltui_string_attach_char(tmp, 'm');

    //background
    tmp = lltui_string_attach_str(tmp, "\e[");
    tmp = lltui_string_attach_number(tmp, color.background);
    tmp = lltui_string_attach_char(tmp, 'm');

    LLTUI_ASSERT(tmp.error, "lltui_cursor_color buffer overflow");

    ctx->cb.tx_cb(tmp.data, tmp.len);
}



void lltui_cursor_clear_line(lltui_ctx* ctx, lltui_pos start_pos, lltui_pos end_pos) {
    do{
        lltui_cursor_move(ctx, start_pos);
        ctx->cb.tx_cb(" ", 1);
    }while(lltui_pos_move_together(&start_pos, &end_pos) == false);
}

void lltui_cursor_draw_line(lltui_ctx* ctx, lltui_pos start_pos, lltui_pos end_pos) {
    uint32_t diff_x = lltui_pos_abs_diff_x(start_pos, end_pos);

    do{
        lltui_cursor_move(ctx, start_pos);

        if (diff_x > 0) {
            ctx->cb.tx_cb("\xE2\x94\x80", 3);
        } else {
            ctx->cb.tx_cb("\xE2\x94\x82", 3);
        }
    } while(lltui_pos_move_together(&start_pos, &end_pos) == false);
    
}

void lltui_cursor_draw_corner(lltui_ctx* ctx, lltui_corner_type type) {
    lltui_string tmp;
    lltui_string_init(&tmp, &str[0], ARRAY_SIZE(str));

    switch(type) {
        case lltui_up_left:     tmp = lltui_string_attach_str(tmp, "\xE2\x94\x8C"); break;
        case lltui_up_right:    tmp = lltui_string_attach_str(tmp, "\xE2\x94\x90"); break;
        case lltui_down_left:   tmp = lltui_string_attach_str(tmp, "\xE2\x94\x94"); break;
        case lltui_down_right:  tmp = lltui_string_attach_str(tmp, "\xE2\x94\x98"); break;
        default: break;
    }

    ctx->cb.tx_cb(tmp.data, tmp.len);
}
